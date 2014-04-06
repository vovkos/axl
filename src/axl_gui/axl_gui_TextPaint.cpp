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

// space

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
CTextPaint::PaintText_utf8 (
	const utf8_t* pText,
	size_t Length
	)
{
	if (Length == -1)
		Length = strlen (pText);
	
	m_p = pText;
	m_pBegin = pText;
	m_pEnd = pText + Length;

	return PaintTextPart_utf8 (Length);
}

int
CTextPaint::PaintText_utf32 (
	const utf32_t* pText,
	size_t Length
	)
{
	if (Length == -1)
		Length = rtl::CStringDetails_utf32::CalcLength (pText);
	
	m_p = (char*) pText;
	m_pBegin = (char*) pText;
	m_pEnd = (char*) (pText + Length);

	return PaintTextPart_utf32 (Length);
}

int
CTextPaint::PaintTextPart_utf8 (size_t Length)
{
	ASSERT (Length <= (size_t) (m_pEnd - m_p));

	if (!Length)
		return m_Point.m_x;

	TRect Rect = CalcTextRect_utf8 (m_p, Length);
	m_pCanvas->DrawText_utf8 (m_Point, Rect, m_p, Length);
	
	m_p += Length;
	m_Point.m_x = Rect.m_Right;

	return m_Point.m_x;
}

int
CTextPaint::PaintTextPart_utf32 (size_t Length)
{
	ASSERT (Length <= (size_t) ((m_pEnd - m_p) / sizeof (utf32_t)));

	if (!Length)
		return m_Point.m_x;

	TRect Rect = CalcTextRect_utf32 ((utf32_t*) m_p, Length);
	m_pCanvas->DrawText_utf32 (m_Point, Rect, (utf32_t*) m_p, Length);
	
	m_p += Length * sizeof (utf32_t);
	m_Point.m_x = Rect.m_Right;

	return m_Point.m_x;
}

// hyper text

int
CTextPaint::PaintHyperText_utf8 (
	const CTextAttrAnchorArray* pAttrArray,
	const utf8_t* pText,
	size_t Length
	)
{
	const TTextAttrAnchor* pAttr = pAttrArray ? pAttrArray->ca () : NULL;
	size_t AttrCount = pAttrArray ? pAttrArray->GetCount () : 0;

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
		return PaintTextPart_utf8 (Length);
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

	const char* p = pText;
	const char* pEnd = p + Length;

	while (p < pEnd && pNextAttr < m_pAttrEnd)
	{	
		size_t MaxLength = pEnd - p;

		Length = pNextAttr->m_Offset - Offset;
		if (Length > MaxLength)
			return PaintTextPart_utf8 (MaxLength);

		PaintTextPart_utf8 (Length);

		m_pCanvas->m_DefTextAttr = pNextAttr->m_Attr;
		m_pAttr = pNextAttr;
		pNextAttr++;

		p = m_p;
		Offset = p - m_pBegin;
	}

	if (p < pEnd)
		PaintTextPart_utf8 (pEnd - p);

	return m_Point.m_x;
}

int
CTextPaint::PaintHyperText_utf32 (
	const CTextAttrAnchorArray* pAttrArray,
	const utf32_t* pText,
	size_t Length
	)
{
	const TTextAttrAnchor* pAttr = pAttrArray ? pAttrArray->ca () : NULL;
	size_t AttrCount = pAttrArray ? pAttrArray->GetCount () : 0;

	size_t Offset = 0;

	if (Length == -1)
		Length = rtl::CStringDetails_utf32::CalcLength (pText);
	
	m_p = (char*) pText;
	m_pBegin = (char*) pText;
	m_pEnd = (char*) (pText + Length);
	m_pAttr = pAttr;
	m_pAttrBegin = pAttr;
	m_pAttrEnd = pAttr + AttrCount;

	if (!Length)
		return m_Point.m_x;

	if (!AttrCount)
	{
		m_pCanvas->m_DefTextAttr.Clear ();
		return PaintTextPart_utf32 (Length);
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

	const utf32_t* p = pText;
	const utf32_t* pEnd = p + Length;

	while (p < pEnd && pNextAttr < m_pAttrEnd)
	{	
		size_t MaxLength = pEnd - p;

		Length = pNextAttr->m_Offset - Offset;
		if (Length > MaxLength)
			return PaintTextPart_utf32 (MaxLength);

		PaintTextPart_utf32 (Length);

		m_pCanvas->m_DefTextAttr = pNextAttr->m_Attr;
		m_pAttr = pNextAttr;
		pNextAttr++;

		p = (utf32_t*) m_p;
		Offset = p - (utf32_t*) m_pBegin;
	}

	if (p < pEnd)
		PaintTextPart_utf32 (pEnd - p);

	return m_Point.m_x;
}

int
CTextPaint::PaintSelHyperText_utf8 (
	const CTextAttrAnchorArray* pAttrArray,
	size_t SelStart,
	size_t SelEnd,
	const utf8_t* pText,
	size_t Length
	)
{
	if (SelStart >= SelEnd) 
		return PaintHyperText_utf8 (pAttrArray, pText, Length);

	if (Length == -1)
		Length = strlen (pText);

	if (pAttrArray)
		m_SelOverlay.Copy (*pAttrArray, pAttrArray->GetCount ());
	else
		m_SelOverlay.Clear ();

	m_SelOverlay.SetAttr (SelStart, SelEnd, m_SelAttr, -1);
	return PaintHyperText_utf8 (&m_SelOverlay, pText, Length);
}

int
CTextPaint::PaintSelHyperText_utf32 (
	const CTextAttrAnchorArray* pAttrArray,
	size_t SelStart,
	size_t SelEnd,
	const utf32_t* pText,
	size_t Length
	)
{
	if (SelStart >= SelEnd) 
		return PaintHyperText_utf32 (pAttrArray, pText, Length);

	if (Length == -1)
		Length = rtl::CStringDetails_utf32::CalcLength (pText);

	if (pAttrArray)
		m_SelOverlay.Copy (*pAttrArray, pAttrArray->GetCount ());
	else
		m_SelOverlay.Clear ();

	m_SelOverlay.SetAttr (SelStart, SelEnd, m_SelAttr, -1);
	return PaintHyperText_utf32 (&m_SelOverlay, pText, Length);
}

// bin hex

int
CTextPaint::PaintBinHexPart (size_t Size)
{
	size_t MaxSize = m_pEnd - m_p;
	if (Size > MaxSize)
		Size = MaxSize;

	if (!Size)
		return m_Point.m_x;

	rtl::CHexEncoding::Encode (&m_StringBuffer, m_p, Size, 0);
	m_StringBuffer.Append (' ');
	
	size_t Length = m_StringBuffer.GetLength ();
	TRect Rect = CalcTextRect_utf8 (m_StringBuffer, Length);
	m_pCanvas->DrawText (m_Point, Rect, m_StringBuffer, Length);

	m_p += Size;
	m_Point.m_x = Rect.m_Right;

	return m_Point.m_x;
}

int
CTextPaint::PaintBinHex (
	const void* p,
	size_t Size
	)
{
	m_p = (const char*) p;
	m_pBegin = m_p;
	m_pEnd = m_p + Size;

	return PaintBinHexPart (-1);
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

	const char* p = (char*) _p;
	const char* pEnd; 

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

		p = m_p;
		Offset = p - m_pBegin;

		m_pCanvas->m_DefTextAttr = pNextAttr->m_Attr;
		m_pAttr = pNextAttr;
		pNextAttr++;
	}

	if (p < pEnd)
		PaintBinHexPart (pEnd - p);

	return m_Point.m_x;
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
		m_SelOverlay.Copy (*pAttrArray, pAttrArray->GetCount ());
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

	char* pCursor = (char*) p + CursorPos;
	
	gui::TTextAttr Attr = m_pCanvas->m_DefTextAttr;

	char Char = rtl::CHexEncoding::GetHexChar_l (*pCursor & 0xf);
	char CharBuffer [8] = { Char, ' ', ' ', 0 };
	TRect Rect = CalcTextRect_utf8 (CharBuffer, 3);
	m_pCanvas->m_DefTextAttr.Overlay (m_SelAttr);
	m_pCanvas->DrawText (m_Point, Rect, CharBuffer, 3);

	m_Point.m_x = Rect.m_Right;

	if (CursorPos + 1 < Size)
	{
		size_t Leftover = Size - CursorPos - 1;

		m_p = m_p + CursorPos + 1;
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

// bin text

int
CTextPaint::PaintBinTextPart (
	rtl::CCharCodec* pCodec,
	size_t Size
	)
{
	size_t MaxSize = m_pEnd - m_p;
	if (Size > MaxSize)
		Size = MaxSize;

	if (!Size)
		return m_Point.m_x;

	m_BinTextBuffer.SetCount (Size);

	size_t UnitSize = pCodec->GetUnitSize ();
	size_t i = 0;

	const char* p = m_p;
	const char* pEnd = p + Size;

	size_t UnitSizeMod = (p - m_pBegin) % UnitSize;
	if (UnitSizeMod)
	{
		size_t IncompleteSize = UnitSize - UnitSizeMod;

		if (IncompleteSize > Size)
			IncompleteSize = Size;

		for (; i < IncompleteSize; i++)
			m_BinTextBuffer [i] = m_UnprintableChar;

		p += IncompleteSize;
	}

	while (p < pEnd)
	{
		utf32_t CodePoint;
		size_t TakenBufferLength = 0;
		size_t TakenSize = 0;
		size_t ExpectedSize;

		size_t Leftover = pEnd - p;
		if (Leftover < UnitSize)
			ExpectedSize = UnitSize;
		else
			pCodec->DecodeToUtf32 (&CodePoint, 1, p, Leftover, &TakenBufferLength, &TakenSize, &ExpectedSize);

		if (!TakenSize)
		{
			size_t End = i + Leftover;

			Leftover = m_pEnd - p;
			if (ExpectedSize <= Leftover)
			{
				pCodec->DecodeToUtf32 (&CodePoint, 1, p, Leftover, &TakenBufferLength);
				
				if (TakenBufferLength == 1) // might still be not enough (e.g. UTF-16)
				{
					m_BinTextBuffer [i] = iswprint (CodePoint) ? CodePoint : m_UnprintableChar;
					i++;
				}
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

	m_p = m_p + Size;
	m_Point.m_x = Rect.m_Right;

	return m_Point.m_x;
}

int
CTextPaint::PaintBinText (
	rtl::CCharCodec* pCodec,
	const void* p,
	size_t Size
	)
{
	m_p = (const char*) p;
	m_pBegin = m_p;
	m_pEnd = m_p + Size;

	return PaintBinTextPart (pCodec, -1);
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

	const char* p = (const char*) _p;
	const char* pEnd; 

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

		p = m_p;
		Offset = p - m_pBegin;

		m_pCanvas->m_DefTextAttr = pNextAttr->m_Attr;
		m_pAttr = pNextAttr;
		pNextAttr++;
	}

	if (p < pEnd)
		PaintBinTextPart (pCodec, pEnd - p);

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
		m_SelOverlay.Copy (*pAttrArray, pAttrArray->GetCount ());
	else
		m_SelOverlay.Clear ();

	m_SelOverlay.SetAttr (SelStart, SelEnd, m_SelAttr, -1);
	return PaintHyperBinText (pCodec, &m_SelOverlay, p, Size);
}

//.............................................................................

} // namespace gui
} // namespace axl
