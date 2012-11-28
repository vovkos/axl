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
	ResetLayout ();

	CType* pLargestMemberType = NULL;

	rtl::CIteratorT <CUnionMember> Member = m_MemberList.GetHead ();
	for (; Member; Member++)
	{
		CUnionMember* pMember = *Member;

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

	m_LlvmFieldTypeArray.Append (pLargestMemberType->GetLlvmType ());
	SetFieldActualSize (pLargestMemberType->GetSize ());

	if (m_FieldAlignedSize > m_FieldActualSize)
		InsertPadding (m_FieldAlignedSize - m_FieldActualSize);

	m_Size = m_FieldAlignedSize;
	return true;
}

llvm::StructType* 
CUnionType::GetLlvmType ()
{
	if (!m_pLlvmType)
		m_pLlvmType = GetLlvmStructType (GetQualifiedName ());
	
	return (llvm::StructType*) m_pLlvmType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {

