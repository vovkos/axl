// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_FONT_H

#include "axl_gui_Def.h"
#include "axl_gui_Color.h"
#include "axl_rtl_BitIdx.h"
#include "axl_ref_RefCount.h"
#include "axl_mem_Factory.h"

namespace axl {
namespace gui {

class CFont;

//.............................................................................

enum EStdFont
{
	EStdFont_Gui = 0,
	EStdFont_Monospace,
	
	EStdFont__Count
};

//.............................................................................

enum EFontFlag
{
	EFontFlag_Bold        = 0x01,
	EFontFlag_Italic      = 0x02,
	EFontFlag_Underline   = 0x04,
	EFontFlag_Strikeout   = 0x08,
	EFontFlag_Transparent = 0x10,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EFontFlag
getFirstFontFlag (uint_t flags)
{
	return (EFontFlag) (1 << rtl::getLoBitIdx (flags));
}

const char* 
getFontFlagString (EFontFlag flag);

inline
const char* 
getFontFlagString (uint_t flags)
{
	return getFontFlagString (getFirstFontFlag (flags));
}

inline
int
overlayFontFlags (
	uint_t baseFontFlags,
	uint_t overlayFontFlags
	)
{
	return (overlayFontFlags & EFontFlag_Transparent) ? baseFontFlags : overlayFontFlags;
}

uint_t
parseFontFlagString (
	const char* string,
	const char** end = NULL
	);

//.............................................................................

struct TFontDesc
{
	char m_faceName [32];
	size_t m_pointSize;
	uint_t m_flags;

	TFontDesc ()
	{
		memset (this, 0, sizeof (TFontDesc));
	}

	TFontDesc (
		const char* faceName, 
		size_t pointSize = 0,
		uint_t flags = 0
		)
	{
		setup (faceName, pointSize, flags);
	}

	void
	setup (
		const char* faceName, 
		size_t pointSize = 0,
		uint_t flags = 0
		);
};

//.............................................................................

struct TFontTuple
{
	TFontDesc m_fontDesc;
	
	CFont* m_baseFont;
	CFont* m_fontModArray [0x10];

	TFontTuple ()
	{
		m_baseFont = NULL;
		memset (m_fontModArray, 0, sizeof (m_fontModArray));
	}
};

//.............................................................................

template <typename T>
class CFontTupleT: 
	public ref::CRefCount,
	public TFontTuple
{
public:
	~CFontTupleT ()
	{
		for (size_t i = 0; i < countof (m_fontModArray); i++)
		{
			T* font = static_cast <T*> (m_fontModArray [i]);
			if (font)
				AXL_MEM_DELETE (font);
		}
	}
};

//.............................................................................

class CFont: public CGuiItem
{
protected:
	TFontTuple* m_tuple;
	TFontDesc m_fontDesc;

public:
	CFont ()
	{
		m_tuple = NULL;
	}

	const TFontDesc*
	getFontDesc ()
	{
		return &m_fontDesc;
	}

	CFont*
	getFontMod (uint_t flags);

	virtual
	bool
	isMonospace () = 0;

	TSize
	calcTextSize (
		const char* text,
		size_t length = -1
		)
	{
		return calcTextSize_utf8 (text, length);
	}

	virtual
	TSize
	calcTextSize_utf8 (
		const utf8_t* text,
		size_t length = -1
		) = 0;

	virtual
	TSize
	calcTextSize_utf16 (
		const utf16_t* text,
		size_t length = -1
		) = 0;

	virtual
	TSize
	calcTextSize_utf32 (
		const utf32_t* text,
		size_t length = -1
		) = 0;

	TSize
	calcTextSize (utf32_t c)
	{
		return calcTextSize_utf32 (&c, 1);
	}
};

//.............................................................................

struct TTextAttr: public TColorAttr
{
	uint_t m_fontFlags;

	TTextAttr ()
	{
		m_fontFlags = EFontFlag_Transparent;
	}

	TTextAttr (
		uint_t foreColor,
		uint_t backColor = EColorFlag_Transparent,
		uint_t fontFlags = EFontFlag_Transparent
		)
	{
		setup (foreColor, backColor, fontFlags);
	}

	TTextAttr (
		const TColorAttr& colorAttr,
		uint_t fontFlags = EFontFlag_Transparent
		)
	{
		setup (colorAttr, fontFlags);
	}

	TTextAttr (const char* string)
	{
		parse (string);
	}

	int 
	cmp (const TTextAttr& attr)
	{
		return memcmp (this, &attr, sizeof (TTextAttr));
	}

	void
	clear ()
	{
		TColorAttr::clear ();
		m_fontFlags = EFontFlag_Transparent;
	}

	void
	setup (
		uint_t foreColor,
		uint_t backColor,
		uint_t fontFlags
		)
	{
		m_foreColor = foreColor;
		m_backColor = backColor;
		m_fontFlags = fontFlags;
	}

	void
	setup (
		const TColorAttr& colorAttr,
		uint_t fontFlags
		)
	{

		*(TColorAttr*) this = colorAttr;
		m_fontFlags = fontFlags;
	}

	void
	overlay (
		const TTextAttr& baseAttr,
		const TTextAttr& overlayAttr
		)
	{
		TColorAttr::overlay (baseAttr, overlayAttr);	
		m_fontFlags = overlayFontFlags (baseAttr.m_fontFlags, overlayAttr.m_fontFlags);
	}

	void
	overlay (const TTextAttr& overlayAttr)
	{
		overlay (*this, overlayAttr);
	}

	void
	parse (
		const char* string,
		const char** end = NULL
		);

	void
	parseOverlay (
		const TTextAttr& baseAttr,
		const char* string,
		const char** end = NULL
		);

	void
	parseOverlay (
		const char* string,
		const char** end = NULL
		)
	{
		parseOverlay (*this, string, end);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

