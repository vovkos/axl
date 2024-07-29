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
#include "axl_cry_EcKey.h"

namespace axl {
namespace cry {

//..............................................................................

void
EcKey::setFlags(uint_t flags) {
	int prevFlags = getFlags();

	addFlags(flags & ~prevFlags);
	removeFlags(prevFlags & ~flags);
}

bool
EcKey::create() {
	close();

	m_h = EC_KEY_new();
	return completeWithLastCryptoError(m_h != NULL);
}

bool
EcKey::create(uint_t curveId) {
	close();

	m_h = EC_KEY_new_by_curve_name(curveId);
	return completeWithLastCryptoError(m_h != NULL);
}

bool
EcKey::createCopy(EC_KEY* src) {
	close();

	m_h = EC_KEY_dup(src);
	return completeWithLastCryptoError(m_h != NULL);
}

bool
EcKey::setPublicKeyData(
	const void* p,
	size_t size,
	BN_CTX* ctx
) {
	EC_GROUP* group = getGroup();
	EcPoint key(group);

	return
		key.setData(group, p, size, ctx) &&
		setPublicKey(key);
}

bool
EcKey::setPublicKeyDecString(
	const sl::StringRef& string,
	BN_CTX* ctx
) {
	EC_GROUP* group = getGroup();
	EcPoint key(group);

	return
		key.setDecString(getGroup(), string, ctx) &&
		setPublicKey(key);
}

bool
EcKey::setPublicKeyHexString(
	const sl::StringRef& string,
	BN_CTX* ctx
) {
	EC_GROUP* group = getGroup();
	EcPoint key(group);

	return
		key.setHexString(group, string, ctx) &&
		setPublicKey(key);
}

size_t
EcKey::signHash(
	void* signatureBuffer,
	size_t signatureBufferSize,
	const void* hash,
	size_t hashSize
) {
	ASSERT(signatureBufferSize >= getMaxSignatureSize());

	uint_t resultSize = (uint_t)signatureBufferSize;
	int result = ECDSA_sign(
		0,
		(const uchar_t*)hash,
		(int)hashSize,
		(uchar_t*)signatureBuffer,
		&resultSize,
		m_h
	);

	if (!result)
		return failWithLastCryptoError<size_t>(-1);

	ASSERT(resultSize <= signatureBufferSize);
	return resultSize;
}

bool
EcKey::signHash(
	sl::Array<char>* signature,
	const void* hash,
	size_t hashSize
) {
	size_t maxSize = getMaxSignatureSize();
	bool result = signature->setCount(maxSize);
	if (!result)
		return false;

	size_t size = signHash(signature->p(), maxSize, hash, hashSize);
	if (size == -1)
		return false;

	ASSERT(size <= maxSize);
	signature->setCount(size);
	return true;
}

//..............................................................................

} // namespace cry
} // namespace axl
