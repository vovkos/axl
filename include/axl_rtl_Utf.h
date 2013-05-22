// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_UTF_H

#include "axl_rtl_ByteOrder.h"
#include "axl_err_Error.h"

namespace axl {
namespace rtl {

//.............................................................................

enum EUtf
{
	EUtf_Utf8 = 0,
	EUtf_Utf16,
	EUtf_Utf16_be,
	EUtf_Utf32,
	EUtf_Utf32_be,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetUtfKindString (EUtf UtfKind);

//.............................................................................

class CUtf8
{
public:
	typedef utf8_t C;

	static
	EUtf 
	GetUtfKind ()
	{
		return EUtf_Utf8;
	}

	static
	const uint8_t*
	GetBom ()
	{
		static uint8_t Bom [] = { 0xef, 0xbb, 0xbf };
		return Bom; 
	}

	static	
	size_t
	GetBomLength ()
	{
		return 3;
	}

	static
	size_t
	GetDecodeCodePointLength (C c)
	{
		size_t Length = 
			(c & 0x80) == 0    ? 1 :     // 0xxxxxxx
			(c & 0xe0) == 0xc0 ? 2 :     // 110xxxxx 10xxxxxx 
			(c & 0xf0) == 0xe0 ? 3 :     // 1110xxxx 10xxxxxx 10xxxxxx
			(c & 0xf8) == 0xf0 ? 4 : -1; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

		return err::CompleteWithSystemError (Length, (size_t) -1, err::EStatus_InvalidParameter);
	}

	static
	size_t
	GetEncodeCodePointLength (utf32_t x)
	{
		size_t Length = 
			x < 0x80 ? 1 :               // 0xxxxxxx
			x < 0x800 ? 2 :              // 110xxxxx 10xxxxxx 
			x < 0x10000 ? 3 :            // 1110xxxx 10xxxxxx 10xxxxxx
			x < 0x200000 ? 4 : -1;       // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

		return err::CompleteWithSystemError (Length, (size_t) -1, err::EStatus_InvalidParameter);
	}	

	static
	utf32_t
	DecodeCodePoint (const C* p)
	{
		utf32_t x = 
			((*p & 0x80) == 0)    ?      // 0xxxxxxx
				*p :         
			((*p & 0xe0) == 0xc0) ?      // 110xxxxx 10xxxxxx 
				((p [0] & 0x1f) << 6)  | 
				 (p [1] & 0x3f) :
			((*p & 0xf0) == 0xe0) ?      // 1110xxxx 10xxxxxx 10xxxxxx
				((p [0] & 0x0f) << 12) | 
				((p [1] & 0x3f) << 6)  |
				 (p [2] & 0x3f) :
			((*p & 0xf8) == 0xf0) ?      // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
				((p [0] & 0x07) << 18) | 
				((p [1] & 0x3f) << 12) |
				((p [2] & 0x3f) << 6)  |
				 (p [3] & 0x3f) : -1;

		return err::CompleteWithSystemError (x, (utf32_t) -1, err::EStatus_InvalidParameter);
	}

	static
	bool
	EncodeCodePoint (
		C* p,
		utf32_t x
		)
	{
		if (x < 0x80)                    // 0xxxxxxx
		{		
			p [0] = (uint8_t) x;  
		}
		else if (x < 0x800)              // 110xxxxx 10xxxxxx 
		{                
			p [0] = (uint8_t) (x >> 6)          | 0xc0;
			p [1] = (uint8_t) (x & 0x3f)        | 0x80;
		}
		else if (x < 0x10000)            // 1110xxxx 10xxxxxx 10xxxxxx
		{
			p [0] = (uint8_t) (x >> 12)         | 0xe0;
			p [1] = (uint8_t) ((x >> 6) & 0x3f) | 0x80;
			p [2] = (uint8_t) (x & 0x3f)        | 0x80;
		}
		else if (x < 0x200000)           // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		{
			p [0] = (uint8_t) (x >> 18)         | 0xf0;
			p [1] = (uint8_t) ((x >> 12) & 0x3f)| 0x80;
			p [2] = (uint8_t) ((x >> 6) & 0x3f) | 0x80;
			p [3] = (uint8_t) (x & 0x3f)        | 0x80;
		}
		else
		{
			return err::Fail (err::EStatus_InvalidParameter);
		}

		return true;
	}
};

//.............................................................................
		
enum EUtf16Surrogate
{
	EUtf16Surrogate_MinLead  = 0xd800,
	EUtf16Surrogate_MaxLead  = 0xdbff,
	EUtf16Surrogate_MinTrail = 0xdc00,
	EUtf16Surrogate_MaxTrail = 0xdfff,
};

class CUtf16
{
public:
	typedef utf16_t C;

	static
	EUtf 
	GetUtfKind ()
	{
		return EUtf_Utf16;
	}

	static
	const uint8_t*
	GetBom ()
	{
		static uint8_t Bom [] = { 0xff, 0xfe };
		return Bom; 
	}

	static	
	size_t
	GetBomLength ()
	{
		return 2;
	}

	static	
	bool
	IsLeadSurrogate (uint16_t c)
	{
		return c >= 0xd800 && c <= 0xdbff;
	}

	static	
	bool
	IsTrailSurrogate (uint16_t c)
	{
		return c >= 0xdc00 && c <= 0xdfff;
	}

	static	
	utf32_t
	GetSurrogateCodePoint (
		uint16_t Lead,
		uint16_t Trail
		)
	{
		return (Lead - 0xd800) * 0x400 + (Trail - 0xdc00) + 0x10000;
	}	

	static	
	bool
	NeedSurrogate (uint32_t x)
	{
		return x > 0xffff;
	}

	static
	utf16_t
	GetLeadSurrogate (uint32_t x)
	{
		return (x - 0x10000) / 0x400 + 0xd800;
	}

	static
	utf16_t
	GetTrailSurrogate (uint32_t x)
	{
		return (x - 0x10000) % 0x400 + 0xdc00;
	}

	static
	size_t
	GetDecodeCodePointLength (C c)
	{
		return IsLeadSurrogate (c) ? 2 : 1;
	}

	static
	size_t
	GetEncodeCodePointLength (utf32_t x)
	{
		return NeedSurrogate (x) ? 2 : 1;
	}

	static
	utf32_t
	DecodeCodePoint (const C* p)
	{
		return IsLeadSurrogate (*p) ? GetSurrogateCodePoint (p [0], p [1]) : (uint16_t) *p;
	}

	static
	bool
	EncodeCodePoint (
		C* p,
		utf32_t x
		)
	{
		if (!NeedSurrogate (x))
		{
			*p = (C) x;
		}
		else
		{
			p [0] = GetLeadSurrogate (x);
			p [1] = GetTrailSurrogate (x);
		}

		return true;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUtf16_be: public CUtf16
{
public:
	static
	EUtf 
	GetUtfKind ()
	{
		return EUtf_Utf16_be;
	}

	static
	const uint8_t*
	GetBom ()
	{
		static uint8_t Bom [] = { 0xfe, 0xff };
		return Bom; 
	}

	static
	size_t
	GetDecodeCodePointLength (C c)
	{
		return GetTrailSurrogate (c) ? 2 : 1;
	}

	static
	utf32_t
	DecodeCodePoint (const C* p)
	{
		return IsTrailSurrogate (*p) ? GetSurrogateCodePoint (p [1], p [0]) : (uint16_t) *p;
	}

	static
	bool
	EncodeCodePoint (
		C* p,
		utf32_t x
		)
	{
		if (!NeedSurrogate (x))
		{
			*p = (C) x;
		}
		else
		{
			p [0] = GetTrailSurrogate (x);
			p [1] = GetLeadSurrogate (x);
		}

		return true;
	}
};

//.............................................................................

class CUtf32
{
public:
	typedef utf32_t C;

	static
	EUtf 
	GetUtfKind ()
	{
		return EUtf_Utf32;
	}

	static
	const uint8_t*
	GetBom ()
	{
		static uint8_t Bom [] = { 0xff, 0xfe, 0x00, 0x00 };
		return Bom; 
	}

	static	
	size_t
	GetBomLength ()
	{
		return 4;
	}

	static
	size_t
	GetDecodeCodePointLength (C c)
	{
		return 1;
	}

	static
	size_t
	GetEncodeCodePointLength (utf32_t x)
	{
		return 1;
	}

	static
	utf32_t
	DecodeCodePoint (const C* p)
	{
		return *p;
	}

	static
	bool
	EncodeCodePoint (
		C* p,
		utf32_t x
		)
	{
		*p = x;
		return true;
	}
};


//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CUtf32_be: public CUtf32
{
public:
	static
	EUtf 
	GetUtfKind ()
	{
		return EUtf_Utf32_be;
	}

	static
	const uint8_t*
	GetBom ()
	{
		static uint8_t Bom [] = { 0x00, 0x00, 0xfe, 0xff };
		return Bom; 
	}

	static
	utf32_t
	DecodeCodePoint (const C* p)
	{
		return rtl::SwapByteOrder32 (*p);
	}

	static
	bool
	EncodeCodePoint (
		C* p,
		utf32_t x
		)
	{
		*p = rtl::SwapByteOrder32 (x);
		return true;
	}
};

//.............................................................................

template <
	typename TDstEncoding,
	typename TSrcEncoding
	>
class CUtfConvertT 
{
public:
	typedef TDstEncoding CDstEncoding;
	typedef TSrcEncoding CSrcEncoding;
	typedef typename CDstEncoding::C CDstUnit;
	typedef typename CSrcEncoding::C CSrcUnit;

public:
	static
	size_t
	CalcRequiredLength (
		const CSrcUnit* p,
		size_t Length
		)
	{
		const CSrcUnit* pEnd = p + Length;

		size_t ResultLength = 0;
		while (p < pEnd)
		{
			size_t SrcCodePointLength = CSrcEncoding::GetDecodeCodePointLength (*p);
			if (SrcCodePointLength == -1)
				return -1;

			if (p + SrcCodePointLength > pEnd)
				break;

			utf32_t x = CSrcEncoding::DecodeCodePoint (p);		

			size_t DstCodePointLength = CDstEncoding::GetEncodeCodePointLength (x);
			if (DstCodePointLength == -1)
				return -1;
			
			p += SrcCodePointLength;
			ResultLength += DstCodePointLength;
		}

		return ResultLength;
	}

	static
	size_t 
	Convert (
		CDstUnit* pDst,
		size_t DstLength,
		const CSrcUnit* pSrc,
		size_t SrcLength
		) // returns number of dst units written
	{
		CDstUnit* pDstEnd = pDst + DstLength;
		const CSrcUnit* pSrcEnd = pSrc + SrcLength;

		size_t ResultLength = 0;
		while (pSrc < pSrcEnd)
		{
			size_t SrcCodePointLength = CSrcEncoding::GetDecodeCodePointLength (*pSrc);
			if (SrcCodePointLength == -1)
				return -1;

			if (pSrc + SrcCodePointLength > pSrcEnd)
				break;

			utf32_t x = CSrcEncoding::DecodeCodePoint (pSrc);

			size_t DstCodePointLength = CDstEncoding::GetEncodeCodePointLength (x);
			if (DstCodePointLength == -1)
				break;

			if (pDst + DstCodePointLength > pDstEnd)
				return -1;

			CDstEncoding::EncodeCodePoint (pDst, x);

			pSrc += SrcCodePointLength;
			pDst += DstCodePointLength;
			ResultLength += DstCodePointLength;
		}

		return ResultLength;
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
