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
#include "axl_sys_win_CryptMsg.h"
#include "axl_sys_win_WinCrypt.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
CryptMsg::openToDecode(
    dword_t encodingType,
    dword_t msgFlags,
    dword_t msgType,
    HCRYPTPROV_LEGACY hCryptProv,
    CERT_INFO* recipientInfo,
    CMSG_STREAM_INFO* streamInfo
	) {
	close();

	m_h = ::CryptMsgOpenToDecode(
		encodingType,
		msgFlags,
		msgType,
		hCryptProv,
		recipientInfo,
		streamInfo
	);

	return err::complete(m_h != NULL);
}

bool
CryptMsg::update(
	const void* p,
	size_t size,
	bool isFinal
) {
	ASSERT(m_h);

	bool_t result = ::CryptMsgUpdate(m_h, (uchar_t*)p, size, isFinal);
	return err::complete(result);
}

size_t
CryptMsg::getParam(
	dword_t type,
	dword_t index,
	void* p,
	size_t size
) {
	ASSERT(m_h);

	dword_t resultSize = size;

	bool_t result = ::CryptMsgGetParam(
		m_h,
		type,
		index,
		p,
		&resultSize
	);

	if (!result) {
		err::setLastSystemError();
		return -1;
	}

	return resultSize;
}

size_t
CryptMsg::getParam(
	sl::Array<char>* buffer,
	dword_t type,
	dword_t index
) {
	size_t size = getParam(type, index, NULL, 0);
	if (size == -1)
		return -1;

	bool result = buffer->setCount(size);
	if (!result)
		return -1;

	return getParam(type, index, buffer->p(), size);
}

//..............................................................................

size_t
findCryptAttr(
	const CRYPT_ATTRIBUTES* attributes,
	const char* oid
) {
	for (size_t i = 0; i < attributes->cAttr; i++) {
		if (strcmp(attributes->rgAttr[i].pszObjId, oid) == 0)
			return i;
	}

	return -1;
}

size_t
getCryptMsgSignerInfoProgramName(
	sl::String_w* programName,
	const CMSG_SIGNER_INFO* signerInfo
) {
	size_t opusInfoIdx = findCryptAttr(&signerInfo->AuthAttrs, SPC_SP_OPUS_INFO_OBJID);
	if (opusInfoIdx == -1) {
		err::setError(err::SystemErrorCode_ObjectNameNotFound);
		return -1;
	}

	sl::Array<char> opusInfoBuffer;

	bool result = cryptDecodeObject(
		&opusInfoBuffer,
		X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		SPC_SP_OPUS_INFO_OBJID,
		signerInfo->AuthAttrs.rgAttr[opusInfoIdx].rgValue[0].pbData,
		signerInfo->AuthAttrs.rgAttr[opusInfoIdx].rgValue[0].cbData
	) != -1;

	if (!result)
		return -1;

	const SPC_SP_OPUS_INFO* opusInfo = (SPC_SP_OPUS_INFO*)opusInfoBuffer.cp();
	*programName = opusInfo->pwszProgramName;
	return programName->getLength();
}

static
bool
getRsaCounterSignTimestamp(
	uint64_t* timestamp,
	const void* p,
	size_t size
) {
	sl::Array<char> counterSignerInfoBuffer;

	bool result = cryptDecodeObject(
		&counterSignerInfoBuffer,
		X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		PKCS7_SIGNER_INFO,
		p,
		size
	) != -1;

	if (!result)
		return false;

	const CMSG_SIGNER_INFO* counterSignerInfo = (CMSG_SIGNER_INFO*)counterSignerInfoBuffer.cp();
	size_t timestampIdx = findCryptAttr(&counterSignerInfo->AuthAttrs, szOID_RSA_signingTime);
	if (timestampIdx == -1) {
		err::setError(err::SystemErrorCode_ObjectNameNotFound);
		return false;
	}

	return cryptDecodeObject(
		timestamp,
		sizeof(uint64_t),
		X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		szOID_RSA_signingTime,
		counterSignerInfo->AuthAttrs.rgAttr[timestampIdx].rgValue[0].pbData,
		counterSignerInfo->AuthAttrs.rgAttr[timestampIdx].rgValue[0].cbData
	) != -1;
}

#pragma pack(push, 1)
struct TimestampInfoRfc3161 {
	uintptr_t m_unknown[9];
	uint64_t m_timestamp;
};
#pragma pack(pop)

static
bool
getRfc3161CounterSignTimestamp(
	uint64_t* timestamp,
	const void* p,
	size_t size
) {
	sl::Array<char> contentBuffer;

	CryptMsg msg;
	bool result =
		msg.openToDecode(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, 0, 0) &&
		msg.update(p, size) &&
		msg.getParam(&contentBuffer, CMSG_CONTENT_PARAM) != -1;

	if (!result)
		return false;

	sl::Array<char> timestampBuffer;
	result = cryptDecodeObject(
		&timestampBuffer,
		X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		TIMESTAMP_INFO,
		contentBuffer,
		contentBuffer.getCount()
	) != -1;

	if (!result)
		return false;

	if (timestampBuffer.getCount() < sizeof(TimestampInfoRfc3161)) {
		err::setError(err::SystemErrorCode_BufferTooSmall);
		return false;
	}

	const TimestampInfoRfc3161* timestampInfo = (TimestampInfoRfc3161*)timestampBuffer.cp();
	*timestamp = timestampInfo->m_timestamp;
	return true;
}

bool
getCryptMsgSignerInfoTimestamp(
	uint64_t* timestamp,
	const CMSG_SIGNER_INFO* signerInfo
) {
	size_t crossCertIdx = findCryptAttr(&signerInfo->UnauthAttrs, szOID_RSA_counterSign);
	if (crossCertIdx != -1)
		return getRsaCounterSignTimestamp(
			timestamp,
			signerInfo->UnauthAttrs.rgAttr[crossCertIdx].rgValue->pbData,
			signerInfo->UnauthAttrs.rgAttr[crossCertIdx].rgValue->cbData
		);

	crossCertIdx = findCryptAttr(&signerInfo->UnauthAttrs, szOID_RFC3161_counterSign);
	if (crossCertIdx != -1)
		return getRfc3161CounterSignTimestamp(
			timestamp,
			signerInfo->UnauthAttrs.rgAttr[crossCertIdx].rgValue->pbData,
			signerInfo->UnauthAttrs.rgAttr[crossCertIdx].rgValue->cbData
		);

	err::setError(err::SystemErrorCode_ObjectNameNotFound);
	return false;
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
