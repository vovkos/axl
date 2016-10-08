// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PSX_MAPPING_H

#include "axl_io_psx_Fd.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

class Mapping
{
protected:
	void* m_p;
	size_t m_size;

public:
	Mapping ()
	{
		m_p = NULL;
		m_size = 0;
	}

	~Mapping ()
	{
		close ();
	}

	operator void* () const
	{
		return m_p;
	}

	void* 
	p () const
	{
		return m_p;
	}
	
	size_t 
	getSize ()
	{
		return m_size;
	}

	bool
	isOpen () const
	{
		return m_p != NULL;
	}

	void
	close ()
	{
		unmap ();
	}

	void*
	map (
		void* addrHint,
		size_t size,
		int protection,
		uint_t flags,
		int fd,
		size_t offset = 0
		);

	bool
	protect (int protection)
	{
		int result = ::mprotect (m_p, m_size, protection);
		return err::complete (result != -1);
	}

	void
	unmap (size_t size = -1);
};

//.............................................................................

class SharedMemory: public Fd
{
public:
	bool
	open (
		const sl::StringRef& name,
		uint_t flags = O_RDWR,
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
		);

	static
	bool
	unlink (const sl::StringRef& name)
	{
		int result = ::shm_unlink (name.sz ());
		return err::complete (result != -1);
	}
};

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

