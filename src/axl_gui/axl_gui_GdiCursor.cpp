#include "pch.h"
#include "axl_gui_GdiCursor.h"
#include "axl_gui_GdiEngine.h"

namespace axl {
namespace gui {

//.............................................................................

CGdiCursor::CGdiCursor ()
{
	m_pEngine = CGdiEngine::GetSingleton ();
}

//.............................................................................

} // namespace gui
} // namespace axl
