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

#if (_AXL_OS_WIN)
typedef void* HCERTSTORE;
#elif (_AXL_OS_DARWIN)
typedef const struct __CFArray* CFArrayRef;
#endif

namespace axl {
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
public:
	X509Store() {}

	X509Store(X509_STORE* store) {
		attach(store);
	}

	bool
	create();

	size_t
	getObjectCount() const {
		ASSERT(m_h);
		return sk_X509_OBJECT_num(::X509_STORE_get0_objects(m_h));
	}

	bool
	addCert(X509* cert);

#if (_AXL_OS_WIN)
	size_t
	addWinCertStore(HCERTSTORE store);
#elif (_AXL_OS_DARWIN)
	size_t
	addDarwinCertArray(CFArrayRef certificateArray); // an array of SecCertificateRef
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
