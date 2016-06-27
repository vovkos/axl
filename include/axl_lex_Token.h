// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LEX_TOKEN_H

#include "axl_err_Error.h"
#include "axl_sl_String.h"
#include "axl_sl_CharLiteral.h"

namespace axl {
namespace lex {

//.............................................................................

class LineCol
{
public:
	int m_line;
	int m_col;

public:
	LineCol ()
	{
		m_line = 0;
		m_col = 0;
	}

	LineCol (
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

	void
	count (
		const char* p,
		size_t length = -1
		)
	{
		clear ();
		incrementalCount (p, length);
	}

	void
	incrementalCount (
		const char* p,
		size_t length = -1
		);
};

//.............................................................................

class SrcPos: public LineCol
{
public:
	sl::String m_filePath;

public:
	SrcPos ()
	{
	}

	SrcPos (
		const sl::String& filePath,
		const LineCol& lineCol
		):
		LineCol (lineCol)
	{
		m_filePath = filePath;
	}

	SrcPos (
		const sl::String& filePath,
		int line,
		int col
		):
		LineCol (line, col)
	{
		m_filePath = filePath;
	}

	SrcPos&
	operator = (const LineCol& lineCol)
	{
		*(LineCol*) this = lineCol;
		return *this;
	}
};

//.............................................................................

// std token should be enough for most implementations

class StdTokenData
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

	sl::String m_string;

public:
	StdTokenData ()
	{
		m_int64 = 0;
	}
};

//.............................................................................

template <
	typename TokenKind_0,
	typename Name_0,
	typename Data_0 = StdTokenData,
	typename Pos_0 = LineCol
	>
class Token
{
public:
	typedef TokenKind_0 TokenKind;
	typedef Name_0 Name;
	typedef Data_0 Data;
	typedef Pos_0 Pos;

public:
	union // it's nice to see it in debugger
	{
		int m_token;
		TokenKind m_enumToken;
		char m_charToken;
		wchar_t m_WCharToken;
	};

	size_t m_channel;
	Data m_data;
	Pos m_pos;

	// define token value in derived class

public:
	Token ()
	{
		m_token = 0;
		m_channel = 0;
	}

	static
	const char*
	getName (int token)
	{
		return Name () (token);
	}

	const char*
	getName () const
	{
		return getName (m_token);
	}
};

//.............................................................................

#define AXL_LEX_BEGIN_TOKEN_NAME_MAP(Class) \
class Class \
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
			return isprint ((uchar_t) tokenKind) ? axl::sl::getCharLiteral ((uchar_t) tokenKind) : unknownName; \
		} \
	} \
};

#define AXL_LEX_END_TOKEN_NAME_MAP() \
	AXL_LEX_END_TOKEN_NAME_MAP_EX("??") // to avoid confusion with '?'

//.............................................................................

} // namespace lex
} // namespace axl

