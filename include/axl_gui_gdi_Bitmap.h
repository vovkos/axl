// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDI_BITMAP_H

#include "axl_gui_Image.h"
#include "axl_gui_gdi_ObjectHandle.h"

namespace axl {
namespace gui {
namespace gdi {

//.............................................................................

class CBitmap: 
	public IImage,
	public CObjectHandleT <HBITMAP>
{
	friend class CEngine;

protected:
	TSize m_Size;

public:
	CBitmap ();

	virtual
	bool
	GetData (
		void* pData,
		int Left,
		int Top,
		int Right,
		int Bottom
		);
};

//.............................................................................

} // namespace gdi
} // namespace gui
} // namespace axl

