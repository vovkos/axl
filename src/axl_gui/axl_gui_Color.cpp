#include "pch.h"
#include "axl_gui_Color.h"
#include "axl_rtl_ByteOrder.h"

namespace axl {
namespace gui {

//.............................................................................

static uint_t g_StdPalColorArray [EStdPalColor__Count] =
{
	0, // EStdPalColor_Widget,
	0, // EStdPalColor_Text,
	0, // EStdPalColor_GrayText,
	0, // EStdPalColor_SelectionText,
	0, // EStdPalColor_SelectionBack,
	0, // EStdPalColor_3DFace,
	0, // EStdPalColor_3DShadow,
	0, // EStdPalColor_3DDarkShadow,
	0, // EStdPalColor_3DLight,
	0, // EStdPalColor_3DHiLight,

	EStdColor_Black,
	EStdColor_White,
	EStdColor_Gray,
	EStdColor_Red,
	EStdColor_Green,
	EStdColor_Blue,
	EStdColor_Purple,
	EStdColor_Cyan,
	EStdColor_LightRed,
	EStdColor_LightGreen,
	EStdColor_LightBlue,
	EStdColor_LightPurple,
	EStdColor_LightCyan,
	EStdColor_PastelGray,
	EStdColor_PastelRed,
	EStdColor_PastelOrange,
	EStdColor_PastelYellow,
	EStdColor_PastelGreen,
	EStdColor_PastelCyan,
	EStdColor_PastelBlue,
	EStdColor_PastelPurple,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const uint_t*
GetStdPalColorArray ()
{
	return g_StdPalColorArray;
}

void
UpdateStdPalSystemColors ()
{
#if (_AXL_ENV == AXL_ENV_WIN)
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_WidgetText]    = InverseRgb (GetSysColor (COLOR_WINDOWTEXT));
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_WidgetBack]    = InverseRgb (GetSysColor (COLOR_WINDOW));
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_GrayText]      = InverseRgb (GetSysColor (COLOR_GRAYTEXT));
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_SelectionText] = InverseRgb (GetSysColor (COLOR_HIGHLIGHTTEXT));
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_SelectionBack] = InverseRgb (GetSysColor (COLOR_HIGHLIGHT));
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_3DFace]        = InverseRgb (GetSysColor (COLOR_3DFACE));
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_3DShadow]      = InverseRgb (GetSysColor (COLOR_3DSHADOW));
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_3DDarkShadow]  = InverseRgb (GetSysColor (COLOR_3DDKSHADOW));
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_3DLight]       = InverseRgb (GetSysColor (COLOR_3DLIGHT));
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_3DHiLight]     = InverseRgb (GetSysColor (COLOR_3DHILIGHT));
#else
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_WidgetText]    = 0x000000;
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_WidgetBack]    = 0xffffff;
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_GrayText]      = 0x808080;
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_SelectionText] = 0xffffff;
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_SelectionBack] = 0x000080;
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_3DFace]        = 0xc0c0c0;
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_3DShadow]      = 0x808080;
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_3DDarkShadow]  = 0x000000;
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_3DLight]       = 0xe0e0e0;
	g_StdPalColorArray [~EColorFlag_Index & EStdPalColor_3DHiLight]     = 0xffffff;
#endif
}

static
class CInitStdPalSystemColors 
{
public:
	CInitStdPalSystemColors ()
	{
		UpdateStdPalSystemColors ();
	}
} g_InitStdColors;

//.............................................................................

uint_t
ParseColorString (
	const char* pString,
	const char** ppEnd
	)
{
	if (!pString)
	{
		if (ppEnd)
			*ppEnd = NULL;

		return EColorFlag_Transparent;
	}

	uint_t Color = EColorFlag_Transparent;

	char* pEnd;

	if (pString[0] == '#')
	{
		uint_t Rgb = strtol (pString + 1, &pEnd, 16);
		if (pEnd != pString)
			Color = Rgb;
	}
	else
	{
		uint_t Index = strtol (pString, &pEnd, 10);
		if (pEnd != pString)
			Color = EColorFlag_Index | Index;
	}

	if (ppEnd)
		*ppEnd = pEnd;

	return Color;
}

//.............................................................................

void
TColorAttr::Parse (
	const char* pString,
	const char** ppEnd
	)
{
	m_ForeColor = ParseColorString (pString, &pString);
	m_BackColor = EColorFlag_Transparent;

	while (isspace (*pString))
		pString++;

	if (*pString == '|')
		m_BackColor = ParseColorString (pString + 1, &pString);

	if (ppEnd)
		*ppEnd = pString;
}

void
TColorAttr::ParseOverlay (
	const TColorAttr& BaseAttr,
	const char* pString,
	const char** ppEnd
	)
{
	TColorAttr OverlayAttr;
	OverlayAttr.Parse (pString, ppEnd);
	Overlay (BaseAttr, OverlayAttr);
}

//.............................................................................

} // namespace gui
} // namespace axl