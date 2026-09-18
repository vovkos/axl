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
#endif

// X509Store is portable; only the bridge to the Windows certificate store is
// not. Keeping it in its own translation unit so that only projects who
// actually use it have to link crypt32

namespace axl {
namespace cry {

#if (_AXL_OS_WIN)

//..............................................................................

size_t
X509Store::addWinCertStore(const sys::win::CertStore& store) {
	ASSERT(m_h);

	size_t count = 0;
	sys::win::Certificate cert;
	for (size_t i = 0;; i++) {
		bool result = store.getNextCertificate(&cert);
		if (!result)
			return -1;

		if (!cert)
			break;

		sl::ArrayRef<char> der = cert.getDer();
		const uchar_t* p = (const uchar_t*)der.cp();

		X509Cert x509;
		::d2i_X509(x509.p(), &p, (long)der.getCount());
		if (!x509)
			return -1;

		result = addCert(x509);
		if (!result)
			return -1;

		count++;
	}

	return count;
}

#endif // _AXL_OS_WIN

//..............................................................................

} // namespace cry
} // namespace axl
