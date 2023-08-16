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
HidRdParser::InitParseItemFuncTable::operator () (ParseItemFunc funcTable[]) const {
	for (size_t i = 0; i < 256 >> 2; i++)
		funcTable[i] = &HidRdParser::parseItem_default;

	// main

	funcTable[HidRdTag_Input >> 2]         = &HidRdParser::parseItem_input;
	funcTable[HidRdTag_Output >> 2]        = &HidRdParser::parseItem_output;
	funcTable[HidRdTag_Feature >> 2]       = &HidRdParser::parseItem_feature;
	funcTable[HidRdTag_Collection >> 2]    = &HidRdParser::parseItem_collection;
	funcTable[HidRdTag_CollectionEnd >> 2] = &HidRdParser::parseItem_collectionEnd;

	// global

	funcTable[HidRdTag_UsagePage >> 2] = &HidRdParser::parseItem_usagePage;
	funcTable[HidRdTag_Push >> 2]      = &HidRdParser::parseItem_push;
	funcTable[HidRdTag_Pop >> 2]       = &HidRdParser::parseItem_pop;

	// local

	funcTable[HidRdTag_Usage >> 2]        = &HidRdParser::parseItem_usage;
	funcTable[HidRdTag_UsageMinimum >> 2] = &HidRdParser::parseItem_usage;
	funcTable[HidRdTag_UsageMaximum >> 2] = &HidRdParser::parseItem_usage;
}

HidRdParser::ParseItemFunc
HidRdParser::getParseItemFunc(HidRdTag tag) {
	static ParseItemFunc funcTable[256 >> 2] = { 0 };
	sl::callOnce(InitParseItemFuncTable(), funcTable);
	return (size_t)tag < 256 ?
		funcTable[(uint8_t)tag >> 2] :
		&HidRdParser::parseItem_default;
}

bool
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
		uint32_t data = 0;
		memcpy(&data, p, size);

		HidRdItemId id = getHidRdTagItemId(tag);
		if (id != HidRdItemId_Invalid)
			m_itemTable.set(id, data);

		ParseItemFunc func = getParseItemFunc(tag);
		(this->*func)(tag, data, size);
		p += size;
	}

	m_rd->finalize();
	return true;
}

void
HidRdParser::parseItem_default(
	HidRdTag tag,
	uint32_t data,
	size_t size
) {
	sl::StringRef tagString = getHidRdTagString(tag);

	if (!size) {
		printf("%s%s\n", m_indent.sz(), tagString.sz());
		return;
	}

	int value = size < sizeof(int) && (data & (0x80 << (size - 1) * 8)) ? -1 : 0;
	memcpy(&value, &data, size);
	printf("%s%s: %d\n", m_indent.sz(), tagString.sz(), value);
}

void
HidRdParser::parseItem_collection(
	HidRdTag tag,
	uint32_t data,
	size_t size
) {
	sl::StringRef collectionString = getHidRdCollectionKindString((HidRdCollectionKind)data);
	printf("%sCollection: %s\n", m_indent.sz(), collectionString.sz());
	m_indent += "    ";
}

void
HidRdParser::parseItem_collectionEnd(
	HidRdTag tag,
	uint32_t data,
	size_t size
) {
	size_t length = m_indent.getLength();
	if (length >= 4)
		m_indent.overrideLength(length - 4);

	printf("%sCollectionEnd\n", m_indent.sz());
}

void
HidRdParser::parseItem_usagePage(
	HidRdTag tag,
	uint32_t data,
	size_t size
) {
	m_usagePage = m_db->getUsagePage(data);
	m_itemTable.resetUsages();
	printf("%sUsagePage: %s\n", m_indent.sz(), m_usagePage->getName().sz());
}

void
HidRdParser::parseItem_usage(
	HidRdTag tag,
	uint32_t data,
	size_t size
) {
	sl::StringRef tagString = getHidRdTagString(tag);
	sl::StringRef usageString = getUsagePage()->getUsageName(data);
	printf("%s%s: %s\n", m_indent.sz(), tagString.sz(), usageString.sz());
}

void
HidRdParser::parseItem_push(
	HidRdTag tag,
	uint32_t data,
	size_t size
) {
	m_stack.append(m_itemTable);
}

void
HidRdParser::parseItem_pop(
	HidRdTag tag,
	uint32_t data,
	size_t size
) {
	if (!m_stack.isEmpty()) {
		m_itemTable = m_stack.getBackAndPop();
		m_usagePage = NULL;
	}
}

void
HidRdParser::finalizeReportField(
	HidReportKind reportKind,
	uint_t valueFlags
) {
	sl::StringRef reportKindString = getHidReportKindString(reportKind);
	sl::String valueFlagsString = getHidRdValueFlagsString(valueFlags);
	printf("%s%s: %s\n", m_indent.sz(), reportKindString.sz(), valueFlagsString.sz());

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
	field->m_usagePage = getUsagePage();
	field->m_valueFlags = valueFlags;
	field->m_bitOffset = m_report->m_bitCount;
	field->m_bitCount = m_itemTable[HidRdItemId_ReportSize] * m_itemTable[HidRdItemId_ReportCount];

	m_report->m_fieldList.insertTail(field);
	m_report->m_bitCount += field->getBitCount();
	m_itemTable.resetLocals();
}

//..............................................................................

} // namespace io
} // namespace axl
