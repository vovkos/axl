#include "pch.h"
#include "axl_gui_GdiCursor.h"
#include "axl_gui_GdiEngine.h"

namespace axl {
namespace gui {

//.............................................................................

GdiCursor::GdiCursor ()
{
	m_engine = GdiEngine::getSingleton ();
}

//.............................................................................

} // namespace gui
} // namespace axl
