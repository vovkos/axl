// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_ESCAPEENCODING_H

#include "axl_rtl_HexEncoding.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename C>
class CEscapeEncodingT
{
public:
	typedef CStringDetailsT <C> CStringDetails;
	typedef CStringT <C> CString;

public:
	static
	C
	FindEscapeChar (C x)
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
		
		default:
			return x;
		};
	}

	static
	C
	FindEscapeReplaceChar (C x)
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
		
		default:
			return x;
		};
	}

	static
	CString
	Encode (
		const C* p, 
		size_t Length = -1
		)
	{
		CString String;

		if (Length == -1)
			Length = CStringDetails::GetLength (p);

		const C* pBase = p;
		const C* pEnd = p + Length;
		for (; p < pEnd; p++)
		{
			C Escape = FindEscapeChar (*p);
		
			if (Escape != *p)
			{
				C EscapeSequence [2] = { '\\', Escape };

				String.Append (pBase, p - pBase);
				String.Append (EscapeSequence, 2);
				pBase = p + 1;
			}
			else if (!isprint (*p))
			{
				uchar_t x = (uchar_t) *p;

				C EscapeSequence [8] = 
				{ 
					'\\', 'x', 
					CHexEncodingT <C>::GetHexChar (x >> 4), 
					CHexEncodingT <C>::GetHexChar (x) 
				};

				String.Append (pBase, p - pBase);
				String.Append (EscapeSequence, 4);
				pBase = p + 1;
			}
		}

		String.Append (pBase, p - pBase);
		return String;
	}

	static
	CString
	Encode (const CString& String)
	{
		return Encode (String, String.GetLength ());
	}

	static
	CString
	Decode (
		const C* p, 
		size_t Length = -1
		)
	{
		enum EState
		{
			EState_Normal = 0,
			EState_Escape,
			EState_Hex
		};

		EState State = EState_Normal;	

		CString String;

		if (Length == -1)
			Length = CStringDetails::GetLength (p);

		C HexCodeString [4] = { 0 };
		C* pHexCodeEnd;
		size_t HexCodeLen;
		long HexCode;

		C Replace;

		const C* pEnd = p + Length;
		const C* pBase = p;
		for (; p < pEnd; p++)
		{
			switch (State)
			{
			case EState_Normal:
				if (*p == '\\')
				{
					String.Append (pBase, p - pBase);
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
					String.Append (Replace);
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

				HexCode = CStringDetails::StrToULong (HexCodeString, &pHexCodeEnd, 16);
				if (pHexCodeEnd != &HexCodeString [HexCodeLen])
					HexCode = '?';

				String.Append ((C) HexCode);
				State = EState_Normal;
				pBase = p + 1;
				break;
			}
		}

		String.Append (pBase, p - pBase);
		return String;
	}

	static
	CString
	Decode (const CString& String)
	{
		return Decode (String, String.GetLength ());
	}
};

//.............................................................................

typedef CEscapeEncodingT <char> CEscapeEncodingA;
typedef CEscapeEncodingT <wchar_t> CEscapeEncodingW;
typedef CEscapeEncodingT <tchar_t> CEscapeEncoding;

//.............................................................................

} // namespace rtl
} // namespace axl

