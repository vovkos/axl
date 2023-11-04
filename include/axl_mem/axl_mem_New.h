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
const struct ZeroInit_t {} ZeroInit;

//..............................................................................

} // namespace mem
} // namespace axl

// replaceable operators new/delete are non-inline and kept in axl_mem_new
// to enable AXL memory tracking, build AXL with -D_AXL_MEM_TRACKER_DEBUG=1
// then link executables (and only executables!) with axl_mem_new

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
