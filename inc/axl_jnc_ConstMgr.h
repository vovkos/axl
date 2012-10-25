// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

//.............................................................................

class CConstMgr
{
protected:
	friend class CModule;
	CModule* m_pModule;

public:
	CConstMgr (CModule* pModule)
	{
		m_pModule = pModule;
	}

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ()
	{
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
