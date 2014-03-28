#include "pch.h"
#include "axl_gui_TextPaint.h"
#include "axl_rtl_HexEncoding.h"

namespace axl {
namespace gui {

//.............................................................................

void
CTextPaint::Init (CCanvas* pCanvas)
{
	m_pCanvas = pCanvas;
	m_p = NULL;
	m_pBegin = NULL;
	m_pEnd = NULL;
	m_pAttr = NULL;
	m_pAttrBegin = NULL;
	m_pAttrEnd = NULL;
	m_Top = 0;
	m_Bottom = 0;
	m_HexEncodingFlags = 0; 
	m_UnprintableChar = '.';
}

// calc monospace/proportional text rect

TRect
CTextPaint::CalcTextRect_utf8 (
	const utf8_t* pText,
	size_t Length
	)
{
	CFont* pFont = m_pCanvas->m_pBaseFont->GetFontMod (m_pCanvas->m_DefTextAttr.m_FontFlags);
	TSize Size = pFont->CalcTextSize_utf8 (pText, Length);
	return TRect (m_Point.m_x, m_Top, m_Point.m_x + Size.m_Width, m_Bottom);
}

TRect
CTextPaint::CalcTextRect_utf32 (
	const utf32_t* pText,
	size_t Length
	)
{
	CFont* pFont = m_pCanvas->m_pBaseFont->GetFontMod (m_pCanvas->m_DefTextAttr.m_FontFlags);
	TSize Size = pFont->CalcTextSize_utf32 (pText, Length);
	return TRect (m_Point.m_x, m_Top, m_Point.m_x + Size.m_Width, m_Bottom);
}

// gap

int 
CTextPaint::PaintSpace_p (
	int Width,
	uint_t Color
	)
{
	TRect Rect;
	Rect.m_Left = m_Point.m_x;
	Rect.m_Top = m_Top;
	Rect.m_Right = m_Point.m_x + Width;
	Rect.m_Bottom = m_Bottom;

	m_pCanvas->DrawRect (Rect, Color);

	m_Point.m_x = Rect.m_Right;
	return m_Point.m_x;
}

int 
CTextPaint::PaintSpace (
	size_t Length,
	uint_t Color
	)
{
	CFont* pFont = m_pCanvas->m_pBaseFont->GetFontMod (m_pCanvas->m_DefTextAttr.m_FontFlags);
	TSize Size = pFont->CalcTextSize (" ", 1);
	return PaintSpace_p (Length * Size.m_Width, Color);
}

// text

int
CTextPaint::PaintTextPart (size_t Length)
{
	char* p = (char*) m_p;

	size_t MaxLength = (char*) m_pEnd - p;
	if (Length > MaxLength)
		Length = MaxLength;

	if (!Length)
		return m_Point.m_x;

	TRect Rect = CalcTextRect (p, Length);
	m_pCanvas->DrawText (m_Point, Rect, p, Length);
	
	m_p = p + Length;
	m_Point.m_x = Rect.m_Right;

	return m_Point.m_x;
}

// bin hex

int
CTextPaint::PaintBinHexPart (size_t Size)
{
	size_t MaxSize = (uchar_t*) m_pEnd - (uchar_t*) m_p;
	if (Size > MaxSize)
		Size = MaxSize;

	if (!Size)
		return m_Point.m_x;

	rtl::CHexEncoding::Encode (&m_StringBuffer, m_p, Size, 0);
	m_StringBuffer.Append (' ');
	
	size_t Length = m_StringBuffer.GetLength ();
	TRect Rect = CalcTextRect (m_StringBuffer, Length);
	m_pCanvas->DrawText (m_Point, Rect, m_StringBuffer, Length);

	m_p = (uchar_t*) m_p + Size;
	m_Point.m_x = Rect.m_Right;

	return m_Point.m_x;
}

// bin text

int
CTextPaint::PaintBinTextPart (
	rtl::CCharCodec* pCodec,
	size_t Size
	)
{
	size_t MaxSize = (uchar_t*) m_pEnd - (uchar_t*) m_p;
	if (Size > MaxSize)
		Size = MaxSize;

	if (!Size)
		return m_Point.m_x;

	m_BinTextBuffer.SetCount (Size);

	size_t i = 0;
	const char* p = (char*) m_p;
	const char* pEnd = p + Size;
	while (p < pEnd)
	{
		utf32_t CodePoint;
		size_t TakenBufferLength;
		size_t TakenSize;
		size_t ExpectedSize;

		size_t Leftover = pEnd - p;

		pCodec->DecodeToUtf32 (&CodePoint, 1, p, Leftover, &TakenBufferLength, &TakenSize, &ExpectedSize);
		if (!TakenSize)
		{
			size_t End = i + Leftover;

			Leftover = (char*) m_pEnd - p;
			if (ExpectedSize <= Leftover)
			{
				pCodec->DecodeToUtf32 (&CodePoint, 1, p, Leftover, &TakenBufferLength);
				ASSERT (TakenBufferLength == 1);

				m_BinTextBuffer [i] = iswprint (CodePoint) ? CodePoint : m_UnprintableChar;
				i++;
			}

			for (; i < End; i++)
				m_BinTextBuffer [i] = m_UnprintableChar;

			break;
		}

		m_BinTextBuffer [i] = iswprint (CodePoint) ? CodePoint : m_UnprintableChar;

		size_t End = i + TakenSize;
		for (i++; i < End; i++)
			m_BinTextBuffer [i] = m_UnprintableChar;

		p += TakenSize;
	}

	ASSERT (i == Size);
	
	TRect Rect = CalcTextRect_utf32 (m_BinTextBuffer, Size);
	m_pCanvas->DrawText_utf32 (m_Point, Rect, m_BinTextBuffer, Size);

	m_p = (uchar_t*) m_p + Size;
	m_Point.m_x = Rect.m_Right;

	return m_Point.m_x;
}

// public paint functions

int
CTextPaint::PaintText (
	const char* pText,
	size_t Length
	)
{
	if (Length == -1)
		Length = strlen (pText);
	
	m_p = pText;
	m_pBegin = pText;
	m_pEnd = pText + Length;

	return PaintTextPart (-1);
}

int
CTextPaint::PaintBinHex (
	const void* p,
	size_t Size
	)
{
	m_p = p;
	m_pBegin = p;
	m_pEnd = (uchar_t*) p + Size;

	return PaintBinHexPart (-1);
}

int
CTextPaint::PaintBinText (
	rtl::CCharCodec* pCodec,
	const void* p,
	size_t Size
	)
{
	m_p = p;
	m_pBegin = p;
	m_pEnd = (uchar_t*) p + Size;

	return PaintBinTextPart (pCodec, -1);
}

int
CTextPaint::PaintHyperText (
	const CTextAttrAnchorArray* pAttrArray,
	const char* pText,
	size_t Length
	)
{
	const TTextAttrAnchor* pAttr = pAttrArray ? pAttrArray->ca () : NULL;
	size_t AttrCount = pAttrArray ? pAttrArray->GetCount () : 0;

	const char* p = pText;
	const char* pEnd; 

	size_t Offset = 0;

	if (Length == -1)
		Length = strlen (pText);
	
	m_p = pText;
	m_pBegin = pText;
	m_pEnd = pText + Length;
	m_pAttr = pAttr;
	m_pAttrBegin = pAttr;
	m_pAttrEnd = pAttr + AttrCount;

	if (!Length)
		return m_Point.m_x;

	if (!AttrCount)
	{
		m_pCanvas->m_DefTextAttr.Clear ();
		return PaintTextPart (Length);
	}

	const TTextAttrAnchor* pNextAttr;

	if (m_pAttr->m_Offset <= Offset)
	{
		m_pCanvas->m_DefTextAttr = m_pAttr->m_Attr;
		pNextAttr = m_pAttr + 1;
	}
	else
	{
		m_pCanvas->m_DefTextAttr.Clear ();
		pNextAttr = m_pAttr;
	}

	pEnd = p + Length;
	while (p < pEnd && pNextAttr < m_pAttrEnd)
	{	
		size_t MaxLength = pEnd - p;

		Length = pNextAttr->m_Offset - Offset;
		if (Length > MaxLength)
			return PaintTextPart (MaxLength);

		PaintTextPart (Length);

		m_pCanvas->m_DefTextAttr = pNextAttr->m_Attr;
		m_pAttr = pNextAttr;
		pNextAttr++;

		p = (char*) m_p;
		Offset = p - (char*) m_pBegin;
	}

	if (p < pEnd)
		PaintTextPart (pEnd - p);

	return m_Point.m_x;
}

int
CTextPaint::PaintHyperBinHex (
	const CTextAttrAnchorArray* pAttrArray,
	const void* _p,
	size_t Size
	)
{
	const TTextAttrAnchor* pAttr = pAttrArray ? pAttrArray->ca () : NULL;
	size_t AttrCount = pAttrArray ? pAttrArray->GetCount () : 0;

	const uchar_t* p = (uchar_t*) _p;
	const uchar_t* pEnd; 

	size_t Offset = 0;

	m_p = p;
	m_pBegin = p;
	m_pEnd = p + Size;
	m_pAttr = pAttr;
	m_pAttrBegin = pAttr;
	m_pAttrEnd = pAttr + AttrCount;

	if (!Size)
		return m_Point.m_x;

	if (!AttrCount)
	{
		m_pCanvas->m_DefTextAttr.Clear ();
		return PaintBinHexPart (Size);
	}

	const TTextAttrAnchor* pNextAttr;

	if (m_pAttr->m_Offset <= Offset)
	{
		m_pCanvas->m_DefTextAttr = m_pAttr->m_Attr;
		pNextAttr = m_pAttr + 1;
	}
	else
	{
		m_pCanvas->m_DefTextAttr.Clear ();
		pNextAttr = m_pAttr;
	}

	pEnd = p + Size;
	while (p < pEnd && pNextAttr < m_pAttrEnd)
	{	
		size_t MaxSize = pEnd - p;

		Size = pNextAttr->m_Offset - Offset;
		if (Size > MaxSize)
			return PaintBinHexPart (MaxSize);

		PaintBinHexPart (Size);

		p = (uchar_t*) m_p;
		Offset = p - (uchar_t*) m_pBegin;

		m_pCanvas->m_DefTextAttr = pNextAttr->m_Attr;
		m_pAttr = pNextAttr;
		pNextAttr++;
	}

	if (p < pEnd)
		PaintBinHexPart (pEnd - p);

	return m_Point.m_x;
}

int
CTextPaint::PaintHyperBinText (
	rtl::CCharCodec* pCodec,
	const CTextAttrAnchorArray* pAttrArray,
	const void* _p,
	size_t Size
	)
{
	const TTextAttrAnchor* pAttr = pAttrArray ? pAttrArray->ca () : NULL;
	size_t AttrCount = pAttrArray ? pAttrArray->GetCount () : 0;

	const uchar_t* p = (uchar_t*) _p;
	const uchar_t* pEnd; 

	size_t Offset = 0;

	m_p = p;
	m_pBegin = p;
	m_pEnd = p + Size;
	m_pAttr = pAttr;
	m_pAttrBegin = pAttr;
	m_pAttrEnd = pAttr + AttrCount;

	if (!Size)
		return m_Point.m_x;

	if (!AttrCount)
	{
		m_pCanvas->m_DefTextAttr.Clear ();
		return PaintBinTextPart (pCodec, Size);
	}

	const TTextAttrAnchor* pNextAttr;

	if (m_pAttr->m_Offset <= Offset)
	{
		m_pCanvas->m_DefTextAttr = m_pAttr->m_Attr;
		pNextAttr = m_pAttr + 1;
	}
	else
	{
		m_pCanvas->m_DefTextAttr.Clear ();
		pNextAttr = m_pAttr;
	}

	pEnd = p + Size;
	while (p < pEnd && pNextAttr < m_pAttrEnd)
	{	
		size_t MaxSize = pEnd - p;

		Size = pNextAttr->m_Offset - Offset;
		if (Size > MaxSize)
			return PaintBinTextPart (pCodec, MaxSize);

		PaintBinTextPart (pCodec, Size);

		p = (uchar_t*) m_p;
		Offset = p - (uchar_t*) m_pBegin;

		m_pCanvas->m_DefTextAttr = pNextAttr->m_Attr;
		m_pAttr = pNextAttr;
		pNextAttr++;
	}

	if (p < pEnd)
		PaintBinTextPart (pCodec, pEnd - p);

	return m_Point.m_x;
}

int
CTextPaint::PaintSelHyperText (
	const CTextAttrAnchorArray* pAttrArray,
	size_t SelStart,
	size_t SelEnd,
	const char* pText,
	size_t Length
	)
{
	if (SelStart >= SelEnd) 
		return PaintHyperText (pAttrArray, pText, Length);

	if (Length == -1)
		Length = strlen (pText);

	if (pAttrArray)
		m_SelOverlay = *pAttrArray;
	else
		m_SelOverlay.Clear ();

	m_SelOverlay.SetAttr (SelStart, SelEnd, m_SelAttr, -1);
	return PaintHyperText (&m_SelOverlay, pText, Length);
}

int
CTextPaint::PaintSelHyperBinHex (
	const CTextAttrAnchorArray* pAttrArray,
	size_t SelStart,
	size_t SelEnd,
	const void* p,
	size_t Size
	)
{
	if (SelStart >= SelEnd) 
		return PaintHyperBinHex (pAttrArray, p, Size);

	if (pAttrArray)
		m_SelOverlay = *pAttrArray;
	else
		m_SelOverlay.Clear ();

	m_SelOverlay.SetAttr (SelStart, SelEnd, m_SelAttr, -1);
	return PaintHyperBinHex (&m_SelOverlay, p, Size);
}

int
CTextPaint::PaintHyperBinHex4BitCursor (
	const CTextAttrAnchorArray* pAttrArray, 
	size_t CursorPos, 
	const void* p, 
	size_t Size
	)
{
	if (CursorPos >= Size) 
		return PaintHyperBinHex (pAttrArray, p, Size);

	PaintHyperBinHex (pAttrArray, p, CursorPos);

	uchar_t* pCursor = (uchar_t*) p + CursorPos;
	
	gui::TTextAttr Attr = m_pCanvas->m_DefTextAttr;

	char Char = rtl::CHexEncoding::GetHexChar_l (*pCursor & 0xf);
	char CharBuffer [8] = { Char, ' ', ' ', 0 };
	TRect Rect = CalcTextRect (CharBuffer, 3);
	m_pCanvas->m_DefTextAttr.Overlay (m_SelAttr);
	m_pCanvas->DrawText (m_Point, Rect, CharBuffer, 3);

	m_Point.m_x = Rect.m_Right;

	if (CursorPos + 1 < Size)
	{
		size_t Leftover = Size - CursorPos - 1;

		m_p = (uchar_t*) m_p + CursorPos + 1;
		m_pCanvas->m_DefTextAttr = Attr;

		if (!pAttrArray)
		{
			PaintHyperBinHex (NULL, pCursor + 1, Leftover);
		}
		else
		{
			m_SelOverlay = *pAttrArray;
			m_SelOverlay.ClearBefore (CursorPos);
			PaintHyperBinHex (&m_SelOverlay, pCursor + 1, Leftover);
		}		
	}

	return m_Point.m_x;
}

int
CTextPaint::PaintSelHyperBinText (
	rtl::CCharCodec* pCodec,
	const CTextAttrAnchorArray* pAttrArray,
	size_t SelStart,
	size_t SelEnd,
	const void* p,
	size_t Size
	)
{
	if (SelStart >= SelEnd) 
		return PaintHyperBinText (pCodec, pAttrArray, p, Size);

	if (pAttrArray)
		m_SelOverlay = *pAttrArray;
	else
		m_SelOverlay.Clear ();

	m_SelOverlay.SetAttr (SelStart, SelEnd, m_SelAttr, -1);
	return PaintHyperBinText (pCodec, &m_SelOverlay, p, Size);
}

//.............................................................................

} // namespace gui
} // namespace axl
