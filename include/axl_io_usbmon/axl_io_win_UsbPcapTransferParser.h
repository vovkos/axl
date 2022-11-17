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

#define _AXL_IO_WIN_USBPCAPTRANSFERPARSER_H

#include "axl_io_win_UsbPcap.h"
#include "axl_io_UsbMonTransferParser.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

#pragma pack(push, 1)

template <typename T>
struct UsbPcapHdrBase:
	usbpcap::pcaprec_hdr_t,
	T {
};

typedef UsbPcapHdrBase<usbpcap::USBPCAP_BUFFER_PACKET_HEADER> UsbPcapPacketHdr;
typedef UsbPcapHdrBase<usbpcap::USBPCAP_BUFFER_CONTROL_HEADER> UsbPcapControlHdr;
typedef UsbPcapHdrBase<usbpcap::USBPCAP_BUFFER_ISOCH_HEADER> UsbPcapIsoHdr;

struct UsbPcapControlSetupHdr: UsbPcapControlHdr {
	usbpcap::USB_DEFAULT_PIPE_SETUP_PACKET setup;
};

#pragma pack(pop)

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

union UsbPcapTransferParserLoHdr {
	usbpcap::pcaprec_hdr_t m_pcapHdr;
	UsbPcapPacketHdr m_packetHdr;
	UsbPcapControlHdr m_controlHdr;
	UsbPcapControlSetupHdr m_controlSetupHdr;
	UsbPcapIsoHdr m_isoHdr;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbPcapTransferParser: public UsbMonTransferParserBase<
	UsbPcapTransferParser,
	UsbPcapTransferParserLoHdr
> {
	friend class UsbMonTransferParserBase<
		UsbPcapTransferParser,
		UsbPcapTransferParserLoHdr
	>;

protected:
	size_t
	parseHeader(
	 	const void* p,
	 	size_t size
	);
};

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
