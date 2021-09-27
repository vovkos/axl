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
#include "axl_io_psx_File.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

bool
File::open(
	const sl::StringRef& fileName,
	uint_t openFlags,
	mode_t mode
) {
	close();

	m_h = ::open(fileName.sz(), openFlags, mode);
	return err::complete(m_h != -1);
}

bool
File::duplicate(int fd) {
	close();

	m_h = ::dup(fd);
	return err::complete(m_h != -1);
}

bool
File::setBlockingMode(bool isBlocking) {
	int result = ::fcntl(m_h, F_GETFL, 0);
	if (result == -1)
		return err::failWithLastSystemError();

	int flags = result;
	if (isBlocking)
		flags &= ~O_NONBLOCK;
	else
		flags |= O_NONBLOCK;

	result = ::fcntl(m_h, F_SETFL, flags);
	return err::complete(result != -1);
}

uint64_t
File::getSize() const {
#if (_AXL_OS_DARWIN)
	struct stat stat;
	int result = ::fstat(m_h, &stat);
#else
	struct stat64 stat;
	int result = ::fstat64(m_h, &stat);
#endif

	if (result == -1) {
		err::setLastSystemError();
		return -1;
	}

	return stat.st_size;
}

uint64_t
File::getPosition() const {
#if (_AXL_OS_DARWIN)
	uint64_t offset = ::lseek(m_h, 0, SEEK_CUR);
#else
	uint64_t offset = ::lseek64(m_h, 0, SEEK_CUR);
#endif

	if (offset == -1)
		err::setLastSystemError();

	return offset;
}

size_t
File::getIncomingDataSize() {
	int value;
	int result = ::ioctl(m_h, FIONREAD, &value);
	if (result == -1) {
		err::setLastSystemError();
		return -1;
	}

	return value;
}

size_t
File::read(
	void* p,
	size_t size
) const {
	size_t actualSize = ::read(m_h, p, size);
	if (actualSize == -1)
		err::setLastSystemError();

	return actualSize;
}

size_t
File::write(
	const void* p,
	size_t size
) {
	size_t actualSize = ::write(m_h, p, size);
	if (actualSize == -1)
		err::setLastSystemError();

	return actualSize;
}

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
