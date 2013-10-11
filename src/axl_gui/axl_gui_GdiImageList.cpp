#include "pch.h"
#include "axl_gui_GdiImageList.h"
#include "axl_gui_GdiEngine.h"

namespace axl {
namespace gui {

//.............................................................................

CGdiImageList::CGdiImageList ()
{
	m_pEngine = CGdiEngine::GetSingleton ();
}

bool
CGdiImageList::InsertImage (
	CImage* pImage,
	size_t Index
	)
{
	return true;
}

bool
CGdiImageList::RemoveImage (size_t Index)
{
	return true;
}

//.............................................................................

} // namespace gui
} // namespace axl
