// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_SHAREDMEMORYTRANSPORT_H

#include "axl_io_Mapping.h"
#include "axl_sys_Lock.h"
#include "axl_sl_Array.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_sys_win_Event.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_sys_psx_Sem.h"
#endif

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
	SharedMemoryTransportFlag_Create        = 0x01,
	SharedMemoryTransportFlag_Message       = 0x02,
	SharedMemoryTransportFlag_DeleteOnClose = 0x04,
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

struct SharedMemoryTransportMessageHdr
{
	uint32_t m_signature;
	uint32_t m_size;
};

//.............................................................................

class SharedMemoryTransportBase
{
protected:
	uint_t m_flags;
	
	File m_file;
	Mapping m_mapping;
	size_t m_mappingSize;

	SharedMemoryTransportHdr* m_hdr;
	char* m_data;
	int32_t m_pendingReqCount;

#if (_AXL_ENV == AXL_ENV_WIN)
	sys::win::Event m_readEvent;
	sys::win::Event m_writeEvent;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	sys::psx::NamedSem m_readEvent;
	sys::psx::NamedSem m_writeEvent;
	sl::String m_readEventName;
	sl::String m_writeEventName;
#endif

protected:
	SharedMemoryTransportBase ();

	~SharedMemoryTransportBase ()
	{
		close ();
	}

public:
	uint_t
	getFlags ()
	{
		return m_flags;
	}

	File::Handle
	getFileHandle () const
	{
		return m_file.getHandle ();
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

	bool
	attach (
		File::Handle fileHandle,
		const char* readEventName,
		const char* writeEventName,
		uint_t flags
		);

	void
	disconnect ();

protected:
	void
	closeImpl ();

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
	read (sl::Array <char>* buffer);

	sl::Array <char>
	read ();
};

//.............................................................................

class SharedMemoryWriter: public SharedMemoryTransportBase
{
protected:
	size_t m_sizeLimitHint;
	sys::Lock m_writeLock; // make write operations atomic

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

	bool
	attach (
		File::Handle fileHandle,
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
