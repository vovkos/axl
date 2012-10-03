// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_MALLOC_H

#include "axl_g_Def.h"

namespace axl {
namespace mem {

//.............................................................................

class CMalloc
{
public:
	class CAlloc
	{
	public:
		void* 
		operator () (size_t Size)
		{
			return malloc (Size);
		}
	};

	class CFree
	{
	public:
		void 
		operator () (void* p)
		{
			return free (p);
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
