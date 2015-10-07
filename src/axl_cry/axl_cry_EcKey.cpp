#include "pch.h"
#include "axl_cry_EcKey.h"
#include "axl_enc_Base32Encoding.h"

namespace axl {
namespace cry {

//.............................................................................

void
EcKey::setFlags (uint_t flags)
{
	int prevFlags = getFlags ();

	addFlags (flags & ~prevFlags);
	removeFlags (prevFlags & ~flags);
}

bool
EcKey::create ()
{
	close ();

	m_h = EC_KEY_new ();
	return completeWithLastCryptoError (m_h != NULL);
}

bool
EcKey::create (uint_t curveId)
{
	close ();

	m_h = EC_KEY_new_by_curve_name (curveId);
	return completeWithLastCryptoError (m_h != NULL);
}

bool
EcKey::createCopy (EC_KEY* src)
{
	close ();

	m_h = EC_KEY_dup (src);
	return completeWithLastCryptoError (m_h != NULL);
}

bool
EcKey::setPublicKeyData (
	const void* p,
	size_t size,
	BN_CTX* ctx
	)
{
	EC_GROUP* group = getGroup ();
	EcPoint key (group);
		
	return 
		key.setData (group, p, size, ctx) && 
		setPublicKey (key);
}

bool
EcKey::setPublicKeyDecString (
	const char* string,
	BN_CTX* ctx
	)
{
	EC_GROUP* group = getGroup ();
	EcPoint key (group);
		
	return 
		key.setDecString (getGroup (), string, ctx) && 
		setPublicKey (key);
}

bool
EcKey::setPublicKeyHexString (
	const char* string,
	BN_CTX* ctx
	)
{
	EC_GROUP* group = getGroup ();
	EcPoint key (group);

	return 
		key.setHexString (group, string, ctx) && 
		setPublicKey (key);
}

size_t
EcKey::signHash (
	void* signature,
	size_t signatureSize,
	const void* hash,
	size_t hashSize
	)
{
	ASSERT (signatureSize >= getSignatureSize ());
	
	uint_t resultSize = (uint_t) signatureSize;
	int result = ECDSA_sign (
		0, 
		(const uchar_t*) hash, 
		(int) hashSize, 
		(uchar_t*) signature, 
		&resultSize,
		m_h
		);

	if (!result)
	{
		setLastCryptoError ();
		return -1;
	}

	ASSERT (resultSize <= signatureSize);
	return resultSize;
}

//.............................................................................

bool
generateEcProductKey (
	EC_KEY* ecKey0,
	sl::String* productKey,
	const char* userName,
	size_t hyphenDistance
	)
{
	char buffer [256];
	sl::Array <char> signature (ref::BufKind_Stack, buffer, sizeof (buffer));
	
	EcKey ecKey (ecKey0);

	bool result = 
		ecKey.sign (&signature, userName, strlen (userName)) &&
		enc::Base32Encoding::encode (productKey, signature, signature.getCount (), hyphenDistance) != -1;

	ecKey.detach ();
	return result;
}

bool
verifyEcProductKey (
	EC_KEY* ecKey0,
	const char* userName,
	const char* productKey
	)
{
	char buffer [256];
	sl::Array <char> signature (ref::BufKind_Stack, buffer, sizeof (buffer));

	EcKey ecKey (ecKey0);

	bool result = 
		enc::Base32Encoding::decode (&signature, productKey) != -1 &&
		ecKey.verify (userName, strlen (userName), signature, signature.getCount ());

	ecKey.detach ();
	return result;
}

//.............................................................................

} // namespace cry
} // namespace axl
