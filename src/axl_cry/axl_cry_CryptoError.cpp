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
}

//..............................................................................

class LoadCryptoStrings
{
public:
	void
	operator () (int)
	{
		ERR_load_crypto_strings();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::StringRef
CryptoErrorProvider::getErrorDescription(ulong_t code)
{
	sl::callOnce(LoadCryptoStrings(), 0);

	const char* functionString = ::ERR_func_error_string(code);
	const char* reasonString = ::ERR_reason_error_string(code);

	return sl::formatString("%s: %s", functionString, reasonString);
}

//..............................................................................

} // namespace cry
} // namespace axl
