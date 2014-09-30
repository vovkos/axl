// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_lex_RagelLexer.h"

namespace axl {
namespace lua {

//.............................................................................

enum EToken
{
	EToken_Eof = 0,
	EToken_Error = -1,
	EToken_OpenCode = 256,
	EToken_OpenData,
	EToken_Data,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_LEX_BEGIN_TOKEN_NAME_MAP (CTokenName)
	AXL_LEX_TOKEN_NAME (EToken_Eof,       "eof")
	AXL_LEX_TOKEN_NAME (EToken_Error,     "error")
	AXL_LEX_TOKEN_NAME (EToken_OpenCode,  "%{")
	AXL_LEX_TOKEN_NAME (EToken_OpenData,  "%(")
	AXL_LEX_TOKEN_NAME (EToken_Data,      "user-data")
AXL_LEX_END_TOKEN_NAME_MAP ();

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef lex::CRagelTokenT <EToken, CTokenName> CToken;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ELexerMachine
{
	ELexerMachine_Main,
	ELexerMachine_UserCode,
	ELexerMachine_UserData,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CLexer: public lex::CRagelLexerT <CLexer, CToken>
{
	friend class lex::CRagelLexerT <CLexer, CToken>;

public:
	static
	int
	getMachineState (ELexerMachine machine);

protected:
	CToken*
	createStringToken (
		int tokenKind,
		int left = 0,
		int right = 0
		)
	{
		CToken* token = createToken (tokenKind);
		token->m_data.m_string.copy (ts + left, token->m_pos.m_length - (left + right));
		return token;
	}

	// implemented in *.rl

	void 
	init ();

	void
	exec ();
};

//.............................................................................

} // namespace lua {
} // namespace axl {
