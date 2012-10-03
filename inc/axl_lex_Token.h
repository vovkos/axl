// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LEX_TOKEN_H

#include "axl_err_Error.h"
#include "axl_rtl_CharLiteral.h"

namespace axl {
namespace lex {

//.............................................................................

class CLineCol
{
public:
	long m_Line;
	long m_Col;

public:
	CLineCol ()
	{
		m_Line = 0;
		m_Col = 0;
	}

	CLineCol (
		long Line,
		long Col
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
		long Line,
		long Col
		):
		CLineCol (Line, Col)
	{
		m_FilePath = FilePath;
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
		__int64 m_Int64;
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

