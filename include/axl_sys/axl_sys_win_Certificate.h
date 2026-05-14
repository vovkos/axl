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

#define _AXL_SYS_WIN_CERTSTORE_H

#include "axl_sl_Handle.h"
#include "axl_sl_String.h"
#include "axl_sl_Array.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class FreeCertContext {
public:
	void
	operator () (const CERT_CONTEXT* p) {
		::CertFreeCertificateContext(p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Certificate: public sl::Handle<const CERT_CONTEXT*, FreeCertContext> {
public:
	Certificate() {}

	Certificate(const CERT_CONTEXT* p):
		sl::Handle<const CERT_CONTEXT*, FreeCertContext>(p) {}

	uint64_t
	getNotBefore() const {
		ASSERT(m_h);
		return *(uint64_t*)&m_h->pCertInfo->NotBefore;
	}

	uint64_t
	getNotAfter() const {
		ASSERT(m_h);
		return *(uint64_t*)&m_h->pCertInfo->NotAfter;
	}

	size_t
	getNameString(
		sl::String_w* string,
		dword_t type,
		dword_t flags = 0,
		void* typeParam = NULL
	) const;

	sl::String_w
	getNameString(
		dword_t type,
		dword_t flags = 0,
		void* typeParam = NULL
	) const;

	size_t
	Certificate::getPropertySize(dword_t propId) const;

	size_t
	Certificate::getProperty(
		sl::Array<char>* buffer,
		dword_t propId
	) const;

	sl::Array<char>
	Certificate::getProperty(dword_t propId) const;

	size_t
	Certificate::getStringProperty(
		sl::String_w* string,
		dword_t propId
	) const;

	sl::String_w
	Certificate::getStringProperty(dword_t propId) const;

	size_t
	getSubjectName(sl::String_w* string) const {
		return getNameString(string, CERT_NAME_SIMPLE_DISPLAY_TYPE);
	}

	sl::String_w
	getSubjectName() const {
		return getNameString(CERT_NAME_SIMPLE_DISPLAY_TYPE);
	}

	size_t
	getIssuerName(sl::String_w* string) const {
		return getNameString(string, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG);
	}

	sl::String_w
	getIssuerName() const {
		return getNameString(CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG);
	}

	size_t
	getFriendlyName(sl::String_w* string) const {
		return getStringProperty(string, CERT_FRIENDLY_NAME_PROP_ID);
	}

	sl::String_w
	getFriendlyName() const {
		return getStringProperty(CERT_FRIENDLY_NAME_PROP_ID);
	}

	size_t
	getThumbprint(sl::Array<char>* thumb) const {
		return getProperty(thumb, CERT_SHA1_HASH_PROP_ID);
	}

	sl::Array<char>
	getThumbprint() const {
		return getProperty(CERT_FRIENDLY_NAME_PROP_ID);
	}

	sl::ArrayRef<char>
	getDer() const {
		ASSERT(m_h);
		return sl::ArrayRef<char>((char*)m_h->pbCertEncoded, m_h->cbCertEncoded);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
size_t
Certificate::getNameString(
	sl::String_w* string,
	dword_t type,
	dword_t flags,
	void* typeParam
) const {
	ASSERT(m_h);

	dword_t size = ::CertGetNameStringW(m_h, type, flags, typeParam, NULL, 0);
	if (!size)
		return err::failWithLastSystemError<size_t>(-1);

	size_t length = size - 1;
	wchar_t* p = string->createBuffer(size - 1);
	::CertGetNameStringW(m_h, type, flags, typeParam, p, size);
	return length;
}

inline
sl::String_w
Certificate::getNameString(
	dword_t type,
	dword_t flags,
	void* typeParam
) const {
	sl::String_w string;
	getNameString(&string, type, flags, typeParam);
	return string;
}

inline
size_t
Certificate::getPropertySize(dword_t propId) const {
	dword_t size = 0;
	bool_t result = ::CertGetCertificateContextProperty(m_h, propId, NULL, &size);
	return result ? size : err::failWithLastSystemError<size_t>(-1);
}

inline
size_t
Certificate::getProperty(
	sl::Array<char>* buffer,
	dword_t propId
) const {
	dword_t size = getPropertySize(propId);
	if (size == -1)
		return -1;

	buffer->setCount(size);
	bool_t result = ::CertGetCertificateContextProperty(m_h, propId, buffer->p(), &size);
	return result ? buffer->getCount() : err::failWithLastSystemError<size_t>(-1);
}

inline
sl::Array<char>
Certificate::getProperty(dword_t propId) const {
	sl::Array<char> prop;
	getProperty(&prop, propId);
	return prop;
}

inline
size_t
Certificate::getStringProperty(
	sl::String_w* string,
	dword_t propId
) const {
	dword_t size = getPropertySize(propId);
	if (size == -1)
		return -1;

	string->createBuffer(size / sizeof(wchar_t) - 1);
	bool_t result = ::CertGetCertificateContextProperty(m_h, propId, string->p(), &size);
	return result ? string->getLength()  : err::failWithLastSystemError<size_t>(-1);
}

inline
sl::String_w
Certificate::getStringProperty(dword_t propId) const {
	sl::String_w string;
	getStringProperty(&string, propId);
	return string;
}

//..............................................................................

class CloseCertStore {
public:
	void
	operator () (HCERTSTORE h) {
		::CertCloseStore(h, 0);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CertStore: public sl::Handle<HCERTSTORE, CloseCertStore> {
public:
	CertStore() {}

	CertStore(HCERTSTORE h):
		sl::Handle<HCERTSTORE, CloseCertStore>(h) {}

	bool
	openSystemStore(
		dword_t location, // e.g., CERT_SYSTEM_STORE_LOCAL_MACHINE
		const sl::StringRef_w& name
	);

	bool
	getNextCertificate(
		Certificate* certificate,
		const CERT_CONTEXT* prev = NULL
	) const;

	Certificate
	getNextCertificate(const CERT_CONTEXT* prev = NULL) const;

	Certificate
	findCertificate(
		dword_t encodingType,
		dword_t findFlags,
		dword_t findType,
		const void* findParam,
		const CERT_CONTEXT* prev = NULL
	);

	bool
	findCertificate(
		Certificate* certificate,
		dword_t encodingType,
		dword_t findFlags,
		dword_t findType,
		const void* findParam,
		const CERT_CONTEXT* prev = NULL
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
CertStore::openSystemStore(
	dword_t location,
	const sl::StringRef_w& name
) {
	close();

	dword_t flags = location | CERT_STORE_OPEN_EXISTING_FLAG | CERT_STORE_READONLY_FLAG;
	HCERTSTORE h = ::CertOpenStore(CERT_STORE_PROV_SYSTEM_W, 0, NULL, flags, name.sz());
	if (!h)
		return err::failWithLastSystemError();

	attach(h);
	return true;
}

inline
bool
CertStore::getNextCertificate(
	Certificate* certificate,
	const CERT_CONTEXT* prev
) const {
	ASSERT(m_h);

	const CERT_CONTEXT* next = ::CertEnumCertificatesInStore(m_h, prev);
	if (!next) {
		dword_t error = ::GetLastError();
		if (error != CRYPT_E_NOT_FOUND)
			return err::fail(error);
	}

	certificate->attach(next);
	return true;
}

inline
Certificate
CertStore::getNextCertificate(const CERT_CONTEXT* prev) const {
	Certificate certificate;
	getNextCertificate(&certificate, prev);
	return certificate;
}

inline
bool
CertStore::findCertificate(
	Certificate* certificate,
	dword_t encodingType,
	dword_t findFlags,
	dword_t findType,
	const void* findParam,
	const CERT_CONTEXT* prev
) {
	ASSERT(m_h);

	const CERT_CONTEXT* next = ::CertFindCertificateInStore(
		m_h,
		encodingType,
		findFlags,
		findType,
		findParam,
		prev
	);

	if (!next) {
		dword_t error = ::GetLastError();
		if (error != CRYPT_E_NOT_FOUND)
			return err::fail(error);
	}

	certificate->attach(next);
	return true;
}

inline
Certificate
CertStore::findCertificate(
	dword_t encodingType,
	dword_t findFlags,
	dword_t findType,
	const void* findParam,
	const CERT_CONTEXT* prev
) {
	Certificate certificate;
	findCertificate(&certificate, encodingType, findFlags, findType, findParam,	prev);
	return certificate;
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
