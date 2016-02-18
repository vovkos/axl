// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_err_ErrorMgr.h"

namespace axl {
namespace cry {

//.............................................................................

// {0216B513-A6BF-4D0E-9185-DD1260C75356}
AXL_SL_DEFINE_GUID (
	g_cryptoErrorGuid, 
	0x0216b513, 0xa6bf, 0x4d0e, 0x91, 0x85, 0xdd, 0x12, 0x60, 0xc7, 0x53, 0x56
	);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CryptoErrorProvider: public err::ErrorProvider
{
public:
	virtual 
	sl::String 
	getErrorDescription (const err::ErrorHdr* error);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline 
void 
registerCryptoErrorProvider ()
{
	err::getErrorMgr ()->registerProvider (
		g_cryptoErrorGuid, 
		sl::getSimpleSingleton <CryptoErrorProvider> ()
		);
}

//.............................................................................

class CryptoError: public err::Error
{
public:
	CryptoError ()
	{
	}

	CryptoError (int code)
	{
		create (code);
	}

	err::ErrorHdr*
	create (int code)
	{
		return createSimpleError (g_cryptoErrorGuid, code);
	}
};

//.............................................................................

inline
err::Error
setCryptoError (int code)
{
	return setError (CryptoError (code));
}

inline
err::Error
setLastCryptoError ()
{
	err::Error error = setError (CryptoError (ERR_peek_last_error ()));
	ERR_clear_error ();
	return error;
}

inline
bool
completeWithCryptoError (
	int result, 
	int errorCode
	)
{
	if (!result)
		setCryptoError (errorCode);

	return result != 0;
}

inline
bool
completeWithLastCryptoError (int result)
{
	if (!result)
		setLastCryptoError ();

	return result != 0;
}

//.............................................................................

} // namespace cry
} // namespace axl

