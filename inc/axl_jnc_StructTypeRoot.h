// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"
#include "axl_jnc_BitFieldType.h"

namespace axl {
namespace jnc {

class CStructType;

//.............................................................................

class CStructTypeRoot: public CNamedType
{
protected:
	struct TMemberLayoutEntry
	{
		size_t m_Offset;
		size_t m_LlvmIndex;
	};

protected:
	size_t m_AlignFactor;
	size_t m_PackFactor;
	size_t m_FieldActualSize;
	size_t m_FieldAlignedSize;
	
	rtl::CArrayT <llvm::Type*> m_LlvmFieldTypeArray;
	CBitFieldType* m_pLastBitFieldType;
	size_t m_LastBitFieldOffset;

public:
	CStructTypeRoot ();

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

	llvm::StructType*
	GetLlvmStructType (const tchar_t* pName);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
