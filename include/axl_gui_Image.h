// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_IMAGE_H

#include "axl_rtl_BitIdx.h"
#include "axl_gui_Def.h"

namespace axl {
namespace gui {

//.............................................................................

enum PixelFormatKind
{
	PixelFormatKind_Rgba = 0,
	PixelFormatKind_Rgb,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char* 
getPixelFormatString (PixelFormatKind pixelFormat);

//.............................................................................

struct ImageDesc
{
	Size m_size;
	PixelFormatKind m_pixelFormat;
	void* m_data;

	ImageDesc ()
	{
		memset (this, 0, sizeof (ImageDesc));
	}

	ImageDesc (
		int width,
		int height,
		PixelFormatKind pixelFormat,
		void* data
		)
	{
		m_size.m_width = width;
		m_size.m_height = height;
		m_pixelFormat = pixelFormat;
		m_data = data;
	}

	ImageDesc (
		const Size& size,
		PixelFormatKind pixelFormat,
		void* data
		)
	{
		m_size = size;
		m_pixelFormat = pixelFormat;
		m_data = data;
	}
};

//.............................................................................

class Image: public GuiItem
{
protected:
	Size m_size;

public:
	Size
	getSize ()
	{
		return m_size;
	}

	// data is returned in EPixelFormat_Rgba

	virtual
	bool
	getData (
		void* data, 
		int left,
		int top,
		int right,
		int bottom
		) = 0;

	bool
	getData (
		void* data,
		const Rect& rect
		)
	{
		return getData (
			data, 
			rect.m_left,
			rect.m_top,
			rect.m_right,
			rect.m_bottom			
			);
	}

	bool
	getData (
		void* data,
		const Size& size
		)
	{
		return getData (
			data, 
			0, 0, 
			size.m_width,
			size.m_height
			);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

