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

#define _AXL_IO_SSLERROR_H

#include "axl_io_SslPch.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace io {

//..............................................................................

inline
void
registerSslErrorProvider() {
	cry::registerCryptoErrorProviders();
	::ERR_load_SSL_strings();
}

//..............................................................................

} // namespace io
} // namespace axl
