// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_TEXTPAINT_H

#include "axl_gui_Canvas.h"
#include "axl_gui_TextAttrAnchorArray.h"
#include "axl_rtl_String.h"
#include "axl_rtl_CharCodec.h"

namespace axl {
namespace gui {

//.............................................................................

class CTextPaint
{
protected:
	CTextAttrAnchorArray m_selOverlay;
	
	// for bin-hex / bin-text
	
	rtl::CString m_stringBuffer; 
	rtl::CArrayT <utf32_t> m_binTextBuffer; 

	const char* m_p;
	const char* m_begin;
	const char* m_end;
	
	// hyper text

	const TTextAttrAnchor* m_attr;
	const TTextAttrAnchor* m_attrBegin;
	const TTextAttrAnchor* m_attrEnd;

public:
	CCanvas* m_canvas;
	TTextAttr m_selAttr;
	uint_t m_hexEncodingFlags;
	char m_unprintableChar;

	TPoint m_point;
	int m_top;
	int m_bottom;

public:
	CTextPaint ()
	{
		init ();
	}

	CTextPaint (CCanvas* canvas)
	{
		init (canvas);
	}

	static inline
	bool
	isPrintable (utf32_t c)
	{
		return c >= 0x20 && iswprint (c);
	}

	TPoint
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
		return paintSpace (length, m_canvas->m_defTextAttr.m_backColor);
	}

	int
	paintSpace_p (
		int width, // in pixels
		uint_t color
		); 

	int
	paintSpace_p (int width) // in pixels
	{
		return paintSpace_p (width, m_canvas->m_defTextAttr.m_backColor);
	}

	// text

	int
	paintText (const rtl::CString& text)
	{
		return paintText_utf8 (text, text.getLength ());
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
		const utf8_t* text, 
		size_t length = -1
		);

	int
	paintText_utf32 (
		const utf32_t* text, 
		size_t length = -1
		);

	// hypertext

	int
	paintHyperText (
		const CTextAttrAnchorArray* attrArray,
		const rtl::CString& text
		)
	{
		return paintHyperText_utf8 (attrArray, text, text.getLength ());
	}

	int
	paintHyperText (
		const CTextAttrAnchorArray* attrArray,
		const char* text, 
		size_t length = -1
		)
	{
		return paintHyperText_utf8 (attrArray, text, length);
	}

	int
	paintHyperText_utf8 (
		const CTextAttrAnchorArray* attrArray,
		const utf8_t* text, 
		size_t length = -1
		);

	int
	paintHyperText_utf32 (
		const CTextAttrAnchorArray* attrArray,
		const utf32_t* text, 
		size_t length = -1
		);

	// hypertext with selection

	int
	paintSelHyperText (
		const CTextAttrAnchorArray* attrArray, 
		size_t selStart, 
		size_t selEnd, 
		const rtl::CString& text
		)
	{
		return paintSelHyperText_utf8 (attrArray, selStart, selEnd, text, text.getLength ());
	}

	int
	paintSelHyperText_utf8 (
		const CTextAttrAnchorArray* attrArray, 
		size_t selStart, 
		size_t selEnd, 
		const utf8_t* text, 
		size_t length = -1
		);

	int
	paintSelHyperText_utf32 (
		const CTextAttrAnchorArray* attrArray, 
		size_t selStart, 
		size_t selEnd, 
		const utf32_t* text, 
		size_t length = -1
		);

	// bin hex

	int
	paintBinHex (
		const void* p,	
		size_t size
		);

	int
	paintHyperBinHex (
		const CTextAttrAnchorArray* attrArray,
		const void* p, 
		size_t size
		);

	int
	paintSelHyperBinHex (
		const CTextAttrAnchorArray* attrArray, 
		size_t selStart, 
		size_t selEnd, 
		const void* p, 
		size_t size
		);

	int
	paintHyperBinHex4BitCursor (
		const CTextAttrAnchorArray* attrArray, 
		size_t cursorPos, 
		const void* p, 
		size_t size
		);

	// bin text

	int
	paintBinText (
		rtl::CCharCodec* codec,
		const void* p, 
		size_t size
		);

	int
	paintHyperBinText (
		rtl::CCharCodec* codec,
		const CTextAttrAnchorArray* attrArray,
		const void* p, 
		size_t size
		);


	int
	paintSelHyperBinText (
		rtl::CCharCodec* codec,
		const CTextAttrAnchorArray* attrArray, 
		size_t selStart, 
		size_t selEnd, 
		const void* p, 
		size_t size
		);

protected:
	void
	init (CCanvas* canvas = NULL);

	TRect
	calcTextRect_utf8 (
		const utf8_t* text,
		size_t length = -1
		);

	TRect
	calcTextRect_utf32 (
		const utf32_t* text,
		size_t length = -1
		);

	TRect
	calcTextRect (utf32_t c)
	{
		return calcTextRect_utf32 (&c, 1);
	}

	int
	paintTextPart_utf8 (size_t length);

	int
	paintTextPart_utf32 (size_t length);

	int
	paintBinHexPart (size_t size);

	int
	paintBinTextPart (
		rtl::CCharCodec* codec,
		size_t size
		);
};

//.............................................................................

} // namespace gui
} // namespace axl
