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
#include "axl_io_lnx_UsbMon.h"

namespace axl {
namespace io {
namespace lnx {

//..............................................................................

size_t
UsbMon::get(
	uint_t code,
	usbmon::mon_bin_hdr* hdr,
	void* data,
	size_t size
) {
	usbmon::mon_bin_get arg;
	arg.hdr = hdr;
	arg.data = data;
	arg.alloc = size;

	int result = ioctl(code, &arg);
	return result != -1 ? AXL_MIN(hdr->len_cap, size) : -1;
}

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
