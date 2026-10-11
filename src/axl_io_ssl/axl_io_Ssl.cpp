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
#include "axl_io_Ssl.h"
#include "axl_io_SslCipher.h"
#include "axl_cry_Pkey.h"
#include "axl_cry_EcKey.h"
#include "axl_cry_EcGroup.h"

namespace axl {
namespace io {

//..............................................................................

bool
Ssl::create(SSL_CTX* ctx) {
	close();

	m_h = SSL_new(ctx);
	return cry::completeWithLastCryptoError(m_h != NULL);
}

bool
Ssl::complete(int result) {
	if (result > 0)
		return true;

	setError(result);
	return false;
}

bool
Ssl::setTmpDh(DH* dh) {
	ASSERT(m_h);
	int result = ::SSL_set_tmp_dh(m_h, dh);
	return cry::completeWithLastCryptoError(result > 0);
}

bool
Ssl::setTmpEcdh(EC_KEY* ec) {
	ASSERT(m_h);
	int result = ::SSL_set_tmp_ecdh(m_h, ec);
	return cry::completeWithLastCryptoError(result > 0);
}

bool
Ssl::setCipherList(const sl::StringRef& listString) {
	ASSERT(m_h);
	int result = ::SSL_set_cipher_list(m_h, listString.sz());
	return complete(result);
}

sl::StringRef
Ssl::getKeyExchangeGroupName() {
	ASSERT(m_h);

#if (OPENSSL_VERSION_NUMBER >= 0x30200000L) // openssl-3.2
	const char* name = ::SSL_get0_group_name(m_h); // also names KEM groups (no tmp key)
	if (name)
		return sl::StringRef(name);
#endif

	// a client sees the group in the server's ephemeral key; a TLS 1.2 server keeps no keys
	// after the exchange, but with ECDHE, the first shared group is the one it picked

	int nid;
	cry::Pkey key;
#if (OPENSSL_VERSION_NUMBER >= 0x10101000L) // openssl-1.1.1
	int result = ::SSL_get_peer_tmp_key(m_h, key.p());
#else
	int result = ::SSL_get_server_tmp_key(m_h, key.p()); // client-side only, but so is 1.1.0's TLS 1.2
#endif
	if (result) {
		nid = key.getId();
		if (nid == EVP_PKEY_DH) // arbitrary parameters, so the size is all there is
			return sl::formatString("DH %d", ::EVP_PKEY_bits(key));

		if (nid == EVP_PKEY_EC) { // the curve is inside the key
			cry::EcKey ecKey(key.getEcKey());
			cry::EcGroup ecGroup(ecKey.getGroup());
			nid = ecGroup.getCurveId();
			ecKey.detach();
			ecGroup.detach();
		}
	} else {
		const SSL_CIPHER* cipher = getCurrentCipher();
		if (!isServer() || !cipher || SslCipher(cipher).getKeyExchangeNid() != NID_kx_ecdhe)
			return sl::StringRef();

#if (OPENSSL_VERSION_NUMBER >= 0x10101000L) // openssl-1.1.1
		nid = ::SSL_get_shared_group(m_h, 0);
#else
		nid = ::SSL_get_shared_curve(m_h, 0);
#endif
	}

	if (nid <= 0)
		return sl::StringRef();

	return cry::EcGroup::getCurveName(nid); // X25519 & X448 are key types themselves
}

bool
Ssl::setExtraData(
	int index,
	void* p
) {
	ASSERT(m_h);
	int result = ::SSL_set_ex_data(m_h, index, p);
	return complete(result);
}

bool
Ssl::useCertificate(const X509* cert) {
	ASSERT(m_h);
	int result = ::SSL_use_certificate(m_h, (X509*)cert);
	return complete(result);
}

bool
Ssl::useCertificateFile(
	const sl::StringRef& fileName,
	int fileType
) {
	ASSERT(m_h);
	int result = ::SSL_use_certificate_file(m_h, fileName.sz(), fileType);
	return complete(result);
}

bool
Ssl::usePrivateKey(const EVP_PKEY* key) {
	ASSERT(m_h);
	int result = ::SSL_use_PrivateKey(m_h, (EVP_PKEY*)key);
	return complete(result);
}

bool
Ssl::usePrivateKeyFile(
	const sl::StringRef& fileName,
	int fileType
) {
	ASSERT(m_h);
	int result = ::SSL_use_PrivateKey_file(m_h, fileName.sz(), fileType);
	return complete(result);
}

cry::PrivateKeyResult
Ssl::usePrivateKeyPem(
	const sl::StringRef& pem,
	const sl::StringRef& passphrase
) {
	ASSERT(m_h);

	cry::Pkey key;
	cry::PrivateKeyResult result = key.readPrivateKeyPem(pem, passphrase);
	return
		result < 0 ?
			result :
		usePrivateKey(key) ?
			cry::PrivateKeyResult_Success :
			cry::PrivateKeyResult_Error;
}

cry::PrivateKeyResult
Ssl::usePrivateKeyPemFile(
	const sl::StringRef& fileName,
	const sl::StringRef& passphrase
) {
	ASSERT(m_h);

	cry::Pkey key;
	cry::PrivateKeyResult result = key.readPrivateKeyPemFile(fileName, passphrase);
	return
		result < 0 ?
			result :
		usePrivateKey(key) ?
			cry::PrivateKeyResult_Success :
			cry::PrivateKeyResult_Error;
}

bool
Ssl::doHandshake() {
	ASSERT(m_h);

	int result = ::SSL_do_handshake(m_h);
	if (result <= 0) {
		setError(result);
		return false;
	}

	return true;
}

bool
Ssl::shutdown() {
	ASSERT(m_h);

	int result = ::SSL_shutdown(m_h);
	if (result < 0) { // 0 here means shutdown in progress
		setError(result);
		return false;
	}

	return true;
}

size_t
Ssl::write(
	const void* p,
	size_t size
) {
	ASSERT(m_h);

	int result = ::SSL_write(m_h, p, (int)size);
	if (result < 0) {
		setError(result);
		return -1;
	}

	return result;
}

size_t
Ssl::read(
	void* p,
	size_t size
) {
	ASSERT(m_h);

	int result = ::SSL_read(m_h, p, (int)size);
	if (result < 0) {
		setError(result);
		return -1;
	}

	return result;
}

//..............................................................................

} // namespace io
} // namespace axl
