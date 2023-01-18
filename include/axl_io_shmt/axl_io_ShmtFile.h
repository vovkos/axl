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

#define _AXL_IO_SHMTFILE_H

#include "axl_io_ShmtPch.h"

namespace axl {
namespace io {

//..............................................................................

enum ShmtConst {
	ShmtConst_FileSignature    = ':mhs',
	ShmtConst_MessageSignature = ':sm\n',
	ShmtConst_DefMappingSize   = 64 * 1024,   // 64 KB
	ShmtConst_DefSizeLimitHint = 1024 * 1024, // 1 MB
};

enum ShmtState {
	ShmtState_MasterConnected = 0x01,
	ShmtState_SlaveConnected  = 0x02,
	ShmtState_Disconnected    = 0x04,
};

enum ShmtFlag { // should be combined with FileFlag
	ShmtFlag_Message = 0x010000,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct ShmtFileHdr {
	uint32_t m_signature;
	uint32_t _m_padding;
	int32_t m_lock;
	uint32_t m_state;
	uint32_t m_readSemaphoreWaitCount;  // to prevent semaphore overflow
	uint32_t m_writeSemaphoreWaitCount; // to prevent semaphore overflow
	uint32_t m_readOffset;   // only modified by reader
	uint32_t m_writeOffset;  // only modified by writer
	uint32_t m_endOffset;    // only modified by writer
	uint32_t m_dataSize;     // modified by both reader and writer
};

struct ShmtMessageHdr {
	uint32_t m_signature;
	uint32_t m_size;
};

//..............................................................................

} // namespace io
} // namespace axl
