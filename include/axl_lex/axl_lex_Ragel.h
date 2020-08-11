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

#define _AXL_LEX_RAGEL_H

#include "axl_lex_Lexer.h"

namespace axl {
namespace lex {

//..............................................................................

class Ragel
{
protected:
	sl::Array<int> m_stack; // stack of states
	char* m_begin;

	// ragel interface variables

	char* p;
	char* pe;
	char* eof;
	char* ts;
	char* te;
	int* stack;
	int act;
	int cs;
	int top;

public:
	Ragel()
	{
		clear();
	}

	void
	setSource(
		const sl::StringRef& source,
		bool isBomNeeded = false
		);

protected:
	int*
	prePush();

	void
	postPop();

	void
	stop()
	{
		pe = p + 1;
	}

	void
	clear();

	// to be implemented:

	// void
	// init ()
	// {
	// }

	// void
	// exec ()
	// {
	// }
};

//..............................................................................

} // namespace lex
} // namespace axl
