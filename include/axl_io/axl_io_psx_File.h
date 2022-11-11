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

#define _AXL_IO_PSX_FILE_H

#include "axl_io_psx_Pch.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

class CloseFile {
public:
	void
	operator () (int h) {
		int result = ::close(h);
		ASSERT(result == 0);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class File: public sl::Handle<int, CloseFile, sl::MinusOne<int> > {
public:
	File() {}

	File(int h):
		sl::Handle<int, CloseFile, sl::MinusOne<int> > (h) {}

	bool
	open(
		const sl::StringRef& fileName,
		uint_t openFlags = O_RDWR | O_CREAT,
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
	);

	bool
	duplicate(int fd);

	bool
	setBlockingMode(bool isBlocking);

	uint64_t
	getSize() const;

	bool
	setSize(uint64_t size) {
#if (_AXL_OS_DARWIN)
		int result = ::ftruncate(m_h, size);
#else
		int result = ::ftruncate64(m_h, size);
#endif
		return err::complete(result != -1);
	}

	uint64_t
	getPosition() const;

	bool
	setPosition(uint64_t offset) {
#if (_AXL_OS_DARWIN)
		uint64_t actualOffset = ::lseek(m_h, offset, SEEK_SET);
#else
		uint64_t actualOffset = ::lseek64(m_h, offset, SEEK_SET);
#endif
		return err::complete(actualOffset != -1);
	}

	size_t
	getIncomingDataSize();

	bool
	flush() {
		int result = ::fsync(m_h);
		return err::complete(result != -1);
	}

	int
	ioctl(uint_t code) {
		int result = ::ioctl(m_h, code);
		return err::complete(result, -1);
	}

	template <typename T>
	int
	ioctl(
		uint_t code,
		T param
	) {
		int result = ::ioctl(m_h, code, param);
		if (result == -1)
			err::setLastSystemError();

		return result;
	}

	int
	fcntl(uint_t code) {
		int result = ::fcntl(m_h, code);
		return err::complete(result, -1);
	}

	template <typename T>
	int
	fcntl(
		uint_t code,
		T param
	) {
		int result = ::fcntl(m_h, code, param);
		if (result == -1)
			err::setLastSystemError();

		return result;
	}

	size_t
	read(
		void* p,
		size_t size
	);

	size_t
	write(
		const void* p,
		size_t size
	);
};

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
