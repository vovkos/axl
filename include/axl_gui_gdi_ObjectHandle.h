// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_GUI_GDI_OBJECTHANDLE_H

#include "axl_rtl_Handle.h"

namespace axl {
namespace gui {
namespace gdi {

//.............................................................................

class CDeleteObject
{
public:
	void
	operator () (HGDIOBJ h)
	{
		::DeleteObject (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CObjectHandleT: public rtl::CHandleT <T, CDeleteObject>
{
public:
	CObjectHandleT ()
	{
	}

	CObjectHandleT (T h):
		rtl::CHandleT <T, CDeleteObject> (h)
	{
	}
};

//.............................................................................

} // namespace gdi
} // namespace gui
} // namespace axl
