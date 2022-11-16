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
UsbPcapTransferParser::parse(
	const void* p,
	size_t size
) {
	switch (m_state) {
	case UsbMonTransferParseState_IncompleteHeader:
		return parseHeader(p, size);

	case UsbMonTransferParseState_CompleteHeader:
		if (m_transferHdr.m_captureSize)
			return parseData(size);

		reset();
		return parseHeader(p, size);

	case UsbMonTransferParseState_IncompleteData:
		return parseData(size);

	case UsbMonTransferParseState_CompleteData:
		reset();
		return parseHeader(p, size);

	default:
		ASSERT(false);
		return err::fail<size_t>(-1, err::SystemErrorCode_InvalidDeviceState);
	}
}

size_t
UsbPcapTransferParser::parseHeader(
	const void* p0,
	size_t size
) {
	const char* p = (char*)p0;
	const char* end = p + size;

	p = buffer(sizeof(UsbPcapPacketHdr), p, end);
	if (m_offset < sizeof(UsbPcapPacketHdr))
		return size;

	m_transferHdr.m_timestamp = sys::getTimestampFromTimeval(m_pcapHdr.ts_sec, m_pcapHdr.ts_usec);
	m_transferHdr.m_status = err::SystemErrorCode_Success;
	m_transferHdr.m_originalSize = m_packetHdr.dataLength;
	m_transferHdr.m_captureSize = m_packetHdr.dataLength;
	m_transferHdr.m_transferType = (UsbMonTransferType)m_packetHdr.transfer;
	m_transferHdr.m_bus = (uint8_t)m_packetHdr.bus;
	m_transferHdr.m_address = (uint8_t)m_packetHdr.device;
	m_transferHdr.m_endpoint = m_packetHdr.endpoint;
	m_transferHdr.m_flags = (m_packetHdr.info & USBPCAP_INFO_PDO_TO_FDO) ? UsbMonTransferFlag_Completed : 0;

	switch (m_packetHdr.transfer) {
	case USBPCAP_TRANSFER_CONTROL:
		if (m_packetHdr.headerLen < sizeof(USBPCAP_BUFFER_CONTROL_HEADER))
			return err::fail<size_t>(-1, "invalid usbpcap control header");

		p = buffer(sizeof(UsbPcapControlHdr), p, end);
		if (m_offset < sizeof(UsbPcapControlHdr))
			return size;

		switch (m_controlHdr.stage) {
		case USBPCAP_CONTROL_STAGE_SETUP:
			if (m_packetHdr.dataLength < sizeof(USB_DEFAULT_PIPE_SETUP_PACKET))
				return err::fail<size_t>(-1, "invalid usbpcap control setup packet");

			p = buffer(sizeof(UsbPcapControlSetupHdr), p, end);
			if (m_offset < sizeof(UsbPcapControlSetupHdr))
				return size;

			ASSERT(sizeof(USB_DEFAULT_PIPE_SETUP_PACKET) == sizeof(UsbMonControlSetup));
			memcpy(&m_transferHdr.m_controlSetup, &m_controlSetupHdr.setup, sizeof(UsbMonControlSetup));

			m_transferHdr.m_originalSize = m_transferHdr.m_controlSetup.m_length;
			m_transferHdr.m_captureSize -= sizeof(USB_DEFAULT_PIPE_SETUP_PACKET);
			break;

		case USBPCAP_CONTROL_STAGE_DATA:
		case USBPCAP_CONTROL_STAGE_STATUS:
			return err::fail<size_t>(-1, "unexpected usbpcap control stage (usbpcap 1.5.0 or higher is required)");
		}

		break;

	case USBPCAP_TRANSFER_ISOCHRONOUS:
		if (m_packetHdr.headerLen < sizeof(USBPCAP_BUFFER_ISOCH_HEADER))
			return err::fail<size_t>(-1, "invalid usbpcap isochronous header");

		p = buffer(sizeof(UsbPcapIsoHdr), p, end);
		if (m_offset < sizeof(UsbPcapIsoHdr))
			return size;

		m_transferHdr.m_isochronousHdr.m_startFrame = m_isoHdr.startFrame;
		m_transferHdr.m_isochronousHdr.m_packetCount = m_isoHdr.numberOfPackets;
		m_transferHdr.m_isochronousHdr.m_errorCount = m_isoHdr.errorCount;
		break;
	}

	m_transferHdr.m_actualSize = m_transferHdr.m_captureSize;
	m_state = UsbMonTransferParseState_CompleteHeader;
	m_offset = 0;
	return p - (char*)p0;
}

inline
size_t
UsbPcapTransferParser::parseData(size_t size) {
	size_t leftover = m_transferHdr.m_captureSize - m_offset;
	size_t taken;

	if (size < leftover) {
		m_state = UsbMonTransferParseState_IncompleteData;
		taken = size;
	} else {
		m_state = UsbMonTransferParseState_CompleteData;
		taken = leftover;
	}

	m_offset += taken;
	return taken;
}

inline
const char*
UsbPcapTransferParser::buffer(
	size_t targetSize,
	const char* p,
	const char* end
) {
	if (m_offset >= targetSize)
		return p;

	size_t available = end - p;
	size_t leftover = targetSize - m_offset;
	size_t taken = AXL_MIN(leftover, available);
	memcpy(m_buffer + m_offset, p, taken);
	m_offset += taken;
	return p + taken;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
