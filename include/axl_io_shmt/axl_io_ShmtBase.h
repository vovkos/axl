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

#pragma once

#define _AXL_IO_SHMTBASE_H

#include "axl_io_ShmtFile.h"

namespace axl {
namespace io {

//..............................................................................

class ShmtBase {
protected:
	uint_t m_flags;

	File m_file;
	Mapping m_mapping;

	volatile ShmtFileHdr* m_hdr;
	char* m_data;
	sys::NamedSemaphore m_readSemaphore;
	sys::NamedSemaphore m_writeSemaphore;

protected:
	ShmtBase();

	~ShmtBase() {
		close();
	}

public:
	uint_t
	getFlags() {
		return m_flags;
	}

	const File*
	getFile() const {
		return &m_file;
	}

	bool
	isOpen() {
		return m_hdr != NULL;
	}

	void
	close();

	bool
	open(
		const sl::StringRef& fileName,
		const sl::StringRef& readSemaphoreName,
		const sl::StringRef& writeSemaphoreName,
		uint_t flags
	);

	bool
	attach(
		File::Handle fileHandle,
		const sl::StringRef& readSemaphoreName,
		const sl::StringRef& writeSemaphoreName,
		uint_t flags
	);

	void
	disconnect();

protected:
	void
	closeImpl();

#if (!_AXL_OS_POSIX)
	bool
	initializeMapping(
		size_t size,
		bool isForced
	) {
		return ensureMappingSize(size);
	}
#else
	bool
	initializeMapping(
		size_t size,
		bool isForced
	);
#endif

	bool
	ensureMappingSize(size_t size);

	bool
	ensureOffsetMapped(size_t offset) {
		return ensureMappingSize(offset + sizeof(ShmtFileHdr));
	}
};

//..............................................................................

} // namespace io
} // namespace axl
