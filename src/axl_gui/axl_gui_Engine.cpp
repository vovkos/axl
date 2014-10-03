#include "pch.h"
#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//.............................................................................

const char* 
getEngineKindString (EngineKind engineKind)
{
	static const char* stringTable [] = 
	{
		"undefined-gui-engine", // EEngine_Undefined,
		"GDI",                  // EEngine_Gdi,
		"QT",                   // EEngine_Qt,
		"GTK+",                 // EEngine_Gtk,
	};

	return engineKind < countof (stringTable) ? 
		stringTable [engineKind] : 
		stringTable [EngineKind_Undefined];
}

//.............................................................................

Font*
Engine::getStdFont (StdFontKind fontKind)
{
	if (fontKind < 0 || fontKind >= StdFontKind__Count)
		return NULL;

	if (!m_stdFontArray [fontKind])
		m_stdFontArray [fontKind] = createStdFont (fontKind);	
		
	return m_stdFontArray [fontKind];
}

Cursor*
Engine::getStdCursor (StdCursorKind cursorKind)
{
	if (cursorKind < 0 || cursorKind >= StdCursorKind__Count)
		return NULL;

	if (!m_stdCursorArray [cursorKind])
		m_stdCursorArray [cursorKind] = createStdCursor (cursorKind);	
		
	return m_stdCursorArray [cursorKind];
}

Canvas*
Engine::getSharedOffscreenCanvas (
	int width,
	int height
	)
{
	FormFactor formFactor = getFormFactor (width, height);
	ASSERT (formFactor < countof (m_sharedOffscreenCanvasArray));
	
	SharedOffscreenCanvas* offscreenCanvas = &m_sharedOffscreenCanvasArray [formFactor];
	if (offscreenCanvas->m_canvas &&
		offscreenCanvas->m_size.m_width >= width && 
		offscreenCanvas->m_size.m_height >= height)
		return offscreenCanvas->m_canvas;

	if (offscreenCanvas->m_size.m_width < width)
		offscreenCanvas->m_size.m_width = width;

	if (offscreenCanvas->m_size.m_height < height)
		offscreenCanvas->m_size.m_height = height;

	offscreenCanvas->m_canvas = createOffscreenCanvas (offscreenCanvas->m_size);
	return offscreenCanvas->m_canvas;
}

void 
Engine::deleteAllSharedOffscreenCanvases ()
{
	for (size_t i = 0; i < FormFactor__Count; i++)
	{
		SharedOffscreenCanvas* offscreenCanvas = &m_sharedOffscreenCanvasArray [i];
		offscreenCanvas->m_canvas = ref::PtrKind_Null;
		offscreenCanvas->m_size.setup (0, 0);
	}
}

//.............................................................................

} // namespace gui
} // namespace axl
