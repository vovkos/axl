// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_PSX_PIPE_H

#include "axl_io_psx_File.h"

namespace axl {
namespace io {
namespace psx {

//.............................................................................

class CPipe
{
public:
	CFile m_ReadFile;
	CFile m_WriteFile;

public:
	bool
	Create ();

	void
	Close ()
	{
		m_ReadFile.Close ();
		m_WriteFile.Close ();
	}

	size_t
	Read (
		void* p,
		size_t Size
		) const
	{
		return m_ReadFile.Read (p, Size);
	}

	size_t
	Write (
		const void* p,
		size_t Size
		)
	{
		return m_WriteFile.Write (p, Size);
	}
};

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
