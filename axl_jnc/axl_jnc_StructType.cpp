#include "stdafx.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CStructType::FindBaseType (
	CStructType* pType,
	size_t* pOffset,
	rtl::CArrayT <size_t>* pLlvmIndexArray
	)
{
	rtl::CStringHashTableMapIteratorAT <CStructBaseType*> It = m_BaseTypeMap.Find (pType->GetSignature ());
	if (It)
	{
		CStructBaseType* pBaseType = It->m_Value;

		if (pOffset)
			*pOffset = pBaseType->m_Offset;

		if (pLlvmIndexArray)
			pLlvmIndexArray->Copy (&pBaseType->m_LlvmIndex, 1);
		
		return true;
	}

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	rtl::CIteratorT <CStructBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		size_t Offset;
		bool Result = BaseType->m_pType->FindBaseType (pType, &Offset, &LlvmIndexArray);
		if (Result)
		{
			if (pOffset)
				*pOffset = BaseType->m_Offset + Offset;

			if (pLlvmIndexArray)
			{
				pLlvmIndexArray->Clear ();
				pLlvmIndexArray->Append (BaseType->m_LlvmIndex);
				pLlvmIndexArray->Append (LlvmIndexArray);
			}

			return true;
		}
	}

	return false;
}

CStructBaseType*
CStructType::AddBaseType (CStructType* pType)
{
	rtl::CStringHashTableMapIteratorAT <CStructBaseType*> It = m_BaseTypeMap.Goto (pType->GetSignature ());
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("'%s' is already a base type"), pType->GetTypeString ());
		return NULL;
	}

	CStructBaseType* pBaseType = AXL_MEM_NEW (CStructBaseType);
	pBaseType->m_pType = pType;
	m_BaseTypeList.InsertTail (pBaseType);
	It->m_Value = pBaseType;
	return pBaseType;
}

CStructMember*
CStructType::FindMember (
	const tchar_t* pName,
	size_t* pBaseTypeOffset,
	rtl::CArrayT <size_t>* pLlvmBaseTypeIndexArray
	)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
	if (It)
	{
		CModuleItem* pItem = It->m_Value;
		return pItem->GetItemKind () == EModuleItem_StructMember ? (CStructMember*) pItem : NULL;
	}

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmBaseTypeIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	rtl::CIteratorT <CStructBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		size_t BaseTypeOffset;
		CStructMember* pMember = BaseType->m_pType->FindMember (
			pName, 
			&BaseTypeOffset, 
			&LlvmBaseTypeIndexArray
			);

		if (pMember)
		{
			if (pBaseTypeOffset)
				*pBaseTypeOffset = BaseType->m_Offset + BaseTypeOffset;

			if (pLlvmBaseTypeIndexArray)
			{
				pLlvmBaseTypeIndexArray->Clear ();
				pLlvmBaseTypeIndexArray->Append (BaseType->m_LlvmIndex);
				pLlvmBaseTypeIndexArray->Append (LlvmBaseTypeIndexArray);
			}

			return pMember;
		}
	}

	return NULL;
}

CStructMember*
CStructType::CreateMember (
	const rtl::CString& Name,
	CType* pType,
	size_t BitCount
	)
{
	CStructMember* pMember = AXL_MEM_NEW (CStructMember);
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
CStructType::CalcLayout ()
{
	bool Result;

	ResetLayout ();

	rtl::CIteratorT <CStructBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CStructBaseType* pBaseType = *BaseType;

		Result = LayoutField (
			pBaseType->m_pType,
			&pBaseType->m_Offset,
			&pBaseType->m_LlvmIndex
			);

		if (!Result)
			return false;
	}

	rtl::CIteratorT <CStructMember> Member = m_MemberList.GetHead ();
	for (; Member; Member++)
	{
		CStructMember* pMember = *Member;

		Result = pMember->m_BitCount ? 
			LayoutBitField (
				pMember->m_pBitFieldBaseType,
				pMember->m_BitCount,
				&pMember->m_pType,
				&pMember->m_Offset,
				&pMember->m_LlvmIndex
				) :
			LayoutField (
				pMember->m_pType,
				&pMember->m_Offset,
				&pMember->m_LlvmIndex
				);

			if (!Result)
				return false;
	}

	if (m_FieldAlignedSize > m_FieldActualSize)
		InsertPadding (m_FieldAlignedSize - m_FieldActualSize);

	m_Size = m_FieldAlignedSize;
	return true;
}

llvm::StructType* 
CStructType::GetLlvmType ()
{
	if (!m_pLlvmType)
		m_pLlvmType = GetLlvmStructType (GetQualifiedName ());
	
	return (llvm::StructType*) m_pLlvmType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {

