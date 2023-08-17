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

void
HidRdParser::InitProcessItemFuncTable::operator () (ProcessItemFunc funcTable[]) const {
	// main

	funcTable[HidRdTag_Input >> 2]         = &HidRdParser::processItem_input;
	funcTable[HidRdTag_Output >> 2]        = &HidRdParser::processItem_output;
	funcTable[HidRdTag_Feature >> 2]       = &HidRdParser::processItem_feature;
	funcTable[HidRdTag_Collection >> 2]    = &HidRdParser::processItem_collection;
	funcTable[HidRdTag_CollectionEnd >> 2] = &HidRdParser::processItem_collectionEnd;

	// global

	funcTable[HidRdTag_UsagePage >> 2] = &HidRdParser::processItem_usagePage;
	funcTable[HidRdTag_Push >> 2]      = &HidRdParser::processItem_push;
	funcTable[HidRdTag_Pop >> 2]       = &HidRdParser::processItem_pop;
}

HidRdParser::ProcessItemFunc
HidRdParser::getProcessItemFunc(HidRdTag tag) {
	static ProcessItemFunc funcTable[256 >> 2] = { 0 };
	sl::callOnce(InitProcessItemFuncTable(), funcTable);
	return (size_t)tag < 256 ?
		funcTable[(uint8_t)tag >> 2] :
		NULL;
}

void
HidRdParser::parse(
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

		uint32_t data;

		HidRdItemId id = getHidRdTagItemId(tag);
		if (id == HidRdItemId_Invalid) {
			data = 0;
			memcpy(&data, p, size);
		} else {
			data = isHidRdItemSigned(id) && (p[size - 1] & 0x80) ? -1 : 0;
			memcpy(&data, p, size);
			m_itemTable.set(id, data);
		}

		ProcessItemFunc func = getProcessItemFunc(tag);
		if (func)
			(this->*func)(tag, data);

		p += size;
	}
}

void
HidRdParser::processItem_collection(
	HidRdTag tag,
	uint32_t data
) {
	HidRdCollection* collection = new HidRdCollection;
	collection->m_collectionKind = (HidRdCollectionKind)data;
	collection->m_usagePage = getUsagePage();
	collection->m_usage = m_itemTable[HidRdItemId_Usage];
	getCollection()->m_collectionList.insertTail(collection);
	m_collectionStack.append(collection);
}

void
HidRdParser::finalizeReportField(
	HidReportKind reportKind,
	uint_t valueFlags
) {
	if (m_itemTable.isSet(HidRdItemId_ReportId))
		m_rd->m_flags |= HidRdFlag_HasReportId;

	if (!m_itemTable.isSet(HidRdItemId_ReportSize))
		m_itemTable.set(HidRdItemId_ReportSize, 1);

	if (!m_itemTable.isSet(HidRdItemId_ReportCount))
		m_itemTable.set(HidRdItemId_ReportCount, 1);

	if (!m_report ||
		m_report->m_reportKind != reportKind ||
		m_report->m_reportId != m_itemTable[HidRdItemId_ReportId]
	)
		m_report = m_rd->getReport(reportKind, m_itemTable[HidRdItemId_ReportId]);

	HidReportField* field = new HidReportField;
	*(HidRdItemTable*)field = m_itemTable;
	field->m_report = m_report;
	field->m_usagePage = getUsagePage();
	field->m_valueFlags = valueFlags;
	field->m_bitOffset = m_report->m_bitCount;
	field->m_bitCount = m_itemTable[HidRdItemId_ReportSize] * m_itemTable[HidRdItemId_ReportCount];
	m_rd->m_fieldList.insertTail(field);

	getCollection()->m_fieldArray.append(field);
	m_report->m_fieldArray.append(field);
	m_report->m_bitCount += field->getBitCount();

	m_itemTable.resetLocals();
}

//..............................................................................

} // namespace io
} // namespace axl
