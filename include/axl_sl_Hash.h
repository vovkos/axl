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
	operator () (const T& key) const
	{ 
		return djb2 (&key, sizeof (key)); 
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashChecksum16
{
public:
	size_t 
	operator () (const T& key) const
	{ 
		return checksum16 (&key, sizeof (key)); 
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
class HashStringBase 
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashStringBase <char>
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
class HashStringBase <wchar_t>
{
public:
	size_t 
	operator () (const wchar_t* p) const
	{ 
		return djb2 (p, wcslen_s (p) * sizeof (wchar_t));
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef HashStringBase <char>    HashString;
typedef HashStringBase <wchar_t> HashString_w;
typedef HashStringBase <utf8_t>  HashString_utf8;
typedef HashStringBase <utf16_t> HashString_utf16;
typedef HashStringBase <utf32_t> HashString_utf32;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashDuckType
{
public:
	size_t 
	operator () (const T* a)
	{
		return a->getHash ();
	}

	size_t 
	operator () (const T& a)
	{
		return a.getHash ();
	}
};

//.............................................................................

} // namespace sl
} // namespace axl
