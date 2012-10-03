// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_lex_RagelLexer.h"

namespace axl {
namespace st {

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

AXL_PRS_BEGIN_TOKEN_NAME_MAP (CTokenName)
	AXL_PRS_TOKEN_NAME (EToken_Eof,       "eof")
	AXL_PRS_TOKEN_NAME (EToken_Error,     "error")
	AXL_PRS_TOKEN_NAME (EToken_OpenCode,  "%{")
	AXL_PRS_TOKEN_NAME (EToken_OpenData,  "%(")
	AXL_PRS_TOKEN_NAME (EToken_Data,      "user-data")
AXL_PRS_END_TOKEN_NAME_MAP ();

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
	GetMachineState (ELexerMachine Machine);

protected:
	CToken*
	CreateStringToken (
		int Token,
		int Left = 0,
		int Right = 0
		)
	{
		CToken* pToken = CreateToken (Token);
		pToken->m_Data.m_String.Copy (ts + Left, pToken->m_Pos.m_Length - (Left + Right));
		return pToken;
	}

	// implemented in *.rl

	void 
	Init ();

	bool
	Exec ();
};

//.............................................................................

} // namespace st {
} // namespace axl {
