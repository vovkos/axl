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

#include "pch.h"
#include "axl_io_HidReportDescriptor.h"

namespace axl {
namespace io {

//..............................................................................

class InitHidRdItemTagStringTable {
public:
	void
	operator () (const char* stringTable[]) const {
		// main

		stringTable[HidRdItemTag_Input >> 2]         =  "Input";
		stringTable[HidRdItemTag_Output >> 2]        =  "Output";
		stringTable[HidRdItemTag_Feature >> 2]       =  "Feature";
		stringTable[HidRdItemTag_Collection >> 2]    =  "Collection";
		stringTable[HidRdItemTag_CollectionEnd >> 2] =  "CollectionEnd";

		// global

		stringTable[HidRdItemTag_UsagePage >> 2]       = "UsagePage";
		stringTable[HidRdItemTag_LogicalMinimum >> 2]  = "LogicalMinimum";
		stringTable[HidRdItemTag_LogicalMaximum >> 2]  = "LogicalMaximum";
		stringTable[HidRdItemTag_PhysicalMinimum >> 2] = "PhysicalMinimum";
		stringTable[HidRdItemTag_PhysicalMaximum >> 2] = "PhysicalMaximum";
		stringTable[HidRdItemTag_UnitExponent >> 2]    = "UnitExponent";
		stringTable[HidRdItemTag_Unit >> 2]            = "Unit";
		stringTable[HidRdItemTag_ReportSize >> 2]      = "ReportSize";
		stringTable[HidRdItemTag_ReportId >> 2]        = "ReportId";
		stringTable[HidRdItemTag_ReportCount >> 2]     = "ReportCount";
		stringTable[HidRdItemTag_Push >> 2]            = "Push";
		stringTable[HidRdItemTag_Pop >> 2]             = "Pop";

		// local

		stringTable[HidRdItemTag_Usage >> 2]             = "Usage";
		stringTable[HidRdItemTag_UsageMinimum >> 2]      = "UsageMinimum";
		stringTable[HidRdItemTag_UsageMaximum >> 2]      = "UsageMaximum";
		stringTable[HidRdItemTag_DesignatorIndex >> 2]   = "DesignatorIndex";
		stringTable[HidRdItemTag_DesignatorMinimum >> 2] = "DesignatorMinimum";
		stringTable[HidRdItemTag_DesignatorMaximum >> 2] = "DesignatorMaximum";
		stringTable[HidRdItemTag_String >> 2]            = "String";
		stringTable[HidRdItemTag_StringMinimum >> 2]     = "StringMinimum";
		stringTable[HidRdItemTag_StringMaximum >> 2]     = "StringMaximum";
		stringTable[HidRdItemTag_Delimiter >> 2]         = "Delimiter";
	}
};

const char*
getHidRdItemTagString(HidRdItemTag tag) {
	static const char* stringTable[256 >> 2] = { 0 };
	sl::callOnce(InitHidRdItemTagStringTable(), stringTable);
	const char* string = stringTable[(uint8_t)tag >> 2];
	return string ? string : "?";
}

//..............................................................................

bool
parseHidRd(
	const void* p0,
	size_t size
) {
	static const size_t sizeTable[] = { 0, 1, 2, 4 };

	const uchar_t* p = (uchar_t*)p0;
	const uchar_t* end = p + size;
	while (p < end) {
		uchar_t a = *p++;
		HidRdItemTag tag = (HidRdItemTag)(a & ~0x03);
		size_t size = sizeTable[a & 0x03];
		uint32_t data = 0;
		memcpy(&data, p, size);

		printf("%s -> %d\n", getHidRdItemTagString(tag), data);
		p += size;
	}

	return true;
}

//..............................................................................

} // namespace io
} // namespace axl
