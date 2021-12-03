#include "pch.h"
#include "axl_mem_ExecutableBlockArena.h"
#include "axl_err_Error.h"

namespace axl {
namespace mem {

//..............................................................................

#if (_AXL_OS_WIN)

void*
allocateExecutablePages(size_t size) {
	void* pages = ::VirtualAlloc(
		NULL,
		size,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE
	);

	if (!pages)
		err::setLastSystemError();

	return pages;
}

bool
freeExecutablePages(
	void* pages,
	size_t size
) {
	bool_t result = ::VirtualFree(pages, size, MEM_RELEASE);
	return err::complete(result);
}

#else

void*
allocateExecutablePages(size_t size) {
	void* pages = ::mmap(
		NULL,
		size,
		PROT_EXEC | PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON,
		-1,
		0
	);

	if (!pages)
		err::setLastSystemError();

	return pages;
}

bool
freeExecutablePages(
	void* p,
	size_t size
) {
	int result = ::munmap(p, size);
	return err::complete(result == 0);
}

#endif

//..............................................................................

} // namespace spy
} // namespace axl
