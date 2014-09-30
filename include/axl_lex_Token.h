// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
	int m_line;
	int m_col;

public:
	CLineCol ()
	{
		m_line = 0;
		m_col = 0;
	}

	CLineCol (
		int line,
		int col
		)
	{
		m_line = line;
		m_col = col;
	}

	void
	clear ()
	{
		m_line = 0;
		m_col = 0;
	}
};

//.............................................................................

class CSrcPos: public CLineCol
{
public:
	rtl::CString m_filePath;

public:
	CSrcPos ()
	{
	}

	CSrcPos (
		const rtl::CString& filePath,
		const CLineCol& lineCol
		):
		CLineCol (lineCol)
	{
		m_filePath = filePath;
	}

	CSrcPos (
		const rtl::CString& filePath,
		int line,
		int col
		):
		CLineCol (line, col)
	{
		m_filePath = filePath;
	}

	CSrcPos&
	operator = (const CLineCol& lineCol)
	{
		m_line = lineCol.m_line;
		m_col = lineCol.m_col;
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
		int m_integer;
		int32_t m_int32;
		uint32_t m_int32_u;
		int64_t m_int64;
		uint64_t m_int64_u;
		float m_float;
		double m_double;
	};

	rtl::CString m_string;

public:
	CStdTokenData ()
	{ 
		m_int64 = 0;
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
		int m_token;
		EToken m_enumToken;
		char m_charToken;
		wchar_t m_WCharToken;
	};

	size_t m_channel;
	CData m_data;
	CPos m_pos;
	
	// define token value in derived class

public:
	CTokenT ()
	{ 
		m_token = 0;
		m_channel = 0;
	}
		
	static
	const char*
	getName (int token)
	{
		return CName () (token);
	}

	const char*
	getName () const
	{
		return getName (m_token);
	}
};

//.............................................................................

#define AXL_LEX_BEGIN_TOKEN_NAME_MAP(CClass) \
class CClass \
{ \
public: \
	const char* \
	operator () (int tokenKind) \
	{ \
		switch (tokenKind) \
		{

#define AXL_LEX_TOKEN_NAME(tokenKind, name) \
		case tokenKind: \
			return name; 

#define AXL_LEX_END_TOKEN_NAME_MAP_EX(unknownName) \
		default: \
			return isprint ((uchar_t) tokenKind) ? axl::rtl::getCharLiteral ((uchar_t) tokenKind) : unknownName; \
		} \
	} \
};

#define AXL_LEX_END_TOKEN_NAME_MAP() \
	AXL_LEX_END_TOKEN_NAME_MAP_EX("??") // to avoid confusion with '?'

//.............................................................................

} // namespace lex
} // namespace axl

