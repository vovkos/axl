#include "stdafx.h"
#include "axl_jnc_UnionType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CUnionMember*
CUnionType::CreateMember (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount
	)
{
	CUnionMember* pMember = AXL_MEM_NEW (CUnionMember);
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	pMember->m_pBitFieldBaseType = BitCount ? pType : NULL;
	pMember->m_BitCount = BitCount;
	m_MemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

bool
CUnionType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_IsLayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	CType* pLargestMemberType = NULL;

	rtl::CIteratorT <CUnionMember> Member = m_MemberList.GetHead ();
	for (; Member; Member++)
	{
		CUnionMember* pMember = *Member;

		Result = pMember->m_pType->CalcLayout ();
		if (!Result)
			return false;

		if (pMember->m_BitCount)
		{
			pMember->m_pType = m_pModule->m_TypeMgr.GetBitFieldType (pMember->m_pBitFieldBaseType, 0, pMember->m_BitCount);
			if (!pMember->m_pType)
				return false;
		}

		if (!pLargestMemberType || pMember->m_pType->GetSize () > pLargestMemberType->GetSize ())
			pLargestMemberType = pMember->m_pType;
	}

	ASSERT (pLargestMemberType);

	m_pStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType ();
	m_pStructType->m_Tag.Format (_T("%s.struct"), m_Tag);
	m_pStructType->CreateMember (pLargestMemberType);
	Result = m_pStructType->CalcLayout ();
	if (!Result)
		return false;

	PostCalcLayout ();
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {

