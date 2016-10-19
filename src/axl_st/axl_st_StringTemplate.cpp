#include "pch.h"
#include "axl_st_StringTemplate.h"

namespace axl {
namespace st {

//..............................................................................

bool
gotoEndOfScriptSnippet (
	Lexer* lexer,
	lex::RagelTokenPos* endPos
	)
{
	const Token* token = lexer->getToken ();
	ASSERT (token->m_token == TokenKind_OpenCode || token->m_token == TokenKind_OpenData);

	int openBracket;
	int closeBracket;
	LexerMachineKind machine;

	if (token->m_token == TokenKind_OpenData)
	{
		openBracket = '(';
		closeBracket = ')';
		machine = LexerMachineKind_UserData;
	}
	else
	{
		openBracket = '{';
		closeBracket = '}';
		machine = LexerMachineKind_UserCode;
	}

	lexer->gotoState (
		Lexer::getMachineState (machine),
		token,
		Lexer::GotoStateKind_EatToken
		);

	int level = 1;
	for (;;)
	{
		token = lexer->getToken ();
		if (token->m_token == TokenKind_Error)
		{
			err::setFormatStringError ("invalid character '\\x%02x'", (uchar_t) token->m_data.m_integer);
			return false;
		}
		else if (token->m_token == TokenKind_Eof)
		{
			lex::setUnexpectedTokenError ("eof", "user-code");
			return false;
		}
		else if (token->m_token == openBracket)
		{
			level++;
		}
		else if (token->m_token == closeBracket)
		{
			level--;
			if (level <= 0)
				break;
		}

		lexer->nextToken ();
	}

	*endPos = token->m_pos;

	lexer->gotoState (
		Lexer::getMachineState (LexerMachineKind_Main),
		token,
		Lexer::GotoStateKind_EatToken
		);

	return true;
}

//..............................................................................

} // namespace st
} // namespace axl
