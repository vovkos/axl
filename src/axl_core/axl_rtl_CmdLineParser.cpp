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
	else
	{
		pValue->Clear ();
	}

	return true;
}

//.............................................................................

CString
GetCmdLineHelpString (const CConstListT <TSwitchInfo>& SwitchInfoList)
{
	enum
	{
		IndentSize        = 2,
		GapSize           = 2,
		SwitchLengthLimit = 40,
	};

	// calculate max switch length

	size_t MaxSwitchLength = 0;

	CIteratorT <TSwitchInfo> It = SwitchInfoList.GetHead ();
	for (; It; It++)
	{
		TSwitchInfo* pSwitchInfo = *It;
		if (!pSwitchInfo->m_Switch) // group
			continue;

		size_t SwitchLength = 0;

		bool HasValue = (pSwitchInfo->m_Switch & ECmdLineSwitchFlag_HasValue) != 0;

		size_t i = 0;
		for (; i < countof (pSwitchInfo->m_NameTable); i++)
		{
			if (!pSwitchInfo->m_NameTable [i])
				break;

			if (pSwitchInfo->m_NameTable [i] [1])
			{
				SwitchLength += 2; // "--"
				SwitchLength += strlen (pSwitchInfo->m_NameTable [i]);
			}
			else
			{
				SwitchLength += 2; // "-c"
			}
		}

		if (i > 1)
		{
			SwitchLength += (i - 1) * 2; // ", "
		}

		if (HasValue)
		{
			SwitchLength++; // '='
			SwitchLength += strlen (pSwitchInfo->m_pValue);
		}

		if (SwitchLength < SwitchLengthLimit && MaxSwitchLength < SwitchLength)
			MaxSwitchLength = SwitchLength;
	}

	size_t DescriptionCol = MaxSwitchLength + IndentSize + GapSize;

	// generate string

	CString String;
	CString LineString;

	It = SwitchInfoList.GetHead ();
	for (; It; It++)
	{
		TSwitchInfo* pSwitchInfo = *It;
		if (!pSwitchInfo->m_Switch) // group
		{
			if (!String.IsEmpty ())
				String.AppendNewLine ();

			LineString = pSwitchInfo->m_pDescription;
		}
		else
		{
			LineString.Copy (' ', IndentSize);

			bool HasValue = (pSwitchInfo->m_Switch & ECmdLineSwitchFlag_HasValue) != 0;

			ASSERT (pSwitchInfo->m_NameTable [0]);
			if (pSwitchInfo->m_NameTable [0] [1])
			{
				LineString += "--";
				LineString += pSwitchInfo->m_NameTable [0];
			}
			else
			{
				LineString += '-';
				LineString += pSwitchInfo->m_NameTable [0] [0];
			}

			for (size_t i = 1; i < countof (pSwitchInfo->m_NameTable); i++)
			{
				if (!pSwitchInfo->m_NameTable [i])
					break;

				LineString += ", ";

				if (pSwitchInfo->m_NameTable [i] [1])
				{
					LineString += "--";
					LineString += pSwitchInfo->m_NameTable [i];
				}
				else
				{
					LineString += '-';
					LineString += pSwitchInfo->m_NameTable [i] [0];
				}
			}

			if (HasValue)
			{
				LineString += '=';
				LineString += pSwitchInfo->m_pValue;
			}

			size_t Length = LineString.GetLength ();
			if (Length < DescriptionCol)
			{
				LineString.Append (' ', DescriptionCol - Length);
			}
			else
			{
				LineString.AppendNewLine ();
				LineString.Append (' ', DescriptionCol);
			}

			LineString += pSwitchInfo->m_pDescription;
		}

		String += LineString;
		String.AppendNewLine ();
	}

	return String;
}

//.............................................................................

} // namespace rtl
} // namespace axl
