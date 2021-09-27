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
#include "axl_sys_win_Authenticode.h"
#include "axl_sys_win_WinTrust.h"
#include "axl_sys_win_WinCrypt.h"
#include "axl_sys_win_Certificate.h"
#include "axl_sys_win_CryptMsg.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
verifyAuthenticodeSignature(
	const sl::StringRef_w& fileName,
	sl::String_w* programName,
	sl::String_w* subjectName,
	sl::String_w* issuerName,
	sl::Array<char>* serialNumber,
	uint64_t* timestamp
) {
	bool result = verifyTrustFile(fileName);
	if (!result)
		return false;

	CertStore store;
	CryptMsg msg;

    dword_t encodingType;
	dword_t contentType;
	dword_t formatType;

	result = ::CryptQueryObject(
		CERT_QUERY_OBJECT_FILE,
		fileName.sz(),
		CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
		CERT_QUERY_FORMAT_FLAG_BINARY,
		0,
		&encodingType,
		&contentType,
		&formatType,
		store.p(),
		msg.p(),
		NULL
	) != 0;

	sl::Array<char> signerInfoBuffer;
	result = msg.getParam(&signerInfoBuffer, CMSG_SIGNER_INFO_PARAM) != -1;
	if (!result)
		return false;

	const CMSG_SIGNER_INFO* signerInfo = (CMSG_SIGNER_INFO*)signerInfoBuffer.cp();

	if (serialNumber)
		serialNumber->copyReverse((char*)signerInfo->SerialNumber.pbData, signerInfo->SerialNumber.cbData);

	if (programName)
		getCryptMsgSignerInfoProgramName(programName, signerInfo);

	if (timestamp)
		getCryptMsgSignerInfoTimestamp(timestamp, signerInfo);

	CERT_INFO certInfo = { 0 };
	certInfo.Issuer = signerInfo->Issuer;
	certInfo.SerialNumber = signerInfo->SerialNumber;

	Certificate certificate;
	result = store.findCertificate(
		&certificate,
		X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		0,
		CERT_FIND_SUBJECT_CERT,
		&certInfo
	);

	if (!result)
		return false;

	if (subjectName)
		certificate.getSubjectName(subjectName);

	if (issuerName)
		certificate.getIssuerName(issuerName);

	return true;
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
