// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PSX_MAPPING_H

#include "axl_io_psx_Fd.h"
#include "axl_err_Error.h"

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

	void
	unmap (size_t size = -1);
};

//.............................................................................

class SharedMemory: public Fd
{
public:
	bool
	open (
		const char* name,
		uint_t flags = O_RDWR,
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
		);

	static
	bool
	unlink (const char* name)
	{
		int result = shm_unlink (name);
		return err::complete (result != -1);
	}
};

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

