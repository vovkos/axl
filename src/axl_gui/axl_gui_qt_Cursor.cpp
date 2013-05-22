#include "pch.h"
#include "axl_gui_qt_Cursor.h"
#include "axl_gui_qt_Engine.h"

namespace axl {
namespace gui {
namespace qt {

//.............................................................................

CCursor::CCursor ()
{
	m_pEngine = CEngine::GetSingleton ();
}

//.............................................................................

} // namespace qt
} // namespace gui
} // namespace axl
