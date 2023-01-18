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
	ASSERT(sizeof(m_buffer) >= sizeof(UsbPcapPacketHdr));

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

	if (m_buffer.m_packetHdr.headerLen < sizeof(USBPCAP_BUFFER_PACKET_HEADER))
		return err::fail<size_t>(-1, "invalid usbpcap packet header size");

	libusb_transfer_type transferType = m_buffer.m_packetHdr.transfer < countof(transferTypeTable) ?
		transferTypeTable[m_buffer.m_packetHdr.transfer] :
		(libusb_transfer_type)-1;

	m_hdr.m_id = m_buffer.m_packetHdr.irpId;
	m_hdr.m_timestamp = sys::getTimestampFromTimeval(m_buffer.m_pcapHdr.ts_sec, m_buffer.m_pcapHdr.ts_usec);
	m_hdr.m_status = m_buffer.m_packetHdr.status;
	m_hdr.m_originalSize = m_buffer.m_packetHdr.dataLength;
	m_hdr.m_captureSize = m_buffer.m_packetHdr.dataLength;
	m_hdr.m_transferType = transferType;
	m_hdr.m_bus = (uint8_t)m_buffer.m_packetHdr.bus;
	m_hdr.m_address = (uint8_t)m_buffer.m_packetHdr.device;
	m_hdr.m_endpoint = m_buffer.m_packetHdr.endpoint;
	m_hdr.m_flags = (m_buffer.m_packetHdr.info & USBPCAP_INFO_PDO_TO_FDO) ? UsbMonTransferFlag_Completed : 0;
	m_hdr.m_urbFunction = m_buffer.m_packetHdr.function;

	switch (transferType) {
	case LIBUSB_TRANSFER_TYPE_CONTROL:
		if (m_buffer.m_packetHdr.headerLen != sizeof(USBPCAP_BUFFER_CONTROL_HEADER))
			return err::fail<size_t>(-1, "invalid usbpcap control header size");

		p = buffer(sizeof(UsbPcapControlHdr), p, end);
		if (m_offset < sizeof(UsbPcapControlHdr))
			return size;

		switch (m_buffer.m_controlHdr.stage) {
		case USBPCAP_CONTROL_STAGE_SETUP:
			if (m_buffer.m_packetHdr.dataLength < sizeof(USB_DEFAULT_PIPE_SETUP_PACKET))
				return err::fail<size_t>(-1, "invalid usbpcap control setup packet size");

			p = buffer(sizeof(UsbPcapControlSetupHdr), p, end);
			if (m_offset < sizeof(UsbPcapControlSetupHdr))
				return size;

			ASSERT(sizeof(USB_DEFAULT_PIPE_SETUP_PACKET) == sizeof(UsbMonControlSetup));
			memcpy(&m_hdr.m_controlSetup, &m_buffer.m_controlSetupHdr.setup, sizeof(UsbMonControlSetup));

			m_hdr.m_originalSize = m_hdr.m_controlSetup.m_length;
			m_hdr.m_captureSize -= sizeof(USB_DEFAULT_PIPE_SETUP_PACKET);
			break;

		case USBPCAP_CONTROL_STAGE_DATA:
		case USBPCAP_CONTROL_STAGE_STATUS:
			return err::fail<size_t>(-1, "unexpected usbpcap control stage (usbpcap 1.5.0 or higher is required)");
		}

		break;

	case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS:
		if (m_buffer.m_packetHdr.headerLen < sizeof(USBPCAP_BUFFER_ISOCH_HEADER))
			return err::fail<size_t>(-1, "invalid usbpcap isochronous header size");

		p = buffer(sizeof(UsbPcapIsoHdr), p, end);
		if (m_offset < sizeof(UsbPcapIsoHdr))
			return size;

		size_t fullHdrSize =
			sizeof(USBPCAP_BUFFER_ISOCH_HEADER) +
			sizeof(USBPCAP_BUFFER_ISO_PACKET) * m_buffer.m_isoHdr.numberOfPackets;

		if (m_buffer.m_packetHdr.headerLen != fullHdrSize)
			return err::fail<size_t>(-1, "invalid usbpcap isochronous extended header size");

		m_hdr.m_isoHdr.m_packetCount = m_buffer.m_isoHdr.numberOfPackets;
		m_hdr.m_isoHdr.m_errorCount = m_buffer.m_isoHdr.errorCount;

		bool result = m_isoPacketArray.setCountZeroConstruct(m_hdr.m_isoHdr.m_packetCount);
		if (!result)
			return -1;

		m_state = UsbMonTransferParserState_IncompleteIsoPacketArray;
		m_isoPacketIdx = 0;
		m_isoDataSize = 0;
		m_offset = 0;
		return p - (char*)p0;
	}

	m_state = UsbMonTransferParserState_CompleteHeader;
	m_offset = 0;
	return p - (char*)p0;
}

size_t
UsbPcapTransferParser::parseIsoPacketTable(
	const void* p0,
	size_t size
) {
	ASSERT(
		m_state == UsbMonTransferParserState_IncompleteIsoPacketArray &&
		m_hdr.m_transferType == LIBUSB_TRANSFER_TYPE_ISOCHRONOUS &&
		m_isoPacketIdx < m_hdr.m_isoHdr.m_packetCount
	);

	const char* p = (char*)p0;
	const char* end = p + size;
	UsbMonIsoPacket* packet = m_isoPacketArray + m_isoPacketIdx;
	UsbMonIsoPacket* packetEnd = m_isoPacketArray + m_hdr.m_isoHdr.m_packetCount;
	ASSERT(packetEnd == m_isoPacketArray.getEnd());

	while (packet < packetEnd) {
		p = buffer(sizeof(USBPCAP_BUFFER_ISO_PACKET), p, end);
		if (m_offset < sizeof(USBPCAP_BUFFER_ISO_PACKET)) {
			m_isoPacketIdx = packet - m_isoPacketArray;
			return size;
		}

		packet->m_status = m_buffer.m_isoPacket.status;
		packet->m_offset = m_buffer.m_isoPacket.offset;
		packet->m_length = m_buffer.m_isoPacket.length;
		packet++;

		m_isoDataSize += m_buffer.m_isoPacket.length;
		m_offset = 0; // reset buffering of iso packet
	}

	if (m_isoDataSize != m_hdr.m_captureSize)
		return err::fail<size_t>(-1, "invalid isochronous cumulative packet length");

	m_state = UsbMonTransferParserState_CompleteHeader;
	m_offset = 0;
	return p - (char*)p0;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
