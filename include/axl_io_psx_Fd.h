// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PSX_FD_H

#include "axl_sl_Handle.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

class CloseFd
{
public:
	void
	operator () (int h)
	{
		::close (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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
		const char* fileName,
		uint_t openFlags = O_RDWR | O_CREAT,
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
		);

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

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
