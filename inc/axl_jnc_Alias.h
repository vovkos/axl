// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

//.............................................................................

class CAlias: public CUserModuleItem
{
protected:
	friend class CVariableMgr;

	CType* m_pType;
	rtl::CBoxListT <CToken> m_Initializer;

public:
	CAlias ()
	{
		m_ItemKind = EModuleItem_Alias;
		m_pType = NULL;
	}
	
	CType* 
	GetType ()
	{
		return m_pType;
	}

	rtl::CConstBoxListT <CToken> 
	GetInitializer ()
	{
		return m_Initializer;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {


