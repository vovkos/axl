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

DoxyToken*
Lexer::createTextToken(
	TokenKind tokenKind,
	size_t left
	)
{
	Token* token = createToken(tokenKind);
	token->m_data.m_string = sl::StringRef(ts + left, te - ts - left);
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
