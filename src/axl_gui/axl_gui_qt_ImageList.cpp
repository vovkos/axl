#include "pch.h"
#include "axl_gui_qt_ImageList.h"
#include "axl_gui_qt_Engine.h"

namespace axl {
namespace gui {
namespace qt {

//.............................................................................

CImageList::CImageList ()
{
	m_pEngine = CEngine::GetSingleton ();
}

bool
CImageList::InsertImage (
	IImage* pImage,
	size_t Index
	)
{
	return true;
}

bool
CImageList::RemoveImage (size_t Index)
{
	return true;
}

//.............................................................................

} // namespace qt
} // namespace gui
} // namespace axl
