#include "pch.h"
#include "axl_gui_qt_Pixmap.h"
#include "axl_gui_qt_Engine.h"
#include "axl_err_Error.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace gui {
namespace qt {

//.............................................................................

CPixmap::CPixmap ()
{
	m_pEngine = CEngine::GetSingleton ();
}

bool
CPixmap::GetData (
	void* pData,
	int Left,
	int Top,
	int Right,
	int Bottom
	)
{
	int Width = Right - Left;
	int Height = Bottom - Top;

	QImage Image = m_QtPixmap.toImage ().convertToFormat (QImage::Format_ARGB32);

	const uint_t* pSrc = (const uint_t*) Image.bits ();	
	pSrc += Width * Top;

	if (Width == m_Size.m_Width)
	{
		memcpy (pData, pSrc, Width * (Bottom - Top));
		return true;
	}
	
	uint_t* pDst = (uint_t*) pData;

	for (int i = 0; i < Height; i++, pDst += Width, pSrc += m_Size.m_Width)
		memcpy (pDst, pSrc, Width * sizeof (uint_t));

	return true;
}

//.............................................................................

} // namespace qt
} // namespace gui
} // namespace axl
