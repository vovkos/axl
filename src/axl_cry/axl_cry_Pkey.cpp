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

static
int
passphraseCallback(
	char* buffer,
	int size,
	int rwFlag,
	void* context
) {
	const sl::StringRef* passphrase = (const sl::StringRef*)context;
	size_t length = passphrase->getLength();
	if (length > (size_t)size)
		return -1;

	memcpy(buffer, passphrase->cp(), length); // not null-terminated
	return (int)length;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
Pkey::readPrivateKeyPem(
	const sl::StringRef& pem,
	const sl::StringRef& passphrase
) {
	close();

	Bio bio;
	bool result = bio.createMemBuf(pem.cp(), pem.getLength());
	if (!result)
		return false;

	m_h = ::PEM_read_bio_PrivateKey(bio, NULL, passphraseCallback, (void*)&passphrase);
	return completeWithLastCryptoError(m_h != NULL);
}

bool
Pkey::readPrivateKeyPemFile(
	const sl::StringRef& fileName,
	const sl::StringRef& passphrase
) {
	close();

	Bio bio;
	bool result = bio.createFile(fileName);
	if (!result)
		return false;

	m_h = ::PEM_read_bio_PrivateKey(bio, NULL, passphraseCallback, (void*)&passphrase);
	return completeWithLastCryptoError(m_h != NULL);
}

//..............................................................................

} // namespace cry
} // namespace axl
