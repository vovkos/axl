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
		m_usagePage = NULL;
		m_usagePageId = 0;
		m_logicalMinimum = 0;
		m_logicalMaximum = 0;
		m_physicalMinimum = 0;
		m_physicalMaximum = 0;
		m_unitExponent = 0;
		m_unit = 0;
		m_reportSize = 0;
		m_reportId = 0;
		m_reportCount = 0;
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
		m_usage = 0;
		m_usageMinimum = 0;
		m_usageMaximum = 0;
		m_designatorIndex = 0;
		m_designatorMinimum = 0;
		m_designatorMaximum = 0;
		m_string = 0;
		m_stringMinimum = 0;
		m_stringMaximum = 0;
		m_delimiter = 0;
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
		int data,
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
		int data,
		size_t size
	);

	void
	parseItem_collection(
		HidRdItemTag tag,
		int data,
		size_t size
	);

	void
	parseItem_collectionEnd(
		HidRdItemTag tag,
		int data,
		size_t size
	);

	void
	parseItem_value(
		HidRdItemTag tag,
		int data,
		size_t size
	);

	void
	parseItem_usagePage(
		HidRdItemTag tag,
		int data,
		size_t size
	);

	void
	parseItem_usage(
		HidRdItemTag tag,
		int data,
		size_t size
	);

	void
	parseItem_push(
		HidRdItemTag tag,
		int data,
		size_t size
	);

	void
	parseItem_pop(
		HidRdItemTag tag,
		int data,
		size_t size
	);
};

//..............................................................................

} // namespace io
} // namespace axl
