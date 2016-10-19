// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_PSX_PIPE_H

#include "axl_io_psx_File.h"

namespace axl {
namespace io {
namespace psx {

//..............................................................................

class Pipe
{
public:
	File m_readFile;
	File m_writeFile;

public:
	bool
	create ();

	void
	close ()
	{
		m_readFile.close ();
		m_writeFile.close ();
	}

	size_t
	read (
		void* p,
		size_t size
		) const
	{
		return m_readFile.read (p, size);
	}

	size_t
	write (
		const void* p,
		size_t size
		)
	{
		return m_writeFile.write (p, size);
	}
};

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
