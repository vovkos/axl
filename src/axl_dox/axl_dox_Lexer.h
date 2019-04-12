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

#pragma once

#include "axl_dox_Pch.h"

namespace axl {
namespace dox {

//..............................................................................

enum TokenKind
{
	TokenKind_Eof = 0,
	TokenKind_Error = -1,
	TokenKind_Text = 256,
	TokenKind_CustomCommand,
	TokenKind_OpeningBrace,
	TokenKind_ClosingBrace,

	TokenKind_Import,
	TokenKind_Enum,
	TokenKind_EnumValue,
	TokenKind_Struct,
	TokenKind_Union,
	TokenKind_Class,
	TokenKind_Alias,
	TokenKind_Variable,
	TokenKind_Field,
	TokenKind_Function,
	TokenKind_Overload,
	TokenKind_Property,
	TokenKind_Event,
	TokenKind_Typedef,
	TokenKind_Namespace,
	TokenKind_Group,
	TokenKind_InGroup,
	TokenKind_SubGroup,
	TokenKind_Title,
	TokenKind_Brief,
	TokenKind_SeeAlso,
	TokenKind_Footnote,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_LEX_BEGIN_TOKEN_NAME_MAP(TokenName)

	AXL_LEX_TOKEN_NAME(TokenKind_Eof,           "eof")
	AXL_LEX_TOKEN_NAME(TokenKind_Error,         "error")
	AXL_LEX_TOKEN_NAME(TokenKind_Text,          "text")
	AXL_LEX_TOKEN_NAME(TokenKind_CustomCommand, "custom-command")
	AXL_LEX_TOKEN_NAME(TokenKind_OpeningBrace,  "@{")
	AXL_LEX_TOKEN_NAME(TokenKind_ClosingBrace,  "@}")

	AXL_LEX_TOKEN_NAME(TokenKind_Import,        "\\import")
	AXL_LEX_TOKEN_NAME(TokenKind_Enum,          "\\enum")
	AXL_LEX_TOKEN_NAME(TokenKind_EnumValue,     "\\enumvalue")
	AXL_LEX_TOKEN_NAME(TokenKind_Struct,        "\\struct")
	AXL_LEX_TOKEN_NAME(TokenKind_Union,         "\\union")
	AXL_LEX_TOKEN_NAME(TokenKind_Class,         "\\class")
	AXL_LEX_TOKEN_NAME(TokenKind_Alias,         "\\alias")
	AXL_LEX_TOKEN_NAME(TokenKind_Variable,      "\\variable")
	AXL_LEX_TOKEN_NAME(TokenKind_Field,         "\\field")
	AXL_LEX_TOKEN_NAME(TokenKind_Function,      "\\function")
	AXL_LEX_TOKEN_NAME(TokenKind_Overload,      "\\overload")
	AXL_LEX_TOKEN_NAME(TokenKind_Property,      "\\property")
	AXL_LEX_TOKEN_NAME(TokenKind_Event,         "\\event")
	AXL_LEX_TOKEN_NAME(TokenKind_Typedef,       "\\typedef")
	AXL_LEX_TOKEN_NAME(TokenKind_Namespace,     "\\namespace")
	AXL_LEX_TOKEN_NAME(TokenKind_Group,         "\\group")
	AXL_LEX_TOKEN_NAME(TokenKind_InGroup,       "\\ingroup")
	AXL_LEX_TOKEN_NAME(TokenKind_SubGroup,      "\\subgroup")
	AXL_LEX_TOKEN_NAME(TokenKind_Title,         "\\title")
	AXL_LEX_TOKEN_NAME(TokenKind_Brief,         "\\brief")
	AXL_LEX_TOKEN_NAME(TokenKind_SeeAlso,       "\\see")
	AXL_LEX_TOKEN_NAME(TokenKind_Footnote,      "\\footnote")

AXL_LEX_END_TOKEN_NAME_MAP();

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef lex::RagelToken<TokenKind, TokenName, lex::StdTokenData> DoxyToken;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Lexer: public lex::RagelLexer<Lexer, DoxyToken>
{
	friend class lex::RagelLexer<Lexer, DoxyToken>;

protected:
	DoxyToken*
	createTextToken(
		TokenKind tokenKind,
		size_t left = 0
		);

	DoxyToken*
	createNewLineToken();

	// implemented in *.rl

	void
	init();

	void
	exec();
};

//..............................................................................

} // namespace dox
} // namespace axl
