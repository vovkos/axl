//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_io_SslError.h"

namespace axl {
namespace io {

//..............................................................................

void
registerSslErrorProviders()
{
	::SSL_load_error_strings();

	err::getErrorMgr()->registerProvider(
		g_sslErrorGuid,
		sl::getSimpleSingleton<SslErrorProvider> ()
		);

	err::getErrorMgr()->registerProvider(
		g_sslX509ErrorGuid,
		sl::getSimpleSingleton<SslX509ErrorProvider> ()
		);
}

//..............................................................................

sl::StringRef
SslErrorProvider::getErrorDescription(ulong_t code)
{
	const char* functionString = ::ERR_func_error_string(code);
	const char* reasonString = ::ERR_reason_error_string(code);

	return sl::formatString("%s: %s", functionString, reasonString);
}

//..............................................................................

} // namespace io
} // namespace axl
