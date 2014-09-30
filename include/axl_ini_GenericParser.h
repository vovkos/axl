// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_INI_GENERICPARSER_H

#include "axl_ini_Parser.h"
#include "axl_rtl_StringHashTable.h"

namespace axl {
namespace ini {

//.............................................................................

class NamedValue: public rtl::ListLink
{
	friend class GenericParser;

protected:
	rtl::String m_name;
	rtl::String m_value;

public:
	rtl::String 
	getName () const
	{
		return m_name;
	}

	rtl::String 
	getValue () const
	{
		return m_value;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Section: public rtl::ListLink
{
	friend class GenericParser;

protected:
	rtl::String m_name;
	rtl::StringHashTableMap <NamedValue*> m_namedValueMap;
	rtl::StdList <NamedValue> m_namedValueList;
	rtl::BoxList <rtl::String> m_unnamedValueList;

public:
	rtl::String 
	getName ()
	{
		return m_name;
	}

	NamedValue*
	getNamedValue (const char* name) const
	{
		rtl::StringHashTableMapIterator <NamedValue*> it = m_namedValueMap.find (name);
		return it ? it->m_value : NULL;
	}

	rtl::ConstBoxList <rtl::String>
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
	rtl::StringHashTableMap <Section*> m_namedSectionMap;
	rtl::StdList <Section> m_namedSectionList;
	Section m_unnamedSection;

public:
	Section* 
	getSection (const char* name) const
	{
		rtl::StringHashTableMapIterator <Section*> it = m_namedSectionMap.find (name);
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
