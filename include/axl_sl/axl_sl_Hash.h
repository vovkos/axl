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

enum {
	Djb2Seed = 5381,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
size_t
djb2() {
	return Djb2Seed;
}

inline
size_t
djb2(
	size_t hash,
	const void* p0,
	size_t size
) {
	const uchar_t* p = (const uchar_t*)p0;
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
) {
	return djb2(Djb2Seed, p, size);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
size_t
djb2(
	size_t hash,
	utf32_t c
) {
	hash = ((hash << 5) + hash) + (c & 0xff);
	hash = ((hash << 5) + hash) + ((c >> 8) & 0xff);
	hash = ((hash << 5) + hash) + ((c >> 16) & 0xff);
	hash = ((hash << 5) + hash) + ((c >> 24) & 0xff);
	return hash;
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
) {
	const utf8_t* end = p + length;
	for (; p < end; p++) {
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
) {
	const utf16_t* end = p + length;
	for (; p < end; p++) {
		utf16_t c = op(*p);
		hash = ((hash << 5) + hash) + (c & 0xff);
		hash = ((hash << 5) + hash) + ((c >> 8) & 0xff);
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
) {
	const utf32_t* end = p + length;
	for (; p < end; p++) {
		utf32_t c = op(*p);
		hash = ((hash << 5) + hash) + (c & 0xff);
		hash = ((hash << 5) + hash) + ((c >> 8) & 0xff);
		hash = ((hash << 5) + hash) + ((c >> 16) & 0xff);
		hash = ((hash << 5) + hash) + ((c >> 24) & 0xff);
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
) {
	const T* end = p + length;
	for (; p < end; p++) {
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
) {
	return djb2_op(op, Djb2Seed, p, length);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint16_t
checksum16(
	const void* p,
	size_t size
);

//..............................................................................

template <typename T>
class HashDjb2 {
public:
	size_t
	operator () (const T* key) const {
		return djb2(key, sizeof(T));
	}

	size_t
	operator () (const T& key) const {
		return djb2(&key, sizeof(T));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashChecksum16 {
public:
	size_t
	operator () (const T* key) const {
		return checksum16(key, sizeof(T));
	}

	size_t
	operator () (const T& key) const {
		return checksum16(&key, sizeof(T));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashId {
public:
	size_t
	operator () (T key) const {
		return (size_t)key;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// bit-mixing helpers for integer hashing with power-of-2 bucket dispatch

inline
size_t
hashInt32(uint32_t x) {
	x ^= x >> 16;
	x *= 0x7feb352d;
	x ^= x >> 15;
	x *= 0x846ca68b;
	x ^= x >> 16;
	return (size_t)x;
}

inline
size_t
hashInt64(uint64_t x) {
	x ^= x >> 33;
	x *= 0xff51afd7ed558ccdULL;
	x ^= x >> 33;
	x *= 0xc4ceb9fe1a85ec53ULL;
	x ^= x >> 33;
	return (size_t)x;
}

inline
size_t
hashInt128(
	uint64_t lo,
	uint64_t hi
) {
	return hashInt64(hashInt64(lo) * 0x9e3779b97f4a7c15ULL + hi);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// HashIntImpl<size_t> is to make sure every integer type of a given size
// is covered regardless of specific typedefs on a given platform

template <size_t size>
class HashIntImpl;

template <>
class HashIntImpl<1> {
public:
	size_t
	operator () (uint8_t key) const {
		return key; // identity for 8-bit
	}
};

template <>
class HashIntImpl<2> {
public:
	size_t
	operator () (uint16_t key) const {
		return hashInt32(key);
	}
};

template <>
class HashIntImpl<4> {
public:
	size_t
	operator () (uint32_t key) const {
		return hashInt32(key);
	}
};

template <>
class HashIntImpl<8> {
public:
	size_t
	operator () (uint64_t key) const {
		return hashInt64(key);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashInt {
public:
	size_t
	operator () (T key) const {
		return HashIntImpl<sizeof(T)>()(key);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class HashPtr {
public:
	size_t
	operator () (const T* p) const {
		return HashInt<size_t>()((size_t)p);
	}
};

//..............................................................................

template <typename T>
class HashSzBase {
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashSzBase<char> {
public:
	size_t
	operator () (const char* p) const {
		return djb2(p, strlen_s(p));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashSzBase<wchar_t> {
public:
	size_t
	operator () (const wchar_t* p) const {
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
class HashSzBase_i {
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashSzBase_i<char> {
public:
	size_t
	operator () (const char* p) const {
		return djb2_op(tolower, p, strlen_s(p));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <>
class HashSzBase_i<wchar_t> {
public:
	size_t
	operator () (const wchar_t* p) const {
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
	typename Arg = ArgType<T>
>
class HashDuckType {
public:
	size_t
	operator () (Arg a) const {
		return a.hash();
	}

	size_t
	operator () (const T* a) const {
		return a->hash();
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
