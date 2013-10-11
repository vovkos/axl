// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_FILTER_H

#include "axl_log_PacketFile.h"

namespace axl {
namespace log {

//.............................................................................

struct CFilter
{
	virtual 
	bool
	Filter (
		CPacketFile* pTargetFile, 
		uint64_t Timestamp,
		uint_t Code, 
		const void* p, 
		size_t Size
		) = 0;

	virtual 
	void 
	Reset ()
	{ 
	}
};

//.............................................................................

} // namespace log 
} // namespace axl
