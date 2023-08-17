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
#include "axl_io_HidDbParser.h"

namespace axl {
namespace io {

//..............................................................................

bool
HidUsagePageDirIniParser::onSection(const sl::StringRef& sectionName) {
	Section section = SectionNameMap::findValue(sectionName, Section_Undefined);
	if (!section)
		return true; // ignore

	if (m_page) {
		bool result = finalizePage();
		if (!result)
			return false;
	}

	m_page = new HidUsagePage(m_db);
	return true;
}

bool
HidUsagePageDirIniParser::onKeyValue(
	const sl::StringRef& keyName,
	const sl::StringRef& value
) {
	if (!m_page)
		return true; // ignore out-of-section keys

	Key key = KeyNameMap::findValue(keyName, Key_Undefined);
	switch (key) {
	case Key_Id:
		m_page->m_id = strtoul(value.sz(), NULL, 0);
		break;

	case Key_Name:
		m_page->m_name = value;
		break;

	case Key_FileName:
		m_page->m_fileName = value;
		break;

	case Key_Format:
		m_page->m_format = value;
		break;
	}

	return true;
}

bool
HidUsagePageDirIniParser::finalizePage() {
	if (!m_page->m_id ||
		m_page->m_name.isEmpty() ||
		m_page->m_fileName.isEmpty() && m_page->m_format.isEmpty()
	)
		return err::fail("invalid usage page");

	m_db->m_usagePageList.insertTail(m_page);
	m_db->m_usagePageMap[m_page->m_id] = m_page;
	m_page = NULL;
	return true;
}

//..............................................................................

bool
HidUsageIniParser::onKeyValue(
	const sl::StringRef& keyName,
	const sl::StringRef& value
) {
	if (!m_section)
		return true; // ignore

	uint_t id = strtoul(keyName.sz(), NULL, 0);
	m_page->m_usageNameMap[id] = value;
	return true;
}

//..............................................................................

} // namespace io
} // namespace axl
