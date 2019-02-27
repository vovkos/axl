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

#define _AXL_GUI_TEXTPAINTER_H

#include "axl_gui_Canvas.h"
#include "axl_gui_TextAttrAnchorArray.h"

namespace axl {
namespace gui {

//..............................................................................

class TextPainter
{
protected:
	enum Def
	{
		Def_BinTextDecodeBufferLength = 2 * 1024, // 8K
	};

protected:
	TextAttrAnchorArray m_selOverlayBuffer;
	sl::String m_stringBuffer;
	sl::Array<utf32_t> m_binTextBuffer;

public:
	Canvas* m_canvas;
	Point m_point;
	int m_top;
	int m_bottom;
	uint_t m_hexEncodingFlags;
	utf32_t m_unprintableChar;

public:
	TextPainter()
	{
		init();
	}

	TextPainter(Canvas* canvas)
	{
		init(canvas);
	}

	Point
	getPoint()
	{
		return m_point;
	}

	// space

	int
	drawSpace(
		size_t length,
		uint_t color
		);

	int
	drawSpace(size_t length)
	{
		return drawSpace(length, m_canvas->m_colorAttr.m_backColor);
	}

	int
	drawSpace_p(
		int width, // in pixels
		uint_t color
		);

	int
	drawSpace_p(int width) // in pixels
	{
		return drawSpace_p(width, m_canvas->m_colorAttr.m_backColor);
	}

	// text

	int
	drawText(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const sl::StringRef& text
		)
	{
		return drawText_utf8(textColor, backColor, fontFlags, text);
	}

	int
	drawText(
		const TextAttr& attr,
		const sl::StringRef& text
		)
	{
		return drawText_utf8(attr, text);
	}

	int
	drawText(const sl::StringRef& text)
	{
		return drawText_utf8(text);
	}

	int
	drawText_utf8(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const sl::StringRef_utf8& text
		);

	int
	drawText_utf8(
		const TextAttr& attr,
		const sl::StringRef_utf8& text
		)
	{
		return drawText_utf8(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			text
			);
	}

	int
	drawText_utf8(const sl::StringRef_utf8& text)
	{
		return drawText_utf8(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			text
			);
	}

	int
	drawText_utf32(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const sl::StringRef_utf32& text
		);

	int
	drawText_utf32(
		const TextAttr& attr,
		const sl::StringRef_utf32& text
		)
	{
		return drawText_utf32(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			text
			);
	}

	int
	drawText_utf32(const sl::StringRef_utf32& text)
	{
		return drawText_utf32(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			text
			);
	}

	// hypertext

	int
	drawHyperText(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const sl::StringRef& text
		)
	{
		return drawHyperText_utf8(
			textColor,
			backColor,
			fontFlags,
			attrArray,
			text
			);
	}

	int
	drawHyperText(
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const sl::StringRef& text
		)
	{
		return drawHyperText_utf8(attr, attrArray, text);
	}

	int
	drawHyperText(
		const TextAttrAnchorArray* attrArray,
		const sl::StringRef& text
		)
	{
		return drawHyperText_utf8(attrArray, text);
	}

	int
	drawHyperText_utf8(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const sl::StringRef_utf8& text
		);

	int
	drawHyperText_utf8(
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const sl::StringRef_utf8& text
		)
	{
		return drawHyperText_utf8(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			text
			);
	}

	int
	drawHyperText_utf8(
		const TextAttrAnchorArray* attrArray,
		const sl::StringRef_utf8& text
		)
	{
		return drawHyperText_utf8(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			attrArray,
			text
			);
	}

	int
	drawHyperText_utf32(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const sl::StringRef_utf32& text
		);

	int
	drawHyperText_utf32(
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const sl::StringRef_utf32& text
		)
	{
		return drawHyperText_utf32(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			text
			);
	}

	int
	drawHyperText_utf32(
		const TextAttrAnchorArray* attrArray,
		const sl::StringRef_utf32& text
		)
	{
		return drawHyperText_utf32(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			attrArray,
			text
			);
	}

	// hypertext with selection

	int
	drawSelHyperText(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		const sl::StringRef& text
		)
	{
		return drawSelHyperText_utf8(
			textColor,
			backColor,
			fontFlags,
			attrArray,
			selAttr,
			selStart,
			selEnd,
			text
			);
	}

	int
	drawSelHyperText(
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		const sl::StringRef& text
		)
	{
		return drawSelHyperText_utf8(
			attr,
			attrArray,
			selAttr,
			selStart,
			selEnd,
			text
			);
	}

	int
	drawSelHyperText(
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		const sl::StringRef& text
		)
	{
		return drawSelHyperText_utf8(
			attrArray,
			selAttr,
			selStart,
			selEnd,
			text
			);
	}

	int
	drawSelHyperText_utf8(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		const sl::StringRef_utf8& text
		);

	int
	drawSelHyperText_utf8(
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		const sl::StringRef_utf8& text
		)
	{
		return drawSelHyperText_utf8(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			selAttr,
			selStart,
			selEnd,
			text
			);
	}

	int
	drawSelHyperText_utf8(
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		const sl::StringRef_utf8& text
		)
	{
		return drawSelHyperText_utf8(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			attrArray,
			selAttr,
			selStart,
			selEnd,
			text
			);
	}

	int
	drawSelHyperText_utf32(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		const sl::StringRef_utf32& text
		);

	int
	drawSelHyperText_utf32(
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		const sl::StringRef_utf32& text
		)
	{
		return drawSelHyperText_utf32(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			selAttr,
			selStart,
			selEnd,
			text
			);
	}

	int
	drawSelHyperText_utf32(
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		const sl::StringRef_utf32& text
		)
	{
		return drawSelHyperText_utf32(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			attrArray,
			selAttr,
			selStart,
			selEnd,
			text
			);
	}

	// bin hex

	int
	drawBinHex(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		size_t halfBitOffset,
		const void* p,
		size_t size
		);

	int
	drawBinHex(
		const TextAttr& attr,
		size_t halfBitOffset,
		const void* p,
		size_t size
		)
	{
		return drawBinHex(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			halfBitOffset,
			p,
			size
			);
	}

	int
	drawBinHex(
		size_t halfBitOffset,
		const void* p,
		size_t size
		)
	{
		return drawBinHex(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			halfBitOffset,
			p,
			size
			);
	}

	int
	drawHyperBinHex(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		size_t halfBitOffset,
		const void* p,
		size_t size
		);

	int
	drawHyperBinHex(
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		size_t halfBitOffset,
		const void* p,
		size_t size
		)
	{
		return drawHyperBinHex(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			halfBitOffset,
			p,
			size
			);
	}

	int
	drawHyperBinHex(
		const TextAttrAnchorArray* attrArray,
		size_t halfBitOffset,
		const void* p,
		size_t size
		)
	{
		return drawHyperBinHex(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			attrArray,
			halfBitOffset,
			p,
			size
			);
	}

	int
	drawSelHyperBinHex(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		size_t halfBitOffset,
		const void* p,
		size_t size
		);

	int
	drawSelHyperBinHex(
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		size_t halfBitOffset,
		const void* p,
		size_t size
		)
	{
		return drawSelHyperBinHex(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			selAttr,
			selStart,
			selEnd,
			halfBitOffset,
			p,
			size
			);
	}

	int
	drawSelHyperBinHex(
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		size_t halfBitOffset,
		const void* p,
		size_t size
		)
	{
		return drawSelHyperBinHex(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			attrArray,
			selAttr,
			selStart,
			selEnd,
			halfBitOffset,
			p,
			size
			);
	}

	// bin text

	int
	drawBinText(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		);

	int
	drawBinText(
		const TextAttr& attr,
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		)
	{
		return drawBinText(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			codec,
			p,
			dataSize,
			bufferSize
			);
	}

	int
	drawBinText(
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		)
	{
		return drawBinText(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			codec,
			p,
			dataSize,
			bufferSize
			);
	}

	int
	drawHyperBinText(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		);

	int
	drawHyperBinText(
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		)
	{
		return drawHyperBinText(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			codec,
			p,
			dataSize,
			bufferSize
			);
	}

	int
	drawHyperBinText(
		const TextAttrAnchorArray* attrArray,
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		)
	{
		return drawHyperBinText(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			attrArray,
			codec,
			p,
			dataSize,
			bufferSize
			);
	}

	int
	drawSelHyperBinText(
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		);

	int
	drawSelHyperBinText(
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		)
	{
		return drawSelHyperBinText(
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			selAttr,
			selStart,
			selEnd,
			codec,
			p,
			dataSize,
			bufferSize
			);
	}

	int
	drawSelHyperBinText(
		const TextAttrAnchorArray* attrArray,
		const TextAttr& selAttr,
		size_t selStart,
		size_t selEnd,
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		)
	{
		return drawSelHyperBinText(
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1,
			attrArray,
			selAttr,
			selStart,
			selEnd,
			codec,
			p,
			dataSize,
			bufferSize
			);
	}

	static
	size_t
	buildBinTextString(
		sl::Array<utf32_t>* binTextBuffer,
		sl::String* stringBuffer,
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize,
		utf32_t unprintableChar = '.'
		);

	static
	sl::String
	buildBinTextString(
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize,
		utf32_t unprintableChar = '.'
		);

protected:
	void
	init(Canvas* canvas = NULL);

	static
	size_t
	buildBinTextBuffer(
		sl::Array<utf32_t>* binTextBuffer,
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize,
		utf32_t unprintableChar = '.'
		);

	size_t
	buildBinTextBuffer(
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		)
	{
		return buildBinTextBuffer(
			&m_binTextBuffer,
			codec,
			p,
			dataSize,
			bufferSize,
			m_unprintableChar
			);
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
