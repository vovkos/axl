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

#include "pch.h"
#include "axl_ini_Lexer.h"
#include "axl_ini_Lexer.rl.cpp"

namespace axl {
namespace ini {

//..............................................................................

bool
Lexer::create(
	const sl::StringRef& filePath,
	const sl::StringRef& source
) {
	this->reset();

	init();

	p   = (char*)source.cp();
	eof = (char*)source.getEnd();

	m_filePath = filePath;
	m_begin = p;
	return true;
}

void
Lexer::parseSection(
	const char* p,
	const char* end
) {
	ASSERT(*p == '[' && end[-1] == ']');

	if (m_isLineContinuation) {
		err::setError("incomplete multi-line value");
		lex::pushSrcPosError(m_filePath, m_line, p - m_begin - m_lineOffset);
		m_scanResultKind = ScanResultKind_Error;
		return;
	}

	p++;
	end--;

	while (p < end && isspace(*p))
		p++;

	while (p < end && isspace(end[-1]))
		end--;

	if (p < end)
		m_sectionName.copy(p, end - p);

	m_scanResultKind = ScanResultKind_Section;
}

void
Lexer::parseKeyValue(
	const char* p,
	const char* end
) {
	ASSERT(!isspace(*p));

	if (m_isLineContinuation) {
		parseValue(p, end);
		return;
	}

	const char* p0 = p;

	while (p < end  && *p != '=' && !isspace(*p))
		p++;

	m_keyName.copy(p0, p - p0);

	while (p < end && isspace(*p))
		p++;

	if (*p == '=') {
		p++;
		parseValue(p, end);
	} else {
		m_scanResultKind = ScanResultKind_KeyValue;
	}
}

void
Lexer::parseValue(
	const char* p,
	const char* end
) {
	while (p < end && isspace(*p))
		p++;

	while (p < end && isspace(end[-1]))
		end--;

	bool isLineContinuation = p < end && end[-1] == '\\';
	if (isLineContinuation) {
		end--;
		while (p < end && isspace(end[-1]))
			end--;
	}

	size_t length = end - p;

	sl::StringRef value = length >= 2 && p[0] == '"' && end[-1] == '"' ?
		sl::StringRef(enc::EscapeEncoding::decode(sl::StringRef(p + 1, length - 2))) :
		sl::StringRef(p, length);

	if (m_isLineContinuation)
		m_value += value;
	else
		m_value = value;

	m_scanResultKind = isLineContinuation ? ScanResultKind_LineContinuation : ScanResultKind_KeyValue;
	m_isLineContinuation = isLineContinuation;
}

Lexer::ScanResultKind
Lexer::scanLine() {
	if (p == eof)
		return ScanResultKind_Eof;

	pe = eof;
	m_sectionName.clear();

	if (!m_isLineContinuation) {
		m_keyName.clear();
		m_value.clear();
	}

	m_scanResultKind = ScanResultKind_Eof; // assume eof

	bool result = exec();
	if (!result) {
		err::setError("invalid syntax");
		lex::pushSrcPosError(m_filePath, m_line, p - m_begin - m_lineOffset);
		return ScanResultKind_Error;
	}

	return m_scanResultKind;
}

void
Lexer::reset() {
	p = NULL;
	pe = NULL;
	eof = NULL;
	ts = NULL;
	te = NULL;
	act = 0;
	cs = 0;

	m_begin = NULL;
	m_line = 0;
	m_lineOffset = 0;
	m_isLineContinuation = false;

	m_filePath.clear();
	m_sectionName.clear();
	m_keyName.clear();
	m_value.clear();
	m_scanResultKind = ScanResultKind_Eof;
}

void
Lexer::newLine(char* line) {
	ASSERT(line[-1] == '\n');

	m_lineOffset = line - m_begin;
	m_line++;
}

//..............................................................................

} // namespace ini
} // namespace axl
