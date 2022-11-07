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

#define _AXL_IO_USBMONITORBASE_H

#include "axl_io_UsbMonTransfer.h"
#include "axl_io_File.h"
#include "axl_sl_BoxList.h"
#include "axl_sl_HashTable.h"

namespace axl {
namespace io {

//..............................................................................

enum UsbMonitorFlag {
	UsbMonitorFlag_Asynchronous           = FileFlag_Asynchronous,
	UsbMonitorFlag_CompletedTransfersOnly = 0x010000,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbMonitorBase {
public:
	enum {
		MinReadBufferSize     = 256,
		DefaultReadBufferSize = 1 * 1024 * 1024, // 1M
	};

protected:
	struct Transfer:
		sl::ListLink,
		UsbMonTransferHdr {
	};

	sl::List<Transfer> m_transferList;
	sl::SimpleHashTable<uint64_t, Transfer*> m_transferMap;
	sl::Array<char> m_readBuffer;
	uint_t m_openFlags;

public:
	UsbMonitorBase() {
		m_openFlags = 0;
	}

	size_t
	getReadBufferSize() {
		return m_readBuffer.getCount();
	}

	bool
	setReadBufferSize(size_t size) {
		return m_readBuffer.setCount(size);
	}

	size_t
	getTransferCount() {
		return m_transferList.getCount();
	}

protected:
	void
	clearTransfers() {
		m_transferList.clear();
		m_transferMap.clear();
	}
};

//..............................................................................

} // namespace io
} // namespace axl
