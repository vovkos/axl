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

#define _AXL_CRY_Pkcs7_H

#include "axl_cry_Pch.h"

namespace axl {
namespace cry {

//..............................................................................

class FreePkcs7
{
public:
	void
	operator () (PKCS7* msg)
	{
		::PKCS7_free(msg);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef sl::Handle<PKCS7*, FreePkcs7> Pkcs7Handle;

//..............................................................................

// DER-encoding

size_t
savePkcs7_der(
	sl::Array<char>* buffer,
	const PKCS7* msg
	);

inline
sl::Array<char>
savePkcs7_der(const PKCS7* msg)
{
	sl::Array<char> buffer;
	savePkcs7_der(&buffer, msg);
	return buffer;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

PKCS7*
loadPkcs7_der(
	const void* p,
	size_t size
	);

inline
PKCS7*
loadPkcs7_der(const sl::ArrayRef<char>& buffer)
{
	return loadPkcs7_der(buffer.cp(), buffer.getCount());
}

//..............................................................................

// PEM-encoding

size_t
savePkcs7_pem(
	sl::String* string,
	const PKCS7* msg
	);

inline
sl::String
savePkcs7_pem(const PKCS7* msg)
{
	sl::String string;
	savePkcs7_pem(&string, msg);
	return string;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

PKCS7*
loadPkcs7_pem(
	const void* p,
	size_t size
	);

inline
PKCS7*
loadPkcs7_pem(const sl::ArrayRef<char>& buffer)
{
	return loadPkcs7_pem(buffer.cp(), buffer.getCount());
}

inline
PKCS7*
loadPkcs7_pem(const sl::StringRef& string)
{
	return loadPkcs7_pem(string.cp(), string.getLength());
}

//..............................................................................

} // namespace cry
} // namespace axl
