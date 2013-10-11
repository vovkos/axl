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
	m_pEngine = CGdiEngine::GetSingleton ();
}

bool
CGdiImage::GetData (
	void* pData,
	int Left,
	int Top,
	int Right,
	int Bottom
	)
{
	bool_t Result;

	int Width = Right - Left;
	int Height = Bottom - Top;

	BITMAPINFO BitmapInfo = { 0 };
	BitmapInfo.bmiHeader.biSize = sizeof (BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	BitmapInfo.bmiHeader.biWidth = m_Size.m_Width;
	BitmapInfo.bmiHeader.biHeight = m_Size.m_Height;

	CScreenDc ScreenDc;

	if (Width == m_Size.m_Width)
	{
		Result = GetDIBits (ScreenDc, m_h, Top, Height, pData, &BitmapInfo, DIB_RGB_COLORS);
		return err::Complete (Result);
	}
	
	char Buffer [1024];
	rtl::CArrayT <uint_t> ColorBuffer (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ColorBuffer.SetCount (m_Size.m_Width * Height);

	Result = GetDIBits (ScreenDc, m_h, Top, Height, ColorBuffer, &BitmapInfo, DIB_RGB_COLORS);
	if (!Result)
		return err::FailWithLastSystemError ();

	uint_t* pDst = (uint_t*) pData;
	const uint_t* pSrc = ColorBuffer;

	for (int i = 0; i < Height; i++, pDst += Width, pSrc += m_Size.m_Width)
		memcpy (pDst, pSrc, Width * sizeof (uint_t));

	return true;
}

//.............................................................................

} // namespace gui
} // namespace axl
