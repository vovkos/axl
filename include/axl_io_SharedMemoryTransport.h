// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SHAREDMEMORYTRANSPORT_H

#include "axl_io_MappedFile.h"
#include "axl_io_Mapping.h"
#include "axl_mt_Event.h"
#include "axl_mt_Lock.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace io {

//.............................................................................

enum SharedMemoryTransportConst
{
	SharedMemoryTransportConst_FileSignature    = ':mhs',
	SharedMemoryTransportConst_MessageSignature = ':sm\n',
	SharedMemoryTransportConst_DefMappingSize   = 64 * 1024,   // 64 KB
	SharedMemoryTransportConst_DefSizeLimitHint = 1024 * 1024, // 1 MB
};

enum SharedMemoryTransportState
{
	SharedMemoryTransportState_MasterConnected,
	SharedMemoryTransportState_SlaveConnected,
	SharedMemoryTransportState_Disconnected,
};

enum SharedMemoryTransportFlag
{
	SharedMemoryTransportFlag_Create  = 0x01,
	SharedMemoryTransportFlag_Message = 0x02,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct SharedMemoryTransportHdr
{
	uint32_t m_signature;
	int32_t m_lock;
	uint32_t m_state;
	uint32_t m_readOffset;   // only modified by reader
	uint32_t m_writeOffset;  // only modified by writer
	uint32_t m_endOffset;    // only modified by writer
	uint32_t m_dataSize;     // modified by both reader and writer
	uint32_t m_padding;
};

struct SharedMemoryransportMessageHdr
{
	uint32_t m_signature;
	uint32_t m_size;
};

//.............................................................................

class SharedMemoryTransportBase
{
protected:
	uint_t m_flags;
	MappedFile m_file;
	SharedMemoryTransportHdr* m_hdr;
	char* m_data;
	size_t m_mappingSize;
	int32_t m_pendingReqCount;

#if (_AXL_ENV == AXL_ENV_WIN)
	mt::win::Event m_readEvent;
	mt::win::Event m_writeEvent;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	mt::psx::Sem m_readEvent;
	mt::psx::Sem m_writeEvent;
	rtl::String m_readEventName;
	rtl::String m_writeEventName;
#endif

protected:
	SharedMemoryTransportBase ();

public:
	uint_t
	getFlags ()
	{
		return m_flags;
	}

	bool
	isOpen ()
	{
		return m_hdr != NULL;
	}

	void
	close ();

	bool
	open (
		const char* fileName,
		const char* readEventName,
		const char* writeEventName,
		uint_t flags
		);

	void
	disconnect ();

protected:
	bool
	ensureMappingSize (size_t size);

	bool
	ensureOffsetMapped (size_t offset)
	{
		return ensureMappingSize (offset + sizeof (SharedMemoryTransportHdr));
	}
};

//.............................................................................

class SharedMemoryReader: public SharedMemoryTransportBase
{
public:
	size_t
	read (rtl::Array <char>* buffer);

	rtl::Array <char>
	read ();
};

//.............................................................................

class SharedMemoryWriter: public SharedMemoryTransportBase
{
protected:
	size_t m_sizeLimitHint;
	mt::Lock m_writeLock; // make write operations atomic

public:
	SharedMemoryWriter ()
	{
		m_sizeLimitHint = SharedMemoryTransportConst_DefSizeLimitHint;
	}

	bool
	open (
		const char* fileName,
		const char* readEventName,
		const char* writeEventName,
		uint_t flags,
		size_t sizeLimitHint = SharedMemoryTransportConst_DefSizeLimitHint
		);

	size_t
	write (
		const void* p,
		size_t size
		)
	{
		return write (&p, &size, 1);
	}

	size_t
	write (
		const void* const* blockArray,
		const size_t* sizeArray,
		size_t count
		);

protected:
	static
	void
	copyWriteChain (
		void* dst,
		const void* const* blockArray,
		const size_t* sizeArray,
		size_t count
		);
};
//.............................................................................

} // namespace io
} // namespace axl
