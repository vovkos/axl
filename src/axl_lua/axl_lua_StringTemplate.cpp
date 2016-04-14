#include "pch.h"
#include "axl_lua_StringTemplate.h"
#include "axl_lua_StringTemplateLexer.h"

namespace axl {
namespace lua {

//.............................................................................

void
StringTemplate::reset ()
{
	m_luaState.create ();
	m_lineCol.clear ();
}

bool
StringTemplate::process (
	sl::String* output,
	const sl::String& filePath,
	const char* source,
	size_t length
	)
{
	bool result;

	output->clear ();

	sl::String luaSource;
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
			
	EmitContext emitContext;
	emitContext.m_self = this;
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
	Lexer* lexer,
	lex::RagelTokenPos* endPos_o
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

	*endPos_o = token->m_pos;

	lexer->gotoState (
		Lexer::getMachineState (LexerMachineKind_Main), 
		token, 
		Lexer::GotoStateKind_EatToken
		);

	return true;
}

bool
StringTemplate::extractLuaSource (
	sl::String* luaSource,
	const sl::String& filePath,
	const char* source,
	size_t length
	)
{
	bool result;

	Lexer lexer;
	lexer.create (filePath, source, length);

	luaSource->clear ();
	
	size_t offset = 0;
	Token::Pos pos;

	int line = 0;

	for (;;)
	{
		const Token* token = lexer.getToken ();
		
		if (token->m_token == TokenKind_Error)
		{
			err::setFormatStringError ("invalid character '\\x%02x'", (uchar_t) token->m_data.m_integer);
			return false;
		}

		if (token->m_pos.m_offset > offset)
			luaSource->appendFormat ("Passthrough (%d, %d);", offset, token->m_pos.m_offset - offset);

		if (token->m_token == TokenKind_Eof)
			return true;

		if (token->m_pos.m_line > line)
			luaSource->append ('\n', token->m_pos.m_line - line);

		offset = token->m_pos.m_offset + token->m_pos.m_length;

		switch (token->m_token)
		{
		case TokenKind_Data:
			luaSource->appendFormat ("Emit (%s);", token->m_data.m_string.cc ());
			pos = token->m_pos;
			lexer.nextToken ();
			break;

		case TokenKind_OpenCode:
			result = extractUserCode (&lexer, &pos);
			if (!result)
				return false;

			luaSource->append (source + offset, pos.m_offset - offset);
			luaSource->append (";");
			break;

		case TokenKind_OpenData:
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
StringTemplate::countLineCol (
	const char* p,
	size_t length
	)
{
	if (length == -1)
		length = axl_strlen (p);

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
StringTemplate::getLine_lua (lua_State* h)
{
	LuaState luaState = h;

	StringTemplate* self = (StringTemplate*) luaState.getContext ();

	luaState.pushInteger (self->m_lineCol.m_line);

	luaState.detach ();
	return 1;
}

int 
StringTemplate::getCol_lua (lua_State* h)
{
	LuaState luaState = h;

	StringTemplate* self = (StringTemplate*) luaState.getContext ();

	luaState.pushInteger (self->m_lineCol.m_col);

	luaState.detach ();
	return 1;
}

int 
StringTemplate::emit_lua (lua_State* h)
{
	LuaState luaState = h;

	EmitContext* context = (EmitContext*) luaState.getContext ();

	size_t count = luaState.getTop ();
	for (size_t i = 1; i <= count; i++)
	{
		const char* p = luaState.getString (i);
		context->m_output->append (p);
		context->m_self->countLineCol (p);
	}

	luaState.detach ();
	return 0;
}

int 
StringTemplate::passthrough_lua (lua_State* h)
{
	LuaState luaState = h;

	EmitContext* context = (EmitContext*) luaState.getContext ();

	size_t offset = luaState.getInteger (1);
	size_t length = luaState.getInteger (2);

	const char* p = context->m_source + offset;

	context->m_output->append (p, length);
	context->m_self->countLineCol (p, length);

	luaState.detach ();
	return 0;
}

//.............................................................................

} // namespace lua
} // namespace axl

