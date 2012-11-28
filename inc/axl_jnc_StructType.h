// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_StructTypeRoot.h"

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

class CStructType: public CStructTypeRoot
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
		m_Flags = ETypeFlag_IsPod;
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

	bool
	CalcLayout ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
