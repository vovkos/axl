// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LOG_BINDATACONFIG_H

#include "axl_g_Def.h"

namespace axl {
namespace log {

//.............................................................................

enum EBinView
{
	EBinView_Hex = 0,
	EBinView_Text,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EBinTextEnc
{
	EBinTextEnc_Ascii = 0,
	EBinTextEnc_Utf8,
	EBinTextEnc_Utf16,
	EBinTextEnc_Utf16_be,
	EBinTextEnc_Utf32,
	EBinTextEnc_Utf32_be,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TBinDataConfig
{
	uint8_t m_BinViewKind;
	uint8_t m_BinTextEnc;
	uint16_t m_BinTextTabSize;
	uint16_t m_BinTextLineSize; // binary data is auto-wrapped
	uint16_t m_BinHexLineSize; // binary data is auto-wrapped
	uint64_t m_BinMergeThreshold;

	TBinDataConfig ()
	{
		SetDefaults ();
	}

	int 
	Cmp (const TBinDataConfig& Config) const
	{
		return memcmp (this, &Config, sizeof (TBinDataConfig));
	}

	uint16_t
	GetBinLineSize () const
	{
		return m_BinViewKind ? m_BinTextLineSize : m_BinHexLineSize;
	}

	void
	SetDefaults ();
};

//.............................................................................

} // namespace log
} // namespace axl
