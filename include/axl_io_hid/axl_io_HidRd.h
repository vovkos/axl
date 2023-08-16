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

#define _AXL_IO_HIDRD_H

#include "axl_io_HidUsageDb.h"

namespace axl {
namespace io {

class HidRdParser;
class HidReport;

//..............................................................................

enum HidRdItemId {
	HidRdItemId_Invalid = -1,

	// globals

	HidRdItemId_UsagePage = 0,
	HidRdItemId_LogicalMinimum,  // signed
	HidRdItemId_LogicalMaximum,  // signed
	HidRdItemId_PhysicalMinimum, // signed
	HidRdItemId_PhysicalMaximum, // signed
	HidRdItemId_UnitExponent,    // signed
	HidRdItemId_Unit,
	HidRdItemId_ReportSize,
	HidRdItemId_ReportId,
	HidRdItemId_ReportCount,

	// locals (unsigned)

	HidRdItemId_Usage,
	HidRdItemId_UsageMinimum,
	HidRdItemId_UsageMaximum,
	HidRdItemId_DesignatorIndex,
	HidRdItemId_DesignatorMinimum,
	HidRdItemId_DesignatorMaximum,
	HidRdItemId_String,
	HidRdItemId_StringMinimum,
	HidRdItemId_StringMaximum,
	HidRdItemId_Delimiter,

	HidRdItemId__Count,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::StringRef
getHidRdItemString(HidRdItemId id);

inline
bool
isHidRdItemSigned(HidRdItemId id) {
	return id >= HidRdItemId_LogicalMinimum && id <= HidRdItemId_UnitExponent;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum HidRdItemMask {
	// global

	HidRdItemMask_UsagePage       = 1 << HidRdItemId_UsagePage,
	HidRdItemMask_LogicalMinimum  = 1 << HidRdItemId_LogicalMinimum,
	HidRdItemMask_LogicalMaximum  = 1 << HidRdItemId_LogicalMaximum,
	HidRdItemMask_PhysicalMinimum = 1 << HidRdItemId_PhysicalMinimum,
	HidRdItemMask_PhysicalMaximum = 1 << HidRdItemId_PhysicalMaximum,
	HidRdItemMask_UnitExponent    = 1 << HidRdItemId_UnitExponent,
	HidRdItemMask_Unit            = 1 << HidRdItemId_Unit,
	HidRdItemMask_ReportSize      = 1 << HidRdItemId_ReportSize,
	HidRdItemMask_ReportId        = 1 << HidRdItemId_ReportId,
	HidRdItemMask_ReportCount     = 1 << HidRdItemId_ReportCount,

	// local

	HidRdItemMask_Usage             = 1 << HidRdItemId_Usage,
	HidRdItemMask_UsageMinimum      = 1 << HidRdItemId_UsageMinimum,
	HidRdItemMask_UsageMaximum      = 1 << HidRdItemId_UsageMaximum,
	HidRdItemMask_DesignatorIndex   = 1 << HidRdItemId_DesignatorIndex,
	HidRdItemMask_DesignatorMinimum = 1 << HidRdItemId_DesignatorMinimum,
	HidRdItemMask_DesignatorMaximum = 1 << HidRdItemId_DesignatorMaximum,
	HidRdItemMask_String            = 1 << HidRdItemId_String,
	HidRdItemMask_StringMinimum     = 1 << HidRdItemId_StringMinimum,
	HidRdItemMask_StringMaximum     = 1 << HidRdItemId_StringMaximum,
	HidRdItemMask_Delimiter         = 1 << HidRdItemId_Delimiter,

	HidRdItemMask_AllGlobals = (1 << (HidRdItemId_ReportCount + 1)) - 1,
	HidRdItemMask_AllLocals  = ((1 << (HidRdItemId_Delimiter + 1)) - 1) & ~((1 << HidRdItemId_Usage) - 1),
	HidRdItemMask_AllUsages =
		HidRdItemMask_Usage |
		HidRdItemMask_UsageMinimum |
		HidRdItemMask_UsageMaximum,
};

//..............................................................................

enum HidRdTag {
	// main

	HidRdTag_Input             = 0x80, // 1000 00 nn
	HidRdTag_Output            = 0x90, // 1001 00 nn
	HidRdTag_Feature           = 0xB0, // 1011 00 nn
	HidRdTag_Collection        = 0xA0, // 1010 00 nn
	HidRdTag_CollectionEnd     = 0xC0, // 1100 00 nn

	// global

	HidRdTag_UsagePage         = 0x04, // 0000 01 nn
	HidRdTag_LogicalMinimum    = 0x14, // 0001 01 nn
	HidRdTag_LogicalMaximum    = 0x24, // 0010 01 nn
	HidRdTag_PhysicalMinimum   = 0x34, // 0011 01 nn
	HidRdTag_PhysicalMaximum   = 0x44, // 0100 01 nn
	HidRdTag_UnitExponent      = 0x54, // 0101 01 nn
	HidRdTag_Unit              = 0x64, // 0110 01 nn
	HidRdTag_ReportSize        = 0x74, // 0111 01 nn
	HidRdTag_ReportId          = 0x84, // 1000 01 nn
	HidRdTag_ReportCount       = 0x94, // 1001 01 nn
	HidRdTag_Push              = 0xA4, // 1010 01 nn
	HidRdTag_Pop               = 0xB4, // 1011 01 nn

	// local

	HidRdTag_Usage             = 0x08, // 0000 10 nn
	HidRdTag_UsageMinimum      = 0x18, // 0001 10 nn
	HidRdTag_UsageMaximum      = 0x28, // 0010 10 nn
	HidRdTag_DesignatorIndex   = 0x38, // 0011 10 nn
	HidRdTag_DesignatorMinimum = 0x48, // 0100 10 nn
	HidRdTag_DesignatorMaximum = 0x58, // 0101 10 nn
	HidRdTag_String            = 0x78, // 0111 10 nn
	HidRdTag_StringMinimum     = 0x88, // 1000 10 nn
	HidRdTag_StringMaximum     = 0x98, // 1001 10 nn
	HidRdTag_Delimiter         = 0xA8, // 1010 10 nn
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

HidRdItemId
getHidRdTagItemId(HidRdTag tag);

sl::StringRef
getHidRdTagString(HidRdTag tag);

//..............................................................................

class HidRdItemTable {
protected:
	uint32_t m_table[HidRdItemId__Count];
	sl::Array<uint32_t> m_auxUsageTable;
	uint_t m_mask;

public:
	HidRdItemTable() {
		reset();
	}

	bool
	isSet(HidRdItemId id) const {
		return (m_mask & (1 << id)) != 0;
	}

	bool
	isSet(HidRdItemMask mask) const {
		return (m_mask & mask) != 0;
	}

	uint32_t
	operator [] (HidRdItemId id) const {
		ASSERT((size_t)id < countof(m_table));
		return m_table[id];
	}

	size_t
	getAuxUsageCount() const {
		return m_auxUsageTable.getCount();
	}

	uint32_t
	getUsage(size_t i) const;

	void
	set(
		HidRdItemId id,
		uint32_t data
	);

	void
	reset();

	void
	resetLocals();

	void
	resetUsages();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
uint32_t
HidRdItemTable::getUsage(size_t i) const {
	return
		!(m_mask & HidRdItemMask_Usage) ?
			m_table[HidRdItemId_UsageMinimum] + i :
		i > 0 && i <= m_auxUsageTable.getCount() ?
			m_auxUsageTable[i - 1] :
			m_table[HidRdItemId_Usage];
}

inline
void
HidRdItemTable::reset() {
	memset(m_table, 0, sizeof(m_table));
	m_auxUsageTable.clear();
	m_mask = 0;
}

inline
void
HidRdItemTable::resetLocals() {
	memset(&m_table[HidRdItemId_Usage], 0, sizeof(uint32_t) * (HidRdItemId__Count - HidRdItemId_Usage));
	m_auxUsageTable.clear();
	m_mask &= ~HidRdItemMask_AllLocals;
}

inline
void
HidRdItemTable::resetUsages() {
	memset(&m_table[HidRdItemId_Usage], 0, sizeof(uint32_t) * (HidRdItemId_UsageMaximum + 1 - HidRdItemId_Usage));
	m_auxUsageTable.clear();
	m_mask &= ~HidRdItemMask_AllUsages;
}

//..............................................................................

enum HidRdValueFlag {
	HidRdValueFlag_Constant      = 0x0001, // otherwise, variable data
	HidRdValueFlag_Variable      = 0x0002, // otherwise, array
	HidRdValueFlag_Relative      = 0x0004, // otherwise, absolute
	HidRdValueFlag_Wrap          = 0x0008, // otherwise, no wrap
	HidRdValueFlag_Nonlinear     = 0x0010, // otherwise, linear
	HidRdValueFlag_NoPreferred   = 0x0020, // otherwise, preferred state
	HidRdValueFlag_NullState     = 0x0040, // otherwise, no null
	HidRdValueFlag_Volatile      = 0x0080, // otherwise, non-volatile
	HidRdValueFlag_BufferedBytes = 0x0100, // otherwise, bit field
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::StringRef
getHidRdValueFlagsString(uint_t flags);

//..............................................................................

enum HidRdUnitSystem {
	HidRdUnitSystem_None             = 0x00,
	HidRdUnitSystem_SiLinear         = 0x01,
	HidRdUnitSystem_SiRotation       = 0x02,
	HidRdUnitSystem_EnglishLinear    = 0x03,
	HidRdUnitSystem_EnglishRotation  = 0x04,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::StringRef
getHidRdUnitSystemString(HidRdUnitSystem system);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum HidRdUnitNibbleRole {
	HidRdUnitNibbleRole_System,             // nibble 0
	HidRdUnitNibbleRole_Length,             // nibble 1
	HidRdUnitNibbleRole_Mass,               // nibble 2
	HidRdUnitNibbleRole_Time,               // nibble 3
	HidRdUnitNibbleRole_Temperature,        // nibble 4
	HidRdUnitNibbleRole_Current,            // nibble 5
	HidRdUnitNibbleRole_LuminousIntensity,  // nibble 6
	HidRdUnitNibbleRole__Count              // 7 total
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum HidRdUnit {
	HidRdUnit_None,

	// nibble 1: length

	HidRdUnit_Centimiter,
	HidRdUnit_Radian,
	HidRdUnit_Inch,
	HidRdUnit_Degree,

	// nibble 2: mass

	HidRdUnit_Gram,
	HidRdUnit_Slug,

	// nibble 3: time

	HidRdUnit_Second,

	// nibble 4: temperature

	HidRdUnit_Kelvin,
	HidRdUnit_Farenheit,

	// nibble 5: current

	HidRdUnit_Ampere,

	// nibble 6: luminous intensity

	HidRdUnit_Candela,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

HidRdUnit
getHidRdUnit(
	HidRdUnitNibbleRole role,
	HidRdUnitSystem system
);

sl::StringRef
getHidRdUnitNameString(HidRdUnit unit);

sl::StringRef
getHidRdUnitAbbrString(HidRdUnit unit);

inline
int
getHidRdUnitExponent(uint_t nibble) {
	return (((nibble & 0x8) ? -1 : 0) & ~0xf) | (nibble & 0xf);
}

sl::StringRef
getHidRdComplexUnitString(uint32_t unit);

//..............................................................................

class HidReportField:
	public sl::ListLink,
	public HidRdItemTable {
	friend class HidRdParser;

protected:
	const HidReport* m_report;
	const HidUsagePage* m_usagePage;
	size_t m_bitOffset;
	size_t m_bitCount;
	uint_t m_valueFlags;

public:
	HidReportField();

	bool
	isPadding() const {
		return !isSet(HidRdItemMask_AllUsages);
	}

	const HidReport*
	getReport() const {
		return m_report;
	}

	const HidUsagePage*
	getUsagePage() const {
		return m_usagePage;
	}

	size_t
	getBitOffset() const {
		return m_bitOffset;
	}

	size_t
	getBitCount() const {
		return m_bitCount;
	}

	uint_t
	getValueFlags() const {
		return m_valueFlags;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
HidReportField::HidReportField() {
	m_report = NULL;
	m_usagePage = NULL;
	m_bitOffset = 0;
	m_bitCount = 0;
	m_valueFlags = 0;
}

//..............................................................................

enum HidReportKind {
	HidReportKind_Invalid = -1,
	HidReportKind_Input,
	HidReportKind_Output,
	HidReportKind_Feature,
	HidReportKind__Count,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::StringRef
getHidReportKindString(HidReportKind reportKind);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HidReport {
	friend class HidRd;
	friend class HidRdParser;

protected:
	HidReportKind m_reportKind;
	uint_t m_reportId;
	sl::Array<const HidReportField*> m_fieldArray;
	size_t m_bitCount; // size of all fields in bits
	size_t m_size;     // size of all fields in bytes

public:
	HidReport();

	HidReportKind
	getReportKind() const {
		return m_reportKind;
	}

	uint_t
	getReportId() const {
		return m_reportId;
	}

	const sl::Array<const HidReportField*>&
	getFieldArray() const {
		return m_fieldArray;
	}

	size_t
	getBitCount() const {
		return m_bitCount;
	}

	size_t
	getSize() const {
		return m_size;
	}

	size_t
	updateSize() {
		return m_size = (m_bitCount + 7) / 8;
	}

	void
	decode(const void* p) const; // at least m_size bytes expected
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
HidReport::HidReport() {
	m_reportKind = HidReportKind_Invalid;
	m_reportId = 0;
	m_bitCount = 0;
	m_size = 0;
}

//..............................................................................

enum HidRdCollectionKind {
	HidRdCollectionKind_Invalid       = -1,
	HidRdCollectionKind_Physical      = 0x00,
	HidRdCollectionKind_Application   = 0x01,
	HidRdCollectionKind_Logical       = 0x02,
	HidRdCollectionKind_Report        = 0x03,
	HidRdCollectionKind_NamedArray    = 0x04,
	HidRdCollectionKind_UsageSwitch   = 0x05,
	HidRdCollectionKind_UsageModifier = 0x06,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::StringRef
getHidRdCollectionKindString(HidRdCollectionKind kind);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HidRdCollection: public sl::ListLink {
	friend class HidRd;
	friend class HidRdParser;

protected:
	HidRdCollectionKind m_collectionKind;
	const HidUsagePage* m_usagePage;
	uint_t m_usage;
	sl::List<HidRdCollection> m_collectionList;
	sl::Array<const HidReportField*> m_fieldArray;

public:
	HidRdCollection();

	HidRdCollectionKind
	getCollectionKind() const {
		return m_collectionKind;
	}

	const HidUsagePage*
	getUsagePage() const {
		return m_usagePage;
	}

	uint_t
	getUsage() const {
		return m_usage;
	}

	const sl::List<HidRdCollection>&
	getCollectionList() const {
		return m_collectionList;
	}

	const sl::Array<const HidReportField*>&
	getFieldArray() const {
		return m_fieldArray;
	}

	void
	clear();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
HidRdCollection::HidRdCollection() {
	m_collectionKind = HidRdCollectionKind_Invalid;
	m_usagePage = NULL;
	m_usage = 0;
}

inline
void
HidRdCollection::clear() {
	m_collectionKind = HidRdCollectionKind_Invalid;
	m_usagePage = NULL;
	m_usage = 0;
	m_collectionList.clear();
	m_fieldArray.clear();
}

//..............................................................................

enum HidRdFlag {
	HidRdFlag_HasReportId = 0x01,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HidRd {
	friend class HidRdParser;

protected:
	typedef sl::SimpleHashTable<uint_t, HidReport> ReportMap;

protected:
	HidRdCollection m_rootCollection;
	sl::List<HidReportField> m_fieldList;
	ReportMap m_reportMapTable[HidReportKind__Count];
	uint_t m_flags;

public:
	HidRd() {
		m_flags = 0;
	}

	uint_t
	getFlags() const {
		return m_flags;
	}

	const HidRdCollection*
	getRootCollection() const {
		return &m_rootCollection;
	}

	const HidReport*
	findReport(
		HidReportKind reportKind,
		uint_t reportId
	) const;

	void
	clear();

	void
	parse(
		HidUsageDb* db,
		const void* p,
		size_t size
	); // never fails

	void
	printReports() const;

	void
	printCollections() const;

protected:
	HidReport*
	getReport(
		HidReportKind reportKind,
		uint_t reportId
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const HidReport*
HidRd::findReport(
	HidReportKind reportKind,
	uint_t reportId
) const {
	ASSERT((size_t)reportKind < countof(m_reportMapTable));
	sl::ConstMapIterator<uint_t, HidReport> it = m_reportMapTable[reportKind].find(reportId);
	return it ? &it->m_value : NULL;
}

//..............................................................................

} // namespace io
} // namespace axl
