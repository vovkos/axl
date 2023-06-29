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
#include "axl_ini_GenericParser.h"

namespace axl {
namespace ini {

//..............................................................................

bool
GenericParser::onSection(const sl::StringRef& sectionName) {
	ASSERT(m_ini);

	sl::StringHashTableIterator<Section*> it = m_ini->m_namedSectionMap.find(sectionName);
	if (it) {
		m_currentSection = it->m_value;
		return true;
	}

	Section* section = new Section;
	section->m_name = sectionName;
	m_ini->m_namedSectionList.insertTail(section);
	m_ini->m_namedSectionMap[section->m_name] = section;
	return true;
}

bool
GenericParser::onKeyValue(
	const sl::StringRef& keyName,
	const sl::StringRef& value
) {
	ASSERT(m_currentSection);

	if (value.isEmpty()) {
		m_currentSection->m_unnamedValueList.insertTail(keyName);
		return true;
	}

	sl::StringHashTableIterator<NamedValue*> it = m_currentSection->m_namedValueMap.find(keyName);
	if (it) {
		it->m_value->m_value = value;
		return true;
	}

	NamedValue* namedValue = new NamedValue;
	namedValue->m_name = keyName;
	namedValue->m_value = value;
	m_currentSection->m_namedValueList.insertTail(namedValue);
	m_currentSection->m_namedValueMap[namedValue->m_value] = namedValue;
	return true;
}

//..............................................................................

} // namespace ini
} // namespace axl
