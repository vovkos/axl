// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LEX_TOKEN_H

#include "axl_err_Error.h"
#include "axl_rtl_String.h"
#include "axl_rtl_CharLiteral.h"

namespace axl {
namespace lex {

//.............................................................................

class CLineCol
{
public:
	int m_Line;
	int m_Col;

public:
	CLineCol ()
	{
		m_Line = 0;
		m_Col = 0;
	}

	CLineCol (
		int Line,
		int Col
		)
	{
		m_Line = Line;
		m_Col = Col;
	}

	void
	Clear ()
	{
		m_Line = 0;
		m_Col = 0;
	}
};

//.............................................................................

class CSrcPos: public CLineCol
{
public:
	rtl::CString m_FilePath;

public:
	CSrcPos ()
	{
	}

	CSrcPos (
		const rtl::CString& FilePath,
		const CLineCol& LineCol
		):
		CLineCol (LineCol)
	{
		m_FilePath = FilePath;
	}

	CSrcPos (
		const rtl::CString& FilePath,
		int Line,
		int Col
		):
		CLineCol (Line, Col)
	{
		m_FilePath = FilePath;
	}

	CSrcPos&
	operator = (const CLineCol& LineCol)
	{
		m_Line = LineCol.m_Line;
		m_Col = LineCol.m_Col;
		return *this;
	}
};

//.............................................................................

// std token should be enough for most implementations

class CStdTokenData
{
public:
	union
	{
		int m_Integer;
		int32_t m_Int32;
		uint32_t m_Int32_u;
		int64_t m_Int64;
		uint64_t m_Int64_u;
		float m_Float;
		double m_Double;
	};

	rtl::CString m_String;
	err::CError m_Error;

public:
	CStdTokenData ()
	{ 
		m_Int64 = 0;
	}
};

//.............................................................................

template <
	typename TEnum,
	typename TName,
	typename TData = CStdTokenData,
	typename TPos = CLineCol
	>
class CTokenT
{
public:
	typedef TEnum EToken;
	typedef TName CName;
	typedef TData CData;
	typedef TPos CPos;

public:
	union // it's nice to see it in debugger
	{
		int m_Token;
		EToken m_EnumToken;
		char m_CharToken;
		wchar_t m_WCharToken;
	};

	size_t m_Channel;
	CData m_Data;
	CPos m_Pos;
	
	// define token value in derived class

public:
	CTokenT ()
	{ 
		m_Token = 0;
		m_Channel = 0;
	}
		
	static
	const char*
	GetName (int Token)
	{
		return CName () (Token);
	}

	const char*
	GetName () const
	{
		return GetName (m_Token);
	}
};

//.............................................................................

#define AXL_PRS_BEGIN_TOKEN_NAME_MAP(Class) \
class Class \
{ \
public: \
	const char* \
	operator () (int Token) \
	{ \
		switch (Token) \
		{

#define AXL_PRS_TOKEN_NAME(Token, Name) \
		case Token: \
			return Name; 

#define AXL_PRS_END_TOKEN_NAME_MAP_EX(Default) \
		default: \
			return isprint (Token) ? axl::rtl::GetCharLiteral ((uchar_t) Token) : Default; \
		} \
	} \
};

#define AXL_PRS_END_TOKEN_NAME_MAP() \
	AXL_PRS_END_TOKEN_NAME_MAP_EX("??") // to avoid confusion with '?'

//.............................................................................

} // namespace lex
} // namespace axl

