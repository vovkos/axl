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

using namespace usbmon;

//..............................................................................

bool
UsbMonitor::open(
	const sl::String& captureDeviceName,
	size_t kernelBufferSize,
	uint_t filterAddress
) {
	if (kernelBufferSize < MinimumKernelBufferSize)
		kernelBufferSize = MinimumKernelBufferSize;

	close();

	return
		m_device.open(captureDeviceName, O_RDWR | O_NONBLOCK) &&
		m_device.setKernelBufferSize(kernelBufferSize) &&
		m_readBuffer.setCount(kernelBufferSize);
}

bool
UsbMonitor::open(
	const sl::String& captureDeviceName,
	uint_t filterAddress
) {
	close();

	bool result = m_device.open(captureDeviceName, O_RDWR | O_NONBLOCK);
	if (!result)
		return false;

	size_t kernelBufferSize = m_device.getKernelBufferSize();
	return m_readBuffer.setCount(kernelBufferSize);
}

size_t
UsbMonitor::read(
	void* p,
	size_t size,
	uint_t timeout
) {
	if (size < sizeof(UsbMonTransferHdr))
		return err::fail<size_t>(-1, err::SystemErrorCode_BufferTooSmall);

	if (!isOpen())
		return err::fail<size_t>(-1, err::SystemErrorCode_InvalidDeviceState);

	usbmon::mon_bin_hdr_ex srcHdr;
	size_t result;

	for (;;) {
		result = get(&srcHdr, timeout);
		if (result == -1 || result == 0)
			return result;

		if (!m_filterAddress || srcHdr.devnum == m_filterAddress)
			break;
	}

	UsbMonTransferHdr* dstHdr = (UsbMonTransferHdr*)p;
	fillUsbMonTransferHdr(dstHdr, &srcHdr);

	const void* data = m_readBuffer;
	size_t dataSize = result - sizeof(srcHdr);

	if (!dataSize)
		return sizeof(UsbMonTransferHdr);

	size_t leftoverSize = size - sizeof(UsbMonTransferHdr);
	size_t copySize = AXL_MIN(dataSize, leftoverSize);
	memcpy(dstHdr + 1, data, dataSize);
	dstHdr->m_actualSize = copySize;
	return sizeof(UsbMonTransferHdr) + copySize;
}

size_t
UsbMonitor::get(
	usbmon::mon_bin_hdr_ex* hdr,
	uint_t timeout
) {
	size_t result;

	if (timeout == 0) { // shortcut
		result = m_device.get(hdr, m_readBuffer, m_readBuffer.getCount());
		return
			result != -1 ? result + sizeof(usbmon::mon_bin_hdr_ex) :
			errno == EWOULDBLOCK ? 0 :
			-1;
	}

	fd_set readSet = { 0 };
	FD_SET(m_device, &readSet);

	if (timeout == -1)
		result = ::select(m_device + 1, &readSet, NULL, NULL, NULL);
	else {
		timeval tv;
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;

		result = ::select(m_device + 1, &readSet, NULL, NULL, &tv);
		if (result == 0)
			return 0;
	}

	if (result == -1) {
		err::setLastSystemError();
		return -1;
	}

	result = m_device.get(hdr, m_readBuffer, m_readBuffer.getCount());
	return
		result != -1 ? result + sizeof(usbmon::mon_bin_hdr_ex) :
		errno == EWOULDBLOCK ? 0 :
		-1;
}

void
UsbMonitor::fillUsbMonTransferHdr(
	UsbMonTransferHdr* dstHdr,
	const usbmon::mon_bin_hdr_ex* srcHdr
) {
	dstHdr->m_timestamp = sys::getTimestampFromTimeval(srcHdr->ts_sec, srcHdr->ts_usec);
	dstHdr->m_status = srcHdr->status;
	dstHdr->m_originalSize = srcHdr->len_urb;
	dstHdr->m_captureSize = srcHdr->len_cap;
	dstHdr->m_actualSize = 0;
	dstHdr->m_transferType = srcHdr->xfer_type;
	dstHdr->m_bus = srcHdr->busnum;
	dstHdr->m_address = srcHdr->devnum;
	dstHdr->m_endpoint = srcHdr->epnum;
	dstHdr->m_flags = srcHdr->type == 'S' ? 0 : UsbMonTransferFlag_Completed;

	switch (dstHdr->m_transferType) {
	case UsbMonTransferType_Control:
		if (srcHdr->type == 'S')
			memcpy(&dstHdr->m_controlSetup, srcHdr->s.setup, sizeof(UsbMonControlSetup));
		break;

	case UsbMonTransferType_Isochronous:
		dstHdr->m_isochronousHdr.m_startFrame = srcHdr->start_frame;
		dstHdr->m_isochronousHdr.m_packetCount = srcHdr->s.iso.numdesc;
		dstHdr->m_isochronousHdr.m_errorCount = srcHdr->s.iso.error_count;
		break;
	}
}

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
