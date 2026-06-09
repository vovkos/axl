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

#define _AXL_MEM_NEW_H

#include "axl_g_Pch.h"

namespace axl {
namespace mem {

//..............................................................................

inline
void*
allocate(size_t size) AXL_NOEXCEPT {
	return operator new (size, std::nothrow);
}

inline
void
deallocate(void* p) AXL_NOEXCEPT {
	operator delete (p);
}

#if (__cpp_aligned_new)

inline
void*
allocate(
	size_t size,
	std::align_val_t align
) AXL_NOEXCEPT {
	return operator new (size, align, std::nothrow);
}

inline
void
deallocate(
	void* p,
	std::align_val_t align
) AXL_NOEXCEPT {
	operator delete (p, align);
}

#endif // __cpp_aligned_new

//..............................................................................

class Deallocate {
public:
	void
	operator () (void* p) const {
		deallocate(p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class Delete {
public:
	void
	operator () (T* p) const {
		delete p;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ExtraSize: size_t {};

//..............................................................................

inline
bool
isZeroMemory(
	const void* p0,
	size_t size
) {
	enum {
		BlockSize = 4 * 1024
	};

	static char block[BlockSize] = { 0 };

	const char* p = (char*)p0;
	while (size) {
		if (size <= BlockSize)
			return memcmp(p, block, size) == 0;

		if (memcmp(p, block, BlockSize))
			return false;

		p += BlockSize;
		size -= BlockSize;
	}

	return true;
}

//..............................................................................

} // namespace mem
} // namespace axl

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// new with extra tail space

inline
void*
operator new (
	size_t size,
	axl::mem::ExtraSize extraSize
) {
	return operator new (size + extraSize);
}

inline
void
operator delete (
	void* p,
	axl::mem::ExtraSize extraSize
) AXL_NOEXCEPT {
	operator delete (p);
}

inline
void*
operator new[] (
	size_t size,
	axl::mem::ExtraSize extraSize
) {
	return operator new (size + extraSize);
}

inline
void
operator delete[] (
	void* p,
	axl::mem::ExtraSize extraSize
) AXL_NOEXCEPT {
	operator delete (p);
}

// non-throwing version

inline
void*
operator new (
	size_t size,
	axl::mem::ExtraSize extraSize,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size + extraSize, nothrow);
}

inline
void*
operator new[] (
	size_t size,
	axl::mem::ExtraSize extraSize,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size + extraSize, nothrow);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (__cpp_aligned_new)

// new with extra tail space

inline
void*
operator new (
	size_t size,
	axl::mem::ExtraSize extraSize,
	std::align_val_t align
) {
	return operator new (size + extraSize, align);
}

inline
void
operator delete (
	void* p,
	axl::mem::ExtraSize extraSize,
	std::align_val_t align
) AXL_NOEXCEPT {
	operator delete (p);
}

inline
void*
operator new[] (
	size_t size,
	axl::mem::ExtraSize extraSize,
	std::align_val_t align
) {
	return operator new (size + extraSize, align);
}

inline
void
operator delete[] (
	void* p,
	axl::mem::ExtraSize extraSize,
	std::align_val_t align
) AXL_NOEXCEPT {
	operator delete (p);
}

// non-throwing version

inline
void*
operator new (
	size_t size,
	axl::mem::ExtraSize extraSize,
	std::align_val_t align,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size + extraSize, align, nothrow);
}

inline
void*
operator new[] (
	size_t size,
	axl::mem::ExtraSize extraSize,
	std::align_val_t align,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size + extraSize, align, nothrow);
}

#endif // __cpp_aligned_new

//..............................................................................
