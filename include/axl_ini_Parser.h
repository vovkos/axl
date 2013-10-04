// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_INI_PARSER_H

#include "axl_ini_Lexer.h"

namespace axl {
namespace ini {

//.............................................................................

template <typename T>
class CParserT: protected CLexer
{
public:
	bool
	Parse (
		const char* pSource,
		size_t Length = -1
		)
	{
		return Parse ("INI", pSource, Length);
	}

	bool
	Parse (
		const rtl::CString& FilePath,
		const char* pSource,
		size_t Length = -1
		)
	{
		CLexer::Create (FilePath, pSource, Length);

		for (;;)
		{
			EScanResult ScanResult = ScanLine ();
			switch (ScanResult)
			{
			case EScanResult_Section:
				static_cast <T*> (this)->OnSection (m_SectionName);
				break;

			case EScanResult_KeyValue:
				static_cast <T*> (this)->OnKeyValue (m_KeyName, m_Value);
				break;

			case EScanResult_Error:
				return false;
			
			case EScanResult_Eof:
			default:
				return true;
			}
		}

		return true;
	}
};

//.............................................................................

} // namespace ini
} // namespace axl
