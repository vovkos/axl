// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_INIPARSER_H

#include "axl_rtl_String.h"

namespace axl {
namespace rtl {

//.............................................................................

class IniParserRoot
{
protected:
	enum StateKind 
	{
		StateKind_Space = 0,
		StateKind_Comment,
		StateKind_Section,
		StateKind_Key,
		StateKind_Value,
	};

	enum LineKind
	{
		LineKind_Empty,
		LineKind_Error,
		LineKind_Section,
		LineKind_KeyValue,
	};

	uint_t m_line;

protected:
	IniParserRoot ()
	{
		m_line = 0;
	}

	bool
	parseLine (
		const char* p,
		size_t length,
		LineKind* lineKind,
		rtl::String* string1,
		rtl::String* string2,
		size_t* lineLength
		);
};

//.............................................................................

template <typename T>
class IniParser: public IniParserRoot
{
public:
	bool
	parse (
		const char* source,
		size_t length = -1
		)
	{
		if (length == -1)
			length = strlen (source);
		
		const char* p = source;
		const char* end = p + length;
		m_line = 0;

		rtl::String string1;
		rtl::String string2;

		for (; p < end; p++)
		{
			LineKind lineKind;
			size_t lineLength 

			bool result = parseLine (p, end - p, &lineKind, &string1, &string2, &lineLength);
			if (!result)
				return false;

			if (isSection)
				static_cast <T*> (this)->onSection (name);
			else 
				static_cast <T*> (this)->onValue (name, value);

			p += lineLength;
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
	void* context,
	const char* sectionName
	);

// return false to stop parsing

typedef 
void
(*FOnKeyValueA)(
	void* context,
	void* sectionContext,
	const char* sectionName,
	const char* keyName,
	const char* value
	);

bool
parseIniA(
	const char* source,
	size_t length,
	FOnSectionA pfnOnSection,
	FOnKeyValueA pfnOnKeyValue,
	void* context
	);

#ifdef _UNICODE
#error unicode parse not supported yet
#else
#define parseIni parseIniA
#define FOnSection FOnSectionA
#define FOnKeyValue FOnKeyValueA
#endif

//.............................................................................

class IniParser
{
protected:
	struct KeyHandler: axl::rtl::ListEntry
	{
		axl::rtl::String m_keyName;
		FOnKeyValue m_pfnOnKeyValue;
		void* m_context;
	};

	struct Section: axl::rtl::ListEntry
	{
		axl::rtl::String m_sectionName;
		axl::rtl::List<KeyHandler> m_keyHandlerList;
		axl::rtl::Rbree m_keyHandlerMap;
		void* m_context;

		Section(): 
			m_keyHandlerMap(axl_rtl_CmpStringI) 
			{ m_context = NULL; }
	};

protected:
	axl::rtl::List<Section> m_sectionList;
	axl::rtl::Rbree m_sectionMap;
	Section* m_currentSection;

public:
	IniParser():
		m_sectionMap(axl_rtl_CmpStringI) 
		{ m_currentSection = NULL; }

	void setSection(const TCHAR* sectionName, void* context);
	void setKeyHandler(const TCHAR* keyName, FOnKeyValue pfnOnKeyValue, void* context);
	void clear();

	bool parse(const TCHAR* source, size_t length = -1);

protected:
	static 
	void* 
	_OnSection(
		void* context,
		const TCHAR* sectionName
		);

	static
	void
	_OnKeyValue(
		void* context,
		void* sectionContext,
		const TCHAR* sectionName,
		const TCHAR* keyName,
		const TCHAR* value
		);
};

//.............................................................................

} // namespace ini

 */