// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_TEXTPAINT_H

#include "axl_gui_Canvas.h"
#include "axl_gui_TextAttrAnchorArray.h"
#include "axl_rtl_String.h"
#include "axl_enc_CharCodec.h"

namespace axl {
namespace gui {

//.............................................................................

class TextPaint
{
protected:
	TextAttrAnchorArray m_selOverlayBuffer;
	rtl::String m_stringBuffer; 
	rtl::Array <utf32_t> m_binTextBuffer; 

public:
	Canvas* m_canvas;
	Point m_point;
	int m_top;
	int m_bottom;
	uint_t m_hexEncodingFlags;
	char m_unprintableChar;

public:
	TextPaint ()
	{
		init ();
	}

	TextPaint (Canvas* canvas)
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
	paintSpace (
		size_t length,
		uint_t color
		);

	int
	paintSpace (size_t length)
	{
		return paintSpace (length, m_canvas->m_colorAttr.m_backColor);
	}

	int
	paintSpace_p (
		int width, // in pixels
		uint_t color
		); 

	int
	paintSpace_p (int width) // in pixels
	{
		return paintSpace_p (width, m_canvas->m_colorAttr.m_backColor);
	}

	// text

	int
	paintText (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const char* text, 
		size_t length = -1
		)
	{
		return paintText_utf8 (textColor, backColor, fontFlags, text, length);
	}

	int
	paintText (
		const TextAttr& attr,
		const char* text, 
		size_t length = -1
		)
	{
		return paintText_utf8 (attr, text, length);
	}

	int
	paintText (
		const char* text, 
		size_t length = -1
		)
	{
		return paintText_utf8 (text, length);
	}

	int
	paintText_utf8 (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const utf8_t* text, 
		size_t length = -1
		);

	int
	paintText_utf8 (
		const TextAttr& attr,
		const utf8_t* text,
		size_t length = -1
		)
	{
		return paintText_utf8 (
			attr.m_foreColor, 
			attr.m_backColor, 
			attr.m_fontFlags, 
			text, 
			length
			);
	}

	int
	paintText_utf8 (
		const utf8_t* text,
		size_t length = -1
		)
	{
		return paintText_utf8 (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			text, 
			length
			);
	}

	int
	paintText_utf32 (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const utf32_t* text, 
		size_t length = -1
		);

	int
	paintText_utf32 (
		const TextAttr& attr,
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return paintText_utf32 (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags, 
			text, 
			length
			);
	}

	int
	paintText_utf32 (
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return paintText_utf32 (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			text, 
			length
			);
	}

	// hypertext

	int
	paintHyperText (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const rtl::String& text
		)
	{
		return paintHyperText_utf8 (
			textColor, 
			backColor,
			fontFlags,
			attrArray, 
			text, 
			text.getLength ()
			);
	}

	int
	paintHyperText (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const char* text, 
		size_t length = -1
		)
	{
		return paintHyperText_utf8 (attr, attrArray, text, length);
	}

	int
	paintHyperText (
		const TextAttrAnchorArray* attrArray,
		const char* text, 
		size_t length = -1
		)
	{
		return paintHyperText_utf8 (attrArray, text, length);
	}

	int
	paintHyperText_utf8 (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const utf8_t* text, 
		size_t length = -1
		);

	int
	paintHyperText_utf8 (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const utf8_t* text, 
		size_t length = -1
		)
	{
		return paintHyperText_utf8 (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			text, 
			length
			);
	}

	int
	paintHyperText_utf8 (
		const TextAttrAnchorArray* attrArray,
		const utf8_t* text, 
		size_t length = -1
		)
	{
		return paintHyperText_utf8 (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			attrArray,
			text, 
			length
			);
	}

	int
	paintHyperText_utf32 (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		const utf32_t* text, 
		size_t length = -1
		);

	int
	paintHyperText_utf32 (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return paintHyperText_utf32 (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			text, 
			length
			);
	}

	int
	paintHyperText_utf32 (
		const TextAttrAnchorArray* attrArray,
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return paintHyperText_utf32 (
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
	paintSelHyperText (
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
		return paintSelHyperText_utf8 (
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
	paintSelHyperText (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const char* text,
		size_t length = -1
		)
	{
		return paintSelHyperText_utf8 (
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
	paintSelHyperText (
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const char* text,
		size_t length = -1
		)
	{
		return paintSelHyperText_utf8 (
			attrArray, 
			selAttr,
			selStart, 
			selEnd, 
			text, 
			length
			);
	}

	int
	paintSelHyperText_utf8 (
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
	paintSelHyperText_utf8 (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const utf8_t* text, 
		size_t length = -1
		)
	{
		return paintSelHyperText_utf8 (
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
	paintSelHyperText_utf8 (
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const utf8_t* text, 
		size_t length = -1
		)
	{
		return paintSelHyperText_utf8 (
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
	paintSelHyperText_utf32 (
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
	paintSelHyperText_utf32 (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return paintSelHyperText_utf32 (
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
	paintSelHyperText_utf32 (
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		const utf32_t* text, 
		size_t length = -1
		)
	{
		return paintSelHyperText_utf32 (
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
	paintBinHex (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		size_t halfBitOffset,
		const void* p,	
		size_t size
		);

	int
	paintBinHex (
		const TextAttr& attr,
		size_t halfBitOffset,
		const void* p,	
		size_t size
		)
	{
		return paintBinHex (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			halfBitOffset,
			p,	
			size
			);
	}

	int
	paintBinHex (
		size_t halfBitOffset,
		const void* p,	
		size_t size
		)
	{
		return paintBinHex (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			halfBitOffset,
			p,	
			size
			);
	}

	int
	paintHyperBinHex (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		size_t halfBitOffset,
		const void* p, 
		size_t size
		);

	int
	paintHyperBinHex (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		size_t halfBitOffset,
		const void* p, 
		size_t size
		)
	{
		return paintHyperBinHex (
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
	paintHyperBinHex (
		const TextAttrAnchorArray* attrArray,
		size_t halfBitOffset,
		const void* p, 
		size_t size
		)
	{
		return paintHyperBinHex (
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
	paintSelHyperBinHex (
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
	paintSelHyperBinHex (
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
		return paintSelHyperBinHex (
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
	paintSelHyperBinHex (
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		size_t halfBitOffset,
		const void* p, 
		size_t size
		)
	{
		return paintSelHyperBinHex (
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
	paintBinText (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		enc::CharCodec* codec,
		const void* p, 
		size_t size
		);

	int
	paintBinText (
		const TextAttr& attr,
		enc::CharCodec* codec,
		const void* p, 
		size_t size
		)
	{
		return paintBinText (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			codec,
			p, 
			size
			);
	}

	int
	paintBinText (
		enc::CharCodec* codec,
		const void* p, 
		size_t size
		)
	{
		paintBinText (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			codec,
			p, 
			size
			);
	}

	int
	paintHyperBinText (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray,
		enc::CharCodec* codec,
		const void* p, 
		size_t size
		);

	int
	paintHyperBinText (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray,
		enc::CharCodec* codec,
		const void* p, 
		size_t size
		)
	{
		return paintHyperBinText (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray,
			codec,
			p, 
			size
			);
	}

	int
	paintHyperBinText (
		const TextAttrAnchorArray* attrArray,
		enc::CharCodec* codec,
		const void* p, 
		size_t size
		)
	{
		return paintHyperBinText (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			attrArray,
			codec,
			p, 
			size
			);
	}

	int
	paintSelHyperBinText (
		uint_t textColor,
		uint_t backColor,
		uint_t fontFlags,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		enc::CharCodec* codec,
		const void* p, 
		size_t size
		);

	int
	paintSelHyperBinText (
		const TextAttr& attr,
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		enc::CharCodec* codec,
		const void* p, 
		size_t size
		)
	{
		return paintSelHyperBinText (
			attr.m_foreColor,
			attr.m_backColor,
			attr.m_fontFlags,
			attrArray, 
			selAttr,
			selStart, 
			selEnd, 
			codec,
			p, 
			size
			);
	}

	int
	paintSelHyperBinText (
		const TextAttrAnchorArray* attrArray, 
		const TextAttr& selAttr,
		size_t selStart, 
		size_t selEnd, 
		enc::CharCodec* codec,
		const void* p, 
		size_t size
		)
	{
		return paintSelHyperBinText (
			m_canvas->m_colorAttr.m_foreColor,
			m_canvas->m_colorAttr.m_backColor,
			-1, 
			attrArray, 
			selAttr,
			selStart, 
			selEnd, 
			codec,
			p, 
			size
			);
	}

protected:
	void
	init (Canvas* canvas = NULL);

	void
	buildBinTextBuffer (
		enc::CharCodec* codec,
		const void* p,
		size_t size
		);
};

//.............................................................................

} // namespace gui
} // namespace axl
