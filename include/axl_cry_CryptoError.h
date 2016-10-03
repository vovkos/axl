// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_cry_Pch.h"

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
	getErrorDescription (const err::ErrorRef& error);
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

	size_t
	create (int code)
	{
		return createSimpleError (g_cryptoErrorGuid, code);
	}
};

//.............................................................................

inline
size_t
setCryptoError (int code)
{
	return setError (CryptoError (code));
}

inline
size_t
setLastCryptoError ()
{
	size_t result = setError (CryptoError (ERR_peek_last_error ()));
	ERR_clear_error ();
	return result;
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

