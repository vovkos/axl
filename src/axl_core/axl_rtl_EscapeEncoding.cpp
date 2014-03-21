#include "pch.h"
#include "axl_rtl_EscapeEncoding.h"
#include "axl_rtl_HexEncoding.h"

namespace axl {
namespace rtl {

//.............................................................................

char
CEscapeEncoding::FindEscapeChar (char x)
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
CEscapeEncoding::FindEscapeReplaceChar (char x)
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
CEscapeEncoding::Encode (
	CString* pString,
	const char* p, 
	size_t Length
	)
{

	if (Length == -1)
		Length = CStringDetails::CalcLength (p);

	const char* pBase = p;
	const char* pEnd = p + Length;
	for (; p < pEnd; p++)
	{
		if (isprint (*p))
			continue;

		char EscapeSequence [4] = { '\\' };

		pString->Append (pBase, p - pBase);

		char Escape = FindEscapeChar (*p);		
		if (Escape != *p)
		{
			EscapeSequence [1] = Escape;
			pString->Append (EscapeSequence, 2);
		}
		else
		{
			EscapeSequence [1] = 'x';
			EscapeSequence [2] = CHexEncoding::GetHexChar_l (*p >> 4);
			EscapeSequence [3] = CHexEncoding::GetHexChar_l (*p);
			pString->Append (EscapeSequence, 4);
		}

		pBase = p + 1;
	}

	pString->Append (pBase, p - pBase);

	return pString->GetLength ();
}

size_t
CEscapeEncoding::Decode (
	CString* pString,
	const char* p, 
	size_t Length
	)
{
	enum EState
	{
		EState_Normal = 0,
		EState_Escape,
		EState_Hex
	};

	EState State = EState_Normal;	

	if (Length == -1)
		Length = CStringDetails::CalcLength (p);

	char HexCodeString [4] = { 0 };
	char* pHexCodeEnd;
	size_t HexCodeLen;
	ulong_t HexCode;

	char Replace;

	const char* pEnd = p + Length;
	const char* pBase = p;
	for (; p < pEnd; p++)
	{
		switch (State)
		{
		case EState_Normal:
			if (*p == '\\')
			{
				pString->Append (pBase, p - pBase);
				State = EState_Escape;
			}

			break;

		case EState_Escape:
			if (*p == 'x')
			{
				State = EState_Hex;
				HexCodeLen = 0;
				break;
			}

			Replace = FindEscapeReplaceChar (*p);
			if (Replace != *p)
			{
				pString->Append (Replace);
				pBase = p + 1;
			}
			else
			{
				pBase = p;
			}

			State = EState_Normal;
			break;
		
		case EState_Hex:			
			HexCodeString [HexCodeLen++] = *p;
			if (HexCodeLen < 2)
				break;

			HexCode = strtoul (HexCodeString, &pHexCodeEnd, 16);
			if (pHexCodeEnd != &HexCodeString [HexCodeLen])
				HexCode = '?';

			pString->Append ((char const*) &HexCode, 1);
			State = EState_Normal;
			pBase = p + 1;
			break;
		}
	}

	pString->Append (pBase, p - pBase);
	return pString->GetLength ();
}

//.............................................................................

} // namespace err
} // namespace axl
