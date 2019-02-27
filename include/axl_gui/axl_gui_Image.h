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

#define _AXL_GUI_IMAGE_H

#include "axl_gui_Engine.h"

namespace axl {
namespace gui {

//..............................................................................

class Image: public GuiItem
{
protected:
	Image(Engine* engine):
		GuiItem(engine)
	{
	}

public:
	bool
	getImageDesc(ImageDesc* imageDesc)
	{
		return m_engine->getImageDesc(this, imageDesc);
	}

	Size
	getSize()
	{
		ImageDesc imageDesc;
		getImageDesc(&imageDesc);
		return imageDesc.m_size;
	}
};

//..............................................................................

class Cursor: public GuiItem
{
protected:
	Cursor(Engine* engine):
		GuiItem(engine)
	{
	}
};

//..............................................................................

} // namespace gui
} // namespace axl
