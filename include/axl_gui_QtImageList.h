// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTIMAGELIST_H

#include "axl_gui_ImageList.h"

namespace axl {
namespace gui {

//.............................................................................
	
class CQtImageList: public CImageList
{
	friend class CQtEngine;

public:
	QImage m_QtImage;
	
public:
	CQtImageList ();

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

