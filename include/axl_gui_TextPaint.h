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
	CTextAttrAnchorArray m_SelOverlay;
	
	// for bin-hex / bin-text
	
	rtl::CString m_StringBuffer; 
	rtl::CArrayT <utf32_t> m_BinTextBuffer; 

	const char* m_p;
	const char* m_pBegin;
	const char* m_pEnd;
	
	// hyper text

	const TTextAttrAnchor* m_pAttr;
	const TTextAttrAnchor* m_pAttrBegin;
	const TTextAttrAnchor* m_pAttrEnd;

public:
	CCanvas* m_pCanvas;
	TTextAttr m_SelAttr;
	uint_t m_HexEncodingFlags;
	char m_UnprintableChar;

	TPoint m_Point;
	int m_Top;
	int m_Bottom;

public:
	CTextPaint ()
	{
		Init ();
	}

	CTextPaint (CCanvas* pCanvas)
	{
		Init (pCanvas);
	}

	TPoint
	GetPoint ()
	{
		return m_Point;
	}

	int
	PaintSpace (
		size_t Length,
		uint_t Color
		);

	int
	PaintSpace (size_t Length)
	{
		return PaintSpace (Length, m_pCanvas->m_DefTextAttr.m_BackColor);
	}

	int
	PaintSpace_p (
		int Width, // in pixels
		uint_t Color
		); 

	int
	PaintSpace_p (int Width) // in pixels
	{
		return PaintSpace_p (Width, m_pCanvas->m_DefTextAttr.m_BackColor);
	}

	int
	PaintText (
		const char* pText, 
		size_t Length = -1
		);

	int
	PaintText (const rtl::CString& Text)
	{
		return PaintText (Text, Text.GetLength ());
	}

	int
	PaintBinHex (
		const void* p,	
		size_t Size
		);

	int
	PaintBinText (
		rtl::CCharCodec* pCodec,
		const void* p, 
		size_t Size
		);

	int
	PaintHyperText (
		const CTextAttrAnchorArray* pAttrArray,
		const char* pText, 
		size_t Length = -1
		);

	int
	PaintHyperText (
		const CTextAttrAnchorArray* pAttrArray,
		const rtl::CString& Text
		)
	{
		return PaintHyperText (pAttrArray, Text, Text.GetLength ());
	}

	int
	PaintHyperBinHex (
		const CTextAttrAnchorArray* pAttrArray,
		const void* p, 
		size_t Size
		);

	int
	PaintHyperBinText (
		rtl::CCharCodec* pCodec,
		const CTextAttrAnchorArray* pAttrArray,
		const void* p, 
		size_t Size
		);

	int
	PaintSelHyperText (
		const CTextAttrAnchorArray* pAttrArray, 
		size_t SelStart, 
		size_t SelEnd, 
		const char* pText, 
		size_t Length = -1
		);

	int
	PaintSelHyperText (
		const CTextAttrAnchorArray* pAttrArray, 
		size_t SelStart, 
		size_t SelEnd, 
		const rtl::CString& Text
		)
	{
		return PaintSelHyperText (pAttrArray, SelStart, SelEnd, Text, Text.GetLength ());
	}

	int
	PaintSelHyperBinHex (
		const CTextAttrAnchorArray* pAttrArray, 
		size_t SelStart, 
		size_t SelEnd, 
		const void* p, 
		size_t Size
		);

	int
	PaintHyperBinHex4BitCursor (
		const CTextAttrAnchorArray* pAttrArray, 
		size_t CursorPos, 
		const void* p, 
		size_t Size
		);

	int
	PaintSelHyperBinText (
		rtl::CCharCodec* pCodec,
		const CTextAttrAnchorArray* pAttrArray, 
		size_t SelStart, 
		size_t SelEnd, 
		const void* p, 
		size_t Size
		);

protected:
	void
	Init (CCanvas* pCanvas = NULL);

	TRect
	CalcTextRect (
		const char* pText,
		size_t Length = -1
		)
	{
		return CalcTextRect_utf8 (pText, Length);
	}

	TRect
	CalcTextRect_utf8 (
		const utf8_t* pText,
		size_t Length = -1
		);

	TRect
	CalcTextRect_utf32 (
		const utf32_t* pText,
		size_t Length = -1
		);

	TRect
	CalcTextRect (utf32_t Char)
	{
		return CalcTextRect_utf32 (&Char, 1);
	}

	int
	PaintTextPart (size_t Length);

	int
	PaintBinHexPart (size_t Size);

	int
	PaintBinTextPart (
		rtl::CCharCodec* pCodec,
		size_t Size
		);
};

//.............................................................................

} // namespace gui
} // namespace axl
