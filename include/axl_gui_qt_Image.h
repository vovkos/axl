// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QT_PIXMAP_H

#include "axl_gui_Image.h"

namespace axl {
namespace gui {
namespace qt {

//.............................................................................

class CPixmap: public IImage
{
	friend class CEngine;

public:
	AXL_OBJ_CLASS_0 (CPixmap, IImage);

public:
	QPixmap m_QtPixmap;

public:
	CPixmap ();

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

} // namespace qt
} // namespace gui
} // namespace axl

