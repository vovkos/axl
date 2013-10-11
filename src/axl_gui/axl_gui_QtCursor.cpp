#include "pch.h"
#include "axl_gui_QtCursor.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//.............................................................................

CQtCursor::CQtCursor ()
{
	m_pEngine = CQtEngine::GetSingleton ();
}

//.............................................................................

} // namespace gui
} // namespace axl
