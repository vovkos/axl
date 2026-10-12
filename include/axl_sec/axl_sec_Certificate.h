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

#define _AXL_SEC_CERTIFICATE_H

#include "axl_sec_Error.h"
#include "axl_cf_Data.h"
#include "axl_cf_Array.h"

namespace axl {
namespace sec {

//..............................................................................

class Certificate: public cf::TypeBase<SecCertificateRef> {
public:
	Certificate() {}

	Certificate(const Certificate& src):
		cf::TypeBase<SecCertificateRef>(src) {}

	Certificate(Certificate&& src):
		cf::TypeBase<SecCertificateRef>(std::move(src)) {}

	Certificate(
		SecCertificateRef p,
		bool isAttach = false
	):
		cf::TypeBase<SecCertificateRef>(p, isAttach) {}

	Certificate&
	operator = (const Certificate& src) {
		copy(src);
		return *this;
	}

	Certificate&
	operator = (Certificate&& src) {
		move(std::move(src));
		return *this;
	}

	Certificate&
	operator = (SecCertificateRef p) {
		copy(p);
		return *this;
	}

	cf::Data
	copyDer() const {
		ASSERT(m_p);
		return cf::Data(::SecCertificateCopyData(m_p), true);
	}
};

//..............................................................................

// the system root certificates (an array of SecCertificateRef)

inline
bool
copyAnchorCertificates(cf::Array* certificateArray) {
	certificateArray->clear();
	OSStatus status = ::SecTrustCopyAnchorCertificates(certificateArray->p());
	return completeWithOsStatus(status);
}

//..............................................................................

} // namespace sec
} // namespace axl
