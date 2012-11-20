// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"
#include "axl_jnc_ImportType.h"

namespace axl {
namespace jnc {

class CStructType;

//.............................................................................

template <typename T>
class CStructTypeT: public CNamedType
{
protected:
	size_t m_ActualSize;
	size_t m_AlignFactor;
	size_t m_PackFactor;

public:
	CStructTypeT ()
	{
		m_ActualSize = 0;
		m_AlignFactor = 1;
		m_PackFactor = 8;
	}

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

protected:
	size_t
	GetFieldOffset (size_t AlignFactor)
	{
		size_t Offset = m_ActualSize;

		if (AlignFactor > m_PackFactor)
			AlignFactor = m_PackFactor;

		size_t Mod = Offset % AlignFactor;
		if (Mod)
			Offset += AlignFactor - Mod;

		return Offset;
	}

	size_t
	SetSize (size_t Size)
	{
		if (m_ActualSize >= Size)
			return m_Size;

		m_ActualSize = Size;
		m_Size = Size;

		size_t Mod = m_Size % m_AlignFactor;
		if (Mod)
			m_Size += m_AlignFactor - Mod;

		return m_Size;
	}
};

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
	size_t m_Offset;
	size_t m_LlvmIndex;

public:
	CStructMember ()
	{
		m_ItemKind = EModuleItem_StructMember;
		m_pType = NULL;
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

class CStructType: public CStructTypeT <CStructType>
{
protected:
	friend class CTypeMgr;
	friend class CParser;

	rtl::CStringHashTableMapAT <CStructBaseType*> m_BaseTypeMap;
	rtl::CStdListT <CStructBaseType> m_BaseTypeList;
	rtl::CStdListT <CStructMember> m_MemberList;

public:
	CStructType ()
	{
		m_TypeKind = EType_Struct;
		m_Flags = ETypeFlag_IsPod | ETypeFlag_IsIncomplete;
	}

	llvm::StructType* 
	GetLlvmType ();

	rtl::CIteratorT <CStructBaseType>
	GetFirstBaseType ()
	{
		return m_BaseTypeList.GetHead ();
	}

	bool
	FindBaseType (
		CStructType* pType,
		size_t* pOffset,
		rtl::CArrayT <size_t>* pLlvmIndexArray
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
		size_t* pOffset,
		rtl::CArrayT <size_t>* pLlvmIndexArray
		);

	CStructMember*
	CreateMember (
		const rtl::CString& Name,
		CType* pType
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
