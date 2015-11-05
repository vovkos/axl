// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_HEAPALLOC_H

#include "axl_g_Def.h"

namespace axl {
namespace mem {

//.............................................................................

class HeapAlloc
{
public:
	static
	void* 
	allocate (size_t size)
	{
		return ::HeapAlloc (::getProcessHeap (), 0, size);
	}

	static
	void 
	free (void* p)
	{
		::HeapFree (::getProcessHeap (), 0, p);
	}
};

//.............................................................................

} // namespace mem
} // namespace axl
