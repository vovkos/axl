#include "pch.h"
#include "axl_gui_Color.h"
#include "axl_rtl_ByteOrder.h"

namespace axl {
namespace gui {

//.............................................................................

static uint_t g_stdPalColorArray [EStdPalColor__Count] =
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
getStdPalColorArray ()
{
	return g_stdPalColorArray;
}

void
updateStdPalSystemColors ()
{
#if (_AXL_ENV == AXL_ENV_WIN)
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_WidgetText]    = inverseRgb (::GetSysColor (COLOR_WINDOWTEXT));
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_WidgetBack]    = inverseRgb (::GetSysColor (COLOR_WINDOW));
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_GrayText]      = inverseRgb (::GetSysColor (COLOR_GRAYTEXT));
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_SelectionText] = inverseRgb (::GetSysColor (COLOR_HIGHLIGHTTEXT));
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_SelectionBack] = inverseRgb (::GetSysColor (COLOR_HIGHLIGHT));
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_3DFace]        = inverseRgb (::GetSysColor (COLOR_3DFACE));
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_3DShadow]      = inverseRgb (::GetSysColor (COLOR_3DSHADOW));
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_3DDarkShadow]  = inverseRgb (::GetSysColor (COLOR_3DDKSHADOW));
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_3DLight]       = inverseRgb (::GetSysColor (COLOR_3DLIGHT));
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_3DHiLight]     = inverseRgb (::GetSysColor (COLOR_3DHILIGHT));
#else
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_WidgetText]    = 0x000000;
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_WidgetBack]    = 0xffffff;
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_GrayText]      = 0x808080;
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_SelectionText] = 0xffffff;
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_SelectionBack] = 0x000080;
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_3DFace]        = 0xc0c0c0;
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_3DShadow]      = 0x808080;
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_3DDarkShadow]  = 0x000000;
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_3DLight]       = 0xe0e0e0;
	g_stdPalColorArray [~EColorFlag_Index & EStdPalColor_3DHiLight]     = 0xffffff;
#endif
}

static
class CInitStdPalSystemColors 
{
public:
	CInitStdPalSystemColors ()
	{
		updateStdPalSystemColors ();
	}
} g_initStdColors;

//.............................................................................

uint_t
parseColorString (
	const char* string,
	const char** end_o
	)
{
	if (!string)
	{
		if (end_o)
			*end_o = NULL;

		return EColorFlag_Transparent;
	}

	uint_t color = EColorFlag_Transparent;

	char* end;

	if (string[0] == '#')
	{
		uint_t rgb = strtol (string + 1, &end, 16);
		if (end != string)
			color = rgb;
	}
	else
	{
		uint_t index = strtol (string, &end, 10);
		if (end != string)
			color = EColorFlag_Index | index;
	}

	if (end_o)
		*end_o = end;

	return color;
}

//.............................................................................

void
TColorAttr::parse (
	const char* string,
	const char** end
	)
{
	m_foreColor = parseColorString (string, &string);
	m_backColor = EColorFlag_Transparent;

	while (isspace (*string))
		string++;

	if (*string == '|')
		m_backColor = parseColorString (string + 1, &string);

	if (end)
		*end = string;
}

void
TColorAttr::parseOverlay (
	const TColorAttr& baseAttr,
	const char* string,
	const char** end
	)
{
	TColorAttr overlayAttr;
	overlayAttr.parse (string, end);
	overlay (baseAttr, overlayAttr);
}

//.............................................................................

} // namespace gui
} // namespace axl