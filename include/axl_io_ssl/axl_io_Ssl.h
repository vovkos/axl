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

#define _AXL_IO_SSL_H

#include "axl_io_SslCtx.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace io {

//..............................................................................

class FreeSsl {
public:
	void
	operator () (SSL* h) {
		SSL_free(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Ssl: public sl::Handle<SSL*, FreeSsl> {
public:
	bool
	create(SSL_CTX* ctx);

	int
	getError(int retCode) {
		int error = ::SSL_get_error(m_h, retCode);
		return error == SSL_ERROR_SSL ? ::ERR_peek_last_error() : error;
	}

	void
	setError(int retCode) {
		int error = getError(retCode);
		err::setError(cry::CryptoError(error));
	}

	void
	setBio(BIO* bio) {
		ASSERT(m_h);
		::SSL_set_bio(m_h, bio, bio);
	}

	void
	setBio(
		BIO* rbio,
		BIO* wbio
	) {
		ASSERT(m_h);
		::SSL_set_bio(m_h, rbio, wbio);
	}

	long
	getOptions() {
		ASSERT(m_h);
		return ::SSL_get_options(m_h);
	}

	long
	setOptions(long options) {
		ASSERT(m_h);
		return ::SSL_set_options(m_h, options);
	}

	long
	clearOptions(long options) {
		ASSERT(m_h);
		return ::SSL_clear_options(m_h, options);
	}

#if (OPENSSL_VERSION_NUMBER >= 0x10100000L) // openssl-1.1.0
	int
	getSecurityLevel() {
		ASSERT(m_h);
		return ::SSL_get_security_level(m_h);
	}

	void
	setSecurityLevel(int level) {
		ASSERT(m_h);
		::SSL_set_security_level(m_h, level);
	}
#endif

	const char*
	getHostname() {
		ASSERT(m_h);
		return ::SSL_get_servername(m_h, TLSEXT_NAMETYPE_host_name);
	}

	void
	setHostname(const sl::StringRef& hostname) {
		ASSERT(m_h);
		::SSL_set_tlsext_host_name(m_h, hostname.sz());
	}

	void
	setInfoCallback(SslInfoCallbackFunc* callbackFunc) {
		ASSERT(m_h);
		::SSL_set_info_callback(m_h, callbackFunc);
	}

	long
	getVerifyResult() {
		ASSERT(m_h);
		return ::SSL_get_verify_result(m_h);
	}

	int
	getVerifyDepth() {
		ASSERT(m_h);
		return ::SSL_get_verify_depth(m_h);
	}

	void
	setVerifyDepth(int depth) {
		ASSERT(m_h);
		::SSL_set_verify_depth(m_h, depth);
	}

	int
	getVerifyMode() {
		ASSERT(m_h);
		return ::SSL_get_verify_mode(m_h);
	}

	void
	setVerifyMode(int mode) {
		ASSERT(m_h);
		::SSL_set_verify(m_h, mode, NULL);
	}

	void
	setVerify(
		int mode,
		SslVerifyCallbackFunc* callbackFunc
	) {
		ASSERT(m_h);
		::SSL_set_verify(m_h, mode, callbackFunc);
	}

	bool
	setTmpDh(DH* dh);

	void
	setTmpDhCallback(SslTmpDhCallbackFunc* callback) {
		ASSERT(m_h);
		::SSL_set_tmp_dh_callback(m_h, callback);
	}

	bool
	setTmpEcdh(EC_KEY* ec);

	STACK_OF(SSL_CIPHER)*
	getCiphers() {
		ASSERT(m_h);
		return ::SSL_get_ciphers(m_h);
	}

	const char*
	getCipherName(int priority) {
		ASSERT(m_h);
		return ::SSL_get_cipher_list(m_h, priority);
	}

	bool
	setCipherList(const sl::StringRef& listString);

	void
	setConnectState() {
		ASSERT(m_h);
		::SSL_set_connect_state(m_h);
	}

	void
	setAcceptState() {
		ASSERT(m_h);
		::SSL_set_accept_state(m_h);
	}

	bool
	getReadAhead() {
		ASSERT(m_h);
		return ::SSL_get_read_ahead(m_h) != 0;
	}

	void
	setReadAhead(bool isReadAhead) {
		ASSERT(m_h);
		::SSL_set_read_ahead(m_h, isReadAhead);
	}

	int
	getState() {
		ASSERT(m_h);
		return ::SSL_get_state(m_h);
	}

	const char*
	getStateString() {
		ASSERT(m_h);
		return ::SSL_state_string(m_h);
	}

	const char*
	getStateStringLong() {
		ASSERT(m_h);
		return ::SSL_state_string_long(m_h);
	}

	const char*
	getCurrentCipherName() {
		ASSERT(m_h);
		return ::SSL_CIPHER_get_name(SSL_get_current_cipher(m_h));
	}

	sl::String
	getCurrentCipherDescription();

	int
	getCurrentCipherBits();

	void*
	getExtraData(int index) {
		ASSERT(m_h);
		return ::SSL_get_ex_data(m_h, index);
	}

	bool
	setExtraData(
		int index,
		void* p
	);

	X509*
	getCertificate() const {
		ASSERT(m_h);
		return ::SSL_get_certificate(m_h);
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
		return ::SSL_get_privatekey(m_h);
	}

	bool
	usePrivateKey(const EVP_PKEY* key);

	bool
	usePrivateKeyFile(
		const sl::StringRef& fileName,
		int fileType = SSL_FILETYPE_PEM
	);

	X509*
	getPeerCertificate() const {
		ASSERT(m_h);
		return ::SSL_get_peer_certificate(m_h);
	}

	STACK_OF(X509)*
	getPeerCertificateChain() const {
		ASSERT(m_h);
		return ::SSL_get_peer_cert_chain(m_h);
	}

	bool
	doHandshake();

	bool
	shutdown();

	size_t
	read(
		void* p,
		size_t size
	);

	size_t
	write(
		const void* p,
		size_t size
	);

	size_t
	getPendingReadSize() {
		ASSERT(m_h);
		return ::SSL_pending(m_h);
	}

protected:
	bool
	complete(int result);
};

//..............................................................................

} // namespace io
} // namespace axl
