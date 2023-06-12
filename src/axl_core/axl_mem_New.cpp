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
#include "axl_mem_New.h"
#include "axl_g_Module.h"
#include <memory>

namespace axl {
namespace mem {

//..............................................................................

#if (_AXL_MEM_TRACKER)

inline
void*
postAllocate(
	void* p,
	size_t size
) {
	if (!p)
		return NULL;

	TrackerBlock* block = (TrackerBlock*)p;
	block->m_size = size;
	g::getModule()->getMemTracker()->add(block);
	return block + 1;
}

inline
void*
preDeallocate(void* p) {
	if (!p)
		return NULL;

	TrackerBlock* block = (TrackerBlock*)((char*)p - sizeof(TrackerBlock));
	g::getModule()->getMemTracker()->remove(block);
	return block;
}

void*
allocate(size_t size) AXL_NOEXCEPT {
	return postAllocate(std::malloc(sizeof(TrackerBlock) + size), size);
}

void
deallocate(void* p) AXL_NOEXCEPT {
	std::free(preDeallocate(p));
}

#	if (__cpp_aligned_new)

void*
allocate(
	size_t size,
	std::align_val_t align
) AXL_NOEXCEPT {
	return postAllocate(std::aligned_alloc((size_t)align, sizeof(TrackerBlock) + size));
}

void
deallocate(
	void* p,
	std::align_val_t align
) AXL_NOEXCEPT {
	std::free(preDeallocate(p));
}

#	endif // __cpp_aligned_new

#else // _AXL_MEM_TRACKER
#endif // _AXL_MEM_TRACKER

//..............................................................................

} // namespace err
} // namespace axl
