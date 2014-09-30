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

enum EPixelFormat
{
	EPixelFormat_Rgba = 0,
	EPixelFormat_Rgb,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char* 
getPixelFormatString (EPixelFormat pixelFormat);

//.............................................................................

struct TImageDesc
{
	TSize m_size;
	EPixelFormat m_pixelFormat;
	void* m_data;

	TImageDesc ()
	{
		memset (this, 0, sizeof (TImageDesc));
	}

	TImageDesc (
		int width,
		int height,
		EPixelFormat pixelFormat,
		void* data
		)
	{
		m_size.m_width = width;
		m_size.m_height = height;
		m_pixelFormat = pixelFormat;
		m_data = data;
	}

	TImageDesc (
		const TSize& size,
		EPixelFormat pixelFormat,
		void* data
		)
	{
		m_size = size;
		m_pixelFormat = pixelFormat;
		m_data = data;
	}
};

//.............................................................................

class CImage: public CGuiItem
{
protected:
	TSize m_size;

public:
	TSize
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
		const TRect& rect
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
		const TSize& size
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

