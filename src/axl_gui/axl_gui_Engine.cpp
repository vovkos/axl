#include "pch.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//.............................................................................

const char* 
GetEngineKindString (EEngine EngineKind)
{
	static const char* StringTable [] = 
	{
		"undefined-gui-engine", // EEngine_Undefined,
		"GDI",                  // EEngine_Gdi,
		"QT",                   // EEngine_Qt,
		"GTK+",                 // EEngine_Gtk,
	};

	return EngineKind < countof (StringTable) ? 
		StringTable [EngineKind] : 
		StringTable [EEngine_Undefined];
}

//.............................................................................

ÑCanvas*
CEngine::GetSharedOffscreenCanvas (
	int Width,
	int Height
	)
{
	EFormFactor FormFactor = GetFormFactor (Width, Height);
	ASSERT (FormFactor < countof (m_SharedOffscreenCanvasArray));
	
	TSharedOffscreenCanvas* pOffscreenCanvas = &m_SharedOffscreenCanvasArray [FormFactor];
	if (pOffscreenCanvas->m_Canvas &&
		pOffscreenCanvas->m_Size.m_Width >= Width && 
		pOffscreenCanvas->m_Size.m_Height >= Height)
		return pOffscreenCanvas->m_Canvas;

	if (pOffscreenCanvas->m_Size.m_Width < Width)
		pOffscreenCanvas->m_Size.m_Width = Width;

	if (pOffscreenCanvas->m_Size.m_Height < Height)
		pOffscreenCanvas->m_Size.m_Height = Height;

	pOffscreenCanvas->m_Canvas = CreateOffscreenCanvas (pOffscreenCanvas->m_Size);
	return pOffscreenCanvas->m_Canvas;
}

void 
CEngine::DeleteAllSharedOffscreenCanvases ()
{
	for (size_t i = 0; i < EFormFactor__Count; i++)
	{
		TSharedOffscreenCanvas* pOffscreenCanvas = &m_SharedOffscreenCanvasArray [i];
		pOffscreenCanvas->m_Canvas = ref::EPtr_Null;
		pOffscreenCanvas->m_Size.Setup (0, 0);
	}
}

//.............................................................................

} // namespace gui
} // namespace axl
