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
#include "axl_cry_EcKey.h"
#include "axl_cry_EcPoint.h"
#include "test.h"

namespace {

//..............................................................................

bool
generateEcProductKey(
	EC_KEY* ecKey0,
	sl::String* productKey,
	const sl::StringRef& userName,
	size_t hyphenDistance = 6
) {
	char buffer[256];
	sl::Array<char> signature(rc::BufKind_Stack, buffer, sizeof(buffer));

	cry::EcKey ecKey(ecKey0);

	bool result =
		ecKey.sign(&signature, userName.cp(), userName.getLength()) &&
		enc::Base32Encoding_nj::encode(productKey, signature, signature.getCount(), hyphenDistance) != -1;

	ecKey.detach();
	return result;
}

inline
sl::String
generateEcProductKey(
	EC_KEY* ecKey,
	const sl::StringRef& userName,
	size_t hyphenDistance = 6
) {
	sl::String productKey;
	generateEcProductKey(ecKey, &productKey, userName, hyphenDistance);
	return productKey;
}

bool
verifyEcProductKey(
	EC_KEY* ecKey0,
	const sl::StringRef& userName,
	const sl::StringRef& productKey
) {
	char buffer[256];
	sl::Array<char> signature(rc::BufKind_Stack, buffer, sizeof(buffer));

	cry::EcKey ecKey(ecKey0);

	bool result =
		enc::Base32Encoding_nj::decode(&signature, productKey) != -1 &&
		ecKey.verify(userName.cp(), userName.getLength(), signature, signature.getCount());

	ecKey.detach();
	return result;
}

//..............................................................................

void run() {
	cry::EcKey key(NID_secp112r1);
	key.generateKey();

	cry::BigNum privateKey = key.getPrivateKey();
	cry::EcPoint publicKey = key.getPublicKey();
	printf("private: %s\n", privateKey.getHexString().sz());
	printf("public:  %s\n", publicKey.getHexString(key.getGroup ()).sz());

	const char* userName = "Vovkos";

	sl::String productKey = generateEcProductKey(key, userName);
	printf("user:    %s\n", userName);
	printf("license: %s\n", productKey.sz());

	bool result = verifyEcProductKey(key, userName, productKey);
	printf("verify:  %d\n", result);
	ASSERT(result);

	privateKey.detach();
	publicKey.detach();

}

//..............................................................................

ADD_TEST_CASE("test_Crypto", run)

}
