#include "pch.h"
#include "axl_sl_CmdLineParser.h"
#include "axl_err_Error.h"

namespace axl {
namespace sl {

//.............................................................................

size_t
CmdLineParserRoot::extractArg (
	const sl::StringRef& cmdLine,
	sl::String* arg
	)
{
	const char* p = cmdLine;
	const char* end = cmdLine.getEnd ();

	while (p < end && isspace (*p))
		p++;

	if (p >= end)
	{
		arg->clear ();
		return p - cmdLine;
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
			err::setError ("unterminated escape sequence");
			return -1;
		}
	}

	arg->copy (p1, p - p1);
	return p - cmdLine;
}

bool
CmdLineParserRoot::parseSwitch (
	ArgKind argKind,
	const sl::StringRef& arg,
	sl::String* switchName,
	sl::String* value
	)
{
	const char* p = arg;
	const char* end = arg.getEnd ();

	if (argKind == ArgKind_CharSwitch)
	{
		switchName->copy (*p);
		p++;
	}
	else
	{
		const char* p0 = p;
		while (p < end && *p != '=' && !isspace (*p))
			p++;

		switchName->copy (p0, p - p0);
	}

	if (*p && !isspace (*p)) // has value
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
		if (!switchInfo->m_switchKind) // group
			continue;

		size_t switchLength = 0;

		size_t i = 0;
		for (; i < countof (switchInfo->m_nameTable); i++)
		{
			if (!switchInfo->m_nameTable [i])
				break;

			if (switchInfo->m_nameTable [i] [1])
			{
				switchLength += 2; // "--"
				switchLength += strlen_s (switchInfo->m_nameTable [i]);
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

		if (switchInfo->m_value)
		{
			switchLength++; // '='
			switchLength += strlen_s (switchInfo->m_value);
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
		if (!switchInfo->m_switchKind) // group
		{
			if (!string.isEmpty ())
				string.appendNewLine ();

			lineString = switchInfo->m_description;
		}
		else
		{
			lineString.copy (' ', indentSize);

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

			if (switchInfo->m_value)
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

} // namespace sl
} // namespace axl
