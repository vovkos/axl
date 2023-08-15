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

struct HidRdParser {
	friend class InitParseItemFuncTable;

protected:
	typedef
	void
	(HidRdParser::*ParseItemFunc)(
		HidRdTag tag,
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
	HidRd* m_rd;
	HidReport* m_report;
	mutable const HidUsagePage* m_usagePage;
	HidRdItemTable m_itemTable;
	sl::Array<HidRdItemTable> m_stack;
	sl::String m_indent;

public:
	HidRdParser(
		HidUsageDb* db,
		HidRd* rd
	);

	bool
	parse(
		const void* p,
		size_t size
	);

protected:
	static
	ParseItemFunc
	getParseItemFunc(HidRdTag tag);

	void
	parseItem_default(
		HidRdTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_collection(
		HidRdTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_collectionEnd(
		HidRdTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_input(
		HidRdTag tag,
		uint32_t data,
		size_t size
	) {
		finalizeReportField(HidReportKind_Input, data);
	}

	void
	parseItem_output(
		HidRdTag tag,
		uint32_t data,
		size_t size
	) {
		finalizeReportField(HidReportKind_Output, data);
	}

	void
	parseItem_feature(
		HidRdTag tag,
		uint32_t data,
		size_t size
	) {
		finalizeReportField(HidReportKind_Feature, data);
	}

	void
	parseItem_usagePage(
		HidRdTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_usage(
		HidRdTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_push(
		HidRdTag tag,
		uint32_t data,
		size_t size
	);

	void
	parseItem_pop(
		HidRdTag tag,
		uint32_t data,
		size_t size
	);

	void
	finalizeReportField(
		HidReportKind reportKind,
		uint_t valueFlags
	);

	const HidUsagePage*
	getUsagePage() const {
		return m_usagePage ? m_usagePage : (m_usagePage = m_db->getUsagePage(m_itemTable[HidRdItemId_UsagePage]));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
HidRdParser::HidRdParser(
	HidUsageDb* db,
	HidRd* rd
) {
	ASSERT(db);
	m_db = db;
	m_rd = rd;
	m_report = NULL;
	m_usagePage = NULL;
}

//..............................................................................

} // namespace io
} // namespace axl
