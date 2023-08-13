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

	funcTable[HidRdItemTag_Input >> 2]         = &HidRdParser::parseItem_value;
	funcTable[HidRdItemTag_Output >> 2]        = &HidRdParser::parseItem_value;
	funcTable[HidRdItemTag_Feature >> 2]       = &HidRdParser::parseItem_value;
	funcTable[HidRdItemTag_Collection >> 2]    = &HidRdParser::parseItem_collection;
	funcTable[HidRdItemTag_CollectionEnd >> 2] = &HidRdParser::parseItem_collectionEnd;

	// global

	funcTable[HidRdItemTag_UsagePage >> 2]       = &HidRdParser::parseItem_usagePage;
	funcTable[HidRdItemTag_Push >> 2]            = &HidRdParser::parseItem_push;
	funcTable[HidRdItemTag_Pop >> 2]             = &HidRdParser::parseItem_pop;

	// local

	funcTable[HidRdItemTag_Usage >> 2]             = &HidRdParser::parseItem_usage;
	funcTable[HidRdItemTag_UsageMinimum >> 2]      = &HidRdParser::parseItem_usage;
	funcTable[HidRdItemTag_UsageMaximum >> 2]      = &HidRdParser::parseItem_usage;
}

HidRdParser::ParseItemFunc
HidRdParser::getParseItemFunc(HidRdItemTag tag) {
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
		HidRdItemTag tag = (HidRdItemTag)(a & ~0x03);
		size_t size = sizeTable[a & 0x03];
		uint32_t data = 0;
		memcpy(&data, p, size);

		ParseItemFunc func = getParseItemFunc(tag);
		(this->*func)(tag, data, size);
		p += size;
	}

	return true;
}

void
HidRdParser::parseItem_default(
	HidRdItemTag tag,
	uint32_t data,
	size_t size
) {
	sl::StringRef tagString = getHidRdItemTagString(tag);

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
	HidRdItemTag tag,
	uint32_t data,
	size_t size
) {
	sl::StringRef collectionString = getHidRdCollectionKindString((HidRdCollectionKind)data);
	printf("%sCollection: %s\n", m_indent.sz(), collectionString.sz());
	m_indent += "    ";
}

void
HidRdParser::parseItem_collectionEnd(
	HidRdItemTag tag,
	uint32_t data,
	size_t size
) {
	size_t length = m_indent.getLength();
	if (length >= 4)
		m_indent.overrideLength(length - 4);

	printf("%sCollectionEnd\n", m_indent.sz());
}

void
HidRdParser::parseItem_value(
	HidRdItemTag tag,
	uint32_t data,
	size_t size
) {
	sl::StringRef tagString = getHidRdItemTagString(tag);
	sl::String valueFlagsString = getHidRdValueFlagsString(data);
	printf("%s%s: %s\n", m_indent.sz(), tagString.sz(), valueFlagsString.sz());
}

void
HidRdParser::parseItem_usagePage(
	HidRdItemTag tag,
	uint32_t data,
	size_t size
) {
	m_usagePageId = data;
	m_usagePage = m_db->getUsagePage(data);
	printf("%sUsagePage: %s\n", m_indent.sz(), m_usagePage->getName().sz());
}

void
HidRdParser::parseItem_usage(
	HidRdItemTag tag,
	uint32_t data,
	size_t size
) {
	if (!m_usagePage)
		m_usagePage = m_db->getUsagePage(m_usagePageId);

	sl::StringRef tagString = getHidRdItemTagString(tag);
	sl::StringRef usageString = m_usagePage->getUsageName(data);
	printf("%s%s: %s\n", m_indent.sz(), tagString.sz(), usageString.sz());
}

void
HidRdParser::parseItem_push(
	HidRdItemTag tag,
	uint32_t data,
	size_t size
) {
	m_stack.append(*this);
}

void
HidRdParser::parseItem_pop(
	HidRdItemTag tag,
	uint32_t data,
	size_t size
) {
	if (!m_stack.isEmpty())
		*(HidRdItemState*)this = m_stack.getBackAndPop();
}

//..............................................................................

} // namespace io
} // namespace axl
