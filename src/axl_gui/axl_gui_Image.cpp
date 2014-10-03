#include "pch.h"
#include "axl_gui_Image.h"

namespace axl {
namespace gui {

//.............................................................................

const char* 
getPixelFormatString (PixelFormat pixelFormat)
{
	static const char* stringTable [] = 
	{
		"RGBA", // EPixelFormat_Rgba = 0,
		"RGB",  // EPixelFormat_Rgb,
	};

	return pixelFormat < countof (stringTable) ? 
		stringTable [pixelFormat] : 
		"undefined-pixel-format";
}

//.............................................................................

} // namespace gui
} // namespace axl
