// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_FSM_STDREGEXPNAMEMGR_H

#include "axl_fsm_RegExp.h"

namespace axl {
namespace fsm {

//.............................................................................

class StdRegExpNameMgr: public RegExpNameMgr
{
protected:
	sl::StringHashTableMap <sl::String> m_nameMap;

public:
	void
	clear ()
	{
		m_nameMap.clear ();
	}

	virtual 
	sl::StringRef
	findName (const sl::StringRef& name)
	{
		return m_nameMap.findValue (name, NULL);
	}

	void
	addName (
		const sl::StringRef& name,
		const sl::StringRef& source
		)
	{
		m_nameMap [name] = source;
	}

	bool
	removeName (const sl::StringRef& name)
	{
		return m_nameMap.eraseKey (name);
	}
};

//.............................................................................

} // namespace fsm
} // namespace axl
