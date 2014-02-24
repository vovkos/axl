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

class CFile: public CFd
{
public:
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
};

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
