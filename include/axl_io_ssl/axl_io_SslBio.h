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

#define _AXL_IO_SSLBIO_H

#include "axl_io_SslCtx.h"

namespace axl {
namespace io {

//..............................................................................

class FreeSslBio
{
public:
	void
	operator () (BIO* h)
	{
		BIO_vfree(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class SslBio: public sl::Handle<BIO*, FreeSslBio>
{
public:
	bool
	create(BIO_METHOD* method);

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
	freeAll()
	{
		ASSERT(m_h);
		::BIO_free_all(detach());
	}
};

//..............................................................................

} // namespace io
} // namespace axl
