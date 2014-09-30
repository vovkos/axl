#include "pch.h"
#include "axl_rtl_IniParser.h"

namespace axl {
namespace rtl {

//.............................................................................

bool
CIniParserRoot::parseLine (
	const char* p0,
	size_t length,
	rtl::CString* name,
	rtl::CString* value,
	bool* isSection,
	size_t* lineLength
	)
{
	ASSERT (length != -1);

	const char* p = p0;
	const char* end = p + length;

	EState state = EState_Space;
	EState prevState = EState_Space;
	
	name->clear ();
	value->clear ();
	*isSection = false;

	for (; p < end; p++)
	{
		char x = *p;
		if (x == '\r') // ignore CR
			continue;

		switch (state)
		{
		case EState_Space:
			switch (x)
			{
			case '\'': case '#': case ';':
				state = EState_Comment;
				break;

			case '[':
				state = EState_Section;
				name
				break;

			case '\n':
				m_line++;

				break;

			default:
				if (isspace(x))
					break;

				p--; // re-read

				if (prevState != EState_Space)
					state = prevState;
				else
					state = EState_Key, keyLine = line, keyName.clear();
			};

			break;

		case EState_Comment:
			if (x == '\n')
				state = prevState, line++;
			break;

		case EState_Section:
			if (x == '\n')
			{
				state = EState_Error; 
				break;
			}
			else if (x != ']')
			{
				sectionName += (char) x; 
				break;
			}

			if (pfnOnSection != NULL)
				sectionContext = pfnOnSection(context, sectionName);

			state = prevState = EState_Space;
			break;

		case EState_Key:
			if (x == '\n' || line != keyLine)
			{
				state = EState_Error;
				break;
			}

			if (isspace(x))
			{
				prevState = state;
				state = EState_Space;
				break;
			}

			if (x != '=')
				keyName += (char) x;
			else
				state = EState_Value, value.clear();

			break;

		case EState_Value:
			if (x == '\n' || line != keyLine)
			{
				pfnOnKeyValue(context, sectionContext, sectionName, keyName, value);
				state = prevState = EState_Space;
				p--;
				break;
			}

			if (value.isEmpty() && isspace(x))
			{
				prevState = state;
				state = EState_Space;
				break;
			}

			value += (char) x;
			break;
		}
	}

	// if there was no \n on the last line

	if (state == EState_Value)
		pfnOnKeyValue(context, sectionContext, sectionName, keyName, value);

	return true;
}


/*

namespace ini {

struct EState { enum 
{
	error = -1,
	space = 0,
	comment,
	section,
	key,
	value,
};};

} // namespace ini

bool
ini::parseIniA(
	const char* source,
	size_t length,
	FOnSectionA pfnOnSection,
	FOnKeyValueA pfnOnKeyValue,
	void* context
	)
{
	ASSERT(pfnOnKeyValue);

	if (length == -1)
		length = strlen(source);

	const char* p = source;
	const char* end = p + length;

//.............................................................................

void ini::CIniParser::setSection(const TCHAR* sectionName, void* context)
{
	rtl::TBinTreeNode* node = m_sectionMap.find((intptr_t) sectionName);
	if (node)
	{
		m_currentSection = (TSection*) node->m_value;
		m_currentSection->m_context = context;
		return;
	}

	m_currentSection = new TSection;
	m_currentSection->m_sectionName = sectionName;
	m_currentSection->m_context = context;
	m_sectionList.insertTail(m_currentSection);
	
	node = m_sectionMap.goto((intptr_t) (const TCHAR*) m_currentSection->m_sectionName);
	node->m_value = (intptr_t) m_currentSection;
}

void ini::CIniParser::setKeyHandler(const TCHAR* keyName, FOnKeyValue pfnOnKeyValue, void* context)
{
	rtl::TBinTreeNode* node;

	if (!m_currentSection)
	{
		m_currentSection = new TSection;
		m_sectionList.insertTail(m_currentSection);

		node = m_sectionMap.goto(NULL);
		node->m_value = (intptr_t) m_currentSection;
	}

	TKeyHandler* handler;

	node = m_sectionMap.find((intptr_t) keyName);
	if (node)
	{
		handler = (TKeyHandler*) node->m_value;
		handler->m_pfnOnKeyValue = pfnOnKeyValue;
		handler->m_context = context;
		return;
	}

	handler = new TKeyHandler;
	handler->m_keyName = keyName;
	handler->m_pfnOnKeyValue = pfnOnKeyValue;
	handler->m_context = context;
	m_currentSection->m_keyHandlerList.insertTail(handler);

	node = m_currentSection->m_keyHandlerMap.goto((intptr_t) (const TCHAR*) handler->m_keyName);
	node->m_value = (intptr_t) handler;
}

void ini::CIniParser::clear()
{
	m_sectionList.clear();
	m_sectionMap.clear();
	m_currentSection = NULL;
}

bool ini::CIniParser::parse(const TCHAR* source, size_t length)
{
	return ini::parseIni(source, length, _OnSection, _OnKeyValue, this);
}

void* ini::CIniParser::_OnSection(
	void* context,
	const TCHAR* sectionName
	)
{
	CIniParser* this = (CIniParser*) context;
	rtl::TBinTreeNode* node = this->m_sectionMap.find((intptr_t) sectionName);
	return node ? (TSection*) node->m_value : NULL;
}

void ini::CIniParser::_OnKeyValue(
	void* context,
	void* sectionContext,
	const TCHAR* sectionName,
	const TCHAR* keyName,
	const TCHAR* value
	)
{
	CIniParser* this = (CIniParser*) context;
	
	rtl::TBinTreeNode* node;
	TSection* section;

	if (sectionContext)
	{
		section = (TSection*) sectionContext;
	}
	else
	{
		node = this->m_sectionMap.find(NULL);
		if (!node)
			return;

		section = (TSection*) node->m_value;
	}
	
	node = section->m_keyHandlerMap.find((intptr_t) keyName);
	if (!node)
		return;

	TKeyHandler* handler = (TKeyHandler*) node->m_value;
	return handler->m_pfnOnKeyValue(
		handler->m_context, 
		section->m_context,
		sectionName,
		keyName,
		value
		);
}

//.............................................................................

*/


//.............................................................................

} // namespace err
} // namespace axl
