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

#define _AXL_IO_HIDRDPARSER_H

#include "axl_io_HidRd.h"
#include "axl_io_HidUsageDb.h"

namespace axl {
namespace io {

//..............................................................................

struct HidRdParserGlobalState {
	const HidUsagePage* m_usagePage;
	uint_t m_usagePageId;
	int m_logicalMinimum;
	int m_logicalMaximum;
	int m_physicalMinimum;
	int m_physicalMaximum;
	int m_unitExponent;
	uint_t m_unit;
	uint_t m_reportSize;
	uint_t m_reportId;
	uint_t m_reportCount;

	HidRdParserGlobalState() {
		memset(this, 0, sizeof(HidRdParserGlobalState));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct HidRdParserLocalState {
	uint_t m_usage;
	uint_t m_usageMinimum;
	uint_t m_usageMaximum;
	int m_designatorIndex;
	int m_designatorMinimum;
	int m_designatorMaximum;
	int m_string;
	int m_stringMinimum;
	int m_stringMaximum;
	int m_delimiter;

	HidRdParserLocalState() {
		memset(this, 0, sizeof(HidRdParserLocalState));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct HidRdItemState:
	HidRdParserGlobalState,
	HidRdParserLocalState {
};

//..............................................................................

struct HidRdParser: HidRdItemState {
	friend class InitParseItemFuncTable;

protected:
	typedef
	void
	(HidRdParser::*ParseItemFunc)(
		HidRdItemTag tag,
		uint32_t data,
		size_t size
	);

	class InitParseItemFuncTable {
	public:
		void
		operator () (ParseItemFunc funcTable[]) const;
	};

protected:
	HidUsageDb* m_db;
	sl::Array<HidRdItemState> m_stack;
	sl::String m_indent;

public:
	HidRdParser(HidUsageDb* db) {
		ASSERT(db);
		m_db = db;
	}

	bool
	parse(
		const void* p,
		size_t size
	);

protected:
	static
	ParseItemFunc
	getParseItemFunc(HidRdItemTag tag);

	void
	parseItem_default(
		HidRdItemTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_collection(
		HidRdItemTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_collectionEnd(
		HidRdItemTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_value(
		HidRdItemTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_usagePage(
		HidRdItemTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_usage(
		HidRdItemTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_push(
		HidRdItemTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_pop(
		HidRdItemTag tag,
		uint32_t data,
		size_t size
	);
};

//..............................................................................

} // namespace io
} // namespace axl
