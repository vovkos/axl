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
	sl::String* string,
	const char* p, 
	size_t length
	)
{
	if (length == -1)
		length = axl_strlen (p);

	string->clear ();
	string->reserve (length);

	char escapeSequence [4] = { '\\' };

	const char* base = p;
	const char* end = p + length;
	for (; p < end; p++)
	{
		if (*p == '\\')
		{
			string->append (base, p - base);

			escapeSequence [1] = '\\';
			string->append (escapeSequence, 2);

			base = p + 1;
		}
		else
		{
			if (isprint (*p))
				continue;

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
	}

	string->append (base, p - base);

	return string->getLength ();
}

static 
inline
bool 
isHexChar (char c)
{
	return 
		c >= '0' && c <= '9' ||
		c >= 'a' && c <= 'f' ||
		c >= 'A' && c <= 'F';
}

size_t
EscapeEncoding::decode (
	sl::String* string,
	const char* p, 
	size_t length
	)
{
	enum State
	{
		State_Normal = 0,
		State_Escape,
		State_Hex,
	};

	State state = State_Normal;	

	if (length == -1)
		length = axl_strlen (p);

	string->clear ();
	string->reserve (length / 2);

	char hexCodeString [16];
	size_t hexCodeLen;
	size_t hexCodeMaxLen;
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
			switch (*p)
			{
			case 'x':
				state = State_Hex;
				hexCodeLen = 0;
				hexCodeMaxLen = 2;
				break;

			case 'u':
				state = State_Hex;
				hexCodeLen = 0;
				hexCodeMaxLen = 4;
				break;

			case 'U':
				state = State_Hex;
				hexCodeLen = 0;
				hexCodeMaxLen = 8;
				break;

			default:
				replace = findEscapeReplaceChar (*p);
				if (replace != *p)
				{
					string->append (replace, 1);
					base = p + 1;
				}
				else
				{
					base = p;
				}

				state = State_Normal;
			}

			break;
		
		case State_Hex:
			if (isHexChar (*p))
			{
				hexCodeString [hexCodeLen++] = *p;
				if (hexCodeLen < hexCodeMaxLen)
					break;

				base = p + 1;
			}
			else
			{
				base = p;
			}

			if (!hexCodeLen)
			{
				hexCode = '?';
				string->append ((char const*) &hexCode, 1);
			}
			else 
			{
				hexCodeString [hexCodeLen] = 0;
				hexCode = strtoul (hexCodeString, NULL, 16);				

				if (hexCodeMaxLen == 2) // \x
				{
					string->append ((char const*) &hexCode, 1);
				}
				else // \u
				{
					utf8_t buffer [8];
					size_t length = enc::Utf8::getEncodeCodePointLength (hexCode);
					enc::Utf8::encodeCodePoint (buffer, hexCode);
					string->append (buffer, length);
				}
			}

			state = State_Normal;
			break;
		}
	}

	string->append (base, p - base);
	return string->getLength ();
}

//.............................................................................

} // namespace enc
} // namespace axl
