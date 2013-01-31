#include "stdafx.h"
#include "axl_jnc_VariableMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CVariableMgr::CVariableMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);
	m_pScopeLevelVariable = NULL;
}

void
CVariableMgr::Clear ()
{
	m_LocalVariableList.Clear ();
	m_GlobalVariableList.Clear ();
	m_pScopeLevelVariable = NULL;
}

CVariable*
CVariableMgr::CreateVariable (
	EVariable VariableKind,
	const rtl::CString& Name,
	CType* pType,
	bool HasInitializer
	)
{
	CVariable* pVariable = AXL_MEM_NEW (CVariable);
	pVariable->m_pModule = m_pModule;
	pVariable->m_VariableKind = VariableKind;
	pVariable->m_Name = Name;
	pVariable->m_QualifiedName = Name;
	pVariable->m_Tag = Name;
	pVariable->m_pType = pType;

	if (VariableKind == EVariable_Local)
	{
		ASSERT (m_pModule->m_FunctionMgr.GetCurrentFunction ());
		
		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateAlloca (pType, Name, NULL, &PtrValue);

		if (!HasInitializer)
			m_pModule->m_LlvmBuilder.CreateStore (pType->GetZeroValue (), PtrValue);

		pVariable->m_pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
		pVariable->m_pLlvmValue = PtrValue.GetLlvmValue ();
		m_LocalVariableList.InsertTail (pVariable);
	}
	else
	{
		ASSERT (VariableKind == EVariable_Global);
		m_GlobalVariableList.InsertTail (pVariable);
	}

	return pVariable;
}

CVariable*
CVariableMgr::CreateVariable (
	const rtl::CString& Name,
	CType* pType,
	bool HasInitializer
	)
{
	EVariable VariableKind = m_pModule->m_FunctionMgr.GetCurrentFunction () ? EVariable_Local : EVariable_Global;
	return CreateVariable (VariableKind, Name, pType, HasInitializer);
}

bool
CVariableMgr::AllocateGlobalVariables ()
{
	rtl::CIteratorT <CVariable> Variable = m_GlobalVariableList.GetHead ();
	for (; Variable; Variable++)
	{
		CVariable* pVariable = *Variable;

		if (!pVariable->m_pLlvmValue)
			pVariable->m_pLlvmValue = new llvm::GlobalVariable (
				*m_pModule->m_pLlvmModule,
				pVariable->m_pType->GetLlvmType (),
				false,
				llvm::GlobalVariable::ExternalLinkage,
				(llvm::Constant*) pVariable->m_pType->GetZeroValue ().GetLlvmValue (),
				(const tchar_t*) pVariable->GetQualifiedName ()
				);
	}

	return true;
}

CVariable*
CVariableMgr::GetScopeLevelVariable ()
{
	if (m_pScopeLevelVariable)
		return m_pScopeLevelVariable;

	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	m_pScopeLevelVariable = CreateVariable (
		EVariable_Global,
		_T("ScopeLevel"),
		pType,
		true
		);

	m_pScopeLevelVariable->m_pLlvmValue = new llvm::GlobalVariable (
		*m_pModule->m_pLlvmModule,
		pType->GetLlvmType (),
		false,
		llvm::GlobalVariable::ExternalLinkage,
		(llvm::Constant*) pType->GetZeroValue ().GetLlvmValue (),
		_T("jnc.ScopeLevel"),
		NULL,
		false // true // thread local -- currently JIT produces an error
		);
	
	return m_pScopeLevelVariable;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
