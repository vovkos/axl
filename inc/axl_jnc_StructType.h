// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_BitFieldType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CStructBaseType: public rtl::TListLink
{
protected:
	friend class CStructType;

	CStructType* m_pType;
	size_t m_Offset;
	size_t m_LlvmIndex;

public:
	CStructBaseType ()
	{
		m_pType = NULL;
		m_Offset = 0;
		m_LlvmIndex = -1;
	}

	CStructType*
	GetType ()
	{
		return m_pType;
	}

	size_t 
	GetOffset ()
	{
		return m_Offset;
	}

	size_t
	GetLlvmIndex ()
	{
		return m_LlvmIndex;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CStructMember: 
	public CModuleItem,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CStructType;
	
	CType* m_pType;
	CType* m_pBitFieldBaseType;
	size_t m_BitCount;
	size_t m_Offset;
	size_t m_LlvmIndex;

public:
	CStructMember ()
	{
		m_ItemKind = EModuleItem_StructMember;
		m_pType = NULL;
		m_pBitFieldBaseType = NULL;
		m_BitCount = 0;
		m_Offset = 0;
		m_LlvmIndex = -1;
	}

	CStructType*
	GetParentType ()
	{
		return (CStructType*) (CNamedType*) m_pParentNamespace; // double cast cause CStructType is not defined yet
	}

	CType*
	GetType ()
	{
		return m_pType;
	}

	size_t
	GetOffset ()
	{
		return m_Offset;
	}

	size_t
	GetLlvmIndex ()
	{
		return m_LlvmIndex;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CStructType: public CNamedType
{
protected:
	friend class CTypeMgr;
	friend class CUnionType;
	friend class CClassType;
	friend class CPointerType;

	size_t m_AlignFactor;
	size_t m_PackFactor;
	size_t m_FieldActualSize;
	size_t m_FieldAlignedSize;

	rtl::CStringHashTableMapAT <CStructBaseType*> m_BaseTypeMap;
	rtl::CStdListT <CStructBaseType> m_BaseTypeList;
	rtl::CStdListT <CStructMember> m_MemberList;

	rtl::CArrayT <llvm::Type*> m_LlvmFieldTypeArray;
	CBitFieldType* m_pLastBitFieldType;
	size_t m_LastBitFieldOffset;

public:
	CStructType ();

	llvm::StructType* 
	GetLlvmType ();

	size_t 
	GetAlignFactor ()
	{
		return m_AlignFactor;
	}

	size_t 
	GetPackFactor ()
	{
		return m_PackFactor;
	}

	size_t 
	GetFieldActualSize ()
	{
		return m_FieldActualSize;
	}

	size_t 
	GetFieldAlignedSize ()
	{
		return m_FieldAlignedSize;
	}

	rtl::CIteratorT <CStructBaseType>
	GetFirstBaseType ()
	{
		return m_BaseTypeList.GetHead ();
	}

	bool
	FindBaseType (
		CStructType* pType,
		size_t* pOffset = NULL,
		rtl::CArrayT <size_t>* pLlvmIndexArray = NULL
		);

	CStructBaseType*
	AddBaseType (CStructType* pType);

	rtl::CIteratorT <CStructMember>
	GetFirstMember ()
	{
		return m_MemberList.GetHead ();
	}

	CStructMember*
	FindMember (
		const tchar_t* pName,
		size_t* pBaseTypeOffset,
		rtl::CArrayT <size_t>* pLlvmBaseTypeIndexArray
		);

	CStructMember*
	CreateMember (
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0
		);

	CStructMember*
	CreateMember (
		CType* pType,
		size_t BitCount = 0
		)
	{
		return CreateMember (rtl::CString (), pType, BitCount);
	}

	bool
	CalcLayout ();

protected:
	void
	ResetLayout ();

	bool
	LayoutField (
		llvm::Type* pLlvmType,
		size_t Size,
		size_t AlignFactor,
		size_t* pOffset,
		size_t* pLlvmIndex
		);

	bool
	LayoutField (
		CType* pType,
		size_t* pOffset,
		size_t* pLlvmIndex
		)
	{
		return LayoutField (pType->GetLlvmType (), pType->GetSize (), pType->GetAlignFactor (), pOffset, pLlvmIndex);
	}

	bool
	LayoutBitField (
		CType* pBaseType,
		size_t BitCount,
		CType** ppType,
		size_t* pOffset,
		size_t* pLlvmIndex
		);

	size_t
	GetFieldOffset (size_t AlignFactor);

	size_t 
	GetBitFieldBitOffset (
		CType* pType,
		size_t BitCount
		);

	size_t
	SetFieldActualSize (size_t Size);

	CArrayType*
	InsertPadding (size_t Size);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
