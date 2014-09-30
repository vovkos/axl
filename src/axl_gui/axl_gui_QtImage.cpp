#include "pch.h"
#include "axl_gui_QtImage.h"
#include "axl_gui_QtEngine.h"
#include "axl_err_Error.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace gui {

//.............................................................................

CQtImage::CQtImage ()
{
	m_engine = CQtEngine::getSingleton ();
}

bool
CQtImage::getData (
	void* data,
	int left,
	int top,
	int right,
	int bottom
	)
{
	int width = right - left;
	int height = bottom - top;

	QImage image = m_qtPixmap.toImage ().convertToFormat (QImage::Format_ARGB32);

	const uint_t* src = (const uint_t*) image.bits ();	
	src += width * top;

	if (width == m_size.m_width)
	{
		memcpy (data, src, width * (bottom - top));
		return true;
	}
	
	uint_t* dst = (uint_t*) data;

	for (int i = 0; i < height; i++, dst += width, src += m_size.m_width)
		memcpy (dst, src, width * sizeof (uint_t));

	return true;
}

//.............................................................................

} // namespace gui
} // namespace axl
