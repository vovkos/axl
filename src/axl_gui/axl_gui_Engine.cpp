#include "pch.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//.............................................................................

Engine::Engine ()
{
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_WidgetText]    = 0x000000;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_WidgetBack]    = 0xffffff;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_GrayText]      = 0x808080;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_SelectionText] = 0xffffff;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_SelectionBack] = 0x4060e0;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_3DFace]        = 0xc0c0c0;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_3DShadow]      = 0x808080;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_3DDarkShadow]  = 0x000000;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_3DLight]       = 0xe0e0e0;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_3DHiLight]     = 0xffffff;

	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_Black]         = StdColor_Black;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_White]         = StdColor_White;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_Gray]          = StdColor_Gray;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_Red]           = StdColor_Red;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_Green]         = StdColor_Green;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_Blue]          = StdColor_Blue;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_Purple]        = StdColor_Purple;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_Cyan]          = StdColor_Cyan;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_LightRed]      = StdColor_LightRed;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_LightGreen]    = StdColor_LightGreen;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_LightBlue]     = StdColor_LightBlue;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_LightPurple]   = StdColor_LightPurple;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_LightCyan]     = StdColor_LightCyan;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_PastelGray]    = StdColor_PastelGray;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_PastelRed]     = StdColor_PastelRed;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_PastelOrange]  = StdColor_PastelOrange;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_PastelYellow]  = StdColor_PastelYellow;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_PastelGreen]   = StdColor_PastelGreen;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_PastelCyan]    = StdColor_PastelCyan;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_PastelBlue]    = StdColor_PastelBlue;
	m_stdPalColorTable [~ColorFlag_Index & StdPalColor_PastelPurple]  = StdColor_PastelPurple;
};

//.............................................................................

} // namespace gui
} // namespace axl