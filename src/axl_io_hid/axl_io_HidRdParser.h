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
#include "axl_io_HidDb.h"

namespace axl {
namespace io {

//..............................................................................

class HidRdParser {
	friend class InitParseItemFuncTable;

	template <typename Parser>
	friend
	void
	parseHidRd(
		Parser* parser,
		const void* p0,
		size_t size
	);

 protected:
	typedef
	void
	(HidRdParser::*ProcessTagFunc)(uint32_t data);

	class InitProcessTagFuncTable {
	public:
		void
		operator () (ProcessTagFunc funcTable[]) const;
	};

protected:
	const HidDb* m_db;
	HidRd* m_rd;
	HidReport* m_report;
	sl::Array<HidRdCollection*> m_collectionStack;
	mutable const HidUsagePage* m_usagePage;
	HidRdItemTable m_itemTable;
	sl::Array<HidRdItemTable> m_stack;

public:
	HidRdParser(
		const HidDb* db,
		HidRd* rd
	);

	void
	parse(
		const void* p,
		size_t size
	);

	static
	bool
	findFirstUsage(
		uint_t* usagePage,
		uint_t* usage,
		const void* p,
		size_t size
	);

protected:
	HidRdCollection*
	getCollection() {
		return !m_collectionStack.isEmpty() ? m_collectionStack.getBack() : &m_rd->m_rootCollection;
	}

	static
	ProcessTagFunc
	getProcessTagFunc(HidRdTag tag);

	bool
	processTag(
		HidRdTag tag,
		HidRdItemId id,
		uint32_t data
	);

	void
	processTag_collection(uint32_t data);

	void
	processTag_collectionEnd(uint32_t data) {
		m_collectionStack.pop();
	}

	void
	processTag_input(uint32_t data) {
		finalizeReportField(HidReportType_Input, data);
	}

	void
	processTag_output(uint32_t data) {
		finalizeReportField(HidReportType_Output, data);
	}

	void
	processTag_feature(uint32_t data) {
		finalizeReportField(HidReportType_Feature, data);
	}

	void
	processTag_usagePage(uint32_t data) {
		m_usagePage = m_db->getUsagePage(data);
		m_itemTable.resetUsages();
	}

	void
	processTag_usage(uint32_t data);

	void
	processTag_push(uint32_t data) {
		m_stack.append(m_itemTable);
	}

	void
	processTag_pop(uint32_t data);

	void
	finalizeReportField(
		HidReportType reportType,
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
	const HidDb* db,
	HidRd* rd
) {
	ASSERT(db && rd);
	m_db = db;
	m_rd = rd;
	m_report = NULL;
	m_usagePage = NULL;
}

inline
void
HidRdParser::processTag_pop(uint32_t data) {
	if (!m_stack.isEmpty()) {
		m_itemTable = m_stack.getBackAndPop();
		m_usagePage = NULL;
	}
}

//..............................................................................

} // namespace io
} // namespace axl
