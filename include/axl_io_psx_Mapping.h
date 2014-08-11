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

class CMapping
{
protected:
	void* m_p;
	size_t m_Size;

public:
	CMapping ()
	{
		m_p = NULL;
		m_Size = 0;
	}

	~CMapping ()
	{
		Close ();
	}

	operator void* () const
	{
		return m_p;
	}

	bool
	IsOpen () const
	{
		return m_p != NULL;
	}

	void
	Close ()
	{
		Unmap ();
	}

	void*
	Map (
		void* pAddrHint,
		size_t Size,
		int Protection,
		uint_t Flags,
		int fd,
		size_t Offset = 0
		);

	void
	Unmap (size_t Size = -1);
};

//.............................................................................

class CSharedMemory: public CFd
{
public:
	bool
	Open (
		const char* pName,
		uint_t Flags = O_RDWR,
		mode_t Mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
		);

	static
	bool
	Unlink (const char* pName)
	{
		int Result = shm_unlink (pName);
		return err::Complete (Result != -1);
	}
};

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl

