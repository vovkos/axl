// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_ALLOC_H

#include "axl_mem_Malloc.h"

namespace axl {
namespace mem {

//.............................................................................

#ifdef _AXL_DEBUG

class StdAlloc
{
public:
	class Alloc
	{
	public:
		void* 
		operator () (
			size_t size,
			const char* tag,
			const char* filePath,
			int line
			);
	};

	class Free
	{
	public:
		void 
		operator () (void* p);
	};

public:
	static
	void* 
	alloc (
		size_t size,
		const char* tag,
		const char* filePath,
		int line
		)
	{
		return Alloc () (size, tag, filePath, line);
	}

	static
	void 
	free (void* p)
	{
		return Free () (p);
	}
};

#else

typedef Malloc StdAlloc;

#endif

//.............................................................................

#ifdef _AXL_DEBUG

#define AXL_MEM_ALLOC(size) \
	axl::mem::StdAlloc::alloc (size, "<buffer>", __FILE__, __LINE__)

#else

#define AXL_MEM_ALLOC(size) \
	axl::mem::StdAlloc::alloc (size)

#endif

#define AXL_MEM_FREE(p) \
	axl::mem::StdAlloc::free (p)

//.............................................................................

} // namespace mem
} // namespace axl
