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
	create();

	void
	close()
	{
		m_readFile.close();
		m_writeFile.close();
	}

	size_t
	read(
		void* p,
		size_t size
		) const
	{
		return m_readFile.read(p, size);
	}

	size_t
	write(
		const void* p,
		size_t size
		)
	{
		return m_writeFile.write(p, size);
	}
};

//..............................................................................

} // namespace psx
} // namespace io
} // namespace axl
