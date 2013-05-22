#include "pch.h"
#include "axl_gui_gdi_Cursor.h"
#include "axl_gui_gdi_Engine.h"

namespace axl {
namespace gui {
namespace gdi {

//.............................................................................

CCursor::CCursor ()
{
	m_pEngine = CEngine::GetSingleton ();
}

//.............................................................................

} // namespace gdi
} // namespace gui
} // namespace axl
