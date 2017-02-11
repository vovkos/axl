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
#include "test.h"

namespace {

//..............................................................................

void run ()
{
	cry::EcKey key (NID_secp112r1);
	key.generateKey ();

	cry::BigNum privateKey = key.getPrivateKey ();
	cry::EcPoint publicKey = key.getPublicKey ();
	printf ("private: %s\n", privateKey.getHexString ().sz ());
	printf ("public:  %s\n", publicKey.getHexString (key.getGroup ()).sz ());

	const char* userName = "Vovkos";

	sl::String productKey = generateEcProductKey (key, userName);
	printf ("user:    %s\n", userName);
	printf ("license: %s\n", productKey.sz ());

	bool result = verifyEcProductKey (key, userName, productKey);
	printf ("verify:  %d\n", result);
	ASSERT (result);

	privateKey.detach ();
	publicKey.detach ();

}

//..............................................................................

ADD_TEST_CASE ("test_Crypto", run)

}
