// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_CHARLITERAL_H

#include "axl_exe_CallOnce.h"

namespace axl {
namespace rtl {

//.............................................................................

inline
const tchar_t*
GetCharLiteral (uchar_t Char)
{
	static tchar_t _LiteralTable [256] [4];

	class CInit
	{
	public:
		void
		operator () ()
		{
			for (int i = 0; i < 256; i++)
				_sntprintf (_LiteralTable [i], 4, _T("%c"), i);
		}
	};

	exe::CallOnce (CInit ());
	return _LiteralTable [Char];
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const tchar_t*
GetCharCodeLiteral (uchar_t Char)
{
	static tchar_t _LiteralTable [256] [4];

	class CInit
	{
	public:
		void
		operator () ()
		{
			for (int i = 0; i < 256; i++)
				_sntprintf (_LiteralTable [i], 4, _T("\\%02x"), i);
		}
	};

	exe::CallOnce (CInit ());
	return _LiteralTable [Char];
}

//.............................................................................

} // namespace rtl
} // namespace axl
