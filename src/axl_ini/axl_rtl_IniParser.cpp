#include "pch.h"
#include "axl_rtl_IniParser.h"

namespace axl {
namespace rtl {

//.............................................................................

bool
IniParserRoot::parseLine (
	const char* p0,
	size_t length,
	rtl::String* name,
	rtl::String* value,
	bool* isSection,
	size_t* lineLength
	)
{
	ASSERT (length != -1);

	const char* p = p0;
	const char* end = p + length;

	StateKind state = StateKind_Space;
	StateKind prevState = StateKind_Space;
	
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
		case StateKind_Space:
			switch (x)
			{
			case '\'': case '#': case ';':
				state = StateKind_Comment;
				break;

			case '[':
				state = StateKind_Section;
				name
				break;

			case '\n':
				m_line++;

				break;

			default:
				if (isspace(x))
					break;

				p--; // re-read

				if (prevState != StateKind_Space)
					state = prevState;
				else
					state = StateKind_Key, keyLine = line, keyName.clear();
			};

			break;

		case StateKind_Comment:
			if (x == '\n')
				state = prevState, line++;
			break;

		case StateKind_Section:
			if (x == '\n')
			{
				state = StateKind_Error; 
				break;
			}
			else if (x != ']')
			{
				sectionName += (char) x; 
				break;
			}

			if (pfnOnSection != NULL)
				sectionContext = pfnOnSection(context, sectionName);

			state = prevState = StateKind_Space;
			break;

		case StateKind_Key:
			if (x == '\n' || line != keyLine)
			{
				state = StateKind_Error;
				break;
			}

			if (isspace(x))
			{
				prevState = state;
				state = StateKind_Space;
				break;
			}

			if (x != '=')
				keyName += (char) x;
			else
				state = StateKind_Value, value.clear();

			break;

		case StateKind_Value:
			if (x == '\n' || line != keyLine)
			{
				pfnOnKeyValue(context, sectionContext, sectionName, keyName, value);
				state = prevState = StateKind_Space;
				p--;
				break;
			}

			if (value.isEmpty() && isspace(x))
			{
				prevState = state;
				state = StateKind_Space;
				break;
			}

			value += (char) x;
			break;
		}
	}

	// if there was no \n on the last line

	if (state == StateKind_Value)
		pfnOnKeyValue(context, sectionContext, sectionName, keyName, value);

	return true;
}


/*

namespace ini {

struct StateKind { enum 
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

void ini::IniParser::setSection(const TCHAR* sectionName, void* context)
{
	rtl::BinreeNode* node = m_sectionMap.find((intptr_t) sectionName);
	if (node)
	{
		m_currentSection = (Section*) node->m_value;
		m_currentSection->m_context = context;
		return;
	}

	m_currentSection = new Section;
	m_currentSection->m_sectionName = sectionName;
	m_currentSection->m_context = context;
	m_sectionList.insertTail(m_currentSection);
	
	node = m_sectionMap.goto((intptr_t) (const TCHAR*) m_currentSection->m_sectionName);
	node->m_value = (intptr_t) m_currentSection;
}

void ini::IniParser::setKeyHandler(const TCHAR* keyName, FOnKeyValue pfnOnKeyValue, void* context)
{
	rtl::BinreeNode* node;

	if (!m_currentSection)
	{
		m_currentSection = new Section;
		m_sectionList.insertTail(m_currentSection);

		node = m_sectionMap.goto(NULL);
		node->m_value = (intptr_t) m_currentSection;
	}

	KeyHandler* handler;

	node = m_sectionMap.find((intptr_t) keyName);
	if (node)
	{
		handler = (KeyHandler*) node->m_value;
		handler->m_pfnOnKeyValue = pfnOnKeyValue;
		handler->m_context = context;
		return;
	}

	handler = new KeyHandler;
	handler->m_keyName = keyName;
	handler->m_pfnOnKeyValue = pfnOnKeyValue;
	handler->m_context = context;
	m_currentSection->m_keyHandlerList.insertTail(handler);

	node = m_currentSection->m_keyHandlerMap.goto((intptr_t) (const TCHAR*) handler->m_keyName);
	node->m_value = (intptr_t) handler;
}

void ini::IniParser::clear()
{
	m_sectionList.clear();
	m_sectionMap.clear();
	m_currentSection = NULL;
}

bool ini::IniParser::parse(const TCHAR* source, size_t length)
{
	return ini::parseIni(source, length, _OnSection, _OnKeyValue, this);
}

void* ini::IniParser::_OnSection(
	void* context,
	const TCHAR* sectionName
	)
{
	IniParser* this = (IniParser*) context;
	rtl::BinreeNode* node = this->m_sectionMap.find((intptr_t) sectionName);
	return node ? (Section*) node->m_value : NULL;
}

void ini::IniParser::_OnKeyValue(
	void* context,
	void* sectionContext,
	const TCHAR* sectionName,
	const TCHAR* keyName,
	const TCHAR* value
	)
{
	IniParser* this = (IniParser*) context;
	
	rtl::BinreeNode* node;
	Section* section;

	if (sectionContext)
	{
		section = (Section*) sectionContext;
	}
	else
	{
		node = this->m_sectionMap.find(NULL);
		if (!node)
			return;

		section = (Section*) node->m_value;
	}
	
	node = section->m_keyHandlerMap.find((intptr_t) keyName);
	if (!node)
		return;

	KeyHandler* handler = (KeyHandler*) node->m_value;
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
