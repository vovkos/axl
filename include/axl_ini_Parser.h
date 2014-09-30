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
class Parser: protected Lexer
{
public:
	bool
	parseFile (const rtl::String& filePath)
	{
		io::SimpleMappedFile file;
		return 
			file.open (filePath, io::FileFlagKind_ReadOnly) &&
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
		const rtl::String& filePath,
		const char* source,
		size_t length = -1
		)
	{
		bool result;

		Lexer::create (filePath, source, length);

		for (;;)
		{
			ScanResultKind scanResult = scanLine ();
			switch (scanResult)
			{
			case ScanResultKind_Error:
				return false;
			
			case ScanResultKind_Eof:
				return static_cast <T*> (this)->finalize ();

			case ScanResultKind_Section:
				result = static_cast <T*> (this)->onSection (m_sectionName);
				if (!result)
					return false;

				break;

			case ScanResultKind_KeyValue:
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
