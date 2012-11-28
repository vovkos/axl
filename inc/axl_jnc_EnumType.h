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

	rtl::CBoxListT <CToken> m_Expression;

public:
	CEnumMember ()
	{
		m_ItemKind = EModuleItem_EnumMember;
		m_Value = 0;
	}

	CEnumType*
	GetParentType ()
	{
		return (CEnumType*) (CNamedType*) m_pParentNamespace; // double cast cause CEnumType is not defined yet
	}

	intptr_t
	GetValue ()
	{
		return m_Value;
	}

	bool
	HasExpression ()
	{
		return !m_Expression.IsEmpty ();
	}

	void
	SetExpression (rtl::CBoxListT <CToken>* pTokenList)
	{
		m_Expression.TakeOver (pTokenList);
	}

	rtl::CBoxIteratorT <CToken>
	GetExpressionFirstToken ()
	{
		return m_Expression.GetHead ();
	}

	rtl::CBoxIteratorT <CToken>
	GetExpressionLastToken ()
	{
		return m_Expression.GetTail ();
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
	CEnumType ();

	rtl::CIteratorT <CEnumMember>
	GetFirstMember ()
	{
		return m_MemberList.GetHead ();
	}

	CEnumMember*
	FindMember (const tchar_t* pName);

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
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
