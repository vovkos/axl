// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

class CBasicBlock;

//.............................................................................

class CScope: 
	public CModuleItem,
	public CNamespace,
	public rtl::TListLink
{
protected:
	friend class CNamespaceMgr;

	size_t m_Level;

	CToken::CPos m_PosEnd;

	void* m_pCodeBegin;
	void* m_pCodeEnd;

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

	CToken::CPos 
	GetPosEnd ()
	{
		return m_PosEnd;
	}

	CScope*
	GetParentScope ()
	{
		return m_pParentNamespace && m_pParentNamespace->GetNamespaceKind () == ENamespace_Scope ? 
			(CScope*) m_pParentNamespace : NULL;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
