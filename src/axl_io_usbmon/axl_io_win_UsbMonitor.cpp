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
		m_device.setKernelBufferSize(kernelBufferSize);

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
	m_pcapBuffer.clear();
	m_snapshotLength = pcapHdr.snaplen;
	m_kernelBufferSize = kernelBufferSize;
	m_flags = flags;
	return true;
}

bool
UsbMonitor::overlappedRead(
	void* p,
	size_t size,
	Overlapped* overlapped
) {
	if (size < MinumumReadSize)
		return err::fail(err::SystemErrorCode_BufferTooSmall);

	if (!m_transferList.isEmpty()) {
		overlapped->m_actualSize = copyTransfers(p, size);
		overlapped->m_state = OverlappedState_Completed;
		return overlapped->m_overlapped.m_completionEvent.signal();
	}

	bool result = m_device.overlappedRead(p, size, &overlapped->m_overlapped);
	if (!result) {
		overlapped->m_state = OverlappedState_Completed;
		overlapped->m_actualSize = -1;
		overlapped->m_error = err::getLastError();
		return false;
	}

	overlapped->m_state = OverlappedState_Reading;
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

	ASSERT(
		overlapped->m_bufferSize >= sizeof(pcaprec_hdr_t) + sizeof(USBPCAP_BUFFER_PACKET_HEADER) &&
		overlapped == m_readList.getHead()
	); // wrong use otherwise

	if (overlapped->m_state == OverlappedState_Reading) {
		size_t result = bufferRead(overlapped);
		if (!result)
			return failRead(overlapped);
	}

	if (!m_transferList.isEmpty()) {
		size_t size = copyTransfers(overlapped->m_buffer, overlapped->m_bufferSize);
		return completeRead(overlapped, size);
	}

	size_t size = m_pcapBuffer.getCount();
	if (size < sizeof(pcaprec_hdr_t) + sizeof(USBPCAP_BUFFER_PACKET_HEADER))
		return failRead(overlapped, "incomplete usbpcap packet");

	const char* p = m_pcapBuffer;
	const char* end = p + size;

	do {
		size_t transferSize = parseTransfer(p, size);
		if (transferSize == -1)
			return failRead(overlapped);

		p += transferSize;
		size = end - p;
	} while (size >= sizeof(pcaprec_hdr_t) + sizeof(USBPCAP_BUFFER_PACKET_HEADER));

	memmove(m_pcapBuffer, p, size);
	m_pcapBuffer.setCount(size);

 	size = copyTransfers(overlapped->m_buffer, overlapped->m_bufferSize);
	return completeRead(overlapped, size);
}

size_t
UsbMonitor::bufferRead(Overlapped* overlapped) {
	ASSERT(overlapped->m_state == OverlappedState_Reading);

	size_t result = m_device.getOverlappedResult(&overlapped->m_overlapped);
	if (result == -1)
		return -1;

	result = m_pcapBuffer.append((char*)overlapped->m_buffer, result);
	if (result == -1)
		return -1;

	overlapped->m_state = OverlappedState_Buffered;
	return result;
}

bool
UsbMonitor::fetchTransferData(
	Transfer* transfer,
	size_t offset
) {
	size_t captureSize = transfer->getHdr()->m_captureSize;
	char* data = transfer->m_buffer + sizeof(UsbMonTransferHdr);
	ASSERT(offset < captureSize);

	sl::Iterator<Overlapped> it = m_readList.getHead();

	// 1 - skip already buffered reads

	while (it && it->m_state == OverlappedState_Buffered)
		it++;

	// 2 - buffer pending reads

	m_pcapBuffer.clear();

	while (it) {
		size_t size = bufferRead(*it);
		if (size == -1)
			return false;

		size_t leftover = captureSize - offset;
		if (size >= leftover) { // done!
			memcpy(data + offset, m_pcapBuffer, leftover);
			m_pcapBuffer.remove(0, leftover);
			return true;
		}

		memcpy(data + offset, m_pcapBuffer, size);
		m_pcapBuffer.clear();
		offset += size;
		it++;
	}

	// 3 - still not enough -- issue one *exact* read to usbpcap

	size_t leftover = captureSize - offset;
	size_t result = m_device.read(data + offset, leftover);
	ASSERT(result == -1 || result == leftover);
	return result != -1;
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
	Transfer* transfer = m_transferPool.get();
	if (!transfer)
		return -1;

	sl::List<Transfer> transferList;
	transferList.insertTail(transfer);
	bool result = transfer->m_buffer.setCount(sizeof(UsbMonTransferHdr) + dataSize);
	if (!result)
		return -1;

	UsbMonTransferHdr* hdr = transfer->getHdr();
	transfer->m_readOffset = 0;
	hdr->m_timestamp = sys::getTimestampFromTimeval(pcapHdr->ts_sec, pcapHdr->ts_usec);
	hdr->m_status = err::SystemErrorCode_Success;
	hdr->m_originalSize = packetHdr->dataLength;
	hdr->m_captureSize = dataSize;
	hdr->m_actualSize = dataSize;
	hdr->m_transferType = (UsbMonTransferType)packetHdr->transfer;
	hdr->m_bus = (uint8_t)packetHdr->bus;
	hdr->m_address = (uint8_t)packetHdr->device;
	hdr->m_endpoint = packetHdr->endpoint;
	hdr->m_flags = (packetHdr->info & USBPCAP_INFO_PDO_TO_FDO) ? UsbMonTransferFlag_Completed : 0;

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
			memcpy(&hdr->m_controlSetup, data, sizeof(UsbMonControlSetup));
			data += sizeof(UsbMonControlSetup);
			dataSize -= sizeof(UsbMonControlSetup);

			hdr->m_originalSize = hdr->m_controlSetup.m_length;
			hdr->m_captureSize = dataSize;
			hdr->m_actualSize = dataSize;
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
		hdr->m_isochronousHdr.m_startFrame = isoHdr->startFrame;
		hdr->m_isochronousHdr.m_packetCount = isoHdr->numberOfPackets;
		hdr->m_isochronousHdr.m_errorCount = isoHdr->errorCount;
		break;
	}

	const char* end;

	if (!dataSize)
		end = data;
	else {
		size_t availableDataSize = size - sizeof(pcaprec_hdr_t) - packetHdr->headerLen;
		if (availableDataSize >= dataSize) { // this transfer is buffered completely
			memcpy(hdr + 1, data, dataSize);
			end = data + dataSize;
		} else { // not buffered completely -- fetch this (and only this!) data from usbpcap
			memcpy(hdr + 1, data, availableDataSize);
			end = (char*)p0 + size;

			bool result = fetchTransferData(transfer, availableDataSize);
			if (!result)
				return -1;
		}
	}

	m_transferList.insertListTail(&transferList);
	return end - (char*)p0;
}

size_t
UsbMonitor::copyTransfers(
	void* p0,
	size_t size
) {
	ASSERT(size >= sizeof(UsbMonTransferHdr));
	ASSERT(!m_transferList.isEmpty());

	Transfer* transfer = *m_transferList.getHead();
	UsbMonTransferHdr* hdr = transfer->getHdr();

	if (m_flags & Flag_MessageMode) {
		size_t copySize = sizeof(UsbMonTransferHdr) + hdr->m_captureSize;
		if (copySize > size) {
			hdr->m_actualSize = size - sizeof(UsbMonTransferHdr);
			copySize = size;
		}

		memcpy(p0, transfer->m_buffer, copySize);
		m_transferList.remove(transfer);
		m_transferPool.put(transfer);
		return copySize;
	}

	// stream mode

	ASSERT(transfer->m_readOffset < sizeof(UsbMonTransferHdr) + hdr->m_captureSize);

	char* p = (char*)p0;
	char* end = p + size;
	while (p < end) {
		size = end - p;
		size_t leftover = sizeof(UsbMonTransferHdr) + hdr->m_captureSize - transfer->m_readOffset;
		if (size < leftover) {
			memcpy(p, transfer->m_buffer + transfer->m_readOffset, size);
			transfer->m_readOffset += size;
			return size;
		}

		memcpy(p, transfer->m_buffer + transfer->m_readOffset, leftover);
		p += leftover;

		m_transferList.remove(transfer);
		m_transferPool.put(transfer);
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
