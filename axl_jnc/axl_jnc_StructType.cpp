#include "stdafx.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CStructType::CStructType ()
{
	m_TypeKind = EType_Struct;
	m_Flags = ETypeFlag_IsPod;
	m_AlignFactor = 1;
	m_PackFactor = 8;
	m_FieldActualSize = 0;
	m_FieldAlignedSize = 0;
	m_pLastBitFieldType = NULL;
	m_LastBitFieldOffset = 0;
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
		if (pItem->GetItemKind () != EModuleItem_StructMember)
			return NULL;

		if (pBaseTypeOffset)
			*pBaseTypeOffset = 0;

		if (pLlvmBaseTypeIndexArray)
			pLlvmBaseTypeIndexArray->Clear ();

		return (CStructMember*) pItem;
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

	if (Name.IsEmpty ())
		return pMember;

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

bool
CStructType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_IsLayoutReady)
		return true;

	bool Result = PreCalcLayout ();
	if (!Result)
		return false;

	ResetLayout ();

	rtl::CIteratorT <CStructBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CStructBaseType* pBaseType = *BaseType;

		Result = pBaseType->m_pType->CalcLayout ();
		if (!Result)
			return false;

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

		Result = pMember->m_pType->CalcLayout ();
		if (!Result)
			return false;

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

	GetLlvmType ()->setBody (
		llvm::ArrayRef<llvm::Type*> (m_LlvmFieldTypeArray, m_LlvmFieldTypeArray.GetCount ()),
		true
		);

	m_Size = m_FieldAlignedSize;

	PostCalcLayout ();
	return true;
}

llvm::StructType* 
CStructType::GetLlvmType ()
{
	if (m_pLlvmType)
		return (llvm::StructType*) m_pLlvmType;
	
	m_pLlvmType = llvm::StructType::create (llvm::getGlobalContext (), (const tchar_t*) m_Tag);
	return (llvm::StructType*) m_pLlvmType;
}

void
CStructType::ResetLayout ()
{
	m_FieldActualSize = 0;
	m_FieldAlignedSize = 0;
	m_LlvmFieldTypeArray.Clear ();
	m_pLastBitFieldType = NULL;
	m_LastBitFieldOffset = 0;
	m_Flags &= ~ETypeFlag_IsLayoutReady;
}

bool
CStructType::LayoutField (
	llvm::Type* pLlvmType,
	size_t Size,
	size_t AlignFactor,
	size_t* pOffset,
	size_t* pLlvmIndex
	)
{
	if (AlignFactor > m_AlignFactor)
		m_AlignFactor = AlignFactor;

	size_t Offset = GetFieldOffset (AlignFactor);
	if (Offset > m_FieldActualSize)
		InsertPadding (Offset - m_FieldActualSize);

	*pOffset = Offset;
	*pLlvmIndex = m_LlvmFieldTypeArray.GetCount ();

	m_pLastBitFieldType = NULL;
	m_LastBitFieldOffset = 0;

	m_LlvmFieldTypeArray.Append (pLlvmType);
	SetFieldActualSize (Offset + Size);
	return true;
}

bool
CStructType::LayoutBitField (
	CType* pBaseType,
	size_t BitCount,
	CType** ppType,
	size_t* pOffset,
	size_t* pLlvmIndex
	)
{
	size_t BitOffset = GetBitFieldBitOffset (pBaseType, BitCount);
	CBitFieldType* pType = m_pModule->m_TypeMgr.GetBitFieldType (pBaseType, BitOffset, BitCount);
	if (!pType)
		return false;

	size_t AlignFactor = pType->GetAlignFactor ();
	if (AlignFactor > m_AlignFactor)
		m_AlignFactor = AlignFactor;

	*ppType = pType;

	if (BitOffset)
	{
		*pOffset = m_LastBitFieldOffset;
		*pLlvmIndex = m_LlvmFieldTypeArray.GetCount () - 1;
		return true;
	}

	size_t Offset = GetFieldOffset (AlignFactor);
	m_pLastBitFieldType = pType;
	m_LastBitFieldOffset = Offset;
	
	if (Offset > m_FieldActualSize)
		InsertPadding (Offset - m_FieldActualSize);

	*pOffset = Offset;
	*pLlvmIndex = m_LlvmFieldTypeArray.GetCount ();

	m_LlvmFieldTypeArray.Append (pType->GetLlvmType ());
	SetFieldActualSize (Offset + pType->GetSize ());
	return true;
}

size_t
CStructType::GetFieldOffset (size_t AlignFactor)
{
	size_t Offset = m_FieldActualSize;

	if (AlignFactor > m_PackFactor)
		AlignFactor = m_PackFactor;

	size_t Mod = Offset % AlignFactor;
	if (Mod)
		Offset += AlignFactor - Mod;

	return Offset;
}

size_t 
CStructType::GetBitFieldBitOffset (
	CType* pType,
	size_t BitCount
	)
{
	if (!m_pLastBitFieldType || m_pLastBitFieldType->GetBaseType ()->Cmp (pType) != 0)
		return 0;

	size_t LastBitOffset = 
		m_pLastBitFieldType->GetBitOffset () + 
		m_pLastBitFieldType->GetBitCount ();

	return LastBitOffset + BitCount <= pType->GetSize () * 8 ? LastBitOffset : 0;
}

size_t
CStructType::SetFieldActualSize (size_t Size)
{
	if (m_FieldActualSize >= Size)
		return m_FieldAlignedSize;

	m_FieldActualSize = Size;
	m_FieldAlignedSize = Size;

	size_t Mod = m_Size % m_AlignFactor;
	if (Mod)
		m_FieldAlignedSize += m_AlignFactor - Mod;

	return m_FieldAlignedSize;
}

CArrayType*
CStructType::InsertPadding (size_t Size)
{
	CArrayType* pType = m_pModule->m_TypeMgr.GetArrayType (EType_Int8_u, Size);
	m_LlvmFieldTypeArray.Append (pType->GetLlvmType ());
	return pType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {

