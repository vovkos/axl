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
#include "axl_io_HidReportSerializer.h"
#include "axl_io_HidRd.h"
#include "axl_enc_Leb128.h"

namespace axl {
namespace io {

//..............................................................................

size_t
HidReportSerializer::saveReportDecodeInfo(
	sl::Array<char>* buffer,
	const HidReport& report
) {
	char block[128]; // more than enough
	char* p = block;
	char* threshold = p + sizeof(block) / 2;

	buffer->clear();
	buffer->setCount(sizeof(uint16_t)); // max size of HID RD is 4096

	p = enc::encodeUleb128(p, report.getReportType());
	p = enc::encodeUleb128(p, report.getReportId());

	const sl::Array<const HidReportField*> fieldArray = report.getFieldArray();
	size_t count = fieldArray.getCount();
	for (size_t i = 0; i < count; i++) {
		const HidReportField& field = *fieldArray[i];

		uint_t mask = field.getMask();
		p = enc::encodeUleb128(p, mask);
		p = enc::encodeUleb128(p, field.getValueFlags());
		p = enc::encodeUleb128(p, field[HidRdItemId_ReportSize]);
		p = enc::encodeUleb128(p, field[HidRdItemId_ReportCount]);

		if (p > threshold) {
			buffer->append(block, p - block);
			p =	block;
		}

		if (field.isPadding())
			continue;

		uint_t usagePage = field.getUsagePage()->getId();
		p = enc::encodeUleb128(p, usagePage);

		if (field.isSet(HidRdItemId_LogicalMinimum))
		 	p = enc::encodeUleb128(p, field[HidRdItemId_LogicalMinimum]);

		if (field.isSet(HidRdItemId_LogicalMaximum))
		 	p = enc::encodeUleb128(p, field[HidRdItemId_LogicalMaximum]);

		if (field.isSet(HidRdItemId_UnitExponent))
		 	p = enc::encodeUleb128(p, field[HidRdItemId_UnitExponent]);

		if (field.isSet(HidRdItemId_Unit))
		 	p = enc::encodeUleb128(p, field[HidRdItemId_Unit]);

		if (!field.isSet(HidRdItemId_Usage)) {
			p = enc::encodeUleb128(p, field[HidRdItemId_UsageMinimum]);
			p = enc::encodeUleb128(p, field[HidRdItemId_UsageMaximum]);
		} else {
			p = enc::encodeUleb128(p, field[HidRdItemId_Usage]);

			size_t auxUsageCount = field.m_auxUsageTable.getCount();
			p = enc::encodeUleb128(p, auxUsageCount);
			for (size_t i = 0; i < auxUsageCount; i++) {
				p = enc::encodeUleb128(p, field.m_auxUsageTable[i]);
				if (p > threshold) {
					buffer->append(block, p - block);
					p =	block;
				}
			}
		}
	}

	size_t size = buffer->append(block, p - block);
	if (size > 0xffff) // can't fit, something wrong with HID RD
		return err::fail<size_t>(-1, "invalid HID report descriptor size");

	*(uint16_t*)buffer->p() = (uint16_t)size;
	return size;
}

size_t
HidReportSerializer::loadReportDecodeInfo(
	HidStandaloneReport* report,
	const HidDb* db,
	const void* p0,
	size_t size
) {
	report->m_fieldArray.clear();
	report->m_fieldList.clear();
	report->m_bitCount = 0;

	if (size < sizeof(uint16_t))
		return err::fail<size_t>(-1, "invalid HID report decode info size");

	size_t decodeInfoSize = *(const uint16_t*)p0;
	if (decodeInfoSize > size)
		return err::fail<size_t>(-1, "invalid HID report decode info");

	const char* p = (char*)p0 + sizeof(uint16_t);
	const char* end = (char*)p0 + decodeInfoSize;

	p = enc::decodeUleb128(&report->m_reportType, p, end);
	p = enc::decodeUleb128(&report->m_reportId, p, end);

	while (p < end) {
		uint_t mask;
		uint_t valueFlags;
		size_t reportSize;
		size_t reportCount;

		p = enc::decodeUleb128(&mask, p, end);
		p = enc::decodeUleb128(&valueFlags, p, end);
		p = enc::decodeUleb128(&reportSize, p, end);
		p = enc::decodeUleb128(&reportCount, p, end);

		if (!reportSize)
			reportSize = 1;

		if (!reportCount)
			reportCount = 1;

		HidReportField* field = new HidReportField;
		field->m_report = report;
		field->m_table[HidRdItemId_ReportSize] = reportSize;
		field->m_table[HidRdItemId_ReportCount] = reportCount;
		field->m_bitCount = reportSize * reportCount;
		field->m_valueFlags = valueFlags;
		field->m_mask = mask & (
			HidRdItemMask_ReportSize |
			HidRdItemMask_ReportCount |
			HidRdItemMask_UsagePage |
			HidRdItemMask_AllUsages |
			HidRdItemMask_LogicalMinimum |
			HidRdItemMask_LogicalMaximum |
			HidRdItemMask_UnitExponent |
			HidRdItemMask_Unit
		);

		report->m_bitCount += field->m_bitCount;
		report->m_fieldArray.append(field);
		report->m_fieldList.insertTail(field);

		if (field->isPadding())
			continue;

		uint_t usagePage;
		p = enc::decodeUleb128(&usagePage, p, end);
		field->m_usagePage = db->getUsagePage(usagePage);
		field->m_table[HidRdItemId_UsagePage] = usagePage;

		if (field->isSet(HidRdItemId_LogicalMinimum))
		 	p = enc::decodeUleb128(&field->m_table[HidRdItemId_LogicalMinimum], p, end);

		if (field->isSet(HidRdItemId_LogicalMaximum))
		 	p = enc::decodeUleb128(&field->m_table[HidRdItemId_LogicalMaximum], p, end);

		if (field->isSet(HidRdItemId_UnitExponent))
		 	p = enc::decodeUleb128(&field->m_table[HidRdItemId_UnitExponent], p, end);

		if (field->isSet(HidRdItemId_Unit))
		 	p = enc::decodeUleb128(&field->m_table[HidRdItemId_Unit], p, end);

		if (!field->isSet(HidRdItemId_Usage)) {
			p = enc::decodeUleb128(&field->m_table[HidRdItemId_UsageMinimum], p, end);
			p = enc::decodeUleb128(&field->m_table[HidRdItemId_UsageMaximum], p, end);
		} else {
			p = enc::decodeUleb128(&field->m_table[HidRdItemId_Usage], p, end);

			size_t auxUsageCount;
			p = enc::decodeUleb128(&auxUsageCount, p, end);
			field->m_auxUsageTable.setCount(auxUsageCount);
			sl::Array<uint32_t>::Rwi rwi = field->m_auxUsageTable;
			for (size_t i = 0; i < auxUsageCount; i++)
				p = enc::decodeUleb128(&rwi[i], p, end);
		}
	}

	report->updateSize();

	ASSERT(p - (char*)p0 == decodeInfoSize);
	return decodeInfoSize;
}

//..............................................................................

} // namespace io
} // namespace axl
