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
	struct Name: public sl::ListLink
	{
		sl::String m_name;
		sl::String m_source;
	};

protected:
	sl::StdList <Name> m_nameList;
	sl::StringHashTableMap <Name*> m_nameMap;

public:
	void
	clear ()
	{
		m_nameList.clear ();
		m_nameMap.clear ();
	}

	virtual 
	const char*
	findName (const char* name)
	{
		sl::StringHashTableMapIterator <Name*> it = m_nameMap.find (name);
		return it ? it->m_value->m_source.cc () : NULL;
	}

	void
	addName (
		const sl::String& name,
		const sl::String& source
		);

	void
	removeName (const char* name);
};

//.............................................................................

} // namespace fsm
} // namespace axl
