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

#pragma once

#define _AXL_CRY_RSA_H

#include "axl_cry_Bio.h"

namespace axl {
namespace cry {

//..............................................................................

class FreeRsa
{
public:
	void
	operator () (RSA* h)
	{
		RSA_free (h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Rsa: public sl::Handle <RSA*, FreeRsa>
{
public:
	Rsa ()
	{
	}

	size_t
	getSize ()
	{
		return RSA_size (m_h);
	}

	bool
	readPublicKey (BIO* bio);

	bool
	readPublicKey (
		const void* pem,
		size_t size
		)
	{
		Bio bio;
		return bio.create (pem, size) && readPublicKey (bio);
	}

	bool
	readPrivateKey (BIO* bio);

	bool
	readPrivateKey (
		const void* pem,
		size_t size
		)
	{
		Bio bio;
		return bio.create (pem, size) && readPrivateKey (bio);
	}

	size_t
	signHash (
		int type,
		void* signature,
		size_t signatureSize,
		const void* hash,
		size_t hashSize
		);

	bool
	signHash (
		int type,
		sl::Array <char>* signature,
		const void* hash,
		size_t hashSize
		);

	sl::Array <char>
	signHash (
		int type,
		const void* hash,
		size_t hashSize
		)
	{
		sl::Array <char> signature;
		signHash (type, &signature, hash, hashSize);
		return signature;
	}

	bool
	sign (
		sl::Array <char>* signature,
		const void* p,
		size_t size
		)
	{
		char hash [MD5_DIGEST_LENGTH];
		MD5 ((const uchar_t*) p, size, (uchar_t*) hash);
		return signHash (NID_md5, signature, hash, sizeof (hash));
	}

	sl::Array <char>
	sign (
		const void* p,
		size_t size
		)
	{
		sl::Array <char> signature;
		sign (&signature, p, size);
		return signature;
	}

	bool
	verifyHash (
		int type,
		const void* hash,
		size_t hashSize,
		const void* signature,
		size_t signatureSize
		)
	{
		return RSA_verify (
			type,
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
		return verifyHash (NID_md5, hash, sizeof (hash), signature, signatureSize);
	}
};

//..............................................................................

} // namespace cry
} // namespace axl
