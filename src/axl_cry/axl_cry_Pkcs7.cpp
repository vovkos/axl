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
#include "axl_cry_Pkcs7.h"
#include "axl_cry_Bio.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//..............................................................................

bool
Pkcs7::create() {
	close();

	m_h = PKCS7_new();
	return completeWithLastCryptoError(m_h != NULL);
}

bool
Pkcs7::loadDer(
	const void* p0,
	size_t size
) {
	close();

	const uchar_t* p = (uchar_t*)p0;
	PKCS7* pkcs7 = PKCS7_new();
	PKCS7* result = d2i_PKCS7(&pkcs7, &p, size);
	if (!result) {
		ASSERT(pkcs7 == NULL); // should have been freed already
		return failWithLastCryptoError();
	}

	attach(pkcs7);
	return true;
}

size_t
Pkcs7::saveDer(sl::Array<char>* buffer) const {
	ASSERT(m_h);

	uchar_t* p = NULL;
	int length = i2d_PKCS7(m_h, &p);
	if (length <= 0)
		return failWithLastCryptoError<size_t>(-1);

	size_t result = buffer->copy((char*)p, length);
	OPENSSL_free(p);
	return result;
}

bool
Pkcs7::loadPem(
	const void* p,
	size_t size
) {
	close();

	Bio bio;
	bio.createMemBuf(p, size);

	PKCS7* pkcs7 = PKCS7_new();
	PKCS7* result = PEM_read_bio_PKCS7(bio, &pkcs7, NULL, NULL);
	if (!result) {
		ASSERT(pkcs7 == NULL); // should have been freed already
		return failWithLastCryptoError();
	}

	attach(pkcs7);
	return true;
}

size_t
Pkcs7::savePem(sl::String* string) const {
	ASSERT(m_h);

	Bio bio;
	bio.createMem();
	PEM_write_bio_PKCS7(bio, m_h);

	BUF_MEM* mem = bio.getBufMem();
	return string->copy(mem->data, mem->length);
}

bool
Pkcs7::verify(
	STACK_OF(X509)* certStack,
	X509_STORE* store,
	BIO* inData,
	BIO* outData,
	int flags
) {
	ASSERT(m_h);
	int result = PKCS7_verify(m_h, certStack, store, inData, outData, flags);
	return completeWithLastCryptoError(result);
}

//..............................................................................

} // namespace cry
} // namespace axl
