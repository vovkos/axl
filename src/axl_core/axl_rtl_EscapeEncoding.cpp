#include "pch.h"
#include "axl_rtl_EscapeEncoding.h"
#include "axl_rtl_HexEncoding.h"

namespace axl {
namespace rtl {

//.............................................................................

char
CEscapeEncoding::findEscapeChar (char x)
{
	switch (x)
	{
	case '\0':
		return '0';

	case '\a':
		return 'a';

	case '\b':
		return 'b';

	case '\t':
		return 't';

	case '\n':
		return 'n';

	case '\f':
		return 'f';

	case '\r':
		return 'r';

	case '\x1b':
		return 'e';

	default:
		return x;
	};
}

char
CEscapeEncoding::findEscapeReplaceChar (char x)
{
	switch (x)
	{
	case '0':
		return '\0';

	case 'a':
		return '\a';

	case 'b':
		return '\b';

	case 't':
		return '\t';

	case 'n':
		return '\n';

	case 'f':
		return '\f';

	case 'r':
		return '\r';

	case 'e':
		return '\x1b';

	default:
		return x;
	};
}

size_t
CEscapeEncoding::encode (
	CString* string,
	const char* p, 
	size_t length
	)
{

	if (length == -1)
		length = CStringDetails::calcLength (p);

	const char* base = p;
	const char* end = p + length;
	for (; p < end; p++)
	{
		if (isprint (*p))
			continue;

		char escapeSequence [4] = { '\\' };

		string->append (base, p - base);

		char escape = findEscapeChar (*p);		
		if (escape != *p)
		{
			escapeSequence [1] = escape;
			string->append (escapeSequence, 2);
		}
		else
		{
			escapeSequence [1] = 'x';
			escapeSequence [2] = CHexEncoding::getHexChar_l (*p >> 4);
			escapeSequence [3] = CHexEncoding::getHexChar_l (*p);
			string->append (escapeSequence, 4);
		}

		base = p + 1;
	}

	string->append (base, p - base);

	return string->getLength ();
}

size_t
CEscapeEncoding::decode (
	CString* string,
	const char* p, 
	size_t length
	)
{
	enum EState
	{
		EState_Normal = 0,
		EState_Escape,
		EState_Hex
	};

	EState state = EState_Normal;	

	if (length == -1)
		length = CStringDetails::calcLength (p);

	char hexCodeString [4] = { 0 };
	char* hexCodeEnd;
	size_t hexCodeLen;
	ulong_t hexCode;

	char replace;

	const char* end = p + length;
	const char* base = p;
	for (; p < end; p++)
	{
		switch (state)
		{
		case EState_Normal:
			if (*p == '\\')
			{
				string->append (base, p - base);
				state = EState_Escape;
			}

			break;

		case EState_Escape:
			if (*p == 'x')
			{
				state = EState_Hex;
				hexCodeLen = 0;
				break;
			}

			replace = findEscapeReplaceChar (*p);
			if (replace != *p)
			{
				string->append (replace);
				base = p + 1;
			}
			else
			{
				base = p;
			}

			state = EState_Normal;
			break;
		
		case EState_Hex:			
			hexCodeString [hexCodeLen++] = *p;
			if (hexCodeLen < 2)
				break;

			hexCode = strtoul (hexCodeString, &hexCodeEnd, 16);
			if (hexCodeEnd != &hexCodeString [hexCodeLen])
				hexCode = '?';

			string->append ((char const*) &hexCode, 1);
			state = EState_Normal;
			base = p + 1;
			break;
		}
	}

	string->append (base, p - base);
	return string->getLength ();
}

//.............................................................................

} // namespace err
} // namespace axl
