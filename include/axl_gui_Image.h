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
GetPixelFormatString (EPixelFormat PixelFormat);

//.............................................................................

struct TImageDesc
{
	TSize m_Size;
	EPixelFormat m_PixelFormat;
	void* m_pData;

	TImageDesc ()
	{
		memset (this, 0, sizeof (TImageDesc));
	}

	TImageDesc (
		int Width,
		int Height,
		EPixelFormat PixelFormat,
		void* pData
		)
	{
		m_Size.m_Width = Width;
		m_Size.m_Height = Height;
		m_PixelFormat = PixelFormat;
		m_pData = pData;
	}

	TImageDesc (
		const TSize& Size,
		EPixelFormat PixelFormat,
		void* pData
		)
	{
		m_Size = Size;
		m_PixelFormat = PixelFormat;
		m_pData = pData;
	}
};

//.............................................................................

class CImage: public CGuiItem
{
protected:
	TSize m_Size;

public:
	TSize
	GetSize ()
	{
		return m_Size;
	}

	// data is returned in EPixelFormat_Rgba

	virtual
	bool
	GetData (
		void* pData, 
		int Left,
		int Top,
		int Right,
		int Bottom
		) = 0;

	bool
	GetData (
		void* pData,
		const TRect& Rect
		)
	{
		return GetData (
			pData, 
			Rect.m_Left,
			Rect.m_Top,
			Rect.m_Right,
			Rect.m_Bottom			
			);
	}

	bool
	GetData (
		void* pData,
		const TSize& Size
		)
	{
		return GetData (
			pData, 
			0, 0, 
			Size.m_Width,
			Size.m_Height
			);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

