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

#include "pch.h"
#include "axl_io_SharedMemoryTransport.h"
#include "axl_sl_String.h"
#include "axl_err_Error.h"
#include "axl_g_Module.h"

namespace axl {
namespace io {

//..............................................................................

SharedMemoryTransportBase::SharedMemoryTransportBase ()
{
	m_flags = 0;
	m_mappingSize = 0;
	m_hdr = NULL;
	m_data = NULL;
	m_pendingReqCount = 0;
}

bool
SharedMemoryTransportBase::open (
	const sl::StringRef& fileName,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName,
	uint_t flags
	)
{
	uint_t fileFlags = io::FileFlag_ShareWrite;
	if (flags & SharedMemoryTransportFlag_DeleteOnClose)
		fileFlags |= io::FileFlag_DeleteOnClose;

	io::File file;
	bool result = file.open (fileName, fileFlags);
	if (!result)
		return false;

	return attach (
		file.m_file.detach (),
		readEventName,
		writeEventName,
		flags
		);
}

bool
SharedMemoryTransportBase::attach (
	File::Handle fileHandle,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName,
	uint_t flags
	)
{
	bool result;

	close ();

	m_file.m_file.attach (fileHandle);

	if (flags & SharedMemoryTransportFlag_Create)
	{
		result = initializeMapping (SharedMemoryTransportConst_DefMappingSize, true);
		if (!result)
		{
			closeImpl ();
			return false;
		}

		m_hdr->m_signature = SharedMemoryTransportConst_FileSignature;
		m_hdr->m_lock = 0;
		m_hdr->m_state = SharedMemoryTransportState_MasterConnected;
		m_hdr->m_readOffset = 0;
		m_hdr->m_writeOffset = 0;
		m_hdr->m_endOffset = 0;
		m_hdr->m_dataSize = 0;

		result =
			m_readEvent.create (readEventName) &&
			m_writeEvent.create (writeEventName);
	}
	else
	{
		result = initializeMapping (sizeof (SharedMemoryTransportHdr), false);
		if (!result)
		{
			closeImpl ();
			return false;
		}

		if (m_hdr->m_signature != SharedMemoryTransportConst_FileSignature)
		{
			err::setError (err::SystemErrorCode_InvalidParameter);
			closeImpl ();
			return false;
		}

		sys::atomicLock (&m_hdr->m_lock);

		if (m_hdr->m_state != SharedMemoryTransportState_MasterConnected ||
			m_hdr->m_readOffset != 0)
		{
			sys::atomicUnlock (&m_hdr->m_lock);
			err::setError (err::SystemErrorCode_InvalidDeviceState);
			closeImpl ();
			return false;
		}

		m_hdr->m_state = SharedMemoryTransportState_SlaveConnected;

		sys::atomicUnlock (&m_hdr->m_lock);

		result =
			m_readEvent.open (readEventName) &&
			m_writeEvent.open (writeEventName);
	}

	if (!result)
	{
		closeImpl ();
		return false;
	}

	m_flags = flags;
	return true;
}

void
SharedMemoryTransportBase::close ()
{
	if (!isOpen ())
		return;

	disconnect ();
	closeImpl ();
}

void
SharedMemoryTransportBase::closeImpl ()
{
	m_file.close ();
	m_readEvent.close ();
	m_writeEvent.close ();
	m_hdr = NULL;
	m_data = NULL;
	m_mappingSize = 0;
	m_pendingReqCount = 0;
}

void
SharedMemoryTransportBase::disconnect ()
{
	sys::atomicLock (&m_hdr->m_lock);
	m_hdr->m_state = SharedMemoryTransportState_Disconnected;
	m_readEvent.signal ();
	m_writeEvent.signal ();
	sys::atomicUnlock (&m_hdr->m_lock);
}

#if (_AXL_OS_POSIX)
bool
SharedMemoryTransportBase::initializeMapping (
	size_t size,
	bool isForced
	)
{
	const g::SystemInfo* systemInfo = g::getModule ()->getSystemInfo ();
	size_t remSize = size % systemInfo->m_pageSize;
	if (remSize)
		size = size - remSize + systemInfo->m_pageSize;

	if (isForced)
	{
		bool result = m_file.setSize (size);
		if (!result)
			return false;
	}
	else
	{
		uint64_t fileSize = m_file.getSize ();
		if (fileSize < size)
		{
			err::setError (err::SystemErrorCode_InvalidParameter);
			return false;
		}
	}

	void* p = m_mapping.open (&m_file, 0, size);
	if (!p)
		return false;

	m_mappingSize = size;
	m_hdr = (SharedMemoryTransportHdr*) p;
	m_data = (char*) (m_hdr + 1);
	return true;
}
#endif

bool
SharedMemoryTransportBase::ensureMappingSize (size_t size)
{
	if (size <= m_mappingSize)
		return true;

	const g::SystemInfo* systemInfo = g::getModule ()->getSystemInfo ();
	size_t remSize = size % systemInfo->m_pageSize;
	if (remSize)
		size = size - remSize + systemInfo->m_pageSize;

#if (_AXL_OS_POSIX)
	sys::atomicLock (&m_hdr->m_lock);
	if (size <= m_file.getSize ())
	{
		sys::atomicUnlock (&m_hdr->m_lock);
	}
	else
	{
		bool result = m_file.setSize (size);
		sys::atomicUnlock (&m_hdr->m_lock);
		if (!result)
			return false;
	}
#endif

	void* p = m_mapping.open (&m_file, 0, size);
	if (!p)
		return false;

	m_mappingSize = size;
	m_hdr = (SharedMemoryTransportHdr*) p;
	m_data = (char*) (m_hdr + 1);
	return true;
}

//..............................................................................

size_t
SharedMemoryReader::read (sl::Array <char>* buffer)
{
	ASSERT (isOpen ());

	sys::atomicLock (&m_hdr->m_lock);

	// wait until we have any data or remote disconnects

	while (
		!m_hdr->m_dataSize &&
		m_hdr->m_state != SharedMemoryTransportState_Disconnected)
	{
		sys::atomicUnlock (&m_hdr->m_lock);
		m_writeEvent.wait ();
		sys::atomicLock (&m_hdr->m_lock);
	}

	if (!m_hdr->m_dataSize)
	{
		ASSERT (m_hdr->m_state == SharedMemoryTransportState_Disconnected);

		sys::atomicUnlock (&m_hdr->m_lock);
		err::setError (err::SystemErrorCode_InvalidDeviceState);
		return -1;
	}

	// if there is data, read it even if remote has disconnected

	size_t readOffset = m_hdr->m_readOffset;
	size_t writeOffset = m_hdr->m_writeOffset;
	size_t endOffset = m_hdr->m_endOffset;
	sys::atomicUnlock (&m_hdr->m_lock);

	bool result = ensureOffsetMapped (endOffset);
	if (!result)
		return -1;

	size_t readSize = 0;

	if (m_flags & SharedMemoryTransportFlag_Message)
	{
		SharedMemoryTransportMessageHdr* msgHdr = (SharedMemoryTransportMessageHdr*) (m_data + readOffset);
		readSize = msgHdr->m_size;
		size_t readEndOffset = readOffset + sizeof (SharedMemoryTransportMessageHdr) + msgHdr->m_size;

		if (msgHdr->m_signature != SharedMemoryTransportConst_MessageSignature || readEndOffset > endOffset)
		{
			err::setError (err::SystemErrorCode_InvalidParameter);
			return -1;
		}

		buffer->copy ((const char*) (msgHdr + 1), readSize);

		sys::atomicLock (&m_hdr->m_lock);
		ASSERT (readEndOffset <= m_hdr->m_endOffset);

		m_hdr->m_readOffset = m_hdr->m_endOffset != m_hdr->m_writeOffset && readEndOffset >= m_hdr->m_endOffset ?
			0 : readEndOffset; // wrap : no wrap
	}
	else
	{
		if (readOffset < writeOffset)
		{
			readSize = writeOffset - readOffset;
			buffer->copy (m_data + readOffset, readSize);
		}
		else
		{
			size_t size1 = endOffset - readOffset;
			size_t size2 = writeOffset;
			readSize = size1 + size2;

			buffer->setCount (readSize);

			if (size1)
				memcpy (*buffer, m_data + readOffset, size1);

			if (size2)
				memcpy (*buffer + size1, m_data + 1, size2);
		}

		sys::atomicLock (&m_hdr->m_lock);
		m_hdr->m_readOffset = writeOffset;
	}

	m_hdr->m_dataSize -= readSize;
	m_readEvent.signal ();
	sys::atomicUnlock (&m_hdr->m_lock);

	return readSize;
}

//..............................................................................

bool
SharedMemoryWriter::open (
	const sl::StringRef& fileName,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName,
	uint_t flags,
	size_t sizeLimitHint
	)
{
	bool result = SharedMemoryTransportBase::open (fileName, readEventName, writeEventName, flags);
	if (!result)
		return false;

	m_sizeLimitHint = sizeLimitHint;
	return true;
}

bool
SharedMemoryWriter::attach (
	File::Handle fileHandle,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName,
	uint_t flags,
	size_t sizeLimitHint
	)
{
	bool result = SharedMemoryTransportBase::attach (fileHandle, readEventName, writeEventName, flags);
	if (!result)
		return false;

	m_sizeLimitHint = sizeLimitHint;
	return true;
}

size_t
SharedMemoryWriter::write (
	const void* const* blockArray,
	const size_t* sizeArray,
	size_t count
	)
{
	ASSERT (isOpen ());

	bool result;

	size_t chainSize = 0;
	for (size_t i = 0; i < count; i++)
		chainSize += sizeArray [i];

	if (!chainSize)
		return 0;

	size_t writeSize = chainSize;

	if (m_flags & SharedMemoryTransportFlag_Message)
		writeSize += sizeof (SharedMemoryTransportMessageHdr);

	sys::ScopeLock scopeLock (&m_writeLock); // ensure atomic write

	sys::atomicLock (&m_hdr->m_lock);

	// if buffer is wrapped, then wait until we have enough space

	while (
		m_hdr->m_writeOffset <= m_hdr->m_readOffset && m_hdr->m_dataSize && // wrapped
		m_hdr->m_writeOffset + writeSize > m_hdr->m_readOffset &&
		m_hdr->m_state != SharedMemoryTransportState_Disconnected
		)
	{
		sys::atomicUnlock (&m_hdr->m_lock);
		m_readEvent.wait ();
		sys::atomicLock (&m_hdr->m_lock);
	}

	if (m_hdr->m_state == SharedMemoryTransportState_Disconnected)
	{
		sys::atomicUnlock (&m_hdr->m_lock);
		err::setError (err::SystemErrorCode_InvalidDeviceState);
		return -1;
	}

	size_t writeOffset = m_hdr->m_writeOffset;

	sys::atomicUnlock (&m_hdr->m_lock);

	size_t writeEndOffset = writeOffset + writeSize;

	result = ensureOffsetMapped (writeEndOffset);
	if (!result)
		return -1;

	if (m_flags & SharedMemoryTransportFlag_Message)
	{
		SharedMemoryTransportMessageHdr* msgHdr = (SharedMemoryTransportMessageHdr*) (m_data + writeOffset);
		msgHdr->m_signature = SharedMemoryTransportConst_MessageSignature;
		msgHdr->m_size = chainSize;
		copyWriteChain (msgHdr + 1, blockArray, sizeArray, count);
	}
	else
	{
		copyWriteChain (m_data + writeOffset, blockArray, sizeArray, count);
	}

	sys::atomicLock (&m_hdr->m_lock);

	if (m_hdr->m_writeOffset <= m_hdr->m_readOffset && m_hdr->m_dataSize) // wrapped
	{
		m_hdr->m_writeOffset = writeEndOffset;
	}
	else
	{
		m_hdr->m_writeOffset = writeEndOffset > m_sizeLimitHint ? 0 : writeEndOffset;
		m_hdr->m_endOffset = writeEndOffset;
	}

	m_hdr->m_dataSize += chainSize;
	m_writeEvent.signal ();
	sys::atomicUnlock (&m_hdr->m_lock);

	return chainSize;
}

void
SharedMemoryWriter::copyWriteChain (
	void* _pDst,
	const void* const* blockArray,
	const size_t* sizeArray,
	size_t count
	)
{
	char* dst = (char*) _pDst;

	for (size_t i = 0; i < count; i++)
	{
		const void* src = blockArray [i];
		size_t size = sizeArray [i];

		memcpy (dst, src, size);
		dst += size;
	}
}

//..............................................................................

} // namespace io
} // namespace axl
