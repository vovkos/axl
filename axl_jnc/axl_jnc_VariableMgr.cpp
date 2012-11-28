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

	m_TempVariableCounter = 0;
}

void
CVariableMgr::Clear ()
{
	m_VariableList.Clear ();
	m_TempVariableCounter = 0;
}

CVariable*
CVariableMgr::CreateVariable (
	EVariable VariableKind,
	const rtl::CString& Name,
	CType* pType
	)
{
	CVariable* pVariable = AXL_MEM_NEW (CVariable);
	pVariable->m_pModule = m_pModule;
	pVariable->m_VariableKind = VariableKind;
	pVariable->m_Name = Name;
	pVariable->m_pType = pType;
	pVariable->m_pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	m_VariableList.InsertTail (pVariable);

	if (m_pModule->m_FunctionMgr.GetCurrentFunction ())
	{
		ASSERT (VariableKind == EVariable_Local || VariableKind == EVariable_Arg);
		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateAlloca (pType, Name, NULL, &PtrValue);
		m_pModule->m_LlvmBuilder.CreateStore (pType->GetZeroValue (), PtrValue);

		pVariable->m_pLlvmValue = PtrValue.GetLlvmValue ();
	}
	else
	{
		ASSERT (VariableKind == EVariable_Global);

		pVariable->m_pLlvmValue = new llvm::GlobalVariable (
			*m_pModule->m_pLlvmModule,
			pType->GetLlvmType (),
			false,
			llvm::GlobalVariable::ExternalLinkage,
			(llvm::Constant*) pType->GetLlvmZeroValue (),
			(const tchar_t*) Name
			);
	}

	return pVariable;
}

CVariable*
CVariableMgr::CreateVariable (
	const rtl::CString& Name,
	CType* pType
	)
{
	EVariable VariableKind = m_pModule->m_FunctionMgr.GetCurrentFunction () ? EVariable_Local : EVariable_Global;
	return CreateVariable (VariableKind, Name, pType);
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
