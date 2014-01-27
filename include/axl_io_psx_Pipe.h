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
	CFile m_FileA;
	CFile m_FileB;

public:
	bool
	Create ();

	void
	Close ()
	{
		m_FileA.Close ();
		m_FileB.Close ();
	}
};

//.............................................................................

} // namespace psx
} // namespace io
} // namespace axl
