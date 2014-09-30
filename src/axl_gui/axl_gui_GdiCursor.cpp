#include "pch.h"
#include "axl_gui_GdiCursor.h"
#include "axl_gui_GdiEngine.h"

namespace axl {
namespace gui {

//.............................................................................

CGdiCursor::CGdiCursor ()
{
	m_engine = CGdiEngine::getSingleton ();
}

//.............................................................................

} // namespace gui
} // namespace axl
