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

namespace axl {
namespace io {

//..............................................................................

bool
Ssl::create(SSL_CTX* ctx)
{
	close();

	m_h = SSL_new(ctx);
	return cry::completeWithLastCryptoError(m_h != NULL);
}

bool
Ssl::complete(int result)
{
	if (result > 0)
		return true;

	setError(result);
	return false;
}

bool
Ssl::setCipherList(const sl::StringRef& listString)
{
	ASSERT(m_h);
	int result = ::SSL_set_cipher_list(m_h, listString.sz());
	return complete(result);
}

sl::String
Ssl::getCurrentCipherDescription()
{
	ASSERT(m_h);
	const SSL_CIPHER* cipher = ::SSL_get_current_cipher(m_h);
	if (!cipher)
		return sl::String();

	char buffer[256];
	::SSL_CIPHER_description(cipher, buffer, sizeof(buffer));
	return buffer;
}

int
Ssl::getCurrentCipherBits()
{
	ASSERT(m_h);
	int bits = 0;
	::SSL_CIPHER_get_bits(SSL_get_current_cipher(m_h), &bits);
	return bits;
}

bool
Ssl::setExtraData(
	int index,
	void* p
	)
{
	ASSERT(m_h);
	int result = ::SSL_set_ex_data(m_h, index, p);
	return complete(result);
}

bool
Ssl::useCertificate(const X509* cert)
{
	ASSERT(m_h);
	int result = ::SSL_use_certificate(m_h, (X509*)cert);
	return complete(result);
}

bool
Ssl::doHandshake()
{
	ASSERT(m_h);

	int result = ::SSL_do_handshake(m_h);
	if (result <= 0)
	{
		setError(result);
		return false;
	}

	return true;
}

bool
Ssl::shutdown()
{
	ASSERT(m_h);

	int result = ::SSL_shutdown(m_h);
	if (result < 0) // 0 here means shutdown in progress
	{
		setError(result);
		return false;
	}

	return true;
}

size_t
Ssl::write(
	const void* p,
	size_t size
	)
{
	ASSERT(m_h);

	int result = ::SSL_write(m_h, p, (int)size);
	if (result < 0)
	{
		setError(result);
		return -1;
	}

	return result;
}

size_t
Ssl::read(
	void* p,
	size_t size
	)
{
	ASSERT(m_h);

	int result = ::SSL_read(m_h, p, (int)size);
	if (result < 0)
	{
		setError(result);
		return -1;
	}

	return result;
}

//..............................................................................

} // namespace io
} // namespace axl
