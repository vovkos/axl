// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_HEAPALLOC_H

#include "axl_g_Def.h"

namespace axl {
namespace mem {

//.............................................................................

class CHeapAlloc
{
public:
	class CAlloc
	{
	public:
		void* 
		operator () (size_t size)
		{
			return ::HeapAlloc (::getProcessHeap (), 0, size);
		}
	};

	class CFree
	{
	public:
		void 
		operator () (void* p)
		{
			::HeapFree (::getProcessHeap (), 0, p);
		}
	};

public:
	static
	void* 
	alloc (size_t size)
	{
		return CAlloc () (size);
	}

	static
	void 
	free (void* p)
	{
		return CFree () (p);
	}
};

//.............................................................................

} // namespace mem
} // namespace axl
