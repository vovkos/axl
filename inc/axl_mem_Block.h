// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
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
};

extern
AXL_SELECT_ANY
TBlock
g_EmptyBlock = { 0 };

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CBlock: public TBlock
{
public:
	CBlock ()
	{
		m_p = NULL;
		m_Size = 0;
	}

	CBlock (
		void* p, 
		size_t Size
		)
	{
		m_p = p;
		m_Size = Size;
	}

	operator TBlock* () // useful when passing as TBlock*
	{
		return this;
	}
};

//.............................................................................

} // namespace mem
} // namespace axl
