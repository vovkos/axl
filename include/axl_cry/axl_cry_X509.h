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

class X509Cert: public sl::Handle<X509*, FreeX509>
{
public:
	X509Cert()
	{
	}

	X509Cert(X509* cert)
	{
		attach(cert);
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
	savePem() const
	{
		sl::String string;
		savePem(&string);
		return string;
	}
};

//..............................................................................

class FreeX509Store
{
public:
	void
	operator () (X509_STORE* store)
	{
		::X509_STORE_free(store);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class X509Store: public sl::Handle<X509_STORE*, FreeX509Store>
{
public:
	X509Store()
	{
	}

	X509Store(X509_STORE* store)
	{
		attach(store);
	}

	bool
	create();

	bool
	addCert(X509* cert);
};

//..............................................................................

} // namespace cry
} // namespace axl
