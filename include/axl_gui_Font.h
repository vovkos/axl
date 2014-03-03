// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_FONT_H

#include "axl_gui_Def.h"
#include "axl_gui_Color.h"
#include "axl_rtl_BitIdx.h"
#include "axl_ref_RefCount.h"
#include "axl_mem_Factory.h"

namespace axl {
namespace gui {

class CFont;

//.............................................................................

enum EStdFont
{
	EStdFont_Gui = 0,
	EStdFont_Monospace,
	
	EStdFont__Count
};

//.............................................................................

enum EFontFlag
{
	EFontFlag_Bold        = 0x01,
	EFontFlag_Italic      = 0x02,
	EFontFlag_Underline   = 0x04,
	EFontFlag_Strikeout   = 0x08,
	EFontFlag_Transparent = 0x10,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EFontFlag
GetFirstFontFlag (uint_t Flags)
{
	return (EFontFlag) (1 << rtl::GetLoBitIdx (Flags));
}

const char* 
GetFontFlagString (EFontFlag Flag);

inline
const char* 
GetFontFlagString (uint_t Flags)
{
	return GetFontFlagString (GetFirstFontFlag (Flags));
}

inline
int
OverlayFontFlags (
	uint_t BaseFontFlags,
	uint_t OverlayFontFlags
	)
{
	return (OverlayFontFlags & EFontFlag_Transparent) ? BaseFontFlags : OverlayFontFlags;
}

uint_t
ParseFontFlagString (
	const char* pString,
	const char** ppEnd = NULL
	);

//.............................................................................

struct TFontDesc
{
	char m_FaceName [32];
	size_t m_PointSize;
	uint_t m_Flags;

	TFontDesc ()
	{
		memset (this, 0, sizeof (TFontDesc));
	}

	TFontDesc (
		const char* pFaceName, 
		size_t PointSize = 0,
		uint_t Flags = 0
		)
	{
		Setup (pFaceName, PointSize, Flags);
	}

	void
	Setup (
		const char* pFaceName, 
		size_t PointSize = 0,
		uint_t Flags = 0
		);
};

//.............................................................................

struct TFontTuple
{
	TFontDesc m_FontDesc;
	
	CFont* m_pBaseFont;
	CFont* m_FontModArray [0x10];

	TFontTuple ()
	{
		m_pBaseFont = NULL;
		memset (m_FontModArray, 0, sizeof (m_FontModArray));
	}
};

//.............................................................................

template <typename T>
class CFontTupleT: 
	public ref::CRefCount,
	public TFontTuple
{
public:
	~CFontTupleT ()
	{
		for (size_t i = 0; i < countof (m_FontModArray); i++)
		{
			T* pFont = static_cast <T*> (m_FontModArray [i]);
			if (pFont)
				AXL_MEM_DELETE (pFont);
		}
	}
};

//.............................................................................

class CFont: public CGuiItem
{
protected:
	TFontTuple* m_pTuple;
	TFontDesc m_FontDesc;

public:
	CFont ()
	{
		m_pTuple = NULL;
	}

	const TFontDesc*
	GetFontDesc ()
	{
		return &m_FontDesc;
	}

	CFont*
	GetFontMod (uint_t Flags);

	virtual
	bool
	IsMonospace () = 0;

	virtual
	TSize
	CalcTextSize (
		const char* pText,
		size_t Length = -1
		) = 0;

	TSize
	CalcTextSize (char Char)
	{
		return CalcTextSize (&Char, 1);
	}
};

//.............................................................................

struct TTextAttr: public TColorAttr
{
	uint_t m_FontFlags;

	TTextAttr ()
	{
		m_FontFlags = EFontFlag_Transparent;
	}

	TTextAttr (
		uint_t ForeColor,
		uint_t BackColor = EColorFlag_Transparent,
		uint_t FontFlags = EFontFlag_Transparent
		)
	{
		Setup (ForeColor, BackColor, FontFlags);
	}

	TTextAttr (
		const TColorAttr& ColorAttr,
		uint_t FontFlags = EFontFlag_Transparent
		)
	{
		Setup (ColorAttr, FontFlags);
	}

	TTextAttr (const char* pString)
	{
		Parse (pString);
	}

	int 
	Cmp (const TTextAttr& Attr)
	{
		return memcmp (this, &Attr, sizeof (TTextAttr));
	}

	void
	Clear ()
	{
		TColorAttr::Clear ();
		m_FontFlags = EFontFlag_Transparent;
	}

	void
	Setup (
		uint_t ForeColor,
		uint_t BackColor,
		uint_t FontFlags
		)
	{
		m_ForeColor = ForeColor;
		m_BackColor = BackColor;
		m_FontFlags = FontFlags;
	}

	void
	Setup (
		const TColorAttr& ColorAttr,
		uint_t FontFlags
		)
	{

		*(TColorAttr*) this = ColorAttr;
		m_FontFlags = FontFlags;
	}

	void
	Overlay (
		const TTextAttr& BaseAttr,
		const TTextAttr& OverlayAttr
		)
	{
		TColorAttr::Overlay (BaseAttr, OverlayAttr);	
		m_FontFlags = OverlayFontFlags (BaseAttr.m_FontFlags, OverlayAttr.m_FontFlags);
	}

	void
	Overlay (const TTextAttr& OverlayAttr)
	{
		Overlay (*this, OverlayAttr);
	}

	void
	Parse (
		const char* String,
		const char** ppEnd = NULL
		);

	void
	ParseOverlay (
		const TTextAttr& BaseAttr,
		const char* pString,
		const char** ppEnd = NULL
		);

	void
	ParseOverlay (
		const char* pString,
		const char** ppEnd = NULL
		)
	{
		ParseOverlay (*this, pString, ppEnd);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

