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

#define _AXL_CRY_X509_H

#include "axl_cry_Pch.h"

namespace axl {
namespace cry {

//..............................................................................

class FreeX509
{
public:
	void
	operator () (X509* cert)
	{
		::X509_free(cert);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class AutoX509: public sl::Handle<X509*, FreeX509>
{
};

//..............................................................................

// DER-encoding

size_t
saveX509_der(
	sl::Array<char>* buffer,
	const X509* cert
	);

inline
sl::Array<char>
saveX509_der(const X509* cert)
{
	sl::Array<char> buffer;
	saveX509_der(&buffer, cert);
	return buffer;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
loadX509_der(
	X509* cert,
	const void* p,
	size_t size
	);

X509*
loadX509_der(
	const void* p,
	size_t size
	);

inline
bool
loadX509_der(
	X509* cert,
	const sl::ArrayRef<char>& buffer
	)
{
	return loadX509_der(cert, buffer.cp(), buffer.getCount());
}

inline
X509*
loadX509_der(const sl::ArrayRef<char>& buffer)
{
	return loadX509_der(buffer.cp(), buffer.getCount());
}

//..............................................................................

// PEM-encoding

size_t
saveX509_pem(
	sl::String* string,
	const X509* cert
	);

inline
sl::String
saveX509_pem(const X509* cert)
{
	sl::String string;
	saveX509_pem(&string, cert);
	return string;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
loadX509_pem(
	X509* cert,
	const void* p,
	size_t size
	);

X509*
loadX509_pem(
	const void* p,
	size_t size
	);

inline
bool
loadX509_pem(
	X509* cert,
	const sl::ArrayRef<char>& buffer
	)
{
	return loadX509_pem(cert, buffer.cp(), buffer.getCount());
}

inline
X509*
loadX509_pem(const sl::ArrayRef<char>& buffer)
{
	return loadX509_pem(buffer.cp(), buffer.getCount());
}

inline
bool
loadX509_pem(
	X509* cert,
	const sl::StringRef& string
	)
{
	return loadX509_pem(cert, string.cp(), string.getLength());
}

inline
X509*
loadX509_pem(const sl::StringRef& string)
{
	return loadX509_pem(string.cp(), string.getLength());
}

//..............................................................................

} // namespace cry
} // namespace axl
