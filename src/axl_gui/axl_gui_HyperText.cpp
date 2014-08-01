#include "pch.h"
#include "axl_gui_HyperText.h"

namespace axl {
namespace gui {

//.............................................................................

void
CHyperText::Clear ()
{
	m_Source.Clear ();
	m_Text.Clear ();
	m_AttrArray.Clear ();
	m_HyperlinkArray.Clear ();
	m_HyperlinkXMap.Clear ();
}

size_t
CHyperText::Backspace (size_t BackLength)
{
	// don't touch m_Source!

	// TODO: backspace attributes and hyperlinks

	size_t Length = m_Text.GetLength ();
	if (BackLength >= Length)
	{
		m_Text.Clear ();
		return 0;
	}

	m_Text.ReduceLength (BackLength);
	return Length - BackLength;
}

size_t 
CHyperText::AppendPlainText (
	const char* pText, 
	size_t Length
	)
{
	if (Length == -1)
		Length = strlen (pText);

	m_Source.Append (pText, Length);
	return m_Text.Append (pText, Length);
}

size_t 
CHyperText::AppendPlainText (
	char Char, 
	size_t Count
	)
{
	m_Source.Append (Char, Count);
	return m_Text.Append (Char, Count);
}

size_t
CHyperText::AppendHyperText (
	const TTextAttr& BaseAttr,
	const char* pText,
	size_t Length
	)
{
	TTextAttr Attr = BaseAttr;

	const char* p = pText;
	const char* pEnd;
	
	size_t LastLength = m_Text.GetLength ();
	
	if (Length == -1)
		Length = strlen (pText);

	pEnd = p + Length;

	m_Source.Append (p, Length);

	for (;;)
	{
		const char* pTag;
		const char* pTagEnd;
		const char* pParam;

		pTag = strchr_e (p, pEnd, '<');
		if (!pTag)
		{
			m_Text.Append (p, pEnd - p);
			break;
		}

		m_Text.Append (p, pTag - p);

		pTagEnd = strchr_e (pTag + 1, pEnd, '>');
		if (!pTagEnd)
			break;

		Length = m_Text.GetLength ();
		m_AttrArray.SetAttr (LastLength, Length, Attr);
		LastLength = Length;

		pParam = strchr_e (pTag + 1, pTagEnd, '=');
		if (pParam)
		{
			Attr.Parse (pTag + 1);
			m_HyperlinkArray.OpenHyperlink (Length, pParam + 1, pTagEnd - pParam - 1);
		}
		else if (pTag[1] == '\b') // backspace
		{
			size_t BackLength = pTag[2] != '*' ? (size_t) strtoul (pTag + 2, NULL, 10) : -1;
			Backspace (BackLength ? BackLength : 1);
		}
		else 
		{
			Attr.Parse (pTag + 1);
			m_HyperlinkArray.CloseHyperlink (Length);
		}

		p = pTagEnd + 1;
	}

	Length = m_Text.GetLength ();
	m_AttrArray.SetAttr (LastLength, Length, Attr);
	return Length;
}

THyperlinkAnchor*
CHyperText::FindHyperlinkByX (int x) const
{
	THyperlinkAnchor* pResult = NULL;

	size_t Begin = 0;
	size_t End = m_HyperlinkXMap.GetCount ();

	while (Begin < End)
	{
		size_t Mid = (Begin + End) / 2;

		const THyperlinkXMapEntry* pMapEntry = &m_HyperlinkXMap [Mid];
		if (pMapEntry->m_x == x)
			return pMapEntry->m_pAnchor;

		if (pMapEntry->m_x < x)
		{
			pResult = pMapEntry->m_pAnchor;
			Begin = Mid + 1;
		}
		else
		{
			End = Mid;
		}
	}

	return pResult;
}

void
CHyperText::CalcHyperlinkXMap (CFont* pBaseFont)
{
	int x = 0;
	size_t Offset = 0;
	size_t Length = m_Text.GetLength ();
	size_t AttrCount = m_AttrArray.GetCount ();
	size_t HyperlinkCount = m_HyperlinkArray.GetCount ();

	const TTextAttrAnchor* pAttrAnchor = m_AttrArray;
	const TTextAttrAnchor* pAttrEnd = pAttrAnchor + AttrCount;
	THyperlinkXMapEntry* pHyperlinkXMapEntry;
	
	uint_t FontFlags = 0;
	CFont* pFont = pBaseFont->GetFontMod (FontFlags);

	m_HyperlinkXMap.SetCount (HyperlinkCount);
	pHyperlinkXMapEntry = m_HyperlinkXMap;

	rtl::CIteratorT <THyperlinkAnchor> HyperlinkAnchor = m_HyperlinkArray.GetHead ();
	for (; HyperlinkAnchor; HyperlinkAnchor++)
	{
		THyperlinkAnchor* pHyperlinkAnchor = *HyperlinkAnchor;
		TSize Size;

		for (; pAttrAnchor < pAttrEnd && pAttrAnchor->m_Offset < pHyperlinkAnchor->m_Offset; pAttrAnchor++)
		{
			if (pAttrAnchor->m_Attr.m_FontFlags == FontFlags)
				continue;

			Size = pFont->CalcTextSize (m_Text.cc () + Offset, pAttrAnchor->m_Offset - Offset);			
			
			x += Size.m_Width;
			Offset = pAttrAnchor->m_Offset;

			FontFlags = pAttrAnchor->m_Attr.m_FontFlags;
			pFont = pBaseFont->GetFontMod (FontFlags);
		}

		Size = pFont->CalcTextSize (m_Text.cc () + Offset, pHyperlinkAnchor->m_Offset - Offset);
		
		x += Size.m_Width;
		Offset = pHyperlinkAnchor->m_Offset;

		pHyperlinkXMapEntry->m_x = x;
		pHyperlinkXMapEntry->m_pAnchor = pHyperlinkAnchor;

		pHyperlinkAnchor++; 
		pHyperlinkXMapEntry++;
	}
}

TSize
CHyperText::CalcTextSize (CFont* pBaseFont) const
{
	TSize Size;

	int x = 0;
	size_t Offset = 0;
	size_t Length = m_Text.GetLength ();
	size_t AttrCount = m_AttrArray.GetCount ();

	uint_t FontFlags = 0;
	CFont* pFont = pBaseFont->GetFontMod (FontFlags);

	for (size_t i = 0; i < AttrCount; i++)
	{
		const TTextAttrAnchor* pAttrAnchor = &m_AttrArray [i];

		if (pAttrAnchor->m_Attr.m_FontFlags == FontFlags)
			continue;

		Size = pFont->CalcTextSize (m_Text.cc () + Offset, pAttrAnchor->m_Offset - Offset);
		
		x += Size.m_Width;
		Offset = pAttrAnchor->m_Offset;

		FontFlags = pAttrAnchor->m_Attr.m_FontFlags;
		pFont = pBaseFont->GetFontMod (FontFlags);
	}

	Size = pFont->CalcTextSize (m_Text.cc () + Offset, Length - Offset);
	Size.m_Width += x;

	return Size;
}

//.............................................................................

} // namespace gui
} // namespace axl
