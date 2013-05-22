// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_VOLATILEPACKETFILTER_H

#include "axl_obj_Root.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace log {

//.............................................................................

struct IVolatilePacketFilter: public obj::IRoot
{
	// {8E9652A7-4A19-47E8-90F2-83C913796A5C}
	AXL_OBJ_INTERFACE (
		IVolatilePacketFilter,
		0x8e9652a7, 0x4a19, 0x47e8, 0x90, 0xf2, 0x83, 0xc9, 0x13, 0x79, 0x6a, 0x5c
		)

	virtual 
	bool 
	Filter (
		uint32_t PacketCode, 
		const void* p, 
		size_t Size, 
		uint_t* pVolatileFlags
		) = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CVolatilePacketFilterStack: 
	public IVolatilePacketFilter,
	public rtl::CArrayT <IVolatilePacketFilter*>
{
public:
	virtual 
	bool 
	Filter (
		uint32_t PacketCode, 
		const void* p, 
		size_t Size, 
		uint_t* pVolatileFlags
		);
};

//.............................................................................

} // namespace log 
} // namespace axl
