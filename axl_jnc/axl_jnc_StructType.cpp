#include "stdafx.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................
	
CStructMember::CStructMember ()
{
	m_ItemKind = EModuleItem_StructMember;
	m_pType = NULL;
	m_pBitFieldBaseType = NULL;
	m_BitCount = 0;
	m_Offset = 0;
	m_LlvmIndex = -1;
}

//.............................................................................

CStructType::CStructType ()
{
	m_TypeKind = EType_Struct;
	m_Flags = ETypeFlag_Pod;
	m_AlignFactor = 1;
	m_PackFactor = 8;
	m_FieldActualSize = 0;
	m_FieldAlignedSize = 0;
	m_pLastBitFieldType = NULL;
	m_LastBitFieldOffset = 0;
}

bool
CStructType::FindBaseTypeImpl (
	CStructType* pType,
	CStructBaseTypeCoord* pCoord,
	size_t Level
	)
{
	rtl::CStringHashTableMapIteratorAT <CStructBaseType*> It = m_BaseTypeMap.Find (pType->GetSignature ());
	if (It)
	{
		if (pCoord)
		{
			CStructBaseType* pBaseType = It->m_Value;
			pCoord->m_Offset = pBaseType->m_Offset;
			pCoord->m_LlvmIndexArray.SetCount (Level + 1);
			pCoord->m_LlvmIndexArray [Level] = pBaseType->m_LlvmIndex;
		}
		
		return true;
	}

	rtl::CIteratorT <CStructBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CStructBaseType* pBaseType = *BaseType;

		bool Result = pBaseType->m_pType->FindBaseTypeImpl (pType, pCoord, Level + 1);
		if (Result)
		{
			if (pCoord)
			{
				pCoord->m_Offset += pBaseType->m_Offset;
				pCoord->m_LlvmIndexArray [Level] = pBaseType->m_LlvmIndex;
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

CModuleItem*
CStructType::FindItemWithBaseTypeList (const tchar_t* pName)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
	if (It)
		return It->m_Value;

	if (m_Name == pName)
		return this;

	rtl::CIteratorT <CStructBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CStructType* pStructType = BaseType->m_pType;
		if (pStructType->m_Name == pName)
			return pStructType;

		CModuleItem* pItem = pStructType->FindItemWithBaseTypeList (pName);
		if (pItem)
			return pItem;
	}

	return NULL;
}

CStructMember*
CStructType::FindMemberImpl (
	const tchar_t* pName,
	CStructBaseTypeCoord* pBaseTypeCoord,
	size_t Level
	)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
	if (It)
	{
		CModuleItem* pItem = It->m_Value;
		if (pItem->GetItemKind () != EModuleItem_StructMember)
			return NULL;

		if (pBaseTypeCoord)
			pBaseTypeCoord->m_LlvmIndexArray.SetCount (Level);

		return (CStructMember*) pItem;
	}

	rtl::CIteratorT <CStructBaseType> BaseType = m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		CStructBaseType* pBaseType = *BaseType;
		CStructMember* pMember = pBaseType->m_pType->FindMemberImpl (pName, pBaseTypeCoord, Level + 1);
		if (pMember)
		{
			if (pBaseTypeCoord)
			{
				pBaseTypeCoord->m_Offset += pBaseType->m_Offset;
				pBaseTypeCoord->m_LlvmIndexArray [Level] = pBaseType->m_LlvmIndex;
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
	pMember->m_StorageKind = m_StorageKind;
	pMember->m_pParentStructType = this;
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	pMember->m_pBitFieldBaseType = BitCount ? pType : NULL;
	pMember->m_BitCount = BitCount;
	m_MemberList.InsertTail (pMember);

	if (!(pType->GetFlags () & ETypeFlag_Pod))
		m_Flags &= ~ETypeFlag_Pod;

	if (!Name.IsEmpty ())
	{
		bool Result = AddItem (pMember);
		if (!Result)
			return NULL;
	}

	return pMember;
}

bool
CStructType::Append (CStructType* pType)
{
	bool Result;

	rtl::CIteratorT <CStructBaseType> BaseType = pType->m_BaseTypeList.GetHead ();
	for (; BaseType; BaseType++)
	{
		Result = AddBaseType (BaseType->m_pType) != NULL;
		if (!Result)
			return false;
	}

	rtl::CIteratorT <CStructMember> Member = pType->m_MemberList.GetHead ();
	for (; Member; Member++)
	{
		Result = Member->m_BitCount ? 
			CreateMember (Member->m_Name, Member->m_pBitFieldBaseType, Member->m_BitCount) != NULL:
			CreateMember (Member->m_Name, Member->m_pType) != NULL;

		if (!Result)
			return false;
	}

	return true;
}

bool
CStructType::CalcLayout ()
{
	if (m_Flags & ETypeFlag_LayoutReady)
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

	llvm::StructType* pLlvmStructType = (llvm::StructType*) GetLlvmType ();
	pLlvmStructType->setBody (
		llvm::ArrayRef<llvm::Type*> (m_LlvmFieldTypeArray, m_LlvmFieldTypeArray.GetCount ()),
		true
		);

	m_Size = m_FieldAlignedSize;

	PostCalcLayout ();
	return true;
}

void
CStructType::ResetLayout ()
{
	m_FieldActualSize = 0;
	m_FieldAlignedSize = 0;
	m_LlvmFieldTypeArray.Clear ();
	m_pLastBitFieldType = NULL;
	m_LastBitFieldOffset = 0;
	m_Flags &= ~ETypeFlag_LayoutReady;
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

} // namespace jnc {
} // namespace axl {

