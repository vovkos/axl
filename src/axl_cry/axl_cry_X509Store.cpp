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
#include "axl_cry_X509Store.h"

#if (_AXL_OS_WIN)
#	include "axl_cry_X509Cert.h" // to decode each DER blob
#	include "axl_sys_win_Certificate.h"
#elif (_AXL_OS_DARWIN)
#	include "axl_cry_X509Cert.h"
#	include "axl_sec_Certificate.h"
#endif

// X509Store is portable; only the bridges to the Windows/Darwin certificate
// stores are not. Keeping them in their own translation unit so that only
// projects who actually use them have to link crypt32 or Security

namespace axl {
namespace cry {

#if (_AXL_OS_WIN)

//..............................................................................

size_t
X509Store::addWinCertStore(HCERTSTORE store) {
	ASSERT(m_h);

	sys::win::CertStore certStore(store);
	sys::win::Certificate cert;
	size_t count = 0;

	for (;;) {
		bool result = certStore.getNextCertificate(&cert);
		if (!result) {
			count = -1;
			break;
		}

		if (!cert)
			break;

		sl::ArrayRef<char> der = cert.getDer();
		const uchar_t* p = (const uchar_t*)der.cp();

		X509Cert x509;
		::d2i_X509(x509.p(), &p, (long)der.getCount());
		if (!x509) { // skip certificates OpenSSL can't parse
			clearCryptoError();
			continue;
		}

		result = addCert(x509);
		if (!result) {
			count = -1;
			break;
		}

		count++;
	}

	certStore.detach(); // owned by the caller
	return count;
}

#elif (_AXL_OS_DARWIN)

//..............................................................................

size_t
X509Store::addDarwinCertArray(CFArrayRef certificateArray) {
	ASSERT(m_h);

	cf::Array array(certificateArray);
	size_t count = 0;
	size_t arrayCount = array.getCount();
	for (size_t i = 0; i < arrayCount; i++) {
		sec::Certificate cert((SecCertificateRef)array.getValue(i));
		cf::Data der = cert.copyDer();
		const uchar_t* p = der.getBytePtr();

		X509Cert x509;
		::d2i_X509(x509.p(), &p, (long)der.getLength());
		if (!x509) { // skip certificates OpenSSL can't parse
			clearCryptoError();
			continue;
		}

		bool result = addCert(x509);
		if (!result)
			return -1;

		count++;
	}

	return count;
}

#endif

//..............................................................................

} // namespace cry
} // namespace axl
