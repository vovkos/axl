// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_MAPPEDFILETRANSPORT_H

#include "axl_io_MappedFile.h"
#include "axl_mt_Event.h"

namespace axl {
namespace io {

//.............................................................................

enum EMappedFileTransport
{
	EMappedFileTransport_FileSignature    = ':tfm',
	EMappedFileTransport_MessageSignature = ':gsm',
};

struct TMappedFileTransportHdr
{
	uint32_t m_Signature;
	uint32_t m_ReadOffset;   // only modified by reader
	uint32_t m_WriteOffset;  // only modified by writer
	uint32_t m_EndOffset;    // only modified by writer (and only grows)
};

struct TMappedFileMessageHdr
{
	uint32_t m_Signature;
	uint32_t m_Size;
};

//.............................................................................

class CMappedFileTransport
{
protected:
	CMappedFile m_File;
	TMappedFileTransportHdr* m_pHdr;
	mt::CEvent m_ReadEvent;
	mt::CEvent m_WriteEvent;
	
public:
	bool
	IsOpen ()
	{
		return m_File.IsOpen ();
	}

	bool 
	Open (
		bool IsWriter,
		const char* pFileName,
		const char* pMutexName,
		const char* pReadEventName,
		const char* pWriteEventName
		);

	void
	Close ();

	void*
	Read (size_t* pSize);

	size_t
	Write (
		const void* p,
		size_t Size
		);

	size_t
	Write (
		size_t BlockCount,
		const void* p,
		size_t Size,
		...
		);
};

//.............................................................................

} // namespace io
} // namespace axl
