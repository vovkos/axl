#include "pch.h"
#include "axl_err_Errno.h"

namespace axl {
namespace err {

//..............................................................................

sl::String
ErrnoProvider::getErrorDescription (int code)
{
	char buffer [512] = { 0 };
#if (_AXL_OS_WIN)
	strerror_s (buffer, countof (buffer) - 1, code);
	return buffer;
#elif (_AXL_OS_POSIX)
#	if (_AXL_OS_LINUX)
	return strerror_r (code, buffer, countof (buffer) - 1);
#	else
	strerror_r (code, buffer, countof (buffer) - 1);
	return buffer;
#	endif
#endif
}

//..............................................................................

} // namespace err
} // namespace axl
