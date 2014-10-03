// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PSX_FILE_H

#include "axl_io_psx_Fd.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

class File: public Fd
{
public:
	uint64_t
	getSize () const;

	bool
	setSize (uint64_t size)
	{
		int result = ::ftruncate64 (m_h, size);
		return err::complete (result != -1);
	}

	uint64_t
	getPosition () const;

	bool
	setPosition (off64_t offset) const
	{
		uint64_t actualOffset = ::lseek64 (m_h, offset, SEEK_SET);
		return err::complete (actualOffset != -1);
	}

	bool
	flush ()
	{
		int result = ::fdatasync (m_h);
		return err::complete (result != -1);
	}
};

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
