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

#pragma once

#define _AXL_CRY_PKEY_H

#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//..............................................................................

class FreePkey {
public:
	void
	operator () (EVP_PKEY* h) {
		EVP_PKEY_free(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Pkey: public sl::Handle<EVP_PKEY*, FreePkey> {
public:
	Pkey() {}

	Pkey(EVP_PKEY* h):
		sl::Handle<EVP_PKEY*, FreePkey>(h) {}

	bool
	readPrivateKeyPem(
		const sl::StringRef& pem,
		const sl::StringRef& passphrase = sl::StringRef()
	);

	bool
	readPrivateKeyPemFile(
		const sl::StringRef& fileName,
		const sl::StringRef& passphrase = sl::StringRef()
	);
};

//..............................................................................

} // namespace cry
} // namespace axl
