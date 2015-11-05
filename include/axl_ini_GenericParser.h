// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_INI_GENERICPARSER_H

#include "axl_ini_Parser.h"
#include "axl_sl_StringHashTable.h"

namespace axl {
namespace ini {

//.............................................................................

class NamedValue: public sl::ListLink
{
	friend class GenericParser;

protected:
	sl::String m_name;
	sl::String m_value;

public:
	sl::String 
	getName () const
	{
		return m_name;
	}

	sl::String 
	getValue () const
	{
		return m_value;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Section: public sl::ListLink
{
	friend class GenericParser;

protected:
	sl::String m_name;
	sl::StringHashTableMap <NamedValue*> m_namedValueMap;
	sl::StdList <NamedValue> m_namedValueList;
	sl::BoxList <sl::String> m_unnamedValueList;

public:
	sl::String 
	getName ()
	{
		return m_name;
	}

	NamedValue*
	getNamedValue (const char* name) const
	{
		sl::StringHashTableMapIterator <NamedValue*> it = m_namedValueMap.find (name);
		return it ? it->m_value : NULL;
	}

	sl::ConstBoxList <sl::String>
	getUnnamedValueList () const
	{
		return m_unnamedValueList;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class GenericIni
{
	friend class GenericParser;

protected:
	sl::StringHashTableMap <Section*> m_namedSectionMap;
	sl::StdList <Section> m_namedSectionList;
	Section m_unnamedSection;

public:
	Section* 
	getSection (const char* name) const
	{
		sl::StringHashTableMapIterator <Section*> it = m_namedSectionMap.find (name);
		return it ? it->m_value : NULL;
	}

	const Section* 
	getUnnamedSection () const
	{
		return &m_unnamedSection;
	}
};

//.............................................................................

class GenericParser: public Parser <GenericParser>
{
protected:
	GenericIni* m_ini;
	Section* m_currentSection;

public:
	GenericParser (GenericIni* ini = NULL)
	{
		setIni (ini);
	}

	GenericIni*
	getIni () const
	{
		return m_ini;
	}

	void
	setIni (GenericIni* ini)
	{
		m_ini = ini;
		m_currentSection = ini ? &ini->m_unnamedSection : NULL;
	}

public:
	bool
	onSection (const char* sectionName);

	bool
	onKeyValue (
		const char* keyName, 
		const char* value
		);
};

//.............................................................................

} // namespace ini
} // namespace axl
