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
#include "axl_io_HidUsage.h"
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
		int data = size && (p[size - 1] & 0x80) ? -1 : 0;
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
	int data,
	size_t size
) {
	const char* tagString = getHidRdItemTagString(tag);

	printf(
		size ? "%s%s (%d)\n" : "%s%s\n",
		m_indent.sz(),
		tagString,
		data
	);
}

void
HidRdParser::parseItem_collection(
	HidRdItemTag tag,
	int data,
	size_t size
) {
	printf("%sCollection (%s)\n", m_indent.sz(), getHidRdCollectionKindString((HidRdCollectionKind)data));
	m_indent += "    ";
}

void
HidRdParser::parseItem_collectionEnd(
	HidRdItemTag tag,
	int data,
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
	int data,
	size_t size
) {
	const char* tagString = getHidRdItemTagString(tag);
	sl::String valueFlagsString = getHidRdValueFlagsString(data);
	printf("%s%s (%s)\n", m_indent.sz(), tagString, valueFlagsString.sz());
}

void
HidRdParser::parseItem_usagePage(
	HidRdItemTag tag,
	int data,
	size_t size
) {
	const char* pageString = getHidUsagePageString((HidUsagePage)data);
	m_usagePage = (HidUsagePage)data;
	printf("%sUsagePage (%s)\n", m_indent.sz(), pageString);
}

void
HidRdParser::parseItem_usage(
	HidRdItemTag tag,
	int data,
	size_t size
) {
	const char* tagString = getHidRdItemTagString(tag);
	sl::String usageString = getHidUsageString(m_usagePage, data);
	printf("%s%s (%s)\n", m_indent.sz(), tagString, usageString.sz());
}

void
HidRdParser::parseItem_push(
	HidRdItemTag tag,
	int data,
	size_t size
) {
	m_stack.append(*this);
}

void
HidRdParser::parseItem_pop(
	HidRdItemTag tag,
	int data,
	size_t size
) {
	if (!m_stack.isEmpty())
		*(HidRdItemState*)this = m_stack.getBackAndPop();
}

//..............................................................................

} // namespace io
} // namespace axl
