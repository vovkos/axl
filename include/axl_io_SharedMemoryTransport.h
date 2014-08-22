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
	ESharedMemoryTransport_MessageSignature = ':gsm',
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
	uint32_t m_Signature;
	int32_t m_Lock;
	uint32_t m_State;
	uint32_t m_ReadOffset;   // only modified by reader
	uint32_t m_WriteOffset;  // only modified by writer
	uint32_t m_EndOffset;    // only modified by writer
	uint32_t m_DataSize;     // modified by both reader and writer
	uint32_t m_Padding;
};

struct TSharedMemoryTransportMessageHdr
{
	uint32_t m_Signature;
	uint32_t m_Size;
};

//.............................................................................

class CSharedMemoryTransportBase
{
protected:
	uint_t m_Flags;
	CMappedFile m_File;
	TSharedMemoryTransportHdr* m_pHdr;
	char* m_pData;
	size_t m_MappingSize;
	int32_t m_PendingReqCount;

#if (_AXL_ENV == AXL_ENV_WIN)
	mt::win::CEvent m_ReadEvent;
	mt::win::CEvent m_WriteEvent;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	mt::psx::CSem m_ReadEvent;
	mt::psx::CSem m_WriteEvent;
	rtl::CString m_ReadEventName;
	rtl::CString m_WriteEventName;
#endif

protected:
	CSharedMemoryTransportBase ();

public:
	uint_t
	GetFlags ()
	{
		return m_Flags;
	}

	bool
	IsOpen ()
	{
		return m_pHdr != NULL;
	}

	void
	Close ();

	bool
	Open (
		const char* pFileName,
		const char* pReadEventName,
		const char* pWriteEventName,
		uint_t Flags
		);

	void
	Disconnect ();

protected:
	bool
	EnsureMappingSize (size_t Size);

	bool
	EnsureOffsetMapped (size_t Offset)
	{
		return EnsureMappingSize (Offset + sizeof (TSharedMemoryTransportHdr));
	}
};

//.............................................................................

class CSharedMemoryReader: public CSharedMemoryTransportBase
{
public:
	size_t
	Read (rtl::CArrayT <char>* pBuffer);

	rtl::CArrayT <char>
	Read ();
};

//.............................................................................

class CSharedMemoryWriter: public CSharedMemoryTransportBase
{
protected:
	size_t m_SizeLimitHint;
	mt::CLock m_WriteLock; // make write operations atomic

public:
	CSharedMemoryWriter ()
	{
		m_SizeLimitHint = ESharedMemoryTransport_DefSizeLimitHint;
	}

	bool
	Open (
		const char* pFileName,
		const char* pReadEventName,
		const char* pWriteEventName,
		uint_t Flags,
		size_t SizeLimitHint = ESharedMemoryTransport_DefSizeLimitHint
		);

	size_t
	Write (
		const void* p,
		size_t Size
		)
	{
		return Write (&p, &Size, 1);
	}

	size_t
	Write (
		const void* const* pBlockArray,
		const size_t* pSizeArray,
		size_t Count
		);

protected:
	static
	void
	CopyWriteChain (
		void* pDst,
		const void* const* pBlockArray,
		const size_t* pSizeArray,
		size_t Count
		);
};
//.............................................................................

} // namespace io
} // namespace axl
