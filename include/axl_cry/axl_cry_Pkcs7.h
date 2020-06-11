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

class Pkcs7: public sl::Handle<PKCS7*, FreePkcs7>
{
public:
	Pkcs7()
	{
	}

	Pkcs7(PKCS7* src)
	{
		attach(src);
	}

	bool
	create();

	bool
	loadDer(
		const void* p,
		size_t size
		);

	size_t
	saveDer(sl::Array<char>* buffer) const;

	inline
	sl::Array<char>
	saveDer() const
	{
		sl::Array<char> buffer;
		saveDer(&buffer);
		return buffer;
	}

	bool
	loadPem(
		const void* p,
		size_t size
		);

	inline
	bool
	loadPem(const sl::StringRef& string)
	{
		return loadPem(string.cp(), string.getLength());
	}

	size_t
	savePem(sl::String* string) const;

	inline
	sl::String
	savePem()
	{
		sl::String string;
		savePem(&string);
		return string;
	}

	bool
	verify(
		STACK_OF(X509)* certStack,
		X509_STORE* store,
		BIO* inData,
		BIO* outData,
		int flags = 0
		);
};

//..............................................................................

} // namespace cry
} // namespace axl
