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
#include "axl_io_HidDb.h"
#include "axl_io_HidRdParser.h"
#include "axl_io_HidReportSerializer.h"
#include "axl_sl_CallOnce.h"

namespace axl {
namespace io {

//..............................................................................

sl::StringRef
getHidRdItemString(HidRdItemId id) {
	static const char* stringTable[] = {
		"UsagePage",          // HidRdItemId_UsagePage = 0,
		"LogicalMinimum",     // HidRdItemId_LogicalMinimum,
		"LogicalMaximum",     // HidRdItemId_LogicalMaximum,
		"PhysicalMinimum",    // HidRdItemId_PhysicalMinimum,
		"PhysicalMaximum",    // HidRdItemId_PhysicalMaximum,
		"UnitExponent",       // HidRdItemId_UnitExponent,
		"Unit",               // HidRdItemId_Unit,
		"ReportSize",         // HidRdItemId_ReportSize,
		"ReportId",           // HidRdItemId_ReportId,
		"ReportCount",        // HidRdItemId_ReportCount,

		// local

		"Usage",             // HidRdItemId_Usage,
		"UsageMinimum",      // HidRdItemId_UsageMinimum,
		"UsageMaximum",      // HidRdItemId_UsageMaximum,
		"DesignatorIndex",   // HidRdItemId_DesignatorIndex,
		"DesignatorMinimum", // HidRdItemId_DesignatorMinimum,
		"DesignatorMaximum", // HidRdItemId_DesignatorMaximum,
		"String",            // HidRdItemId_String,
		"StringMinimum",     // HidRdItemId_StringMinimum,
		"StringMaximum",     // HidRdItemId_StringMaximum,
		"Delimiter",         // HidRdItemId_Delimiter,
	};

	return (size_t)id < countof(stringTable) ?
		sl::StringRef(stringTable[id]) :
		sl::formatString("Item 0x%02x", id);
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

inline
void
printItem(
	const sl::StringRef& indent,
	const HidReportField& field,
	HidRdItemId id
) {
	if (field.isSet(id))
		printf(
			"%s%s: %d\n",
			indent.sz(),
			getHidRdItemString(id).sz(),
			(int)field[id]
		);
}

void
printFieldArray(
	sl::String* indent,
	const sl::ArrayRef<const HidReportField*>& fieldArray
) {
	size_t count = fieldArray.getCount();
	for (size_t i = 0; i < count; i++) {
		const HidReportField& field = *fieldArray[i];
		const HidUsagePage* page = field.getUsagePage();

		if (field.isPadding()) {
			printf("%sField: Padding %d-bit\n", indent->sz(), field.getBitCount());
			continue;
		}

		printf(
			"%sField: %s %d-bit\n",
	    	indent->sz(),
			getHidReportTypeString(field.getReport()->getReportType()).sz(),
			field.getBitCount()
		);

		indent->append(' ', 4);

		printf(
			"%sUsagePage: %s\n",
			indent->sz(),
			page->getName().sz()
		);

		if (field.isSet(HidRdItemId_Usage)) {
			size_t auxUsageCount = field.getAuxUsageCount();
			for (size_t i = 0; i <= auxUsageCount; i++) {
				uint_t usage = field.getUsage(i);
				printf(
					"%sUsage: %s\n",
					indent->sz(),
					page->getUsageName(usage).sz()
				);
			}
		}

		if (field.isSet(HidRdItemId_UsageMinimum))
			printf(
				"%sUsageMinimum: %s\n",
				indent->sz(),
				page->getUsageName(field[HidRdItemId_UsageMinimum]).sz()
			);

		if (field.isSet(HidRdItemId_UsageMaximum))
			printf(
				"%sUsageMaximum: %s\n",
				indent->sz(),
				page->getUsageName(field[HidRdItemId_UsageMaximum]).sz()
			);

		printItem(*indent, field, HidRdItemId_LogicalMinimum);
		printItem(*indent, field, HidRdItemId_LogicalMaximum);
		printItem(*indent, field, HidRdItemId_PhysicalMinimum);
		printItem(*indent, field, HidRdItemId_PhysicalMaximum);
		printItem(*indent, field, HidRdItemId_ReportId);
		printItem(*indent, field, HidRdItemId_ReportSize);
		printItem(*indent, field, HidRdItemId_ReportCount);

		indent->chop(4);
	}
}

void
printCollection(
	sl::String* indent,
	const HidRdCollection* collection
) {
	const HidUsagePage* page = collection->getUsagePage();
	uint_t usage = collection->getUsage();

	printf(
		"%sCollection: %s (%s: %s)\n",
		indent->sz(),
		getHidRdCollectionKindString(collection->getCollectionKind()).sz(),
		page->getName().sz(),
		page->getUsageName(usage).sz()
	);

	indent->append(' ', 4);
	printFieldArray(indent, collection->getFieldArray());
	sl::ConstIterator<HidRdCollection> it = collection->getCollectionList().getHead();
	for (; it; it++)
		printCollection(indent, *it);

	indent->chop(4);
}

//..............................................................................

void
HidRdItemTable::set(
	HidRdItemId id,
	uint32_t data
) {
	ASSERT((size_t)id < countof(m_table));

	// special handling for multi-usage is required

	if (id == HidRdItemId_Usage && (m_mask & HidRdItemMask_Usage))
		m_auxUsageTable.append(data);
	else {
		m_table[id] = data;
		m_mask |= 1 << id;
	}
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
getHidReportTypeString(HidReportType reportType) {
	static const char* stringTable[] = {
		"Undefined", // HidReportType_Undefined,
		"Input",     // HidReportType_Input,
		"Output",    // HidReportType_Output,
		"Feature",   // HidReportType_Feature,
	};

	return (size_t)reportType < countof(stringTable) ?
		sl::StringRef(stringTable[reportType]) :
		sl::formatString("Report 0x%x", reportType);
}

//..............................................................................

size_t
HidReport::saveDecodeInfo(sl::Array<char>* buffer) const {
	return HidReportSerializer::saveReportDecodeInfo(buffer, *this);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
HidStandaloneReport::loadDecodeInfo(
	const HidDb* db,
	const void* p,
	size_t size
) {
	return HidReportSerializer::loadReportDecodeInfo(this, db, p, size);
}

void
HidReport::print(sl::String* indent) const {
	printf(
		"%sReport: %s %d-bit (%d bytes)\n",
		indent->sz(),
		getHidReportTypeString(m_reportType).sz(),
		m_bitCount,
		m_size
	);

	indent->append(' ', 4);

	printf(
		"%sReportId: %d\n",
		indent->sz(),
		m_reportId
	);

	printFieldArray(indent, m_fieldArray);
	indent->chop(4);
}

void
HidReport::decode(const void* p) const {
	size_t bitOffset = 0;

	size_t count = m_fieldArray.getCount();
	for (size_t i = 0; i < count; i++) {
		const HidReportField& field = *m_fieldArray[i];
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
	HidReportType reportType,
	uint_t reportId
) {
	ASSERT((size_t)(reportType - 1) < countof(m_reportMapTable));

	sl::MapIterator<uint_t, HidReport> it = m_reportMapTable[reportType - 1].visit(reportId);
	HidReport* report = &it->m_value;
	if (report->m_reportType == HidReportType_Undefined) {
		report->m_reportType = reportType;
		report->m_reportId = reportId;
	} else
		ASSERT(
			report->m_reportType == reportType &&
			report->m_reportId == reportId
		);

	return report;
}

void
HidRd::clear() {
	m_rootCollection.clear();
	m_fieldList.clear();

	for (size_t i = 0; i < countof(m_reportMapTable); i++)
		m_reportMapTable[i].clear();

	m_flags = 0;
}

void
HidRd::parse(
	const HidDb* db,
	const void* p,
	size_t size
) {
	clear();

	HidRdParser parser(db, this);
	parser.parse(p, size);

	for (uint_t i = 0; i < countof(m_reportMapTable); i++) {
		sl::MapIterator<uint_t, HidReport> it = m_reportMapTable[i].getHead();
		for (; it; it++)
			it->m_value.updateSize();
	}
}

//..............................................................................

void
HidRd::printReports() const {
	printf("HID RD Reports%s\n", (m_flags & HidRdFlag_HasReportId) ? " (with IDs)" : "");

	sl::String indent(' ', 4);
	for (uint_t i = 0; i < countof(m_reportMapTable); i++) {
		sl::ConstMapIterator<uint_t, HidReport> it = m_reportMapTable[i].getHead();
		for (; it; it++) {
			const HidReport& report = it->m_value;
			report.print(&indent);
		}
	}
}

void
HidRd::printCollections() const {
	printf("HID RD Collections%s\n", (getFlags() & HidRdFlag_HasReportId) ? " (with IDs)" : "");

	sl::String indent(' ', 4);
	sl::ConstIterator<HidRdCollection> it = m_rootCollection.getCollectionList().getHead();
	for (; it; it++)
		printCollection(&indent, *it);

	printFieldArray(&indent, m_rootCollection.getFieldArray());
}

//..............................................................................

} // namespace io
} // namespace axl
