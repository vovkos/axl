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
#include "axl_io_UsbMonTransferParserBase.h"

namespace axl {
namespace io {
namespace lnx {

//..............................................................................

union UsbMonTransferParserBuffer {
	usbmon::mon_bin_hdr m_monHdr;
	usbmon::mon_bin_isodesc m_isoDesc;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbMonTransferParser: public UsbMonTransferParserBase<
	UsbMonTransferParser,
	UsbMonTransferParserBuffer
> {
	friend class UsbMonTransferParserBase<UsbMonTransferParser, UsbMonTransferParserBuffer>;

protected:
	size_t
	parseHeader(
	 	const void* p,
	 	size_t size
	);

	size_t
	parseIsoPacketTable(
	 	const void* p,
	 	size_t size
	);
};

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
