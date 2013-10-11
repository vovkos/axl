#include "pch.h"
#include "axl_gui_QtImageList.h"
#include "axl_gui_QtEngine.h"

namespace axl {
namespace gui {

//.............................................................................

CQtImageList::CQtImageList ()
{
	m_pEngine = CQtEngine::GetSingleton ();
}

bool
CQtImageList::InsertImage (
	CImage* pImage,
	size_t Index
	)
{
	return true;
}

bool
CQtImageList::RemoveImage (size_t Index)
{
	return true;
}

//.............................................................................

} // namespace gui
} // namespace axl
