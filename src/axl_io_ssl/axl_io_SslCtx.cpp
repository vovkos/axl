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

namespace axl {
namespace io {

//..............................................................................

bool
SslCtx::create(const SSL_METHOD* method)
{
	close();
	m_h = SSL_CTX_new(method);
	return cry::completeWithLastCryptoError(m_h != NULL);
}

bool
SslCtx::loadVerifyLocations(
	const sl::StringRef& caFileName,
	const sl::StringRef& caDir
	)
{
	ASSERT(m_h);

	int result = ::SSL_CTX_load_verify_locations(
		m_h,
		!caFileName.isEmpty() ? caFileName.sz() : NULL,
		!caDir.isEmpty() ? caDir.sz() : NULL
		);

	return cry::completeWithLastCryptoError(result > 0);
}

bool
SslCtx::setCipherList(const sl::StringRef& listString)
{
	ASSERT(m_h);
	int result = ::SSL_CTX_set_cipher_list(m_h, listString.sz());
	return cry::completeWithLastCryptoError(result > 0);
}

bool
SslCtx::useCertificate(const X509* cert)
{
	ASSERT(m_h);
	int result = ::SSL_CTX_use_certificate(m_h, (X509*)cert);
	return cry::completeWithLastCryptoError(result > 0);
}

bool
SslCtx::useCertificateFile(
	const sl::StringRef& fileName,
	int fileType
	)
{
	ASSERT(m_h);
	int result = ::SSL_CTX_use_certificate_file(m_h, fileName.sz(), fileType);
	return cry::completeWithLastCryptoError(result > 0);
}

bool
SslCtx::usePrivateKey(const EVP_PKEY* key)
{
	ASSERT(m_h);
	int result = ::SSL_CTX_use_PrivateKey(m_h, (EVP_PKEY*)key);
	return cry::completeWithLastCryptoError(result > 0);
}

bool
SslCtx::usePrivateKeyFile(
	const sl::StringRef& fileName,
	int fileType
	)
{
	ASSERT(m_h);
	int result = ::SSL_CTX_use_PrivateKey_file(m_h, fileName.sz(), fileType);
	return cry::completeWithLastCryptoError(result > 0);
}

bool
SslCtx::addExtraChainCertificate(const X509* cert)
{
	ASSERT(m_h);
	int result = ::SSL_CTX_add_extra_chain_cert(m_h, (X509*)cert);
	return cry::completeWithLastCryptoError(result > 0);
}

//..............................................................................

} // namespace io
} // namespace axl
