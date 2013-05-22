// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QT_CURSOR_H

#include "axl_gui_Cursor.h"

namespace axl {
namespace gui {
namespace qt {

//.............................................................................

class CCursor: public ICursor
{
	friend class CEngine;
	
public:
	AXL_OBJ_CLASS_0 (CCursor, ICursor);

public:
	QCursor m_QtCursor;

public:	
	CCursor ();
};

//.............................................................................

} // namespace qt
} // namespace gui
} // namespace axl

