// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_CRY_ECPRODUCTKEY_H

#include "axl_cry_BigNum.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//.............................................................................

class ProductKey
{
public:
	bool generateKeys ();

	bool generateProductKey (
		const rtl::String& privateKey, 
		const rtl::String& username
		);

	bool verifyProductKey (
		const rtl::String& username, 
		const rtl::String& productKey, 
		const rtl::String& publicKey
		);

	rtl::String getPrivateKey()
	{
		return m_privateKey;
	}

	rtl::String getPublicKey ()
	{
		return m_publicKey;
	}

	rtl::String getProductKey ()
	{
		return m_productKey;
	}

private:
	rtl::String m_privateKey;
	rtl::String m_publicKey;
	rtl::String m_productKey;
};

//.............................................................................

} // namespace cry
} // namespace axl
