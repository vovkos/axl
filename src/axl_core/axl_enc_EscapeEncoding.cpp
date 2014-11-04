#include "pch.h"
#include "axl_enc_EscapeEncoding.h"
#include "axl_enc_HexEncoding.h"

namespace axl {
namespace enc {

//.............................................................................

char
EscapeEncoding::findEscapeChar (char x)
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
EscapeEncoding::findEscapeReplaceChar (char x)
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
EscapeEncoding::encode (
	rtl::String* string,
	const char* p, 
	size_t length
	)
{

	if (length == -1)
		length = rtl::StringDetails::calcLength (p);

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
			escapeSequence [2] = HexEncoding::getHexChar_l (*p >> 4);
			escapeSequence [3] = HexEncoding::getHexChar_l (*p);
			string->append (escapeSequence, 4);
		}

		base = p + 1;
	}

	string->append (base, p - base);

	return string->getLength ();
}

size_t
EscapeEncoding::decode (
	rtl::String* string,
	const char* p, 
	size_t length
	)
{
	enum State
	{
		State_Normal = 0,
		State_Escape,
		State_Hex
	};

	State state = State_Normal;	

	if (length == -1)
		length = rtl::StringDetails::calcLength (p);

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
		case State_Normal:
			if (*p == '\\')
			{
				string->append (base, p - base);
				state = State_Escape;
			}

			break;

		case State_Escape:
			if (*p == 'x')
			{
				state = State_Hex;
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

			state = State_Normal;
			break;
		
		case State_Hex:			
			hexCodeString [hexCodeLen++] = *p;
			if (hexCodeLen < 2)
				break;

			hexCode = strtoul (hexCodeString, &hexCodeEnd, 16);
			if (hexCodeEnd != &hexCodeString [hexCodeLen])
				hexCode = '?';

			string->append ((char const*) &hexCode, 1);
			state = State_Normal;
			base = p + 1;
			break;
		}
	}

	string->append (base, p - base);
	return string->getLength ();
}

//.............................................................................

} // namespace enc
} // namespace axl
