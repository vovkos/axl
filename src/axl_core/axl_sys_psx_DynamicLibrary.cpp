#include "pch.h"
#include "axl_sys_psx_DynamicLibrary.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
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
} // namespace sys
} // namespace axl
