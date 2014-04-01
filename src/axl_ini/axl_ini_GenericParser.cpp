#include "pch.h"
#include "axl_ini_GenericParser.h"

namespace axl {
namespace ini {

//.............................................................................

bool
CGenericParser::OnSection (const char* pSectionName)
{
	ASSERT (m_pIni);

	rtl::CStringHashTableMapIteratorT <CSection*> It = m_pIni->m_NamedSectionMap.Find (pSectionName);
	if (It)
	{
		m_pCurrentSection = It->m_Value;
		return true;
	}

	CSection* pSection = AXL_MEM_NEW (CSection);
	pSection->m_Name = pSectionName;
	m_pIni->m_NamedSectionList.InsertTail (pSection);
	m_pIni->m_NamedSectionMap [pSection->m_Name] = pSection;
	return true;
}

bool
CGenericParser::OnKeyValue (
	const char* pKeyName, 
	const char* pValue
	)
{
	ASSERT (m_pCurrentSection);

	if (!pValue || !*pValue)
	{
		m_pCurrentSection->m_UnnamedValueList.InsertTail (pKeyName);
		return true;
	}

	rtl::CStringHashTableMapIteratorT <CNamedValue*> It = m_pCurrentSection->m_NamedValueMap.Find (pKeyName);
	if (It)
	{
		It->m_Value->m_Value = pValue;
		return true;
	}

	CNamedValue* pNamedValue = AXL_MEM_NEW (CNamedValue);
	pNamedValue->m_Name = pKeyName;
	pNamedValue->m_Value = pValue;
	m_pCurrentSection->m_NamedValueList.InsertTail (pNamedValue);
	m_pCurrentSection->m_NamedValueMap [pNamedValue->m_Value] = pNamedValue;
	return true;
}

//.............................................................................

} // namespace lua
} // namespace axl

