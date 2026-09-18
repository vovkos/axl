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

#define _AXL_CRY_X509STORE_H

#include "axl_cry_CryptoError.h"

namespace axl {

#ifdef _AXL_OS_WIN

namespace sys {
namespace win {

class CertStore;

} // namespace win
} // namespace sys

#endif // _AXL_OS_WIN

namespace cry {

//..............................................................................

class FreeX509Store {
public:
	void
	operator () (X509_STORE* store) {
		::X509_STORE_free(store);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class X509Store: public sl::Handle<X509_STORE*, FreeX509Store> {
	AXL_DISABLE_COPY(X509Store)

public:
	X509Store() {}

	X509Store(X509_STORE* store) {
		attach(store);
	}

	bool
	create();

	bool
	addCert(X509* cert);

#ifdef _AXL_OS_WIN
	size_t
	addWinCertStore(const sys::win::CertStore& store);
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
X509Store::create() {
	close();
	m_h = ::X509_STORE_new();
	return completeWithLastCryptoError(m_h != NULL);
}

inline
bool
X509Store::addCert(X509* cert) {
	ASSERT(m_h);
	int result = ::X509_STORE_add_cert(m_h, cert);
	return completeWithLastCryptoError(result);
}

//..............................................................................

} // namespace cry
} // namespace axl
