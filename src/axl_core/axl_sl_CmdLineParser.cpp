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
#include "axl_sl_CmdLineParser.h"
#include "axl_enc_EscapeEncoding.h"
#include "axl_err_Error.h"

namespace axl {
namespace sl {

//..............................................................................

size_t
CmdLineParserRoot::extractArg(
	sl::String* arg,
	const sl::StringRef& cmdLine
	)
{
	const char* p = cmdLine.cp();
	const char* end = cmdLine.getEnd();

	while (p < end && isspace(*p))
		p++;

	if (p >= end)
	{
		arg->clear();
		return p - cmdLine.cp();
	}

#if (_AXL_OS_WIN) // only supports escaped quotation marks
	arg->clear();

	if (*p != '\"')
	{
		const char* p0 = p;

		while (p < end && !isspace(*p))
			if (*p++ == '\\' && p < end && *p == '\"')
			{
				arg->append(p0, p - p0 - 1);
				p0 = p;
				p++;
			}

		arg->append(p0, p - p0);
	}
	else
	{
		p++;
		const char* p0 = p;

		while (p < end && *p != '\"')
			if (*p++ == '\\' && p < end && *p == '\"')
			{
				arg->append(p0, p - p0 - 1);
				p0 = p;
				p++;
			}

		arg->append(p0, p - p0);

		if (p < end)
			p++; // skip trailing quotation mark
	}
#else
	if (*p != '\"')
	{
		const char* p0 = p;

		for (; p < end && !isspace(*p); p++)
			if (*p == '\\')
				p++;

		if (p > end) // unterminated escape
			p = end;

		enc::EscapeEncoding::decode(arg, sl::StringRef(p0, p - p0));
	}
	else
	{
		p++;
		const char* p0 = p;

		for (; p < end && *p != '\"'; p++)
			if (*p == '\\')
				p++;

		if (p > end) // unterminated escape
			p = end;

		enc::EscapeEncoding::decode(arg, sl::StringRef(p0, p - p0));

		if (p < end)
			p++; // skip trailing quotation mark
	}
#endif

	return p - cmdLine.cp();
}

bool
CmdLineParserRoot::parseSwitch(
	ArgKind argKind,
	const sl::StringRef& arg,
	sl::String* switchName,
	sl::String* value
	)
{
	const char* p = arg.cp();
	const char* end = arg.getEnd();

	if (argKind == ArgKind_CharSwitch)
	{
		switchName->copy(*p);
		p++;
	}
	else
	{
		const char* p0 = p;
		while (p < end && *p != '=' && !isspace(*p))
			p++;

		switchName->copy(p0, p - p0);
	}

	if (p < end && !isspace(*p)) // has value
	{
		if (*p == '=')
			p++;

		value->copy(p, end - p);
	}
	else
	{
		value->clear();
	}

	return true;
}

//..............................................................................

String
getCmdLineHelpString(const ConstList<SwitchInfo>& switchInfoList)
{
	enum
	{
		indentSize        = 2,
		gapSize           = 2,
		switchLengthLimit = 40,
	};

	// calculate max switch length

	size_t maxSwitchLength = 0;

	ConstIterator<SwitchInfo> it = switchInfoList.getHead();
	for (; it; it++)
	{
		const SwitchInfo* switchInfo = *it;
		if (!switchInfo->m_switchKind) // group
			continue;

		size_t switchLength = 0;

		size_t i = 0;
		for (; i < countof(switchInfo->m_nameTable); i++)
		{
			if (!switchInfo->m_nameTable[i])
				break;

			if (switchInfo->m_nameTable[i][1])
			{
				switchLength += 2; // "--"
				switchLength += strlen_s(switchInfo->m_nameTable[i]);
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
			switchLength += strlen_s(switchInfo->m_value);
		}

		if (switchLength < switchLengthLimit && maxSwitchLength < switchLength)
			maxSwitchLength = switchLength;
	}

	size_t descriptionCol = maxSwitchLength + indentSize + gapSize;

	// generate string

	String string;
	String lineString;

	it = switchInfoList.getHead();
	for (; it; it++)
	{
		const SwitchInfo* switchInfo = *it;
		if (!switchInfo->m_switchKind) // group
		{
			if (!string.isEmpty())
				string.appendNewLine();

			lineString = switchInfo->m_description;
		}
		else
		{
			lineString.copy(' ', indentSize);

			ASSERT(switchInfo->m_nameTable[0]);
			if (switchInfo->m_nameTable[0][1])
			{
				lineString += "--";
				lineString += switchInfo->m_nameTable[0];
			}
			else
			{
				lineString += '-';
				lineString += switchInfo->m_nameTable[0][0];
			}

			for (size_t i = 1; i < countof(switchInfo->m_nameTable); i++)
			{
				if (!switchInfo->m_nameTable[i])
					break;

				lineString += ", ";

				if (switchInfo->m_nameTable[i][1])
				{
					lineString += "--";
					lineString += switchInfo->m_nameTable[i];
				}
				else
				{
					lineString += '-';
					lineString += switchInfo->m_nameTable[i][0];
				}
			}

			if (switchInfo->m_value)
			{
				lineString += '=';
				lineString += switchInfo->m_value;
			}

			size_t length = lineString.getLength();
			if (length < descriptionCol)
			{
				lineString.append(' ', descriptionCol - length);
			}
			else
			{
				lineString.appendNewLine();
				lineString.append(' ', descriptionCol);
			}

			lineString += switchInfo->m_description;
		}

		string += lineString;
		string.appendNewLine();
	}

	return string;
}

//..............................................................................

} // namespace sl
} // namespace axl
