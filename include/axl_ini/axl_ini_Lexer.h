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

#define _AXL_INI_LEXER_H

#include "axl_ini_Pch.h"

namespace axl {
namespace ini {

//..............................................................................

class Lexer: lex::Ragel {
public:
	enum ScanResultKind {
		ScanResultKind_Error = -1,
		ScanResultKind_Eof   = 0,
		ScanResultKind_Section,
		ScanResultKind_KeyValue,
		ScanResultKind_LineContinuation,
	};

protected:
	sl::String m_filePath;

	int m_line;
	size_t m_lineOffset;
	bool m_isLineContinuation;

	sl::String m_sectionName;
	sl::String m_keyName;
	sl::String m_value;

	ScanResultKind m_scanResultKind;

protected: // should only be used as part of ini::CParser
	Lexer() {
		reset();
	}

	bool
	create(
		const sl::StringRef& filePath,
		const sl::StringRef& source
	);

	void
	parseSection(
		const char* p,
		const char* end
	);

	void
	parseKeyValue(
		const char* p,
		const char* end
	);

	void
	parseValue(
		const char* p,
		const char* end
	);

	ScanResultKind
	scanLine();

	void
	setLineCol(
		int line,
		int col
	) {
		m_line = line;
		m_lineOffset = p - m_begin - col;
	}

	void
	setLineCol(const lex::LineCol& lineCol) {
		setLineCol(lineCol.m_line, lineCol.m_col);
	}

	void
	reset();

protected:
	// these are to be called from withing ragel scanner (*.rl)

	void
	newLine(char* line);

	// implemented in lexer.rl

	void
	init();

	bool
	exec();
};

//..............................................................................

} // namespace ini
} // namespace axl
