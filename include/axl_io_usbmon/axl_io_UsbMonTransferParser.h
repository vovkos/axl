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

#define _AXL_IO_USBMONTRANSFERPARSER_H

#if (_AXL_OS_WIN)
#	include "axl_io_win_UsbPcapTransferParser.h"

namespace axl {
namespace io {

typedef win::UsbPcapTransferParser UsbMonTransferParser;

}
}

#elif (_AXL_OS_LINUX)
#	include "axl_io_lnx_UsbMonTransferParser.h"

namespace axl {
namespace io {

typedef lnx::UsbMonTransferParser UsbMonTransferParser;

}
}

#endif

