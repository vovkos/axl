#include "pch.h"
#include "axl_ini_GenericParser.h"

namespace axl {
namespace ini {

//.............................................................................

bool
GenericParser::onSection (const char* sectionName)
{
	ASSERT (m_ini);

	rtl::StringHashTableMapIterator <Section*> it = m_ini->m_namedSectionMap.find (sectionName);
	if (it)
	{
		m_currentSection = it->m_value;
		return true;
	}

	Section* section = AXL_MEM_NEW (Section);
	section->m_name = sectionName;
	m_ini->m_namedSectionList.insertTail (section);
	m_ini->m_namedSectionMap [section->m_name] = section;
	return true;
}

bool
GenericParser::onKeyValue (
	const char* keyName, 
	const char* value
	)
{
	ASSERT (m_currentSection);

	if (!value || !*value)
	{
		m_currentSection->m_unnamedValueList.insertTail (keyName);
		return true;
	}

	rtl::StringHashTableMapIterator <NamedValue*> it = m_currentSection->m_namedValueMap.find (keyName);
	if (it)
	{
		it->m_value->m_value = value;
		return true;
	}

	NamedValue* namedValue = AXL_MEM_NEW (NamedValue);
	namedValue->m_name = keyName;
	namedValue->m_value = value;
	m_currentSection->m_namedValueList.insertTail (namedValue);
	m_currentSection->m_namedValueMap [namedValue->m_value] = namedValue;
	return true;
}

//.............................................................................

} // namespace lua
} // namespace axl

