#include "stdafx.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CStructMember*
CStructType::FindMember (
	const tchar_t* pName,
	bool Traverse
	)
{
	rtl::CHashTableMapIteratorT <const tchar_t*, CModuleItem*> It = m_ItemMap.Find (pName); 
	if (It)
		return (CStructMember*) It->m_Value;

	if (!Traverse)
		return NULL;

	size_t Count = m_BaseTypeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructType* pBaseType = m_BaseTypeArray [i];
		CStructMember* pMember = pBaseType->FindMember (pName, true);
		if (pMember)
			return pMember;
	}

	return NULL;
}

CStructMember*
CStructType::CreateMember (
	const rtl::CString& Name,
	CType* pType,
	size_t PackFactor
	)
{
	CStructMember* pMember = AXL_MEM_NEW (CStructMember);
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	pMember->m_Offset = 0; // TODO: calc
	m_MemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {

