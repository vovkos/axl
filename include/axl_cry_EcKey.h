// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_CRY_ECKEY_H

#include "axl_cry_BigNum.h"
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
	createCopy (const EC_KEY* src);

	bool
	copy (const EC_KEY* src)
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

	const EC_GROUP*
	getGroup ()
	{
		return EC_KEY_get0_group (m_h);
	}

	bool
	setGroup (const EC_GROUP* group)
	{
		int result = EC_KEY_set_group (m_h, group);
		return completeWithLastCryptoError (result);
	}

	const BIGNUM*
	getPrivateKey ()
	{
		return EC_KEY_get0_private_key (m_h);
	}

	bool
	setPrivateKey (const BIGNUM* privateKey)
	{
		int result = EC_KEY_set_private_key (m_h, privateKey);
		return completeWithLastCryptoError (result);
	}

	const EC_POINT*
	getPublicKey ()
	{
		return EC_KEY_get0_public_key (m_h);
	}

	bool
	setPublicKey (const EC_POINT* publicKey)
	{
		int result = EC_KEY_set_public_key (m_h, publicKey);
		return completeWithLastCryptoError (result);
	}

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
		return signature->setCount (signatureSize) && signHash (signature, signatureSize, hash, hashSize);
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
		const void* signature,
		size_t signatureSize,
		const void* hash,
		size_t hashSize
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
		const void* signature,
		size_t signatureSize,
		const void* p,
		size_t size
		)
	{
		char hash [MD5_DIGEST_LENGTH];
		MD5 ((const uchar_t*) p, size, (uchar_t*) hash);
		return verifyHash (signature, signatureSize, hash, sizeof (hash));
	}
};

//.............................................................................

} // namespace cry
} // namespace axl
