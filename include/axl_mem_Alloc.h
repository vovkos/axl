// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_ALLOC_H

#include "axl_mem_Malloc.h"

namespace axl {
namespace mem {

//.............................................................................

typedef 
void 
FFree (void* p);

//.............................................................................

#ifdef _DEBUG

class CStdAlloc
{
public:
	class CAlloc
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

	class CFree
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
		return CAlloc () (size, tag, filePath, line);
	}

	static
	void 
	free (void* p)
	{
		return CFree () (p);
	}
};

#else

typedef CMalloc CStdAlloc;

#endif

//.............................................................................

#ifdef _DEBUG

#define AXL_MEM_ALLOC(size) \
	axl::mem::CStdAlloc::alloc (size, "<buffer>", __FILE__, __LINE__)

#else

#define AXL_MEM_ALLOC(size) \
	axl::mem::CStdAlloc::alloc (size)

#endif

#define AXL_MEM_FREE(p) \
	axl::mem::CStdAlloc::free (p)

//.............................................................................

} // namespace mem
} // namespace axl
