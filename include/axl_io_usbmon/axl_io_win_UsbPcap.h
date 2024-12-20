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

#define _AXL_IO_WIN_USBPCAP_H

#include "axl_io_win_File.h"
#include <winioctl.h>

namespace axl {
namespace io {
namespace win {
namespace usbpcap {

// excerpt from USBPcap.h

#pragma pack(push)
#pragma pack(1)

#ifndef __USB_H__
typedef LONG USBD_STATUS;
#endif

//..............................................................................

enum {
	IOCTL_USBPCAP_SETUP_BUFFER     = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS),
	IOCTL_USBPCAP_START_FILTERING  = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS),
	IOCTL_USBPCAP_STOP_FILTERING   = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS),
	IOCTL_USBPCAP_GET_HUB_SYMLINK  = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS),
	IOCTL_USBPCAP_SET_SNAPLEN_SIZE = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_READ_ACCESS),

	DLT_USBPCAP = 249,

	USBPCAP_TRANSFER_ISOCHRONOUS = 0,
	USBPCAP_TRANSFER_INTERRUPT   = 1,
	USBPCAP_TRANSFER_CONTROL     = 2,
	USBPCAP_TRANSFER_BULK        = 3,
	USBPCAP_TRANSFER_IRP_INFO    = 0xFE,
	USBPCAP_TRANSFER_UNKNOWN     = 0xFF,

	USBPCAP_INFO_PDO_TO_FDO = (1 << 0),

	USBPCAP_CONTROL_STAGE_SETUP    = 0,
	USBPCAP_CONTROL_STAGE_DATA     = 1,
	USBPCAP_CONTROL_STAGE_STATUS   = 2,
	USBPCAP_CONTROL_STAGE_COMPLETE = 3,
};

struct USBPCAP_IOCTL_SIZE {
	UINT32 size;
};

struct USBPCAP_ADDRESS_FILTER {
	UINT32 addresses[4];
	BOOLEAN filterAll;
};

struct pcap_hdr_t {
	UINT32 magic_number;
	UINT16 version_major;
	UINT16 version_minor;
	INT32 thiszone;
	UINT32 sigfigs;
	UINT32 snaplen;
	UINT32 network;
};

struct pcaprec_hdr_t {
	UINT32 ts_sec;
	UINT32 ts_usec;
	UINT32 incl_len;
	UINT32 orig_len;
};

struct USBPCAP_BUFFER_PACKET_HEADER {
	USHORT headerLen;
	UINT64 irpId;
	USBD_STATUS status;
	USHORT function;
	UCHAR info;
	USHORT bus;
	USHORT device;
	UCHAR endpoint;
	UCHAR transfer;
	UINT32 dataLength;
};

struct USBPCAP_BUFFER_CONTROL_HEADER {
	USBPCAP_BUFFER_PACKET_HEADER header;
	UCHAR stage;
};

struct USBPCAP_BUFFER_ISO_PACKET {
	ULONG offset;
	ULONG length;
	USBD_STATUS status;
};

struct USBPCAP_BUFFER_ISOCH_HEADER {
	USBPCAP_BUFFER_PACKET_HEADER header;
	ULONG startFrame;
	ULONG numberOfPackets;
	ULONG errorCount;

	// followed by USBPCAP_BUFFER_ISO_PACKET[numberOfPackets];
};

union BM_REQUEST_TYPE {
	struct BM {
		UCHAR Recipient : 2;
		UCHAR Reserved  : 3;
		UCHAR Type      : 2;
		UCHAR Dir       : 1;
	} s;
	UCHAR B;
};

struct USB_DEFAULT_PIPE_SETUP_PACKET {
	BM_REQUEST_TYPE bmRequestType;
	UCHAR bRequest;

	union _wValue {
		struct {
			UCHAR LowByte;
			UCHAR HiByte;
		};
		USHORT W;
	} wValue;

	union _wIndex {
		struct {
			UCHAR LowByte;
			UCHAR HiByte;
		};
		USHORT W;
	} wIndex;

	USHORT wLength;
};

//..............................................................................

#pragma pack(pop)

} // namespace usbpcap

class UsbPcap {
public:
	enum {
		// as suggested by USBPcapCMD

		IoctlOutputBufferSize   = 1024,             // 1K
		DefaultKernelBufferSize = 1 * 1024 * 1024,  // 1M
		DefaultSnapshotLength   = 65535,            // 64K - 1
	};

	enum Mode {
		Mode_Capture,
		Mode_Enumerate,
	};

protected:
	axl::io::win::File m_device;

public:
	bool
	isOpen() const {
		return m_device.isOpen();
	}

	bool
	open(
		const sl::String_w& name,
		Mode mode = Mode_Capture
	);

	void
	close() {
		m_device.close();
	}

	bool
	readPcapHdr();

	sl::String_w
	getHubSymlink();

	bool
	getHubSymlink(sl::String_w* symlink);

	bool
	setSnapshotLength(size_t size);

	bool
	setKernelBufferSize(size_t size);

	bool
	setFilter(uint_t deviceAddress) {
		return setFilter(&deviceAddress, 1);
	}

	bool
	setFilter(
		const uint_t* deviceAddressTable,
		size_t deviceCount
	);

	bool
	clearFilter();

	bool
	stopFiltering();

	size_t
	read(
		void* p,
		size_t size
	) {
		return m_device.overlappedRead(p, size);
	}

	bool
	overlappedRead(
		void* p,
		dword_t size,
		OVERLAPPED* overlapped
	) {
		return m_device.overlappedRead(p, size, overlapped);
	}

	static
	bool
	isOverlappedIoCompleted(OVERLAPPED* overlapped) {
		return File::isOverlappedIoCompleted(overlapped);
	}

	bool
	getOverlappedResult(
		OVERLAPPED* overlapped,
		dword_t* actualSize
	) const {
		return m_device.getOverlappedResult(overlapped, actualSize);
	}

	size_t
	getOverlappedResult(OVERLAPPED* overlapped) {
		return m_device.getOverlappedResult(overlapped);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
sl::String_w
UsbPcap::getHubSymlink() {
	sl::String_w string;
	getHubSymlink(&string);
	return string;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
