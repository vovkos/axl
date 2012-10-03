#include "stdafx.h"
#include "axl_st_StringTemplate.h"
#include "axl_st_Lexer.h"

namespace axl {
namespace st {

//.............................................................................

bool
CStringTemplate::Process (
	rtl::CString* pResult,
	const rtl::CString& FilePath,
	const tchar_t* pTemplate,
	size_t Length
	)
{
	bool Result;

	pResult->Clear ();

	rtl::CStringA LuaSource;
	Result = ExtractLuaSource (&LuaSource, FilePath, pTemplate, Length);
	if (!Result)
		return false;

	if (LuaSource.IsEmpty ())
	{
		pResult->Copy (pTemplate, Length);
		return true;
	}

	luaL_openlibs (m_LuaState);

	Result = m_LuaState.Load (FilePath, LuaSource, LuaSource.GetLength ());
	if (!Result)
		return false;
			
	TPassthroughContext PassthroughContext;
	PassthroughContext.m_pResult = pResult;
	PassthroughContext.m_pTemplate = pTemplate;

	m_LuaState.RegisterFunction ("emit", Emit_lua, (intptr_t) pResult);
	m_LuaState.RegisterFunction ("passthrough", Passthrough_lua, (intptr_t) &PassthroughContext);

	Result = m_LuaState.PCall (0, 0);
	if (!Result)
		return false;
	
	m_LuaState.ClearStack ();
	return true;
}

bool 
ExtractUserCode (
	CLexer* pLexer,
	lex::CRagelTokenPos* pEndPos
	)
{
	const CToken* pToken = pLexer->GetToken ();
	ASSERT (pToken->m_Token == EToken_OpenCode || pToken->m_Token == EToken_OpenData);

	int OpenBracket;
	int CloseBracket;
	ELexerMachine Machine;

	if (pToken->m_Token == EToken_OpenData)
	{
		OpenBracket = '(';
		CloseBracket = ')';
		Machine = ELexerMachine_UserData;
	}
	else
	{
		OpenBracket = '{';
		CloseBracket = '}';
		Machine = ELexerMachine_UserCode;
	}

	pLexer->GotoState (
		CLexer::GetMachineState (Machine), 
		pToken, 
		CLexer::EGotoState_EatToken
		);

	int Level = 1;
	for (;;)
	{
		pToken = pLexer->GetToken ();
		if (pToken->m_Token == EToken_Error)
		{
			err::SetError (pToken->m_Data.m_Error);
			return false;
		}
		else if (pToken->m_Token == EToken_Eof)
		{
			err::SetUnexpectedTokenError (_T("eof"), _T("user-code"));
			return false;
		}
		else if (pToken->m_Token == OpenBracket)
		{
			Level++;
		}
		else if (pToken->m_Token == CloseBracket)
		{
			Level--;
			if (Level <= 0)
				break;
		}

		pLexer->NextToken ();
	}

	*pEndPos = pToken->m_Pos;

	pLexer->GotoState (
		CLexer::GetMachineState (ELexerMachine_Main), 
		pToken, 
		CLexer::EGotoState_EatToken
		);

	return true;
}

bool
CStringTemplate::ExtractLuaSource (
	rtl::CStringA* pLuaSource,
	const rtl::CString& FilePath,
	const tchar_t* pTemplate,
	size_t Length
	)
{
	bool Result;

	CLexer Lexer;
	Lexer.Create (FilePath, pTemplate, Length);

	pLuaSource->Clear ();
	
	size_t Offset = 0;
	CToken::CPos Pos;

	long Line = 0;


	for (;;)
	{
		const CToken* pToken = Lexer.GetToken ();
		
		if (pToken->m_Token == EToken_Error)
		{
			err::SetError (pToken->m_Data.m_Error);
			return false;
		}

		if (pToken->m_Pos.m_Offset > Offset)
			pLuaSource->AppendFormat ("passthrough (%d, %d);", Offset, pToken->m_Pos.m_Offset - Offset);

		if (pToken->m_Token == EToken_Eof)
			return true;

		if (pToken->m_Pos.m_Line > Line)
			pLuaSource->Append ('\n', pToken->m_Pos.m_Line - Line);

		Offset = pToken->m_Pos.m_Offset + pToken->m_Pos.m_Length;

		switch (pToken->m_Token)
		{
		case EToken_Data:
			pLuaSource->AppendFormat ("emit (%s);", pToken->m_Data.m_String);
			Pos = pToken->m_Pos;
			Lexer.NextToken ();
			break;

		case EToken_OpenCode:
			Result = ExtractUserCode (&Lexer, &Pos);
			if (!Result)
				return false;

			pLuaSource->Append (pTemplate + Offset, Pos.m_Offset - Offset);
			pLuaSource->Append (";");
			break;

		case EToken_OpenData:
			Result = ExtractUserCode (&Lexer, &Pos);
			if (!Result)
				return false;

			pLuaSource->Append ("emit (");
			pLuaSource->Append (pTemplate + Offset, Pos.m_Offset - Offset);
			pLuaSource->Append (");");
			break;

		default:
			ASSERT (false);
		}

		Offset = Pos.m_Offset + Pos.m_Length;
		Line = Pos.m_Line;
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int 
CStringTemplate::Emit_lua (lua_State* pLuaState)
{
	lua::CLuaState LuaState = pLuaState;

	rtl::CString* pResult = (rtl::CString*) LuaState.GetContext ();

	size_t Count = lua_gettop (pLuaState);
	for (size_t i = 1; i <= Count; i++)
	{
		const char* p = LuaState.GetString (i);
		pResult->Append (p);
	}

	LuaState.Detach ();
	return 0;
}

int 
CStringTemplate::Passthrough_lua (lua_State* pLuaState)
{
	lua::CLuaState LuaState = pLuaState;

	TPassthroughContext* pContext = (TPassthroughContext*) LuaState.GetContext ();

	size_t Offset = LuaState.GetInteger (1);
	size_t Length = LuaState.GetInteger (2);

	pContext->m_pResult->Append (pContext->m_pTemplate + Offset, Length);

	LuaState.Detach ();
	return 0;
}

//.............................................................................

} // namespace lua
} // namespace axl

