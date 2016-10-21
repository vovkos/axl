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

class LoadCryptoStrings
{
public:
	void
	operator () (int)
	{
		ERR_load_crypto_strings ();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::StringRef
CryptoErrorProvider::getErrorDescription (const err::ErrorRef& error)
{
	sl::callOnce (LoadCryptoStrings (), 0);
	return ERR_reason_error_string (error->m_code);
}

//..............................................................................

} // namespace cry
} // namespace axl
