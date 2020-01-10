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

class FreeSsl
{
public:
	void
	operator () (SSL* h)
	{
		SSL_free(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Ssl: public sl::Handle<SSL*, FreeSsl>
{
public:
	bool
	create(SSL_CTX* ctx);

	int
	getError(int retCode)
	{
		return ::SSL_get_error(m_h, retCode);
	}

	void
	setError(int retCode)
	{
		int error = getError(retCode);
		err::setError(cry::CryptoError(error));
	}

	void
	setBio(BIO* bio)
	{
		ASSERT(m_h);
		::SSL_set_bio(m_h, bio, bio);
	}

	void
	setBio(
		BIO* rbio,
		BIO* wbio
		)
	{
		ASSERT(m_h);
		::SSL_set_bio(m_h, rbio, wbio);
	}

	long
	getOptions()
	{
		ASSERT(m_h);
		return ::SSL_get_options(m_h);
	}

	long
	setOptions(long options)
	{
		ASSERT(m_h);
		return ::SSL_set_options(m_h, options);
	}

	long
	clearOptions(long options)
	{
		ASSERT(m_h);
		return ::SSL_clear_options(m_h, options);
	}

	long
	getVerifyResult()
	{
		ASSERT(m_h);
		return ::SSL_get_verify_result(m_h);
	}

	int
	getVerifyDepth()
	{
		ASSERT(m_h);
		return ::SSL_get_verify_depth(m_h);
	}

	void
	setVerifyDepth(int depth)
	{
		ASSERT(m_h);
		::SSL_set_verify_depth(m_h, depth);
	}

	void
	setInfoCallback(SslInfoCallbackFunc* callbackFunc)
	{
		ASSERT(m_h);
		::SSL_set_info_callback(m_h, callbackFunc);
	}

	void
	setVerify(
		int mode,
		SslVerifyCallbackFunc* callbackFunc = NULL
		)
	{
		ASSERT(m_h);
		::SSL_set_verify(m_h, mode, callbackFunc);
	}

	bool
	setCipherList(const sl::StringRef& listString);

	void
	setConnectState()
	{
		ASSERT(m_h);
		::SSL_set_connect_state(m_h);
	}

	void
	setAcceptState()
	{
		ASSERT(m_h);
		::SSL_set_accept_state(m_h);
	}

	bool
	getReadAhead()
	{
		ASSERT(m_h);
		return ::SSL_get_read_ahead(m_h) != 0;
	}

	void
	setReadAhead(bool isReadAhead)
	{
		ASSERT(m_h);
		::SSL_set_read_ahead(m_h, isReadAhead);
	}

	int
	getState()
	{
		ASSERT(m_h);
		return ::SSL_get_state(m_h);
	}

	const char*
	getStateString()
	{
		ASSERT(m_h);
		return ::SSL_state_string(m_h);
	}

	const char*
	getStateStringLong()
	{
		ASSERT(m_h);
		return ::SSL_state_string_long(m_h);
	}

	void*
	getExtraData(int index)
	{
		ASSERT(m_h);
		return ::SSL_get_ex_data(m_h, index);
	}

	bool
	setExtraData(
		int index,
		void* p
		);

	X509*
	getPeerCertificate()
	{
		ASSERT(m_h);
		return ::SSL_get_peer_certificate(m_h);
	}

	STACK_OF(X509)*
	getPeerCertificateChain()
	{
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
	getPendingReadSize()
	{
		ASSERT(m_h);
		return ::SSL_pending(m_h);
	}
};

//..............................................................................

} // namespace io
} // namespace axl
