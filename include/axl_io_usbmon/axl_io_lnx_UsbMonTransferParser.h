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

#define _AXL_IO_LNX_USBMONTRANSFERPARSER_H

#include "axl_io_lnx_UsbMon.h"
#include "axl_io_UsbMonTransferParser.h"

namespace axl {
namespace io {
namespace lnx {

//..............................................................................

class UsbMonTransferParser: public UsbMonTransferParserBase<
	UsbMonTransferParser,
	usbmon::mon_bin_hdr
> {
	friend class UsbMonTransferParserBase<UsbMonTransferParser, usbmon::mon_bin_hdr>;

protected:
	size_t
	parseHeader(
	 	const void* p,
	 	size_t size
	);
};

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
