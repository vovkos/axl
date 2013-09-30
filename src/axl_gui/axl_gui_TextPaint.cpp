#include "pch.h"
#include "axl_gui_TextPaint.h"
#include "axl_rtl_HexEncoding.h"

namespace axl {
namespace gui {

//.............................................................................

void
CTextPaint::Init (ICanvas* pCanvas)
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
	m_MonospaceCharSize.m_Width = 0;
	m_MonospaceCharSize.m_Height = 0;
	m_HexEncodingFlags = 0; 
	m_UnprintableChar = '.';
}

// calc monospace/proportional text rect

TRect
CTextPaint::CalcTextRect (
	const char* pText,
	size_t Length
	)
{
	TRect Rect;

	Rect.m_Left = m_Point.m_x;
	Rect.m_Top = m_Top;
	Rect.m_Bottom = m_Bottom;

	if (m_MonospaceCharSize.m_Width)
	{
		Rect.m_Right = m_Point.m_x + Length * m_MonospaceCharSize.m_Width;
	}
	else
	{
		TSize Size = m_pCanvas->GetFont ()->CalcTextSize (pText, Length);
		Rect.m_Right = m_Point.m_x + Size.m_Width;
	}

	return Rect;
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
	if (m_MonospaceCharSize.m_Width)
		return PaintSpace_p (Length * m_MonospaceCharSize.m_Width, Color);

	TRect Rect = CalcTextRect (' ');
	return PaintSpace_p (Length * (Rect.m_Right - Rect.m_Left), Color);
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

	rtl::CHexEncoding::Encode (&m_BufferString, m_p, Size, 0);
	m_BufferString.Append (' ');
	
	size_t Length = m_BufferString.GetLength ();
	TRect Rect = CalcTextRect (m_BufferString, Length);
	m_pCanvas->DrawText (m_Point, Rect, m_BufferString, Length);

	m_p = (uchar_t*) m_p + Size;
	m_Point.m_x = Rect.m_Right;

	return m_Point.m_x;
}

// bin ascii

int
CTextPaint::PaintBinAsciiPart (size_t Size)
{
	size_t MaxSize = (uchar_t*) m_pEnd - (uchar_t*) m_p;
	if (Size > MaxSize)
		Size = MaxSize;

	if (!Size)
		return m_Point.m_x;

	m_BufferString.Copy ((char*) m_p, Size);
	size_t Length = m_BufferString.GetLength ();
	char* p = m_BufferString.GetBuffer ();

	for (size_t i = 0; i < Length; i++)
		if (!isprint (p [i]))
			p [i] = '.';

	TRect Rect = CalcTextRect (m_BufferString, Length);
	m_pCanvas->DrawText (m_Point, Rect, m_BufferString, Length);

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
CTextPaint::PaintBinAscii (
	const void* p,
	size_t Size
	)
{
	m_p = p;
	m_pBegin = p;
	m_pEnd = (uchar_t*) p + Size;

	return PaintBinAsciiPart (-1);
}

int
CTextPaint::PaintHyperText (
	const CTextAttrAnchorArray* pAttrArray,
	const char* pText,
	size_t Length
	)
{
	const TTextAttrAnchor* pAttr = *pAttrArray;
	size_t AttrCount = pAttrArray->GetCount ();

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
	const TTextAttrAnchor* pAttr = *pAttrArray;
	size_t AttrCount = pAttrArray->GetCount ();

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
CTextPaint::PaintHyperBinAscii (
	const CTextAttrAnchorArray* pAttrArray,
	const void* _p,
	size_t Size
	)
{
	const TTextAttrAnchor* pAttr = *pAttrArray;
	size_t AttrCount = pAttrArray->GetCount ();

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
		return PaintBinAsciiPart (Size);
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
			return PaintBinAsciiPart (MaxSize);

		PaintBinAsciiPart (Size);

		p = (uchar_t*) m_p;
		Offset = p - (uchar_t*) m_pBegin;

		m_pCanvas->m_DefTextAttr = pNextAttr->m_Attr;
		m_pAttr = pNextAttr;
		pNextAttr++;
	}

	if (p < pEnd)
		PaintBinAsciiPart (pEnd - p);

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
	size_t Count = pAttrArray->GetCount ();
	const TTextAttrAnchor* p = *pAttrArray;

	if (SelStart >= SelEnd) 
		return PaintHyperText (pAttrArray, pText, Length);

	if (Length == -1)
		Length = strlen (pText);

	m_SelOverlay = *pAttrArray;
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

	m_SelOverlay = *pAttrArray;
	m_SelOverlay.SetAttr (SelStart, SelEnd, m_SelAttr, -1);
	return PaintHyperBinHex (&m_SelOverlay, p, Size);
}

int
CTextPaint::PaintSelHyperBinAscii (
	const CTextAttrAnchorArray* pAttrArray,
	size_t SelStart,
	size_t SelEnd,
	const void* p,
	size_t Size
	)
{
	if (SelStart >= SelEnd) 
		return PaintHyperBinAscii (pAttrArray, p, Size);

	m_SelOverlay = *pAttrArray;
	m_SelOverlay.SetAttr (SelStart, SelEnd, m_SelAttr, -1);
	return PaintHyperBinAscii (&m_SelOverlay, p, Size);
}

//.............................................................................

} // namespace gui
} // namespace axl
