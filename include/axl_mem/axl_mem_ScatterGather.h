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

#define _AXL_MEM_SCATTERGATHER_H

#include "axl_mem_Block.h"

namespace axl {
namespace mem {

//..............................................................................

size_t
getScatterGatherSize(
	const Block* blockArray,
	size_t blockCount
	);

size_t
copyScatterGather(
	void* buffer,
	const Block* blockArray,
	size_t blockCount
	);

size_t
safeCopyScatterGather(
	void* buffer,
	size_t bufferSize,
	const Block* blockArray,
	size_t blockCount
	);

//..............................................................................

} // namespace mem
} // namespace axl
