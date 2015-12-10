// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_FONT_H

#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

class Font;

//.............................................................................

class FontTuple: public GuiItem
{
protected:
	Font* m_fontModArray [0x10];

protected:
	FontTuple (Engine* engine):
		GuiItem (engine)
	{
		memset (m_fontModArray, 0, sizeof (m_fontModArray));
	}

	~FontTuple ()
	{
		clear ();
	}

public:
	Font* 
	createFont (
		const char* family,
		size_t pointSize,
		uint_t flags = 0
		)
	{
		return m_engine->createFont (this, family, pointSize, flags);
	}

	Font* 
	createFont (const FontDesc& fontDesc)
	{
		return m_engine->createFont (this, fontDesc);
	}

	Font* 
	getFont (uint_t flags = 0)
	{
		flags = (flags & FontFlag_Undefined) ? 0 : (flags & 0x0f);
		return m_fontModArray [flags] ? m_fontModArray [flags] : m_engine->getFontMod (this, flags);
	}

	void
	clear ()
	{
		m_engine->clearFontTuple (this);
	}
};

//.............................................................................

class Font: public GuiItem
{
protected:
	FontTuple* m_tuple;

protected:
	Font (Engine* engine):
		GuiItem (engine)
	{
		m_tuple = NULL;
	}

public:
	Font* 
	getFontMod (uint_t flags)
	{
		return (flags & FontFlag_Undefined) ? this : m_tuple->getFont (flags);
	}

	bool
	getFontDesc (FontDesc* fontDesc)
	{
		return m_engine->getFontDesc (this, fontDesc);
	}

	bool
	isMonospace ()
	{
		return m_engine->isMonospaceFont (this);
	}

	Size
	calcTextSize (
		const char* text,
		size_t length = -1
		)
	{
		return calcTextSize_utf8 (text, length);
	}

	Size
	calcTextSize_utf8 (
		const utf8_t* text,
		size_t length = -1
		)
	{
		return m_engine->calcTextSize_utf8 (this, text, length);
	}

	Size
	calcTextSize_utf16 (
		const utf16_t* text,
		size_t length = -1
		)
	{
		return m_engine->calcTextSize_utf16 (this, text, length);
	}

	Size
	calcTextSize_utf32 (
		const utf32_t* text,
		size_t length = -1
		)
	{
		return m_engine->calcTextSize_utf32 (this, text, length);
	}

	Size
	calcTextSize (utf32_t c)
	{
		return calcTextSize_utf32 (&c, 1);
	}
};

//.............................................................................

inline
Font*
Engine::getStdFont (
	StdFontKind fontKind,
	uint_t flags
	)
{
	return getStdFontTuple (fontKind)->getFont (flags);
}

//.............................................................................


} // namespace gui
} // namespace axl

