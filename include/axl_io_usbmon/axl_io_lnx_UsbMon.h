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

#define _AXL_IO_LNX_USBMON_H

#include "axl_sl_String.h"
#include "axl_io_psx_File.h"

namespace axl {
namespace io {
namespace lnx {
namespace usbmon {

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// excerpts from linux/drivers/usb/mon/mon_bin.c:

//..............................................................................

enum {
	SETUP_LEN = 8,
};

struct mon_bin_hdr {
	u64 id;
	unsigned char type;
	unsigned char xfer_type;
	unsigned char epnum;
	unsigned char devnum;
	unsigned short busnum;
	char flag_setup;
	char flag_data;
	s64 ts_sec;
	s32 ts_usec;
	int status;
	unsigned int len_urb;
	unsigned int len_cap;
	union {
		unsigned char setup[SETUP_LEN];
		struct iso_rec {
			int error_count;
			int numdesc;
		} iso;
	} s;
};

struct mon_bin_hdr_ex: mon_bin_hdr {
	int interval;
	int start_frame;
	unsigned int xfer_flags;
	unsigned int ndesc;
};

struct mon_bin_isodesc {
	int iso_status;
	unsigned int iso_off;
	unsigned int iso_len;
	u32 _pad;
};

struct mon_bin_stats {
	u32 queued;
	u32 dropped;
};

struct mon_bin_get {
	struct mon_bin_hdr* hdr;
	void* data;
	size_t alloc;
};

struct mon_bin_mfetch {
	u32* offvec;
	u32 nfetch;
	u32 nflush;
};

enum {
	MON_IOC_MAGIC      = 0x92,
	MON_IOCQ_URB_LEN   = _IO(MON_IOC_MAGIC,   1),
	MON_IOCG_STATS     = _IOR(MON_IOC_MAGIC,  3,  struct mon_bin_stats),
	MON_IOCT_RING_SIZE = _IO(MON_IOC_MAGIC,   4),
	MON_IOCQ_RING_SIZE = _IO(MON_IOC_MAGIC,   5),
	MON_IOCX_GET       = _IOW(MON_IOC_MAGIC,  6,  struct mon_bin_get),
	MON_IOCX_MFETCH    = _IOWR(MON_IOC_MAGIC, 7,  struct mon_bin_mfetch),
	MON_IOCH_MFLUSH    = _IO(MON_IOC_MAGIC,   8),
	MON_IOCX_GETX      = _IOW(MON_IOC_MAGIC,  10, struct mon_bin_get),
};

//..............................................................................

} // namespace usbmon

class UsbMon: public axl::io::psx::File {
public:
	size_t
	getKernelBufferSize() const {
		return ((UsbMon*)(this))->ioctl(usbmon::MON_IOCQ_RING_SIZE);
	}

	bool
	setKernelBufferSize(size_t size) const {
		return ((UsbMon*)(this))->ioctl(usbmon::MON_IOCT_RING_SIZE, size) != -1;
	}

	size_t
	getNextUrbSize() const {
		return ((UsbMon*)(this))->ioctl(usbmon::MON_IOCQ_URB_LEN);
	}

	bool
	getStats(usbmon::mon_bin_stats* stats) const {
		return ((UsbMon*)(this))->ioctl(usbmon::MON_IOCG_STATS, stats);
	}

	size_t
	flush(size_t eventCount = -1) {
		return ioctl(usbmon::MON_IOCH_MFLUSH, eventCount);
	}

	size_t
	get(
		usbmon::mon_bin_hdr* hdr,
		void* data,
		size_t size
	) {
		return get(usbmon::MON_IOCX_GET, hdr, data, size);
	}

	size_t
	get(
		usbmon::mon_bin_hdr_ex* hdr,
		void* data,
		size_t size
	) {
		return get(usbmon::MON_IOCX_GETX, hdr, data, size);
	}

protected:
	size_t
	get(
		uint_t code,
		usbmon::mon_bin_hdr* hdr,
		void* data,
		size_t size
	);
};

//..............................................................................

} // namespace lnx
} // namespace io
} // namespace axl
