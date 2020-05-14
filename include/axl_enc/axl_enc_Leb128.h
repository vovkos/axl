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

#define _AXL_ENC_LEB128_H

#include "axl_g_Pch.h"

namespace axl {
namespace enc {

//..............................................................................

size_t
uleb128(
	const void* p,
	size_t size,
	uint64_t* result = NULL
	);

size_t
sleb128(
	const void* p,
	size_t size,
	int64_t* result = NULL
	);

//..............................................................................

} // namespace enc
} // namespace axl
