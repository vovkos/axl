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
#include "axl_io_SslError.h"

namespace axl {
namespace io {

//..............................................................................

bool
SslCtx::create(const SSL_METHOD* method)
{
	close();
	m_h = SSL_CTX_new(method);
	return completeSslOp(m_h != NULL);
}

bool
SslCtx::loadVerifyLocations(
	const sl::StringRef& caFileName,
	const sl::StringRef& caDir
	)
{
	ASSERT(m_h);

	bool_t result = ::SSL_CTX_load_verify_locations(
		m_h,
		!caFileName.isEmpty() ? caFileName.sz() : NULL,
		!caDir.isEmpty() ? caDir.sz() : NULL
		);

	return completeSslOp(result);
}

bool
SslCtx::setCipherList(const sl::StringRef& listString)
{
	ASSERT(m_h);
	bool_t result = ::SSL_CTX_set_cipher_list(m_h, listString.sz());
	return completeSslOp(result);
}

//..............................................................................

} // namespace io
} // namespace axl
