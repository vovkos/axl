// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_CRY_ECKEY_H

#include "axl_cry_BigNum.h"
#include "axl_cry_EcPoint.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//.............................................................................

class FreeEcKey
{
public:
	void 
	operator () (EC_KEY* h)
	{
		EC_KEY_free (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class EcKey: public rtl::Handle <EC_KEY*, FreeEcKey> 
{
public:
	EcKey ()
	{
		create ();
	}

	EcKey (uint_t curveId)
	{
		create (curveId);
	}

	EcKey (EC_KEY* h):
		rtl::Handle <EC_KEY*, FreeEcKey> (h)
	{
	}

	bool
	create ();

	bool
	create (uint_t curveId);
	
	bool
	createCopy (EC_KEY* src);

	bool
	copy (EC_KEY* src)
	{
		EC_KEY* result = EC_KEY_copy (m_h, src);
		return completeWithLastCryptoError (result != NULL);
	}

	uint_t 
	getFlags ()
	{
		return EC_KEY_get_flags (m_h);
	}

	void
	addFlags (uint_t flags)
	{
		EC_KEY_set_flags (m_h, flags);
	}

	void
	removeFlags (uint_t flags)
	{
		EC_KEY_clear_flags (m_h, flags);
	}

	void
	setFlags (uint_t flags);

	uint_t 
	getEncFlags ()
	{
		return EC_KEY_get_enc_flags (m_h);
	}

	void
	setEncFlags (uint_t flags)
	{
		EC_KEY_set_enc_flags (m_h, flags);
	}

	EC_GROUP*
	getGroup ()
	{
		return (EC_GROUP*) EC_KEY_get0_group (m_h);
	}

	bool
	setGroup (EC_GROUP* group)
	{
		int result = EC_KEY_set_group (m_h, group);
		return completeWithLastCryptoError (result);
	}

	BIGNUM*
	getPrivateKey ()
	{
		return (BIGNUM*) EC_KEY_get0_private_key (m_h);
	}

	bool
	setPrivateKey (BIGNUM* key)
	{
		int result = EC_KEY_set_private_key (m_h, key);
		return completeWithLastCryptoError (result);
	}

	bool
	setPrivateKeyData (
		const void* p,
		size_t size
		)
	{
		BigNum key;
		return key.setData (p, size) && setPrivateKey (key);
	}

	bool
	setPrivateKeyDecString (const char* string)
	{
		BigNum key;
		return key.setDecString (string) && setPrivateKey (key);
	}

	bool
	setPrivateKeyHexString (const char* string)
	{
		BigNum key;
		return key.setHexString (string) && setPrivateKey (key);
	}

	EC_POINT*
	getPublicKey ()
	{
		return (EC_POINT*) EC_KEY_get0_public_key (m_h);
	}

	bool
	setPublicKey (EC_POINT* key)
	{
		int result = EC_KEY_set_public_key (m_h, key);
		return completeWithLastCryptoError (result);
	}

	bool
	setPublicKeyData (
		const void* p,
		size_t size,
		BN_CTX* ctx = NULL
		);

	bool
	setPublicKeyDecString (
		const char* string,
		BN_CTX* ctx = NULL
		);

	bool
	setPublicKeyHexString (
		const char* string,
		BN_CTX* ctx = NULL
		);

	bool
	generateKey ()
	{
		int result = EC_KEY_generate_key (m_h);
		return completeWithLastCryptoError (result);
	}

	size_t
	getSignatureSize ()
	{
		return ECDSA_size (m_h);
	}

	size_t
	signHash (
		void* signature,
		size_t signatureSize,
		const void* hash,
		size_t hashSize
		);

	bool
	signHash (
		rtl::Array <char>* signature,
		const void* hash,
		size_t hashSize
		)
	{
		size_t signatureSize = getSignatureSize ();
		return signature->setCount (signatureSize) && signHash (*signature, signatureSize, hash, hashSize);
	}

	bool
	sign (
		rtl::Array <char>* signature,
		const void* p,
		size_t size
		)
	{
		char hash [MD5_DIGEST_LENGTH];
		MD5 ((const uchar_t*) p, size, (uchar_t*) hash);
		return signHash (signature, hash, sizeof (hash));
	}

	rtl::Array <char>
	signHash (
		const void* hash,
		size_t hashSize
		)
	{
		rtl::Array <char> signature;
		signHash (&signature, hash, hashSize);
		return signature;
	}

	rtl::Array <char>
	sign (
		const void* p,
		size_t size
		)
	{
		rtl::Array <char> signature;
		sign (&signature, p, size);
		return signature;
	}

	bool
	verifyHash (
		const void* hash,
		size_t hashSize,
		const void* signature,
		size_t signatureSize
		)
	{
		return ECDSA_verify (
			0, 
			(const uchar_t*) hash, 
			(int) hashSize, 
			(const uchar_t*) signature,
			(int) signatureSize, 
			m_h
			) == 1;
	}

	bool
	verify (
		const void* p,
		size_t size,
		const void* signature,
		size_t signatureSize
		)
	{
		char hash [MD5_DIGEST_LENGTH];
		MD5 ((const uchar_t*) p, size, (uchar_t*) hash);
		return verifyHash (hash, sizeof (hash), signature, signatureSize);
	}
};

//.............................................................................

bool
generateEcProductKey (
	EC_KEY* ecKey,
	rtl::String* productKey,
	const char* userName,
	size_t hyphenDistance = 6
	);

inline
rtl::String 
generateEcProductKey (
	EC_KEY* ecKey,
	const char* userName,
	size_t hyphenDistance = 6
	)
{
	rtl::String productKey;
	generateEcProductKey (ecKey, &productKey, userName, hyphenDistance);
	return productKey;
}

bool
verifyEcProductKey (
	EC_KEY* ecKey,
	const char* userName,
	const char* productKey
	);

//.............................................................................

} // namespace cry
} // namespace axl