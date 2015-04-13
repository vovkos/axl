#include "pch.h"
#include "axl_mt_psx_DynamicLibrary.h"
#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

bool
DynamicLibrary::open (
	const char* name,
	int flags
	)
{
	close ();

	m_h = ::dlopen (name, flags);
	if (!m_h)
	{
		err::setStringError (dlerror ());
		return false;
	}

	return true;
}

void*
DynamicLibrary::getSymbol (const char* name)
{
	void* p = ::dlsym (m_h, name);
	if (!p)
	{
		err::setStringError (dlerror ());
		return NULL;
	}

	return p;
}

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
