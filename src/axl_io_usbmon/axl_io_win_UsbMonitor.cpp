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
#include "axl_io_win_UsbMonitor.h"
#include "axl_sys_Time.h"

namespace axl {
namespace io {
namespace win {

using namespace usbpcap;

//..............................................................................

bool
UsbMonitor::open(
	const sl::String& captureDeviceName,
	size_t snapshotLength,
	size_t kernelBufferSize,
	uint_t addressFilter,
	uint_t flags
) {
	close();

	if (kernelBufferSize < MinimumKernelBufferSize)
		kernelBufferSize = MinimumKernelBufferSize;

	bool result =
		m_device.open(captureDeviceName) &&
		m_device.setFilter(addressFilter) &&
		m_device.setSnapshotLength(snapshotLength) &&
		m_device.setKernelBufferSize(kernelBufferSize) &&
		m_readBuffer.setCount(kernelBufferSize); // enough to read the whole kernel buffer

	if (!result)
		return false;

	pcap_hdr_t pcapHdr = { 0 };
	size_t size = m_device.read(&pcapHdr, sizeof(pcapHdr));
	if (size == -1)
		return false;

	if (size != sizeof(pcapHdr) ||
		pcapHdr.magic_number != 0xa1b2c3d4 ||
		pcapHdr.network != DLT_USBPCAP
	)
		return err::fail("invalid pcap file header");

	ASSERT(pcapHdr.thiszone == 0);
	ASSERT(pcapHdr.snaplen == snapshotLength);

	m_readList.clear();
	m_transferList.clear();
	m_readBufferOffset = 0;
	m_snapshotLength = pcapHdr.snaplen;
	m_flags = flags;
	return true;
}

bool
UsbMonitor::overlappedRead(
	void* p,
	size_t size,
	Overlapped* overlapped
) {
	if ((m_flags & Flag_MessageMode) && size < sizeof(UsbMonTransferHdr))
		return err::fail(err::SystemErrorCode_BufferTooSmall);

	if (!m_transferList.isEmpty()) {
		overlapped->m_actualSize = copyTransfers(p, size);
		overlapped->m_state = OverlappedState_Completed;
		return true;
	}

	if (!m_readList.isEmpty()) {
		ASSERT(m_readBufferOffset < m_readBuffer.getCount());

		bool result = m_device.overlappedRead(
			m_readBuffer + m_readBufferOffset,
			m_readBuffer.getCount() - m_readBufferOffset,
			&overlapped->m_overlapped
		);

		if (!result)
			return false;

		if (m_device.isOverlappedIoCompleted(&overlapped->m_overlapped)) {
			overlapped->m_state = OverlappedState_Completed;
			overlapped->m_actualSize = m_device.getOverlappedResult(&overlapped->m_overlapped);
			if (overlapped->m_actualSize == -1) {
				overlapped->m_error = err::getLastError();
				return false;
			}

			return true;
		}
	}

	overlapped->m_state = OverlappedState_Pending;
	overlapped->m_buffer = p;
	overlapped->m_bufferSize = size;
	m_readList.insertTail(overlapped);
	return true;
}

size_t
UsbMonitor::getOverlappedResult(Overlapped* overlapped) {
	if (overlapped->m_state == OverlappedState_Completed) {
		if (overlapped->m_actualSize == -1)
			err::setError(overlapped->m_error);

		return overlapped->m_actualSize;
	}

	ASSERT(overlapped == m_readList.getHead()); // wrong use otherwise

	if (!m_transferList.isEmpty()) {
		size_t size = copyTransfers(overlapped->m_buffer, overlapped->m_bufferSize);
		return completePendingRead(overlapped, size);
	}

	size_t size = m_device.getOverlappedResult(&overlapped->m_overlapped);
	if (size == -1)
		return failPendingRead(overlapped);

	size += m_readBufferOffset;
	m_readBufferOffset = 0;

	// the very first transfer gets a special hanlding -- we make sure it's fully fetched and parsed

	while (size < sizeof(pcaprec_hdr_t) + sizeof(USBPCAP_BUFFER_PACKET_HEADER)) {
		size_t extraSize = m_device.read(
			m_readBuffer + size,
			m_readBuffer.getCount() - size
		);

		if (extraSize == -1)
			return failPendingRead(overlapped);

		size += extraSize;
	}

	const char* end = m_readBuffer + size;
	size_t transferSize = parseTransfer(m_readBuffer, size);
	if (transferSize == -1)
		return failPendingRead(overlapped);

	// parse remaining transfers in the buffer (

	const char* p = m_readBuffer + transferSize;
	while (p < end) {
		size = end - p;
		if (size < sizeof(pcaprec_hdr_t) + sizeof(USBPCAP_BUFFER_PACKET_HEADER)) // not enough -- stop
			break;

		transferSize = parseTransfer(p, size);
		if (transferSize == -1)
			break;

		p += transferSize;
	}

	m_readBufferOffset = end - p;
	memmove(m_readBuffer, p, m_readBufferOffset);

 	size = copyTransfers(overlapped->m_buffer, overlapped->m_bufferSize);
	return completePendingRead(overlapped, size);
}

size_t
UsbMonitor::parseTransfer(
	const void* p0,
	size_t size
) {
	ASSERT(size >= sizeof(pcaprec_hdr_t) + sizeof(USBPCAP_BUFFER_PACKET_HEADER));

	const pcaprec_hdr_t* pcapHdr = (pcaprec_hdr_t*)p0;
	const USBPCAP_BUFFER_PACKET_HEADER* packetHdr = (USBPCAP_BUFFER_PACKET_HEADER*)(pcapHdr + 1);
	if (pcapHdr->incl_len < sizeof(USBPCAP_BUFFER_PACKET_HEADER) ||
		packetHdr->headerLen < sizeof(USBPCAP_BUFFER_PACKET_HEADER) ||
		size < sizeof(pcaprec_hdr_t) + packetHdr->headerLen
	)
		return err::fail<size_t>(-1, "invalid usbpcap packet header");

	const char* data = (char*)packetHdr + packetHdr->headerLen;
	size_t dataSize = packetHdr->dataLength;
	Transfer* transfer = AXL_MEM_NEW_EXTRA(Transfer, dataSize);
	if (!transfer)
		return -1;

	transfer->m_readOffset = 0;
	transfer->m_hdr.m_timestamp = sys::getTimestampFromTimeval(pcapHdr->ts_sec, pcapHdr->ts_usec);
	transfer->m_hdr.m_status = err::SystemErrorCode_Success;
	transfer->m_hdr.m_originalSize = packetHdr->dataLength;
	transfer->m_hdr.m_captureSize = dataSize;
	transfer->m_hdr.m_actualSize = dataSize;
	transfer->m_hdr.m_transferType = (UsbMonTransferType)packetHdr->transfer;
	transfer->m_hdr.m_bus = (uint8_t)packetHdr->bus;
	transfer->m_hdr.m_address = (uint8_t)packetHdr->device;
	transfer->m_hdr.m_endpoint = packetHdr->endpoint;
	transfer->m_hdr.m_flags = (packetHdr->info & USBPCAP_INFO_PDO_TO_FDO) ? UsbMonTransferFlag_Completed : 0;

	sl::List<Transfer> transferList;
	transferList.insertTail(transfer);

	switch (packetHdr->transfer) {
		const USBPCAP_BUFFER_CONTROL_HEADER* controlHdr;
		const USBPCAP_BUFFER_ISOCH_HEADER* isoHdr;

	case USBPCAP_TRANSFER_CONTROL:
		if (packetHdr->headerLen < sizeof(USBPCAP_BUFFER_CONTROL_HEADER))
			return err::fail<size_t>(-1, "incomplete usbpcap control header");

		controlHdr = (USBPCAP_BUFFER_CONTROL_HEADER*)packetHdr;
		switch (controlHdr->stage) {
		case USBPCAP_CONTROL_STAGE_SETUP:
			if (packetHdr->dataLength < sizeof(USB_DEFAULT_PIPE_SETUP_PACKET) ||
				size < sizeof(pcaprec_hdr_t) + packetHdr->headerLen + sizeof(USB_DEFAULT_PIPE_SETUP_PACKET)
			)
				return err::fail<size_t>(-1, "invalid usbpcap control setup packet");

			ASSERT(sizeof(USB_DEFAULT_PIPE_SETUP_PACKET) == sizeof(UsbMonControlSetup));
			memcpy(&transfer->m_hdr.m_controlSetup, data, sizeof(UsbMonControlSetup));
			data += sizeof(UsbMonControlSetup);
			dataSize -= sizeof(UsbMonControlSetup);

			transfer->m_hdr.m_originalSize = transfer->m_hdr.m_controlSetup.m_length;
			transfer->m_hdr.m_captureSize = dataSize;
			transfer->m_hdr.m_actualSize = dataSize;
			break;

		case USBPCAP_CONTROL_STAGE_DATA:
		case USBPCAP_CONTROL_STAGE_STATUS:
			return err::fail<size_t>(-1, "unexpected usbpcap control stage (usbpcap 1.5.0 or higher is required)");
		}

		break;

	case USBPCAP_TRANSFER_ISOCHRONOUS:
		if (packetHdr->headerLen < sizeof(USBPCAP_BUFFER_ISOCH_HEADER))
			return err::fail<size_t>(-1, "incomplete usbpcap isochronous header");

		isoHdr = (USBPCAP_BUFFER_ISOCH_HEADER*)packetHdr;
		transfer->m_hdr.m_isochronousHdr.m_startFrame = isoHdr->startFrame;
		transfer->m_hdr.m_isochronousHdr.m_packetCount = isoHdr->numberOfPackets;
		transfer->m_hdr.m_isochronousHdr.m_errorCount = isoHdr->errorCount;
		break;
	}

	const char* p = data;

	if (dataSize) {
		size_t availableDataSize = size - sizeof(pcaprec_hdr_t) - packetHdr->headerLen;
		if (availableDataSize >= dataSize) { // this transfer is buffered completely
			memcpy(transfer->m_data, data, dataSize);
			p += dataSize;
		} else { // not buffered completely -- fetch this (and only this!) data from the driver
			memcpy(transfer->m_data, data, availableDataSize);
			p = (char*)p0 + size;

			size_t offset = dataSize;
			while (offset < dataSize) {
				size_t extraSize = m_device.read(
					transfer->m_data + offset,
					dataSize - offset
				);

				if (extraSize == -1)
					return -1;

				offset += extraSize;
			}
		}
	}

	// move

	m_transferList.insertListTail(&transferList);
	return p - (char*)p0;
}

size_t
UsbMonitor::copyTransfers(
	void* p0,
	size_t size
) {
	ASSERT(size >= sizeof(UsbMonTransferHdr));
	ASSERT(!m_transferList.isEmpty());

	if (m_flags & Flag_MessageMode) {
		Transfer* transfer = m_transferList.removeHead();
		size_t copySize = sizeof(UsbMonTransferHdr) + transfer->m_hdr.m_captureSize;
		if (copySize > size) {
			transfer->m_hdr.m_actualSize = size - sizeof(UsbMonTransferHdr);
			copySize = size;
		}

		memcpy(p0, &transfer->m_hdr, copySize);
		AXL_MEM_DELETE(transfer);
		return copySize;
	}

	// stream mode

	Transfer* transfer = *m_transferList.getHead();
	ASSERT(transfer->m_readOffset < sizeof(UsbMonTransferHdr) + transfer->m_hdr.m_captureSize);

	char* p = (char*)p0;
	char* end = p + size;
	while (p < end) {
		size = end - p;
		size_t leftover = sizeof(UsbMonTransferHdr) + transfer->m_hdr.m_captureSize - transfer->m_readOffset;
		if (size < leftover) {
			memcpy(p, (char*)&transfer->m_hdr + transfer->m_readOffset, size);
			transfer->m_readOffset += size;
			return size;
		}

		memcpy(p, (char*)&transfer->m_hdr + transfer->m_readOffset, leftover);
		p += leftover;

		m_transferList.erase(transfer);
		if (m_transferList.isEmpty())
			break;

		transfer = *m_transferList.getHead();
	}

	return p - (char*)p0;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
