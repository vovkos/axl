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

#define _AXL_INI_GENERICPARSER_H

#include "axl_ini_Parser.h"

namespace axl {
namespace ini {

//..............................................................................

class NamedValue: public sl::ListLink
{
	friend class GenericParser;

protected:
	sl::String m_name;
	sl::String m_value;

public:
	const sl::String&
	getName() const
	{
		return m_name;
	}

	const sl::String&
	getValue() const
	{
		return m_value;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Section: public sl::ListLink
{
	friend class GenericParser;

protected:
	sl::String m_name;
	sl::StringHashTable<NamedValue*> m_namedValueMap;
	sl::List<NamedValue> m_namedValueList;
	sl::BoxList<sl::String> m_unnamedValueList;

public:
	const sl::String&
	getName()
	{
		return m_name;
	}

	NamedValue*
	getNamedValue(const sl::StringRef& name) const
	{
		sl::ConstStringHashTableIterator<NamedValue*> it = m_namedValueMap.find(name);
		return it ? it->m_value : NULL;
	}

	sl::ConstBoxList<sl::String>
	getUnnamedValueList() const
	{
		return m_unnamedValueList;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class GenericIni
{
	friend class GenericParser;

protected:
	sl::StringHashTable<Section*> m_namedSectionMap;
	sl::List<Section> m_namedSectionList;
	Section m_unnamedSection;

public:
	Section*
	getSection(const sl::StringRef& name) const
	{
		sl::ConstStringHashTableIterator<Section*> it = m_namedSectionMap.find(name);
		return it ? it->m_value : NULL;
	}

	const Section*
	getUnnamedSection() const
	{
		return &m_unnamedSection;
	}
};

//..............................................................................

class GenericParser: public Parser<GenericParser>
{
protected:
	GenericIni* m_ini;
	Section* m_currentSection;

public:
	GenericParser(GenericIni* ini = NULL)
	{
		setIni(ini);
	}

	GenericIni*
	getIni() const
	{
		return m_ini;
	}

	void
	setIni(GenericIni* ini)
	{
		m_ini = ini;
		m_currentSection = ini ? &ini->m_unnamedSection : NULL;
	}

public:
	bool
	onSection(const sl::StringRef& sectionName);

	bool
	onKeyValue(
		const sl::StringRef& keyName,
		const sl::StringRef& value
		);
};

//..............................................................................

} // namespace ini
} // namespace axl
