//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

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

	Block()
	{
		setup(NULL, 0);
	}

	Block(
		void* p,
		size_t size
		)
	{
		setup(p, size);
	}

	void
	setup(
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
