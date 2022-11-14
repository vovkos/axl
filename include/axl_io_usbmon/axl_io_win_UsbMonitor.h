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

#define _AXL_IO_WIN_USBMONITOR_H

#include "axl_io_UsbMonTransfer.h"
#include "axl_io_win_UsbPcap.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

class UsbMonitor {
public:
	enum {
		MinimumKernelBufferSize = 1 * 1024,
	};

	enum Flag {
		Flag_MessageMode = 0x01, // read returns a single transfer
	};

	enum OverlappedState {
		OverlappedState_Undefined,
		OverlappedState_Completed,
		OverlappedState_Pending,
	};

	class Overlapped: public sl::ListLink {
		friend class UsbMonitor;

	protected:
		void* m_buffer;
		size_t m_bufferSize;
		size_t m_actualSize;
		err::Error m_error;
		win::StdOverlapped m_overlapped;
		OverlappedState m_state;

	public:
		Overlapped();

		OverlappedState
		getState() const {
			return m_state;
		}

		sys::NotificationEvent&
		getCompletionEvent() {
			return m_overlapped.m_completionEvent;
		}
	};

protected:
	struct Transfer: sl::ListLink {
		size_t m_readOffset;
		UsbMonTransferHdr m_hdr;
		char m_data[1];
	};

protected:
	win::UsbPcap m_device;
	sl::AuxList<Overlapped> m_readList;
	sl::List<Transfer> m_transferList;
	sl::Array<char> m_readBuffer;
	size_t m_readBufferOffset;
	size_t m_snapshotLength;
	uint_t m_flags;

public:
	UsbMonitor();

	bool
	isOpen() const {
		return m_device.isOpen();
	}

	bool
	open(
		const sl::String& captureDeviceName,
		size_t snapshotLength,
		size_t kernelBufferSize,
		uint_t addressFilter,
		uint_t flags = 0
	);

	bool
	open(
		const sl::String& captureDeviceName,
		uint_t filterAddress,
		uint_t flags = 0
	) {
		return open(
			captureDeviceName,
			win::UsbPcap::DefaultSnapshotLength,
			win::UsbPcap::DefaultKernelBufferSize,
			filterAddress,
			flags
		);
	}

	void
	close() {
		m_device.close();
		m_transferList.clear();
	}

	size_t
	getSnapshotLength() const {
		return m_snapshotLength;
	}

	size_t
	getKernelBufferSize() const {
		return m_readBuffer.getCount();
	}

	bool
	setKernelBufferSize(size_t size) {
		return
			m_readBuffer.setCount(size) &&
			m_device.setKernelBufferSize(size);
	}

	bool
	setFilter(uint_t address) {
		return m_device.setFilter(address);
	}

	size_t
	read(
		void* p,
		size_t size
	);

	bool
	overlappedRead(
		void* p,
		size_t size,
		Overlapped* overlapped
	);

	static
	bool
	isOverlappedIoCompleted(Overlapped* overlapped) {
		return UsbPcap::isOverlappedIoCompleted(&overlapped->m_overlapped);
	}

	size_t
	getOverlappedResult(Overlapped* overlapped);

protected:
	void
	finalizePendingRead(
		Overlapped* overlapped,
		size_t actualSize
	);

	size_t
	copyTransfers(
		void* p,
		size_t size
	);

	size_t
	parseTransfer(
		const void* p,
		size_t size
	);

	void
	removePendingRead(Overlapped* overlapped);

	size_t
	completePendingRead(
		Overlapped* overlapped,
		size_t actualSize
	);

	size_t
	failPendingRead(Overlapped* overlapped);

	size_t
	failPendingRead(
		Overlapped* overlapped,
		const err::ErrorRef& error
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
UsbMonitor::Overlapped::Overlapped() {
	m_buffer = NULL;
	m_bufferSize = 0;
	m_actualSize = 0;
	m_state = OverlappedState_Undefined;
}

inline
UsbMonitor::UsbMonitor() {
	m_readBufferOffset = 0;
	m_snapshotLength = 0;
	m_flags = 0;
}

inline
size_t
UsbMonitor::read(
	void* p,
	size_t size
) {
	Overlapped overlapped;
	bool result = overlappedRead(p, size, &overlapped);
	return result ? getOverlappedResult(&overlapped) : - 1;
}

inline
void
UsbMonitor::removePendingRead(Overlapped* overlapped) {
	ASSERT(overlapped->m_state == OverlappedState_Pending);
	overlapped->m_state = OverlappedState_Completed;
	m_readList.remove(overlapped);
}

inline
size_t
UsbMonitor::completePendingRead(
	Overlapped* overlapped,
	size_t actualSize
) {
	overlapped->m_actualSize = actualSize;
	removePendingRead(overlapped);
	return actualSize;
}

inline
size_t
UsbMonitor::failPendingRead(Overlapped* overlapped) {
	overlapped->m_actualSize = -1;
	overlapped->m_error = err::getLastError();
	removePendingRead(overlapped);
	m_readBufferOffset = 0;
	return -1;
}

inline
size_t
UsbMonitor::failPendingRead(
	Overlapped* overlapped,
	const err::ErrorRef& error
) {
	overlapped->m_actualSize = -1;
	overlapped->m_error = error;
	removePendingRead(overlapped);
	setError(error);
	m_readBufferOffset = 0;
	return -1;
}


//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
