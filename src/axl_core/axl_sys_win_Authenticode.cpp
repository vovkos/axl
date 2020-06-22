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
	sl::Array<char>* serialNumber,
	sl::String_w* programName,
	sl::String_w* subjectName,
	sl::String_w* issuerName,
	uint64_t* timestamp
	)
{
	serialNumber->clear();
	programName->clear();
	subjectName->clear();
	issuerName->clear();
	*timestamp = 0;

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
		);

	sl::Array<char> signerInfoBuffer;
	result = msg.getParam(&signerInfoBuffer, CMSG_SIGNER_INFO_PARAM) != -1;
	if (!result)
		return false;

	const CMSG_SIGNER_INFO* signerInfo = (CMSG_SIGNER_INFO*)signerInfoBuffer.cp();
	serialNumber->copyReverse((char*)signerInfo->SerialNumber.pbData, signerInfo->SerialNumber.cbData);
	getCryptMsgSignerInfoProgramName(programName, signerInfo);
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

	certificate.getSubjectName(subjectName);
	certificate.getIssuerName(issuerName);
	return true;
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
