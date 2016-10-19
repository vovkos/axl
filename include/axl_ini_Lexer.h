// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_INI_LEXER_H

#include "axl_ini_Pch.h"

namespace axl {
namespace ini {

//..............................................................................

class Lexer
{
public:
	enum ScanResultKind
	{
		ScanResultKind_Error = -1,
		ScanResultKind_Eof   = 0,
		ScanResultKind_Section,
		ScanResultKind_KeyValue,
	};

protected:
	sl::String m_filePath;

	char* m_begin;
	int m_line;
	size_t m_lineOffset;

	sl::String m_sectionName;
	sl::String m_keyName;
	sl::String m_value;

	// ragel interface variables

	char* p;
	char* pe;
	char* eof;
	char* ts;
	char* te;
	int act;
	int cs;

protected: // should only be used as part of ini::CParser
	Lexer ()
	{
		reset ();
	}

	bool
	create (
		const sl::StringRef& filePath,
		const sl::StringRef& source
		);

	void
	parseSection (
		const char* p,
		const char* end
		);

	void
	parseKeyValue (
		const char* p,
		const char* end
		);

	ScanResultKind
	scanLine ();

	void
	setLineCol (
		int line,
		int col
		)
	{
		m_line = line;
		m_lineOffset = p - m_begin - col;
	}

	void
	setLineCol (const lex::LineCol& lineCol)
	{
		setLineCol (lineCol.m_line, lineCol.m_col);
	}

	void
	reset ();

protected:
	// these are to be called from withing ragel scanner (*.rl)

	void
	newLine (char* line);

	void
	stop ()
	{
		pe = p + 1;
	}

	// implemented in lexer.rl

	void
	init ();

	bool
	exec ();
};

//..............................................................................

} // namespace ini
} // namespace axl
