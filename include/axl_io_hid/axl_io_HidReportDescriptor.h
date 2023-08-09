//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_IO_HIDREPORTDESCRIPTOR_H

#include "axl_io_HidPch.h"
#include "axl_sl_StringHashTable.h"

namespace axl {
namespace io {

//..............................................................................

enum HidRdItemTag {
	// main

	HidRdItemTag_Input             = 0x80, // 1000 00 nn
	HidRdItemTag_Output            = 0x90, // 1001 00 nn
	HidRdItemTag_Feature           = 0xB0, // 1011 00 nn
	HidRdItemTag_Collection        = 0xA0, // 1010 00 nn
	HidRdItemTag_CollectionEnd     = 0xC0, // 1100 00 nn

	// global

	HidRdItemTag_UsagePage         = 0x04, // 0000 01 nn
	HidRdItemTag_LogicalMinimum    = 0x14, // 0001 01 nn
	HidRdItemTag_LogicalMaximum    = 0x24, // 0010 01 nn
	HidRdItemTag_PhysicalMinimum   = 0x34, // 0011 01 nn
	HidRdItemTag_PhysicalMaximum   = 0x44, // 0100 01 nn
	HidRdItemTag_UnitExponent      = 0x54, // 0101 01 nn
	HidRdItemTag_Unit              = 0x64, // 0110 01 nn
	HidRdItemTag_ReportSize        = 0x74, // 0111 01 nn
	HidRdItemTag_ReportId          = 0x84, // 1000 01 nn
	HidRdItemTag_ReportCount       = 0x94, // 1001 01 nn
	HidRdItemTag_Push              = 0xA4, // 1010 01 nn
	HidRdItemTag_Pop               = 0xB4, // 1011 01 nn

	// local

	HidRdItemTag_Usage             = 0x08, // 0000 10 nn
	HidRdItemTag_UsageMinimum      = 0x18, // 0001 10 nn
	HidRdItemTag_UsageMaximum      = 0x28, // 0010 10 nn
	HidRdItemTag_DesignatorIndex   = 0x38, // 0011 10 nn
	HidRdItemTag_DesignatorMinimum = 0x48, // 0100 10 nn
	HidRdItemTag_DesignatorMaximum = 0x58, // 0101 10 nn
	HidRdItemTag_String            = 0x78, // 0111 10 nn
	HidRdItemTag_StringMinimum     = 0x88, // 1000 10 nn
	HidRdItemTag_StringMaximum     = 0x98, // 1001 10 nn
	HidRdItemTag_Delimiter         = 0xA8, // 1010 10 nn
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getHidRdItemTagString(HidRdItemTag tag);

//..............................................................................

enum HidRdMainValueFlag {
	HidRdMainValueFlag_Constant      = 0x0001, // otherwise, variable data
	HidRdMainValueFlag_Variable      = 0x0002, // otherwise, array
	HidRdMainValueFlag_Relative      = 0x0004, // otherwise, absolute
	HidRdMainValueFlag_Wrap          = 0x0008, // otherwise, no wrap
	HidRdMainValueFlag_Nonlinear     = 0x0010, // otherwise, linear
	HidRdMainValueFlag_NoPreferred   = 0x0020, // otherwise, preferred state
	HidRdMainValueFlag_NullState     = 0x0040, // otherwise, no null
	HidRdMainValueFlag_Volatile      = 0x0080, // otherwise, non-volatile
	HidRdMainValueFlag_BufferedBytes = 0x0100, // otherwise, bit field
};

//..............................................................................

struct HidRdItem: sl::ListLink {
	uint8_t m_tag;
	uint32_t m_value;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HidRd {
protected:


};

bool
parseHidRd(
	const void* p,
	size_t size
);

//..............................................................................

} // namespace io
} // namespace axl
