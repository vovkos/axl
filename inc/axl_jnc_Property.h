// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

class CProperty:
	public CModuleItem,
	public rtl::TListLink
{
protected:
	friend class CFunctionMgr;

	CPropertyType* m_pType;
	CFunction* m_pGetter;
	CFunctionOverload m_Setter;

public:
	CProperty ()
	{
		m_ItemKind = EModuleItem_Property;
		m_pGetter = NULL;
	}

	CPropertyType* 
	GetType ()
	{
		return m_pType;
	}

	CFunction* 
	GetGetter ()
	{
		return m_pGetter;
	}

	CFunctionOverload*
	GetSetter ()
	{	
		return &m_Setter;
	}
};

//.............................................................................

class CGlobalProperty: 
	public CModuleItem,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CFunctionMgr;

	CProperty* m_pProperty;

public:
	CGlobalProperty ()
	{
		m_ItemKind = EModuleItem_GlobalProperty;
		m_pProperty = NULL;
	}

	CProperty* 
	GetProperty ()
	{
		return m_pProperty;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
