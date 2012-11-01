#include "stdafx.h"
#include "axl_rtl_HexEncode.h"

//.............................................................................

size_t 
AXL_API
axl_rtl_AppendEncodeHexString(
	axl_rtl_TString* pString,
	const void* _p, 
	size_t Size,
	int Flags
	)
{
	static tchar_t _HexCharsUpper[] = _T("0123456789ABCDEF");
	static tchar_t _HexCharsLower[] = _T("0123456789abcdef");

	tchar_t* pHexChars = (Flags & axl_rtl_EBinHexEncode_Lowercase) ? _HexCharsLower : _HexCharsUpper;

	uchar_t* p = (uchar_t*) _p;
	uchar_t* pEnd = p + Size;
	tchar_t* pBuffer;

	size_t OldLength = axl_rtl_TString_GetLength(pString);

	if (!Size)
		return OldLength;

	if (Flags & axl_rtl_EBinHexEncode_NoSpace)
	{
		size_t NewLength = OldLength + Size * 2;
		pBuffer = axl_rtl_TString_GetBuffer(pString, NewLength, true);
		pBuffer += OldLength;

		for (; p < pEnd; p++)
		{
			uchar_t x = *p;

			*pBuffer++ = pHexChars[(x & 0xf0) >> 4];
			*pBuffer++ = pHexChars[(x & 0x0f)];
		}
	}
	else
	{
		size_t NewLength = OldLength + Size * 3 - 1;
		pBuffer = axl_rtl_TString_GetBuffer(pString, NewLength + 1, true);
		pBuffer += OldLength;

		for (; p < pEnd; p++)
		{
			uchar_t x = *p;

			*pBuffer++ = pHexChars[(x & 0xf0) >> 4];
			*pBuffer++ = pHexChars[(x & 0x0f)];
			*pBuffer++ = _T(' ');
		}

		axl_rtl_TString_ReduceLength(pString, 1);
	}	

	return axl_rtl_TString_GetLength(pString);
}

size_t
AXL_API
axl_rtl_AppendDecodeHexString(
	axl_rtl_TByteArray* pData,
	const tchar_t* p,
	size_t Length
	)
{
	enum
	{
		EState_Normal = 0,
		EState_Hex
	};

	int State = EState_Normal;	

	tchar_t CodeString[4] = {0};
	size_t CodeLen;
	size_t i;

	tchar_t* pEnd;
	long x;

	for (i = 0; i < Length && *p; p++, i++)
	{
		bool_t IsSpace = isspace(*p);

		switch (State)
		{
		case EState_Normal:
			if (IsSpace)
				break;

			CodeLen = 0;
			CodeString[CodeLen++] = *p;
			State = EState_Hex;
			break;

		case EState_Hex:	
			if (!IsSpace)
			{
				CodeString[CodeLen++] = *p;
				if (CodeLen < 2)
					break;
			}

			CodeString[CodeLen] = 0;
			x = _tcstol(CodeString, &pEnd, 16);

			if (pEnd == &CodeString[CodeLen])
				axl_rtl_TByteArray_Append(pData, &x, sizeof(uchar_t));
			else
				i = Length; // not a hex string anymore, break the loop

			State = EState_Normal;
			break;
		}
	}

	if (State == EState_Hex)
	{
		CodeString[CodeLen] = 0;
		x = _tcstol(CodeString, &pEnd, 16);

		if (pEnd == &CodeString[CodeLen])
			axl_rtl_TByteArray_Append(pData, &x, sizeof(uchar_t));
	}

	return axl_rtl_TByteArray_GetCount(pData);
}

//..................................................................................
