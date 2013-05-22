// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_CHARLITERAL_H

#include "axl_mt_CallOnce.h"

namespace axl {
namespace rtl {

//.............................................................................

class CInitCharLiteralTable
{
public:
	void
	operator () (char LiteralTable [] [4])
	{
		for (int i = 0; i < 256; i++)
			_snprintf (LiteralTable [i], 4, "%c", i);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const char*
GetCharLiteral (uchar_t Char)
{
	static char _LiteralTable [256] [4];
	mt::CallOnce (CInitCharLiteralTable (), _LiteralTable);
	return _LiteralTable [Char];
}

//.............................................................................

class CInitCharCodeTable
{
public:
	void
	operator () (char LiteralTable [] [4])
	{
		for (int i = 0; i < 256; i++)
			_snprintf (LiteralTable [i], 4, "\\%02x", i);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const char*
GetCharCodeLiteral (uchar_t Char)
{
	static char _LiteralTable [256] [4];
	mt::CallOnce (CInitCharCodeTable (), _LiteralTable);
	return _LiteralTable [Char];
}

//.............................................................................

} // namespace rtl
} // namespace axl
