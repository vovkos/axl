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

#define _AXL_CRY_X509CERT_H

#include "axl_cry_CryptoError.h"
#include "axl_sys_Time.h"

namespace axl {

namespace cry {

//..............................................................................

// X.509 numbers its versions from 0, so "v3" is 2 -- decode this wart here
// rather than at every call site

enum X509Version {
	X509Version_1 = 0,
	X509Version_2 = 1,
	X509Version_3 = 2,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class FreeX509 {
public:
	void
	operator () (X509* cert) {
		::X509_free(cert);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class X509Cert: public sl::Handle<X509*, FreeX509> {
	AXL_DISABLE_COPY(X509Cert)

public:
	X509Cert() {}

	X509Cert(X509* cert) {
		attach(cert);
	}

	bool
	create();

	bool
	setVersion(X509Version version);

	ASN1_INTEGER*
	getSerialNumber() const;

	bool
	setSerialNumber(long serial);

	ASN1_TIME*
	getNotBefore() const;

	ASN1_TIME*
	getNotAfter() const;

	bool
	setNotBefore(const ASN1_TIME* time);

	bool
	setNotBefore(uint64_t timestamp);

	bool
	setNotAfter(const ASN1_TIME* time);

	bool
	setNotAfter(uint64_t timestamp);

	bool
	setSubjectName(X509_NAME* name);

	bool
	setIssuerName(X509_NAME* name);

	bool
	setPublicKey(EVP_PKEY* key);

	bool
	sign(
		EVP_PKEY* key,
		const EVP_MD* digest = ::EVP_sha256() // pass NULL for Ed25519/Ed448
	);

	bool
	loadDer(
		const void* p,
		size_t size
	);

	size_t
	saveDer(sl::Array<char>* buffer) const;

	inline
	sl::Array<char>
	saveDer() const {
		sl::Array<char> buffer;
		saveDer(&buffer);
		return buffer;
	}

	bool
	loadPem(
		const void* p,
		size_t size
	);

	inline
	bool
	loadPem(const sl::StringRef& string) {
		return loadPem(string.cp(), string.getLength());
	}

	size_t
	savePem(sl::String* string) const;

	inline
	sl::String
	savePem() const {
		sl::String string;
		savePem(&string);
		return string;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
X509Cert::create() {
	close();
	m_h = ::X509_new();
	return completeWithLastCryptoError(m_h != NULL);
}

inline
ASN1_TIME*
X509Cert::getNotBefore() const {
	ASSERT(m_h);

#if (OPENSSL_VERSION_NUMBER >= 0x10100000)
	return ::X509_getm_notBefore(m_h);
#else
	return X509_get_notBefore(m_h);
#endif
}

inline
ASN1_TIME*
X509Cert::getNotAfter() const {
	ASSERT(m_h);

#if (OPENSSL_VERSION_NUMBER >= 0x10100000)
	return ::X509_getm_notAfter(m_h);
#else
	return X509_get_notAfter(m_h);
#endif
}

inline
bool
X509Cert::setVersion(X509Version version) {
	ASSERT(m_h);
	int result = ::X509_set_version(m_h, version);
	return completeWithLastCryptoError(result);
}

inline
ASN1_INTEGER*
X509Cert::getSerialNumber() const {
	ASSERT(m_h);
	return ::X509_get_serialNumber(m_h);
}

inline
bool
X509Cert::setSerialNumber(long serial) {
	int result = ::ASN1_INTEGER_set(getSerialNumber(), serial);
	return completeWithLastCryptoError(result);
}

inline
bool
X509Cert::setNotBefore(const ASN1_TIME* time) {
	ASSERT(m_h);

#if (OPENSSL_VERSION_NUMBER >= 0x10100000)
	int result = ::X509_set1_notBefore(m_h, time);
#else
	int result = X509_set_notBefore(m_h, (ASN1_TIME*)time); // pre-1.1.0 takes non-const
#endif

	return completeWithLastCryptoError(result);
}

inline
bool
X509Cert::setNotBefore(uint64_t timestamp) {
	time_t time = sys::getUnixTimeFromTimestamp(timestamp);
	bool result = ::ASN1_TIME_set(getNotBefore(), time) != NULL;
	return completeWithLastCryptoError(result);
}

inline
bool
X509Cert::setNotAfter(const ASN1_TIME* time) {
	ASSERT(m_h);

#if (OPENSSL_VERSION_NUMBER >= 0x10100000)
	int result = ::X509_set1_notAfter(m_h, time);
#else
	int result = X509_set_notAfter(m_h, (ASN1_TIME*)time); // pre-1.1.0 takes non-const
#endif

	return completeWithLastCryptoError(result);
}

inline
bool
X509Cert::setNotAfter(uint64_t timestamp) {
	time_t time = sys::getUnixTimeFromTimestamp(timestamp);
	bool result = ::ASN1_TIME_set(getNotAfter(), time) != NULL;
	return completeWithLastCryptoError(result);
}

inline
bool
X509Cert::setSubjectName(X509_NAME* name) {
	ASSERT(m_h);
	int result = ::X509_set_subject_name(m_h, name);
	return completeWithLastCryptoError(result);
}

inline
bool
X509Cert::setIssuerName(X509_NAME* name) {
	ASSERT(m_h);
	int result = ::X509_set_issuer_name(m_h, name);
	return completeWithLastCryptoError(result);
}

inline
bool
X509Cert::setPublicKey(EVP_PKEY* key) {
	ASSERT(m_h);
	int result = ::X509_set_pubkey(m_h, key);
	return completeWithLastCryptoError(result);
}

inline
bool
X509Cert::sign(
	EVP_PKEY* key,
	const EVP_MD* digest
) {
	ASSERT(m_h);
	int result = ::X509_sign(m_h, key, digest);
	return completeWithLastCryptoError(result);
}

//..............................................................................

} // namespace cry
} // namespace axl
