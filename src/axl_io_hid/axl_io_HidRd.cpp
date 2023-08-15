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
#include "axl_io_HidUsageDb.h"
#include "axl_sl_CallOnce.h"

namespace axl {
namespace io {

//..............................................................................

void
HidRdItemTable::set(
	HidRdItemId id,
	uint32_t data
) {
	ASSERT((size_t)id < countof(m_table));

	// special handling for multi-usage is required

	if (id != HidRdItemId_Usage || !(m_mask & HidRdItemMask_Usage)) {
		m_table[id] = data;
		m_mask |= 1 << id;
	} else if (!m_usageTable.isEmpty())
		m_usageTable.append(data);
	else {
		m_usageTable.setCount(2);
		m_usageTable[0] = m_table[HidRdItemId_Usage];
		m_usageTable[1] = data;
	}
}

//..............................................................................

class InitHidRdTagItemIdTable {
public:
	void
	operator () (HidRdItemId idTable[]) const {
		memset(idTable, -1, sizeof(HidRdItemId) * (256 >> 2));

		// global

		idTable[HidRdTag_UsagePage >> 2]       = HidRdItemId_UsagePage;
		idTable[HidRdTag_LogicalMinimum >> 2]  = HidRdItemId_LogicalMinimum;
		idTable[HidRdTag_LogicalMaximum >> 2]  = HidRdItemId_LogicalMaximum;
		idTable[HidRdTag_PhysicalMinimum >> 2] = HidRdItemId_PhysicalMinimum;
		idTable[HidRdTag_PhysicalMaximum >> 2] = HidRdItemId_PhysicalMaximum;
		idTable[HidRdTag_UnitExponent >> 2]    = HidRdItemId_UnitExponent;
		idTable[HidRdTag_Unit >> 2]            = HidRdItemId_Unit;
		idTable[HidRdTag_ReportSize >> 2]      = HidRdItemId_ReportSize;
		idTable[HidRdTag_ReportId >> 2]        = HidRdItemId_ReportId;
		idTable[HidRdTag_ReportCount >> 2]     = HidRdItemId_ReportCount;

		// local

		idTable[HidRdTag_Usage >> 2]             = HidRdItemId_Usage;
		idTable[HidRdTag_UsageMinimum >> 2]      = HidRdItemId_UsageMinimum;
		idTable[HidRdTag_UsageMaximum >> 2]      = HidRdItemId_UsageMaximum;
		idTable[HidRdTag_DesignatorIndex >> 2]   = HidRdItemId_DesignatorIndex;
		idTable[HidRdTag_DesignatorMinimum >> 2] = HidRdItemId_DesignatorMinimum;
		idTable[HidRdTag_DesignatorMaximum >> 2] = HidRdItemId_DesignatorMaximum;
		idTable[HidRdTag_String >> 2]            = HidRdItemId_String;
		idTable[HidRdTag_StringMinimum >> 2]     = HidRdItemId_StringMinimum;
		idTable[HidRdTag_StringMaximum >> 2]     = HidRdItemId_StringMaximum;
		idTable[HidRdTag_Delimiter >> 2]         = HidRdItemId_Delimiter;
	}
};

HidRdItemId
getHidRdTagItemId(HidRdTag tag) {
	static HidRdItemId idTable[256 >> 2];
	sl::callOnce(InitHidRdTagItemIdTable(), idTable);
	size_t i = tag >> 2;
	return i < countof(idTable) ? idTable[i] : HidRdItemId_Invalid;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class InitHidRdTagStringTable {
public:
	void
	operator () (const char* stringTable[]) const {
		// main

		stringTable[HidRdTag_Input >> 2]         =  "Input";
		stringTable[HidRdTag_Output >> 2]        =  "Output";
		stringTable[HidRdTag_Feature >> 2]       =  "Feature";
		stringTable[HidRdTag_Collection >> 2]    =  "Collection";
		stringTable[HidRdTag_CollectionEnd >> 2] =  "CollectionEnd";

		// global

		stringTable[HidRdTag_UsagePage >> 2]       = "UsagePage";
		stringTable[HidRdTag_LogicalMinimum >> 2]  = "LogicalMinimum";
		stringTable[HidRdTag_LogicalMaximum >> 2]  = "LogicalMaximum";
		stringTable[HidRdTag_PhysicalMinimum >> 2] = "PhysicalMinimum";
		stringTable[HidRdTag_PhysicalMaximum >> 2] = "PhysicalMaximum";
		stringTable[HidRdTag_UnitExponent >> 2]    = "UnitExponent";
		stringTable[HidRdTag_Unit >> 2]            = "Unit";
		stringTable[HidRdTag_ReportSize >> 2]      = "ReportSize";
		stringTable[HidRdTag_ReportId >> 2]        = "ReportId";
		stringTable[HidRdTag_ReportCount >> 2]     = "ReportCount";
		stringTable[HidRdTag_Push >> 2]            = "Push";
		stringTable[HidRdTag_Pop >> 2]             = "Pop";

		// local

		stringTable[HidRdTag_Usage >> 2]             = "Usage";
		stringTable[HidRdTag_UsageMinimum >> 2]      = "UsageMinimum";
		stringTable[HidRdTag_UsageMaximum >> 2]      = "UsageMaximum";
		stringTable[HidRdTag_DesignatorIndex >> 2]   = "DesignatorIndex";
		stringTable[HidRdTag_DesignatorMinimum >> 2] = "DesignatorMinimum";
		stringTable[HidRdTag_DesignatorMaximum >> 2] = "DesignatorMaximum";
		stringTable[HidRdTag_String >> 2]            = "String";
		stringTable[HidRdTag_StringMinimum >> 2]     = "StringMinimum";
		stringTable[HidRdTag_StringMaximum >> 2]     = "StringMaximum";
		stringTable[HidRdTag_Delimiter >> 2]         = "Delimiter";
	}
};

sl::StringRef
getHidRdTagString(HidRdTag tag) {
	static const char* stringTable[256 >> 2] = { 0 };
	sl::callOnce(InitHidRdTagStringTable(), stringTable);
	size_t i = tag >> 2;
	const char* tagString = i < countof(stringTable) ? stringTable[i] : NULL;
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::StringRef
getHidReportKindString(HidReportKind reportKind) {
	static const char* stringTable[] = {
		"Input",     // HidReportKind_Input,
		"Output",    // HidReportKind_Output,
		"Feature",   // HidReportKind_Feature,
	};

	return (size_t)reportKind < countof(stringTable) ?
		sl::StringRef(stringTable[reportKind]) :
		sl::formatString("Report 0x%x", reportKind);
}

//..............................................................................

void
HidReportField::finalize() {
	size_t reportSize = (*this)[HidRdItemId_ReportSize];
	size_t reportCount = (*this)[HidRdItemId_ReportCount];
	m_bitCount = reportSize * reportCount;

	if (isPadding() || !(m_valueFlags & HidRdValueFlag_Variable)) // arrays don't need usage tables
		return;

	size_t usageCount = m_usageTable.getCount();
	m_usageTable.setCount(reportCount);

	if (isSet(HidRdItemId_Usage)) {
		uint32_t usage = (*this)[HidRdItemId_Usage];
		for (size_t i = usageCount; i < reportCount; i++)
			m_usageTable[i] = usage;
	} else {
		uint32_t usage = isSet(HidRdItemMask_UsageMinimum) ?
			(*this)[HidRdItemId_UsageMinimum] :
			(*this)[HidRdItemId_UsageMaximum] - reportCount + 1;

		for (size_t i = 0; i < reportCount; i++, usage++)
			m_usageTable[i] = usage;
	}
}

//..............................................................................

int
extractValue(
	const uchar_t* p,
	size_t bitOffset,
	size_t bitCount
) {
	p += bitOffset / 8;
	int value = (int)(*(uint64_t*)p >> (bitOffset & 7));
	int mask = (1 << bitCount) - 1;

	return bitCount > 1 && (value & (1 << (bitCount - 1))) ? // negative
		(value & mask) | ((int)-1 & ~mask) :
		value & mask;
}

template <typename DecodeImplFunc>
size_t
decodeImpl(
	const void* p,
	size_t bitOffset,
	const HidReportField& field,
	DecodeImplFunc decodeImplFunc
) {
	size_t reportSize = field[HidRdItemId_ReportSize];
	size_t reportCount = field[HidRdItemId_ReportCount];

	for (size_t i = 0; i < reportCount; i++, bitOffset += reportSize) {
		int value = extractValue((uchar_t*)p, bitOffset, reportSize);
		if (value)
			decodeImplFunc(field, i, value);
	}

	return bitOffset;
}

void
decodeVariable(
	const HidReportField& field,
	size_t i,
	int value
) {
	const HidUsagePage* page = field.getUsagePage();
	uint_t usage = field.getUsage(i);
	printf("%s: %d\n", page->getUsageName(usage).sz(), value);
}

void
decodeArray(
	const HidReportField& field,
	size_t i,
	int value
) {
	const HidUsagePage* page = field.getUsagePage();
	uint_t usage = field[HidRdItemId_UsageMinimum] + value;
	printf("[%d]: %s\n", i, page->getUsageName(usage).sz());
}

void
HidReport::decode(const void* p) const {
	size_t bitOffset = 0;

	sl::ConstIterator<HidReportField> it = m_fieldList.getHead();
	for (; it; it++) {
		const HidReportField& field = **it;
		if (field.isPadding()) {
			bitOffset += field.getBitCount();
			continue;
		}

		bitOffset = (field.getValueFlags() & HidRdValueFlag_Variable) ?
			decodeImpl(p, bitOffset, field, decodeVariable) :
			decodeImpl(p, bitOffset, field, decodeArray);
	}
}

//..............................................................................

HidReport*
HidRd::getReport(
	HidReportKind reportKind,
	uint_t reportId
) {
	ASSERT((size_t)reportKind < countof(m_reportMapTable));

	sl::MapIterator<uint_t, HidReport> it = m_reportMapTable[reportKind].visit(reportId);
	HidReport* report = &it->m_value;
	if (report->m_reportKind == HidReportKind_Invalid) {
		report->m_reportKind = reportKind;
		report->m_reportId = reportId;
	} else
		ASSERT(
			report->m_reportKind == reportKind &&
			report->m_reportId == reportId
		);

	return report;
}

void
HidRd::finalize() {
	for (uint_t i = 0; i < HidReportKind__Count; i++) {
		sl::MapIterator<uint_t, HidReport> it = m_reportMapTable[i].getHead();
		for (; it; it++)
			it->m_value.updateSize();
	}
}

void
HidRd::print() {
	printf("HID RD (flags: 0x%x)\n", m_flags);
	sl::String indent;
	for (uint_t i = 0; i < HidReportKind__Count; i++) {
		sl::ConstMapIterator<uint_t, HidReport> it = m_reportMapTable[i].getHead();
		for (; it; it++) {
			const HidReport& report = it->m_value;
			indent.copy(' ', 4);
			printf(
				"%s%s (id: %d, size: %d bits / %d bytes)\n",
				indent.sz(),
				getHidReportKindString(report.m_reportKind).sz(),
				report.m_reportId,
				report.m_bitCount,
				report.m_size
			);

			sl::ConstIterator<HidReportField> it2 = report.m_fieldList.getHead();
			for (; it2; it2++) {
				const HidReportField& field = **it2;
				const HidUsagePage* page = field.getUsagePage();
				indent.copy(' ', 8);

				if (field.isPadding()) {
					printf(
						"%sPadding (offset: %d bits, size: %d bits)\n",
				    	indent.sz(),
						field.getBitOffset(),
						field.getBitCount()
					);

					continue;
				}

				printf(
					"%sField (%s, offset: %d bits, size: %d bits)\n",
			    	indent.sz(),
					page->getName().sz(),
					field.getBitOffset(),
					field.getBitCount()
				);

				indent.copy(' ', 12);

				if (it2->isSet(HidRdItemId_Usage))
					printf(
						"%sUsage: %s\n",
						indent.sz(),
						page->getUsageName(field[HidRdItemId_Usage]).sz()
					);

				if (it2->isSet(HidRdItemId_UsageMinimum))
					printf(
						"%sUsage Min: %s\n",
						indent.sz(),
						page->getUsageName(field[HidRdItemId_UsageMinimum]).sz()
					);

				if (it2->isSet(HidRdItemId_UsageMaximum))
					printf(
						"%sUsage Max: %s\n",
						indent.sz(),
						page->getUsageName(field[HidRdItemId_UsageMaximum]).sz()
					);

				printf(
					"%sReport Size: %d\n",
					indent.sz(),
					field[HidRdItemId_ReportSize]
				);

				printf(
					"%sReport Count: %d\n",
					indent.sz(),
					field[HidRdItemId_ReportCount]
				);
			}
		}
	}
}

//..............................................................................

} // namespace io
} // namespace axl
