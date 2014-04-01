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
	rtl::CString m_Name;
	rtl::CString m_Value;

public:
	rtl::CString 
	GetName () const
	{
		return m_Name;
	}

	rtl::CString 
	GetValue () const
	{
		return m_Value;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CSection: public rtl::TListLink
{
	friend class CGenericParser;

protected:
	rtl::CString m_Name;
	rtl::CStringHashTableMapT <CNamedValue*> m_NamedValueMap;
	rtl::CStdListT <CNamedValue> m_NamedValueList;
	rtl::CBoxListT <rtl::CString> m_UnnamedValueList;

public:
	rtl::CString 
	GetName ()
	{
		return m_Name;
	}

	CNamedValue*
	GetNamedValue (const char* pName) const
	{
		rtl::CStringHashTableMapIteratorT <CNamedValue*> It = m_NamedValueMap.Find (pName);
		return It ? It->m_Value : NULL;
	}

	rtl::CConstBoxListT <rtl::CString>
	GetUnnamedValueList () const
	{
		return m_UnnamedValueList;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CGenericIni
{
	friend class CGenericParser;

protected:
	rtl::CStringHashTableMapT <CSection*> m_NamedSectionMap;
	rtl::CStdListT <CSection> m_NamedSectionList;
	CSection m_UnnamedSection;

public:
	CSection* 
	GetSection (const char* pName) const
	{
		rtl::CStringHashTableMapIteratorT <CSection*> It = m_NamedSectionMap.Find (pName);
		return It ? It->m_Value : NULL;
	}

	const CSection* 
	GetUnnamedSection () const
	{
		return &m_UnnamedSection;
	}
};

//.............................................................................

class CGenericParser: public CParserT <CGenericParser>
{
protected:
	CGenericIni* m_pIni;
	CSection* m_pCurrentSection;

public:
	CGenericParser (CGenericIni* pIni = NULL)
	{
		SetIni (pIni);
	}

	CGenericIni*
	GetIni () const
	{
		return m_pIni;
	}

	void
	SetIni (CGenericIni* pIni)
	{
		m_pIni = pIni;
		m_pCurrentSection = pIni ? &pIni->m_UnnamedSection : NULL;
	}

public:
	bool
	OnSection (const char* pSectionName);

	bool
	OnKeyValue (
		const char* pKeyName, 
		const char* pValue
		);
};

//.............................................................................

} // namespace ini
} // namespace axl
