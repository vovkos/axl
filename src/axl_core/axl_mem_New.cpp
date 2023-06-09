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
#include <memory>


namespace axl {
namespace mem {

//..............................................................................

void*
allocate(size_t size) AXL_NOEXCEPT {
	return malloc(size);
}

void
deallocate(void* p) AXL_NOEXCEPT {
	free(p);
}

#if (__cpp_aligned_new)

void*
allocate(
	size_t size,
	std::align_val_t align
) AXL_NOEXCEPT {
	return _aligned_malloc(size, (size_t)align);
}

void
deallocate(
	void* p,
	std::align_val_t align
) AXL_NOEXCEPT {
	_aligned_free(p);
}

#endif // __cpp_aligned_new

//..............................................................................

} // namespace err
} // namespace axl
