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
#include "axl_cry_Pkey.h"
#include "axl_cry_Bio.h"

namespace axl {
namespace cry {

//..............................................................................

struct PassphraseContext {
	const sl::StringRef* m_passphrase;
	bool m_isPassphraseProtected;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

static
int
passphraseCallback(
	char* buffer,
	int size,
	int rwFlag,
	void* context0
) {
	PassphraseContext* context = (PassphraseContext*)context0;
	context->m_isPassphraseProtected = true;

	size_t length = context->m_passphrase->getLength();
	if (length > (size_t)size)
		return -1;

	memcpy(buffer, context->m_passphrase->cp(), length);
	return (int)length;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

PrivateKeyResult
Pkey::readPrivateKeyPem(
	const sl::StringRef& pem,
	const sl::StringRef& passphrase
) {
	close();

	Bio bio;
	bool result = bio.createMemBuf(pem.cp(), pem.getLength());
	if (!result)
		return PrivateKeyResult_Error;

	PassphraseContext context = { &passphrase, false };
	m_h = ::PEM_read_bio_PrivateKey(bio, NULL, passphraseCallback, &context);
	return m_h ?
		PrivateKeyResult_Success :
		failWithLastCryptoError(
			context.m_isPassphraseProtected ?
			PrivateKeyResult_PassphraseError :
			PrivateKeyResult_Error
		);
}

PrivateKeyResult
Pkey::readPrivateKeyPemFile(
	const sl::StringRef& fileName,
	const sl::StringRef& passphrase
) {
	close();

	Bio bio;
	bool result = bio.createFile(fileName);
	if (!result)
		return PrivateKeyResult_Error;

	PassphraseContext context = { &passphrase, false };
	m_h = ::PEM_read_bio_PrivateKey(bio, NULL, passphraseCallback, &context);
	return m_h ?
		PrivateKeyResult_Success :
		failWithLastCryptoError(
			context.m_isPassphraseProtected ?
			PrivateKeyResult_PassphraseError :
			PrivateKeyResult_Error
		);
}

size_t
Pkey::writePrivateKeyPem(sl::String* string) const {
	ASSERT(m_h);

	Bio bio;
	bool result = bio.createMem();
	if (!result)
		return -1;

	result = ::PEM_write_bio_PrivateKey(bio, m_h, NULL, NULL, 0, NULL, NULL) != 0;
	if (!result)
		return failWithLastCryptoError<size_t>(-1);

	BUF_MEM* mem = bio.getBufMem();
	return string->copy(mem->data, mem->length);
}

//..............................................................................

} // namespace cry
} // namespace axl
