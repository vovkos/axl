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
#include "axl_io_lnx_UsbMonTransferParser.h"
#include "axl_sys_Time.h"

namespace axl {
namespace io {
namespace lnx {

using namespace usbmon;

//..............................................................................

size_t
UsbMonTransferParser::parseHeader(
	const void* p0,
	size_t size
) {
	static libusb_transfer_type transferTypeTable[] = {
		LIBUSB_TRANSFER_TYPE_ISOCHRONOUS, // PIPE_ISOCHRONOUS = 0,
		LIBUSB_TRANSFER_TYPE_INTERRUPT,   // PIPE_INTERRUPT   = 1,
		LIBUSB_TRANSFER_TYPE_CONTROL,     // PIPE_CONTROL     = 2,
		LIBUSB_TRANSFER_TYPE_BULK,        // PIPE_BULK        = 3,
	};

	const char* p = (char*)p0;
	const char* end = p + size;

	p = buffer(sizeof(mon_bin_hdr), p, end);
	if (m_offset < sizeof(mon_bin_hdr))
		return size;

	libusb_transfer_type transferType = m_buffer.m_monHdr.xfer_type < countof(transferTypeTable) ?
		transferTypeTable[m_buffer.m_monHdr.xfer_type] :
		(libusb_transfer_type)-1;

	m_hdr.m_timestamp = sys::getTimestampFromTimeval(m_buffer.m_monHdr.ts_sec, m_buffer.m_monHdr.ts_usec);
	m_hdr.m_id = m_buffer.m_monHdr.id;
	m_hdr.m_status = m_buffer.m_monHdr.status;
	m_hdr.m_originalSize = m_buffer.m_monHdr.len_urb;
	m_hdr.m_captureSize = m_buffer.m_monHdr.len_cap;
	m_hdr.m_transferType = transferType;
	m_hdr.m_bus = m_buffer.m_monHdr.busnum;
	m_hdr.m_address = m_buffer.m_monHdr.devnum;
	m_hdr.m_endpoint = m_buffer.m_monHdr.epnum;
	m_hdr.m_flags = m_buffer.m_monHdr.type == 'S' ? 0 : UsbMonTransferFlag_Completed;
	m_hdr.m_urbFunction = 0;

	switch (transferType) {
	case LIBUSB_TRANSFER_TYPE_CONTROL:
		if (m_buffer.m_monHdr.type == 'S')
			memcpy(&m_hdr.m_controlSetup, m_buffer.m_monHdr.s.setup, sizeof(UsbMonControlSetup));
		break;

	case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS:
		m_hdr.m_isoHdr.m_packetCount = m_buffer.m_monHdr.s.iso.numdesc;
		m_hdr.m_isoHdr.m_errorCount = m_buffer.m_monHdr.s.iso.error_count;

		bool result = m_isoPacketTable.setCountZeroConstruct(m_hdr.m_isoHdr.m_packetCount);
		if (!result)
			return -1;

		m_state = UsbMonTransferParserState_IncompleteIsoPacketTable;
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
UsbMonTransferParser::parseIsoPacketTable(
	const void* p0,
	size_t size
) {
	ASSERT(
		m_state == UsbMonTransferParserState_IncompleteIsoPacketTable &&
		m_hdr.m_transferType == LIBUSB_TRANSFER_TYPE_ISOCHRONOUS &&
		m_isoPacketIdx < m_hdr.m_isoHdr.m_packetCount
	);

	const char* p = (char*)p0;
	const char* end = p + size;
	UsbMonIsoPacket* packet = m_isoPacketTable + m_isoPacketIdx;
	UsbMonIsoPacket* packetEnd = m_isoPacketTable + m_hdr.m_isoHdr.m_packetCount;
	ASSERT(packetEnd == m_isoPacketTable.getEnd());

	while (packet < packetEnd) {
		p = buffer(sizeof(usbmon::mon_bin_isodesc), p, end);
		if (m_offset < sizeof(usbmon::mon_bin_isodesc)) {
			m_isoPacketIdx = packet - m_isoPacketTable;
			return size;
		}

		packet->m_status = m_buffer.m_isoDesc.iso_status;
		packet->m_offset = m_buffer.m_isoDesc.iso_off;
		packet->m_length = m_buffer.m_isoDesc.iso_len;
		packet++;

		m_isoDataSize += m_buffer.m_isoDesc.iso_len;
		m_offset = 0; // reset buffering of iso packet
	}

	m_hdr.m_captureSize -= sizeof(usbmon::mon_bin_isodesc) * m_hdr.m_isoHdr.m_packetCount;

	bool isIncoming = (m_hdr.m_endpoint & 0x80) != 0;
	bool isCompleted = (m_hdr.m_flags & UsbMonTransferFlag_Completed) != 0;
	bool hasData = isIncoming == isCompleted;
	if (hasData && m_isoDataSize != m_hdr.m_captureSize)
		return err::fail<size_t>(-1, "invalid isochronous cumulative packet length");

	m_state = UsbMonTransferParserState_CompleteHeader;
	m_offset = 0;
	return p - (char*)p0;
}

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
