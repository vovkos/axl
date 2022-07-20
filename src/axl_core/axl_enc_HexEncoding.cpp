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
#include "axl_enc_HexEncoding.h"

namespace axl {
namespace enc {

//..............................................................................

size_t
HexEncoding::decode(
	sl::Array<char>* buffer,
	const sl::StringRef& source
) {
	enum State {
		State_Normal = 0,
		State_Hex
	};

	State state = State_Normal;

	buffer->clear();
	buffer->reserve(source.getLength() / 2);

	char hexCodeString[4] = { 0 };
	char* hexCodeEnd;
	size_t hexCodeLen;

	uchar_t x;

	const char* p = source.cp();
	const char* end = source.getEnd();

	for (; p < end; p++) {
		bool_t isSpace = isspace(*p);

		switch (state) {
		case State_Normal:
			if (isSpace)
				break;

			hexCodeString[0] = *p;
			hexCodeLen = 1;
			state = State_Hex;
			break;

		case State_Hex:
			if (!isSpace) {
				hexCodeString[hexCodeLen++] = *p;
				if (hexCodeLen < 2)
					break;
			}

			hexCodeString[hexCodeLen] = 0;
			x = (uchar_t)strtoul(hexCodeString, &hexCodeEnd, 16);

			if (hexCodeEnd == &hexCodeString[hexCodeLen])
				buffer->append(x);
			else
				p = end; // not a hex string anymore, break the loop

			state = State_Normal;
			break;
		}
	}

	if (state == State_Hex) {
		hexCodeString[hexCodeLen] = 0;
		x = (uchar_t)strtoul(hexCodeString, &hexCodeEnd, 16);

		if (hexCodeEnd == &hexCodeString[hexCodeLen])
			buffer->append(x);
	}

	return buffer->getCount();
}

//..............................................................................

} // namespace enc
} // namespace axl
