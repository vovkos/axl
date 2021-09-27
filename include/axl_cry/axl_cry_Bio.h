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

#define _AXL_CRY_BIO_H

#include "axl_cry_Pch.h"

namespace axl {
namespace cry {

#if (_AXL_OS_WIN)
typedef SOCKET socket_t;
#else
typedef int socket_t;
#endif

//..............................................................................

class FreeBio {
public:
	void
	operator () (BIO* h) {
		::BIO_free(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Bio: public sl::Handle<BIO*, FreeBio> {
public:
	Bio() {}

#if (OPENSSL_VERSION_NUMBER >= 0x10100000)
	bool
	create(const BIO_METHOD* method);
#else
	bool
	create(BIO_METHOD* method);
#endif

	bool
	createMem() {
		return create(BIO_s_mem());
	}

	bool
	createMemBuf(
		const void* p,
		size_t size = -1
	);

	bool
	createSocket(
		socket_t socket,
		bool isAutoClose = false
	);

	bool
	createFp(
		FILE* file,
		bool isAutoClose = false
	);

	bool
	createFd(
		int fd,
		bool isAutoClose = false
	);

	void
	freeAll() {
		ASSERT(m_h);
		::BIO_free_all(detach());
	}

	BUF_MEM*
	getBufMem();
};

//..............................................................................

} // namespace cry
} // namespace axl
