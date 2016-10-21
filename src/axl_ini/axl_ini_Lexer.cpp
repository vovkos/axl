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
Lexer::create (
	const sl::StringRef& filePath,
	const sl::StringRef& source
	)
{
	this->reset ();

	init ();

	p   = (char*) source.cp ();
	eof = (char*) source.getEnd ();

	m_filePath = filePath;
	m_begin = p;

	return true;
}

void
Lexer::parseSection (
	const char* p,
	const char* end
	)
{
	ASSERT (*p == '[');

	if (end [-1] != ']')
		return;

	p++;
	end--;

	while (p < end && isspace (*p))
		p++;

	while (p < end && isspace (end [-1]))
		end--;

	if (p < end)
	{
		m_sectionName.copy (p, end - p);
		stop ();
	}
}

void
Lexer::parseKeyValue (
	const char* p,
	const char* end
	)
{
	ASSERT (!isspace (*p));

	const char* p0 = p;

	while (p < end && !isspace (*p))
		p++;

	m_keyName.copy (p0, p - p0);

	while (p < end && isspace (*p))
		p++;

	if (*p == '=')
	{
		p++;

		while (p < end && isspace (*p))
			p++;

		while (p < end && isspace (end [-1]))
			end--;

		if (p < end)
		{
			size_t length = end - p;
			m_value.copy (p, length);

			if (m_value [0] == '"' && m_value [length - 1] == '"')
				m_value = enc::EscapeEncoding::decode (m_value.getSubString (1, length - 2));
		}
	}

	stop ();
}

Lexer::ScanResultKind
Lexer::scanLine ()
{
	if (p == eof)
		return ScanResultKind_Eof;

	pe = eof;
	m_sectionName.clear ();
	m_keyName.clear ();
	m_value.clear ();

	bool result = exec ();
	if (!result)
	{
		err::setError ("invalid syntax");
		lex::pushSrcPosError (m_filePath, m_line, p - m_begin - m_lineOffset);
		return ScanResultKind_Error;
	}

	return
		!m_sectionName.isEmpty () ? ScanResultKind_Section :
		!m_keyName.isEmpty () ? ScanResultKind_KeyValue : ScanResultKind_Eof;
}

void
Lexer::reset ()
{
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

	m_filePath.clear ();
	m_sectionName.clear ();
	m_keyName.clear ();
	m_value.clear ();
}

void
Lexer::newLine (char* line)
{
	ASSERT (line [-1] == '\n');

	m_lineOffset = line - m_begin;
	m_line++;
}

//..............................................................................

} // namespace ini
} // namespace axl
