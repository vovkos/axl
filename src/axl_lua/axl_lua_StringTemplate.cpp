#include "pch.h"
#include "axl_lua_StringTemplate.h"
#include "axl_lua_StringTemplateLexer.h"

namespace axl {
namespace lua {

//.............................................................................

void
CStringTemplate::Reset ()
{
	m_LuaState.Create ();
	m_LineCol.Clear ();
}

bool
CStringTemplate::Process (
	rtl::CString* pResult,
	const rtl::CString& FilePath,
	const char* pTemplate,
	size_t Length
	)
{
	bool Result;

	pResult->Clear ();

	rtl::CString LuaSource;
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
			
	TEmitContext EmitContext;
	EmitContext.m_pThis = this;
	EmitContext.m_pResult = pResult;
	EmitContext.m_pTemplate = pTemplate;

	m_LuaState.RegisterFunction ("GetLine", GetLine_lua, (intptr_t) this);
	m_LuaState.RegisterFunction ("GetCol", GetCol_lua, (intptr_t) this);
	m_LuaState.RegisterFunction ("Emit", Emit_lua, (intptr_t) &EmitContext);
	m_LuaState.RegisterFunction ("Passthrough", Passthrough_lua, (intptr_t) &EmitContext);

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
			err::SetUnexpectedTokenError ("eof", "user-code");
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
	rtl::CString* pLuaSource,
	const rtl::CString& FilePath,
	const char* pTemplate,
	size_t Length
	)
{
	bool Result;

	CLexer Lexer;
	Lexer.Create (FilePath, pTemplate, Length);

	pLuaSource->Clear ();
	
	size_t Offset = 0;
	CToken::CPos Pos;

	int Line = 0;

	for (;;)
	{
		const CToken* pToken = Lexer.GetToken ();
		
		if (pToken->m_Token == EToken_Error)
		{
			err::SetError (pToken->m_Data.m_Error);
			return false;
		}

		if (pToken->m_Pos.m_Offset > Offset)
			pLuaSource->AppendFormat ("Passthrough (%d, %d);", Offset, pToken->m_Pos.m_Offset - Offset);

		if (pToken->m_Token == EToken_Eof)
			return true;

		if (pToken->m_Pos.m_Line > Line)
			pLuaSource->Append ('\n', pToken->m_Pos.m_Line - Line);

		Offset = pToken->m_Pos.m_Offset + pToken->m_Pos.m_Length;

		switch (pToken->m_Token)
		{
		case EToken_Data:
			pLuaSource->AppendFormat ("Emit (%s);", pToken->m_Data.m_String.cc ()); // thanks a lot gcc
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

			pLuaSource->Append ("Emit (");
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

void
CStringTemplate::CountLineCol (
	const char* p,
	size_t Length
	)
{
	if (!p)
		return;

	if (Length == -1)
		Length = strlen (p);

	const char* pEnd = p + Length;
	const char* pLine = p;

	for (; p < pEnd; p++)
		if (*p == '\n')
		{
			m_LineCol.m_Line++;
			pLine = p + 1;
		}

	m_LineCol.m_Col = (int) (p - pLine);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int 
CStringTemplate::GetLine_lua (lua_State* pLuaState)
{
	CLuaState LuaState = pLuaState;

	CStringTemplate* pThis = (CStringTemplate*) LuaState.GetContext ();

	LuaState.PushInteger (pThis->m_LineCol.m_Line);

	LuaState.Detach ();
	return 1;
}

int 
CStringTemplate::GetCol_lua (lua_State* pLuaState)
{
	CLuaState LuaState = pLuaState;

	CStringTemplate* pThis = (CStringTemplate*) LuaState.GetContext ();

	LuaState.PushInteger (pThis->m_LineCol.m_Col);

	LuaState.Detach ();
	return 1;
}

int 
CStringTemplate::Emit_lua (lua_State* pLuaState)
{
	CLuaState LuaState = pLuaState;

	TEmitContext* pContext = (TEmitContext*) LuaState.GetContext ();

	size_t Count = lua_gettop (pLuaState);
	for (size_t i = 1; i <= Count; i++)
	{
		const char* p = LuaState.GetString (i);
		pContext->m_pResult->Append (p);
		pContext->m_pThis->CountLineCol (p);
	}

	LuaState.Detach ();
	return 0;
}

int 
CStringTemplate::Passthrough_lua (lua_State* pLuaState)
{
	CLuaState LuaState = pLuaState;

	TEmitContext* pContext = (TEmitContext*) LuaState.GetContext ();

	size_t Offset = LuaState.GetInteger (1);
	size_t Length = LuaState.GetInteger (2);

	const char* p = pContext->m_pTemplate + Offset;

	pContext->m_pResult->Append (p, Length);
	pContext->m_pThis->CountLineCol (p, Length);

	LuaState.Detach ();
	return 0;
}

//.............................................................................

} // namespace lua
} // namespace axl

