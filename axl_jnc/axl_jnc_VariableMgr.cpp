#include "stdafx.h"
#include "axl_jnc_VariableMgr.h"

namespace axl {
namespace jnc {

//.............................................................................

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
	pVariable->m_Name = Name;
	pVariable->m_pType = pType;
	m_VariableList.InsertTail (pVariable);
	return pVariable;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
