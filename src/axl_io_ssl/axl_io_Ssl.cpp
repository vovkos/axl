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
#include "axl_io_SslError.h"

namespace axl {
namespace io {

//..............................................................................

bool
Ssl::create(SSL_CTX* ctx)
{
	close();

	m_h = SSL_new(ctx);
	return completeSslOp(m_h != NULL);
}

bool
Ssl::setCipherList(const sl::StringRef& listString)
{
	ASSERT(m_h);
	bool_t result = ::SSL_set_cipher_list(m_h, listString.sz());
	return completeSslOp(result);
}

bool
Ssl::doHandshake()
{
	ASSERT(m_h);
	int result = ::SSL_do_handshake(m_h);
	return completeSslOp(result);
}

bool
Ssl::shutdown()
{
	ASSERT(m_h);
	int result = ::SSL_shutdown(m_h);
	return completeSslOp(result);
}

size_t
Ssl::write(
	const void* p,
	size_t size
	)
{
	ASSERT(m_h);
	int result = ::SSL_write(m_h, p, (int)size);
	return completeSslOp<size_t>(result, -1);
}

size_t
Ssl::read(
	void* p,
	size_t size
	)
{
	ASSERT(m_h);
	int result = ::SSL_read(m_h, p, (int)size);
	return completeSslOp<size_t>(result, -1);
}

//..............................................................................

} // namespace io
} // namespace axl
