#include "pch.h"
#include "axl_err_ParseError.h"
#include "axl_rtl_Unpacker.h"

namespace axl {
namespace err {

//.............................................................................

rtl::CString 
CParseErrorProvider::getErrorDescription (const TError* error)
{
	rtl::CUnpacker unpacker (error + 1, error->m_size - sizeof (TError));

	rtl::CString string;

	switch (error->m_code)
	{
	case EParseError_SrcPos:
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

	case EParseError_InvalidSyntax:
		string = "syntax error";
		break;

	case EParseError_InvalidSyntaxIn:
		{
		const char* location;
		
		unpacker.unpack (&location);
		string.format ("invalid syntax in '%s'", location);
		}
		break;

	case EParseError_ExpectedToken:
		{
		const char* expectedToken;
		const char* actualToken;
		
		unpacker.unpack (&expectedToken);
		unpacker.unpack (&actualToken);
		string.format ("expected '%s' before '%s'", expectedToken, actualToken);
		}
		break;

	case EParseError_UnexpectedToken:
		{
		const char* token;
		
		unpacker.unpack (&token);
		string.format ("unexpected '%s'", token);
		}
		break;

	case EParseError_UnexpectedTokenIn:
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

