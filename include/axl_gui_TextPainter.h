// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_TEXTPAINTER_H

#include "axl_gui_Canvas.h"
#include "axl_gui_TextAttrAnchorArray.h"

namespace axl {
namespace gui {

//.............................................................................

class TextPainter
{
protected:
	TextAttrAnchorArray m_selOverlayBuffer;
	sl::String m_stringBuffer; 
	sl::Array <utf32_t> m_binTextBuffer; 

public:
	Canvas* m_canvas;
	Point m_point;
	int m_top;
	int m_bottom;
	uint_t m_hexEncodingFlags;
	char m_unprintableChar;

public:
	TextPainter ()
	{
		init ();
	}

	TextPainter (Canvas* canvas)
	{
		init (canvas);
	}

	Point
	getPoint ()
	{
		return m_point;
	}

	// space

	int
	drawSpace (
		size_t length,
		uint_t color
		);

	int
	drawSpace (size_t length)
	{
		return drawSpace (length, m_canvas->m_colorAttr.m_backColor);
	}

	int
	drawSpace_p (
		int width, // in pixels
		uint_t color
		); 

	int
	drawSpace_p (int width) // in pixels
	{
		return drawSpace_p (width, m_canvas->m_colorAttr.m_backColor);
	}

	// text

	int
	drawText (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const char* text, 
		size_t length = -1
		)
	{
		return drawText_utf8 (textColor, backColor, fontFlags, text, length);
	}

	int
	drawText (
		const TextAttr& attr,
		const char* text, 
		size_t length = -1
		)
	{
		return drawText_utf8 (attr, text, length);
	}

	int
	drawText (
		const char* text, 
		size_t length = -1
		)
	{
		return drawText_utf8 (text, length);
	}

	int
	drawText_utf8 (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const utf8_t* text, 
		size_t length = -1
		);

	int
	drawText_utf8 (
		const TextAttr& attr,
		const utf8_t* text,
		size_t length = -1
		)
	{
		return drawText_utf8 (
			attr.m_foreColor, 
			attr.m_backColor, 
			attr.m_fontFlags, 
			text, 
			length
			);
	}

	int
	drawText_utf8 (
		const utf8_t* text,
		size_t length = -1
		)
	{
		return drawText_utf8 (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			text, 
			length
			);
	}

	int
	drawText_utf32 (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const utf32_t* text, 
		size_t length = -1
		);

	int
	drawText_utf32 (
		const TextAttr& attr,
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return drawText_utf32 (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags, 
			text, 
			length
			);
	}

	int
	drawText_utf32 (
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return drawText_utf32 (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			text, 
			length
			);
	}

	// hypertext

	int
	drawHyperText (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const sl::String& text
		)
	{
		return drawHyperText_utf8 (
			textColor, 
			backColor,
			fontFlags,
			attrArray, 
			text, 
			text.getLength ()
			);
	}

	int
	drawHyperText (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const char* text, 
		size_t length = -1
		)
	{
		return drawHyperText_utf8 (attr, attrArray, text, length);
	}

	int
	drawHyperText (
		const TextAttrAnchorArray* attrArray,
		const char* text, 
		size_t length = -1
		)
	{
		return drawHyperText_utf8 (attrArray, text, length);
	}

	int
	drawHyperText_utf8 (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const utf8_t* text, 
		size_t length = -1
		);

	int
	drawHyperText_utf8 (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const utf8_t* text, 
		size_t length = -1
		)
	{
		return drawHyperText_utf8 (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			text, 
			length
			);
	}

	int
	drawHyperText_utf8 (
		const TextAttrAnchorArray* attrArray,
		const utf8_t* text, 
		size_t length = -1
		)
	{
		return drawHyperText_utf8 (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			attrArray,
			text, 
			length
			);
	}

	int
	drawHyperText_utf32 (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const utf32_t* text, 
		size_t length = -1
		);

	int
	drawHyperText_utf32 (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return drawHyperText_utf32 (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			text, 
			length
			);
	}

	int
	drawHyperText_utf32 (
		const TextAttrAnchorArray* attrArray,
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return drawHyperText_utf32 (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			attrArray,
			text, 
			length
			);
	}

	// hypertext with selection

	int
	drawSelHyperText (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const char* text,
		size_t length = -1
		)
	{
		return drawSelHyperText_utf8 (
			textColor,
			backColor,
			fontFlags,
			attrArray, 
			selAttr,
			selStart, 
			selEnd, 
			text, 
			length
			);
	}

	int
	drawSelHyperText (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const char* text,
		size_t length = -1
		)
	{
		return drawSelHyperText_utf8 (
			attr,
			attrArray, 
			selAttr,
			selStart, 
			selEnd, 
			text, 
			length
			);
	}

	int
	drawSelHyperText (
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const char* text,
		size_t length = -1
		)
	{
		return drawSelHyperText_utf8 (
			attrArray, 
			selAttr,
			selStart, 
			selEnd, 
			text, 
			length
			);
	}

	int
	drawSelHyperText_utf8 (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const utf8_t* text, 
		size_t length = -1
		);

	int
	drawSelHyperText_utf8 (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const utf8_t* text, 
		size_t length = -1
		)
	{
		return drawSelHyperText_utf8 (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray, 
			selAttr,
			selStart, 
			selEnd, 
			text, 
			length
			);
	}

	int
	drawSelHyperText_utf8 (
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const utf8_t* text, 
		size_t length = -1
		)
	{
		return drawSelHyperText_utf8 (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			attrArray, 
			selAttr,
			selStart, 
			selEnd, 
			text, 
			length
			);
	}

	int
	drawSelHyperText_utf32 (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const utf32_t* text, 
		size_t length = -1
		);

	int
	drawSelHyperText_utf32 (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return drawSelHyperText_utf32 (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray, 
			selAttr,
			selStart, 
			selEnd, 
			text, 
			length
			);
	}

	int
	drawSelHyperText_utf32 (
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return drawSelHyperText_utf32 (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			attrArray, 
			selAttr,
			selStart, 
			selEnd, 
			text, 
			length
			);
	}

	// bin hex

	int
	drawBinHex (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		size_t halfBitOffset,
		const void* p,	
		size_t size
		);

	int
	drawBinHex (
		const TextAttr& attr,
		size_t halfBitOffset,
		const void* p,	
		size_t size
		)
	{
		return drawBinHex (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			halfBitOffset,
			p,	
			size
			);
	}

	int
	drawBinHex (
		size_t halfBitOffset,
		const void* p,	
		size_t size
		)
	{
		return drawBinHex (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			halfBitOffset,
			p,	
			size
			);
	}

	int
	drawHyperBinHex (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		size_t halfBitOffset,
		const void* p, 
		size_t size
		);

	int
	drawHyperBinHex (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		size_t halfBitOffset,
		const void* p, 
		size_t size
		)
	{
		return drawHyperBinHex (
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
	drawHyperBinHex (
		const TextAttrAnchorArray* attrArray,
		size_t halfBitOffset,
		const void* p, 
		size_t size
		)
	{
		return drawHyperBinHex (
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
	drawSelHyperBinHex (
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
	drawSelHyperBinHex (
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
		return drawSelHyperBinHex (
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
	drawSelHyperBinHex (
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		size_t halfBitOffset,
		const void* p, 
		size_t size
		)
	{
		return drawSelHyperBinHex (
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
	drawBinText (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		enc::CharCodec* codec,
		const void* p, 
		size_t dataSize,
		size_t bufferSize
		);

	int
	drawBinText (
		const TextAttr& attr,
		enc::CharCodec* codec,
		const void* p, 
		size_t dataSize,
		size_t bufferSize
		)
	{
		return drawBinText (
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
	drawBinText (
		enc::CharCodec* codec,
		const void* p, 
		size_t dataSize,
		size_t bufferSize
		)
	{
		return drawBinText (
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
	drawHyperBinText (
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
	drawHyperBinText (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		enc::CharCodec* codec,
		const void* p, 
		size_t dataSize,
		size_t bufferSize
		)
	{
		return drawHyperBinText (
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
	drawHyperBinText (
		const TextAttrAnchorArray* attrArray,
		enc::CharCodec* codec,
		const void* p, 
		size_t dataSize,
		size_t bufferSize
		)
	{
		return drawHyperBinText (
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
	drawSelHyperBinText (
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
	drawSelHyperBinText (
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
		return drawSelHyperBinText (
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
	drawSelHyperBinText (
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
		return drawSelHyperBinText (
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

protected:
	void
	init (Canvas* canvas = NULL);

	size_t
	buildBinTextBuffer (
		enc::CharCodec* codec,
		const void* p,
		size_t dataSize,
		size_t bufferSize
		);
};

//.............................................................................

} // namespace gui
} // namespace axl
