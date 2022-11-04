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
#include "axl_io_lnx_UsbMonitor.h"
#include "axl_sys_Time.h"

namespace axl {
namespace io {
namespace lnx {

//..............................................................................

size_t
UsbMonitor::read(
	void* p,
	size_t size,
	uint_t timeout
) {
	if (size < sizeof(UsbMonTransferHdr))
		return err::fail(err::SystemErrorCode_BufferTooSmall);

	if (m_readBuffer.getCount() <= MinReadBufferSize)
		m_readBuffer.setCount(DefaultReadBufferSize);

	const usbmon::mon_bin_hdr* srcHdr;

	for (;;) {
		size_t result = m_device.read(m_readBuffer, m_readBuffer.getCount());
		if (result == -1)
			return -1;

		if (result < sizeof(usbmon::mon_bin_hdr)) {
			err::setFormatStringError("unexpected usbmon read size: %d", result);
			return -1;
		}

		srcHdr = (usbmon::mon_bin_hdr*)m_readBuffer.cp();
		if (!m_filterAddress || srcHdr->devnum == m_filterAddress)
			break;
	}

	UsbMonTransferHdr* dstHdr = (UsbMonTransferHdr*)p;
	dstHdr->m_timestamp = sys::getTimestampFromTimeval(srcHdr->ts_sec, srcHdr->ts_usec);
	dstHdr->m_status = srcHdr->status;
	dstHdr->m_dataSize = srcHdr->len_cap;
	dstHdr->m_transferType = srcHdr->xfer_type;
	dstHdr->m_bus = srcHdr->busnum;
	dstHdr->m_address = srcHdr->devnum;
	dstHdr->m_endpoint = srcHdr->epnum;
	dstHdr->m_flags = srcHdr->type != 'S' ? UsbMonTransferFlag_Completed : 0;

	switch (dstHdr->m_transferType) {
	case UsbMonTransferType_Control:
		if (srcHdr->type == 'S')
			memcpy(dstHdr->m_controlSetup, srcHdr->s.setup, sizeof(dstHdr->m_controlSetup));
		break;

	case UsbMonTransferType_Isochronous:
		memset(&dstHdr->m_iso, 0, sizeof(dstHdr->m_iso)); // not yet
		break;
	}

	if (!srcHdr->len_cap)
		return sizeof(UsbMonTransferHdr);

	size_t leftoverSize = size - sizeof(UsbMonTransferHdr);
	size_t copySize = AXL_MIN(srcHdr->len_cap, leftoverSize);
	memcpy(dstHdr + 1, srcHdr + 1, copySize);
	return sizeof(UsbMonTransferHdr) + copySize;
}

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
