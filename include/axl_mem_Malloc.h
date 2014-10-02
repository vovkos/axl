// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_MALLOC_H

#include "axl_g_Def.h"

namespace axl {
namespace mem {

//.............................................................................

class Malloc
{
public:
	class Alloc
	{
	public:
		void* 
		operator () (size_t size)
		{
			return ::malloc (size);
		}
	};

	class Free
	{
	public:
		void 
		operator () (void* p)
		{
			return ::free (p);
		}
	};

public:
	static
	void* 
	alloc (size_t size)
	{
		return Alloc () (size);
	}

	static
	void 
	free (void* p)
	{
		return Free () (p);
	}
};

//.............................................................................

} // namespace mem
} // namespace axl
