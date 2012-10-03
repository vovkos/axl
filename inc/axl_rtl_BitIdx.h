// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_BITIDX_H

#include "axl_g_Def.h"

namespace axl {
namespace rtl {

//.............................................................................

// finding lowest bit's index with binary search

inline
uchar_t
GetLoBitIdx8 (uchar_t x)
{
	return (x & 0x0f) ?
		(x & 0x03) ?
			(x & 0x01) ? 0 : 1 : 
			(x & 0x04) ? 2 : 3 :
		(x & 0x30) ?
			(x & 0x10) ? 4 : 5 : 
			(x & 0x40) ? 6 : 7;
}

inline
uchar_t
GetLoBitIdx16 (ushort_t x)
{
	return (x & 0x00ff) ? 
		GetLoBitIdx8 ((uchar_t) x) : 
		(8 + GetLoBitIdx8 ((uchar_t) (x >> 8)));
}

inline
uchar_t
GetLoBitIdx32 (ulong_t x)
{
	return (x & 0x0000ffff) ? 
		GetLoBitIdx16 ((ushort_t) x) : 
		(16 + GetLoBitIdx16 ((ushort_t) (x >> 16)));
}

inline
uchar_t
GetLoBitIdx64 (ulonglong_t x)
{
	return (x & 0x00000000ffffffff) ? 
		GetLoBitIdx32 ((ulong_t) x) : 
		(32 + GetLoBitIdx32 ((ulong_t) (x >> 32)));
}

//.............................................................................

// finding highest bit's index with binary search

inline
uchar_t
GetHiBitIdx8 (uchar_t x)
{
	return (x & 0xf0) ?
		(x & 0xc0) ?
			(x & 0x80) ? 7 : 6 :
			(x & 0x20) ? 5 : 4 : 
		(x & 0x0c) ?
			(x & 0x08) ? 3 : 2 :
			(x & 0x02) ? 1 : 0; 
}

inline
uchar_t
GetHiBitIdx16 (ushort_t x)
{
	return (x & 0xff00) ? 
		(8 + GetHiBitIdx8 ((uchar_t) (x >> 8))) :
		GetHiBitIdx8 ((uchar_t) x);
}

inline
uchar_t
GetHiBitIdx32 (ulong_t x)
{
	return (x & 0xffff0000) ? 
		(16 + GetHiBitIdx16 ((ushort_t) (x >> 16))) :
		GetHiBitIdx16 ((ushort_t) x); 
}

inline
uchar_t
GetHiBitIdx64 (ulonglong_t x)
{
	return (x & 0xffffffff00000000) ? 
		(32 + GetHiBitIdx32 ((ulong_t) (x >> 32))) :
		GetHiBitIdx32 ((ulong_t) x);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifdef _WIN64

#define GetLoBitIdx  GetLoBitIdx64
#define GetHiBitIdx  GetHiBitIdx64

#else

#define GetLoBitIdx  GetLoBitIdx32
#define GetHiBitIdx  GetHiBitIdx32

#endif

//.............................................................................

// bitmask generation

inline
uchar_t
GetLoBitmask8 (size_t To)
{
	return To >= 8 ? (uchar_t) -1 : ((uchar_t) 1 << (uchar_t) To) - 1;
}

inline
uchar_t
GetHiBitmask8 (size_t From)
{
	return From >= 8 ? 0 : ~(((uchar_t) 1 << (uchar_t) From) - 1);
}

inline
uchar_t
GetBitmask8 (
	size_t From,
	size_t To   
	)
{
	return GetLoBitmask8 (To) & GetHiBitmask8 (From);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ushort_t
GetLoBitmask16 (size_t To)
{
	return To >= 16 ? (ushort_t) -1 : ((ushort_t) 1 << (ushort_t) To) - 1;
}

inline
ushort_t
GetHiBitmask16 (size_t From)
{
	return From >= 16 ? 0 : ~(((ushort_t) 1 << (ushort_t) From) - 1);
}

inline
ushort_t
GetBitmask16 (
	size_t From,
	size_t To   
	)
{
	return GetLoBitmask16 (To) & GetHiBitmask16 (From);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ulong_t
GetLoBitmask32 (size_t To)
{
	return To >= 32 ? (ulong_t) -1 : ((ulong_t) 1 << (ulong_t) To) - 1;
}

inline
ulong_t
GetHiBitmask32 (size_t From)
{
	return From >= 32 ? 0 : ~(((ulong_t) 1 << (ulong_t) From) - 1);
}

inline
ulong_t
GetBitmask32 (
	size_t From,
	size_t To   
	)
{
	return GetLoBitmask32 (To) & GetHiBitmask32 (From);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ulonglong_t
GetLoBitmask64 (size_t To)
{
	return To >= 64 ? (ulonglong_t) -1 : ((ulonglong_t) 1 << (ulonglong_t) To) - 1;
}

inline
ulonglong_t
GetHiBitmask64 (size_t From)
{
	return From >= 64 ? 0 : ~(((ulonglong_t) 1 << (ulonglong_t) From) - 1);
}

inline
ulonglong_t
GetBitmask64 (
	size_t From,
	size_t To   
	)
{
	return GetLoBitmask64 (To) & GetHiBitmask64 (From);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifdef _WIN64

#define GetBitmask   GetBitmask64
#define GetLoBitmask GetLoBitmask64
#define GetHiBitmask GetHiBitmask64

#else

#define GetBitmask   GetBitmask32
#define GetLoBitmask GetLoBitmask32
#define GetHiBitmask GetHiBitmask32

#endif

//.............................................................................

// finding the closest power of 2 which is 
// (gt) greater than... or
// (ge) greater or equal 

inline
size_t 
GetMinPower2Gt (size_t Size)
{
	size_t HiBitIdx = GetHiBitIdx (Size);
	return 2 << HiBitIdx;
}

inline
size_t 
GetMinPower2Ge (size_t Size)
{
	return Size && !(Size & (Size - 1)) ? Size : GetMinPower2Gt (Size);
}

//.............................................................................

} // namespace rtl
} // namespace axl
