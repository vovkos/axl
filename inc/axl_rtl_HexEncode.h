// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_rtl_String.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace rtl {

//.............................................................................

enum
{
	axl_rtl_EBinHexEncode_Lowercase = 1,
	axl_rtl_EBinHexEncode_NoSpace = 2,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// hex-encoded string

size_t
AXL_API
axl_rtl_AppendEncodeHexString(
	axl_rtl_TString* pString,
	const void* p, 
	size_t Size,
	int Flags
	);

size_t
AXL_API
axl_rtl_AppendDecodeHexString(
	axl_rtl_TByteArray* pData,
	const tchar_t* p,
	size_t Length
	);

// ascii string

size_t
AXL_API
axl_rtl_AppendEncodeAsciiString(
	axl_rtl_TString* pString,
	const void* p, 
	size_t Size,
	tchar_t UnprintCharReplace
	);

//.............................................................................

AXL_INLINE
size_t
axl_rtl_EncodeHexString(
	axl_rtl_TString* pString,
	const void* p, 
	size_t Size,
	int Flags
	)
{
	axl_rtl_TString_Clear(pString);
	return axl_rtl_AppendEncodeHexString(pString, p, Size, Flags);
}

AXL_INLINE
size_t
axl_rtl_DecodeHexString(
	axl_rtl_TByteArray* pData,
	const tchar_t* p,
	size_t Length
	)
{
	axl_rtl_TByteArray_Clear(pData);
	return axl_rtl_AppendDecodeHexString(pData, p, Length);
}

//.............................................................................

} // namespace rtl
} // namespace axl

