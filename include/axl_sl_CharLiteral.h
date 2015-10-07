// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_CHARLITERAL_H

#include "axl_mt_CallOnce.h"

namespace axl {
namespace sl {

//.............................................................................

class InitCharLiteralTable
{
public:
	void
	operator () (char literalTable [] [4])
	{
		for (int i = 0; i < 256; i++)
			_snprintf (literalTable [i], 4, "%c", i);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const char*
getCharLiteral (uchar_t c)
{
	static char literalTable [256] [4];
	mt::callOnce (InitCharLiteralTable (), literalTable);
	return literalTable [c];
}

//.............................................................................

class InitCharCodeTable
{
public:
	void
	operator () (char literalTable [] [4])
	{
		for (int i = 0; i < 256; i++)
			_snprintf (literalTable [i], 4, "\\%02x", i);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const char*
getCharCodeLiteral (uchar_t c)
{
	static char literalTable [256] [4];
	mt::callOnce (InitCharCodeTable (), literalTable);
	return literalTable [c];
}

//.............................................................................

} // namespace sl
} // namespace axl
