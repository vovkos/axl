// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDIIMAGE_H

#include "axl_gui_Image.h"
#include "axl_gui_GdiObjectHandle.h"

namespace axl {
namespace gui {

//.............................................................................

class CGdiImage: 
	public CImage,
	public CGdiObjectHandleT <HBITMAP>
{
	friend class CGdiEngine;

protected:
	TSize m_size;

public:
	CGdiImage ();

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

