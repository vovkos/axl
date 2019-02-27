//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_SL_HASH_H

#include "axl_sl_ArgType.h"

namespace axl {
namespace sl {

//..............................................................................

inline
size_t
djb2()
{
	return 5381;
}

inline
size_t
djb2(
	size_t hash,
	const void* p0,
	size_t size
	)
{
	const uchar_t* p = (const uchar_t*) p0;
	const uchar_t* end = p + size;

	for (; p < end; p++)
		hash = ((hash << 5) + hash) + *p; /* hash * 33 + c */

	return hash;
}

inline
size_t
djb2(
	const void* p,
	size_t size
	)
{
	return djb2(5381, p, size);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// with per-unit operation

template <typename O>
size_t
djb2_op(
	O op,
	size_t hash,
	const utf8_t* p,
	size_t length
	)
{
	const utf8_t* end = p + length;
	for (; p < end; p++)
	{
		utf8_t c = op(*p);
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

template <typename O>
size_t
djb2_op(
	O op,
	size_t hash,
	const utf16_t* p,
	size_t length
	)
{
	const utf16_t* end = p + length;
	for (; p < end; p++)
	{
		utf16_t c = op(*p);
		hash = ((hash << 5) + hash) + c & 0xff;
		hash = ((hash << 5) + hash) + (c >> 8) & 0xff;
	}

	return hash;
}

template <typename O>
size_t
djb2_op(
	O op,
	size_t hash,
	const utf32_t* p,
	size_t length
	)
{
	const utf32_t* end = p + length;
	for (; p < end; p++)
	{
		utf32_t c = op(*p);
		hash = ((hash << 5) + hash) + c & 0xff;
		hash = ((hash << 5) + hash) + (c >> 8) & 0xff;
		hash = ((hash << 5) + hash) + (c >> 16) & 0xff;
		hash = ((hash << 5) + hash) + (c >> 24) & 0xff;
	}

	return hash;
}

template <
	typename T,
	typename O
	>
size_t
djb2_op(
	O op,
	size_t hash,
	const T* p,
	size_t length
	)
{
	const T* end = p + length;
	for (; p < end; p++)
	{
		T c = op(*p);
		hash = djb2(hash, &c, sizeof(c));
	}

	return hash;
}

template <
	typename T,
	typename O
	>
size_t
djb2_op(
	O op,
	const T* p,
	size_t length
	)
{
	return djb2_op(op, 5381, p, length);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint16_t
checksum16(
	const void* p,
	size_t size
	);

//..............................................................................

template <typename T>
class HashDjb2
{
public:
	size_t
	operator () (const T* key) const
	{
		return djb2(key, sizeof(T));
	}

	size_t
	operator () (const T& key) const
	{
		return djb2(&key, sizeof(T));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashChecksum16
{
public:
	size_t
	operator () (const T* key) const
	{
		return checksum16(key, sizeof(T));
	}

	size_t
	operator () (const T& key) const
	{
		return checksum16(&key, sizeof(T));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashId
{
public:
	size_t
	operator () (T key) const
	{
		return (size_t)key;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
		return H() (*a, *b);
	}
};

//..............................................................................

template <typename T>
class HashSzBase
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashSzBase<char>
{
public:
	size_t
	operator () (const char* p) const
	{
		return djb2(p, strlen_s(p));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashSzBase<wchar_t>
{
public:
	size_t
	operator () (const wchar_t* p) const
	{
		return djb2(p, wcslen_s(p)* sizeof(wchar_t));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef HashSzBase<char>    HashSz;
typedef HashSzBase<wchar_t> HashSz_w;
typedef HashSzBase<utf8_t>  HashSz_utf8;
typedef HashSzBase<utf16_t> HashSz_utf16;
typedef HashSzBase<utf32_t> HashSz_utf32;

//..............................................................................

template <typename T>
class HashSzBase_i
{
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashSzBase_i<char>
{
public:
	size_t
	operator () (const char* p) const
	{
		return djb2_op(tolower, p, strlen_s(p));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashSzBase_i<wchar_t>
{
public:
	size_t
	operator () (const wchar_t* p) const
	{
		return djb2_op(towlower, p, wcslen_s(p)* sizeof(wchar_t));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef HashSzBase_i<char>    HashSz_i;
typedef HashSzBase_i<wchar_t> HashSz_i_w;
typedef HashSzBase_i<utf8_t>  HashSz_i_utf8;
typedef HashSzBase_i<utf16_t> HashSz_i_utf16;
typedef HashSzBase_i<utf32_t> HashSz_i_utf32;

//..............................................................................

template <
	typename T,
	typename Arg = typename ArgType<T>::Type
	>
class HashDuckType
{
public:
	size_t
	operator () (Arg a) const
	{
		return a.hash();
	}

	size_t
	operator () (const T* a) const
	{
		return a->hash();
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
