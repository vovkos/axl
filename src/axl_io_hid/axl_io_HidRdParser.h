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

struct HidRdParser {
	friend class InitParseItemFuncTable;

protected:
	typedef
	void
	(HidRdParser::*ProcessItemFunc)(
		HidRdTag tag,
		uint32_t data
	);

	class InitProcessItemFuncTable {
	public:
		void
		operator () (ProcessItemFunc funcTable[]) const;
	};

protected:
	HidDb* m_db;
	HidRd* m_rd;
	HidReport* m_report;
	sl::Array<HidRdCollection*> m_collectionStack;
	mutable const HidUsagePage* m_usagePage;
	HidRdItemTable m_itemTable;
	sl::Array<HidRdItemTable> m_stack;

public:
	HidRdParser(
		HidDb* db,
		HidRd* rd
	);

	void
	parse(
		const void* p,
		size_t size
	);

protected:
	HidRdCollection*
	getCollection() {
		return !m_collectionStack.isEmpty() ? m_collectionStack.getBack() : &m_rd->m_rootCollection;
	}

	static
	ProcessItemFunc
	getProcessItemFunc(HidRdTag tag);

	void
	processItem_collection(
		HidRdTag tag,
		uint32_t data
	);

	void
	processItem_collectionEnd(
		HidRdTag tag,
		uint32_t data
	) {
		m_collectionStack.pop();
	}

	void
	processItem_input(
		HidRdTag tag,
		uint32_t data
	) {
		finalizeReportField(HidReportKind_Input, data);
	}

	void
	processItem_output(
		HidRdTag tag,
		uint32_t data
	) {
		finalizeReportField(HidReportKind_Output, data);
	}

	void
	processItem_feature(
		HidRdTag tag,
		uint32_t data
	) {
		finalizeReportField(HidReportKind_Feature, data);
	}

	void
	processItem_usagePage(
		HidRdTag tag,
		uint32_t data
	) {
		m_usagePage = m_db->getUsagePage(data);
		m_itemTable.resetUsages();
	}

	void
	processItem_push(
		HidRdTag tag,
		uint32_t data
	) {
		m_stack.append(m_itemTable);
	}

	void
	processItem_pop(
		HidRdTag tag,
		uint32_t data
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
	HidDb* db,
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
HidRdParser::processItem_pop(
	HidRdTag tag,
	uint32_t data
) {
	if (!m_stack.isEmpty()) {
		m_itemTable = m_stack.getBackAndPop();
		m_usagePage = NULL;
	}
}

//..............................................................................

} // namespace io
} // namespace axl
