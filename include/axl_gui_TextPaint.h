// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_TEXTPAINT_H

#include "axl_gui_Canvas.h"
#include "axl_gui_TextAttrAnchorArray.h"
#include "axl_rtl_String.h"

namespace axl {
namespace gui {

//.............................................................................

class CTextPaint
{
protected:
	CTextAttrAnchorArray m_SelOverlay;
	
	// for binhex / binascii
	
	rtl::CString m_BufferString; 

	const void* m_p;
	const void* m_pBegin;
	const void* m_pEnd;
	
	// hyper text

	const TTextAttrAnchor* m_pAttr;
	const TTextAttrAnchor* m_pAttrBegin;
	const TTextAttrAnchor* m_pAttrEnd;

public:
	ÑCanvas* m_pCanvas;
	TTextAttr m_SelAttr;
	uint_t m_HexEncodingFlags;
	char m_UnprintableChar;
	TSize m_MonospaceCharSize; 

	TPoint m_Point;
	int m_Top;
	int m_Bottom;

public:
	CTextPaint ()
	{
		Init ();
	}

	CTextPaint (ÑCanvas* pCanvas)
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
	PaintBinAscii (
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
	PaintHyperBinAscii (
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
	PaintSelHyperBinAscii (
		const CTextAttrAnchorArray* pAttrArray, 
		size_t SelStart, 
		size_t SelEnd, 
		const void* p, 
		size_t Size
		);

protected:
	void
	Init (ÑCanvas* pCanvas = NULL);

	TRect
	CalcTextRect (
		const char* pText,
		size_t Length = -1
		);

	TRect
	CalcTextRect (char Char)
	{
		return CalcTextRect (&Char, 1);
	}

	int
	PaintTextPart (size_t Length);

	int
	PaintBinHexPart (size_t Size);

	int
	PaintBinAsciiPart (size_t Size);

};

//.............................................................................

} // namespace gui
} // namespace axl
