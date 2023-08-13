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

sl::StringRef
getHidRdItemTagString(HidRdItemTag tag) {
	static const char* stringTable[256 >> 2] = { 0 };
	sl::callOnce(InitHidRdItemTagStringTable(), stringTable);
	const char* tagString = (size_t)tag < 256 ? stringTable[(uint8_t)tag >> 2] : NULL;
	return tagString ?
		sl::StringRef(tagString) :
		sl::formatString("Item 0x%02x", tag);
}

//..............................................................................

sl::StringRef
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

sl::StringRef
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
		sl::StringRef(stringTable[kind]) :
		sl::formatString("Collection 0x%02x", kind);
}


//..............................................................................

sl::StringRef
getHidRdUnitSystemString(HidRdUnitSystem system) {
	static const char* stringTable[] = {
		"None",              // HidRdUnitSystem_None             = 0x00,
		"SI Linear",         // HidRdUnitSystem_SiLinear         = 0x01,
		"SI Rotation",       // HidRdUnitSystem_SiRotation       = 0x02,
		"English Linear",    // HidRdUnitSystem_EnglishLinear    = 0x03,
		"English Rotation",  // HidRdUnitSystem_EnglishRotation  = 0x04,
	};

	return (size_t)system < countof(stringTable) ?
		sl::StringRef(stringTable[system]) :
		sl::formatString("Unit System 0x%x", system);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

HidRdUnit
getHidRdUnit(
	HidRdUnitNibbleRole role,
	HidRdUnitSystem system
) {
	HidRdUnit unitTable[7][5] = {
		//     None       SI Linear             SI Rotation        English Linear       English Rotation
		{ HidRdUnit_None, HidRdUnit_None,       HidRdUnit_None,    HidRdUnit_None,      HidRdUnit_None },      // nibble 0 (system)
		{ HidRdUnit_None, HidRdUnit_Centimiter, HidRdUnit_Radian,  HidRdUnit_Inch,      HidRdUnit_Degree },    // nibble 1 (length)
		{ HidRdUnit_None, HidRdUnit_Gram,       HidRdUnit_Gram,    HidRdUnit_Slug,      HidRdUnit_Slug },      // nibble 2 (mass)
		{ HidRdUnit_None, HidRdUnit_Second,     HidRdUnit_Second,  HidRdUnit_Second,    HidRdUnit_Second },    // nibble 3 (time)
		{ HidRdUnit_None, HidRdUnit_Kelvin,     HidRdUnit_Kelvin,  HidRdUnit_Farenheit, HidRdUnit_Farenheit }, // nibble 4 (temperature)
		{ HidRdUnit_None, HidRdUnit_Ampere,     HidRdUnit_Ampere,  HidRdUnit_Ampere,    HidRdUnit_Ampere },    // nibble 5 (current)
		{ HidRdUnit_None, HidRdUnit_Candela,    HidRdUnit_Candela, HidRdUnit_Candela,   HidRdUnit_Candela },   // nibble 6 (luminous intensity)
	};

	return (size_t)role < countof(unitTable) && (size_t)system <= countof(unitTable[0]) ?
		unitTable[role][system] :
		HidRdUnit_None;
}

sl::StringRef
getHidRdUnitNameString(HidRdUnit unit) {
	static const char* stringTable[] = {
		"None",        // HidRdUnit_None,
		"Centimiter",  // HidRdUnit_Centimiter,
		"Radian",      // HidRdUnit_Radian,
		"Inch",        // HidRdUnit_Inch,
		"Degree",      // HidRdUnit_Degree,
		"Gram",        // HidRdUnit_Gram,
		"Slug",        // HidRdUnit_Slug,
		"Second",      // HidRdUnit_Second,
		"Kelvin",      // HidRdUnit_Kelvin,
		"Farenheit",   // HidRdUnit_Farenheit,
		"Ampere",      // HidRdUnit_Ampere,
		"Candela",     // HidRdUnit_Candela,
	};

	return (size_t)unit < countof(stringTable) ?
		sl::StringRef(stringTable[unit]) :
		sl::formatString("Unit 0x%x", unit);
}

sl::StringRef
getHidRdUnitAbbrString(HidRdUnit unit) {
	static const char* stringTable[] = {
		"none",  // HidRdUnit_None,
		"cm",    // HidRdUnit_Centimiter,
		"rad",   // HidRdUnit_Radian,
		"in",    // HidRdUnit_Inch,
		"deg",   // HidRdUnit_Degree,
		"g",     // HidRdUnit_Gram,
		"slug",  // HidRdUnit_Slug,
		"s",     // HidRdUnit_Second,
		"K",     // HidRdUnit_Kelvin,
		"F",     // HidRdUnit_Farenheit,
		"A",     // HidRdUnit_Ampere,
		"cd",    // HidRdUnit_Candela,
	};

	return (size_t)unit < countof(stringTable) ?
		sl::StringRef(stringTable[unit]) :
		sl::formatString("unit 0x%x", unit);
}

sl::StringRef
getHidRdComplexUnitString(uint32_t unit) {
	HidRdUnitSystem system = (HidRdUnitSystem)(unit & 0x0000000f);

	sl::String string;
	sl::StringRef stringRef;

	for (uint_t i = 1, j = 0, shift = 4; i < HidRdUnitNibbleRole__Count; i++, shift += 4) {
		uint_t nibble = (unit & (0xf << shift)) >> shift;
		HidRdUnit unit = getHidRdUnit((HidRdUnitNibbleRole)i, system);
		int exponent = getHidRdUnitExponent(nibble);
		if (!exponent)
			continue;

		if (j == 0 && exponent == 1) // first simple unit
			stringRef = getHidRdUnitAbbrString(unit);
		else {
			if (string.isEmpty())
				string = stringRef;

			stringRef = getHidRdUnitAbbrString(unit);
			if (exponent < 0) {
				if (string.isEmpty())
					string = '1';

				string.appendFormat(
					exponent == -1 ? "/%s" : "/%s%d",
					stringRef.sz(),
					-exponent
				);
			} else {
				if (!string.isEmpty())
					string += '*';

				string.appendFormat(
					exponent == 1 ? "%s" : "%s%d",
					stringRef.sz(),
					exponent
				);
			}
		}

		j++;
	}

	return string.isEmpty() ? stringRef : string;
}

//..............................................................................

} // namespace io
} // namespace axl
