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

#define _AXL_CRY_AES_H

#include "axl_cry_Pch.h"
#include "axl_err_Error.h"

namespace axl {
namespace cry {

//..............................................................................

struct AesKey: AES_KEY {
	AesKey() {
		memset(this, 0, sizeof(AesKey));
	}

	bool
	setEncryptKey(
		const void* p,
		size_t bitCount
	) {
		int result = AES_set_encrypt_key((const uchar_t*)p, (int)bitCount, this);
		return err::completeWithSystemError(result == 0, err::SystemErrorCode_InvalidParameter);
	}

	bool
	setDecryptKey(
		const void* p,
		size_t bitCount
	) {
		int result = AES_set_decrypt_key((const uchar_t*)p, (int)bitCount, this);
		return err::completeWithSystemError(result == 0, err::SystemErrorCode_InvalidParameter);
	}

	void cbcEncrypt(
		void* dst,
		const void* src,
		size_t size,
		void* iv
	) {
		AES_cbc_encrypt(
			(const uchar_t*)src,
			(uchar_t*)dst,
			size,
			this,
			(uchar_t*)iv,
			AES_ENCRYPT
		);
	}

	void cbcDecrypt(
		void* dst,
		const void* src,
		size_t size,
		void* iv
	) {
		AES_cbc_encrypt(
			(const uchar_t*)src,
			(uchar_t*)dst,
			size,
			this,
			(uchar_t*)iv,
			AES_DECRYPT
		);
	}
};

//..............................................................................

} // namespace cry
} // namespace axl
