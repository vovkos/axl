// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_lex_RagelLexer.h"
#include "axl_rtl_EscapeEncoding.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EToken
{
	// common tokens

	EToken_Eof = 0,
	EToken_Error = -1,
	EToken_Identifier = 256,
	EToken_Integer,
	EToken_Fp,
	EToken_Literal,

	// special literals

	EToken_HexLiteral,
	EToken_FmtLiteral,
	EToken_FmtSpecifier,

	// global declarations & pragmas

	EToken_Namespace,
	EToken_Using,
	EToken_Extend,
	EToken_Pack,

	// storage specifiers
		
	EToken_Typedef,
	EToken_Alias,
	EToken_Static,
	EToken_Thread,
	EToken_Stack,
	EToken_Heap,
	EToken_UHeap,
	EToken_Abstract,
	EToken_Virtual,
	EToken_Override,

	// access specifiers

	EToken_Public,
	EToken_Protected,
	EToken_Friend,

	// type modifiers

	EToken_Signed,
	EToken_Unsigned,
	EToken_BigEndian,
	EToken_Nullable,
	EToken_Const,
	EToken_ReadOnly,
	EToken_Mutable,
	EToken_Volatile,
	EToken_Weak,
	EToken_Thin,
	EToken_Unsafe,
	EToken_Cdecl,
	EToken_Stdcall,
	EToken_Function,
	EToken_Property,
	EToken_Multicast,
	EToken_Event,
	EToken_AutoEv,
	EToken_Bindable,
	EToken_AutoGet,
	EToken_Indexed,

	// type specifiers

	EToken_Auto,
	EToken_Void,
	EToken_Object,
	EToken_Variant,
	EToken_Bool,
	EToken_Int8,
	EToken_Int16,
	EToken_Int32,
	EToken_Int64,
	EToken_Float,
	EToken_Double,
	EToken_Char,
	EToken_Int,
	EToken_IntPtr,

	// named type specifiers

	EToken_Enum,
	EToken_EnumC,
	EToken_Struct,
	EToken_Union,
	EToken_Class,

	// special members

	EToken_Get,
	EToken_Set,
	EToken_This,
	EToken_PreConstruct,
	EToken_Construct,
	EToken_Destruct,
	EToken_Operator,
	EToken_Postfix,
	EToken_PropValue,
	EToken_OnChange,

	// statements

	EToken_If,
	EToken_Else,
	EToken_For,
	EToken_While,
	EToken_Do,
	EToken_Break,
	EToken_Continue,
	EToken_Return,
	EToken_Switch,
	EToken_Case,
	EToken_Default,
	EToken_Once,
	EToken_Silent,

	// constants

	EToken_True,
	EToken_False,
	EToken_Null,

	// keyword operators

	EToken_New,
	EToken_Delete,
	EToken_SizeOf,
	EToken_TypeOf,

	// symbol tokens

	EToken_Inc,
	EToken_Dec,
	EToken_Ptr,
	EToken_Shl,
	EToken_Shr,
	EToken_LogAnd,
	EToken_LogOr,
	EToken_Eq,
	EToken_Ne,
	EToken_Le,
	EToken_Ge,
	EToken_RefAssign,
	EToken_AddAssign,
	EToken_SubAssign,
	EToken_MulAssign,
	EToken_DivAssign,
	EToken_ModAssign,
	EToken_ShlAssign,
	EToken_ShrAssign,
	EToken_AndAssign,
	EToken_XorAssign,
	EToken_OrAssign,
	EToken_AtAssign,
	EToken_Ellipsis,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_PRS_BEGIN_TOKEN_NAME_MAP (CTokenName)

	// common tokens

	AXL_PRS_TOKEN_NAME (EToken_Eof,          "eof")
	AXL_PRS_TOKEN_NAME (EToken_Error,        "error")
	AXL_PRS_TOKEN_NAME (EToken_Identifier,   "identifier")
	AXL_PRS_TOKEN_NAME (EToken_Integer,      "integer-constant")
	AXL_PRS_TOKEN_NAME (EToken_Fp,           "floating-point-constant")
	AXL_PRS_TOKEN_NAME (EToken_Literal,      "string-literal")
	AXL_PRS_TOKEN_NAME (EToken_HexLiteral,   "hex-literal")
	AXL_PRS_TOKEN_NAME (EToken_FmtLiteral,   "fmt-literal")
	AXL_PRS_TOKEN_NAME (EToken_FmtSpecifier, "fmt-specifier")

	// global declarations & pragmas

	AXL_PRS_TOKEN_NAME (EToken_Namespace,    "namespace")
	AXL_PRS_TOKEN_NAME (EToken_Using,        "using")
	AXL_PRS_TOKEN_NAME (EToken_Extend,       "extend")
	AXL_PRS_TOKEN_NAME (EToken_Pack,         "pack")

	// storage specifiers

	AXL_PRS_TOKEN_NAME (EToken_Typedef,      "typedef")
	AXL_PRS_TOKEN_NAME (EToken_Alias,        "alias")
	AXL_PRS_TOKEN_NAME (EToken_Static,       "static")
	AXL_PRS_TOKEN_NAME (EToken_Thread,       "thread")
	AXL_PRS_TOKEN_NAME (EToken_Stack,        "stack")
	AXL_PRS_TOKEN_NAME (EToken_Heap,         "heap")
	AXL_PRS_TOKEN_NAME (EToken_UHeap,        "uheap")
	AXL_PRS_TOKEN_NAME (EToken_Abstract,     "abstract")
	AXL_PRS_TOKEN_NAME (EToken_Virtual,      "virtual")
	AXL_PRS_TOKEN_NAME (EToken_Override,     "override")

	// access specifiers

	AXL_PRS_TOKEN_NAME (EToken_Public,       "public")
	AXL_PRS_TOKEN_NAME (EToken_Protected,    "protected")
	AXL_PRS_TOKEN_NAME (EToken_Friend,       "friend")

	// storage modifiers

	AXL_PRS_TOKEN_NAME (EToken_Nullable,     "nullable")
	AXL_PRS_TOKEN_NAME (EToken_Mutable,      "mutable")

	// type modifiers

	AXL_PRS_TOKEN_NAME (EToken_Signed,       "signed")
	AXL_PRS_TOKEN_NAME (EToken_Unsigned,     "unsigned")
	AXL_PRS_TOKEN_NAME (EToken_BigEndian,    "bigendian")
	AXL_PRS_TOKEN_NAME (EToken_Const,        "const")
	AXL_PRS_TOKEN_NAME (EToken_ReadOnly,     "readonly")
	AXL_PRS_TOKEN_NAME (EToken_Volatile,     "volatile")
	AXL_PRS_TOKEN_NAME (EToken_Weak,         "weak")
	AXL_PRS_TOKEN_NAME (EToken_Thin,         "thin")
	AXL_PRS_TOKEN_NAME (EToken_Unsafe,       "unsafe")
	AXL_PRS_TOKEN_NAME (EToken_Cdecl,        "cdecl")
	AXL_PRS_TOKEN_NAME (EToken_Stdcall,      "stdcall")
	AXL_PRS_TOKEN_NAME (EToken_Function,     "function")
	AXL_PRS_TOKEN_NAME (EToken_Property,     "property")
	AXL_PRS_TOKEN_NAME (EToken_Multicast,    "multicast")	
	AXL_PRS_TOKEN_NAME (EToken_Event,        "event")	
	AXL_PRS_TOKEN_NAME (EToken_Bindable,     "bindable")
	AXL_PRS_TOKEN_NAME (EToken_AutoGet,      "autoget")
	AXL_PRS_TOKEN_NAME (EToken_Indexed,      "indexed")

	// type specifiers

	AXL_PRS_TOKEN_NAME (EToken_Auto,         "auto")
	AXL_PRS_TOKEN_NAME (EToken_Void,         "void")
	AXL_PRS_TOKEN_NAME (EToken_Object,       "object")
	AXL_PRS_TOKEN_NAME (EToken_Variant,      "variant")
	AXL_PRS_TOKEN_NAME (EToken_Bool,         "bool")
	AXL_PRS_TOKEN_NAME (EToken_Int8,         "int8")
	AXL_PRS_TOKEN_NAME (EToken_Int16,        "int16")
	AXL_PRS_TOKEN_NAME (EToken_Int32,        "int32")
	AXL_PRS_TOKEN_NAME (EToken_Int64,        "int64")
	AXL_PRS_TOKEN_NAME (EToken_Float,        "float")
	AXL_PRS_TOKEN_NAME (EToken_Double,       "double")
	AXL_PRS_TOKEN_NAME (EToken_Char,         "char")
	AXL_PRS_TOKEN_NAME (EToken_Int,          "int")
	AXL_PRS_TOKEN_NAME (EToken_IntPtr,       "intptr")

	// named type specifiers

	AXL_PRS_TOKEN_NAME (EToken_Enum,         "enum")
	AXL_PRS_TOKEN_NAME (EToken_EnumC,        "enumc")
	AXL_PRS_TOKEN_NAME (EToken_Struct,       "struct")
	AXL_PRS_TOKEN_NAME (EToken_Union,        "union")
	AXL_PRS_TOKEN_NAME (EToken_Class,        "class")
	AXL_PRS_TOKEN_NAME (EToken_AutoEv,       "autoev")

	// special members

	AXL_PRS_TOKEN_NAME (EToken_Get,          "get")
	AXL_PRS_TOKEN_NAME (EToken_Set,          "set")
	AXL_PRS_TOKEN_NAME (EToken_This,         "this")
	AXL_PRS_TOKEN_NAME (EToken_PreConstruct, "preconstruct")
	AXL_PRS_TOKEN_NAME (EToken_Construct,    "construct")
	AXL_PRS_TOKEN_NAME (EToken_Destruct,     "destruct")
	AXL_PRS_TOKEN_NAME (EToken_Operator,     "operator")
	AXL_PRS_TOKEN_NAME (EToken_Postfix,      "postfix")
	AXL_PRS_TOKEN_NAME (EToken_PropValue,    "propvalue")
	AXL_PRS_TOKEN_NAME (EToken_OnChange,     "onchange")

	// statements

	AXL_PRS_TOKEN_NAME (EToken_If,           "if")
	AXL_PRS_TOKEN_NAME (EToken_Else,         "else")
	AXL_PRS_TOKEN_NAME (EToken_For,          "for")
	AXL_PRS_TOKEN_NAME (EToken_While,        "while")
	AXL_PRS_TOKEN_NAME (EToken_Do,           "do")
	AXL_PRS_TOKEN_NAME (EToken_Break,        "break")
	AXL_PRS_TOKEN_NAME (EToken_Continue,     "continue")
	AXL_PRS_TOKEN_NAME (EToken_Return,       "return")
	AXL_PRS_TOKEN_NAME (EToken_Switch,       "switch")
	AXL_PRS_TOKEN_NAME (EToken_Case,         "case")
	AXL_PRS_TOKEN_NAME (EToken_Default,      "default")
	AXL_PRS_TOKEN_NAME (EToken_Once,         "once")
	AXL_PRS_TOKEN_NAME (EToken_Silent,       "silent")

	// constants

	AXL_PRS_TOKEN_NAME (EToken_True,         "true")
	AXL_PRS_TOKEN_NAME (EToken_False,        "false")
	AXL_PRS_TOKEN_NAME (EToken_Null,         "null")

	// keyword operators

	AXL_PRS_TOKEN_NAME (EToken_New,          "new")
	AXL_PRS_TOKEN_NAME (EToken_Delete,       "delete")
	AXL_PRS_TOKEN_NAME (EToken_SizeOf,       "sizeof")
	AXL_PRS_TOKEN_NAME (EToken_TypeOf,       "typeof")

	// symbol tokens

	AXL_PRS_TOKEN_NAME (EToken_Inc,          "++")
	AXL_PRS_TOKEN_NAME (EToken_Dec,          "--")
	AXL_PRS_TOKEN_NAME (EToken_Ptr,          "->")
	AXL_PRS_TOKEN_NAME (EToken_Shl,          "<<")
	AXL_PRS_TOKEN_NAME (EToken_Shr,          ">>")
	AXL_PRS_TOKEN_NAME (EToken_LogAnd,       "&&")	
	AXL_PRS_TOKEN_NAME (EToken_LogOr,        "||")
	AXL_PRS_TOKEN_NAME (EToken_Eq,           "==")
	AXL_PRS_TOKEN_NAME (EToken_Ne,           "!=")
	AXL_PRS_TOKEN_NAME (EToken_Le,           "<=")
	AXL_PRS_TOKEN_NAME (EToken_Ge,           ">=")
	AXL_PRS_TOKEN_NAME (EToken_RefAssign,    ":=")	
	AXL_PRS_TOKEN_NAME (EToken_AddAssign,    "+=")
	AXL_PRS_TOKEN_NAME (EToken_SubAssign,    "-=")
	AXL_PRS_TOKEN_NAME (EToken_MulAssign,    "*=")
	AXL_PRS_TOKEN_NAME (EToken_DivAssign,    "/=")
	AXL_PRS_TOKEN_NAME (EToken_ModAssign,    "%=")
	AXL_PRS_TOKEN_NAME (EToken_ShlAssign,    "<<=")
	AXL_PRS_TOKEN_NAME (EToken_ShrAssign,    ">>=")
	AXL_PRS_TOKEN_NAME (EToken_AndAssign,    "&=")
	AXL_PRS_TOKEN_NAME (EToken_XorAssign,    "^=")
	AXL_PRS_TOKEN_NAME (EToken_OrAssign,     "|=")	
	AXL_PRS_TOKEN_NAME (EToken_AtAssign,     "@=")	
	AXL_PRS_TOKEN_NAME (EToken_Ellipsis,     "...")
	
AXL_PRS_END_TOKEN_NAME_MAP ();	

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CTokenData: public lex::CStdTokenData
{
public:
	rtl::CArrayT <uchar_t> m_BinData;
};

typedef lex::CRagelTokenT <EToken, CTokenName, CTokenData> CToken;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CLexer: public lex::CRagelLexerT <CLexer, CToken>
{
	friend class lex::CRagelLexerT <CLexer, CToken>;

protected:
	CToken* m_pFmtLiteralToken;
	rtl::CArrayT <intptr_t> m_ParenthesesLevelStack;

public:
	CLexer ()
	{
		m_pFmtLiteralToken = NULL;
	}

protected:
	CToken*
	CreateStringToken (
		int Token,
		size_t Left = 0,
		size_t Right = 0,
		bool UseEscapeEncoding = false
		)
	{
		CToken* pToken = CreateToken (Token);
		ASSERT (pToken->m_Pos.m_Length >= Left + Right);

		size_t Length = pToken->m_Pos.m_Length - (Left + Right);
		if (UseEscapeEncoding)
			pToken->m_Data.m_String = rtl::CEscapeEncoding::Decode (ts + Left, Length);
		else
			pToken->m_Data.m_String.Copy (ts + Left, Length);

		return pToken;
	}

	CToken*
	CreateHexLiteralToken ()
	{
		CToken* pToken = CreateToken (EToken_HexLiteral);
		ASSERT (pToken->m_Pos.m_Length >= 4);
		pToken->m_Data.m_BinData = rtl::CHexEncoding::Decode (ts + 3, pToken->m_Pos.m_Length - 4);
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
		size_t Left = 0
		)
	{
		CToken* pToken = CreateToken (EToken_Integer);
		pToken->m_Data.m_Int64_u = _strtoui64 (ts + Left, NULL, Radix);
		return pToken;
	}

	CToken*
	CreateFpToken ()
	{
		CToken* pToken = CreateToken (EToken_Fp);
		pToken->m_Data.m_Double = strtod (ts, NULL);
		return pToken;
	}

	CToken*
	CreateConstIntegerToken (int Value)
	{
		CToken* pToken = CreateToken (EToken_Integer);
		pToken->m_Data.m_Integer = Value;
		return pToken;
	}

	// formatting literals

	CToken*
	PreCreateFmtLiteralToken ()
	{
		ASSERT (!m_pFmtLiteralToken);
		m_pFmtLiteralToken = PreCreateToken (0);
		return m_pFmtLiteralToken;
	}

	CToken*
	CreateFmtLiteralToken (int Token)
	{
		ASSERT (m_pFmtLiteralToken);
		CToken* pToken = m_pFmtLiteralToken;
		
		size_t Left = pToken->m_Pos.m_Length;
		size_t Right = te - ts;

		m_pFmtLiteralToken = NULL;

		pToken->m_Pos.m_Length = te - pToken->m_Pos.m_p;
		ASSERT (pToken->m_Pos.m_Length >= Left + Right);

		pToken->m_Token = Token;
		pToken->m_Data.m_String = rtl::CEscapeEncoding::Decode (
			pToken->m_Pos.m_p + Left, 
			pToken->m_Pos.m_Length - (Left + Right));
		return pToken;
	}

	CToken*
	CreateFmtSimpleIdentifierToken ()
	{
		CreateFmtLiteralToken (EToken_FmtLiteral); 

		// important: prevent Break () -- otherwise we could feed half-created fmt-literal token to the parser

		size_t PrevTokenizeLimit = m_TokenizeLimit; 
		m_TokenizeLimit = -1;

		CToken* pToken = CreateStringToken (EToken_Identifier, 1, 0); 

		m_TokenizeLimit = PrevTokenizeLimit; 

		PreCreateFmtLiteralToken (); 
		return pToken;
	}

	CToken*
	CreateFmtSpecifierToken ()
	{
		ASSERT (*ts == ',');
		ts++;

		while (ts < te && (*ts == ' ' || *ts == '\t'))
			ts++;

		return ts < te ? CreateStringToken (EToken_FmtSpecifier) : NULL;
	}

	void
	OnLeftParentheses ()
	{
		if (!m_ParenthesesLevelStack.IsEmpty ())
			m_ParenthesesLevelStack [m_ParenthesesLevelStack.GetCount () - 1]++;

		CreateToken ('('); 
	}

	bool
	OnRightParentheses ()
	{
		if (!m_ParenthesesLevelStack.IsEmpty ())
		{
			size_t i = m_ParenthesesLevelStack.GetCount () - 1;
			if (m_ParenthesesLevelStack [i] == 1)
			{
				m_ParenthesesLevelStack.Pop ();
				PreCreateFmtLiteralToken ();
				return false;
			}

			m_ParenthesesLevelStack [i]--;
		}

		CreateToken (')'); 
		return true;
	}

	bool
	OnComma ()
	{
		if (!m_ParenthesesLevelStack.IsEmpty ())
		{
			size_t i = m_ParenthesesLevelStack.GetCount () - 1;
			if (m_ParenthesesLevelStack [i] == 1)
			{
				ASSERT (*ts == ','); 
				p = ts - 1; // need to reparse colon with 'fmt_spec' machine
				return false;
			}
		}

		CreateToken (','); 
		return true;
	}

	void
	TerminateFmtSpecifier ()
	{
		ASSERT (!m_ParenthesesLevelStack.IsEmpty () && m_pFmtLiteralToken == NULL);
		m_ParenthesesLevelStack.Pop ();
		PreCreateFmtLiteralToken ();
	}

	void
	TerminateFmtLiteral ()
	{
		ASSERT (!m_ParenthesesLevelStack.IsEmpty () && m_pFmtLiteralToken == NULL);
		m_ParenthesesLevelStack.Pop ();
	}

	// implemented in *.rl

	void 
	Init ();

	bool
	Exec ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
