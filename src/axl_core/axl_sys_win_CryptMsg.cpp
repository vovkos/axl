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

size_t
CryptMsg::getParam(
	dword_t type,
	dword_t index,
	void* p,
	size_t size
	)
{
	ASSERT(m_h);

	dword_t resultSize = size;

	bool_t result = ::CryptMsgGetParam(
		m_h,
		type,
		index,
		p,
		&resultSize
		);

	if (!result)
	{
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
	)
{
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
	)
{
	for (size_t i = 0; i < attributes->cAttr; i++)
	{
		if (strcmp(attributes->rgAttr[i].pszObjId, oid) == 0)
			return i;
	}

	return -1;
}

size_t
getCryptMsgSignerInfoProgramName(
	sl::String_w* programName,
	const CMSG_SIGNER_INFO* signerInfo
	)
{
	size_t opusInfoIdx = findCryptAttr(&signerInfo->AuthAttrs, SPC_SP_OPUS_INFO_OBJID);
	if (opusInfoIdx == -1)
	{
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

bool
getCryptMsgSignerInfoTimestamp(
	uint64_t* timestamp,
	const CMSG_SIGNER_INFO* signerInfo
	)
{
	size_t timestampIdx = findCryptAttr(&signerInfo->AuthAttrs, szOID_RSA_signingTime);
	if (timestampIdx == -1)
	{
		err::setError(err::SystemErrorCode_ObjectNameNotFound);
		return false;
	}

	return cryptDecodeObject(
		timestamp,
		sizeof(uint64_t),
		X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		szOID_RSA_signingTime,
		signerInfo->AuthAttrs.rgAttr[timestampIdx].rgValue[0].pbData,
		signerInfo->AuthAttrs.rgAttr[timestampIdx].rgValue[0].cbData
		) != -1;
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
