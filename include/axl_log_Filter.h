// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_FILTER_H

#include "axl_log_PacketFile.h"
#include "axl_obj_Root.h"

namespace axl {
namespace log {

//.............................................................................

struct IFilter: public obj::IRoot
{
	// {CFAF2B93-663D-4BBD-8E4C-BD5E26D89F92}
	AXL_OBJ_INTERFACE (
		IFilter,
		0xcfaf2b93, 0x663d, 0x4bbd, 0x8e, 0x4c, 0xbd, 0x5e, 0x26, 0xd8, 0x9f, 0x92
		)

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
