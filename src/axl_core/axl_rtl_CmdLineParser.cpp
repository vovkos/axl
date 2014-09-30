#include "pch.h"
#include "axl_rtl_CmdLineParser.h"
#include "axl_err_Error.h"

namespace axl {
namespace rtl {

//.............................................................................

size_t
CmdLineParserRoot::extractArg (
	const char* p,
	const char* end,
	rtl::String* arg
	)
{
	const char* p0 = p;

	while (p < end && isspace (*p))
		p++;

	if (p >= end)
	{
		arg->clear ();
		return p - p0;
	}

	const char* p1 = p;

	if (*p != '\"')
	{
		while (p < end && !isspace (*p))
			p++;
	}
	else
	{
		p1++;

		while (p < end)
		{
			if (*p == '\"')
				break;
			else if (*p == '\\')
				p++;

			p++;
		}

		if (p > end)
		{
			err::setStringError ("unterminated escape sequence");
			return -1;
		}
	}

	arg->copy (p1, p - p1);
	return p - p0;
}

bool
CmdLineParserRoot::parseArg (
	const char* p,
	rtl::String* switchName,
	rtl::String* value
	)
{
	if (*p != '-')
	{
		switchName->clear ();
		value->copy (p);
		return true;
	}

	p++;

	if (*p != '-')
	{
		if (!isalpha (*p))
		{
			err::setStringError ("invalid command line switch syntax");
			return false;
		}

		switchName->copy (*p);
		p++;
	}
	else
	{
		p++;

		if (!isalpha (*p))
		{
			err::setStringError ("invalid command line switch syntax");
			return false;
		}

		const char* p1 = p;

		while (*p && *p != '=' && !isspace (*p))
			p++;

		switchName->copy (p1, p - p1);
	}

	if (*p && !isspace (*p)) // have value
	{
		if (*p == '=')
			p++;

		const char* p1 = p;

		while (*p && !isspace (*p))
			p++;

		value->copy (p1, p - p1);
	}
	else
	{
		value->clear ();
	}

	return true;
}

//.............................................................................

String
getCmdLineHelpString (const ConstList <SwitchInfo>& switchInfoList)
{
	enum
	{
		indentSize        = 2,
		gapSize           = 2,
		switchLengthLimit = 40,
	};

	// calculate max switch length

	size_t maxSwitchLength = 0;

	Iterator <SwitchInfo> it = switchInfoList.getHead ();
	for (; it; it++)
	{
		SwitchInfo* switchInfo = *it;
		if (!switchInfo->m_switch) // group
			continue;

		size_t switchLength = 0;

		bool hasValue = (switchInfo->m_switch & CmdLineSwitchFlagKind_HasValue) != 0;

		size_t i = 0;
		for (; i < countof (switchInfo->m_nameTable); i++)
		{
			if (!switchInfo->m_nameTable [i])
				break;

			if (switchInfo->m_nameTable [i] [1])
			{
				switchLength += 2; // "--"
				switchLength += strlen (switchInfo->m_nameTable [i]);
			}
			else
			{
				switchLength += 2; // "-c"
			}
		}

		if (i > 1)
		{
			switchLength += (i - 1) * 2; // ", "
		}

		if (hasValue)
		{
			switchLength++; // '='
			switchLength += strlen (switchInfo->m_value);
		}

		if (switchLength < switchLengthLimit && maxSwitchLength < switchLength)
			maxSwitchLength = switchLength;
	}

	size_t descriptionCol = maxSwitchLength + indentSize + gapSize;

	// generate string

	String string;
	String lineString;

	it = switchInfoList.getHead ();
	for (; it; it++)
	{
		SwitchInfo* switchInfo = *it;
		if (!switchInfo->m_switch) // group
		{
			if (!string.isEmpty ())
				string.appendNewLine ();

			lineString = switchInfo->m_description;
		}
		else
		{
			lineString.copy (' ', indentSize);

			bool hasValue = (switchInfo->m_switch & CmdLineSwitchFlagKind_HasValue) != 0;

			ASSERT (switchInfo->m_nameTable [0]);
			if (switchInfo->m_nameTable [0] [1])
			{
				lineString += "--";
				lineString += switchInfo->m_nameTable [0];
			}
			else
			{
				lineString += '-';
				lineString += switchInfo->m_nameTable [0] [0];
			}

			for (size_t i = 1; i < countof (switchInfo->m_nameTable); i++)
			{
				if (!switchInfo->m_nameTable [i])
					break;

				lineString += ", ";

				if (switchInfo->m_nameTable [i] [1])
				{
					lineString += "--";
					lineString += switchInfo->m_nameTable [i];
				}
				else
				{
					lineString += '-';
					lineString += switchInfo->m_nameTable [i] [0];
				}
			}

			if (hasValue)
			{
				lineString += '=';
				lineString += switchInfo->m_value;
			}

			size_t length = lineString.getLength ();
			if (length < descriptionCol)
			{
				lineString.append (' ', descriptionCol - length);
			}
			else
			{
				lineString.appendNewLine ();
				lineString.append (' ', descriptionCol);
			}

			lineString += switchInfo->m_description;
		}

		string += lineString;
		string.appendNewLine ();
	}

	return string;
}

//.............................................................................

} // namespace rtl
} // namespace axl
