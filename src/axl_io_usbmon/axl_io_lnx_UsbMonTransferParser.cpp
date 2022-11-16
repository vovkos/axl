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
	const char* p = (char*)p0;
	const char* end = p + size;

	p = buffer(sizeof(mon_bin_hdr), p, end);
	if (m_offset < sizeof(mon_bin_hdr))
		return size;

	m_hiHdr.m_timestamp = sys::getTimestampFromTimeval(m_loHdr.ts_sec, m_loHdr.ts_usec);
	m_hiHdr.m_status = m_loHdr.status;
	m_hiHdr.m_originalSize = m_loHdr.len_urb;
	m_hiHdr.m_captureSize = m_loHdr.len_cap;
	m_hiHdr.m_actualSize = 0;
	m_hiHdr.m_transferType = m_loHdr.xfer_type;
	m_hiHdr.m_bus = m_loHdr.busnum;
	m_hiHdr.m_address = m_loHdr.devnum;
	m_hiHdr.m_endpoint = m_loHdr.epnum;
	m_hiHdr.m_flags = m_loHdr.type == 'S' ? 0 : UsbMonTransferFlag_Completed;

	switch (m_hiHdr.m_transferType) {
	case UsbMonTransferType_Control:
		if (m_loHdr.type == 'S')
			memcpy(&m_hiHdr.m_controlSetup, m_loHdr.s.setup, sizeof(UsbMonControlSetup));
		break;

	case UsbMonTransferType_Isochronous:
		m_hiHdr.m_isochronousHdr.m_startFrame = 0;
		m_hiHdr.m_isochronousHdr.m_packetCount = m_loHdr.s.iso.numdesc;
		m_hiHdr.m_isochronousHdr.m_errorCount = m_loHdr.s.iso.error_count;
		break;
	}

	m_hiHdr.m_actualSize = m_hiHdr.m_captureSize;
	m_state = UsbMonTransferParserState_CompleteHeader;
	m_offset = 0;
	return p - (char*)p0;
}

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
