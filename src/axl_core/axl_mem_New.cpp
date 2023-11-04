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

#include "pch.h"
#include "axl_mem_Tracker.h"
#include <memory>

#if (_AXL_MEM_TRACKER)

namespace axl {
namespace mem {

// on gcc/clang replacement new/delete operators should not have "hidden" visibility

#if (_AXL_CPP_GCC)
#	define AXL_VISIBILITY_DEFAULT __attribute__((visibility("default")))
#else
#	define AXL_VISIBILITY_DEFAULT
#endif

//..............................................................................

inline
void*
addTrackerBlock(
	void* p,
	size_t size
) AXL_NOEXCEPT {
	TrackerBlock* block = (TrackerBlock*)p;
	block->m_size = size;
	getTracker()->add(block);
	return block + 1;
}

inline
void*
removeTrackerBlock(void* p) AXL_NOEXCEPT {
	TrackerBlock* block = (TrackerBlock*)((char*)p - sizeof(TrackerBlock));
	getTracker()->remove(block);
	return block;
}

inline
void*
removeTrackerBlock(
	void* p,
	size_t size
) AXL_NOEXCEPT {
	TrackerBlock* block = (TrackerBlock*)((char*)p - sizeof(TrackerBlock));
	ASSERT(size = sizeof(TrackerBlock) + block->m_size);
	getTracker()->remove(block);
	return block;
}

inline
void*
trackingAllocate(size_t size) AXL_NOEXCEPT {
	void* p = std::malloc(sizeof(TrackerBlock) + size);
	return p ? addTrackerBlock(p, size) : NULL;
}

inline
void
trackingDeallocate(void* p) AXL_NOEXCEPT {
	if (p)
		std::free(removeTrackerBlock(p));
}

inline
void
trackingDeallocate(
	void* p,
	size_t size
) AXL_NOEXCEPT {
	if (p)
		std::free(removeTrackerBlock(p, size));
}

#	if (__cpp_aligned_new)
inline
void*
trackingAllocate(
	size_t size,
	std::align_val_t align
) AXL_NOEXCEPT {
	void* p = std::aligned_alloc((size_t)align, sizeof(TrackerBlock) + size);
	return p ? addTrackerBlock(p, size) : NULL;
}
#	endif

//..............................................................................

} // namespace mem
} // namespace axl

AXL_VISIBILITY_DEFAULT
void*
operator new (size_t size) {
	void* p = axl::mem::trackingAllocate(size);
	if (!p)
		throw new std::bad_alloc();
	return p;
}

AXL_VISIBILITY_DEFAULT
void
operator delete (void* p) AXL_NOEXCEPT {
	axl::mem::trackingDeallocate(p);
}

AXL_VISIBILITY_DEFAULT
void
operator delete (
	void* p,
	size_t size
) AXL_NOEXCEPT {
	axl::mem::trackingDeallocate(p, size);
}

AXL_VISIBILITY_DEFAULT
void*
operator new[] (size_t size) {
	return operator new (size);
}

AXL_VISIBILITY_DEFAULT
void
operator delete[] (void* p) AXL_NOEXCEPT {
	operator delete (p);
}

AXL_VISIBILITY_DEFAULT
void
operator delete[] (
	void* p,
	size_t size
) AXL_NOEXCEPT {
	operator delete (p, size);
}

// non-throwing version

AXL_VISIBILITY_DEFAULT
void*
operator new (
	size_t size,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return axl::mem::trackingAllocate(size);
}

AXL_VISIBILITY_DEFAULT
void
operator delete (
	void* p,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	operator delete (p);
}

AXL_VISIBILITY_DEFAULT
void*
operator new[] (
	size_t size,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size, nothrow);
}

AXL_VISIBILITY_DEFAULT
void
operator delete[] (
	void* p,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	operator delete (p);
}

#	if (__cpp_aligned_new)

// overaligned new

AXL_VISIBILITY_DEFAULT
void*
operator new (
	size_t size,
	std::align_val_t align
) {
	void* p = axl::mem::trackingAllocate(size, align);
	if (!p)
		throw new std::bad_alloc();
	return p;
}

AXL_VISIBILITY_DEFAULT
void
operator delete (
	void* p,
	std::align_val_t align
) AXL_NOEXCEPT {
	axl::mem::trackingDeallocate(p);
}

AXL_VISIBILITY_DEFAULT
void
operator delete (
	void* p,
	size_t size,
	std::align_val_t align
) AXL_NOEXCEPT {
	axl::mem::trackingDeallocate(p, size);
}

AXL_VISIBILITY_DEFAULT
void*
operator new[] (
	size_t size,
	std::align_val_t align
) {
	return operator new (size, align);
}

AXL_VISIBILITY_DEFAULT
void
operator delete[] (
	void* p,
	std::align_val_t align
) AXL_NOEXCEPT {
	operator delete (p, align);
}

AXL_VISIBILITY_DEFAULT
void
operator delete[] (
	void* p,
	size_t size,
	std::align_val_t align
) AXL_NOEXCEPT {
	operator delete (p, size, align);
}

// non-throwing version

AXL_VISIBILITY_DEFAULT
void*
operator new (
	size_t size,
	std::align_val_t align,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return axl::mem::trackingAllocate(size, align);
}

AXL_VISIBILITY_DEFAULT
void
operator delete (
	void* p,
	std::align_val_t align,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	operator delete (p, align);
}

AXL_VISIBILITY_DEFAULT
void*
operator new[] (
	size_t size,
	std::align_val_t align,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	return operator new (size, align, nothrow);
}

AXL_VISIBILITY_DEFAULT
void
operator delete[] (
	void* p,
	std::align_val_t align,
	const std::nothrow_t& nothrow
) AXL_NOEXCEPT {
	operator delete (p, align);
}

#	endif // __cpp_aligned_new
#endif    // _AXL_MEM_TRACKER

//..............................................................................
