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
#include "axl_io_SslBio.h"
#include "axl_io_SslError.h"

namespace axl {
namespace io {

//..............................................................................

bool
SslBio::create(BIO_METHOD* method)
{
	close();
	m_h = BIO_new(method);
	return completeSslOp(m_h != NULL);
}

bool
SslBio::createSocket(
	socket_t socket,
	bool isAutoClose
	)
{
	close();
	m_h = BIO_new_socket((int)socket, isAutoClose);
	return completeSslOp(m_h != NULL);
}

bool
SslBio::createFp(
	FILE* file,
	bool isAutoClose
	)
{
	close();
	m_h = BIO_new_fp(file, isAutoClose);
	return completeSslOp(m_h != NULL);
}

bool
SslBio::createFd(
	int fd,
	bool isAutoClose
	)
{
	close();
	m_h = BIO_new_fd(fd, isAutoClose);
	return completeSslOp(m_h != NULL);
}

//..............................................................................

} // namespace io
} // namespace axl
