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

#define _AXL_LEX_SRCPOS_H

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

struct LineColOffset: public LineCol
{
	size_t m_offset;

	LineColOffset()
	{
		m_offset = 0;
	}

	LineColOffset(
		int line,
		int col,
		size_t offset
		):
		LineCol(line, col)
	{
		m_offset = offset;
	}

	void
	clear()
	{
		LineCol::clear();
		m_offset = 0;
	}

	void
	count(const sl::StringRef& string)
	{
		LineCol::count(string);
		m_offset = string.getLength();
	}

	void
	incrementalCount(const sl::StringRef& string)
	{
		LineCol::incrementalCount(string);
		m_offset += string.getLength();
	}
};

//..............................................................................

} // namespace lex
} // namespace axl
