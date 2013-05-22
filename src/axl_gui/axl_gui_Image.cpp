#include "pch.h"
#include "axl_gui_Image.h"

namespace axl {
namespace gui {

//.............................................................................

const char* 
GetPixelFormatString (EPixelFormat PixelFormat)
{
	static const char* StringTable [] = 
	{
		"RGBA", // EPixelFormat_Rgba = 0,
		"RGB",  // EPixelFormat_Rgb,
	};

	return PixelFormat < countof (StringTable) ? 
		StringTable [PixelFormat] : 
		"undefined-pixel-format";
}

//.............................................................................

} // namespace gui
} // namespace axl
