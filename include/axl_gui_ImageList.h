// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_IMAGELIST_H

#include "axl_gui_Image.h"

namespace axl {
namespace gui {

//.............................................................................

class CImageList: public CGuiItem
{
protected:
	size_t m_Count;
	TSize m_Size;

public:
	CImageList ()
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
		CImage* pImage,
		size_t Index
		) = 0;

	virtual
	bool
	RemoveImage (size_t Index) = 0;

	bool
	AppendImage (CImage* pImage)
	{
		return InsertImage (pImage, m_Count);
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

