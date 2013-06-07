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

	m_pConstructor = NULL;
	m_pDestructor = NULL;
	m_pLlvmModule = NULL;
}

bool
CModule::Create (const rtl::CString& FilePath)
{
	Clear ();
	m_FilePath = FilePath;
	m_pLlvmModule = (llvm::Module*) AXL_MEM_ALLOC (sizeof (llvm::Module));
	new (m_pLlvmModule) llvm::Module ((const char*) m_FilePath, llvm::getGlobalContext());
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
	pFunction->m_Tag = "module.this";
	m_pConstructor = pFunction;
	return true;
}

bool
CModule::SetDestructor (CFunction* pFunction)
{
	if (m_pDestructor)
	{
		err::SetFormatStringError ("module already has '~this' method");
		return false;
	}

	pFunction->m_FunctionKind = EFunction_ModuleDestructor;
	pFunction->m_StorageKind = EStorage_Static;
	pFunction->m_Tag = "module.~this";
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

bool
CModule::Compile ()
{
	bool Result = 
		m_TypeMgr.ResolveImportTypes () &&
		m_FunctionMgr.ResolveOrphanFunctions () &&
		m_FunctionMgr.ScanAutoEvs () &&
		m_FunctionMgr.CalcPropertyLayouts () &&
		m_FunctionMgr.CalcAutoEvLayouts () &&
		m_TypeMgr.CalcTypeLayouts () &&
		m_VariableMgr.AllocateGlobalVariables () &&
		m_FunctionMgr.CompileFunctions ();

	if (!Result)
		return false;

	if (!m_pConstructor && !m_VariableMgr.GetGlobalVariableArray ().IsEmpty ())
	{
		Result = CreateDefaultConstructor ();
		if (!Result)
			return false;
	}

	if (!m_pDestructor && 
		(!m_TypeMgr.GetStaticDestructArray ().IsEmpty () || 
		 m_VariableMgr.GetStaticDestructArray ().IsEmpty ()))
	{
		Result = CreateDefaultDestructor ();
		if (!Result)
			return false;
	}
	
	return true;	
}

bool
CModule::CreateDefaultConstructor ()
{
	bool Result;

	ASSERT (!m_pConstructor);

	CFunctionType* pType = (CFunctionType*) (CType*) CGetType (this, EStdType_SimpleFunction);
	CFunction* pFunction = m_FunctionMgr.CreateFunction (EFunction_ModuleConstructor, pType);
	pFunction->m_StorageKind = EStorage_Static;
	pFunction->m_Tag = "module.this";

	m_FunctionMgr.InternalPrologue (pFunction);

	Result = m_VariableMgr.InitializeGlobalVariables ();
	if (!Result)
		return false;

	m_FunctionMgr.InternalEpilogue ();

	m_pConstructor = pFunction;
	return true;
}

bool
CModule::CreateDefaultDestructor ()
{
	bool Result;

	ASSERT (!m_pDestructor);

	CFunctionType* pType = (CFunctionType*) (CType*) CGetType (this, EStdType_SimpleFunction);
	CFunction* pFunction = m_FunctionMgr.CreateFunction (EFunction_ModuleDestructor, pType);
	pFunction->m_StorageKind = EStorage_Static;
	pFunction->m_Tag = "module.~this";
		
	m_FunctionMgr.InternalPrologue (pFunction);

	rtl::CArrayT <CVariable*> StaticVariableDestructArray = m_VariableMgr.GetStaticDestructArray ();
	size_t Count = StaticVariableDestructArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CVariable* pVariable = StaticVariableDestructArray [i];
		ASSERT (pVariable->GetType ()->GetTypeKind () == EType_Class);

		CClassType* pClassType = (CClassType*) pVariable->GetType ();
		CFunction* pDestructor = pClassType->GetDestructor ();
		ASSERT (pDestructor);

		CValue ThisValue (pVariable->GetLlvmValue (), pClassType->GetClassPtrType ());
		Result = m_OperatorMgr.CallOperator (pDestructor, ThisValue);
		if (!Result)
			return false;
	}

	rtl::CArrayT <CClassType*> StaticTypeDestructArray = m_TypeMgr.GetStaticDestructArray ();
	Count = StaticTypeDestructArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CClassType* pClassType = StaticTypeDestructArray [i];
		CFunction* pDestructor = pClassType->GetStaticDestructor ();
		ASSERT (pDestructor);

		Result = m_OperatorMgr.CallOperator (pDestructor);
		if (!Result)
			return false;
	}

	m_FunctionMgr.InternalEpilogue ();

	m_pDestructor = pFunction;
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
