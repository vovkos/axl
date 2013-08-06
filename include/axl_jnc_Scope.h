// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Namespace.h"
#include "axl_jnc_DestructList.h"

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

	rtl::CBoxListT <CValue> m_GcRootList;
	llvm::DIScope m_LlvmDiScope;

public:
	CBasicBlock* m_pBreakBlock;
	CBasicBlock* m_pContinueBlock;
	CDestructList m_DestructList;

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
	GetGcRootList ()
	{
		return m_GcRootList;
	}

	void
	AddToGcRootList (const CValue& Value)
	{
		m_GcRootList.InsertTail (Value);
	}

	llvm::DIScope 
	GetLlvmDiScope ()
	{
		return m_LlvmDiScope;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
