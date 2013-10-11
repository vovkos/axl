// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_QTCURSOR_H

#include "axl_gui_Cursor.h"

namespace axl {
namespace gui {

//.............................................................................

class CQtCursor: public CCursor
{
	friend class CQtEngine;
	
public:
	QCursor m_QtCursor;

public:	
	CQtCursor ();
};

//.............................................................................

} // namespace gui
} // namespace axl

