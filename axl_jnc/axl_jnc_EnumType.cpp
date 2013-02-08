#include "stdafx.h"
#include "axl_jnc_EnumType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CEnumType::CEnumType ()
{
	m_TypeKind = EType_Enum;
	m_Flags = ETypeFlag_Pod;
	m_CurrentValue = 0;
}

CEnumMember*
CEnumType::FindMember (const tchar_t* pName)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName); 
	if (!It)
		return NULL;

	CModuleItem* pItem = It->m_Value;
	return pItem->GetItemKind () == EModuleItem_EnumMember ? (CEnumMember*) pItem : NULL;
}

CEnumMember*
CEnumType::CreateMember (
	const rtl::CString& Name,
	intptr_t Value
	)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Goto (Name); 
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("redefinition of member '%s' in '%s'"), Name, GetTypeString ());
		return NULL;
	}

	CEnumMember* pMember = AXL_MEM_NEW (CEnumMember);
	pMember->m_Name = Name;
	pMember->m_pParentEnumType = this;
	pMember->m_Value = Value;
	m_MemberList.InsertTail (pMember);
	
	m_CurrentValue = Value + 1;
	It->m_Value = pMember;
	return pMember;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {