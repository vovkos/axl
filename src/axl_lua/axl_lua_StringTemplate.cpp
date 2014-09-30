#include "pch.h"
#include "axl_lua_StringTemplate.h"
#include "axl_lua_StringTemplateLexer.h"

namespace axl {
namespace lua {

//.............................................................................

void
CStringTemplate::reset ()
{
	m_luaState.create ();
	m_lineCol.clear ();
}

bool
CStringTemplate::process (
	rtl::CString* output,
	const rtl::CString& filePath,
	const char* source,
	size_t length
	)
{
	bool result;

	output->clear ();

	rtl::CString luaSource;
	result = extractLuaSource (&luaSource, filePath, source, length);
	if (!result)
		return false;

	if (luaSource.isEmpty ())
	{
		output->copy (source, length);
		return true;
	}

	luaL_openlibs (m_luaState);

	result = m_luaState.load (filePath, luaSource, luaSource.getLength ());
	if (!result)
		return false;
			
	TEmitContext emitContext;
	emitContext.m_this = this;
	emitContext.m_output = output;
	emitContext.m_source = source;

	m_luaState.registerFunction ("GetLine", getLine_lua, (intptr_t) this);
	m_luaState.registerFunction ("GetCol", getCol_lua, (intptr_t) this);
	m_luaState.registerFunction ("Emit", emit_lua, (intptr_t) &emitContext);
	m_luaState.registerFunction ("Passthrough", passthrough_lua, (intptr_t) &emitContext);

	result = m_luaState.PCall (0, 0);
	if (!result)
		return false;
	
	m_luaState.clearStack ();
	return true;
}

bool 
extractUserCode (
	CLexer* lexer,
	lex::CRagelTokenPos* endPos_o
	)
{
	const CToken* token = lexer->getToken ();
	ASSERT (token->m_token == EToken_OpenCode || token->m_token == EToken_OpenData);

	int openBracket;
	int closeBracket;
	ELexerMachine machine;

	if (token->m_token == EToken_OpenData)
	{
		openBracket = '(';
		closeBracket = ')';
		machine = ELexerMachine_UserData;
	}
	else
	{
		openBracket = '{';
		closeBracket = '}';
		machine = ELexerMachine_UserCode;
	}

	lexer->gotoState (
		CLexer::getMachineState (machine), 
		token, 
		CLexer::EGotoState_EatToken
		);

	int level = 1;
	for (;;)
	{
		token = lexer->getToken ();
		if (token->m_token == EToken_Error)
		{
			err::setFormatStringError ("invalid character '\\x%02x'", (uchar_t) token->m_data.m_integer);
			return false;
		}
		else if (token->m_token == EToken_Eof)
		{
			err::setUnexpectedTokenError ("eof", "user-code");
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

	*endPos_o = token->m_pos;

	lexer->gotoState (
		CLexer::getMachineState (ELexerMachine_Main), 
		token, 
		CLexer::EGotoState_EatToken
		);

	return true;
}

bool
CStringTemplate::extractLuaSource (
	rtl::CString* luaSource,
	const rtl::CString& filePath,
	const char* source,
	size_t length
	)
{
	bool result;

	CLexer lexer;
	lexer.create (filePath, source, length);

	luaSource->clear ();
	
	size_t offset = 0;
	CToken::CPos pos;

	int line = 0;

	for (;;)
	{
		const CToken* token = lexer.getToken ();
		
		if (token->m_token == EToken_Error)
		{
			err::setFormatStringError ("invalid character '\\x%02x'", (uchar_t) token->m_data.m_integer);
			return false;
		}

		if (token->m_pos.m_offset > offset)
			luaSource->appendFormat ("Passthrough (%d, %d);", offset, token->m_pos.m_offset - offset);

		if (token->m_token == EToken_Eof)
			return true;

		if (token->m_pos.m_line > line)
			luaSource->append ('\n', token->m_pos.m_line - line);

		offset = token->m_pos.m_offset + token->m_pos.m_length;

		switch (token->m_token)
		{
		case EToken_Data:
			luaSource->appendFormat ("Emit (%s);", token->m_data.m_string.cc ()); // thanks a lot gcc
			pos = token->m_pos;
			lexer.nextToken ();
			break;

		case EToken_OpenCode:
			result = extractUserCode (&lexer, &pos);
			if (!result)
				return false;

			luaSource->append (source + offset, pos.m_offset - offset);
			luaSource->append (";");
			break;

		case EToken_OpenData:
			result = extractUserCode (&lexer, &pos);
			if (!result)
				return false;

			luaSource->append ("Emit (");
			luaSource->append (source + offset, pos.m_offset - offset);
			luaSource->append (");");
			break;

		default:
			ASSERT (false);
		}

		offset = pos.m_offset + pos.m_length;
		line = pos.m_line;
	}
}

void
CStringTemplate::countLineCol (
	const char* p,
	size_t length
	)
{
	if (!p)
		return;

	if (length == -1)
		length = strlen (p);

	const char* end = p + length;
	const char* line = p;

	for (; p < end; p++)
		if (*p == '\n')
		{
			m_lineCol.m_line++;
			line = p + 1;
		}

	m_lineCol.m_col = (int) (p - line);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int 
CStringTemplate::getLine_lua (lua_State* h)
{
	CLuaState luaState = h;

	CStringTemplate* self = (CStringTemplate*) luaState.getContext ();

	luaState.pushInteger (self->m_lineCol.m_line);

	luaState.detach ();
	return 1;
}

int 
CStringTemplate::getCol_lua (lua_State* h)
{
	CLuaState luaState = h;

	CStringTemplate* self = (CStringTemplate*) luaState.getContext ();

	luaState.pushInteger (self->m_lineCol.m_col);

	luaState.detach ();
	return 1;
}

int 
CStringTemplate::emit_lua (lua_State* h)
{
	CLuaState luaState = h;

	TEmitContext* context = (TEmitContext*) luaState.getContext ();

	size_t count = luaState.getTop ();
	for (size_t i = 1; i <= count; i++)
	{
		const char* p = luaState.getString (i);
		context->m_output->append (p);
		context->m_this->countLineCol (p);
	}

	luaState.detach ();
	return 0;
}

int 
CStringTemplate::passthrough_lua (lua_State* h)
{
	CLuaState luaState = h;

	TEmitContext* context = (TEmitContext*) luaState.getContext ();

	size_t offset = luaState.getInteger (1);
	size_t length = luaState.getInteger (2);

	const char* p = context->m_source + offset;

	context->m_output->append (p, length);
	context->m_this->countLineCol (p, length);

	luaState.detach ();
	return 0;
}

//.............................................................................

} // namespace lua
} // namespace axl

