// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTIMAGE_H

#include "axl_gui_Image.h"

namespace axl {
namespace gui {

//.............................................................................

class CQtImage: public CImage
{
	friend class CQtEngine;

public:
	QPixmap m_QtPixmap;
	
public:
	CQtImage ();

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

} // namespace gui
} // namespace axl

