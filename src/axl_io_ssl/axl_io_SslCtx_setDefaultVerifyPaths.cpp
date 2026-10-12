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
#include "axl_io_SslCtx.h"
#include "axl_cry_CryptoError.h"
#include "axl_cry_X509Store.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_Certificate.h"
#elif (_AXL_OS_DARWIN)
#	include "axl_sec_Certificate.h"
#else
#	include "axl_io_FilePathUtils.h"
#endif

// SslCtx::setDefaultVerifyPaths lives in its own translation unit so that only
// projects who actually use it have to link crypt32 or Security/CoreFoundation

namespace axl {
namespace io {

//..............................................................................

bool
SslCtx::setDefaultVerifyPaths() {
	ASSERT(m_h);

#if (_AXL_OS_WIN)
	// on windows, OpenSSL's compiled-in OPENSSLDIR rarely exists on the target system;
	// use the windows root store instead

	sys::win::CertStore winStore;
	bool result = winStore.openSystemStore(CERT_SYSTEM_STORE_CURRENT_USER, L"ROOT");
	if (!result)
		return false;

	cry::X509Store store(getCertStore());
	result = store.addWinCertStore(winStore) != -1;
	store.detach(); // owned by the SSL_CTX
	return result;
#elif (_AXL_OS_DARWIN)
	// on darwin, the system trust store is the keychain; PEM bundles are partial copies

	cf::Array anchorArray;
	bool result = sec::copyAnchorCertificates(&anchorArray);
	if (!result)
		return false;

	cry::X509Store store(getCertStore());
	result = store.addDarwinCertArray(anchorArray) != -1;
	store.detach(); // owned by the SSL_CTX
	return result;
#else
	int result = ::SSL_CTX_set_default_verify_paths(m_h);
	if (!result)
		return cry::failWithLastCryptoError();

	// the default CA file (if any) is loaded eagerly; the default CA dir -- lazily

	cry::X509Store store(getCertStore());
	size_t count = store.getObjectCount();
	store.detach(); // owned by the SSL_CTX
	if (count)
		return true;

	const char* caDir = ::getenv(::X509_get_default_cert_dir_env());
	if (doesFileExist(caDir ? caDir : ::X509_get_default_cert_dir()))
		return true;

	// OpenSSL built with a non-system OPENSSLDIR (e.g., bundled); probe distro CA bundles

	static const char* const caFileNameTable[] = {
		"/etc/ssl/certs/ca-certificates.crt",                // Debian, Ubuntu, Arch, Gentoo
		"/etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem", // RHEL 7+, Fedora
		"/etc/pki/tls/certs/ca-bundle.crt",                  // older RHEL, Fedora
		"/etc/ssl/ca-bundle.pem",                            // openSUSE
		"/etc/ssl/cert.pem",                                 // Alpine
	};

	for (size_t i = 0; i < countof(caFileNameTable); i++)
		if (doesFileExist(caFileNameTable[i]))
			return loadVerifyLocations(caFileNameTable[i]);

	return loadVerifyLocations(sl::StringRef(), "/etc/ssl/certs");
#endif
}

//..............................................................................

} // namespace io
} // namespace axl
