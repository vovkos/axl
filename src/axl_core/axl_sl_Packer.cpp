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
#include "axl_sl_Packer.h"

namespace axl {
namespace sl {

//..............................................................................

axl_va_list
PackerSeq::pack_va(
	void* _p,
	size_t* size,
	axl_va_list va
) {
	size_t count = m_sequence.getCount();
	size_t totalSize = 0;

	if (!_p) {
		for (size_t i = 0; i < count; i++) {
			size_t size = 0;
			va = m_sequence[i]->pack_va(NULL, &size, va);
			totalSize += size;
		}
	} else {
		uchar_t* p = (uchar_t*)_p;

		for (size_t i = 0; i < count; i++) {
			size_t size = 0;

			va = m_sequence[i]->pack_va(p, &size, va);

			p += size;
			totalSize += size;
		}
	}

	*size = totalSize;
	return va;
}

size_t
PackerSeq::appendFormat(const char* formatString) {
	if (!formatString)
		return m_sequence.getCount();

	const char* pF = formatString;

	for (; *pF; pF++) {
		if (*pF != '%')
			continue;

		pF++;

		switch (*pF) {
		case 'd':
		case 'i':
		case 'o':
		case 'u':
		case 'x':
		case 'X':
			append<Pack<int> >();
			break;

		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
			append<Pack<double> >();
			break;

		case 'p':
			append<Pack<size_t> >();
			break;

		case 's':
			append<PackString>();
			break;

		case 'c':
			append<Pack<char> >();
			break;

		case 'S':
			append<PackString_w>();
			break;

		case 'C':
			append<Pack<wchar_t> >();
			break;

		case 'B':
			append<Pack<uint8_t> >();
			break;

		case 'W':
			append<Pack<uint16_t> >();
			break;

		case 'D':
			append<Pack<uint32_t> >();
			break;

		case 'Z':
			append<Pack<size_t> >();
			break;

		case 'P':
			append<PackPtrSize>();
			break;

		case 'R':
			append<PackLastError>();
			break;
		}
	}

	return m_sequence.getCount();
}

//..............................................................................

size_t
Package::append_va(
	Packer* packer,
	axl_va_list va
) {
	bool result;

	size_t size;
	packer->pack_va(NULL, &size, va);

	size_t oldSize = m_buffer.getCount();
	size_t newSize = oldSize + size;

	result = m_buffer.setCount(newSize);
	if (!result)
		return oldSize;

	packer->pack_va(m_buffer.p() + oldSize, &size, va);
	return newSize;
}

size_t
Package::append(
	const void* p,
	size_t size
) {
	bool result;

	size_t oldSize = m_buffer.getCount();
	size_t newSize = oldSize + size;

	result = m_buffer.setCount(newSize);
	if (!result)
		return oldSize;

	memcpy(m_buffer.p() + oldSize, p, size);
	return newSize;
}

//..............................................................................

} // namespace err
} // namespace axl
