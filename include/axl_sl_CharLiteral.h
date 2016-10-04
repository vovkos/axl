// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_CHARLITERAL_H

#include "axl_sl_CallOnce.h"

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
sl::StringRef
getCharLiteral (uchar_t c)
{
	static char literalTable [256] [4];
	sl::callOnce (InitCharLiteralTable (), literalTable);
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
sl::StringRef
getCharCodeLiteral (uchar_t c)
{
	static char literalTable [256] [4];
	sl::callOnce (InitCharCodeTable (), literalTable);
	return literalTable [c];
}

//.............................................................................

} // namespace sl
} // namespace axl
