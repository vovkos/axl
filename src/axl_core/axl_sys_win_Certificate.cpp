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
#include "axl_sys_win_Certificate.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

size_t
Certificate::getNameString(
	sl::String_w* string,
	dword_t type,
	dword_t flags,
	void* typeParam
) {
	ASSERT(m_h);

	dword_t size = ::CertGetNameStringW(
		m_h,
		type,
		flags,
		typeParam,
		NULL,
		0
	);

	utf16_t* p = string->createBuffer(size);

	::CertGetNameStringW(
		m_h,
		type,
		flags,
		typeParam,
		p,
		size
	);

	return string->updateLength();
}

//..............................................................................

bool
CertStore::findCertificate(
	Certificate* certificate,
	dword_t encodingType,
	dword_t findFlags,
	dword_t findType,
	const void* findParam,
	const CERT_CONTEXT* prevCertificate
) {
	ASSERT(m_h);

	certificate->close();

	const CERT_CONTEXT* result = ::CertFindCertificateInStore(
		m_h,
		encodingType,
		findFlags,
		findType,
		findParam,
		prevCertificate
	);

	if (!result)
		return err::failWithLastSystemError();

	certificate->attach(result);
	return true;
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
