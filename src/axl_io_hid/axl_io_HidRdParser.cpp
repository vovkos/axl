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
#include "axl_io_HidRdParser.h"
#include "axl_sl_CallOnce.h"

namespace axl {
namespace io {

//..............................................................................

template <typename Parser>
void
parseHidRd(
	Parser* parser,
	const void* p0,
	size_t size
) {
	static const size_t sizeTable[] = { 0, 1, 2, 4 };

	const uchar_t* p = (uchar_t*)p0;
	const uchar_t* end = p + size;
	while (p < end) {
		uchar_t a = *p++;
		HidRdTag tag = (HidRdTag)(a & ~0x03);
		size_t size = sizeTable[a & 0x03];
		HidRdItemId id = getHidRdTagItemId(tag);
		uint32_t data = isHidRdItemSigned(id) && (p[size - 1] & 0x80) ? -1 : 0;
		memcpy(&data, p, size);

		bool result = parser->processTag(tag, id, data);
		if (!result)
			break;

		p += size;
	}
}

//..............................................................................

struct FirstUsageFinder {
	uint_t m_usagePage;
	uint_t m_usage;
	uint_t m_mask;

	FirstUsageFinder();

	bool
	isFound() const {
		return
			(m_mask & (HidRdItemMask_UsagePage | HidRdItemMask_Usage)) ==
			(HidRdItemMask_UsagePage | HidRdItemMask_Usage);
	}

	bool
	processTag(
		HidRdTag tag,
		HidRdItemId id,
		uint32_t data
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
FirstUsageFinder::FirstUsageFinder() {
	m_usagePage = 0;
	m_usage = 0;
	m_mask = 0;
}

inline
bool
FirstUsageFinder::processTag(
	HidRdTag tag,
	HidRdItemId id,
	uint32_t data
) {
	switch (tag) {
	case HidRdTag_UsagePage:
		m_usagePage = data;
		m_mask |= HidRdItemMask_UsagePage;
		break;

	case HidRdTag_Usage:
		if (data & 0xffff0000) {
			m_usagePage = data >> 16;
			m_usage = data & 0xffff;
			m_mask |= HidRdItemMask_UsagePage | HidRdItemMask_Usage;
			return false;
		}

		m_usage = data;
		m_mask |= HidRdItemMask_Usage;
		break;
	}

	return !isFound();
}

//..............................................................................

void
HidRdParser::InitProcessTagFuncTable::operator () (ProcessTagFunc funcTable[]) const {
	// main

	funcTable[HidRdTag_Input >> 2]         = &HidRdParser::processTag_input;
	funcTable[HidRdTag_Output >> 2]        = &HidRdParser::processTag_output;
	funcTable[HidRdTag_Feature >> 2]       = &HidRdParser::processTag_feature;
	funcTable[HidRdTag_Collection >> 2]    = &HidRdParser::processTag_collection;
	funcTable[HidRdTag_CollectionEnd >> 2] = &HidRdParser::processTag_collectionEnd;

	// global

	funcTable[HidRdTag_UsagePage >> 2] = &HidRdParser::processTag_usagePage;
	funcTable[HidRdTag_Push >> 2]      = &HidRdParser::processTag_push;
	funcTable[HidRdTag_Pop >> 2]       = &HidRdParser::processTag_pop;

	// local

	funcTable[HidRdTag_Usage >> 2] = &HidRdParser::processTag_usage;
}

HidRdParser::ProcessTagFunc
HidRdParser::getProcessTagFunc(HidRdTag tag) {
	static ProcessTagFunc funcTable[256 >> 2] = { 0 };
	sl::callOnce(InitProcessTagFuncTable(), funcTable);
	return (size_t)tag < 256 ?
		funcTable[(uint8_t)tag >> 2] :
		NULL;
}

void
HidRdParser::parse(
	const void* p,
	size_t size
) {
	parseHidRd(this, p, size);
}

bool
HidRdParser::findFirstUsage(
	uint_t* usagePage,
	uint_t* usage,
	const void* p,
	size_t size
) {
	FirstUsageFinder finder;
	parseHidRd(&finder, p, size);
	*usagePage = finder.m_usagePage;
	*usage = finder.m_usage;
	return finder.isFound();
}

bool
HidRdParser::processTag(
	HidRdTag tag,
	HidRdItemId id,
	uint32_t data
) {
	if (id != HidRdItemId_Invalid)
		m_itemTable.set(id, data);

	ProcessTagFunc func = getProcessTagFunc(tag);
	if (func)
		(this->*func)(data);

	return true;
}

void
HidRdParser::processTag_collection(uint32_t data) {
	HidRdCollection* collection = new HidRdCollection;
	collection->m_collectionKind = (HidRdCollectionKind)data;
	collection->m_usagePage = getUsagePage();
	collection->m_usage = m_itemTable[HidRdItemId_Usage];
	getCollection()->m_collectionList.insertTail(collection);
	m_collectionStack.append(collection);
}

void
HidRdParser::processTag_usage(uint32_t data) {
	if (data & 0xffff0000) { // contains usage page
		processTag_usagePage(data >> 16);
		m_itemTable.set(HidRdItemId_Usage, (data & 0xffff));
	}
}

void
HidRdParser::finalizeReportField(
	HidReportType reportType,
	uint_t valueFlags
) {
	if (m_itemTable.isSet(HidRdItemId_ReportId))
		m_rd->m_flags |= HidRdFlag_HasReportId;

	if (!m_itemTable.isSet(HidRdItemId_ReportSize))
		m_itemTable.set(HidRdItemId_ReportSize, 1);

	if (!m_itemTable.isSet(HidRdItemId_ReportCount))
		m_itemTable.set(HidRdItemId_ReportCount, 1);

	if (!m_report ||
		m_report->m_reportType != reportType ||
		m_report->m_reportId != m_itemTable[HidRdItemId_ReportId]
	)
		m_report = m_rd->getReport(reportType, m_itemTable[HidRdItemId_ReportId]);

	size_t bitCount = m_itemTable[HidRdItemId_ReportSize] * m_itemTable[HidRdItemId_ReportCount];

	HidReportField* field = new HidReportField;
	*(HidRdItemTable*)field = m_itemTable;
	field->m_report = m_report;
	field->m_usagePage = getUsagePage();
	field->m_valueFlags = valueFlags;
	field->m_bitCount = bitCount;
	m_rd->m_fieldList.insertTail(field);

	getCollection()->m_fieldArray.append(field);
	m_report->m_fieldArray.append(field);
	m_report->m_bitCount += bitCount;

	m_itemTable.resetLocals();
}

//..............................................................................

} // namespace io
} // namespace axl
