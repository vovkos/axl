// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_SYSTEMINFO_H

#include "axl_g_Def.h"

namespace axl {
namespace win {

//.............................................................................

struct TSystemInfo
{
	ulong_t m_PageSize;
	ulong_t m_AllocationGranularity;
	ulong_t m_ProcessorCount;
	ulong_t m_ProcessorArchitecture;
	ulong_t m_ProcessorLevel;
	ulong_t m_ProcessorRevision;
};

//.............................................................................

} // namespace axl
} // namespace win
