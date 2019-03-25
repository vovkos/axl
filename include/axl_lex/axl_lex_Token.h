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

#define _AXL_LEX_TOKEN_H

#include "axl_lex_Pch.h"

namespace axl {
namespace lex {

//..............................................................................

struct LineCol
{
	int m_line;
	int m_col;

	LineCol()
	{
		m_line = 0;
		m_col = 0;
	}

	LineCol(
		int line,
		int col
		)
	{
		m_line = line;
		m_col = col;
	}

	void
	clear()
	{
		m_line = 0;
		m_col = 0;
	}

	void
	count(const sl::StringRef& string)
	{
		clear();
		incrementalCount(string);
	}

	void
	incrementalCount(const sl::StringRef& string);
};

//..............................................................................

struct SrcPos: LineCol
{
	sl::StringRef m_filePath;

	SrcPos()
	{
	}

	SrcPos(
		const sl::StringRef& filePath,
		const LineCol& lineCol
		):
		LineCol(lineCol)
	{
		m_filePath = filePath;
	}

	SrcPos(
		const sl::StringRef& filePath,
		int line,
		int col
		):
		LineCol(line, col)
	{
		m_filePath = filePath;
	}

	SrcPos&
	operator = (const LineCol& lineCol)
	{
		*(LineCol*)this = lineCol;
		return *this;
	}
};

//..............................................................................

// std token should be enough for most implementations

struct StdTokenData
{
	union
	{
		//! \unnamed{union}
		int m_integer;
		int32_t m_int32;
		uint32_t m_int32_u;
		int64_t m_int64;
		uint64_t m_int64_u;
		float m_float;
		double m_double;
	};

	sl::StringRef m_string;

	StdTokenData()
	{
		m_int64 = 0;
	}
};

//..............................................................................

enum TokenChannelMask
{
	TokenChannelMask_Main = 1,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TokenKind0,
	typename Name0,
	typename Data0 = StdTokenData,
	typename Pos0 = LineCol
	>
struct Token
{
	typedef TokenKind0 TokenKind;
	typedef Name0 Name;
	typedef Data0 Data;
	typedef Pos0 Pos;

	union // it's nice to see it in debugger
	{
		//! \unnamed{union:4}
		int m_token;
		TokenKind m_tokenKind;
		char m_charToken;
		wchar_t m_WCharToken;
	};

	uint_t m_channelMask;

	Data m_data;
	Pos m_pos;

	// define token value in derived class

	Token()
	{
		m_token = 0;
		m_channelMask = TokenChannelMask_Main;
	}

	static
	const char*
	getName(int token)
	{
		return Name() (token);
	}

	const char*
	getName() const
	{
		return getName(m_token);
	}
};

//..............................................................................

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
			return isprint((uchar_t)tokenKind) ? axl::sl::getCharLiteral((uchar_t)tokenKind) : unknownName; \
		} \
	} \
};

#define AXL_LEX_END_TOKEN_NAME_MAP() \
	AXL_LEX_END_TOKEN_NAME_MAP_EX("??") // to avoid confusion with '?'

//..............................................................................

} // namespace lex
} // namespace axl
