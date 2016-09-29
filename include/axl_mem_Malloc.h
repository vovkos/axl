// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_MALLOC_H

#include "axl_g_Pch.h"

namespace axl {
namespace mem {

//.............................................................................

class Malloc
{
public:
	static
	void* 
	allocate (size_t size)
	{
		return ::malloc (size);
	}

	static
	void 
	free (void* p)
	{
		return ::free (p);
	}
};

//.............................................................................

} // namespace mem
} // namespace axl
