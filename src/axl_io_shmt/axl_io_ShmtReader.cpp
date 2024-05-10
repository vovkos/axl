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
#include "axl_io_ShmtReader.h"

namespace axl {
namespace io {

//..............................................................................

size_t
ShmtReader::read(sl::Array<char>* buffer) {
	ASSERT(isOpen());

	sys::atomicLock(&m_hdr->m_lock);

	// wait until we have any data or remote disconnects

	if (!m_hdr->m_dataSize) {
		while (
			!m_hdr->m_dataSize &&
			!(m_hdr->m_state & ShmtState_Disconnected)) {
			m_hdr->m_writeSemaphoreWaitCount++;
			sys::atomicUnlock(&m_hdr->m_lock);

			m_writeSemaphore.wait();

			sys::atomicLock(&m_hdr->m_lock);
		}

		if (!m_hdr->m_dataSize) {
			ASSERT(m_hdr->m_state & ShmtState_Disconnected);

			sys::atomicUnlock(&m_hdr->m_lock);
			err::setError(err::SystemErrorCode_InvalidDeviceState);
			return -1;
		}
	}

	// if there is data, read it even if remote has disconnected

	size_t readOffset = m_hdr->m_readOffset;
	size_t writeOffset = m_hdr->m_writeOffset;
	size_t endOffset = m_hdr->m_endOffset;
	sys::atomicUnlock(&m_hdr->m_lock);

	bool result = ensureOffsetMapped(endOffset);
	if (!result)
		return -1;

	size_t readSize = 0;

	if (m_flags & ShmtFlag_Message) {
		ShmtMessageHdr* msgHdr = (ShmtMessageHdr*)(m_data + readOffset);
		readSize = msgHdr->m_size;
		size_t readEndOffset = readOffset + sizeof(ShmtMessageHdr) + msgHdr->m_size;

#if (_AXL_IO_SHMT_ALIGN_MESSAGES)
		ASSERT(sl::isAligned<AXL_PTR_SIZE> (readOffset));
		readEndOffset = sl::align<AXL_PTR_SIZE> (readEndOffset);
#endif

		if (msgHdr->m_signature != ShmtConst_MessageSignature || readEndOffset > endOffset) {
			err::setError(err::SystemErrorCode_InvalidParameter);
			return -1;
		}

		buffer->copy((const char*) (msgHdr + 1), readSize);

		sys::atomicLock(&m_hdr->m_lock);
		ASSERT(readEndOffset <= m_hdr->m_endOffset);

		m_hdr->m_readOffset = m_hdr->m_endOffset != m_hdr->m_writeOffset && readEndOffset >= m_hdr->m_endOffset ?
			0 : readEndOffset; // wrap : no wrap
	} else {
		if (readOffset < writeOffset) {
			readSize = writeOffset - readOffset;
			buffer->copy(m_data + readOffset, readSize);
		} else {
			size_t size1 = endOffset - readOffset;
			size_t size2 = writeOffset;
			readSize = size1 + size2;

			buffer->setCount(readSize);
			char* p = buffer->p();
			if (size1)
				memcpy(p, m_data + readOffset, size1);

			if (size2)
				memcpy(p + size1, m_data + 1, size2);
		}

		sys::atomicLock(&m_hdr->m_lock);
		m_hdr->m_readOffset = writeOffset;
	}

	m_hdr->m_dataSize -= readSize;

	if (m_hdr->m_readSemaphoreWaitCount) {
		m_readSemaphore.signal(m_hdr->m_readSemaphoreWaitCount);
		m_hdr->m_readSemaphoreWaitCount = 0;
	}

	sys::atomicUnlock(&m_hdr->m_lock);

	return readSize;
}

//..............................................................................

} // namespace io
} // namespace axl
