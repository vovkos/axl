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

#define _AXL_GUI_CANVAS_H

#include "axl_gui_Engine.h"
#include "axl_gui_ColorAttr.h"
#include "axl_gui_TextAttr.h"
#include "axl_gui_Font.h"

namespace axl {
namespace gui {

class Image;

//..............................................................................

class Canvas: public GuiItem
{
protected:
	Font* m_driverFont;
	ColorAttr m_driverColorAttr;

public:
	Font* m_font;
	ColorAttr m_colorAttr;
	Palette m_palette;

protected:
	Canvas(Engine* engine):
		GuiItem(engine)
	{
		m_font = engine->getStdFont(StdFontKind_Gui);
		m_driverFont = NULL;
		m_colorAttr.setup(StdPalColor_WidgetText, StdPalColor_WidgetBack);
	}

public:
	void
	setTextAttr(const TextAttr& attr)
	{
		m_colorAttr = attr;
		m_font = m_font->getFontMod(attr.m_fontFlags);
	}

	// rect drawing

	bool
	drawRect(
		int left,
		int top,
		int right,
		int bottom,
		uint_t color
		)
	{
		return m_engine->drawRect(this, left, top, right, bottom, color);
	}

	bool
	drawRect(
		int left,
		int top,
		int right,
		int bottom
		)
	{
		return drawRect(left, top, right, bottom, m_colorAttr.m_backColor);
	}

	bool
	drawRect(
		const Rect& rect,
		uint_t color
		)
	{
		return drawRect(
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			color
			);
	}

	bool
	drawRect(const Rect& rect)
	{
		return drawRect(
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			m_colorAttr.m_backColor
			);
	}

	// alpha rect drawing

	bool
	drawAlphaRect(
		int left,
		int top,
		int right,
		int bottom,
		uint_t color,
		uint_t alpha
		)
	{
		return m_engine->drawAlphaRect(this, left, top, right, bottom, color, alpha);
	}

	bool
	drawAlphaRect(
		int left,
		int top,
		int right,
		int bottom,
		uint_t alpha
		)
	{
		return drawAlphaRect(left, top, right, bottom, m_colorAttr.m_backColor, alpha);
	}

	bool
	drawAlphaRect(
		const Rect& rect,
		uint_t color,
		uint_t alpha
		)
	{
		return drawAlphaRect(
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			color,
			alpha
			);
	}

	bool
	drawAlphaRect(
		const Rect& rect,
		uint_t alpha
		)
	{
		return drawAlphaRect(
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			m_colorAttr.m_backColor,
			alpha
			);
	}

	// gradient rect drawing

	bool
	drawGradientRect(
		int left,
		int top,
		int right,
		int bottom,
		int x1,
		int y1,
		uint_t color1,
		int x2,
		int y2,
		uint_t color2
		)
	{
		return m_engine->drawGradientRect(
			this,
			left,
			top,
			right,
			bottom,
			x1,
			y1,
			color1,
			x2,
			y2,
			color2
			);
	}

	bool
	drawGradientRect(
		const Rect& rect,
		const Point& point1,
		uint_t color1,
		const Point& point2,
		uint_t color2
		)
	{
		return m_engine->drawGradientRect(
			this,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			point1.m_x,
			point1.m_y,
			color1,
			point2.m_x,
			point2.m_y,
			color2
			);
	}

	// alpha gradient rect drawing

	bool
	drawAlphaGradientRect(
		int left,
		int top,
		int right,
		int bottom,
		int x1,
		int y1,
		uint_t color1,
		uint_t alpha1,
		int x2,
		int y2,
		uint_t color2,
		uint_t alpha2
		)
	{
		return m_engine->drawAlphaGradientRect(
			this,
			left,
			top,
			right,
			bottom,
			x1,
			y1,
			color1,
			alpha1,
			x2,
			y2,
			color2,
			alpha2
			);
	}

	bool
	drawAlphaGradientRect(
		const Rect& rect,
		const Point& point1,
		uint_t color1,
		uint_t alpha1,
		const Point& point2,
		uint_t color2,
		uint_t alpha2
		)
	{
		return m_engine->drawAlphaGradientRect(
			this,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			point1.m_x,
			point1.m_y,
			color1,
			alpha1,
			point2.m_x,
			point2.m_y,
			color2,
			alpha2
			);
	}

	// default to utf8

	bool
	drawText(
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const sl::StringRef& text
		)
	{
		return drawText_utf8(
			x,
			y,
			left,
			top,
			right,
			bottom,
			textColor,
			backColor,
			fontFlags,
			text
			);
	}

	bool
	drawText(
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		const sl::StringRef& text
		)
	{
		return drawText_utf8(x, y, left, top, right, bottom, text);
	}

	bool
	drawText(
		const Point& point,
		const Rect& rect,
		const TextAttr& textAttr,
		const sl::StringRef& text
		)
	{
		return drawText_utf8(point, rect, textAttr, text);
	}

	bool
	drawText(
		const Point& point,
		const Rect& rect,
		const sl::StringRef& text
		)
	{
		return drawText_utf8(point, rect, text);
	}

	// utf8 text drawing

	bool
	drawText_utf8(
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const sl::StringRef_utf8& text
		)
	{
		return m_engine->drawText_utf8(
			this,
			x,
			y,
			left,
			top,
			right,
			bottom,
			textColor,
			backColor,
			fontFlags,
			text
			);
	}

	bool
	drawText_utf8(
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		const sl::StringRef_utf8& text
		)
	{
		return drawText_utf8(
			x,
			y,
			left,
			top,
			right,
			bottom,
			m_colorAttr.m_foreColor,
			m_colorAttr.m_backColor,
			-1,
			text
			);
	}

	bool
	drawText_utf8(
		const Point& point,
		const Rect& rect,
		const TextAttr& textAttr,
		const sl::StringRef_utf8& text
		)
	{
		return drawText_utf8(
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			textAttr.m_foreColor,
			textAttr.m_backColor,
			textAttr.m_fontFlags,
			text
			);
	}

	bool
	drawText_utf8(
		const Point& point,
		const Rect& rect,
		const sl::StringRef_utf8& text
		)
	{
		return drawText_utf8(
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			m_colorAttr.m_foreColor,
			m_colorAttr.m_backColor,
			0,
			text
			);
	}

	// utf16 text drawing

	bool
	drawText_utf16(
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const sl::StringRef_utf16& text
		)
	{
		return m_engine->drawText_utf16(
			this,
			x,
			y,
			left,
			top,
			right,
			bottom,
			textColor,
			backColor,
			fontFlags,
			text
			);
	}

	bool
	drawText_utf16(
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		const sl::StringRef_utf16& text
		)
	{
		return drawText_utf16(
			x,
			y,
			left,
			top,
			right,
			bottom,
			m_colorAttr.m_foreColor,
			m_colorAttr.m_backColor,
			-1,
			text
			);
	}

	bool
	drawText_utf16(
		const Point& point,
		const Rect& rect,
		const TextAttr& textAttr,
		const sl::StringRef_utf16& text
		)
	{
		return drawText_utf16(
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			textAttr.m_foreColor,
			textAttr.m_backColor,
			textAttr.m_fontFlags,
			text
			);
	}

	bool
	drawText_utf16(
		const Point& point,
		const Rect& rect,
		const sl::StringRef_utf16& text
		)
	{
		return drawText_utf16(
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			m_colorAttr.m_foreColor,
			m_colorAttr.m_backColor,
			-1,
			text
			);
	}

	// utf32 text drawing

	bool
	drawText_utf32(
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const sl::StringRef_utf32& text
		)
	{
		return m_engine->drawText_utf32(
			this,
			x,
			y,
			left,
			top,
			right,
			bottom,
			textColor,
			backColor,
			fontFlags,
			text
			);
	}

	bool
	drawText_utf32(
		int x,
		int y,
		int left,
		int top,
		int right,
		int bottom,
		const sl::StringRef_utf32& text
		)
	{
		return drawText_utf32(
			x,
			y,
			left,
			top,
			right,
			bottom,
			m_colorAttr.m_foreColor,
			m_colorAttr.m_backColor,
			-1,
			text
			);
	}

	bool
	drawText_utf32(
		const Point& point,
		const Rect& rect,
		const TextAttr& textAttr,
		const sl::StringRef_utf32& text
		)
	{
		return drawText_utf32(
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			textAttr.m_foreColor,
			textAttr.m_backColor,
			textAttr.m_fontFlags,
			text
			);
	}

	bool
	drawText_utf32(
		const Point& point,
		const Rect& rect,
		const sl::StringRef_utf32& text
		)
	{
		return drawText_utf32(
			point.m_x,
			point.m_y,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom,
			m_colorAttr.m_foreColor,
			m_colorAttr.m_backColor,
			0,
			text
			);
	}

	// image drawing

	bool
	drawImage(
		int x,
		int y,
		Image* image,
		int left,
		int top,
		int right,
		int bottom
		)
	{
		return m_engine->drawImage(
			this,
			x,
			y,
			image,
			left,
			top,
			right,
			bottom
			);
	}

	bool
	drawImage(
		const Point& point,
		Image* image,
		const Rect& rect = Rect()
		)
	{
		return drawImage(
			point.m_x,
			point.m_y,
			image,
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom
			);
	}

	// bitblt

	bool
	copyRect(
		int x,
		int y,
		Canvas* srcCanvas,
		int left,
		int top,
		int right,
		int bottom
		)
	{
		return m_engine->copyRect(
			this,
			x,
			y,
			srcCanvas,
			left,
			right,
			top,
			bottom
			);
	}

	bool
	copyRect(
		const Point& point,
		Canvas* srcCanvas,
		const Rect& srcRect
		)
	{
		return copyRect(
			point.m_x,
			point.m_y,
			srcCanvas,
			srcRect.m_left,
			srcRect.m_top,
			srcRect.m_right,
			srcRect.m_bottom
			);
	}
};

//..............................................................................

template <typename T>
class OffscreenCanvasCache
{
protected:
	struct Entry
	{
		Size m_size;
		T m_canvas;
	};

protected:
	Engine* m_engine;
	Entry* m_canvasTable[FormFactor__Count];

public:
	OffscreenCanvasCache(Engine* engine)
	{
		m_engine = engine;
		memset(m_canvasTable, 0, sizeof(m_canvasTable));
	}

	~OffscreenCanvasCache()
	{
		clear();
	}

	void
	clear()
	{
		for (size_t i = 0; i < countof(m_canvasTable); i++)
			if (m_canvasTable[i])
				AXL_MEM_DELETE(m_canvasTable[i]);

		memset(m_canvasTable, 0, sizeof(m_canvasTable));
	}

	T*
	getCanvas(
		uint_t width,
		uint_t height
		)
	{
		FormFactor formFactor = getFormFactor(width, height);
		ASSERT(formFactor < countof(m_canvasTable));

		Entry* entry = m_canvasTable[formFactor];
		if (!entry)
		{
			entry = AXL_MEM_NEW(Entry);
			m_canvasTable[formFactor] = entry;
		}

		if (entry->m_size.m_width_u >= width &&
			entry->m_size.m_height_u >= height)
			return &entry->m_canvas;

		width = width < entry->m_size.m_width_u ?
			entry->m_size.m_width_u :
			sl::align<16> (width);

		height = entry->m_size.m_height_u < height ?
			entry->m_size.m_height_u :
			sl::align<16> (height);

		bool result = m_engine->createOffscreenCanvas(&entry->m_canvas, width, height);
		if (!result)
			return NULL;

		entry->m_size.m_width_u = width;
		entry->m_size.m_height_u = height;
		return &entry->m_canvas;
	}

	T*
	getCanvas(const Size& size)
	{
		return getCanvas(size.m_width, size.m_height);
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
