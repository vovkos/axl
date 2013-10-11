// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDIIMAGELIST_H

#include "axl_gui_ImageList.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {

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
	
class CGdiImageList: 
	public CImageList,
	public rtl::CHandleT <HIMAGELIST, CDestroyImageList>
{
	friend class CGdiEngine;

public:
	CGdiImageList ();

	virtual
	bool
	InsertImage (
		CImage* pImage,
		size_t Index
		);

	virtual
	bool
	RemoveImage (size_t Index);
};

//.............................................................................

} // namespace gui
} // namespace axl

