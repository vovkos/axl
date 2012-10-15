#include "stdafx.h"
#include "axl_jnc_EnumType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CEnumMember*
CEnumType::FindMember (
	const tchar_t* pName,
	bool Traverse
	)
{
	rtl::CHashTableMapIteratorT <const tchar_t*, CModuleItem*> It = m_ItemMap.Find (pName); 
	if (It)
		return (CEnumMember*) It->m_Value;

	if (!Traverse)
		return NULL;

	//size_t Count = m_BaseTypeArray.GetCount ();
	//for (size_t i = 0; i < Count; i++)
	//{
	//	CEnumType* pBaseType = m_BaseTypeArray [i];
	//	CEnumMember* pMember = pBaseType->FindMember (pName, true);
	//	if (pMember)
	//		return pMember;
	//}

	return NULL;
}

CEnumMember*
CEnumType::CreateMember (
	const rtl::CString& Name,
	intptr_t Value
	)
{
	CEnumMember* pMember = AXL_MEM_NEW (CEnumMember);
	pMember->m_Name = Name;
	pMember->m_Value = Value;
	m_MemberList.InsertTail (pMember);
	
	m_CurrentValue = Value + 1;

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
