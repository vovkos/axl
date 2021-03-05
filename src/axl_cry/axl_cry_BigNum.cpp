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
#include "axl_cry_BigNum.h"

namespace axl {
namespace cry {

//..............................................................................

BIGNUM*
BnCtx::getBigNum()
{
	BIGNUM* result = BN_CTX_get(m_h);
	return completeWithLastCryptoError<BIGNUM*>(result, NULL);
}

//..............................................................................

bool
BigNum::create()
{
	close();

	m_h = BN_new();
	return completeWithLastCryptoError(m_h != NULL);
}

bool
BigNum::createCopy(BIGNUM* src)
{
	close();

	m_h = BN_dup(src);
	return completeWithLastCryptoError(m_h != NULL);
}

size_t
BigNum::getData(
	void* p,
	size_t size
	)
{
	ASSERT(size >= getSize());

	int result = BN_bn2bin(m_h, (uchar_t*)p);
	if (result <= 0)
		return failWithLastCryptoError<size_t>(-1);

	return result;
}

bool
BigNum::getDecString(sl::String* string)
{
	char* p = BN_bn2dec(m_h);
	if (!p)
		return failWithLastCryptoError();

	*string = p;
	OPENSSL_free(p);
	return true;
}

bool
BigNum::getHexString(sl::String* string)
{
	char* p = BN_bn2hex(m_h);
	if (!p)
		return failWithLastCryptoError();

	*string = p;
	OPENSSL_free(p);
	return true;
}

//..............................................................................

} // namespace cry
} // namespace axl
