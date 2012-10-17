// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Namespace.h"

namespace axl {
namespace jnc {

//.............................................................................

class CVariable:
	public CModuleItem,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CVariableMgr;

	CType* m_pType;
	void* m_p;

public:
	CVariable ()
	{
		m_ItemKind = EModuleItem_Variable;
		m_pType = NULL;
		m_p = NULL;
	}

	CType* 
	GetType ()
	{
		return m_pType;
	}

	void* 
	GetData ()
	{
		return m_p;
	}	
};

//.............................................................................

class CVariableMgr
{
protected:
	friend class CModule;
	CModule* m_pModule;

	rtl::CStdListT <CVariable> m_VariableList;

public:
	CVariableMgr ()
	{
		m_pModule = NULL;
	}

	void
	Clear ();

	CVariable*
	CreateVariable (
		const rtl::CString& Name,
		CType* pType
		);
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
