// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

namespace axl {
namespace pg {

//.............................................................................

enum EToken
{
	EToken_Eof = 0,
	EToken_Error = -1,
	EToken_Identifier = 256,
	EToken_Integer,
	EToken_Literal,
	EToken_LL,
	EToken_Import,
	EToken_Using,
	EToken_Class,
	EToken_NoAst,
	EToken_Default,
	EToken_Arg,
	EToken_Local,
	EToken_Enter,
	EToken_Leave,
	EToken_Start,
	EToken_Pragma,
	EToken_Resolver,
	EToken_Priority,
	EToken_Any,
	EToken_Epsilon,
	EToken_Nullable,
	EToken_OpenBrace,
	EToken_CloseBrace,
	EToken_OpenChevron,
	EToken_CloseChevron,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_PRS_BEGIN_TOKEN_NAME_MAP (CTokenName)
	AXL_PRS_TOKEN_NAME (EToken_Eof,          "eof")
	AXL_PRS_TOKEN_NAME (EToken_Error,        "error")
	AXL_PRS_TOKEN_NAME (EToken_Identifier,   "identifier")
	AXL_PRS_TOKEN_NAME (EToken_Integer,      "integer-constant")
	AXL_PRS_TOKEN_NAME (EToken_Literal,      "string-literal")
	AXL_PRS_TOKEN_NAME (EToken_LL,           "LL")
	AXL_PRS_TOKEN_NAME (EToken_Import,       "import")
	AXL_PRS_TOKEN_NAME (EToken_Using,        "using")
	AXL_PRS_TOKEN_NAME (EToken_Class,        "class")
	AXL_PRS_TOKEN_NAME (EToken_NoAst,        "noast")
	AXL_PRS_TOKEN_NAME (EToken_Default,      "default")
	AXL_PRS_TOKEN_NAME (EToken_Arg,          "arg")
	AXL_PRS_TOKEN_NAME (EToken_Local,        "local")
	AXL_PRS_TOKEN_NAME (EToken_Enter,        "enter")
	AXL_PRS_TOKEN_NAME (EToken_Leave,        "leave")
	AXL_PRS_TOKEN_NAME (EToken_Start,        "start")
	AXL_PRS_TOKEN_NAME (EToken_Pragma,       "pragma")
	AXL_PRS_TOKEN_NAME (EToken_Resolver,     "resolver")
	AXL_PRS_TOKEN_NAME (EToken_Priority,     "priority")
	AXL_PRS_TOKEN_NAME (EToken_Any,          "any")
	AXL_PRS_TOKEN_NAME (EToken_Epsilon,      "epsilon")
	AXL_PRS_TOKEN_NAME (EToken_Nullable,     "nullable")
	AXL_PRS_TOKEN_NAME (EToken_OpenBrace,    "{.")
	AXL_PRS_TOKEN_NAME (EToken_CloseBrace,   ".}")
	AXL_PRS_TOKEN_NAME (EToken_OpenChevron,  "<.")
	AXL_PRS_TOKEN_NAME (EToken_CloseChevron, ".>")
AXL_PRS_END_TOKEN_NAME_MAP ();

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef lex::CRagelTokenT <EToken, CTokenName> CToken;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ELexerMachine
{
	ELexerMachine_Main,
	ELexerMachine_UserCode,
	ELexerMachine_UserCode2ndPass,
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

	CToken*
	CreateCharToken (int Token)
	{
		CToken* pToken = CreateToken (Token);
		pToken->m_Data.m_Integer = ts [1];
		return pToken;
	}

	CToken*
	CreateIntegerToken (
		int Radix = 10,
		int Left = 0
		)
	{
		CToken* pToken = CreateToken (EToken_Integer);
		pToken->m_Data.m_Integer = strtol (ts + Left, NULL, Radix);
		return pToken;
	}

	CToken*
	CreateConstIntegerToken (int Value)
	{
		CToken* pToken = CreateToken (EToken_Integer);
		pToken->m_Data.m_Integer = Value;
		return pToken;
	}

	// implemented in *.rl

	void 
	Init ();

	bool
	Exec ();
};

//.............................................................................

} // namespace pg {
} // namespace axl {
