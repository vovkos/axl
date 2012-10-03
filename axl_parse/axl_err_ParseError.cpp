#include "stdafx.h"
#include "axl_err_ParseError.h"
#include "axl_rtl_Unpacker.h"

namespace axl {
namespace err {

//.............................................................................

rtl::CString 
CParseErrorProviderA::GetErrorDescription (const TError* pError)
{
	rtl::CUnpacker Unpacker (pError + 1, pError->m_Size - sizeof (CError));

	rtl::CStringA String;

	switch (pError->m_Code)
	{
	case EParseError_SrcPos:
		{
		const char* pFilePath;
		long Line;
		long Col;

		Unpacker.Unpack (&pFilePath);
		Unpacker.Unpack (&Line);
		Unpacker.Unpack (&Col);

		String.Format("%s(%d)", pFilePath,  Line + 1);
		}
		break; 

	case EParseError_InvalidSyntax:
		String = "syntax error";
		break;

	case EParseError_InvalidSyntaxIn:
		{
		const char* pLocation;
		
		Unpacker.Unpack (&pLocation);
		String.Format ("invalid syntax in '%s'", pLocation);
		}
		break;

	case EParseError_ExpectedToken:
		{
		const char* pExpectedToken;
		const char* pActualToken;
		
		Unpacker.Unpack (&pExpectedToken);
		Unpacker.Unpack (&pActualToken);
		String.Format ("expected '%s' before '%s'", pExpectedToken, pActualToken);
		}
		break;

	case EParseError_UnexpectedToken:
		{
		const char* pToken;
		
		Unpacker.Unpack (&pToken);
		String.Format ("unexpected '%s'", pToken);
		}
		break;

	case EParseError_UnexpectedTokenIn:
		{
		const char* pToken;
		const char* pLocation;
		
		Unpacker.Unpack (&pToken);
		Unpacker.Unpack (&pLocation);
		String.Format ("unexpected '%s' in '%s'", pToken, pLocation);
		}
		break;

	default:
		String = "parsing error";
	}

	return String;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

rtl::CString 
CParseErrorProviderW::GetErrorDescription (const TError* pError)
{
	rtl::CUnpacker Unpacker (pError + 1, pError->m_Size - sizeof (CError));

	rtl::CStringW String;

	switch (pError->m_Code)
	{
	case EParseError_SrcPos:
		{
		const wchar_t* pFilePath;
		long Line;
		long Col;

		Unpacker.Unpack (&pFilePath);
		Unpacker.Unpack (&Line);
		Unpacker.Unpack (&Col);

		String.Format(L"%s(%d)", pFilePath,  Line + 1);
		}
		break; 

	case EParseError_InvalidSyntax:
		String = L"syntax error";
		break;

	case EParseError_InvalidSyntaxIn:
		{
		const wchar_t* pLocation;
		
		Unpacker.Unpack (&pLocation);
		String.Format (L"invalid syntax in '%s'", pLocation);
		}
		break;

	case EParseError_ExpectedToken:
		{
		const wchar_t* pExpectedToken;
		const wchar_t* pActualToken;
		
		Unpacker.Unpack (&pExpectedToken);
		Unpacker.Unpack (&pActualToken);
		String.Format (L"expected '%s' before '%s'", pExpectedToken, pActualToken);
		}
		break;

	case EParseError_UnexpectedToken:
		{
		const wchar_t* pToken;
		
		Unpacker.Unpack (&pToken);
		String.Format (L"unexpected '%s'", pToken);
		}
		break;

	case EParseError_UnexpectedTokenIn:
		{
		const wchar_t* pToken;
		const wchar_t* pLocation;
		
		Unpacker.Unpack (&pToken);
		Unpacker.Unpack (&pLocation);
		String.Format (L"unexpected '%s' in '%s'", pToken, pLocation);
		}
		break;

	default:
		String = L"parsing error";
	}

	return String;
}


//.............................................................................

} // namespace err
} // namespace axl

