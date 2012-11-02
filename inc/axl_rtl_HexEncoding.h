// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_HEXENCODING_H

#include "axl_rtl_String.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace rtl {

//.............................................................................

enum EHexEncode
{
	EHexEncode_UpperCase = 1,
	EHexEncode_NoSpace   = 2,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename C>
class CHexEncodingT
{
public:
	typedef CStringDetailsT <C> CStringDetails;
	typedef CStringT <C> CString;

public:
	static 
	C
	GetHexChar (
		uchar_t x,
		bool IsUpperCase = false
		)
	{
		static C _HexCharArrayLowerCase [17] = "0123456789abcdef";
		static C _HexCharArrayUpperCase [17] = "0123456789ABCDEF";
	
		return IsUpperCase ? _HexCharArrayUpperCase [x & 0xf] : _HexCharArrayLowerCase [x & 0xf];
	}

	static 
	uchar_t
	GetHexCharValue (C x)
	{
		return 
			x >= '0' && x <= '9' ? x - '0' :
			x >= 'a' && x <= 'f' ? 10 + x - 'a' :
			x >= 'A' && x <= 'F' ? 10 + x - 'A' : 0;
	}

	static
	CString
	Encode (
		const void* p, 
		size_t Size,
		int Flags = 0
		)
	{
		CString String;

		if (!Size)
			return String;

		bool IsUpperCase = (Flags & EHexEncode_UpperCase) != 0;
		bool IsNoSpace = (Flags & EHexEncode_NoSpace) != 0;

		const uchar_t* pSrc = (const uchar_t*) p;
		const uchar_t* pEnd = pSrc + Size;
		C* pDst;

		if (IsNoSpace)
		{
			size_t Length = Size * 2;
			pDst = String.GetBuffer (Length, false);

			for (; pSrc < pEnd; pSrc++)
			{
				uchar_t x = *pSrc;

				*pDst++ = GetHexChar (x >> 4, IsUpperCase);
				*pDst++ = GetHexChar (x, IsUpperCase);
			}
		}
		else
		{
			size_t Length = Size * 3 - 1;
			pDst = String.GetBuffer (Length, false);

			uchar_t x = *pSrc;
			*pDst++ = GetHexChar (x >> 4);
			*pDst++ = GetHexChar (x);

			for (pSrc++; pSrc < pEnd; pSrc++)
			{
				uchar_t x = *pSrc;

				*pDst++ = _T(' ');
				*pDst++ = GetHexChar (x >> 4, IsUpperCase);
				*pDst++ = GetHexChar (x, IsUpperCase);
			}
		}	

		return String;
	}

	static
	CArrayT <uchar_t>
	Decode (
		const C* p, 
		size_t Length = -1
		)
	{
		enum EState
		{
			EState_Normal = 0,
			EState_Hex
		};

		EState State = EState_Normal;	

		if (Length == -1)
			Length = CStringDetails::GetLength (p);

		CArrayT <uchar_t> Array;
		Array.Reserve (Length / 2);

		C HexCodeString [4] = { 0 };
		C* pHexCodeEnd;
		size_t HexCodeLen;

		uchar_t x;

		const C* pEnd = p + Length;
		for (; p < pEnd; p++)
		{
			bool_t IsSpace = isspace(*p);

			switch (State)
			{
			case EState_Normal:
				if (IsSpace)
					break;

				HexCodeLen = 0;
				HexCodeString [HexCodeLen++] = *p;
				State = EState_Hex;
				break;

			case EState_Hex:	
				if (!IsSpace)
				{
					HexCodeString [HexCodeLen++] = *p;
					if (HexCodeLen < 2)
						break;
				}

				HexCodeString [HexCodeLen] = 0;
				x = (uchar_t) CStringDetails::StrToULong (HexCodeString, &pHexCodeEnd, 16);

				if (pHexCodeEnd == &HexCodeString [HexCodeLen])
					Array.Append (x);
				else
					p = pEnd; // not a hex string anymore, break the loop

				State = EState_Normal;
				break;
			}
		}

		if (State == EState_Hex)
		{
			HexCodeString [HexCodeLen] = 0;
			x = (uchar_t) CStringDetails::StrToULong (HexCodeString, &pHexCodeEnd, 16);

			if (pHexCodeEnd == &HexCodeString [HexCodeLen])
				Array.Append (x);
		}

		return Array;
	}

	static
	CArrayT <uchar_t>
	Decode (const CString& String)
	{
		return Decode (String, String.GetLength ());
	}
};

//.............................................................................

typedef CHexEncodingT <char> CHexEncodingA;
typedef CHexEncodingT <wchar_t> CHexEncodingW;
typedef CHexEncodingT <tchar_t> CHexEncoding;

//.............................................................................

} // namespace rtl
} // namespace axl
