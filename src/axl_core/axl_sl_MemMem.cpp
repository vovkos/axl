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
#include "axl_sl_BoyerMooreFind.h"

namespace axl {
namespace sl {

//..............................................................................

// building good-skip table is not worth it -- it's a single search

const void*
memMem(
	const void* p1,
	size_t size1,
	const void* p2,
	size_t size2
) {
	BinaryBoyerMooreFind find(p2, size2, BoyerMooreFlag_Horspool);
	size_t offset = find.find(p1, size1);
	return offset != -1 ? (char*)p1 + offset : NULL;
}

const void*
reverseMemMem(
	const void* p1,
	size_t size1,
	const void* p2,
	size_t size2
) {
	BinaryBoyerMooreFind find(p2, size2, BoyerMooreFlag_Horspool | BoyerMooreFlag_Reverse);
	size_t offset = find.find(p1, size1);
	return offset != -1 ? (char*)p1 + offset : NULL;
}

//..............................................................................

} // namespace sl
} // namespace axl
