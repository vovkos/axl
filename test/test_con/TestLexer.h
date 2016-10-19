#pragma once

//..............................................................................

enum TokenKind
{
	TokenKind_Eof = 0,
	TokenKind_Error = -1,
	TokenKind_Identifier = 256,
	TokenKind_Integer,
	TokenKind_Double,
	TokenKind_Literal,
	TokenKind_Comment,
	TokenKind_Ne,
	TokenKind_Le,
	TokenKind_Ge,

	// keywords

	TokenKind_Message,

	TokenKind_STRING,
	TokenKind_WSTRING,
	TokenKind_BOOLEAN,
	TokenKind_BYTE,
	TokenKind_CHAR,
	TokenKind_WORD,
	TokenKind_SHORT,
	TokenKind_DWORD,
	TokenKind_LONG,
	TokenKind_REAL,
	TokenKind_ENUM,
	TokenKind_STRUCT,
	TokenKind_DOEVENTS,
	TokenKind_CONST,
	TokenKind_DIM,
	TokenKind_AS,
	TokenKind_END,
	TokenKind_DECLARE,
	TokenKind_FUNCTION,
	TokenKind_SUB,
	TokenKind_GOTO,
	TokenKind_GOSUB,
	TokenKind_RETURN,
	TokenKind_SELECT,
	TokenKind_CASE,
	TokenKind_IF,
	TokenKind_THEN,
	TokenKind_ELSE,
	TokenKind_FOR,
	TokenKind_TO,
	TokenKind_STEP,
	TokenKind_NEXT,
	TokenKind_EXIT,
	TokenKind_WHILE,
	TokenKind_WEND,
	TokenKind_DO,
	TokenKind_LOOP,
	TokenKind_UNTIL,
	TokenKind_BYREF,
	TokenKind_BYVAL,
	TokenKind_XOR,
	TokenKind_OR,
	TokenKind_AND,
	TokenKind_NOT,
	TokenKind_MOD,
	TokenKind_OBJECT,
	TokenKind_SYSCALL,
	TokenKind_EVENT,
	TokenKind_PROPERTY,
	TokenKind_GET,
	TokenKind_SET,
	TokenKind_REF,
	TokenKind_SIZEOF,
	TokenKind_INCLUDE,
	TokenKind_PUBLIC,
	TokenKind_STATIC,
	TokenKind_LIBRARY,

	TokenKind_PPLINE,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_PRS_BEGIN_TOKEN_NAME_MAP (TokenName)
	AXL_PRS_TOKEN_NAME (TokenKind_Eof,        "eof")
	AXL_PRS_TOKEN_NAME (TokenKind_Error,      "error")
	AXL_PRS_TOKEN_NAME (TokenKind_Identifier, "identifier")
	AXL_PRS_TOKEN_NAME (TokenKind_Integer,    "integer-constant")
	AXL_PRS_TOKEN_NAME (TokenKind_Double,     "double-constant")
	AXL_PRS_TOKEN_NAME (TokenKind_Literal,    "string-literal")
	AXL_PRS_TOKEN_NAME (TokenKind_Comment,    "comment")
	AXL_PRS_TOKEN_NAME (TokenKind_Ne,         "<>")
	AXL_PRS_TOKEN_NAME (TokenKind_Le,         "<=")
	AXL_PRS_TOKEN_NAME (TokenKind_Ge,         ">=")
	AXL_PRS_TOKEN_NAME (TokenKind_Message,    "message")

	AXL_PRS_TOKEN_NAME (TokenKind_STRING,     "string")
	AXL_PRS_TOKEN_NAME (TokenKind_WSTRING,    "wstring")
	AXL_PRS_TOKEN_NAME (TokenKind_BOOLEAN,    "boolean")
	AXL_PRS_TOKEN_NAME (TokenKind_BYTE,       "byte")
	AXL_PRS_TOKEN_NAME (TokenKind_CHAR,       "char")
	AXL_PRS_TOKEN_NAME (TokenKind_WORD,       "word")
	AXL_PRS_TOKEN_NAME (TokenKind_SHORT,      "short")
	AXL_PRS_TOKEN_NAME (TokenKind_DWORD,      "dword")
	AXL_PRS_TOKEN_NAME (TokenKind_LONG,       "long")
	AXL_PRS_TOKEN_NAME (TokenKind_REAL,       "real")
	AXL_PRS_TOKEN_NAME (TokenKind_ENUM,       "enum")
	AXL_PRS_TOKEN_NAME (TokenKind_STRUCT,     "struct")
	AXL_PRS_TOKEN_NAME (TokenKind_DOEVENTS,   "doevents")
	AXL_PRS_TOKEN_NAME (TokenKind_CONST,      "const")
	AXL_PRS_TOKEN_NAME (TokenKind_DIM,        "dim")
	AXL_PRS_TOKEN_NAME (TokenKind_AS,         "as")
	AXL_PRS_TOKEN_NAME (TokenKind_END,        "end")
	AXL_PRS_TOKEN_NAME (TokenKind_DECLARE,    "declare")
	AXL_PRS_TOKEN_NAME (TokenKind_FUNCTION,   "function")
	AXL_PRS_TOKEN_NAME (TokenKind_SUB,        "sub")
	AXL_PRS_TOKEN_NAME (TokenKind_GOTO,       "goto")
	AXL_PRS_TOKEN_NAME (TokenKind_GOSUB,      "gosub")
	AXL_PRS_TOKEN_NAME (TokenKind_RETURN,     "return")
	AXL_PRS_TOKEN_NAME (TokenKind_SELECT,     "select")
	AXL_PRS_TOKEN_NAME (TokenKind_CASE,       "case")
	AXL_PRS_TOKEN_NAME (TokenKind_IF,         "if")
	AXL_PRS_TOKEN_NAME (TokenKind_THEN,       "then")
	AXL_PRS_TOKEN_NAME (TokenKind_ELSE,       "else")
	AXL_PRS_TOKEN_NAME (TokenKind_FOR,        "for")
	AXL_PRS_TOKEN_NAME (TokenKind_TO,         "to")
	AXL_PRS_TOKEN_NAME (TokenKind_STEP,       "step")
	AXL_PRS_TOKEN_NAME (TokenKind_NEXT,       "next")
	AXL_PRS_TOKEN_NAME (TokenKind_EXIT,       "exit")
	AXL_PRS_TOKEN_NAME (TokenKind_WHILE,      "while")
	AXL_PRS_TOKEN_NAME (TokenKind_WEND,       "wend")
	AXL_PRS_TOKEN_NAME (TokenKind_DO,         "do")
	AXL_PRS_TOKEN_NAME (TokenKind_LOOP,       "loop")
	AXL_PRS_TOKEN_NAME (TokenKind_UNTIL,      "until")
	AXL_PRS_TOKEN_NAME (TokenKind_BYREF,      "byref")
	AXL_PRS_TOKEN_NAME (TokenKind_BYVAL,      "byval")
	AXL_PRS_TOKEN_NAME (TokenKind_XOR,        "xor")
	AXL_PRS_TOKEN_NAME (TokenKind_OR,         "or")
	AXL_PRS_TOKEN_NAME (TokenKind_AND,        "and")
	AXL_PRS_TOKEN_NAME (TokenKind_NOT,        "not")
	AXL_PRS_TOKEN_NAME (TokenKind_MOD,        "mod")
	AXL_PRS_TOKEN_NAME (TokenKind_OBJECT,     "object")
	AXL_PRS_TOKEN_NAME (TokenKind_SYSCALL,    "syscall")
	AXL_PRS_TOKEN_NAME (TokenKind_EVENT,      "event")
	AXL_PRS_TOKEN_NAME (TokenKind_PROPERTY,   "property")
	AXL_PRS_TOKEN_NAME (TokenKind_GET,        "get")
	AXL_PRS_TOKEN_NAME (TokenKind_SET,        "set")
	AXL_PRS_TOKEN_NAME (TokenKind_REF,        "ref")
	AXL_PRS_TOKEN_NAME (TokenKind_SIZEOF,     "sizeof")
	AXL_PRS_TOKEN_NAME (TokenKind_INCLUDE,    "incldue")
	AXL_PRS_TOKEN_NAME (TokenKind_PUBLIC,     "public")
	AXL_PRS_TOKEN_NAME (TokenKind_STATIC,     "static")
	AXL_PRS_TOKEN_NAME (TokenKind_LIBRARY,    "library")
	AXL_PRS_TOKEN_NAME (TokenKind_PPLINE,     "line")

AXL_PRS_END_TOKEN_NAME_MAP ()

//..............................................................................

typedef axl::lex::RagelToken <TokenKind, TokenName> Token;

class Lexer: public axl::lex::RagelLexer <Lexer, Token>
{
	friend class axl::lex::RagelLexer <Lexer, Token>;

protected:
	Token*
	createIntegerToken (
		int begin,
		int base
		)
	{
		Token* token = createToken (TokenKind_Integer);
		token->m_data.m_integer = strtoul(ts + begin, NULL, base);
		return token;
	}

	Token*
	createDoubleToken ()
	{
		Token* token = createToken (TokenKind_Double);
		token->m_data.m_double = strtod (ts, NULL);
		return token;
	}

	Token*
	createStringToken(
		int begin,
		int end,
		TokenKind token,
		size_t channel = 0
		)
	{
		Token* token = createToken (token, channel);
		token->m_data.m_string.copy (ts + begin, token->m_pos.m_length - begin - end);
		return token;
	}

	uchar_t
	getHexValue (char c)
	{
		return
			c >= '0' && c <= '9' ? c - '0' :
			c >= 'a' && c <= 'f' ? 10 + c - 'a' :
			c >= 'A' && c <= 'F' ? 10 + c - 'A' : 0;
	}

	Token*
	createCharConstToken(
		int begin,
		int end
		)
	{
		Token* token = createToken (TokenKind_Integer);
		token->m_data.m_int64 = 0;

		if (!token->m_pos.m_length)
			return token;

		size_t length = token->m_pos.m_length - begin - end;
		if (length > 16) // int64
			length = 16;

		char* p = ts + begin;
		int base = 1 << (length - 1) * 4;

		for (; base; p++, base >>= 4)
			token->m_data.m_int64 += base * getHexValue (*p);

		return token;
	}

	Token*
	createIntegerConstToken (int value)
	{
		Token* token = createToken (TokenKind_Integer);
		token->m_data.m_integer = value;
		return token;
	}

	// implemented in TestLexer.rl

	void
	init ();

	bool
	exec ();
};

//..............................................................................
