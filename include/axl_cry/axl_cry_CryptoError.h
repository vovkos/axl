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

#include "axl_cry_Pch.h"

namespace axl {
namespace cry {

//..............................................................................

// {0216B513-A6BF-4D0E-9185-DD1260C75356}
AXL_SL_DEFINE_GUID(
	g_cryptoErrorGuid,
	0x0216b513, 0xa6bf, 0x4d0e, 0x91, 0x85, 0xdd, 0x12, 0x60, 0xc7, 0x53, 0x56
);

// {A5C41EB8-75FF-487D-B04E-BD2336388B38}
AXL_SL_DEFINE_GUID(
	g_x509ErrorGuid,
	0xa5c41eb8, 0x75ff, 0x487d, 0xb0, 0x4e, 0xbd, 0x23, 0x36, 0x38, 0x8b, 0x38
);

//..............................................................................

class CryptoErrorProvider: public err::ErrorProvider {
public:
	static
	sl::StringRef
	getErrorDescription(ulong_t code);

	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error) {
		return getErrorDescription(error->m_code);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CryptoError: public err::Error {
public:
	CryptoError() {}

	CryptoError(ulong_t code) {
		create(code);
	}

	size_t
	create(ulong_t code) {
		return createSimpleError(g_cryptoErrorGuid, code);
	}
};

//..............................................................................

class X509ErrorProvider: public err::ErrorProvider {
public:
	static
	sl::StringRef
	getErrorDescription(long code) {
		return ::X509_verify_cert_error_string(code);
	}

	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error) {
		return getErrorDescription(error->m_code);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class X509Error: public err::Error {
public:
	X509Error() {}

	X509Error(long code) {
		create(code);
	}

	size_t
	create(long code) {
		return createSimpleError(g_x509ErrorGuid, (uint_t)code);
	}
};

//..............................................................................

void
registerCryptoErrorProviders();

inline
size_t
setCryptoError(ulong_t code) {
	return setError(CryptoError(code));
}

inline
void
clearCryptoError() {
	while (ERR_get_error());
}

inline
size_t
setLastCryptoError() {
	return setError(CryptoError(::ERR_peek_last_error()));
}

template <typename T>
T
failWithLastCryptoError(T failResult) {
	setLastCryptoError();
	return failResult;
}

inline
bool
failWithLastCryptoError() {
	return failWithLastCryptoError<bool>(false);
}

template <typename T>
T
completeWithLastCryptoError(
	T result,
	T failResult
) {
	if (result == failResult)
		setLastCryptoError();

	return result;
}

inline
bool
completeWithLastCryptoError(int result) {
	return completeWithLastCryptoError<bool>(result != 0, false);
}

//..............................................................................

} // namespace cry
} // namespace axl
