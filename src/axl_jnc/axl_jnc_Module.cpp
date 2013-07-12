#include "pch.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

void
CModule::Clear ()
{
	m_FilePath.Clear ();
	m_TypeMgr.Clear ();
	m_NamespaceMgr.Clear ();
	m_FunctionMgr.Clear ();
	m_VariableMgr.Clear ();
	m_ConstMgr.Clear ();
	m_ControlFlowMgr.Clear ();
	m_CalcLayoutArray.Clear ();
	m_CompileArray.Clear ();

	m_pConstructor = NULL;
	m_pDestructor = NULL;
	m_pLlvmModule = NULL;
}

bool
CModule::Create (
	const rtl::CString& FilePath,
	llvm::Module* pLlvmModule
	)
{
	Clear ();
	m_FilePath = FilePath;
	m_pLlvmModule = pLlvmModule;
	return true;
}

bool
CModule::SetConstructor (CFunction* pFunction)
{
	if (!pFunction->GetType ()->GetArgArray ().IsEmpty ())
	{
		err::SetFormatStringError ("module 'this' cannot have arguments");
		return false;
	}

	if (m_pConstructor)
	{
		err::SetFormatStringError ("module already has 'this' method");
		return false;
	}

	pFunction->m_FunctionKind = EFunction_ModuleConstructor;
	pFunction->m_StorageKind = EStorage_Static;
	pFunction->m_Tag = "module.construct";
	m_pConstructor = pFunction;
	return true;
}

bool
CModule::SetDestructor (CFunction* pFunction)
{
	if (m_pDestructor)
	{
		err::SetFormatStringError ("module already has 'destruct' method");
		return false;
	}

	pFunction->m_FunctionKind = EFunction_ModuleDestructor;
	pFunction->m_StorageKind = EStorage_Static;
	pFunction->m_Tag = "module.destruct";
	m_pDestructor = pFunction;
	return true;
}

bool
CModule::SetFunctionPointer (
	llvm::ExecutionEngine* pLlvmExecutionEngine,
	const char* pName,
	void* pf
	)
{
	CModuleItem* pItem = m_NamespaceMgr.GetGlobalNamespace ()->FindItem (pName);
	if (!pItem || pItem->GetItemKind () != EModuleItem_Function)
		return false;

	llvm::Function* pLlvmFunction = ((CFunction*) pItem)->GetLlvmFunction ();
	if (!pLlvmFunction)
		return false;

	pLlvmExecutionEngine->addGlobalMapping (pLlvmFunction, pf);
	return true;
}

bool
CModule::Link (CModule* pModule)
{
	err::SetFormatStringError ("module link is not yet implemented");
	return false;
}

void 
CModule::MarkForLayout (CModuleItem* pItem)
{
	if (pItem->m_Flags & EModuleItemFlag_NeedLayout)
		return;

	pItem->m_Flags |= EModuleItemFlag_NeedLayout;
	m_CalcLayoutArray.Append (pItem);
}

void 
CModule::MarkForCompile (CModuleItem* pItem)
{
	if (pItem->m_Flags & EModuleItemFlag_NeedCompile)
		return;

	pItem->m_Flags |= EModuleItemFlag_NeedCompile;
	m_CompileArray.Append (pItem);
}

bool
CModule::Compile ()
{
	bool Result;

	// step 1: resolve import types

	Result = m_TypeMgr.ResolveImportTypes ();
	if (!Result)
		return false;

	// step 2: calc layouts

	for (size_t i = 0; i < m_CalcLayoutArray.GetCount (); i++) // new items could be added in process
	{
		Result = m_CalcLayoutArray [i]->EnsureLayout ();
		if (!Result)
			return false;
	}

	// step 3: compile module constructor 

	if (m_pConstructor)
	{
		if (!m_pConstructor->HasBody ())
		{
			err::SetFormatStringError ("unresolved module constructor");
			return false;
		}

		Result = m_pConstructor->Compile ();
		if (!Result)
			return false;
	}
	else
	{
		Result = CreateDefaultConstructor ();
		if (!Result)
			return false;
	}

	// step 4: compile the rest 

	for (size_t i = 0; i < m_CompileArray.GetCount (); i++) // new items could be added in process
	{
		Result = m_CompileArray [i]->Compile ();
		if (!Result)
			return false;
	}

	// step 5: ensure module destructor (if needed)

	if (!m_pDestructor && 
		(!m_VariableMgr.GetGlobalStaticDestructArray ().IsEmpty () ||
		 !m_VariableMgr.GetLazyStaticDestructList ().IsEmpty ()))
	{
		CreateDefaultDestructor ();
	}

	// step 6: deal with tls 

	Result = 
		m_VariableMgr.CreateTlsStructType () &&
		m_FunctionMgr.InjectTlsPrologues ();

	if (!Result)
		return false;

	return true;
}

bool
CModule::CreateDefaultConstructor ()
{
	bool Result;

	ASSERT (!m_pConstructor);

	CFunctionType* pType = (CFunctionType*) GetSimpleType (EStdType_SimpleFunction);
	CFunction* pFunction = m_FunctionMgr.CreateFunction (EFunction_ModuleConstructor, pType);
	pFunction->m_StorageKind = EStorage_Static;
	pFunction->m_Tag = "module.construct";

	m_pConstructor = pFunction;

	m_FunctionMgr.InternalPrologue (pFunction);

	CBasicBlock* pBlock = m_ControlFlowMgr.GetCurrentBlock ();
	m_ControlFlowMgr.SetCurrentBlock (pFunction->GetEntryBlock ());

	Result = m_VariableMgr.AllocatePrimeStaticVariables ();
	if (!Result)
		return false;

	m_ControlFlowMgr.SetCurrentBlock (pBlock);

	Result = m_VariableMgr.InitializeGlobalStaticVariables ();
	if (!Result)
		return false;

	m_FunctionMgr.InternalEpilogue ();

	return true;
}

void
CModule::CreateDefaultDestructor ()
{
	ASSERT (!m_pDestructor);

	CFunctionType* pType = (CFunctionType*) GetSimpleType (EStdType_SimpleFunction);
	CFunction* pFunction = m_FunctionMgr.CreateFunction (EFunction_ModuleDestructor, pType);
	pFunction->m_StorageKind = EStorage_Static;
	pFunction->m_Tag = "module.destruct";

	m_pDestructor = pFunction;

	m_FunctionMgr.InternalPrologue (pFunction);

	m_OperatorMgr.ProcessDestructArray (m_VariableMgr.GetGlobalStaticDestructArray ());
	m_OperatorMgr.ProcessLazyStaticDestructList (m_VariableMgr.GetLazyStaticDestructList ());

	m_FunctionMgr.InternalEpilogue ();
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
