// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_MERGECRITERIA_H

#include "axl_log_BinDataConfig.h"
#include "axl_log_PacketFile.h"

namespace axl {
namespace log {

//.............................................................................

enum EMergeFlag
{
	EMergeFlag_MergeableBackward  = 0x08,
	EMergeFlag_MergeableForward   = 0x04,
	EMergeFlag_OverrideLineAttr   = 0x02,
	EMergeFlag_Mergeable          = 0x0c,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
uint_t
GetMergeFlagsFromPacketCode (uint32_t PacketCode)
{
	return PacketCode >> 28;
}

//.............................................................................

struct TMergeCriteria
{
	uint32_t m_PartCode;
	uint32_t m_MergeFlags:8;
	uint32_t m_LineKind:8;
	uint64_t m_Timestamp;
	TBinDataConfig m_BinDataConfig;

	TMergeCriteria ();

	void 
	Break ()
	{
		m_MergeFlags = 0;
	}

	bool 
	ShouldMerge (const TMergeCriteria& Criteria2) const;
};

//.............................................................................

} // namespace log
} // namespace axl
