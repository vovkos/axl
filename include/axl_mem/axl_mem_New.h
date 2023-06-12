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

#if (_AXL_DEBUG)
#	define _AXL_MEM_TRACKER _AXL_MEM_TRACKER_DEBUG
#else
#	define _AXL_MEM_TRACKER _AXL_MEM_TRACKER_RELEASE
#endif

namespace axl {
namespace mem {

//..............................................................................

enum ExtraSize: size_t {};
const struct ZeroInit_t {} ZeroInit;

//..............................................................................

#if (_AXL_MEM_TRACKER)

void*
allocate(size_t size) AXL_NOEXCEPT;

void
deallocate(void* p) AXL_NOEXCEPT;

#	if (__cpp_aligned_new)

void*
allocate(
	size_t size,
	std::align_val_t align
) AXL_NOEXCEPT;

void
deallocate(
	void* p,
	std::align_val_t align
) AXL_NOEXCEPT;

#	endif // __cpp_aligned_new

#else // _AXL_MEM_TRACKER

inline
void*
allocate(size_t size) AXL_NOEXCEPT {
	return std::malloc(size);
}

inline
void
deallocate(void* p) AXL_NOEXCEPT {
	std::free(p);
}

#	if (__cpp_aligned_new)

inline
void*
allocate(
	size_t size,
	std::align_val_t align
) AXL_NOEXCEPT {
	return std::aligned_alloc(align, size);
}

inline
void
deallocate(
	void* p,
	std::align_val_t align
) AXL_NOEXCEPT {
	std::free(p);
}

#	endif // __cpp_aligned_new

#endif // _AXL_MEM_TRACKER

//..............................................................................

class Deallocate {
public:
	void
	operator () (void* p) const {
		deallocate(p);
	}
};

typedef Deallocate StdFree; // legacy name

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class Delete {
public:
	void
	operator () (T* p) const {
		delete p;
	}
};

//..............................................................................

} // namespace mem
} // namespace axl

#if (_AXL_MEM_TRACKER)

// the most basic new

inline
void*
operator new (size_t size) {
	void* p = axl::mem::allocate(size);
	if (!p)
		throw new std::bad_alloc();
	return p;
}

inline
void
operator delete (void* p) AXL_NOEXCEPT {
	axl::mem::deallocate(p);
}

inline
void*
operator new[] (size_t size) {
	return operator new (size);
}

inline
void
operator delete[] (void* p) AXL_NOEXCEPT {
	operator delete (p);
}

// non-throwing version

inline
void*
operator new (
    size_t size,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return axl::mem::allocate(size);
}

inline
void*
operator new[] (
    size_t size,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
    return operator new (size, nothrow);
}

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// zero-initialized new

inline
void*
operator new (
    size_t size,
    const axl::mem::ZeroInit_t& zeroInit
) {
	void* p = operator new (size);
	memset(p, 0, size);
	return p;
}

inline
void
operator delete (
	void* p,
    const axl::mem::ZeroInit_t& zeroInit
) AXL_NOEXCEPT {
	operator delete (p);
}

inline
void*
operator new[] (
    size_t size,
    const axl::mem::ZeroInit_t& zeroInit
) {
	return operator new (size, zeroInit);
}

inline
void
operator delete[] (
	void* p,
    const axl::mem::ZeroInit_t& zeroInit
) AXL_NOEXCEPT {
	operator delete (p);
}

// non-throwing version

inline
void*
operator new (
    size_t size,
    const axl::mem::ZeroInit_t& zeroInit,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	void* p = operator new (size, nothrow);
	if (p)
		memset(p, 0, size);
	return p;
}

inline
void*
operator new[] (
    size_t size,
    const axl::mem::ZeroInit_t& zeroInit,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size, zeroInit, nothrow);
}

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

// zero-initialized new with extra tail space

inline
void*
operator new (
    size_t size,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit
) {
	return operator new (size + extraSize, zeroInit);
}

inline
void
operator delete (
	void* p,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit
) AXL_NOEXCEPT {
	operator delete (p);
}

inline
void*
operator new[] (
    size_t size,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit
) {
	return operator new (size + extraSize, zeroInit);
}

inline
void
operator delete[] (
	void* p,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit
) AXL_NOEXCEPT {
	operator delete (p);
}

// non-throwing version

inline
void*
operator new (
    size_t size,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size + extraSize, zeroInit, nothrow);
}

inline
void*
operator new[] (
    size_t size,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size + extraSize, zeroInit, nothrow);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (__cpp_aligned_new)

// overaligned new

inline
void*
operator new (
	size_t size,
	std::align_val_t align
) {
	void* p = axl::mem::allocate(size, align);
	if (!p)
		throw new std::bad_alloc();
	return p;
}

inline
void
operator delete (
	void* p,
	std::align_val_t align
) {
	axl::mem::deallocate(p, align);
}

inline
void*
operator new[] (
	size_t size,
	std::align_val_t align
) {
	return operator new (size);
}

inline
void
operator delete[] (
	void* p,
	std::align_val_t align
) AXL_NOEXCEPT {
	operator delete (p);
}

// non-throwing version

inline
void*
operator new (
    size_t size,
	std::align_val_t align,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return axl::mem::allocate(size, align);
}

inline
void*
operator new[] (
    size_t size,
	std::align_val_t align,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
    return operator new (size, nothrow);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// zero-initialized new

inline
void*
operator new (
    size_t size,
    const axl::mem::ZeroInit_t& zeroInit,
	std::align_val_t align
) {
	void* p = operator new (size, align);
	memset(p, 0, size);
	return p;
}

inline
void
operator delete (
	void* p,
	std::align_val_t align,
    const axl::mem::ZeroInit_t& zeroInit
) AXL_NOEXCEPT {
	operator delete (p);
}

inline
void*
operator new[] (
    size_t size,
    const axl::mem::ZeroInit_t& zeroInit,
	std::align_val_t align
) {
	return operator new (size, zeroInit, align);
}

inline
void
operator delete[] (
	void* p,
    const axl::mem::ZeroInit_t& zeroInit,
	std::align_val_t align
) AXL_NOEXCEPT {
	operator delete (p);
}

// non-throwing version

inline
void*
operator new (
    size_t size,
    const axl::mem::ZeroInit_t& zeroInit,
	std::align_val_t align,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	void* p = operator new (size, align, nothrow);
	if (p)
		memset(p, 0, size);
	return p;
}

inline
void*
operator new[] (
    size_t size,
    const axl::mem::ZeroInit_t& zeroInit,
	std::align_val_t align,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size, zeroInit, align, nothrow);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// zero-initialized new with extra tail space

inline
void*
operator new (
    size_t size,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit,
	std::align_val_t align
) {
	return operator new (size + extraSize, zeroInit, align);
}

inline
void
operator delete (
	void* p,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit,
	std::align_val_t align
) AXL_NOEXCEPT {
	operator delete (p);
}

inline
void*
operator new[] (
    size_t size,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit,
	std::align_val_t align
) {
	return operator new (size + extraSize, zeroInit, align);
}

inline
void
operator delete[] (
	void* p,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit,
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
    const axl::mem::ZeroInit_t& zeroInit,
	std::align_val_t align,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size + extraSize, zeroInit, align, nothrow);
}

inline
void*
operator new[] (
    size_t size,
    axl::mem::ExtraSize extraSize,
    const axl::mem::ZeroInit_t& zeroInit,
	std::align_val_t align,
    const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size + extraSize, zeroInit, align, nothrow);
}

#endif // __cpp_aligned_new

//..............................................................................

// legacy macros

#define AXL_MEM_ALLOCATE(size) \
	(axl::mem::allocate(size))

#define AXL_MEM_FREE(size) \
	(axl::mem::deallocate(size))

#define AXL_MEM_NEW(T) \
	(new T)

#define AXL_MEM_NEW_EXTRA(T, extra) \
	(new (axl::mem::ExtraSize(extra)) T)

#define AXL_MEM_NEW_ARGS(T, args) \
	(new T args)

#define AXL_MEM_NEW_ARGS_EXTRA(T, args, extra) \
	(new (axl::mem::ExtraSize(extra)) T args)

#define AXL_MEM_ZERO_NEW(T) \
	(new (axl::mem::ZeroInit) T)

#define AXL_MEM_ZERO_NEW_EXTRA(T, extra) \
	(new (axl::mem::ExtraSize(extra), axl::mem::ZeroInit) T)

#define AXL_MEM_ZERO_NEW_ARGS(T, args) \
	(new (axl::mem::ZeroInit) T args)

#define AXL_MEM_ZERO_NEW_ARGS_EXTRA(T, args, extra) \
	(new (axl::mem::ExtraSize(extra), axl::mem::ZeroInit) T args)

#define AXL_MEM_DELETE(p) \
	(delete p)

//..............................................................................
