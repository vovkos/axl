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

class Font;

//.............................................................................

enum StdFontKind
{
	StdFontKind_Gui = 0,
	StdFontKind_Monospace,
	
	StdFontKind__Count
};

//.............................................................................

enum FontFlagKind
{
	FontFlagKind_Bold        = 0x01,
	FontFlagKind_Italic      = 0x02,
	FontFlagKind_Underline   = 0x04,
	FontFlagKind_Strikeout   = 0x08,
	FontFlagKind_Transparent = 0x10,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
FontFlagKind
getFirstFontFlag (uint_t flags)
{
	return (FontFlagKind) (1 << rtl::getLoBitIdx (flags));
}

const char* 
getFontFlagString (FontFlagKind flag);

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
	return (overlayFontFlags & FontFlagKind_Transparent) ? baseFontFlags : overlayFontFlags;
}

uint_t
parseFontFlagString (
	const char* string,
	const char** end = NULL
	);

//.............................................................................

struct FontDesc
{
	char m_faceName [32];
	size_t m_pointSize;
	uint_t m_flags;

	FontDesc ()
	{
		memset (this, 0, sizeof (FontDesc));
	}

	FontDesc (
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

struct Fontuple
{
	FontDesc m_fontDesc;
	
	Font* m_baseFont;
	Font* m_fontModArray [0x10];

	Fontuple ()
	{
		m_baseFont = NULL;
		memset (m_fontModArray, 0, sizeof (m_fontModArray));
	}
};

//.............................................................................

template <typename T>
class FontTuple: 
	public ref::RefCount,
	public Fontuple
{
public:
	~FontTuple ()
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

class Font: public GuiItem
{
protected:
	Fontuple* m_tuple;
	FontDesc m_fontDesc;

public:
	Font ()
	{
		m_tuple = NULL;
	}

	const FontDesc*
	getFontDesc ()
	{
		return &m_fontDesc;
	}

	Font*
	getFontMod (uint_t flags);

	virtual
	bool
	isMonospace () = 0;

	Size
	calcTextSize (
		const char* text,
		size_t length = -1
		)
	{
		return calcTextSize_utf8 (text, length);
	}

	virtual
	Size
	calcTextSize_utf8 (
		const utf8_t* text,
		size_t length = -1
		) = 0;

	virtual
	Size
	calcTextSize_utf16 (
		const utf16_t* text,
		size_t length = -1
		) = 0;

	virtual
	Size
	calcTextSize_utf32 (
		const utf32_t* text,
		size_t length = -1
		) = 0;

	Size
	calcTextSize (utf32_t c)
	{
		return calcTextSize_utf32 (&c, 1);
	}
};

//.............................................................................

struct TextAttr: public ColorAttr
{
	uint_t m_fontFlags;

	TextAttr ()
	{
		m_fontFlags = FontFlagKind_Transparent;
	}

	TextAttr (
		uint_t foreColor,
		uint_t backColor = ColorFlagKind_Transparent,
		uint_t fontFlags = FontFlagKind_Transparent
		)
	{
		setup (foreColor, backColor, fontFlags);
	}

	TextAttr (
		const ColorAttr& colorAttr,
		uint_t fontFlags = FontFlagKind_Transparent
		)
	{
		setup (colorAttr, fontFlags);
	}

	TextAttr (const char* string)
	{
		parse (string);
	}

	int 
	cmp (const TextAttr& attr)
	{
		return memcmp (this, &attr, sizeof (TextAttr));
	}

	void
	clear ()
	{
		ColorAttr::clear ();
		m_fontFlags = FontFlagKind_Transparent;
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
		const ColorAttr& colorAttr,
		uint_t fontFlags
		)
	{

		*(ColorAttr*) this = colorAttr;
		m_fontFlags = fontFlags;
	}

	void
	overlay (
		const TextAttr& baseAttr,
		const TextAttr& overlayAttr
		)
	{
		ColorAttr::overlay (baseAttr, overlayAttr);	
		m_fontFlags = overlayFontFlags (baseAttr.m_fontFlags, overlayAttr.m_fontFlags);
	}

	void
	overlay (const TextAttr& overlayAttr)
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
		const TextAttr& baseAttr,
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

