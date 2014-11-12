// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_IMAGE_H

#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//.............................................................................

class Image: public GuiItem
{
protected:
	Image (Engine* engine):
		GuiItem (engine)
	{
	}

public:
	bool
	getImageDesc (ImageDesc* imageDesc)
	{
		return m_engine->getImageDesc (this, imageDesc);
	}

	Size
	getSize ()
	{
		ImageDesc imageDesc;
		getImageDesc (&imageDesc);
		return imageDesc.m_size;
	}
};

//.............................................................................

class Cursor: public GuiItem
{
protected:
	Cursor (Engine* engine):
		GuiItem (engine)
	{
	}
};

//.............................................................................

} // namespace gui
} // namespace axl

