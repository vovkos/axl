// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDIIMAGE_H

#include "axl_gui_GdiObjectHandle.h"
#include "axl_gui_Image.h"

namespace axl {
namespace gui {

//.............................................................................

class GdiImage: 
	public Image,
	public GdiObjectHandle <HBITMAP>
{
	friend class GdiEngine;

protected:
	Size m_size;

public:
	GdiImage ();

	virtual
	bool
	getData (
		void* data,
		int left,
		int top,
		int right,
		int bottom
		);
};

//.............................................................................

} // namespace gui
} // namespace axl

