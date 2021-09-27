//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_GUI_GDIIMAGE_H

#include "axl_gui_GdiObjectHandle.h"
#include "axl_gui_Image.h"

namespace axl {
namespace gui {

//..............................................................................

class GdiImage:
	public Image,
	public GdiObjectHandle<HBITMAP> {
	friend class GdiEngine;

protected:
	Size m_size;

public:
	GdiImage();

	virtual
	bool
	getData(
		void* data,
		int left,
		int top,
		int right,
		int bottom
	);
};

//..............................................................................

} // namespace gui
} // namespace axl
