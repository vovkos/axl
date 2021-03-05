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

namespace axl {
namespace cry {

//..............................................................................

bool
X509Cert::create()
{
	close();

	m_h = X509_new();
	return completeWithLastCryptoError(m_h != NULL);
}

bool
X509Cert::loadDer(
	const void* p0,
	size_t size
	)
{
	close();

	const uchar_t* p = (uchar_t*)p0;
	X509* cert = X509_new();
	X509* result = d2i_X509(&cert, &p, size);
	if (!result)
	{
		ASSERT(cert == NULL); // should have been freed already
		return failWithLastCryptoError();
	}

	attach(cert);
	return true;
}

size_t
X509Cert::saveDer(sl::Array<char>* buffer) const
{
	uchar_t* p = NULL;
	int length = i2d_X509(m_h, &p);
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
	)
{
	close();

	Bio bio;
	bio.createMemBuf(p, size);

	X509* cert = X509_new();
	X509* result = PEM_read_bio_X509(bio, &cert, NULL, NULL);
	if (!result)
	{
		ASSERT(cert == NULL); // should have been freed already
		return failWithLastCryptoError();
	}

	attach(cert);
	return true;
}

size_t
X509Cert::savePem(sl::String* string) const
{
	Bio bio;
	bio.createMem();
	PEM_write_bio_X509(bio, m_h);

	BUF_MEM* mem = bio.getBufMem();
	return string->copy(mem->data, mem->length);
}

//..............................................................................

bool
X509Store::create()
{
	close();
	m_h = X509_STORE_new();
	return completeWithLastCryptoError(m_h != NULL);
}

bool
X509Store::addCert(X509* cert)
{
	ASSERT(m_h);
	int result = X509_STORE_add_cert(m_h, cert);
	return completeWithLastCryptoError(result);
}

//..............................................................................

} // namespace cry
} // namespace axl
