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
#include "axl_dox_Lexer.h"
#include "axl_dox_Lexer.rl.cpp"

namespace axl {
namespace dox {

//..............................................................................

sl::StringRef
Lexer::getCommandParam()
{
	const DoxyToken* commandToken = getToken();
	const DoxyToken* nextToken = getToken(1);

	return
		nextToken->m_token == TokenKind_Text &&
		nextToken->m_pos.m_line == commandToken->m_pos.m_line ?
		nextToken->m_data.m_string.getTrimmedString() :
		sl::String();
}

DoxyToken*
Lexer::createTextToken(
	TokenKind tokenKind,
	size_t left,
	size_t right
	)
{
	Token* token = createToken(tokenKind);
	token->m_data.m_string = sl::StringRef(ts + left, te - ts - left - right);
	return token;
}

DoxyToken*
Lexer::createCustomCommandParamToken()
{
	size_t length = te - ts;
	if (*(te - 1) == '}')
		length--;

	Token* token = createToken(TokenKind_Text);
	token->m_data.m_string = sl::StringRef(ts, length).getTrimmedString();
	return token;
}

DoxyToken*
Lexer::createNewLineToken()
{
	ASSERT(*ts == '\n');

	Token* token = createToken('\n');
	token->m_data.m_string = sl::StringRef(ts + 1, te - ts - 1);
	return token;
}

//..............................................................................

} // namespace dox
} // namespace axl
