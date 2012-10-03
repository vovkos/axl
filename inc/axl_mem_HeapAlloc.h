// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
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
		operator () (size_t Size)
		{
			return ::HeapAlloc (::GetProcessHeap (), 0, Size);
		}
	};

	class CFree
	{
	public:
		void 
		operator () (void* p)
		{
			::HeapFree (::GetProcessHeap (), 0, p);
		}
	};

public:
	static
	void* 
	Alloc (size_t Size)
	{
		return CAlloc () (Size);
	}

	static
	void 
	Free (void* p)
	{
		return CFree () (p);
	}
};

//.............................................................................

} // namespace mem
} // namespace axl
