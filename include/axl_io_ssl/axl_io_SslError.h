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

namespace axl {
namespace io {

//..............................................................................

// {97F19105-D4D8-4C40-A0BA-D17EA9ABCBEF}
AXL_SL_DEFINE_GUID(
	g_sslErrorGuid,
	0x97f19105, 0xd4d8, 0x4c40, 0xa0, 0xba, 0xd1, 0x7e, 0xa9, 0xab, 0xcb, 0xef
	);

// {A5C41EB8-75FF-487D-B04E-BD2336388B38}
AXL_SL_DEFINE_GUID(
	g_sslX509ErrorGuid,
	0xa5c41eb8, 0x75ff, 0x487d, 0xb0, 0x4e, 0xbd, 0x23, 0x36, 0x38, 0x8b, 0x38
	);

//..............................................................................

class SslErrorProvider: public err::ErrorProvider
{
public:
	static
	sl::StringRef
	getErrorDescription(ulong_t code);

	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error)
	{
		return getErrorDescription(error->m_code);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class SslX509ErrorProvider: public err::ErrorProvider
{
public:
	static
	sl::StringRef
	getErrorDescription(ulong_t code)
	{
		return ::X509_verify_cert_error_string(code);
	}

	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error)
	{
		return getErrorDescription(error->m_code);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
registerSslErrorProviders();

//..............................................................................

class SslError: public err::Error
{
public:
	SslError()
	{
	}

	SslError(long code)
	{
		create(code);
	}

	size_t
	create(long code)
	{
		return createSimpleError(g_sslErrorGuid, (uint_t)code);
	}
};

//..............................................................................

class SslX509Error: public err::Error
{
public:
	SslX509Error()
	{
	}

	SslX509Error(long code)
	{
		create(code);
	}

	size_t
	create(long code)
	{
		return createSimpleError(g_sslX509ErrorGuid, (uint_t)code);
	}
};

//..............................................................................

inline
ulong_t
getLastSslErrorCode()
{
	return ::ERR_peek_last_error();
}

inline
void
setLastSslError()
{
	err::setError(SslError(getLastSslErrorCode()));
}

inline
void
clearSslErrorQueue()
{
	while (::ERR_get_error());
}

template <typename T>
T
completeSslOp(
	T result,
	T failResult
	)
{
	if (result == failResult)
		setLastSslError();

	return result;
}

inline
bool
completeSslOp(int result)
{
	return completeSslOp<bool>(result != 0, false);
}

//..............................................................................

} // namespace io
} // namespace axl
