#include "pch.h"
#include "axl_cry_BigNum.h"

namespace axl {
namespace cry {

//.............................................................................

BIGNUM*
BnCtx::getBigNum ()
{
	BIGNUM* result = BN_CTX_get (m_h);
	if (!result)
	{
		setLastCryptoError ();
		return NULL;
	}

	return result;
}

//.............................................................................

bool
BigNum::create ()
{
	close ();
	
	m_h = BN_new ();
	return completeWithLastCryptoError (m_h != NULL);
}

bool
BigNum::createCopy (BIGNUM* src)
{
	close ();

	m_h = BN_dup (src);
	return completeWithLastCryptoError (m_h != NULL);
}

size_t
BigNum::getData (
	void* p,
	size_t size
	)
{
	ASSERT (size >= getSize ());

	int result = BN_bn2bin (m_h, (uchar_t*) p);
	if (result <= 0)
	{
		setLastCryptoError ();
		return -1;
	}

	return result;
}

bool
BigNum::getDecString (rtl::String* string)
{		
	char* p = BN_bn2dec (m_h);
	if (!p)
	{
		setLastCryptoError ();
		return false;
	}

	*string = p;
	OPENSSL_free (p);
	return true;
}

bool
BigNum::getHexString (rtl::String* string)
{
	char* p = BN_bn2hex (m_h);
	if (!p)
	{
		setLastCryptoError ();
		return false;
	}

	*string = p;
	OPENSSL_free (p);
	return true;
}

//.............................................................................

} // namespace cry
} // namespace axl
