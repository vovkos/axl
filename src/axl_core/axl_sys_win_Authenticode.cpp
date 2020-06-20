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
	result = msg.getParam(&signerInfoBuffer, CMSG_SIGNER_INFO_PARAM, 0) != -1;
	if (!result)
		return false;

	const CMSG_SIGNER_INFO* signerInfo = (CMSG_SIGNER_INFO*)signerInfoBuffer.cp();
	serialNumber->copyReverse((char*)signerInfo->SerialNumber.pbData, signerInfo->SerialNumber.cbData);
	*programName = getCryptMsgSignerInfoProgramName(signerInfo);

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

	uint64_t timestamp0 = getCryptMsgSignerInfoTimestamp(signerInfo);

	size_t crossCertIdx = findCryptAttr(&signerInfo->UnauthAttrs, szOID_RSA_counterSign);
	if (crossCertIdx != -1)
	{
		sl::Array<char> counterSignerInfoBuffer;

		result = cryptDecodeObject(
			&counterSignerInfoBuffer,
			X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
			PKCS7_SIGNER_INFO,
			signerInfo->UnauthAttrs.rgAttr[crossCertIdx].rgValue[0].pbData,
			signerInfo->UnauthAttrs.rgAttr[crossCertIdx].rgValue[0].cbData
			) != -1;

		if (result)
		{
			const CMSG_SIGNER_INFO* counterSignerInfo = (CMSG_SIGNER_INFO*)counterSignerInfoBuffer.cp();
			*timestamp = getCryptMsgSignerInfoTimestamp(counterSignerInfo);
		}
	}
	else
	{
		crossCertIdx = findCryptAttr(&signerInfo->UnauthAttrs, szOID_RFC3161_counterSign);
		if (crossCertIdx != -1)
		{
			// not yet

		}
	}

	return true;
}
/*
{
                        //Using an RFC3161 time stamp
                        if(pSignerInfo->UnauthAttrs.rgAttr[n].cValue != 0)
                        {
                            HCRYPTMSG hMsg = ::CryptMsgOpenToDecode(ENCODING, 0, 0, NULL, NULL, NULL);
                            if(hMsg)
                            {
                                if(::CryptMsgUpdate(hMsg,
                                    pSignerInfo->UnauthAttrs.rgAttr[n].rgValue->pbData,
                                    pSignerInfo->UnauthAttrs.rgAttr[n].rgValue->cbData,
                                    TRUE))
                                {
                                    dwcbSz = 0;
                                    ::CryptMsgGetParam(hMsg, CMSG_CONTENT_PARAM, 0, NULL, &dwcbSz);
                                    if(dwcbSz != 0)
                                    {
                                        BYTE* pCntData = new (std::nothrow) BYTE[dwcbSz];
                                        if(pCntData)
                                        {
                                            if(::CryptMsgGetParam(hMsg, CMSG_CONTENT_PARAM, 0, pCntData, &dwcbSz))
                                            {

                                                //Retrieve time stamp
                                                FILETIME ftUtc = {0};

                                                void* pTmData = NULL;
                                                DWORD dwcbTmDataSz = 0;

                                                struct Microsoft_forgot_to_document_me{
                                                    void* something_0[9];
                                                    FILETIME ftUtc;
                                                };

                                                #ifndef TIMESTAMP_INFO
                                                #define TIMESTAMP_INFO                     ((LPCSTR) 80)
                                                #endif

                                                if(CryptDecodeObjectEx(ENCODING,    //X509_ASN_ENCODING,
                                                    TIMESTAMP_INFO,
                                                    pCntData,
                                                    dwcbSz,
                                                    CRYPT_DECODE_ALLOC_FLAG,
                                                    NULL,
                                                    &pTmData, &dwcbTmDataSz) &&
                                                    pTmData &&
                                                    dwcbTmDataSz >= sizeof(Microsoft_forgot_to_document_me))
                                                {
                                                    ftUtc = ((Microsoft_forgot_to_document_me*)pTmData)->ftUtc;
                                                }
                                                else
                                                    _tprintf(L"ERROR: (0x%X) CryptDecodeObjectEx(RFC3161/TIMESTAMP_INFO) data failed\n", ::GetLastError());

                                                if(pTmData)
                                                {
                                                    ::LocalFree(pTmData);
                                                    pTmData = NULL;
                                                }


                                                //Try to get signer info
                                                dwcbSz = 0;
                                                ::CryptMsgGetParam(hMsg, CMSG_SIGNER_INFO_PARAM, 0, NULL, &dwcbSz);
                                                if(dwcbSz != 0)
                                                {
                                                    CMSG_SIGNER_INFO* pTmSignerData = (CMSG_SIGNER_INFO*)new (std::nothrow) BYTE[dwcbSz];
                                                    if(pTmSignerData)
                                                    {
                                                        if(::CryptMsgGetParam(hMsg, CMSG_SIGNER_INFO_PARAM, 0, pTmSignerData, &dwcbSz))
                                                        {
                                                            CRYPT_DATA_BLOB c7Data;
                                                            c7Data.pbData = pSignerInfo->UnauthAttrs.rgAttr[n].rgValue->pbData;
                                                            c7Data.cbData = pSignerInfo->UnauthAttrs.rgAttr[n].rgValue->cbData;

                                                            //Try to locate the appropriate store
                                                            FindAppropriateStoreAndPrintCertificateInformation(pTmSignerData, &c7Data, L"TimeStamp Certificate", TRUE, &ftUtc);
                                                        }
                                                        else
                                                            _tprintf(L"ERROR: (0x%X) CryptMsgGetParam(RFC3161/CMSG_SIGNER_INFO_PARAM) data failed\n", ::GetLastError());

                                                        //Free mem
                                                        delete[] pTmSignerData;
                                                        pTmSignerData = NULL;
                                                    }
                                                    else
                                                        _tprintf(L"ERROR: (0x%X) new(RFC3161) failed\n", ::GetLastError());
                                                }
                                                else
                                                    _tprintf(L"ERROR: (0x%X) CryptMsgGetParam(RFC3161/CMSG_SIGNER_INFO_PARAM) failed\n", ::GetLastError());


                                            }
                                            else
                                                _tprintf(L"ERROR: (0x%X) CryptMsgGetParam(RFC3161/CMSG_CONTENT_PARAM) data failed\n", ::GetLastError());

                                            //Free mem
                                            delete[] pCntData;
                                            pCntData = NULL;
                                        }
                                        else
                                            _tprintf(L"ERROR: (0x%X) new(RFC3161) failed\n", ::GetLastError());
                                    }
                                    else
                                        _tprintf(L"ERROR: (0x%X) CryptMsgGetParam(RFC3161/CMSG_CONTENT_PARAM) failed\n", ::GetLastError());
                                }
                                else
                                    _tprintf(L"ERROR: (0x%X) CryptMsgUpdate(RFC3161) failed\n", ::GetLastError());

                                //Free handle
                                ::CryptMsgClose(hMsg);
                                hMsg = NULL;
                            }
                            else
                                _tprintf(L"ERROR: (0x%X) CryptMsgOpenToDecode(ENCODING) failed\n", ::GetLastError());
                        }

                    }
                }
            }
*/

//..............................................................................

#define ENCODING (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)

typedef struct {
    LPWSTR lpszProgramName;
    LPWSTR lpszPublisherLink;
    LPWSTR lpszMoreInfoLink;
} SPROG_PUBLISHERINFO, *PSPROG_PUBLISHERINFO;

BOOL GetProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo,
                             PSPROG_PUBLISHERINFO Info);
BOOL GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME *st);
BOOL PrintCertificateInfo(PCCERT_CONTEXT pCertContext);
BOOL GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo,
                            PCMSG_SIGNER_INFO *pCounterSignerInfo);

int ShowAuthenticodeInfo(LPCWSTR pszFileName)
{
    HCERTSTORE hStore = NULL;
    HCRYPTMSG hMsg = NULL;
    PCCERT_CONTEXT pCertContext = NULL;
    BOOL fResult;
	DWORD dwEncoding, dwContentType, dwFormatType;
    PCMSG_SIGNER_INFO pSignerInfo = NULL;
    PCMSG_SIGNER_INFO pCounterSignerInfo = NULL;
    DWORD dwSignerInfo;
    CERT_INFO CertInfo;
    SPROG_PUBLISHERINFO ProgPubInfo;
    SYSTEMTIME st;

    ZeroMemory(&ProgPubInfo, sizeof(ProgPubInfo));
    __try
    {
        // Get message handle and store handle from the signed file.
        fResult = CryptQueryObject(CERT_QUERY_OBJECT_FILE,
                                   pszFileName,
                                   CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                                   CERT_QUERY_FORMAT_FLAG_BINARY,
                                   0,
                                   &dwEncoding,
                                   &dwContentType,
                                   &dwFormatType,
                                   &hStore,
                                   &hMsg,
                                   NULL);

        if (!fResult)
        {
            printf("CryptQueryObject failed with %x\n", GetLastError());
            __leave;
        }

        // Get signer information size.
        fResult = CryptMsgGetParam(hMsg,
                                   CMSG_SIGNER_INFO_PARAM,
                                   0,
                                   NULL,
                                   &dwSignerInfo);
        if (!fResult)
        {
            printf("CryptMsgGetParam failed with %x\n", GetLastError());
            __leave;
        }

        // Allocate memory for signer information.
        pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);
        if (!pSignerInfo)
        {
            printf("Unable to allocate memory for Signer Info.\n");
            __leave;
        }

        // Get Signer Information.
        fResult = CryptMsgGetParam(hMsg,
                                   CMSG_SIGNER_INFO_PARAM,
                                   0,
                                   (PVOID)pSignerInfo,
                                   &dwSignerInfo);
        if (!fResult)
        {
            printf("CryptMsgGetParam failed with %x\n", GetLastError());
            __leave;
        }

        // Get program name and publisher information from
        // signer info structure.
        if (GetProgAndPublisherInfo(pSignerInfo, &ProgPubInfo))
        {
            if (ProgPubInfo.lpszProgramName != NULL)
            {
                wprintf(L"Program Name : %s\n",
                    ProgPubInfo.lpszProgramName);
            }

            if (ProgPubInfo.lpszPublisherLink != NULL)
            {
                wprintf(L"Publisher Link : %s\n",
                    ProgPubInfo.lpszPublisherLink);
            }

            if (ProgPubInfo.lpszMoreInfoLink != NULL)
            {
                wprintf(L"MoreInfo Link : %s\n",
                    ProgPubInfo.lpszMoreInfoLink);
            }
        }

        printf("\n");

        // Search for the signer certificate in the temporary
        // certificate store.
        CertInfo.Issuer = pSignerInfo->Issuer;
        CertInfo.SerialNumber = pSignerInfo->SerialNumber;

        pCertContext = CertFindCertificateInStore(hStore,
                                                  ENCODING,
                                                  0,
                                                  CERT_FIND_SUBJECT_CERT,
                                                  (PVOID)&CertInfo,
                                                  NULL);
        if (!pCertContext)
        {
            printf("CertFindCertificateInStore failed with %x\n",
                GetLastError());
            __leave;
        }

        // Print Signer certificate information.
        printf("Signer Certificate:\n\n");
        PrintCertificateInfo(pCertContext);
        printf("\n");

        // Get the timestamp certificate signerinfo structure.
        if (GetTimeStampSignerInfo(pSignerInfo, &pCounterSignerInfo))
        {
            // Search for Timestamp certificate in the temporary
            // certificate store.
            CertInfo.Issuer = pCounterSignerInfo->Issuer;
            CertInfo.SerialNumber = pCounterSignerInfo->SerialNumber;

            pCertContext = CertFindCertificateInStore(hStore,
                                                ENCODING,
                                                0,
                                                CERT_FIND_SUBJECT_CERT,
                                                (PVOID)&CertInfo,
                                                NULL);
            if (!pCertContext)
            {
                printf("CertFindCertificateInStore failed with %x\n",
                    GetLastError());
                __leave;
            }

            // Print timestamp certificate information.
            printf("TimeStamp Certificate:\n\n");
            PrintCertificateInfo(pCertContext);
            printf("\n");

            // Find Date of timestamp.
            if (GetDateOfTimeStamp(pCounterSignerInfo, &st))
            {
                printf("Date of TimeStamp : %02d/%02d/%04d %02d:%02d\n",
                                            st.wMonth,
                                            st.wDay,
                                            st.wYear,
                                            st.wHour,
                                            st.wMinute);
            }
            printf("\n");
        }
    }
    __finally
    {
        // Clean up.
        if (ProgPubInfo.lpszProgramName != NULL)
            LocalFree(ProgPubInfo.lpszProgramName);
        if (ProgPubInfo.lpszPublisherLink != NULL)
            LocalFree(ProgPubInfo.lpszPublisherLink);
        if (ProgPubInfo.lpszMoreInfoLink != NULL)
            LocalFree(ProgPubInfo.lpszMoreInfoLink);

        if (pSignerInfo != NULL) LocalFree(pSignerInfo);
        if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
        if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
        if (hStore != NULL) CertCloseStore(hStore, 0);
        if (hMsg != NULL) CryptMsgClose(hMsg);
    }
    return 0;
}

BOOL PrintCertificateInfo(PCCERT_CONTEXT pCertContext)
{
    BOOL fReturn = FALSE;
    LPTSTR szName = NULL;
    DWORD dwData;

    __try
    {
        // Print Serial Number.
        printf("Serial Number: ");
        dwData = pCertContext->pCertInfo->SerialNumber.cbData;
        for (DWORD n = 0; n < dwData; n++)
        {
            printf("%02x ",
              pCertContext->pCertInfo->SerialNumber.pbData[dwData - (n + 1)]);
        }
        printf("\n");

        // Get Issuer name size.
        if (!(dwData = CertGetNameString(pCertContext,
                                         CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                         CERT_NAME_ISSUER_FLAG,
                                         NULL,
                                         NULL,
                                         0)))
        {
            printf("CertGetNameString failed.\n");
            __leave;
        }

        // Allocate memory for Issuer name.
        szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));
        if (!szName)
        {
            printf("Unable to allocate memory for issuer name.\n");
            __leave;
        }

        // Get Issuer name.
        if (!(CertGetNameString(pCertContext,
                                CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                CERT_NAME_ISSUER_FLAG,
                                NULL,
                                szName,
                                dwData)))
        {
            printf("CertGetNameString failed.\n");
            __leave;
        }

        // print Issuer name.
        printf("Issuer Name: %S\n", szName);
        LocalFree(szName);
        szName = NULL;

        // Get Subject name size.
        if (!(dwData = CertGetNameString(pCertContext,
                                         CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                         0,
                                         NULL,
                                         NULL,
                                         0)))
        {
            printf("CertGetNameString failed.\n");
            __leave;
        }

        // Allocate memory for subject name.
        szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));
        if (!szName)
        {
            printf("Unable to allocate memory for subject name.\n");
            __leave;
        }

        // Get subject name.
        if (!(CertGetNameString(pCertContext,
                                CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                0,
                                NULL,
                                szName,
                                dwData)))
        {
            printf("CertGetNameString failed.\n");
            __leave;
        }

        // Print Subject Name.
        printf("Subject Name: %S\n", szName);

        fReturn = TRUE;
    }
    __finally
    {
        if (szName != NULL) LocalFree(szName);
    }

    return fReturn;
}

LPWSTR AllocateAndCopyWideString(LPCWSTR inputString)
{
    LPWSTR outputString = NULL;

    outputString = (LPWSTR)LocalAlloc(LPTR,
        (wcslen(inputString) + 1) * sizeof(WCHAR));
    if (outputString != NULL)
    {
        lstrcpyW(outputString, inputString);
    }
    return outputString;
}

BOOL GetProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo,
                             PSPROG_PUBLISHERINFO Info)
{
    BOOL fReturn = FALSE;
    PSPC_SP_OPUS_INFO OpusInfo = NULL;
    DWORD dwData;
    BOOL fResult;

    __try
    {
        // Loop through authenticated attributes and find
        // SPC_SP_OPUS_INFO_OBJID OID.
        for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++)
        {
            if (lstrcmpA(SPC_SP_OPUS_INFO_OBJID,
                        pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0)
            {
                // Get Size of SPC_SP_OPUS_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                            SPC_SP_OPUS_INFO_OBJID,
                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                            0,
                            NULL,
                            &dwData);
                if (!fResult)
                {
                    printf("CryptDecodeObject failed with %x\n",
                        GetLastError());
                    __leave;
                }

                // Allocate memory for SPC_SP_OPUS_INFO structure.
                OpusInfo = (PSPC_SP_OPUS_INFO)LocalAlloc(LPTR, dwData);
                if (!OpusInfo)
                {
                    printf("Unable to allocate memory for Publisher Info.\n");
                    __leave;
                }

                // Decode and get SPC_SP_OPUS_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                            SPC_SP_OPUS_INFO_OBJID,
                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                            pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                            0,
                            OpusInfo,
                            &dwData);
                if (!fResult)
                {
                    printf("CryptDecodeObject failed with %x\n",
                        GetLastError());
                    __leave;
                }

                // Fill in Program Name if present.
                if (OpusInfo->pwszProgramName)
                {
                    Info->lpszProgramName =
                        AllocateAndCopyWideString(OpusInfo->pwszProgramName);
                }
                else
                    Info->lpszProgramName = NULL;

                // Fill in Publisher Information if present.
                if (OpusInfo->pPublisherInfo)
                {

                    switch (OpusInfo->pPublisherInfo->dwLinkChoice)
                    {
                        case SPC_URL_LINK_CHOICE:
                            Info->lpszPublisherLink =
                                AllocateAndCopyWideString(OpusInfo->pPublisherInfo->pwszUrl);
                            break;

                        case SPC_FILE_LINK_CHOICE:
                            Info->lpszPublisherLink =
                                AllocateAndCopyWideString(OpusInfo->pPublisherInfo->pwszFile);
                            break;

                        default:
                            Info->lpszPublisherLink = NULL;
                            break;
                    }
                }
                else
                {
                    Info->lpszPublisherLink = NULL;
                }

                // Fill in More Info if present.
                if (OpusInfo->pMoreInfo)
                {
                    switch (OpusInfo->pMoreInfo->dwLinkChoice)
                    {
                        case SPC_URL_LINK_CHOICE:
                            Info->lpszMoreInfoLink =
                                AllocateAndCopyWideString(OpusInfo->pMoreInfo->pwszUrl);
                            break;

                        case SPC_FILE_LINK_CHOICE:
                            Info->lpszMoreInfoLink =
                                AllocateAndCopyWideString(OpusInfo->pMoreInfo->pwszFile);
                            break;

                        default:
                            Info->lpszMoreInfoLink = NULL;
                            break;
                    }
                }
                else
                {
                    Info->lpszMoreInfoLink = NULL;
                }

                fReturn = TRUE;

                break; // Break from for loop.
            } // lstrcmp SPC_SP_OPUS_INFO_OBJID
        } // for
    }
    __finally
    {
        if (OpusInfo != NULL) LocalFree(OpusInfo);
    }

    return fReturn;
}

BOOL GetDateOfTimeStamp(PCMSG_SIGNER_INFO pSignerInfo, SYSTEMTIME *st)
{
    BOOL fResult;
    FILETIME lft, ft;
    DWORD dwData;
    BOOL fReturn = FALSE;

    // Loop through authenticated attributes and find
    // szOID_RSA_signingTime OID.
    for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++)
    {
        if (lstrcmpA(szOID_RSA_signingTime,
                    pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0)
        {
            // Decode and get FILETIME structure.
            dwData = sizeof(ft);
            fResult = CryptDecodeObject(ENCODING,
                        szOID_RSA_signingTime,
                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
                        pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
                        0,
                        (PVOID)&ft,
                        &dwData);
            if (!fResult)
            {
                printf("CryptDecodeObject failed with %x\n",
                    GetLastError());
                break;
            }

            // Convert to local time.
            FileTimeToLocalFileTime(&ft, &lft);
            FileTimeToSystemTime(&lft, st);

            fReturn = TRUE;

            break; // Break from for loop.

        } //lstrcmp szOID_RSA_signingTime
    } // for

    return fReturn;
}

BOOL GetTimeStampSignerInfo(PCMSG_SIGNER_INFO pSignerInfo, PCMSG_SIGNER_INFO *pCounterSignerInfo)
{
    PCCERT_CONTEXT pCertContext = NULL;
    BOOL fReturn = FALSE;
    BOOL fResult;
    DWORD dwSize;

    __try
    {
        *pCounterSignerInfo = NULL;

        // Loop through unathenticated attributes for
        // szOID_RSA_counterSign OID.
        for (DWORD n = 0; n < pSignerInfo->UnauthAttrs.cAttr; n++)
        {
            if (lstrcmpA(pSignerInfo->UnauthAttrs.rgAttr[n].pszObjId,
                         szOID_RSA_counterSign) == 0)
            {
                // Get size of CMSG_SIGNER_INFO structure.
                fResult = CryptDecodeObject(ENCODING,
                           PKCS7_SIGNER_INFO,
                           pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                           pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                           0,
                           NULL,
                           &dwSize);
                if (!fResult)
                {
                    printf("CryptDecodeObject failed with %x\n",
                        GetLastError());
                    __leave;
                }

                // Allocate memory for CMSG_SIGNER_INFO.
                *pCounterSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSize);
                if (!*pCounterSignerInfo)
                {
                    printf("Unable to allocate memory for timestamp info.\n");
                    __leave;
                }

                // Decode and get CMSG_SIGNER_INFO structure
                // for timestamp certificate.
                fResult = CryptDecodeObject(ENCODING,
                           PKCS7_SIGNER_INFO,
                           pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].pbData,
                           pSignerInfo->UnauthAttrs.rgAttr[n].rgValue[0].cbData,
                           0,
                           (PVOID)*pCounterSignerInfo,
                           &dwSize);
                if (!fResult)
                {
                    printf("CryptDecodeObject failed with %x\n",
                        GetLastError());
                    __leave;
                }

                fReturn = TRUE;

                break; // Break from for loop.
            }
        }
    }
    __finally
    {
        // Clean up.
        if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
    }

    return fReturn;
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
