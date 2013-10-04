// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_INI_LEXER_H

#include "axl_lex_Token.h"
#include "axl_err_ParseError.h"
#include "axl_rtl_EscapeEncoding.h"

namespace axl {
namespace ini {

//.............................................................................

class CLexer
{
public:
	enum EScanResult
	{
		EScanResult_Error = -1,
		EScanResult_Eof   = 0,
		EScanResult_Section,
		EScanResult_KeyValue,
	};

protected:
	rtl::CString m_FilePath;

	char* m_pBegin;
	int m_Line;
	size_t m_LineOffset;

	rtl::CString m_SectionName;
	rtl::CString m_KeyName;
	rtl::CString m_Value;

	// ragel interface variables

	char* p;
	char* pe;
	char* eof;
	char* ts;
	char* te;
	int act;
	int cs;

protected: // should only be used as part of ini::CParser
	CLexer ()
	{
		Reset ();
	}

	bool
	Create (
		const rtl::CString& FilePath,
		const rtl::CString& Source
		)
	{
		return Create (FilePath, Source, Source.GetLength ());
	}

	bool
	Create (
		const rtl::CString& FilePath,
		const char* pSource,
		size_t Length = -1
		)
	{
		this->Reset (); 
		
		Init ();

		p   = (char*) pSource;
		eof = (char*) pSource + Length;

		m_FilePath = FilePath;
		m_pBegin = p;

		return true;
	}

	void
	ParseSection (
		const char* p,
		const char* pEnd
		)
	{
		ASSERT (*p == '[');
		
		if (pEnd [-1] != ']')
			return;

		p++;
		pEnd--;

		while (p < pEnd && isspace (*p))
			p++;

		while (p < pEnd && isspace (pEnd [-1]))
			pEnd--;

		if (p < pEnd)
		{
			m_SectionName.Copy (p, pEnd - p);
			Break ();
		}
	}

	void
	ParseKeyValue (
		const char* p,
		const char* pEnd
		)
	{
		ASSERT (!isspace (*p));

		const char* p0 = p;

		while (p < pEnd && !isspace (*p))
			p++;

		m_KeyName.Copy (p0, p - p0);

		while (p < pEnd && isspace (*p))
			p++;

		if (*p == '=')
		{
			p++;

			while (p < pEnd && isspace (*p))
				p++;

			while (p < pEnd && isspace (pEnd [-1]))
				pEnd--;

			if (p < pEnd)
			{
				size_t Length = pEnd - p;
				m_Value.Copy (p, Length);					

				if (m_Value [0] == '"' && m_Value [Length - 1] == '"')
					m_Value = rtl::CEscapeEncoding::Decode (m_Value.cc () + 1, Length - 2);
			}	
		}

		Break ();
	}

	EScanResult
	ScanLine ()
	{
		if (p == eof) 
			return EScanResult_Eof;

		pe = eof;
		m_SectionName.Clear ();
		m_KeyName.Clear ();
		m_Value.Clear ();

		bool Result = Exec ();
		if (!Result)
		{
			err::SetStringError ("invalid syntax");
			err::PushSrcPosError (m_FilePath, m_Line, p - m_pBegin - m_LineOffset);
			return EScanResult_Error;
		}
		
		return 
			!m_SectionName.IsEmpty () ? EScanResult_Section : 
			!m_KeyName.IsEmpty () ? EScanResult_KeyValue : EScanResult_Eof;
	}

	void
	SetLineCol (
		int Line,
		int Col
		)
	{
		m_Line = Line;
		m_LineOffset = p - m_pBegin - Col;
	}

	void
	SetLineCol (const lex::CLineCol& LineCol)
	{
		SetLineCol (LineCol.m_Line, LineCol.m_Col);
	}

	void 
	Reset ()
	{
		p = NULL;
		pe = NULL;
		eof = NULL;
		ts = NULL;
		te = NULL;
		act = 0;
		cs = 0;

		m_pBegin = NULL;
		m_Line = 0;
		m_LineOffset = 0;

		m_FilePath.Clear ();
		m_SectionName.Clear ();
		m_KeyName.Clear ();
		m_Value.Clear ();
	}

protected:
	// these are to be called from withing ragel scanner (*.rl)
	
	void 
	NewLine (char* pLine)
	{ 
		ASSERT (pLine [-1] == '\n');

		m_LineOffset = pLine - m_pBegin;
		m_Line++;
	}

	void
	Break ()
	{
		pe = p + 1;
	}

	// implemented in lexer.rl

	void
	Init ();

	bool
	Exec ();
};

//.............................................................................

} // namespace ini
} // namespace axl
