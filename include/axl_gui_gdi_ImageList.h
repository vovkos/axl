// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDI_IMAGELIST_H

#include "axl_gui_ImageList.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {
namespace gdi {

//.............................................................................

class CDestroyImageList
{
public:
	void
	operator () (HIMAGELIST h)
	{
		ImageList_Destroy (h);
	}
};

//.............................................................................
	
class CImageList: 
	public IImageList,
	public rtl::CHandleT <HIMAGELIST, CDestroyImageList>
{
	friend class CEngine;

public:
	AXL_OBJ_CLASS_0 (CImageList, IImageList);

	CImageList ();

	virtual
	bool
	InsertImage (
		IImage* pImage,
		size_t Index
		);

	virtual
	bool
	RemoveImage (size_t Index);
};

//.............................................................................

} // namespace gdi
} // namespace gui
} // namespace axl

