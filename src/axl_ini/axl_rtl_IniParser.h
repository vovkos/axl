// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_INIPARSER_H

#include "axl_rtl_String.h"

namespace axl {
namespace rtl {

//.............................................................................

class CIniParserRoot
{
protected:
	enum EState 
	{
		EState_Space = 0,
		EState_Comment,
		EState_Section,
		EState_Key,
		EState_Value,
	};

	enum ELine
	{
		ELine_Empty,
		ELine_Error,
		ELine_Section,
		ELine_KeyValue,
	};

	uint_t m_Line;

protected:
	CIniParserRoot ()
	{
		m_Line = 0;
	}

	bool
	ParseLine (
		const char* p,
		size_t Length,
		ELine* pLineKind,
		rtl::CString* pString1,
		rtl::CString* pString2,
		size_t* pLineLength
		);
};

//.............................................................................

template <typename T>
class CIniParserT: public CIniParserRoot
{
public:
	bool
	Parse (
		const char* pSource,
		size_t Length = -1
		)
	{
		if (Length == -1)
			Length = strlen (pSource);
		
		const char* p = pSource;
		const char* pEnd = p + Length;
		m_Line = 0;

		rtl::CString String1;
		rtl::CString String2;

		for (; p < pEnd; p++)
		{
			ELine LineKind;
			size_t LineLength 

			bool Result = ParseLine (p, pEnd - p, &LineKind, &String1, &String2, &LineLength);
			if (!Result)
				return false;

			if (IsSection)
				static_cast <T*> (this)->OnSection (Name);
			else 
				static_cast <T*> (this)->OnValue (Name, Value);

			p += LineLength;
		}

		return true;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl

/*

#pragma once

namespace ini {

//.............................................................................

typedef 
void*
(*FOnSectionA)(
	void* pContext,
	const char* pSectionName
	);

// return false to stop parsing

typedef 
void
(*FOnKeyValueA)(
	void* pContext,
	void* pSectionContext,
	const char* pSectionName,
	const char* pKeyName,
	const char* pValue
	);

bool
ParseIniA(
	const char* pSource,
	size_t Length,
	FOnSectionA pfnOnSection,
	FOnKeyValueA pfnOnKeyValue,
	void* pContext
	);

#ifdef _UNICODE
#error Unicode parse not supported yet
#else
#define ParseIni ParseIniA
#define FOnSection FOnSectionA
#define FOnKeyValue FOnKeyValueA
#endif

//.............................................................................

class CIniParser
{
protected:
	struct TKeyHandler: axl::rtl::TListEntry
	{
		axl::rtl::CString m_KeyName;
		FOnKeyValue m_pfnOnKeyValue;
		void* m_pContext;
	};

	struct TSection: axl::rtl::TListEntry
	{
		axl::rtl::CString m_SectionName;
		axl::rtl::CListT<TKeyHandler> m_KeyHandlerList;
		axl::rtl::CRbTree m_KeyHandlerMap;
		void* m_pContext;

		TSection(): 
			m_KeyHandlerMap(axl_rtl_CmpStringI) 
			{ m_pContext = NULL; }
	};

protected:
	axl::rtl::CListT<TSection> m_SectionList;
	axl::rtl::CRbTree m_SectionMap;
	TSection* m_pCurrentSection;

public:
	CIniParser():
		m_SectionMap(axl_rtl_CmpStringI) 
		{ m_pCurrentSection = NULL; }

	void SetSection(const TCHAR* pSectionName, void* pContext);
	void SetKeyHandler(const TCHAR* pKeyName, FOnKeyValue pfnOnKeyValue, void* pContext);
	void Clear();

	bool Parse(const TCHAR* pSource, size_t Length = -1);

protected:
	static 
	void* 
	_OnSection(
		void* pContext,
		const TCHAR* pSectionName
		);

	static
	void
	_OnKeyValue(
		void* pContext,
		void* pSectionContext,
		const TCHAR* pSectionName,
		const TCHAR* pKeyName,
		const TCHAR* pValue
		);
};

//.............................................................................

} // namespace ini

 */