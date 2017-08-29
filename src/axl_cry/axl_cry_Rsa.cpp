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
#include "axl_cry_Rsa.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//..............................................................................

bool
Rsa::readPublicKey (BIO* bio)
{
	close ();

	bool result = PEM_read_bio_RSA_PUBKEY (bio, &m_h, 0, NULL) != NULL;
	return completeWithLastCryptoError (result);
}

bool
Rsa::readPrivateKey (BIO* bio)
{
	close ();

	bool result = PEM_read_bio_RSA_PUBKEY (bio, &m_h, 0, NULL) != NULL;
	return completeWithLastCryptoError (result);
}

size_t
Rsa::signHash (
	int type,
	void* signatureBuffer,
	size_t signatureBufferSize,
	const void* hash,
	size_t hashSize
	)
{
	ASSERT (signatureBufferSize >= getSize ());

	uint_t signatureSize = (uint_t) signatureBufferSize;
	int result = RSA_sign (
		type,
		(const uchar_t*) hash,
		(int) hashSize,
		(uchar_t*) signatureBuffer,
		&signatureSize,
		m_h
		);

	if (!result)
	{
		setLastCryptoError ();
		return -1;
	}

	ASSERT (signatureSize <= signatureBufferSize);
	return signatureSize;
}

bool
Rsa::signHash (
	int type,
	sl::Array <char>* signature,
	const void* hash,
	size_t hashSize
	)
{
	size_t signatureBufferSize = getSize ();
	bool result = signature->setCount (signatureBufferSize);
	if (!result)
		return false;

	size_t size = signHash (type, *signature, signatureBufferSize, hash, hashSize);
	if (size == -1)
		return false;

	ASSERT (size <= signatureBufferSize);
	signature->setCount (size);
	return true;
}

//..............................................................................

} // namespace cry
} // namespace axl
