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

#include "pch.h"
#include "axl_mem_ScatterGather.h"

namespace axl {
namespace mem {

//..............................................................................

size_t
getScatterGatherSize(
	const Block* blockArray,
	size_t blockCount
) {
	size_t size = 0;
	const Block* block = blockArray;
	const Block* end = blockArray + blockCount;

	for (; block < end; block++)
		size += block->m_size;

	return size;
}

size_t
copyScatterGather(
	void* buffer,
	const Block* blockArray,
	size_t blockCount
) {
	char* p = (char*)buffer;
	const Block* block = blockArray;
	const Block* end = blockArray + blockCount;

	for (; block < end; block++) {
		memcpy(p, block->m_p, block->m_size);
		p += block->m_size;
	}

	return p - (char*)buffer;
}

size_t
safeCopyScatterGather(
	void* buffer,
	size_t bufferSize,
	const Block* blockArray,
	size_t blockCount
) {
	char* p = (char*)buffer;
	const Block* block = blockArray;
	const Block* blockEnd = blockArray + blockCount;

	for (; block < blockEnd; block++) {
		if (block->m_size >= bufferSize) {
			memcpy(p, block->m_p, bufferSize);
			p += bufferSize;
			break;
		}

		memcpy(p, block->m_p, block->m_size);
		p += block->m_size;
		bufferSize -= block->m_size;
	}

	return p - (char*)buffer;
}

//..............................................................................

} // namespace err
} // namespace axl
