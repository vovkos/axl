#include "pch.h"
#include "axl_rtl_CmdLineParser.h"
#include "axl_err_Error.h"

namespace axl {
namespace rtl {

//.............................................................................

size_t
CCmdLineParserRoot::ExtractArg (
	const char* p,
	const char* pEnd,
	rtl::CString* pArg
	)
{
	const char* p0 = p;

	while (p < pEnd && isspace (*p))
		p++;

	if (p >= pEnd)
	{
		pArg->Clear ();
		return p - p0;
	}

	const char* p1 = p;

	if (*p != '\"')
	{
		while (p < pEnd && !isspace (*p))
			p++;
	}
	else
	{
		p1++;

		while (p < pEnd)
		{
			if (*p == '\"')
				break;
			else if (*p == '\\')
				p++;

			p++;
		}

		if (p > pEnd)
		{
			err::SetStringError ("unterminated escape sequence");
			return -1;
		}
	}

	pArg->Copy (p1, p - p1);
	return p - p0;
}

bool
CCmdLineParserRoot::ParseArg (
	const char* p,
	rtl::CString* pSwitchName,
	rtl::CString* pValue
	)
{
	if (*p != '-')
	{
		pSwitchName->Clear ();
		pValue->Copy (p);
		return true;
	}
	
	p++;

	if (*p != '-')
	{
		if (!isalpha (*p))
		{
			err::SetStringError ("invalid command line switch syntax");
			return false;
		}

		pSwitchName->Copy (*p);
		p++;
	}
	else
	{
		p++;

		if (!isalpha (*p))
		{
			err::SetStringError ("invalid command line switch syntax");
			return false;
		}

		const char* p1 = p;

		while (*p && *p != '=' && !isspace (*p))
			p++;

		pSwitchName->Copy (p1, p - p1);
	}

	if (*p && !isspace (*p)) // have value
	{
		if (*p == '=')
			p++;

		const char* p1 = p;

		while (*p && !isspace (*p))
			p++;

		pValue->Copy (p1, p - p1);
	}

	return true;
}

//.............................................................................

} // namespace rtl
} // namespace axl

