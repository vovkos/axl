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

enum SharedMemoryTransportKind
{
	SharedMemoryTransportKind_FileSignature    = ':mhs',
	SharedMemoryTransportKind_MessageSignature = ':sm\n',
	SharedMemoryTransportKind_DefMappingSize   = 64 * 1024,   // 64 KB
	SharedMemoryTransportKind_DefSizeLimitHint = 1024 * 1024, // 1 MB
};

enum SharedMemoryTransportStateKind
{
	SharedMemoryTransportStateKind_MasterConnected,
	SharedMemoryTransportStateKind_SlaveConnected,
	SharedMemoryTransportStateKind_Disconnected,
};

enum SharedMemoryTransportFlagKind
{
	SharedMemoryTransportFlagKind_Create  = 0x01,
	SharedMemoryTransportFlagKind_Message = 0x02,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct SharedMemoryransportHdr
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

class SharedMemoryransportBase
{
protected:
	uint_t m_flags;
	MappedFile m_file;
	SharedMemoryransportHdr* m_hdr;
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
	SharedMemoryransportBase ();

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
		return ensureMappingSize (offset + sizeof (SharedMemoryransportHdr));
	}
};

//.............................................................................

class SharedMemoryReader: public SharedMemoryransportBase
{
public:
	size_t
	read (rtl::Array <char>* buffer);

	rtl::Array <char>
	read ();
};

//.............................................................................

class SharedMemoryWriter: public SharedMemoryransportBase
{
protected:
	size_t m_sizeLimitHint;
	mt::Lock m_writeLock; // make write operations atomic

public:
	SharedMemoryWriter ()
	{
		m_sizeLimitHint = SharedMemoryTransportKind_DefSizeLimitHint;
	}

	bool
	open (
		const char* fileName,
		const char* readEventName,
		const char* writeEventName,
		uint_t flags,
		size_t sizeLimitHint = SharedMemoryTransportKind_DefSizeLimitHint
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
