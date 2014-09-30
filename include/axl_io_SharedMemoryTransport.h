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

enum ESharedMemoryTransport
{
	ESharedMemoryTransport_FileSignature    = ':mhs',
	ESharedMemoryTransport_MessageSignature = ':sm\n',
	ESharedMemoryTransport_DefMappingSize   = 64 * 1024,   // 64 KB
	ESharedMemoryTransport_DefSizeLimitHint = 1024 * 1024, // 1 MB
};

enum ESharedMemoryTransportState
{
	ESharedMemoryTransportState_MasterConnected,
	ESharedMemoryTransportState_SlaveConnected,
	ESharedMemoryTransportState_Disconnected,
};

enum ESharedMemoryTransportFlag
{
	ESharedMemoryTransportFlag_Create  = 0x01,
	ESharedMemoryTransportFlag_Message = 0x02,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TSharedMemoryTransportHdr
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

struct TSharedMemoryTransportMessageHdr
{
	uint32_t m_signature;
	uint32_t m_size;
};

//.............................................................................

class CSharedMemoryTransportBase
{
protected:
	uint_t m_flags;
	CMappedFile m_file;
	TSharedMemoryTransportHdr* m_hdr;
	char* m_data;
	size_t m_mappingSize;
	int32_t m_pendingReqCount;

#if (_AXL_ENV == AXL_ENV_WIN)
	mt::win::CEvent m_readEvent;
	mt::win::CEvent m_writeEvent;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	mt::psx::CSem m_readEvent;
	mt::psx::CSem m_writeEvent;
	rtl::CString m_readEventName;
	rtl::CString m_writeEventName;
#endif

protected:
	CSharedMemoryTransportBase ();

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
		return ensureMappingSize (offset + sizeof (TSharedMemoryTransportHdr));
	}
};

//.............................................................................

class CSharedMemoryReader: public CSharedMemoryTransportBase
{
public:
	size_t
	read (rtl::CArrayT <char>* buffer);

	rtl::CArrayT <char>
	read ();
};

//.............................................................................

class CSharedMemoryWriter: public CSharedMemoryTransportBase
{
protected:
	size_t m_sizeLimitHint;
	mt::CLock m_writeLock; // make write operations atomic

public:
	CSharedMemoryWriter ()
	{
		m_sizeLimitHint = ESharedMemoryTransport_DefSizeLimitHint;
	}

	bool
	open (
		const char* fileName,
		const char* readEventName,
		const char* writeEventName,
		uint_t flags,
		size_t sizeLimitHint = ESharedMemoryTransport_DefSizeLimitHint
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
