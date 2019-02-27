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

#define _AXL_IO_MAPPING_H

#include "axl_io_File.h"

#if (_AXL_OS_WIN)
#	include "axl_io_win_Mapping.h"
#elif (_AXL_OS_POSIX)
#	include "axl_io_psx_Mapping.h"
#endif

namespace axl {
namespace io {

//..............................................................................

class Mapping
{
protected:
	void* m_p;
	size_t m_size;

#if (_AXL_OS_WIN)
	win::Mapping m_mapping;
	win::MappedView m_view;
#elif (_AXL_OS_POSIX)
	psx::Mapping m_mapping;
	psx::SharedMemory m_sharedMemory;
	sl::String m_sharedMemoryName;
#endif

public:
	Mapping()
	{
		m_p = NULL;
		m_size = 0;
	}

	operator void* ()
	{
		return m_p;
	}

	bool
	isOpen()
	{
		return m_p != NULL;
	}

	void*
	p()
	{
		return m_p;
	}

	size_t
	getSize()
	{
		return m_size;
	}

	void*
	open(
		File* file,
		uint64_t offset,
		size_t size,
		uint_t flags = 0 // FileFlag
		);

	void*
	open(
		File* file,
		uint_t flags = 0 // FileFlag
		)
	{
		return open(file, 0, -1, flags);
	}

	void*
	open(
		const sl::StringRef& name,
		size_t size,
		uint_t flags = 0 // FileFlag
		);

	void
	close();
};

//..............................................................................

} // namespace io
} // namespace axl
