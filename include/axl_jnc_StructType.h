// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_DerivableType.h"
#include "axl_jnc_BitFieldType.h"

namespace axl {
namespace jnc {

class CStructType;
class CUnionType;

//.............................................................................

enum EStdField
{
	EStdField_OnChange = 0,
	EStdField_Value,
	EStdField__Count
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char* 
GetStdFieldString (EStdField Field);

//.............................................................................

class CStructField: public CUserModuleItem
{
	friend class CStructType;
	friend class CUnionType;
	
protected:
	CDerivableType* m_pParentType; // struct or union
	CType* m_pType;
	int m_PtrTypeFlags;
	rtl::CBoxListT <CToken> m_Initializer;

	CType* m_pBitFieldBaseType;
	size_t m_BitCount;
	
	size_t m_Offset;
	uint_t m_LlvmIndex;

public:
	CStructField ();

	CDerivableType*
	GetParentType ()
	{
		return m_pParentType;
	}

	CType*
	GetType ()
	{
		return m_pType;
	}

	int
	GetPtrTypeFlags ()
	{
		return m_PtrTypeFlags;
	}

	rtl::CConstBoxListT <CToken> 
	GetInitializer ()
	{
		return m_Initializer;
	}

	size_t
	GetOffset ()
	{
		return m_Offset;
	}

	uint_t
	GetLlvmIndex ()
	{
		return m_LlvmIndex;
	}
};

//.............................................................................

class CStructType: public CDerivableType
{
	friend class CTypeMgr;
	friend class CClassType;
	friend class CProperty;
	friend class CAutoEv;

protected:
	size_t m_AlignFactor;
	size_t m_PackFactor;
	size_t m_FieldActualSize;
	size_t m_FieldAlignedSize;

	rtl::CStdListT <CStructField> m_FieldList;
	rtl::CArrayT <CStructField*> m_FieldArray;
	rtl::CArrayT <CStructField*> m_InitializedFieldArray;
	rtl::CArrayT <llvm::Type*> m_LlvmFieldTypeArray;
	CBitFieldType* m_pLastBitFieldType;
	size_t m_LastBitFieldOffset;

public:
	CStructType ();

	virtual
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

	rtl::CConstListT <CStructField>
	GetFieldList ()
	{
		return m_FieldList;
	}

	CStructField*
	GetFieldByIndex (size_t Index);

	rtl::CArrayT <CStructField*> 
	GetInitializedFieldArray ()
	{
		return m_InitializedFieldArray;
	}

	bool
	IsClassStructType ();

	CStructField*
	CreateField (
		const rtl::CString& Name,
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0,
		rtl::CBoxListT <CToken>* pInitializer = NULL
		);

	CStructField*
	CreateField (
		CType* pType,
		size_t BitCount = 0,
		int PtrTypeFlags = 0
		)
	{
		return CreateField (rtl::CString (), pType, BitCount, PtrTypeFlags);
	}

	bool
	Append (CStructType* pType);

	virtual
	bool
	CalcLayout ();

	bool
	CallBaseTypeConstructors ();

	bool
	InitializeFields ();

protected:
	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format ("struct %s", m_Tag.cc ()); // thanks a lot gcc
	}

	virtual 
	void
	PrepareLlvmType ()
	{
		m_pLlvmType = llvm::StructType::create (llvm::getGlobalContext (), m_Tag.cc ());
	}

	bool
	LayoutField (
		llvm::Type* pLlvmType,
		size_t Size,
		size_t AlignFactor,
		size_t* pOffset,
		uint_t* pLlvmIndex
		);

	bool
	LayoutField (
		CType* pType,
		size_t* pOffset,
		uint_t* pLlvmIndex
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
		uint_t* pLlvmIndex
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

} // namespace jnc {
} // namespace axl {