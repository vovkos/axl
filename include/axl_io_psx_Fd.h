// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PSX_FD_H

#include "axl_rtl_Handle.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

class CCloseFd
{
public:
	void
	operator () (int h)
	{
		::close (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CFd: public rtl::CHandleT <int, CCloseFd, rtl::CMinusOneT <int> >
{
public:
	CFd ()
	{
	}

	CFd (int h):
		rtl::CHandleT <int, CCloseFd, rtl::CMinusOneT <int> > (h)
	{
	}

	bool
	Open (
		const char* pFileName,
		uint_t OpenFlags = O_RDWR | O_CREAT,
		mode_t Mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
		);

	bool
	SetBlockingMode (bool IsBlocking);

	size_t
	GetIncomingDataSize ();

	size_t
	Read (
		void* p,
		size_t Size
		) const;

	size_t
	Write (
		const void* p,
		size_t Size
		);
};

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
