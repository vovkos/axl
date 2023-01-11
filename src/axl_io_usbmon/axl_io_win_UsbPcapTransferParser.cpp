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
#include "axl_io_win_UsbPcapTransferParser.h"
#include "axl_sys_Time.h"

namespace axl {
namespace io {
namespace win {

using namespace usbpcap;

//..............................................................................

size_t
UsbPcapTransferParser::parseHeader(
	const void* p0,
	size_t size
) {
	static libusb_transfer_type transferTypeTable[] = {
		LIBUSB_TRANSFER_TYPE_ISOCHRONOUS, // USBPCAP_TRANSFER_ISOCHRONOUS = 0,
		LIBUSB_TRANSFER_TYPE_INTERRUPT,   // USBPCAP_TRANSFER_INTERRUPT   = 1,
		LIBUSB_TRANSFER_TYPE_CONTROL,     // USBPCAP_TRANSFER_CONTROL     = 2,
		LIBUSB_TRANSFER_TYPE_BULK,        // USBPCAP_TRANSFER_BULK        = 3,
	};

	const char* p = (char*)p0;
	const char* end = p + size;

	p = buffer(sizeof(UsbPcapPacketHdr), p, end);
	if (m_offset < sizeof(UsbPcapPacketHdr))
		return size;

	libusb_transfer_type transferType = m_loHdr.m_packetHdr.transfer < countof(transferTypeTable) ?
		transferTypeTable[m_loHdr.m_packetHdr.transfer] :
		LIBUSB_TRANSFER_TYPE_BULK;

	m_hiHdr.m_id = m_loHdr.m_packetHdr.irpId;
	m_hiHdr.m_timestamp = sys::getTimestampFromTimeval(m_loHdr.m_pcapHdr.ts_sec, m_loHdr.m_pcapHdr.ts_usec);
	m_hiHdr.m_status = m_loHdr.m_packetHdr.status;
	m_hiHdr.m_originalSize = m_loHdr.m_packetHdr.dataLength;
	m_hiHdr.m_captureSize = m_loHdr.m_packetHdr.dataLength;
	m_hiHdr.m_transferType = transferType;
	m_hiHdr.m_bus = (uint8_t)m_loHdr.m_packetHdr.bus;
	m_hiHdr.m_address = (uint8_t)m_loHdr.m_packetHdr.device;
	m_hiHdr.m_endpoint = m_loHdr.m_packetHdr.endpoint;
	m_hiHdr.m_flags = (m_loHdr.m_packetHdr.info & USBPCAP_INFO_PDO_TO_FDO) ? UsbMonTransferFlag_Completed : 0;

	switch (transferType) {
	case LIBUSB_TRANSFER_TYPE_CONTROL:
		if (m_loHdr.m_packetHdr.headerLen < sizeof(USBPCAP_BUFFER_CONTROL_HEADER))
			return err::fail<size_t>(-1, "invalid usbpcap control header");

		p = buffer(sizeof(UsbPcapControlHdr), p, end);
		if (m_offset < sizeof(UsbPcapControlHdr))
			return size;

		switch (m_loHdr.m_controlHdr.stage) {
		case USBPCAP_CONTROL_STAGE_SETUP:
			if (m_loHdr.m_packetHdr.dataLength < sizeof(USB_DEFAULT_PIPE_SETUP_PACKET))
				return err::fail<size_t>(-1, "invalid usbpcap control setup packet");

			p = buffer(sizeof(UsbPcapControlSetupHdr), p, end);
			if (m_offset < sizeof(UsbPcapControlSetupHdr))
				return size;

			ASSERT(sizeof(USB_DEFAULT_PIPE_SETUP_PACKET) == sizeof(UsbMonControlSetup));
			memcpy(&m_hiHdr.m_controlSetup, &m_loHdr.m_controlSetupHdr.setup, sizeof(UsbMonControlSetup));

			m_hiHdr.m_originalSize = m_hiHdr.m_controlSetup.m_length;
			m_hiHdr.m_captureSize -= sizeof(USB_DEFAULT_PIPE_SETUP_PACKET);
			break;

		case USBPCAP_CONTROL_STAGE_DATA:
		case USBPCAP_CONTROL_STAGE_STATUS:
			return err::fail<size_t>(-1, "unexpected usbpcap control stage (usbpcap 1.5.0 or higher is required)");
		}

		break;

	case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS:
		if (m_loHdr.m_packetHdr.headerLen < sizeof(USBPCAP_BUFFER_ISOCH_HEADER))
			return err::fail<size_t>(-1, "invalid usbpcap isochronous header");

		p = buffer(sizeof(UsbPcapIsoHdr), p, end);
		if (m_offset < sizeof(UsbPcapIsoHdr))
			return size;

		m_hiHdr.m_isochronousHdr.m_startFrame = m_loHdr.m_isoHdr.startFrame;
		m_hiHdr.m_isochronousHdr.m_packetCount = m_loHdr.m_isoHdr.numberOfPackets;
		m_hiHdr.m_isochronousHdr.m_errorCount = m_loHdr.m_isoHdr.errorCount;
		break;
	}

	m_state = UsbMonTransferParserState_CompleteHeader;
	m_offset = 0;
	return p - (char*)p0;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
