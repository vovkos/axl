#include "pch.h"
#include "axl_rtl_HexEncoding.h"

namespace axl {
namespace rtl {

//.............................................................................

size_t
CHexEncoding::Encode (
	CString* pString,
	const void* p, 
	size_t Size,
	uint_t Flags
	)
{
	if (!Size)
		return 0;

	const uchar_t* pSrc = (const uchar_t*) p;
	const uchar_t* pSrcEnd = pSrc + Size;

	size_t Length;
	if ((Flags & EHexEncode_NoSpace))
	{
		Length = Size * 2;
		char* pDst = pString->GetBuffer (Length, false);
		if (!pDst)
			return -1;

		if (Flags & EHexEncode_UpperCase)
			Encode_nsu (pDst, pSrc, pSrcEnd);
		else
			Encode_nsl (pDst, pSrc, pSrcEnd);
	}
	else
	{
		Length = Size * 3 - 1;
		char* pDst = pString->GetBuffer (Length, false);
		if (!pDst)
			return -1;

		if (Flags & EHexEncode_UpperCase)
			Encode_u (pDst, pSrc, pSrcEnd);
		else
			Encode_l (pDst, pSrc, pSrcEnd);
	}	

	return Length;
}

void
CHexEncoding::Encode_l (
	char* pDst,
	const uchar_t* pSrc, 
	const uchar_t* pSrcEnd
	)
{
	uchar_t x = *pSrc;
	*pDst++ = GetHexChar_l (x >> 4);
	*pDst++ = GetHexChar_l (x);

	for (pSrc++; pSrc < pSrcEnd; pSrc++)
	{
		uchar_t x = *pSrc;

		*pDst++ = ' ';
		*pDst++ = GetHexChar_l (x >> 4);
		*pDst++ = GetHexChar_l (x);
	}
}

void
CHexEncoding::Encode_u (
	char* pDst,
	const uchar_t* pSrc, 
	const uchar_t* pSrcEnd
	)
{
	uchar_t x = *pSrc;
	*pDst++ = GetHexChar_u (x >> 4);
	*pDst++ = GetHexChar_u (x);

	for (pSrc++; pSrc < pSrcEnd; pSrc++)
	{
		uchar_t x = *pSrc;

		*pDst++ = ' ';
		*pDst++ = GetHexChar_u (x >> 4);
		*pDst++ = GetHexChar_u (x);
	}
}

void
CHexEncoding::Encode_nsl (
	char* pDst,
	const uchar_t* pSrc, 
	const uchar_t* pSrcEnd
	)
{
	for (; pSrc < pSrcEnd; pSrc++)
	{
		uchar_t x = *pSrc;

		*pDst++ = GetHexChar_l (x >> 4);
		*pDst++ = GetHexChar_l (x);
	}
}

void
CHexEncoding::Encode_nsu (
	char* pDst,
	const uchar_t* pSrc, 
	const uchar_t* pSrcEnd
	)
{
	for (; pSrc < pSrcEnd; pSrc++)
	{
		uchar_t x = *pSrc;

		*pDst++ = GetHexChar_u (x >> 4);
		*pDst++ = GetHexChar_u (x);
	}
}

size_t
CHexEncoding::Decode (
	CArrayT <uchar_t>* pBuffer,
	const char* p, 
	size_t Length
	)
{
	enum EState
	{
		EState_Normal = 0,
		EState_Hex
	};

	EState State = EState_Normal;	

	if (Length == -1)
		Length = CStringDetails::CalcLength (p);

	pBuffer->Reserve (Length / 2);

	char HexCodeString [4] = { 0 };
	char* pHexCodeEnd;
	size_t HexCodeLen;

	uchar_t x;

	const char* pEnd = p + Length;
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
			x = (uchar_t) strtoul (HexCodeString, &pHexCodeEnd, 16);

			if (pHexCodeEnd == &HexCodeString [HexCodeLen])
				pBuffer->Append (x);
			else
				p = pEnd; // not a hex string anymore, break the loop

			State = EState_Normal;
			break;
		}
	}

	if (State == EState_Hex)
	{
		HexCodeString [HexCodeLen] = 0;
		x = (uchar_t) strtoul (HexCodeString, &pHexCodeEnd, 16);

		if (pHexCodeEnd == &HexCodeString [HexCodeLen])
			pBuffer->Append (x);
	}

	return pBuffer->GetCount ();
}

//.............................................................................

} // namespace err
} // namespace axl
