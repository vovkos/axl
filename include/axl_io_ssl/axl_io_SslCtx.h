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

#define _AXL_IO_SSLCTX_H

#include "axl_io_SslPch.h"

namespace axl {
namespace io {

//..............................................................................

class FreeSslCtx {
public:
	void
	operator () (SSL_CTX* h) {
		SSL_CTX_free(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef
void
SslInfoCallbackFunc(
	const SSL* ssl,
	int where,
	int ret
);

typedef
int
SslVerifyCallbackFunc(
	int isPreVerifyOk,
	X509_STORE_CTX* ctx
);

typedef
DH*
SslTmpDhCallbackFunc(
	SSL* ssl,
	int isExport,
	int keyLength
);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class SslCtx: public sl::Handle<SSL_CTX*, FreeSslCtx> {
public:
	bool
	create(const SSL_METHOD* method = SSLv23_method());

	long
	getOptions() {
		ASSERT(m_h);
		return ::SSL_CTX_get_options(m_h);
	}

	long
	setOptions(long options) {
		ASSERT(m_h);
		return ::SSL_CTX_set_options(m_h, options);
	}

	long
	clearOptions(long options) {
		ASSERT(m_h);
		return ::SSL_CTX_clear_options(m_h, options);
	}

	void
	setInfoCallback(SslInfoCallbackFunc* callbackFunc) {
		ASSERT(m_h);
		::SSL_CTX_set_info_callback(m_h, callbackFunc);
	}

	int
	getVerifyDepth() {
		ASSERT(m_h);
		return ::SSL_CTX_get_verify_depth(m_h);
	}

	void
	setVerifyDepth(int depth) {
		ASSERT(m_h);
		::SSL_CTX_set_verify_depth(m_h, depth);
	}

	int
	getVerifyMode() {
		ASSERT(m_h);
		return ::SSL_CTX_get_verify_mode(m_h);
	}

	void
	setVerifyMode(int mode) {
		ASSERT(m_h);
		::SSL_CTX_set_verify(m_h, mode, NULL);
	}

	void
	setVerify(
		int mode,
		SslVerifyCallbackFunc* callbackFunc
	) {
		ASSERT(m_h);
		::SSL_CTX_set_verify(m_h, mode, callbackFunc);
	}

	bool
	loadVerifyLocations(
		const sl::StringRef& caFileName,
		const sl::StringRef& caDir = sl::StringRef()
	);

	bool
	setTmpDh(DH* dh);

	void
	setTmpDhCallback(SslTmpDhCallbackFunc* callback) {
		ASSERT(m_h);
		::SSL_CTX_set_tmp_dh_callback(m_h, callback);
	}

	bool
	setTmpEcdh(EC_KEY* ecKey);

	bool
	setCipherList(const sl::StringRef& listString);

	X509*
	getCertificate() const {
		ASSERT(m_h);
		return ::SSL_CTX_get0_certificate(m_h);
	}

	bool
	useCertificate(const X509* cert);

	bool
	useCertificateFile(
		const sl::StringRef& fileName,
		int fileType = SSL_FILETYPE_PEM
	);

	EVP_PKEY*
	getPrivateKey() const {
		ASSERT(m_h);
		return ::SSL_CTX_get0_privatekey(m_h);
	}

	bool
	usePrivateKey(const EVP_PKEY* key);

	bool
	usePrivateKeyFile(
		const sl::StringRef& fileName,
		int fileType = SSL_FILETYPE_PEM
	);

	bool
	addExtraChainCertificate(const X509* cert);

	void
	clearExtraChainCertificates() {
		ASSERT(m_h);
		::SSL_CTX_clear_chain_certs(m_h);
	}
};

//..............................................................................

} // namespace io
} // namespace axl
