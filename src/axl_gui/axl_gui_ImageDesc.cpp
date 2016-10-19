#include "pch.h"
#include "axl_gui_ImageDesc.h"

namespace axl {
namespace gui {

//..............................................................................

const char*
getPixelFormatString (PixelFormat pixelFormat)
{
	static const char* stringTable [] =
	{
		"RGBA", // PixelFormat_Rgba = 0,
		"RGB",  // PixelFormat_Rgb,
	};

	return (size_t) pixelFormat < countof (stringTable) ?
		stringTable [pixelFormat] :
		"undefined-pixel-format";
}

//..............................................................................

} // namespace gui
} // namespace axl
