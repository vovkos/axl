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

#include "pch.h"
#include "axl_lex_ParseError.h"
#include "axl_sl_Unpacker.h"

namespace axl {
namespace lex {

//..............................................................................

void
LineCol::incrementalCount (const sl::StringRef& string)
{
	const char* p = string.cp ();
	const char* end = string.getEnd ();
	const char* line = p;

	for (; p < end; p++)
		if (*p == '\n')
		{
			m_line++;
			line = p + 1;
		}

	m_col = (int) (p - line);
}

//..............................................................................

} // namespace lex
} // namespace axl
