#include "pch.h"
#include "axl_gui_GdiImage.h"
#include "axl_gui_GdiEngine.h"
#include "axl_err_Error.h"
#include "axl_rtl_Array.h"

namespace axl {
namespace gui {

//.............................................................................

CGdiImage::CGdiImage ()
{
	m_engine = CGdiEngine::getSingleton ();
}

bool
CGdiImage::getData (
	void* data,
	int left,
	int top,
	int right,
	int bottom
	)
{
	bool_t result;

	int width = right - left;
	int height = bottom - top;

	BITMAPINFO bitmapInfo = { 0 };
	bitmapInfo.bmiHeader.biSize = sizeof (bitmapInfo.bmiHeader);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biWidth = m_size.m_width;
	bitmapInfo.bmiHeader.biHeight = m_size.m_height;

	CScreenDc screenDc;

	if (width == m_size.m_width)
	{
		result = ::GetDIBits (screenDc, m_h, top, height, data, &bitmapInfo, DIB_RGB_COLORS);
		return err::complete (result);
	}
	
	char buffer [1024];
	rtl::CArrayT <uint_t> colorBuffer (ref::EBuf_Stack, buffer, sizeof (buffer));
	colorBuffer.setCount (m_size.m_width * height);

	result = ::GetDIBits (screenDc, m_h, top, height, colorBuffer, &bitmapInfo, DIB_RGB_COLORS);
	if (!result)
		return err::failWithLastSystemError ();

	uint_t* dst = (uint_t*) data;
	const uint_t* src = colorBuffer;

	for (int i = 0; i < height; i++, dst += width, src += m_size.m_width)
		memcpy (dst, src, width * sizeof (uint_t));

	return true;
}

//.............................................................................

} // namespace gui
} // namespace axl
