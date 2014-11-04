#include "pch.h"
#include "axl_err_Errno.h"

namespace axl {
namespace err {

//.............................................................................

rtl::String
ErrnoProvider::getErrorDescription (int code)
{
	char buffer [512] = { 0 };
#if (_AXL_ENV == AXL_ENV_WIN)
	strerror_s (buffer, countof (buffer) - 1, code);
	return buffer;
#else
	return strerror_r (code, buffer, countof (buffer) - 1);
#endif
}

//.............................................................................

} // namespace err
} // namespace axl
