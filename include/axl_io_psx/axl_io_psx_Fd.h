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

#define _AXL_IO_PSX_FD_H

#include "axl_io_psx_Pch.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

class CloseFd
{
public:
	void
	operator () (int h)
	{
		::close (h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Fd: public sl::Handle <int, CloseFd, sl::MinusOne <int> >
{
public:
	Fd ()
	{
	}

	Fd (int h):
		sl::Handle <int, CloseFd, sl::MinusOne <int> > (h)
	{
	}

	bool
	open (
		const sl::StringRef& fileName,
		uint_t openFlags = O_RDWR | O_CREAT,
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
		);

	int
	ioctl (int code)
	{
		int result = ::ioctl (m_h, code);
		return err::complete (result, -1);
	}

	template <typename T>
	int
	ioctl (
		int code,
		T param
		)
	{
		int result = ::ioctl (m_h, code, param);
		if (result == -1)
			err::setLastSystemError ();

		return result;
	}

	int
	fcntl (int code)
	{
		int result = ::fcntl (m_h, code);
		return err::complete (result, -1);
	}

	template <typename T>
	int
	fcntl (
		int code,
		T param
		)
	{
		int result = ::fcntl (m_h, code, param);
		if (result == -1)
			err::setLastSystemError ();

		return result;
	}

	bool
	setBlockingMode (bool isBlocking);

	size_t
	getIncomingDataSize ();

	size_t
	read (
		void* p,
		size_t size
		) const;

	size_t
	write (
		const void* p,
		size_t size
		);
};

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
