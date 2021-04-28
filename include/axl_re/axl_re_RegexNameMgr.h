//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_RE_REGEXNAMEMGR_H

namespace axl {
namespace re {

//..............................................................................

class RegexNameMgr
{
public:
	virtual
	sl::StringRef
	findName(const sl::StringRef& name) = 0;
};

//..............................................................................

class StdRegexNameMgr: public RegexNameMgr
{
protected:
	sl::StringHashTable<sl::String> m_nameMap;

public:
	void
	clear()
	{
		m_nameMap.clear();
	}

	virtual
	sl::StringRef
	findName(const sl::StringRef& name)
	{
		return m_nameMap.findValue(name, NULL);
	}

	void
	addName(
		const sl::StringRef& name,
		const sl::StringRef& source
		)
	{
		m_nameMap[name] = source;
	}

	bool
	removeName(const sl::StringRef& name)
	{
		return m_nameMap.eraseKey(name);
	}
};

//..............................................................................

} // namespace re
} // namespace axl
