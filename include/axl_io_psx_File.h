// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_PSX_FILE_H

#include "axl_io_psx_Fd.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

class CFile: public CFd
{
public:
	bool
	Open (
		const char* pFileName,
		uint_t OpenFlags = O_RDWR | O_CREAT,
		mode_t Mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
		);

	uint64_t
	GetSize () const;

	bool
	SetSize (uint64_t Size)
	{
		int Result = ftruncate64 (m_h, Size);
		return err::Complete (Result != -1);
	}

	uint64_t
	GetPosition () const;

	bool
	SetPosition (off64_t Offset) const
	{
		uint64_t ActualOffset = lseek64 (m_h, Offset, SEEK_SET);
		return err::Complete (ActualOffset != -1);
	}

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
