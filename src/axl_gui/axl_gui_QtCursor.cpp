#include "pch.h"
#include "axl_gui_QtCursor.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//.............................................................................

QtCursor::QtCursor ()
{
	m_engine = QtEngine::getSingleton ();
}

//.............................................................................

} // namespace gui
} // namespace axl
