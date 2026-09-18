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
#include "axl_cry_X509.h"
#include "axl_cry_Bio.h"
#include "axl_cry_CryptoError.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_Certificate.h"
#endif

namespace axl {
namespace cry {

//..............................................................................

bool
X509Name::addEntry(
	const sl::StringRef& field,
	const sl::StringRef& value
) {
	ASSERT(m_h);

	int result = ::X509_NAME_add_entry_by_txt(
		m_h,
		field.sz(),
		MBSTRING_UTF8,
		(const uchar_t*)value.cp(),
		(int)value.getLength(),
		-1, // append
		0   // not multi-valued
	);

	return completeWithLastCryptoError(result);
}

size_t
X509Name::getString(sl::String* string) const {
	ASSERT(m_h);

	Bio bio;
	bool result = bio.createMem();
	if (!result)
		return -1;

	// RFC 2253 -- X509_NAME_oneline's format is legacy and ambiguous

	if (::X509_NAME_print_ex(bio, m_h, 0, XN_FLAG_RFC2253) < 0)
		return failWithLastCryptoError<size_t>(-1);

	BUF_MEM* mem = bio.getBufMem();
	return string->copy(mem->data, mem->length);
}

//..............................................................................

bool
X509Cert::loadDer(
	const void* p0,
	size_t size
) {
	close();

	const uchar_t* p = (uchar_t*)p0;
	X509* cert = ::X509_new();
	X509* result = ::d2i_X509(&cert, &p, size);
	if (!result) {
		ASSERT(cert == NULL); // should have been freed already
		return failWithLastCryptoError();
	}

	attach(cert);
	return true;
}

size_t
X509Cert::saveDer(sl::Array<char>* buffer) const {
	uchar_t* p = NULL;
	int length = ::i2d_X509(m_h, &p);
	if (length <= 0)
		return failWithLastCryptoError<size_t>(-1);

	size_t result = buffer->copy((char*)p, length);
	OPENSSL_free(p);
	return result;
}

bool
X509Cert::loadPem(
	const void* p,
	size_t size
) {
	close();

	Bio bio;
	bio.createMemBuf(p, size);

	X509* cert = ::X509_new();
	X509* result = ::PEM_read_bio_X509(bio, &cert, NULL, NULL);
	if (!result) {
		ASSERT(cert == NULL); // should have been freed already
		return failWithLastCryptoError();
	}

	attach(cert);
	return true;
}

size_t
X509Cert::savePem(sl::String* string) const {
	Bio bio;
	bio.createMem();
	::PEM_write_bio_X509(bio, m_h);

	BUF_MEM* mem = bio.getBufMem();
	return string->copy(mem->data, mem->length);
}

//..............................................................................

#if (_AXL_OS_WIN)

size_t
X509Store::addWinCertStore(const sys::win::CertStore& store) {
	ASSERT(m_h);

	size_t count = 0;
	sys::win::Certificate cert;
	for (size_t i = 0;; i++) {
		bool result = store.getNextCertificate(&cert);
		if (!result)
			return -1;

		if (!cert)
			break;

		sl::ArrayRef<char> der = cert.getDer();
		const uchar_t* p = (const uchar_t*)der.cp();

		X509Cert x509;
		::d2i_X509(x509.p(), &p, (long)der.getCount());
		if (!x509)
			return -1;

		result = addCert(x509);
		if (!result)
			return -1;

		count++;
	}

	return count;
}

#endif // _AXL_OS_WIN

//..............................................................................

} // namespace cry
} // namespace axl
