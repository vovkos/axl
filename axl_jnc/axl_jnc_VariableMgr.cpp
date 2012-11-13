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
	const rtl::CString& Name,
	CType* pType
	)
{
	CVariable* pVariable = AXL_MEM_NEW (CVariable);
	pVariable->m_pModule = m_pModule;
	pVariable->m_Name = Name;
	pVariable->m_pType = pType;
	m_VariableList.InsertTail (pVariable);

	if (m_pModule->m_FunctionMgr.GetCurrentFunction ())
	{
		llvm::Type* pLlvmType = pType->GetLlvmType ();
		pVariable->m_VariableKind = EVariable_Local;
		pVariable->m_pLlvmAlloca = m_pModule->m_LlvmBuilder.CreateAlloca (
			pLlvmType, 
			0, 
			(const tchar_t*) Name
			);
	}

	return pVariable;
}

CVariable*
CVariableMgr::CreateTempVariable (CType* pType)
{
	ASSERT (m_pModule->m_FunctionMgr.GetCurrentFunction ());

	rtl::CString Name;
	Name.Format (_T("_tmp%d"), m_TempVariableCounter++);
	return CreateVariable (Name, pType);
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
