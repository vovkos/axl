// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Namespace.h"
#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

class CBasicBlock;

//.............................................................................

class CScope: 
	public CModuleItem,
	public CNamespace
{
protected:
	friend class CNamespaceMgr;

	size_t m_Level;

	CToken::CPos m_BeginPos;
	CToken::CPos m_EndPos;
	CFunction* m_pFunction;

	rtl::CBoxListT <CValue> m_DestructList;

public:
	CBasicBlock* m_pBreakBlock;
	CBasicBlock* m_pContinueBlock;

	// local variable list for debugger

public:
	CScope ();

	size_t
	GetLevel ()
	{
		return m_Level;
	}

	const CToken::CPos&
	GetBeginPos ()
	{
		return m_BeginPos;
	}

	const CToken::CPos&
	GetEndPos ()
	{
		return m_EndPos;
	}

	CFunction*
	GetFunction ()
	{
		return m_pFunction;
	}

	CScope*
	GetParentScope ()
	{
		return m_pParentNamespace && m_pParentNamespace->GetNamespaceKind () == ENamespace_Scope ? 
			(CScope*) m_pParentNamespace : NULL;
	}

	void
	AddToDestructList (const CValue& Value)
	{
		m_DestructList.InsertTail (Value);
	}

	rtl::CBoxListT <CValue>*
	GetDestructList ()
	{
		return &m_DestructList;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
