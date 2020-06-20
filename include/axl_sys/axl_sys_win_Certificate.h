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

#define _AXL_SYS_WIN_CERTSTORE_H

#include "axl_sl_Handle.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class FreeCertContext
{
public:
	void
	operator () (const CERT_CONTEXT* p)
	{
		::CertFreeCertificateContext(p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Certificate: public sl::Handle<const CERT_CONTEXT*, FreeCertContext>
{
public:
	Certificate()
	{
	}

	Certificate(const CERT_CONTEXT* p)
	{
		attach(p);
	}

	size_t
	getNameString(
		sl::String_w* string,
		dword_t type,
		dword_t flags = 0,
		void* typeParam = NULL
		);

	sl::String_w
	getNameString(
		dword_t type,
		dword_t flags = 0,
		void* typeParam = NULL
		)
	{
		sl::String_w string;
		getNameString(&string, type, flags, typeParam);
		return string;
	}

	size_t
	getSubjectName(sl::String_w* string)
	{
		return getNameString(string, CERT_NAME_SIMPLE_DISPLAY_TYPE);
	}

	sl::String_w
	getSubjectName()
	{
		return getNameString(CERT_NAME_SIMPLE_DISPLAY_TYPE);
	}

	size_t
	getIssuerName(sl::String_w* string)
	{
		return getNameString(string, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG);
	}

	sl::String
	getIssuerName()
	{
		return getNameString(CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG);
	}
};

//..............................................................................

class CloseCertStore
{
public:
	void
	operator () (HCERTSTORE h)
	{
		::CertCloseStore(h, 0);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CertStore: public sl::Handle<HCERTSTORE, CloseCertStore>
{
public:
	CertStore()
	{
	}

	CertStore(HCERTSTORE h)
	{
		attach(h);
	}

	bool
	findCertificate(
		Certificate* certificate,
		dword_t encodingType,
		dword_t findFlags,
		dword_t findType,
		const void* findParam,
		const CERT_CONTEXT* prevCertificate = NULL
		);
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
