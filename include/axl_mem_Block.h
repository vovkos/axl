// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MEM_BLOCK_H

#include "axl_g_Def.h"

namespace axl {
namespace mem {

//.............................................................................

// memory block descriptor

struct TBlock
{
	void* m_p;
	size_t m_Size;

	TBlock ()
	{
		Setup (NULL, 0);
	}

	TBlock (
		void* p, 
		size_t Size
		)
	{
		Setup (p, Size);
	}

	void
	Setup (
		void* p, 
		size_t Size
		)
	{
		m_p = p;
		m_Size = Size;
	}
};

//.............................................................................

} // namespace mem
} // namespace axl
