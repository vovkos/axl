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

#define _AXL_IO_SSLCIPHER_H

#include "axl_io_SslPch.h"

namespace axl {
namespace io {

//..............................................................................

// SSL_CIPHER-s live in OpenSSL's static tables -- nothing to free

class SslCipher {
protected:
	const SSL_CIPHER* m_cipher;

public:
	SslCipher(const SSL_CIPHER* cipher = NULL) {
		m_cipher = cipher;
	}

	operator const SSL_CIPHER* () const {
		return m_cipher;
	}

	const char*
	getName() const {
		ASSERT(m_cipher);
		return ::SSL_CIPHER_get_name(m_cipher);
	}

	const char*
	getVersion() const { // the protocol version the cipher first appeared in
		ASSERT(m_cipher);
		return ::SSL_CIPHER_get_version(m_cipher);
	}

	sl::String
	getDescription() const;

	int
	getBits(int* algBits = NULL) const {
		ASSERT(m_cipher);
		return ::SSL_CIPHER_get_bits(m_cipher, algBits);
	}

	uint32_t
	getId() const {
		ASSERT(m_cipher);
		return ::SSL_CIPHER_get_id(m_cipher);
	}

#if (OPENSSL_VERSION_NUMBER >= 0x10100000L) // openssl-1.1.0
	int
	getKeyExchangeNid() const {
		ASSERT(m_cipher);
		return ::SSL_CIPHER_get_kx_nid(m_cipher);
	}

	int
	getAuthNid() const {
		ASSERT(m_cipher);
		return ::SSL_CIPHER_get_auth_nid(m_cipher);
	}

	int
	getCipherNid() const {
		ASSERT(m_cipher);
		return ::SSL_CIPHER_get_cipher_nid(m_cipher);
	}

	int
	getDigestNid() const {
		ASSERT(m_cipher);
		return ::SSL_CIPHER_get_digest_nid(m_cipher);
	}

	bool
	isAead() const {
		ASSERT(m_cipher);
		return ::SSL_CIPHER_is_aead(m_cipher) != 0;
	}
#endif

#if (OPENSSL_VERSION_NUMBER >= 0x10101000L) // openssl-1.1.1
	const char*
	getStandardName() const { // RFC/IANA name, e.g. TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
		ASSERT(m_cipher);
		return ::SSL_CIPHER_standard_name(m_cipher);
	}

	uint16_t
	getProtocolId() const { // the two-byte code on the wire
		ASSERT(m_cipher);
		return ::SSL_CIPHER_get_protocol_id(m_cipher);
	}
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
sl::String
SslCipher::getDescription() const {
	ASSERT(m_cipher);

	char buffer[256];
	::SSL_CIPHER_description(m_cipher, buffer, sizeof(buffer));
	return buffer;
}

//..............................................................................

} // namespace io
} // namespace axl
