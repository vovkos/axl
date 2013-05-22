#include "pch.h"
#include "axl_pg_DefineMgr.h"

namespace axl {
namespace pg {

//.............................................................................
	
CDefine*
CDefineMgr::GetDefine (const rtl::CString& Name)
{
	rtl::CStringHashTableMapIteratorT <CDefine*> It = m_DefineMap.Goto (Name);
	if (It->m_Value)
		return It->m_Value;

	CDefine* pDefine = AXL_MEM_NEW (CDefine);
	pDefine->m_Name = Name;
	m_DefineList.InsertTail (pDefine);
	It->m_Value = pDefine;
	return pDefine;
}

//.............................................................................

} // namespace pg {
} // namespace axl {
