// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_MEMMEM_H

#include "axl_g_Pch.h"

namespace axl {
namespace sl {

//..............................................................................

const void*
memMem (
	const void* p1,
	size_t size1,
	const void* p2,
	size_t size2
	);

inline
void*
memMem (
	void* p1,
	size_t size1,
	const void* p2,
	size_t size2
	)
{
	return (void*) memMem ((const void*) p1, size1, p2, size2);
}

const void*
reverseMemMem (
	const void* p1,
	size_t size1,
	const void* p2,
	size_t size2
	);

inline
void*
reverseMemMem (
	void* p1,
	size_t size1,
	const void* p2,
	size_t size2
	)
{
	return (void*) reverseMemMem ((const void*) p1, size1, p2, size2);
}

//..............................................................................

} // namespace sl
} // namespace axl
