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
#include "axl_io_ShmtWriter.h"

namespace axl {
namespace io {

//..............................................................................

bool
ShmtWriter::open(
	const sl::StringRef& fileName,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName,
	uint_t flags,
	size_t sizeLimitHint
) {
	bool result = ShmtBase::open(fileName, readEventName, writeEventName, flags);
	if (!result)
		return false;

	m_sizeLimitHint = sizeLimitHint;
	return true;
}

bool
ShmtWriter::attach(
	File::Handle fileHandle,
	const sl::StringRef& readEventName,
	const sl::StringRef& writeEventName,
	uint_t flags,
	size_t sizeLimitHint
) {
	bool result = ShmtBase::attach(fileHandle, readEventName, writeEventName, flags);
	if (!result)
		return false;

	m_sizeLimitHint = sizeLimitHint;
	return true;
}

size_t
ShmtWriter::write(
	const void* const* blockArray,
	const size_t* sizeArray,
	size_t count
) {
	ASSERT(isOpen());

	bool result;

	size_t chainSize = 0;
	for (size_t i = 0; i < count; i++)
		chainSize += sizeArray[i];

	if (!chainSize)
		return 0;

	size_t writeSize = chainSize;

	if (m_flags & ShmtFlag_Message) {
		writeSize += sizeof(ShmtMessageHdr);
#if (_AXL_IO_SHMT_ALIGN_MESSAGES)
		writeSize = sl::align<AXL_PTR_SIZE>(writeSize);
#endif
	}

	sys::ScopeLock scopeLock(&m_writeLock); // ensure atomic write

	sys::atomicLock(&m_hdr->m_lock);

	// if buffer is wrapped, then wait until we have enough space

	if (m_hdr->m_dataSize &&
		m_hdr->m_writeOffset <= m_hdr->m_readOffset &&
		m_hdr->m_writeOffset + writeSize > m_hdr->m_readOffset) {
		while (
			m_hdr->m_dataSize &&
			m_hdr->m_writeOffset <= m_hdr->m_readOffset &&
			m_hdr->m_writeOffset + writeSize > m_hdr->m_readOffset &&
			!(m_hdr->m_state & ShmtState_Disconnected)) {
			m_hdr->m_readSemaphoreWaitCount++;
			sys::atomicUnlock(&m_hdr->m_lock);

			m_readSemaphore.wait();

			sys::atomicLock(&m_hdr->m_lock);
		}

		if (m_hdr->m_state & ShmtState_Disconnected) {
			sys::atomicUnlock(&m_hdr->m_lock);
			err::setError(err::SystemErrorCode_InvalidDeviceState);
			return -1;
		}
	}

	size_t writeOffset = m_hdr->m_writeOffset;

	sys::atomicUnlock(&m_hdr->m_lock);

	size_t writeEndOffset = writeOffset + writeSize;

	result = ensureOffsetMapped(writeEndOffset);
	if (!result)
		return -1;

	if (m_flags & ShmtFlag_Message) {
#if (_AXL_IO_SHMT_ALIGN_MESSAGES)
		ASSERT(sl::isAligned<AXL_PTR_SIZE>(writeOffset));
#endif

		ShmtMessageHdr* msgHdr = (ShmtMessageHdr*)(m_data + writeOffset);
		msgHdr->m_signature = ShmtConst_MessageSignature;
		msgHdr->m_size = chainSize;
		copyWriteChain(msgHdr + 1, blockArray, sizeArray, count);
	} else {
		copyWriteChain(m_data + writeOffset, blockArray, sizeArray, count);
	}

	sys::atomicLock(&m_hdr->m_lock);

	if (m_hdr->m_writeOffset <= m_hdr->m_readOffset && m_hdr->m_dataSize) { // wrapped
		m_hdr->m_writeOffset = writeEndOffset;
	} else {
		m_hdr->m_writeOffset = writeEndOffset > m_sizeLimitHint ? 0 : writeEndOffset;
		m_hdr->m_endOffset = writeEndOffset;
	}

	m_hdr->m_dataSize += chainSize;

	if (m_hdr->m_writeSemaphoreWaitCount) {
		m_writeSemaphore.signal(m_hdr->m_writeSemaphoreWaitCount);
		m_hdr->m_writeSemaphoreWaitCount = 0;
	}

	sys::atomicUnlock(&m_hdr->m_lock);

	return chainSize;
}

void
ShmtWriter::copyWriteChain(
	void* _pDst,
	const void* const* blockArray,
	const size_t* sizeArray,
	size_t count
) {
	char* dst = (char*)_pDst;

	for (size_t i = 0; i < count; i++) {
		const void* src = blockArray[i];
		size_t size = sizeArray[i];

		memcpy(dst, src, size);
		dst += size;
	}
}

//..............................................................................

} // namespace io
} // namespace axl
