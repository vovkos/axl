#include "pch.h"
#include "axl_cry_EcProductKey.h"
#include "axl_cry_EcKey.h"
#include "axl_cry_EcPoint.h"
#include "axl_enc_Base32Encoding.h"

#define CURVE_NAME NID_secp112r1

namespace axl {
namespace cry {

//.............................................................................

rtl::String getPrivateKeyInDecimal (EC_KEY* keys)
{
	rtl::String keyString;

	const BIGNUM* privateKey = EC_KEY_get0_private_key (keys);
	char* keyBuffer = BN_bn2dec (privateKey);
	keyString = keyBuffer;
	OPENSSL_free (keyBuffer);

	return keyString;
}

rtl::String getPublicKeyInDecimal (EC_KEY* keys)
{
	rtl::String keyString;

	BIGNUM* publicKey = EC_POINT_point2bn (
		EC_KEY_get0_group (keys),
		EC_KEY_get0_public_key (keys),
		POINT_CONVERSION_COMPRESSED,
		NULL,
		NULL
		);

	char* keyBuffer = BN_bn2dec (publicKey);
	keyString = keyBuffer;
	OPENSSL_free (keyBuffer);
	BN_free (publicKey);
	
	return keyString;
}

bool initPublicKey (EC_KEY* keys)
{
	int result = 0;

	const EC_GROUP* group = EC_KEY_get0_group (keys);

	EC_POINT* publicKey = EC_POINT_new (group);
	result = EC_POINT_mul (group, publicKey, EC_KEY_get0_private_key (keys), NULL, NULL, NULL);

	if (result)
		result = EC_KEY_set_public_key (keys, publicKey);
	
	EC_POINT_free (publicKey);
	return result == 1;
}

//.............................................................................

bool ProductKey::generateKeys ()
{
	m_privateKey.clear ();
	m_publicKey.clear ();
	
	EC_KEY* keys = EC_KEY_new_by_curve_name (CURVE_NAME);

	if (!keys)
		return false;

	int result = EC_KEY_generate_key (keys);

	if (!result)
	{
		EC_KEY_free (keys);
		return false;
	}

	m_privateKey = getPrivateKeyInDecimal (keys);
	m_publicKey = getPublicKeyInDecimal (keys);

	EC_KEY_free (keys);
	return true;
}

bool ProductKey::generateProductKey (
	const rtl::String& privateKeyString, 
	const rtl::String& username)
{
	m_productKey.clear ();
	
	int result;

	BigNum privateKey;
	result = BN_dec2bn (privateKey.getHandlePtr (), privateKeyString.cc ());

	if (!result)
		return false;

	EcKey keys = EC_KEY_new_by_curve_name (CURVE_NAME);

	if (!keys)
		return false;

	result = EC_KEY_set_private_key (keys, privateKey);

	if (!result)
		return false;

	if (!initPublicKey (keys))
		return false;

	unsigned char digest [MD5_DIGEST_LENGTH];
	MD5 ((const unsigned char*) username.cc (), username.getLength (), digest);

	int maxSize = ECDSA_size (keys);

	if (!maxSize)
		return false;

	unsigned char* signature = new unsigned char [maxSize];
	unsigned int size;

	result = ECDSA_sign (0, digest, MD5_DIGEST_LENGTH, signature, &size, keys);

	if (result)
		m_productKey; // = ProductKey::encode (signature, size);

	delete [] signature;
	
	return result == 1;
}

bool ProductKey::verifyProductKey (
	const rtl::String& username, 
	const rtl::String& productKey, 
	const rtl::String& publicKey
	)
{
	rtl::String key; // = decode (productKey);
	if (key.isEmpty ())
		return false;

	int result;

	BigNum bn;
	result = BN_dec2bn (bn.getHandlePtr (), publicKey.cc ());

	if (!result)
		return false;

	EcKey ecKey = EC_KEY_new_by_curve_name (CURVE_NAME);
	if (!ecKey)
		return false;

	const EC_GROUP* group = EC_KEY_get0_group (ecKey);
	EcPoint publicKeyPoint = EC_POINT_bn2point (group, bn, NULL, NULL);

	if (!publicKeyPoint)
		return false;

	result = EC_KEY_set_public_key (ecKey, publicKeyPoint);

	if (!result)
		return false;
	
	unsigned char digest [MD5_DIGEST_LENGTH];
	MD5 ((const unsigned char*) username.cc (), username.getLength (), digest);

	return ECDSA_verify (
		0, 
		digest, 
		MD5_DIGEST_LENGTH, 
		(const unsigned char*) key.cc (), 
		(int) key.getLength (), 
		ecKey
		) == 1;
}

//.............................................................................

} // namespace cry
} // namespace axl
