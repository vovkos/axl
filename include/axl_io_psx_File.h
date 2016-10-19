// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PSX_FILE_H

#include "axl_io_psx_Fd.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

class File: public Fd
{
public:
	uint64_t
	getSize () const;

	bool
	setSize (uint64_t size)
	{
#if (_AXL_OS_DARWIN)
		int result = ::ftruncate (m_h, size);
#else
		int result = ::ftruncate64 (m_h, size);
#endif
		return err::complete (result != -1);
	}

	uint64_t
	getPosition () const;

	bool
	setPosition (uint64_t offset) const
	{
#if (_AXL_OS_DARWIN)
		uint64_t actualOffset = ::lseek (m_h, offset, SEEK_SET);
#else
		uint64_t actualOffset = ::lseek64 (m_h, offset, SEEK_SET);
#endif
		return err::complete (actualOffset != -1);
	}

	bool
	flush ()
	{
		int result = ::fsync (m_h);
		return err::complete (result != -1);
	}
};

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
