// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"
#include "axl_jnc_ImportType.h"

namespace axl {
namespace jnc {

//.............................................................................

class CStructClassType: public CNamedType
{
protected:
	rtl::CArrayT <CType*> m_BaseTypeArray;
	rtl::CArrayT <CImportType*> m_GenericArgumentArray;

	size_t m_PackFactor;

public:
	CStructClassType ()
	{
		m_PackFactor = 8;
	}

	size_t 
	GetPackFactor ()
	{
		return m_PackFactor;
	}

	size_t 
	GetBaseTypeCount ()
	{
		return m_BaseTypeArray.GetCount ();
	}

	CType*
	GetBaseType (size_t Index)
	{
		ASSERT (Index < m_BaseTypeArray.GetCount ());
		return m_BaseTypeArray [Index];
	}

	bool
	AddBaseType (CType* pType); 

	size_t 
	GetGenericArgumentCount ()
	{
		return m_GenericArgumentArray.GetCount ();
	}

	CImportType*
	GetGenericArgument (size_t Index)
	{
		ASSERT (Index < m_GenericArgumentArray.GetCount ());
		return m_GenericArgumentArray [Index];
	}

	bool
	AddGenericArgument (CImportType* pType);
};

//.............................................................................

class CStructMember: 
	public CModuleItem,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CStructType;

	CType* m_pType;
	size_t m_Offset;

public:
	CStructMember ()
	{
		m_ItemKind = EModuleItem_StructMember;
		m_pType = NULL;
		m_Offset = 0;
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
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CStructType: public CStructClassType
{
protected:
	friend class CTypeMgr;
	friend class CParser;

	rtl::CStdListT <CStructMember> m_MemberList;

public:
	CStructType ()
	{
		m_TypeKind = EType_Struct;
	}

	llvm::StructType* 
	GetLlvmType ();

	CStructMember*
	FindMember (
		const tchar_t* pName,
		bool Traverse = false
		);

	CStructMember*
	CreateMember (
		const rtl::CString& Name,
		CType* pType
		);

	rtl::CIteratorT <CStructMember>
	GetFirstMember ()
	{
		return m_MemberList.GetHead ();
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
