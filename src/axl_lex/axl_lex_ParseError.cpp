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

sl::StringRef
ParseErrorProvider::getErrorDescription (const err::ErrorRef& error)
{
	sl::Unpacker unpacker (error + 1, error->m_size - sizeof (err::ErrorHdr));

	switch (error->m_code)
	{
	case ParseErrorCode_SrcPos:
		{
		const char* filePath;
		int line;
		int col;

		unpacker.unpack (&filePath);
		unpacker.unpack (&line);
		unpacker.unpack (&col);

		return sl::formatString ("%s(%d,%d)", filePath, line + 1, col + 1);
		}

	case ParseErrorCode_InvalidSyntax:
		return "syntax error";

	case ParseErrorCode_InvalidSyntaxIn:
		{
		const char* location;

		unpacker.unpack (&location);
		return sl::formatString ("invalid syntax in '%s'", location);
		}

	case ParseErrorCode_ExpectedToken:
		{
		const char* expectedToken;
		const char* actualToken;

		unpacker.unpack (&expectedToken);
		unpacker.unpack (&actualToken);
		return sl::formatString ("expected '%s' before '%s'", expectedToken, actualToken);
		}

	case ParseErrorCode_UnexpectedToken:
		{
		const char* token;

		unpacker.unpack (&token);
		return sl::formatString ("unexpected '%s'", token);
		}

	case ParseErrorCode_UnexpectedTokenIn:
		{
		const char* token;
		const char* location;

		unpacker.unpack (&token);
		unpacker.unpack (&location);
		return sl::formatString ("unexpected '%s' in '%s'", token, location);
		}

	default:
		return "parsing error";
	}
}

//..............................................................................

} // namespace lex
} // namespace axl
