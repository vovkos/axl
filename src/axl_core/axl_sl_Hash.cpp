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
#include "axl_sl_Hash.h"

namespace axl {
namespace sl {

//..............................................................................

uint16_t
checksum16(
	const void* _p,
	size_t size
) {
	ushort_t* p = (ushort_t*)_p;
	uchar_t* last = (uchar_t*)_p + size - 1;

	uintptr_t sum = 0;

	for (; (uchar_t*)p < last; p++)
		sum += *p;

	if (size & 1)
		sum += *last;

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	return (ushort_t) ~sum;
}

//..............................................................................

} // namespace err
} // namespace axl
