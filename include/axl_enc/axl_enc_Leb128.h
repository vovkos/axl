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

#include "axl_sl_UnsignedType.h"

namespace axl {
namespace enc {

//..............................................................................

template <typename T>
char*
encodeUleb128(
	char* p,
	T value0
) {
	typedef sl::UnsignedType<T>::Type U;

	U value = value0; // should be an implicit lossless conversion

	for (;;) {
		uint8_t byte = value & 0x7f;
		value >>= 7;
		if (!value) {
			*p++ = byte;
			break;
		}

		byte |= 0x80;
		*p++ = byte;
	};

	return p;
}

template <typename T>
const char*
decodeUleb128(
	T* result,
	const char* p,
	const char* end
) {
	typedef sl::UnsignedType<T>::Type U;

	U value = 0;
	uint_t shift = 0;

	while (p < end) {
		uint8_t byte = *p++;
		value |= (U)(byte & 0x7f) << shift;
		if (!(byte & 0x80))
			break;

		shift += 7;
	}

	*result = (T)value;
	return p;
}

template <typename T>
const char*
decodeSleb128(
	T* result,
	const char* p,
	const char* end
) {
	typedef sl::UnsignedType<T>::Type U;

	U value = 0;
	uint_t shift = 0;

	while (p < end) {
		uint8_t byte = *p++;
		value |= (U)(byte & 0x7f) << shift;
		shift += 7;

		if (!(byte & 0x80)) {
			if (shift < 64 && (byte & 0x40))
				value |= ((U)-1 << shift);

			break;
		}
	}

	*result = (T)value;
	return p;
}

//..............................................................................

} // namespace enc
} // namespace axl
