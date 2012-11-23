#include "stdafx.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CStructBaseType*
CStructType::AddBaseType (CStructType* pType)
{
	size_t AlignFactor = pType->GetAlignFactor ();
	if (AlignFactor > m_AlignFactor)
		m_AlignFactor = AlignFactor;

	size_t Offset = GetFieldOffset (AlignFactor);

	rtl::CStringHashTableMapIteratorAT <CStructBaseType*> It = m_BaseTypeMap.Goto (pType->GetSignature ());
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("'%s' is already a base type"), pType->GetTypeString ());
		return NULL;
	}

	CStructBaseType* pBaseType = AXL_MEM_NEW (CStructBaseType);
	pBaseType->m_pType = pType;
	pBaseType->m_Offset = Offset;
	m_BaseTypeList.InsertTail (pBaseType);
	It->m_Value = pBaseType;

	SetSize (Offset + pType->GetSize ());

	return pBaseType;
}

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

CStructMember*
CStructType::FindMember (
	const tchar_t* pName,
	size_t* pOffset,
	rtl::CArrayT <size_t>* pLlvmIndexArray
	)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
	if (It)
	{
		CModuleItem* pItem = It->m_Value;
		if (pItem->GetItemKind () != EModuleItem_StructMember)
			return NULL;

		CStructMember* pMember = (CStructMember*) pItem;
		*pOffset = pMember->m_Offset;
		pLlvmIndexArray->Copy (&pMember->m_LlvmIndex, 1);
		return pMember;
	}

	char Buffer [256];
	rtl::CArrayT <size_t> LlvmIndexArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	rtl::CIteratorT <CStructBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		size_t Offset;
		CStructMember* pMember = BaseType->m_pType->FindMember (pName, &Offset, &LlvmIndexArray);
		if (pMember)
		{
			*pOffset = BaseType->m_Offset + Offset;

			pLlvmIndexArray->Clear ();
			pLlvmIndexArray->Append (BaseType->m_LlvmIndex);
			pLlvmIndexArray->Append (LlvmIndexArray);	
			return pMember;
		}
	}

	return NULL;
}

CStructMember*
CStructType::CreateMember (
	const rtl::CString& Name,
	CType* pType
	)
{
	if (!(pType->GetFlags () & ETypeFlag_IsPod))
	{
		err::SetFormatStringError (_T("'%s' is illegal struct member type (POD only allowed)"), pType->GetTypeString ());
		return NULL;
	}

	size_t AlignFactor = pType->GetAlignFactor ();
	if (AlignFactor > m_AlignFactor)
		m_AlignFactor = AlignFactor;

	size_t Offset;

	if (pType->GetTypeKind () != EType_BitField)
	{
		Offset = GetFieldOffset (AlignFactor);
		SetSize (Offset + pType->GetSize ());
	}
	else
	{
		CBitFieldType* pBitFieldType = (CBitFieldType*) pType;
		if (pBitFieldType->GetBitOffset ()) // we use the last member's offset
		{
			ASSERT (!m_MemberList.IsEmpty ());
			Offset = m_MemberList.GetTail ()->m_Offset;
		}
		else // new slot
		{
			Offset = GetFieldOffset (AlignFactor);
			SetSize (Offset + pType->GetSize ());
		}

		m_pLastBitFieldType = pBitFieldType;
	}

	CStructMember* pMember = AXL_MEM_NEW (CStructMember);
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	pMember->m_Offset = Offset;
	m_MemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;


	return pMember;
}

llvm::StructType* 
CStructType::GetLlvmType ()
{
	if (m_Flags & ETypeFlag_IsLlvmReady)
		return (llvm::StructType*) m_pLlvmType;
	
	char Buffer [256];
	rtl::CArrayT <llvm::Type*> LlvmMemberTypeArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	LlvmMemberTypeArray.Reserve (m_BaseTypeList.GetCount () + m_MemberList.GetCount ()); // could be more, if padding members are inserted
	
	size_t Offset = 0;
	size_t LlvmIndex = 0;

	rtl::CIteratorT <CStructBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CStructBaseType* pBaseType = *BaseType;

		if (pBaseType->m_Offset > Offset) // insert padding
		{
			CArrayType* pPaddingArrayType = m_pModule->m_TypeMgr.GetArrayType (EType_Int8_u, pBaseType->m_Offset - Offset);
			LlvmMemberTypeArray.Append (pPaddingArrayType->GetLlvmType ());
			Offset = pBaseType->m_Offset;
			LlvmIndex++;
		}

		LlvmMemberTypeArray.Append (pBaseType->m_pType->GetLlvmType ());
		Offset = pBaseType->m_Offset + pBaseType->m_pType->GetSize ();
		pBaseType->m_LlvmIndex = LlvmIndex++;
	}

	rtl::CIteratorT <CStructMember> Member = m_MemberList.GetHead ();
	for (; Member; Member++)
	{
		CStructMember* pMember = *Member;

		if (pMember->m_Offset > Offset) // insert padding
		{
			CArrayType* pPaddingArrayType = m_pModule->m_TypeMgr.GetArrayType (EType_Int8_u, pMember->m_Offset - Offset);
			LlvmMemberTypeArray.Append (pPaddingArrayType->GetLlvmType ());
			Offset = pMember->m_Offset;
			LlvmIndex++;
		}

		if (pMember->m_pType->GetTypeKind () == EType_BitField &&
			((CBitFieldType*) pMember->m_pType)->GetBitOffset () != 0)
		{
			pMember->m_LlvmIndex = LlvmIndex - 1;
		}
		else
		{
			pMember->m_LlvmIndex = LlvmIndex++;
			LlvmMemberTypeArray.Append (pMember->m_pType->GetLlvmType ());
			Offset = pMember->m_Offset + pMember->m_pType->GetSize ();
		}
	}

	ASSERT (Offset == m_ActualSize);

	if (m_Size > m_ActualSize) // insert padding
	{
		CArrayType* pPaddingArrayType = m_pModule->m_TypeMgr.GetArrayType (EType_Int8_u, m_Size - m_ActualSize);
		LlvmMemberTypeArray.Append (pPaddingArrayType->GetLlvmType ());
	}

	llvm::StructType* pLlvmType = llvm::StructType::create (
		llvm::getGlobalContext (),
		llvm::ArrayRef<llvm::Type*> (LlvmMemberTypeArray, LlvmMemberTypeArray.GetCount ()),
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

