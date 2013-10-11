// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QT_IMAGELIST_H

#include "axl_gui_ImageList.h"

namespace axl {
namespace gui {
namespace qt {

//.............................................................................
	
class CImageList: public IImageList
{
	friend class CEngine;

public:
	QImage m_QtImage;
	
public:
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

} // namespace qt
} // namespace gui
} // namespace axl

