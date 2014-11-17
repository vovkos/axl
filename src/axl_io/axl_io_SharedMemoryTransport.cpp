#include "pch.h"
#include "axl_io_SharedMemoryTransport.h"
#include "axl_rtl_String.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//.............................................................................

SharedMemoryTransportBase::SharedMemoryTransportBase ()
{
	m_flags = 0;
	m_hdr = NULL;
	m_data = NULL;
	m_mappingSize = 0;
	m_pendingReqCount = 0;
}

bool
SharedMemoryTransportBase::open (
	const char* fileName,
	const char* readEventName,
	const char* writeEventName,
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
	const char* readEventName,
	const char* writeEventName,
	uint_t flags
	)
{
	bool result;

	close ();

	m_file.attach (fileHandle);

	result = ensureMappingSize (SharedMemoryTransportConst_DefMappingSize);
	if (!result)
	{
		m_file.close ();
		return false;
	}

	if (flags & SharedMemoryTransportFlag_Create)
	{
		m_hdr->m_signature = SharedMemoryTransportConst_FileSignature;
		m_hdr->m_lock = 0;
		m_hdr->m_state = SharedMemoryTransportState_MasterConnected;
		m_hdr->m_readOffset = 0;
		m_hdr->m_writeOffset = 0;
		m_hdr->m_endOffset = 0;
		m_hdr->m_dataSize = 0;

#if (_AXL_ENV == AXL_ENV_WIN)
		result =
			m_readEvent.create (NULL, false, false, rtl::String_utf16 (readEventName)) &&
			m_writeEvent.create (NULL, false, false, rtl::String_utf16 (writeEventName));
#elif (_AXL_ENV == AXL_ENV_POSIX)
		result = m_readEvent.open (readEventName, O_CREAT);
		if (result)
		{
			m_readEventName = readEventName;

			result = m_writeEvent.open (writeEventName, O_CREAT);
			if (result)
				m_writeEventName = writeEventName;
		}
#endif
	}
	else
	{
		if (m_hdr->m_signature != SharedMemoryTransportConst_FileSignature)
		{
			err::setError (err::SystemErrorCode_InvalidParameter);
			return false;
		}

		mt::atomicLock (&m_hdr->m_lock);

		if (m_hdr->m_state != SharedMemoryTransportState_MasterConnected ||
			m_hdr->m_readOffset != 0)
		{
			err::setError (err::SystemErrorCode_InvalidDeviceState);
			return false;
		}

		m_hdr->m_state = SharedMemoryTransportState_SlaveConnected;

		mt::atomicUnlock (&m_hdr->m_lock);

#if (_AXL_ENV == AXL_ENV_WIN)
		result =
			m_readEvent.open (EVENT_ALL_ACCESS, false, rtl::String_utf16 (readEventName)) &&
			m_writeEvent.open (EVENT_ALL_ACCESS, false, rtl::String_utf16 (writeEventName));
#elif (_AXL_ENV == AXL_ENV_POSIX)
		result =
			m_readEvent.open (readEventName, 0) &&
			m_writeEvent.open (writeEventName, 0);
#endif
	}

	if (!result)
	{
		close ();
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

	m_file.close ();
	m_readEvent.close ();
	m_writeEvent.close ();
	m_hdr = NULL;
	m_data = NULL;
	m_mappingSize = 0;
	m_pendingReqCount = 0;

#if (_AXL_ENV == AXL_ENV_POSIX)
	if (!m_readEventName.isEmpty ())
	{
		mt::psx::Sem::unlink (m_readEventName);
		m_readEventName.clear ();
	}

	if (!m_writeEventName.isEmpty ())
	{
		mt::psx::Sem::unlink (m_writeEventName);
		m_writeEventName.clear ();
	}
#endif
}

void
SharedMemoryTransportBase::disconnect ()
{
	mt::atomicLock (&m_hdr->m_lock);
	m_hdr->m_state = SharedMemoryTransportState_Disconnected;

#if (_AXL_ENV == AXL_ENV_WIN)
	m_readEvent.signal ();
	m_writeEvent.signal ();
#elif (_AXL_ENV == AXL_ENV_POSIX)
	m_readEvent.post ();
	m_writeEvent.post ();
#endif

	mt::atomicUnlock (&m_hdr->m_lock);
}

bool
SharedMemoryTransportBase::ensureMappingSize (size_t size)
{
	if (size <= m_mappingSize)
		return true;

	void* p = m_file.view (0, size);
	if (!p)
		return false;

	m_hdr = (SharedMemoryTransportHdr*) p;
	m_data = (char*) (m_hdr + 1);
	m_mappingSize = size;
	return true;
}

//.............................................................................

rtl::Array <char>
SharedMemoryReader::read ()
{
	rtl::Array <char> buffer;
	read (&buffer);
	return buffer;
}

size_t
SharedMemoryReader::read (rtl::Array <char>* buffer)
{
	ASSERT (isOpen ());

	mt::atomicLock (&m_hdr->m_lock);

	// if buffer is empty, then wait until we have any data

	while (
		!m_hdr->m_dataSize &&
		m_hdr->m_state != SharedMemoryTransportState_Disconnected)
	{
		mt::atomicUnlock (&m_hdr->m_lock);
		m_writeEvent.wait ();
		mt::atomicLock (&m_hdr->m_lock);
	}

	if (m_hdr->m_state == SharedMemoryTransportState_Disconnected)
	{
		mt::atomicUnlock (&m_hdr->m_lock);
		err::setError (err::SystemErrorCode_InvalidDeviceState);
		return -1;
	}

	size_t readOffset = m_hdr->m_readOffset;
	size_t writeOffset = m_hdr->m_writeOffset;
	size_t endOffset = m_hdr->m_endOffset;
	mt::atomicUnlock (&m_hdr->m_lock);

	bool result = ensureOffsetMapped (endOffset);
	if (!result)
		return -1;

	size_t readSize = 0;

	if (m_flags & SharedMemoryTransportFlag_Message)
	{
		SharedMemoryransportMessageHdr* msgHdr = (SharedMemoryransportMessageHdr*) (m_data + readOffset);
		readSize = msgHdr->m_size;
		size_t readEndOffset = readOffset + sizeof (SharedMemoryransportMessageHdr) + msgHdr->m_size;

		if (msgHdr->m_signature != SharedMemoryTransportConst_MessageSignature || readEndOffset > endOffset)
		{
			err::setError (err::SystemErrorCode_InvalidParameter);
			return -1;
		}

		buffer->copy ((const char*) (msgHdr + 1), readSize);

		mt::atomicLock (&m_hdr->m_lock);
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

		mt::atomicLock (&m_hdr->m_lock);
		m_hdr->m_readOffset = writeOffset;
	}

	m_hdr->m_dataSize -= readSize;
	mt::atomicUnlock (&m_hdr->m_lock);

#if (_AXL_ENV == AXL_ENV_WIN)
	m_readEvent.signal ();
#elif (_AXL_ENV == AXL_ENV_POSIX)
	m_readEvent.post ();
#endif

	return readSize;
}

//.............................................................................

bool
SharedMemoryWriter::open (
	const char* fileName,
	const char* readEventName,
	const char* writeEventName,
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
	const char* readEventName,
	const char* writeEventName,
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
		writeSize += sizeof (SharedMemoryransportMessageHdr);

	mt::ScopeLock scopeLock (&m_writeLock); // ensure atomic write

	mt::atomicLock (&m_hdr->m_lock);

	// if buffer is wrapped, then wait until we have enough space

	while (
		m_hdr->m_writeOffset <= m_hdr->m_readOffset && m_hdr->m_dataSize && // wrapped
		m_hdr->m_writeOffset + writeSize > m_hdr->m_readOffset &&
		m_hdr->m_state != SharedMemoryTransportState_Disconnected
		)
	{
		mt::atomicUnlock (&m_hdr->m_lock);
		m_readEvent.wait ();
		mt::atomicLock (&m_hdr->m_lock);
	}

	if (m_hdr->m_state == SharedMemoryTransportState_Disconnected)
	{
		mt::atomicUnlock (&m_hdr->m_lock);
		err::setError (err::SystemErrorCode_InvalidDeviceState);
		return -1;
	}

	size_t readOffset = m_hdr->m_readOffset;
	size_t writeOffset = m_hdr->m_writeOffset;
	size_t dataSize = m_hdr->m_dataSize;

	mt::atomicUnlock (&m_hdr->m_lock);

	size_t writeEndOffset = writeOffset + writeSize;

	result = ensureOffsetMapped (writeEndOffset);
	if (!result)
		return -1;

	if (m_flags & SharedMemoryTransportFlag_Message)
	{
		SharedMemoryransportMessageHdr* msgHdr = (SharedMemoryransportMessageHdr*) (m_data + writeOffset);
		msgHdr->m_signature = SharedMemoryTransportConst_MessageSignature;
		msgHdr->m_size = chainSize;
		copyWriteChain (msgHdr + 1, blockArray, sizeArray, count);
	}
	else
	{
		copyWriteChain (m_data + writeOffset, blockArray, sizeArray, count);
	}

	mt::atomicLock (&m_hdr->m_lock);

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
	mt::atomicUnlock (&m_hdr->m_lock);

#if (_AXL_ENV == AXL_ENV_WIN)
	m_writeEvent.signal ();
#elif (_AXL_ENV == AXL_ENV_POSIX)
	m_writeEvent.post ();
#endif

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

//.............................................................................

} // namespace axl {
} // namespace io {
