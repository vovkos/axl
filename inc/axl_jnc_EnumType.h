// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

class CEnumType;

//.............................................................................

enum EEnumTypeFlag
{
	EEnumTypeFlag_Exposed = 0x010000,
};

//.............................................................................

class CEnumConst: public CUserModuleItem
{
protected:
	friend class CEnumType;
	friend class CNamespace;

	CEnumType* m_pParentEnumType;
	intptr_t m_Value;
	rtl::CBoxListT <CToken> m_Expression;

public:
	CEnumConst ()
	{
		m_ItemKind = EModuleItem_EnumConst;
		m_pParentEnumType = NULL;
		m_Value = 0;
	}

	CEnumType*
	GetParentEnumType ()
	{
		return m_pParentEnumType;
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
	
	CType* m_pBaseType;
	rtl::CStdListT <CEnumConst> m_ConstList;
	intptr_t m_CurrentValue;

public:
	CEnumType ();

	CType*
	GetBaseType ()
	{
		return m_pBaseType;
	}

	rtl::CConstListT <CEnumConst>
	GetConstList ()
	{
		return m_ConstList;
	}

	CEnumConst*
	CreateConst (
		const rtl::CString& Name,
		intptr_t Value
		);

	CEnumConst*
	CreateConst (const rtl::CString& Name)
	{
		return CreateConst (Name, m_CurrentValue);
	}

protected:
	virtual 
	void
	PrepareTypeString ()
	{
		m_TypeString.Format ((m_Flags & EEnumTypeFlag_Exposed) ? _T("enumc %s") : _T("enum %s"), m_Tag);
	}

	virtual 
	void
	PrepareLlvmType ()
	{
		m_pLlvmType = llvm::Type::getInt32Ty (llvm::getGlobalContext ());
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
