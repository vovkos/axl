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

#define _AXL_INI_PARSER_H

#include "axl_ini_Lexer.h"

namespace axl {
namespace ini {

//..............................................................................

template <typename T>
class Parser: protected Lexer
{
public:
	bool
	parseFile(const sl::StringRef& filePath)
	{
		io::SimpleMappedFile file;
		return
			file.open(filePath, io::FileFlag_ReadOnly) &&
			parse(filePath, sl::StringRef((const char*) file.p(), file.getMappingSize()));
	}

	bool
	parse(const sl::StringRef& source)
	{
		return parse("INI", source);
	}

	bool
	parse(
		const sl::StringRef& filePath,
		const sl::StringRef& source
		)
	{
		bool result;

		Lexer::create(filePath, source);

		for (;;)
		{
			ScanResultKind scanResult = scanLine();
			switch (scanResult)
			{
			case ScanResultKind_Error:
				return false;

			case ScanResultKind_Eof:
				return static_cast<T*> (this)->finalize();

			case ScanResultKind_Section:
				result = static_cast<T*> (this)->onSection(m_sectionName);
				if (!result)
					return false;

				break;

			case ScanResultKind_KeyValue:
				result = static_cast<T*> (this)->onKeyValue(m_keyName, m_value);
				if (!result)
					return false;

				break;

			default:
				ASSERT(false);
			}
		}
	}

	bool
	onSection(const sl::StringRef& sectionName) // overridable
	{
		return true;
	}

	bool
	finalize() // overridable
	{
		return true;
	}

	static
	bool
	parseBoolValue(const sl::StringRef& value)
	{
		return _stricmp(value.sz(), "true") == 0 || atoi (value.sz()) != 0;
	}
};

//..............................................................................

} // namespace ini
} // namespace axl
