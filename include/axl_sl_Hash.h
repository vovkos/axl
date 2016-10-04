// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_SL_HASH_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//.............................................................................

size_t
djb2 (
	const void* p,
	size_t size
	);

uint16_t
checksum16 (
	const void* p, 
	size_t size
	);

//.............................................................................

template <typename T>
class HashDjb2
{
public:
	size_t 
	operator () (const T* key) const
	{ 
		return djb2 (key, sizeof (T)); 
	}

	size_t 
	operator () (const T& key) const
	{ 
		return djb2 (&key, sizeof (T)); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashChecksum16
{
public:
	size_t 
	operator () (const T* key) const
	{ 
		return checksum16 (key, sizeof (T)); 
	}

	size_t 
	operator () (const T& key) const
	{ 
		return checksum16 (&key, sizeof (T)); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashId
{
public:
	size_t 
	operator () (T key) const
	{ 
		return (size_t) key; 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename H
	>
class HashPtr
{
public:
	size_t 
	operator () (
		const T* a, 
		const T* b
		) const
	{ 
		return H () (*a, *b);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashSzBase 
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashSzBase <char>
{
public:
	size_t 
	operator () (const char* p) const
	{ 
		return djb2 (p, strlen_s (p));
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashSzBase <wchar_t>
{
public:
	size_t 
	operator () (const wchar_t* p) const
	{ 
		return djb2 (p, wcslen_s (p) * sizeof (wchar_t));
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef HashSzBase <char>    HashSz;
typedef HashSzBase <wchar_t> HashSz_w;
typedef HashSzBase <utf8_t>  HashSz_utf8;
typedef HashSzBase <utf16_t> HashSz_utf16;
typedef HashSzBase <utf32_t> HashSz_utf32;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename T,
	typename Arg = typename ArgType <T>::Type
	>
class HashDuckType
{
public:
	size_t 
	operator () (Arg a)
	{
		return a.hash ();
	}

	size_t 
	operator () (const T* a)
	{
		return a->hash ();
	}
};

//.............................................................................

} // namespace sl
} // namespace axl
