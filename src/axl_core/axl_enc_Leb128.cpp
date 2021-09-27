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
#include "axl_enc_Leb128.h"

namespace axl {
namespace enc {

// C-adaptation of the pseudo-code at https://en.wikipedia.org/wiki/LEB128

//..............................................................................

size_t
uleb128(
	const void* p0,
	size_t size,
	uint64_t* result0
) {
	const uint8_t* p = (uint8_t*)p0;
	size_t i = 0;
	uint64_t result = 0;
	uint_t shift = 0;

	while (i < size) {
		uint8_t byte = p[i++];
		result |= (uint64_t)(byte & 0x7f) << shift;
		if (!(byte & 0x80))
			break;

		shift += 7;
	}

	if (result0)
		*result0 = result;

	return i;
}

size_t
sleb128(
	const void* p0,
	size_t size,
	int64_t* result0
) {
	const uint8_t* p = (uint8_t*)p0;
	size_t i = 0;
	uint64_t result = 0;
	uint_t shift = 0;

	while (i < size) {
		uint8_t byte = p[i++];
		result |= (uint64_t)(byte & 0x7f) << shift;
		shift += 7;

		if (!(byte & 0x80)) {
			if (shift < 64 && (byte & 0x40))
				result |= (~0 << shift);

			break;
		}
	}

	if (result0)
		*result0 = result;

	return i;
}

//..............................................................................

} // namespace enc
} // namespace axl
