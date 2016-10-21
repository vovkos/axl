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

#include "pch.h"
#include "axl_gui_GdiCursor.h"
#include "axl_gui_GdiEngine.h"

namespace axl {
namespace gui {

//..............................................................................

GdiCursor::GdiCursor ()
{
	m_engine = GdiEngine::getSingleton ();
}

//..............................................................................

} // namespace gui
} // namespace axl
