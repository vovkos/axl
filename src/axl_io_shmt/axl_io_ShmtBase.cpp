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
#include "axl_io_ShmtBase.h"

namespace axl {
namespace io {

//..............................................................................

ShmtBase::ShmtBase() {
	m_flags = 0;
	m_hdr = NULL;
	m_data = NULL;
}

bool
ShmtBase::open(
	const sl::StringRef& fileName,
	const sl::StringRef& readSemaphoreName,
	const sl::StringRef& writeSemaphoreName,
	uint_t flags
) {
	io::File file;
	bool result = file.open(fileName, flags | io::FileFlag_ShareWrite);
	if (!result)
		return false;

	return attach(
		file.m_file.detach(),
		readSemaphoreName,
		writeSemaphoreName,
		flags
	);
}

bool
ShmtBase::attach(
	File::Handle fileHandle,
	const sl::StringRef& readSemaphoreName,
	const sl::StringRef& writeSemaphoreName,
	uint_t flags
) {
	bool result;

	close();

	m_file.m_file.attach(fileHandle);

	if (!(flags & FileFlag_OpenExisting)) {
		result = initializeMapping(ShmtConst_DefMappingSize, true);
		if (!result) {
			closeImpl();
			return false;
		}

		m_hdr->m_signature = ShmtConst_FileSignature;
		m_hdr->m_lock = 0;
		m_hdr->m_state = ShmtState_MasterConnected;
		m_hdr->m_readOffset = 0;
		m_hdr->m_writeOffset = 0;
		m_hdr->m_endOffset = 0;
		m_hdr->m_dataSize = 0;
		m_hdr->m_readSemaphoreWaitCount = 0;
		m_hdr->m_writeSemaphoreWaitCount = 0;

		result =
			m_readSemaphore.create(readSemaphoreName) &&
			m_writeSemaphore.create(writeSemaphoreName);
	} else {
		result = initializeMapping(sizeof(ShmtFileHdr), false);
		if (!result) {
			closeImpl();
			return false;
		}

		if (m_hdr->m_signature != ShmtConst_FileSignature) {
			err::setError(err::SystemErrorCode_InvalidParameter);
			closeImpl();
			return false;
		}

		sys::atomicLock(&m_hdr->m_lock);

		if (!(m_hdr->m_state & ShmtState_MasterConnected) ||
			(m_hdr->m_state & ShmtState_SlaveConnected) ||
			m_hdr->m_readOffset != 0) {
			sys::atomicUnlock(&m_hdr->m_lock);
			err::setError(err::SystemErrorCode_InvalidDeviceState);
			closeImpl();
			return false;
		}

		m_hdr->m_state |= ShmtState_SlaveConnected;

		sys::atomicUnlock(&m_hdr->m_lock);

		result =
			m_readSemaphore.open(readSemaphoreName) &&
			m_writeSemaphore.open(writeSemaphoreName);
	}

	if (!result) {
		closeImpl();
		return false;
	}

	m_flags = flags;
	return true;
}

void
ShmtBase::close() {
	if (!isOpen())
		return;

	disconnect();
	closeImpl();
}

void
ShmtBase::closeImpl() {
	m_file.close();
	m_readSemaphore.close();
	m_writeSemaphore.close();
	m_hdr = NULL;
	m_data = NULL;
}

void
ShmtBase::disconnect() {
	sys::atomicLock(&m_hdr->m_lock);

	m_hdr->m_state |= ShmtState_Disconnected;

	if (m_hdr->m_readSemaphoreWaitCount) {
		m_readSemaphore.signal(m_hdr->m_readSemaphoreWaitCount);
		m_hdr->m_readSemaphoreWaitCount = 0;
	}

	if (m_hdr->m_writeSemaphoreWaitCount) {
		m_writeSemaphore.signal(m_hdr->m_writeSemaphoreWaitCount);
		m_hdr->m_writeSemaphoreWaitCount = 0;
	}

	sys::atomicUnlock(&m_hdr->m_lock);
}

#if (_AXL_OS_POSIX)
bool
ShmtBase::initializeMapping(
	size_t size,
	bool isForced
) {
	const g::SystemInfo* systemInfo = g::getModule()->getSystemInfo();
	size_t remSize = size % systemInfo->m_pageSize;
	if (remSize)
		size = size - remSize + systemInfo->m_pageSize;

	if (isForced) {
		bool result = m_file.setSize(size);
		if (!result)
			return false;
	} else {
		uint64_t fileSize = m_file.getSize();
		if (fileSize < size) {
			err::setError(err::SystemErrorCode_InvalidParameter);
			return false;
		}
	}

	void* p = m_mapping.open(&m_file, 0, size);
	if (!p)
		return false;

	m_hdr = (ShmtFileHdr*)p;
	m_data = (char*)(m_hdr + 1);
	return true;
}
#endif

bool
ShmtBase::ensureMappingSize(size_t size) {
	if (size <= m_mapping.getSize())
		return true;

	const g::SystemInfo* systemInfo = g::getModule()->getSystemInfo();
	size = sl::align(size, systemInfo->m_pageSize);

#if (_AXL_OS_POSIX)
	sys::atomicLock(&m_hdr->m_lock);
	if (size <= m_file.getSize()) {
		sys::atomicUnlock(&m_hdr->m_lock);
	} else {
		bool result = m_file.setSize(size);
		sys::atomicUnlock(&m_hdr->m_lock);
		if (!result)
			return false;
	}
#endif

	Mapping mapping;
	void* p = mapping.open(&m_file, 0, size);
	if (!p)
		return false;

	sl::takeOver(&m_mapping, &mapping);
	m_hdr = (ShmtFileHdr*)p;
	m_data = (char*)(m_hdr + 1);
	return true;
}

//..............................................................................

} // namespace io
} // namespace axl
