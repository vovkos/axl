// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTCURSOR_H

#include "axl_gui_Cursor.h"

namespace axl {
namespace gui {

//.............................................................................

class QtCursor: public Cursor
{
	friend class QtEngine;
	
public:
	QCursor m_qtCursor;

public:	
	QtCursor ();
};

//.............................................................................

} // namespace gui
} // namespace axl

