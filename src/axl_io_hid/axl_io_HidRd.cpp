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
#include "axl_io_HidRd.h"
#include "axl_sl_CallOnce.h"

namespace axl {
namespace io {

//..............................................................................

class InitHidRdItemTagStringTable {
public:
	void
	operator () (const char* stringTable[]) const {
		for (size_t i = 0; i < 256 >> 2; i++)
			stringTable[i] = "Unknown";

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
	return (size_t)tag < 256 ?
		stringTable[(uint8_t)tag >> 2] :
		"Unknown";
}

//..............................................................................

sl::String
getHidRdValueFlagsString(uint_t flags) {
	static const char* stringTable[] = {
		"Constant",       // HidRdValueFlag_Constant      = 0x0001,
		"Variable",       // HidRdValueFlag_Variable      = 0x0002,
		"Relative",       // HidRdValueFlag_Relative      = 0x0004,
		"Wrap",           // HidRdValueFlag_Wrap          = 0x0008,
		"Nonlinear",      // HidRdValueFlag_Nonlinear     = 0x0010,
		"NoPreferred",    // HidRdValueFlag_NoPreferred   = 0x0020,
		"NullState",      // HidRdValueFlag_NullState     = 0x0040,
		"Volatile",       // HidRdValueFlag_Volatile      = 0x0080,
		"BufferedBytes",  // HidRdValueFlag_BufferedBytes = 0x0100,
	};

	sl::String string;

	while (flags) {
		size_t i = sl::getLoBitIdx(flags);
		if (i < countof(stringTable)) {
			string += stringTable[i];
			string += ' ';
		}

		flags &= ~(1 << i);
	}

	string.trimRight();
	return string;
}

//..............................................................................

const char*
getHidRdCollectionKindString(HidRdCollectionKind kind) {
	static const char* stringTable[] = {
		"Physical",       // HidRdCollectionKind_Physical      = 0x00,
		"Application",    // HidRdCollectionKind_Application   = 0x01,
		"Logical",        // HidRdCollectionKind_Logical       = 0x02,
		"Report",         // HidRdCollectionKind_Report        = 0x03,
		"NamedArray",     // HidRdCollectionKind_NamedArray    = 0x04,
		"UsageSwitch",    // HidRdCollectionKind_UsageSwitch   = 0x05,
		"UsageModifier",  // HidRdCollectionKind_UsageModifier = 0x06,
	};

	return (size_t)kind < countof(stringTable) ?
		stringTable[kind] :
		"Unknown";
}

//..............................................................................

} // namespace io
} // namespace axl
