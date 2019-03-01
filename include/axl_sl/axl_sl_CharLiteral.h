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

#define _AXL_SL_CHARLITERAL_H

#include "axl_sl_CallOnce.h"

namespace axl {
namespace sl {

//..............................................................................

class InitCharLiteralTable
{
public:
	void
	operator () (char literalTable[][4]) const
	{
		for (int i = 0; i < 256; i++)
			_snprintf(literalTable[i], 4, "%c", i);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const char*
getCharLiteral(uchar_t c)
{
	static char literalTable[256][4];
	sl::callOnce(InitCharLiteralTable(), literalTable);
	return literalTable[c];
}

//..............................................................................

class InitCharCodeTable
{
public:
	void
	operator () (char literalTable[][4]) const
	{
		for (int i = 0; i < 256; i++)
			_snprintf(literalTable[i], 4, "\\%02x", i);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const char*
getCharCodeLiteral(uchar_t c)
{
	static char literalTable[256][4];
	sl::callOnce(InitCharCodeTable(), literalTable);
	return literalTable[c];
}

//..............................................................................

} // namespace sl
} // namespace axl
