// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_IMAGEDESC_H

#include "axl_gui_Def.h"

namespace axl {
namespace gui {

//.............................................................................

enum PixelFormat
{
	PixelFormat_Rgba = 0,
	PixelFormat_Rgb,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getPixelFormatString (PixelFormat pixelFormat);

//.............................................................................

struct ImageDesc
{
	Size m_size;
	PixelFormat m_pixelFormat;

	ImageDesc ()
	{
		memset (this, 0, sizeof (ImageDesc));
	}

	ImageDesc (
		uint_t width,
		uint_t height,
		PixelFormat pixelFormat = PixelFormat_Rgba
		)
	{
		m_size.setup (width, height);
		m_pixelFormat = pixelFormat;
	}

	ImageDesc (
		const Size& size,
		PixelFormat pixelFormat = PixelFormat_Rgba
		)
	{
		m_size = size;
		m_pixelFormat = pixelFormat;
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

