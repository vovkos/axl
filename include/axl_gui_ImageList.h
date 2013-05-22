// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_IMAGELIST_H

#include "axl_gui_Image.h"

namespace axl {
namespace gui {

//.............................................................................

struct IImageList: IGuiItem
{
	// {51DBDE74-2992-4CD6-8BCA-893B9D9F2402}
	AXL_OBJ_INTERFACE (
		IImageList,
		0x51dbde74, 0x2992, 0x4cd6, 0x8b, 0xca, 0x89, 0x3b, 0x9d, 0x9f, 0x24, 0x02
		)

protected:
	size_t m_Count;
	TSize m_Size;

public:
	IImageList ()
	{
		m_Count = 0;
	}

	size_t
	GetImageCount ()
	{
		return m_Count;
	}

	TSize
	GetImageSize ()
	{
		return m_Size;
	}

	virtual
	bool
	InsertImage (
		IImage* pImage,
		size_t Index
		) = 0;

	virtual
	bool
	RemoveImage (size_t Index) = 0;

	bool
	AppendImage (IImage* pImage)
	{
		return InsertImage (pImage, m_Count);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

