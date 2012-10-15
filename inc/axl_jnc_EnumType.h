// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

//.............................................................................

class CEnumMember: 
	public CModuleItem,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CEnumType;
	friend class CNamespace;

	intptr_t m_Value;

public:
	CEnumMember ()
	{
		m_ItemKind = EModuleItem_EnumMember;
		m_Value = 0;
	}

	intptr_t
	GetValue ()
	{
		return m_Value;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CEnumType: public CNamedType
{
protected:
	friend class CTypeMgr;
	friend class CParser;

	rtl::CStdListT <CEnumMember> m_MemberList;
	intptr_t m_CurrentValue;

public:
	CEnumType ()
	{
		m_TypeKind = EType_Enum;
		m_CurrentValue = 0;
	}

	CEnumMember*
	FindMember (
		const tchar_t* pName,
		bool Traverse = false
		);

	CEnumMember*
	CreateMember (
		const rtl::CString& Name,
		intptr_t Value
		);

	CEnumMember*
	CreateMember (const rtl::CString& Name)
	{
		return CreateMember (Name, m_CurrentValue);
	}

	rtl::CIteratorT <CEnumMember>
	GetFirstMember ()
	{
		return m_MemberList.GetHead ();
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
