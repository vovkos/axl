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

#define _AXL_IO_SIMPLEMAPPEDFILE_H

#include "axl_io_File.h"
#include "axl_io_Mapping.h"

namespace axl {
namespace io {

//..............................................................................

class SimpleMappedFile {
protected:
	File m_file;
	Mapping m_mapping;
	uint_t m_openFlags;

public:
	SimpleMappedFile() {
		m_openFlags = 0;
	}

	bool
	isOpen() const {
		return m_mapping.isOpen();
	}

	void*
	p() const {
		return m_mapping.p();
	}

	operator void* () const {
		return m_mapping;
	}

	size_t
	getMappingSize() const {
		return m_mapping.getSize();
	}

	uint64_t
	getFileSize() const {
		return m_file.getSize();
	}

	bool
	open(
		const sl::StringRef& fileName,
		uint64_t offset,
		size_t size,
		uint_t flags = 0 // FileFlag
	);

	bool
	open(
		const sl::StringRef& fileName,
		uint_t flags = 0 // FileFlag
	) {
		return open(fileName, 0, -1, flags);
	}

	void*
	view(
		uint64_t offset,
		size_t size
	) {
		return m_mapping.open(&m_file, offset, size, m_openFlags);
	}

	bool
	unmapAndSetFileSize(uint64_t size) {
		m_mapping.close();
		return m_file.setSize(size);
	}

	void
	close();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
SimpleMappedFile::open(
	const sl::StringRef& fileName,
	uint64_t offset,
	size_t size,
	uint_t flags
) {
	close();

	bool result = m_file.open(fileName, flags);
	if (!result)
		return false;

	m_openFlags = flags;
	return !size || m_mapping.open(&m_file, offset, size, flags);
}

inline
void
SimpleMappedFile::close() {
	m_file.close();
	m_mapping.close();
	m_openFlags = 0;
}

//..............................................................................

} // namespace io
} // namespace axl
