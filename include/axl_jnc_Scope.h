// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Namespace.h"
#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

class CBasicBlock;
class CFunction;

//.............................................................................

class CScope: 
	public CModuleItem,
	public CNamespace
{
	friend class CNamespaceMgr;
	friend class CFunctionMgr;
	friend class CParser;

protected:
	size_t m_Level;

	CToken::CPos m_BeginPos;
	CToken::CPos m_EndPos;
	CFunction* m_pFunction;

	rtl::CBoxListT <CValue> m_DestructList;
	rtl::CBoxListT <CValue> m_GcRootList;

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
		return m_pParentNamespace && m_pParentNamespace->GetNamespaceKind () == ENamespace_Scope ? (CScope*) m_pParentNamespace : NULL;
	}

	rtl::CConstBoxListT <CValue>
	GetDestructList ()
	{
		return m_DestructList;
	}

	void
	AddToDestructList (const CValue& Value)
	{
		m_DestructList.InsertTail (Value);
	}

	rtl::CConstBoxListT <CValue>
	GetGcRootList ()
	{
		return m_GcRootList;
	}

	void
	AddToGcRootList (const CValue& Value)
	{
		m_GcRootList.InsertTail (Value);
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
