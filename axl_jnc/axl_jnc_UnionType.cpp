#include "stdafx.h"
#include "axl_jnc_UnionType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CUnionMember*
CUnionType::CreateMember (
	const rtl::CString& Name,
	CType* pType
	)
{
	size_t AlignFactor = pType->GetAlignFactor ();
	if (AlignFactor > m_AlignFactor)
		m_AlignFactor = AlignFactor;

	size_t Offset = GetFieldOffset (AlignFactor);
	SetSize (Offset + pType->GetSize ());

	CUnionMember* pMember = AXL_MEM_NEW (CUnionMember);
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	m_MemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

llvm::StructType* 
CUnionType::GetLlvmType ()
{
	if (m_Flags & ETypeFlag_IsLlvmReady)
		return (llvm::StructType*) m_pLlvmType;

	rtl::CIteratorT <CUnionMember> Member = m_MemberList.GetHead ();

	CType* pLargestMemberType = Member->GetType ();

	for (Member++; Member; Member++)
	{
		CType* pMemberType = Member->GetType ();
		if (pMemberType->GetSize () > pLargestMemberType->GetSize ())
			pLargestMemberType = pMemberType;
	}

	llvm::Type* LlvmMemberTypeArray [2] = { pLargestMemberType->GetLlvmType () };
	size_t LlvmMemberCount = 1;

	if (m_Size > m_ActualSize) // insert padding
	{
		CArrayType* pPaddingArrayType = m_pModule->m_TypeMgr.GetArrayType (EType_Int8_u, m_Size - m_ActualSize);
		LlvmMemberTypeArray [1] = pPaddingArrayType->GetLlvmType ();
		LlvmMemberCount = 2;
	}
	
	llvm::StructType* pLlvmType = llvm::StructType::create (
		llvm::getGlobalContext (),
		llvm::ArrayRef<llvm::Type*> (LlvmMemberTypeArray, LlvmMemberCount),
		(const tchar_t*) GetQualifiedName (), 
		true
		);

	m_pLlvmType = pLlvmType;
	m_Flags |= ETypeFlag_IsLlvmReady;
	return pLlvmType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {

