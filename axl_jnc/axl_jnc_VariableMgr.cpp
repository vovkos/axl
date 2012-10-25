#include "stdafx.h"
#include "axl_jnc_VariableMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CVariableMgr::CVariableMgr (CModule* pModule)
{
	m_pModule = pModule;
}

void
CVariableMgr::Clear ()
{
	m_VariableList.Clear ();
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
		pVariable->m_pLlvmAlloca = m_pModule->m_ControlFlowMgr.GetLlvmBuilder ()->CreateAlloca (pLlvmType);
	}

	return pVariable;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
