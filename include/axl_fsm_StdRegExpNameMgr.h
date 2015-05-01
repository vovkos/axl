// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_FSM_STDREGEXPNAMEMGR_H

#include "axl_fsm_RegExp.h"
#include "axl_rtl_StringHashTable.h"

namespace axl {
namespace fsm {

//.............................................................................

class StdRegExpNameMgr: public RegExpNameMgr
{
protected:
	struct Name: public rtl::ListLink
	{
		rtl::String m_name;
		rtl::String m_source;
	};

protected:
	rtl::StdList <Name> m_nameList;
	rtl::StringHashTableMap <Name*> m_nameMap;

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
		rtl::StringHashTableMapIterator <Name*> it = m_nameMap.find (name);
		return it ? it->m_value->m_source.cc () : NULL;
	}

	void
	addName (
		const rtl::String& name,
		const rtl::String& source
		);

	void
	removeName (const char* name);
};

//.............................................................................

} // namespace fsm
} // namespace axl
