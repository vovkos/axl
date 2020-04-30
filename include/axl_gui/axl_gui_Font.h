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

#define _AXL_GUI_FONT_H

#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

class Font;

//..............................................................................

class FontTuple: public GuiItem
{
protected:
	Font* m_fontModArray[0x10];

protected:
	FontTuple(Engine* engine):
		GuiItem(engine)
	{
		memset(m_fontModArray, 0, sizeof(m_fontModArray));
	}

	~FontTuple()
	{
		clear();
	}

public:
	Font*
	createFont(
		const sl::StringRef& family,
		size_t pointSize,
		uint_t flags = 0
		)
	{
		return m_engine->createFont(this, family, pointSize, flags);
	}

	Font*
	createFont(const FontDesc& fontDesc)
	{
		return m_engine->createFont(this, fontDesc);
	}

	Font*
	getFont(uint_t flags = 0)
	{
		flags = (flags & FontFlag_Undefined) ? 0 : (flags & 0x0f);
		return m_fontModArray[flags] ? m_fontModArray[flags] : m_engine->getFontMod(this, flags);
	}

	void
	clear()
	{
		m_engine->clearFontTuple(this);
	}
};

//..............................................................................

class Font: public GuiItem
{
protected:
	FontTuple* m_tuple;

protected:
	Font(Engine* engine):
		GuiItem(engine)
	{
		m_tuple = NULL;
	}

public:
	Font*
	getFontMod(uint_t flags)
	{
		return (flags & FontFlag_Undefined) ? this : m_tuple->getFont(flags);
	}

	bool
	getFontDesc(FontDesc* fontDesc)
	{
		return m_engine->getFontDesc(this, fontDesc);
	}

	bool
	isMonospace()
	{
		return m_engine->isMonospaceFont(this);
	}
};

//..............................................................................

inline
Font*
Engine::getStdFont(
	StdFontKind fontKind,
	uint_t flags
	)
{
	return getStdFontTuple(fontKind)->getFont(flags);
}

//..............................................................................


} // namespace gui
} // namespace axl
