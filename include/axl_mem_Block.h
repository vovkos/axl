// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_BLOCK_H

#include "axl_g_Pch.h"

namespace axl {
namespace mem {

//..............................................................................

// memory block descriptor

struct Block
{
	void* m_p;
	size_t m_size;

	Block ()
	{
		setup (NULL, 0);
	}

	Block (
		void* p,
		size_t size
		)
	{
		setup (p, size);
	}

	void
	setup (
		void* p,
		size_t size
		)
	{
		m_p = p;
		m_size = size;
	}
};

//..............................................................................

} // namespace mem
} // namespace axl
