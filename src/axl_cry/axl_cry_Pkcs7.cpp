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

size_t
savePkcs7_der(
	sl::Array<char>* buffer,
	const PKCS7* msg
	)
{
	uchar_t* p = NULL;
	int length = i2d_PKCS7((PKCS7*)msg, &p);
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

PKCS7*
loadPkcs7_der(
	const void* p0,
	size_t size
	)
{
	const uchar_t* p = (uchar_t*)p0;
	PKCS7* msg = PKCS7_new();
	PKCS7* result = d2i_PKCS7(&msg, &p, size);
	if (!result)
	{
		ASSERT(msg == NULL); // should have been freed already
		cry::setLastCryptoError();
		return NULL;
	}

	return msg;
}

//..............................................................................

size_t
savePkcs7_pem(
	sl::String* string,
	const PKCS7* msg
	)
{
	Bio bio;
	bio.createMem();
	PEM_write_bio_PKCS7(bio, (PKCS7*)msg);

	BUF_MEM* mem = bio.getBufMem();
	return string->copy(mem->data, mem->length);
}

PKCS7*
loadPkcs7_pem(
	const void* p,
	size_t size
	)
{
	Bio bio;
	bio.createMemBuf(p, size);

	PKCS7* msg = PKCS7_new();
	PKCS7* result = PEM_read_bio_PKCS7(bio, &msg, NULL, NULL);
	if (!result)
	{
		ASSERT(msg == NULL); // should have been freed already
		cry::setLastCryptoError();
		return NULL;
	}

	return msg;
}
//..............................................................................

} // namespace cry
} // namespace axl
