// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_INI_PARSER_H

#include "axl_ini_Lexer.h"
#include "axl_io_MappedFile.h"

namespace axl {
namespace ini {

//.............................................................................

template <typename T>
class CParserT: protected CLexer
{
public:
	bool
	parseFile (const rtl::CString& filePath)
	{
		io::CSimpleMappedFile file;
		return 
			file.open (filePath, io::EFileFlag_ReadOnly) &&
			parse (filePath, (const char*) file.p (), (size_t) file.getSize ());
	}

	bool
	parse (
		const char* source,
		size_t length = -1
		)
	{
		return parse ("INI", source, length);
	}

	bool
	parse (
		const rtl::CString& filePath,
		const char* source,
		size_t length = -1
		)
	{
		bool result;

		CLexer::create (filePath, source, length);

		for (;;)
		{
			EScanResult scanResult = scanLine ();
			switch (scanResult)
			{
			case EScanResult_Error:
				return false;
			
			case EScanResult_Eof:
				return static_cast <T*> (this)->finalize ();

			case EScanResult_Section:
				result = static_cast <T*> (this)->onSection (m_sectionName);
				if (!result)
					return false;

				break;

			case EScanResult_KeyValue:
				result = static_cast <T*> (this)->onKeyValue (m_keyName, m_value);
				if (!result)
					return false;

				break;

			default:
				ASSERT (false);
			}
		}
	}

	bool 
	onSection (const char* sectionName) // overridable
	{
		return true;
	}

	bool
	finalize () // overridable
	{
		return true;
	}

	static 
	bool
	parseBoolValue (const char* value)
	{
		return _stricmp (value, "true") == 0 || atoi (value) != 0;
	}
};

//.............................................................................

} // namespace ini
} // namespace axl
