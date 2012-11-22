// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_StructType.h"

namespace axl {
namespace jnc {

class CUnionType;

//.............................................................................

class CUnionMember: 
	public CModuleItem,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CUnionType;
	
	CType* m_pType;

public:
	CUnionMember ()
	{
		m_ItemKind = EModuleItem_UnionMember;
		m_pType = NULL;
	}

	CUnionType*
	GetParentType ()
	{
		return (CUnionType*) (CNamedType*) m_pParentNamespace; // double cast cause CUnionType is not defined yet
	}

	CType*
	GetType ()
	{
		return m_pType;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUnionType: public CStructTypeT <CUnionType>
{
protected:
	friend class CTypeMgr;
	friend class CParser;

	rtl::CStdListT <CUnionMember> m_MemberList;

public:
	CUnionType ()
	{
		m_TypeKind = EType_Union;
		m_Flags = ETypeFlag_IsPod;
	}

	llvm::StructType* 
	GetLlvmType ();

	rtl::CIteratorT <CUnionMember>
	GetFirstMember ()
	{
		return m_MemberList.GetHead ();
	}

	CUnionMember*
	FindMember (const tchar_t* pName)
	{
		rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName);
		return It && It->m_Value->GetItemKind () == EModuleItem_UnionMember ? (CUnionMember*) It->m_Value : NULL;
	}

	CUnionMember*
	CreateMember (
		const rtl::CString& Name,
		CType* pType
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
