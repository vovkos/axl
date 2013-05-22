#include "pch.h"
#include "axl_gui_gdi_ImageList.h"
#include "axl_gui_gdi_Engine.h"

namespace axl {
namespace gui {
namespace gdi {

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

} // namespace gdi
} // namespace gui
} // namespace axl
