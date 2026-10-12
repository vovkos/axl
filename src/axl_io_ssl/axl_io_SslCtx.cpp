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
#include "axl_io_SslCtx.h"
#include "axl_cry_CryptoError.h"
#include "axl_cry_Pkey.h"

#if (_AXL_OS_WIN)
#	include "axl_cry_X509Store.h"
#	include "axl_sys_win_Certificate.h"
#endif

namespace axl {
namespace io {

//..............................................................................

bool
SslCtx::create(const SSL_METHOD* method) {
	close();
	m_h = SSL_CTX_new(method);
	return cry::completeWithLastCryptoError(m_h != NULL);
}

bool
SslCtx::setDefaultVerifyPaths() {
	ASSERT(m_h);

#if (_AXL_OS_WIN)
	// on windows, SSL_CTX_set_default_verify_paths point to its build tree (useless)
	// instead, use the system root store as default

	sys::win::CertStore winStore;
	bool result = winStore.openSystemStore(CERT_SYSTEM_STORE_CURRENT_USER, L"ROOT");
	if (!result)
		return false;

	cry::X509Store store(getCertStore());
	result = store.addWinCertStore(winStore) != -1;
	store.detach(); // owned by the SSL_CTX
	return result;
#else
	int result = ::SSL_CTX_set_default_verify_paths(m_h);
	return cry::completeWithLastCryptoError(result);
#endif
}

bool
SslCtx::loadVerifyLocations(
	const sl::StringRef& caFileName,
	const sl::StringRef& caDir
) {
	ASSERT(m_h);
	int result = ::SSL_CTX_load_verify_locations(m_h, caFileName.szn(), caDir.szn());
	return cry::completeWithLastCryptoError(result);
}

bool
SslCtx::setTmpDh(DH* dh) {
	ASSERT(m_h);
	int result = ::SSL_CTX_set_tmp_dh(m_h, dh);
	return cry::completeWithLastCryptoError(result);
}

bool
SslCtx::setTmpEcdh(EC_KEY* ecKey) {
	ASSERT(m_h);
	int result = ::SSL_CTX_set_tmp_ecdh(m_h, ecKey);
	return cry::completeWithLastCryptoError(result);
}

bool
SslCtx::setCipherList(const sl::StringRef& listString) {
	ASSERT(m_h);
	int result = ::SSL_CTX_set_cipher_list(m_h, listString.sz());
	return cry::completeWithLastCryptoError(result);
}

bool
SslCtx::useCertificate(const X509* cert) {
	ASSERT(m_h);
	int result = ::SSL_CTX_use_certificate(m_h, (X509*)cert);
	return cry::completeWithLastCryptoError(result);
}

bool
SslCtx::useCertificateFile(
	const sl::StringRef& fileName,
	int fileType
) {
	ASSERT(m_h);
	int result = ::SSL_CTX_use_certificate_file(m_h, fileName.sz(), fileType);
	return cry::completeWithLastCryptoError(result);
}

bool
SslCtx::usePrivateKey(const EVP_PKEY* key) {
	ASSERT(m_h);
	int result = ::SSL_CTX_use_PrivateKey(m_h, (EVP_PKEY*)key);
	return cry::completeWithLastCryptoError(result);
}

bool
SslCtx::usePrivateKeyFile(
	const sl::StringRef& fileName,
	int fileType
) {
	ASSERT(m_h);
	int result = ::SSL_CTX_use_PrivateKey_file(m_h, fileName.sz(), fileType);
	return cry::completeWithLastCryptoError(result);
}

cry::PrivateKeyResult
SslCtx::usePrivateKeyPem(
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
SslCtx::usePrivateKeyPemFile(
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
SslCtx::addExtraChainCertificate(const X509* cert) {
	ASSERT(m_h);
	int result = ::SSL_CTX_add_extra_chain_cert(m_h, (X509*)cert);
	return cry::completeWithLastCryptoError(result);
}

//..............................................................................

} // namespace io
} // namespace axl
