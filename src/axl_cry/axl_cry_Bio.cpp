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
#include "axl_cry_Bio.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//..............................................................................

#if (OPENSSL_VERSION_NUMBER >= 0x10100000)
bool
Bio::create(const BIO_METHOD* method)
#else
bool
Bio::create(BIO_METHOD* method)
#endif
{
	close();
	m_h = BIO_new(method);
	return completeWithLastCryptoError(m_h != NULL);
}

bool
Bio::createSocket(
	socket_t socket,
	bool isAutoClose
	)
{
	close();
	m_h = BIO_new_socket((int)socket, isAutoClose);
	return completeWithLastCryptoError(m_h != NULL);
}

bool
Bio::createFp(
	FILE* file,
	bool isAutoClose
	)
{
	close();
	m_h = BIO_new_fp(file, isAutoClose);
	return completeWithLastCryptoError(m_h != NULL);
}

bool
Bio::createFd(
	int fd,
	bool isAutoClose
	)
{
	close();
	m_h = BIO_new_fd(fd, isAutoClose);
	return completeWithLastCryptoError(m_h != NULL);
}

bool
Bio::createMemBuf(
	const void* p,
	size_t size
	)
{
	close();
	m_h = BIO_new_mem_buf((void*)p, size);
	return completeWithLastCryptoError(m_h != NULL);
}

BUF_MEM*
Bio::getBufMem()
{
	BUF_MEM* mem = NULL;
	BIO_get_mem_ptr(m_h, &mem);
	return mem;
}

//..............................................................................

} // namespace cry
} // namespace axl
