// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_ALLOC_H

#include "axl_mem_Malloc.h"

namespace axl {
namespace mem {

//.............................................................................

typedef 
void 
(*FFree) (void* p);

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
			size_t Size,
			const char* pTag,
			const char* pFilePath,
			int Line
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
	Alloc (
		size_t Size,
		const char* pTag,
		const char* pFilePath,
		int Line
		)
	{
		return CAlloc () (Size, pTag, pFilePath, Line);
	}

	static
	void 
	Free (void* p)
	{
		return CFree () (p);
	}
};

#else

typedef CMalloc CStdAlloc;

#endif

//.............................................................................

#ifdef _DEBUG

#define AXL_MEM_ALLOC(Size) \
	axl::mem::CStdAlloc::Alloc (Size, "<buffer>", __FILE__, __LINE__)

#else

#define AXL_MEM_ALLOC(Size) \
	axl::mem::CStdAlloc::Alloc (Size)

#endif

#define AXL_MEM_FREE(p) \
	axl::mem::CStdAlloc::Free (p)

//.............................................................................

} // namespace mem
} // namespace axl
