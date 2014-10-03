#include "pch.h"
#include "axl_err_ParseError.h"
#include "axl_rtl_Unpacker.h"

namespace axl {
namespace err {

//.............................................................................

rtl::String 
ParseErrorProvider::getErrorDescription (const ErrorData* error)
{
	rtl::Unpacker unpacker (error + 1, error->m_size - sizeof (ErrorData));

	rtl::String string;

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

		string.format("%s(%d,%d)", filePath, line + 1, col + 1);
		}
		break; 

	case ParseErrorCode_InvalidSyntax:
		string = "syntax error";
		break;

	case ParseErrorCode_InvalidSyntaxIn:
		{
		const char* location;
		
		unpacker.unpack (&location);
		string.format ("invalid syntax in '%s'", location);
		}
		break;

	case ParseErrorCode_ExpectedToken:
		{
		const char* expectedToken;
		const char* actualToken;
		
		unpacker.unpack (&expectedToken);
		unpacker.unpack (&actualToken);
		string.format ("expected '%s' before '%s'", expectedToken, actualToken);
		}
		break;

	case ParseErrorCode_UnexpectedToken:
		{
		const char* token;
		
		unpacker.unpack (&token);
		string.format ("unexpected '%s'", token);
		}
		break;

	case ParseErrorCode_UnexpectedTokenIn:
		{
		const char* token;
		const char* location;
		
		unpacker.unpack (&token);
		unpacker.unpack (&location);
		string.format ("unexpected '%s' in '%s'", token, location);
		}
		break;

	default:
		string = "parsing error";
	}

	return string;
}

//.............................................................................

} // namespace err
} // namespace axl

