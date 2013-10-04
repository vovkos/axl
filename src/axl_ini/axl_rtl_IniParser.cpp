#include "pch.h"
#include "axl_rtl_IniParser.h"

namespace axl {
namespace rtl {

//.............................................................................

bool
CIniParserRoot::ParseLine (
	const char* p0,
	size_t Length,
	rtl::CString* pName,
	rtl::CString* pValue,
	bool* pIsSection,
	size_t* pLineLength
	)
{
	ASSERT (Length != -1);

	const char* p = p0;
	const char* pEnd = p + Length;

	EState State = EState_Space;
	EState PrevState = EState_Space;
	
	pName->Clear ();
	pValue->Clear ();
	*pIsSection = false;

	for (; p < pEnd; p++)
	{
		char x = *p;
		if (x == '\r') // ignore CR
			continue;

		switch (State)
		{
		case EState_Space:
			switch (x)
			{
			case '\'': case '#': case ';':
				State = EState_Comment;
				break;

			case '[':
				State = EState_Section;
				pName
				break;

			case '\n':
				m_Line++;

				break;

			default:
				if (isspace(x))
					break;

				p--; // re-read

				if (PrevState != EState_Space)
					State = PrevState;
				else
					State = EState_Key, KeyLine = Line, KeyName.Clear();
			};

			break;

		case EState_Comment:
			if (x == '\n')
				State = PrevState, Line++;
			break;

		case EState_Section:
			if (x == '\n')
			{
				State = EState_Error; 
				break;
			}
			else if (x != ']')
			{
				SectionName += (char) x; 
				break;
			}

			if (pfnOnSection != NULL)
				pSectionContext = pfnOnSection(pContext, SectionName);

			State = PrevState = EState_Space;
			break;

		case EState_Key:
			if (x == '\n' || Line != KeyLine)
			{
				State = EState_Error;
				break;
			}

			if (isspace(x))
			{
				PrevState = State;
				State = EState_Space;
				break;
			}

			if (x != '=')
				KeyName += (char) x;
			else
				State = EState_Value, Value.Clear();

			break;

		case EState_Value:
			if (x == '\n' || Line != KeyLine)
			{
				pfnOnKeyValue(pContext, pSectionContext, SectionName, KeyName, Value);
				State = PrevState = EState_Space;
				p--;
				break;
			}

			if (Value.IsEmpty() && isspace(x))
			{
				PrevState = State;
				State = EState_Space;
				break;
			}

			Value += (char) x;
			break;
		}
	}

	// if there was no \n on the last line

	if (State == EState_Value)
		pfnOnKeyValue(pContext, pSectionContext, SectionName, KeyName, Value);

	return true;
}


/*

namespace ini {

struct EState { enum 
{
	Error = -1,
	Space = 0,
	Comment,
	Section,
	Key,
	Value,
};};

} // namespace ini

bool
ini::ParseIniA(
	const char* pSource,
	size_t Length,
	FOnSectionA pfnOnSection,
	FOnKeyValueA pfnOnKeyValue,
	void* pContext
	)
{
	ASSERT(pfnOnKeyValue);

	if (Length == -1)
		Length = strlen(pSource);

	const char* p = pSource;
	const char* pEnd = p + Length;

//.............................................................................

void ini::CIniParser::SetSection(const TCHAR* pSectionName, void* pContext)
{
	rtl::TBinTreeNode* pNode = m_SectionMap.Find((intptr_t) pSectionName);
	if (pNode)
	{
		m_pCurrentSection = (TSection*) pNode->m_Value;
		m_pCurrentSection->m_pContext = pContext;
		return;
	}

	m_pCurrentSection = new TSection;
	m_pCurrentSection->m_SectionName = pSectionName;
	m_pCurrentSection->m_pContext = pContext;
	m_SectionList.InsertTail(m_pCurrentSection);
	
	pNode = m_SectionMap.Goto((intptr_t) (const TCHAR*) m_pCurrentSection->m_SectionName);
	pNode->m_Value = (intptr_t) m_pCurrentSection;
}

void ini::CIniParser::SetKeyHandler(const TCHAR* pKeyName, FOnKeyValue pfnOnKeyValue, void* pContext)
{
	rtl::TBinTreeNode* pNode;

	if (!m_pCurrentSection)
	{
		m_pCurrentSection = new TSection;
		m_SectionList.InsertTail(m_pCurrentSection);

		pNode = m_SectionMap.Goto(NULL);
		pNode->m_Value = (intptr_t) m_pCurrentSection;
	}

	TKeyHandler* pHandler;

	pNode = m_SectionMap.Find((intptr_t) pKeyName);
	if (pNode)
	{
		pHandler = (TKeyHandler*) pNode->m_Value;
		pHandler->m_pfnOnKeyValue = pfnOnKeyValue;
		pHandler->m_pContext = pContext;
		return;
	}

	pHandler = new TKeyHandler;
	pHandler->m_KeyName = pKeyName;
	pHandler->m_pfnOnKeyValue = pfnOnKeyValue;
	pHandler->m_pContext = pContext;
	m_pCurrentSection->m_KeyHandlerList.InsertTail(pHandler);

	pNode = m_pCurrentSection->m_KeyHandlerMap.Goto((intptr_t) (const TCHAR*) pHandler->m_KeyName);
	pNode->m_Value = (intptr_t) pHandler;
}

void ini::CIniParser::Clear()
{
	m_SectionList.Clear();
	m_SectionMap.Clear();
	m_pCurrentSection = NULL;
}

bool ini::CIniParser::Parse(const TCHAR* pSource, size_t Length)
{
	return ini::ParseIni(pSource, Length, _OnSection, _OnKeyValue, this);
}

void* ini::CIniParser::_OnSection(
	void* pContext,
	const TCHAR* pSectionName
	)
{
	CIniParser* pThis = (CIniParser*) pContext;
	rtl::TBinTreeNode* pNode = pThis->m_SectionMap.Find((intptr_t) pSectionName);
	return pNode ? (TSection*) pNode->m_Value : NULL;
}

void ini::CIniParser::_OnKeyValue(
	void* pContext,
	void* pSectionContext,
	const TCHAR* pSectionName,
	const TCHAR* pKeyName,
	const TCHAR* pValue
	)
{
	CIniParser* pThis = (CIniParser*) pContext;
	
	rtl::TBinTreeNode* pNode;
	TSection* pSection;

	if (pSectionContext)
	{
		pSection = (TSection*) pSectionContext;
	}
	else
	{
		pNode = pThis->m_SectionMap.Find(NULL);
		if (!pNode)
			return;

		pSection = (TSection*) pNode->m_Value;
	}
	
	pNode = pSection->m_KeyHandlerMap.Find((intptr_t) pKeyName);
	if (!pNode)
		return;

	TKeyHandler* pHandler = (TKeyHandler*) pNode->m_Value;
	return pHandler->m_pfnOnKeyValue(
		pHandler->m_pContext, 
		pSection->m_pContext,
		pSectionName,
		pKeyName,
		pValue
		);
}

//.............................................................................

*/


//.............................................................................

} // namespace err
} // namespace axl
