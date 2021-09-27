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

#define _AXL_SYS_WIN_CRYPTMSG_H

#include "axl_sl_Handle.h"
#include "axl_sl_Array.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class CloseCryptMsg {
public:
	void
	operator () (HCRYPTMSG h) {
		::CryptMsgClose(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CryptMsg: public sl::Handle<HCRYPTMSG, CloseCryptMsg> {
public:
	CryptMsg() {}

	CryptMsg(HCRYPTMSG h) {
		attach(h);
	}

	bool
	openToDecode(
		dword_t encodingType,
		dword_t msgFlags,
		dword_t msgType,
		HCRYPTPROV_LEGACY hCryptProv = NULL,
		CERT_INFO* recipientInfo = NULL,
		CMSG_STREAM_INFO* streamInfo = NULL
	);

	bool
	update(
		const void* p,
		size_t size,
		bool isFinal = true
	);

	size_t
	getParam(
		dword_t type,
		dword_t index,
		void* p,
		size_t size
	);

	size_t
	getParam(
		sl::Array<char>* buffer,
		dword_t type,
		dword_t index = 0
	);
};

//..............................................................................

size_t
findCryptAttr(
	const CRYPT_ATTRIBUTES* attributes,
	const char* oid
);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
getCryptMsgSignerInfoProgramName(
	sl::String_w* programName,
	const CMSG_SIGNER_INFO* signerInfo
);

inline
sl::String_w
getCryptMsgSignerInfoProgramName(const CMSG_SIGNER_INFO* signerInfo) {
	sl::String_w programName;
	getCryptMsgSignerInfoProgramName(&programName, signerInfo);
	return programName;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
getCryptMsgSignerInfoTimestamp(
	uint64_t* timestamp,
	const CMSG_SIGNER_INFO* signerInfo
);

inline
uint64_t
getCryptMsgSignerInfoTimestamp(const CMSG_SIGNER_INFO* signerInfo) {
	uint64_t timestamp = 0;
	getCryptMsgSignerInfoTimestamp(&timestamp, signerInfo);
	return timestamp;
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
