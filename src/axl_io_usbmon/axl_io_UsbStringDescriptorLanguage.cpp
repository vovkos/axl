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
#include "axl_io_UsbStringDescriptorLanguage.h"

namespace axl {
namespace io {

//..............................................................................

ushort_t
chooseUsbStringDescriptorLanguage(
	const sl::String_utf16& string,
	uint_t preferredLangId
) {
	size_t length = string.getLength();
	if (!length)
		return 0;

	ushort_t firstLangId = string[0];
	if (!preferredLangId || (firstLangId & 0xff) == preferredLangId)
		return firstLangId;

	for (size_t i = 1; i < length; i++) {
		ushort_t langId = string[i];
		if ((langId & 0xff) == preferredLangId)
			return langId;
	}

	return firstLangId;
}

//..............................................................................

} // namespace io
} // namespace axl
