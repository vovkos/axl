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
#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//..............................................................................

void
registerCryptoErrorProviders()
{
	err::getErrorMgr()->registerProvider(
		g_cryptoErrorGuid,
		sl::getSimpleSingleton<CryptoErrorProvider> ()
		);

	err::getErrorMgr()->registerProvider(
		g_x509ErrorGuid,
		sl::getSimpleSingleton<X509ErrorProvider> ()
		);

	ERR_load_crypto_strings();
}

//..............................................................................

sl::StringRef
CryptoErrorProvider::getErrorDescription(ulong_t code)
{
	const char* functionString = ::ERR_func_error_string(code);
	const char* reasonString = ::ERR_reason_error_string(code);

	sl::String errorString;
	errorString.format("OpenSSL error 0x%x", code);

	if (functionString)
		errorString.appendFormat(" in %s", functionString);

	if (reasonString)
		errorString.appendFormat(": %s", reasonString);

	return errorString;
}

//..............................................................................

} // namespace cry
} // namespace axl
