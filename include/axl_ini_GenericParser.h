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

class CNamedValue: public rtl::TListLink
{
	friend class CGenericParser;

protected:
	rtl::CString m_name;
	rtl::CString m_value;

public:
	rtl::CString 
	getName () const
	{
		return m_name;
	}

	rtl::CString 
	getValue () const
	{
		return m_value;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CSection: public rtl::TListLink
{
	friend class CGenericParser;

protected:
	rtl::CString m_name;
	rtl::CStringHashTableMapT <CNamedValue*> m_namedValueMap;
	rtl::CStdListT <CNamedValue> m_namedValueList;
	rtl::CBoxListT <rtl::CString> m_unnamedValueList;

public:
	rtl::CString 
	getName ()
	{
		return m_name;
	}

	CNamedValue*
	getNamedValue (const char* name) const
	{
		rtl::CStringHashTableMapIteratorT <CNamedValue*> it = m_namedValueMap.find (name);
		return it ? it->m_value : NULL;
	}

	rtl::CConstBoxListT <rtl::CString>
	getUnnamedValueList () const
	{
		return m_unnamedValueList;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CGenericIni
{
	friend class CGenericParser;

protected:
	rtl::CStringHashTableMapT <CSection*> m_namedSectionMap;
	rtl::CStdListT <CSection> m_namedSectionList;
	CSection m_unnamedSection;

public:
	CSection* 
	getSection (const char* name) const
	{
		rtl::CStringHashTableMapIteratorT <CSection*> it = m_namedSectionMap.find (name);
		return it ? it->m_value : NULL;
	}

	const CSection* 
	getUnnamedSection () const
	{
		return &m_unnamedSection;
	}
};

//.............................................................................

class CGenericParser: public CParserT <CGenericParser>
{
protected:
	CGenericIni* m_ini;
	CSection* m_currentSection;

public:
	CGenericParser (CGenericIni* ini = NULL)
	{
		setIni (ini);
	}

	CGenericIni*
	getIni () const
	{
		return m_ini;
	}

	void
	setIni (CGenericIni* ini)
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
