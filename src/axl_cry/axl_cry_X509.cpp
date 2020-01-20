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

size_t
saveX509_der(
	sl::Array<char>* buffer,
	const X509* cert
	)
{
	uchar_t* p = NULL;
	int length = i2d_X509((X509*)cert, &p);
	if (length <= 0)
	{
		cry::setLastCryptoError();
		return -1;
	}

	size_t result = buffer->copy((char*)p, length);
	OPENSSL_free(p);
	return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
loadX509_der(
	X509* cert,
	const void* p0,
	size_t size
	)
{
	const uchar_t* p = (uchar_t*)p0;
	X509* result = d2i_X509(&cert, &p, size);
	if (!result)
	{
		cry::setLastCryptoError();
		return false;
	}

	return true;
}

X509*
loadX509_der(
	const void* p,
	size_t size
	)
{
	X509* cert = X509_new();
	bool result = loadX509_der(cert, p, size);
	if (!result)
	{
		X509_free(cert);
		return NULL;
	}

	return cert;
}

//..............................................................................

size_t
saveX509_pem(
	sl::String* string,
	const X509* cert
	)
{
	Bio bio;
	bio.createMem();
	PEM_write_bio_X509(bio, (X509*)cert);

	BUF_MEM* mem = bio.getBufMem();
	return string->copy(mem->data, mem->length);
}

bool
loadX509_pem(
	X509* cert,
	const void* p,
	size_t size
	)
{
	Bio bio;
	bio.createMemBuf(p, size);
	X509* result = PEM_read_bio_X509(bio, &cert, NULL, NULL);
	if (!result)
	{
		cry::setLastCryptoError();
		return false;
	}

	return true;
}

X509*
loadX509_pem(
	const void* p,
	size_t size
	)
{
	Bio bio;
	bio.createMemBuf(p, size);

	X509* cert = X509_new();
	X509* result = PEM_read_bio_X509(bio, &cert, NULL, NULL);
	if (!result)
	{
		X509_free(cert);
		return NULL;
	}

	return cert;
}
//..............................................................................

} // namespace cry
} // namespace axl
