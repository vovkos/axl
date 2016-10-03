// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_INI_LEXER_H

#include "axl_ini_Pch.h"

namespace axl {
namespace ini {

//.............................................................................

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
		const sl::String& filePath,
		const sl::String& source
		)
	{
		return create (filePath, source, source.getLength ());
	}

	bool
	create (
		const sl::String& filePath,
		const char* source,
		size_t length = -1
		)
	{
		this->reset (); 
		
		init ();

		p   = (char*) source;
		eof = (char*) source + length;

		m_filePath = filePath;
		m_begin = p;

		return true;
	}

	void
	parseSection (
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
	parseKeyValue (
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
					m_value = enc::EscapeEncoding::decode (m_value.cc () + 1, length - 2);
			}	
		}

		stop ();
	}

	ScanResultKind
	scanLine ()
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
	reset ()
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

protected:
	// these are to be called from withing ragel scanner (*.rl)
	
	void 
	newLine (char* line)
	{ 
		ASSERT (line [-1] == '\n');

		m_lineOffset = line - m_begin;
		m_line++;
	}

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

//.............................................................................

} // namespace ini
} // namespace axl
