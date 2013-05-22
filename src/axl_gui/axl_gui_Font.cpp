#include "pch.h"
#include "axl_gui_Font.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//.............................................................................

const char* 
GetFontFlagString (EFontFlag Flag)
{
	static const char* StringTable [] = 
	{
		"bold",      // EFontFlag_Bold      = 0x01,
		"italic",    // EFontFlag_Italic    = 0x02,
		"underline", // EFontFlag_Underline = 0x04,
		"strikeout", // EFontFlag_Strikeout = 0x08,
	};

	size_t i = rtl::GetLoBitIdx32 (Flag >> 8);

	return i < countof (StringTable) ? 
		StringTable [i] : 
		"undefined-font-flag";
}

uint_t
ParseFontFlagString (
	const char* pString,
	const char** ppEnd
	)
{
	if (!pString)
	{
		if (ppEnd)
			*ppEnd = NULL;

		return EFontFlag_Transparent;
	}

	uint_t Flags = 0;

	const char* p = pString;

	for (;;)
	{
		int Mod = 0;

		switch (*p)
		{
		case 'b': 
		case 'B':
			Mod = EFontFlag_Bold;
			break;

		case 'i': 
		case 'I':
			Mod = EFontFlag_Italic;
			break;

		case 'u': 
		case 'U':
			Mod = EFontFlag_Underline;
			break;

		case 's': 
		case 'S':
			Mod = EFontFlag_Strikeout;
			break;
		}

		if (!Mod)
			break;

		Flags |= Mod;
		p++;
	}

	if (ppEnd)
		*ppEnd = p;

	return Flags;
}

//.............................................................................

void
TFontDesc::Setup (
	const char* pFaceName, 
	size_t PointSize,
	uint_t Flags
	)
{
	size_t Length = strlen (pFaceName);
	if (Length >= countof (m_FaceName))
		Length = countof (m_FaceName) - 1;
	
	memcpy (m_FaceName, pFaceName, Length * sizeof (char));
	m_FaceName [Length] = 0;
	m_PointSize = PointSize;
	m_Flags = Flags;
}

//.............................................................................

IFont*
IFont::GetFontMod (uint_t Flags)
{
	if (Flags & EFontFlag_Transparent)
		return this;

	Flags &= (countof (m_pTuple->m_FontModArray) - 1);
	
	return m_pTuple->m_FontModArray [Flags] ?
		m_pTuple->m_FontModArray [Flags] :
		m_pEngine->GetFontMod (this, Flags);
}

//.............................................................................

void
TTextAttr::Parse (
	const char* pString,
	const char** ppEnd
	)
{
	TColorAttr::Parse (pString, &pString);

	while (isspace (*pString))
		pString++;

	if (*pString == '+')
		m_FontFlags = ParseFontFlagString (pString + 1, &pString);

	if (ppEnd)
		*ppEnd = pString;
}

void
TTextAttr::ParseOverlay (
	const TTextAttr& BaseAttr,
	const char* pString,
	const char** ppEnd
	)
{
	TTextAttr OverlayAttr;
	OverlayAttr.Parse (pString, ppEnd);
	Overlay (BaseAttr, OverlayAttr);
}

//.............................................................................

} // namespace gui
} // namespace axl
