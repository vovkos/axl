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
#include "axl_cry_AppStoreReceipt.h"
#include "axl_cry_CryptoError.h"
#include "axl_cry_Pkcs7.h"
#include "axl_cry_Asn1.h"
#include "axl_cry_X509.h"
#include "axl_cry_Bio.h"
#include "axl_sys_Time.h"

#if (_AXL_OS_DARWIN)
#	include "axl_iok_ComputerGuid.h"
#endif

namespace axl {
namespace cry {

//..............................................................................

// decoding assumes GMT (which seems to always be the case with Apple receipts)

static
uint64_t
decodeRfc3339Timestamp(const sl::StringRef& string)
{
	int year = 0;
	int month = 0;
	int monthDay = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;

	int result = sscanf(
		string.sz(),
		"%d-%d-%dT%d:%d:%d",
		&year,
		&month,
		&monthDay,
		&hour,
		&minute,
		&second
		);

	if (result < 3)
	{
		err::setError(err::SystemErrorCode_InvalidParameter);
		return 0;
	}

	sys::Time time;
	time.m_year = year;
	time.m_month = month - 1;
	time.m_monthDay = monthDay;
	time.m_hour = hour;
	time.m_minute = minute;
	time.m_second = second;

	return time.getTimestamp(0);
}

//..............................................................................

AppStoreIap::AppStoreIap()
{
	m_quantity = 0;
	m_webOrderLineItemId = 0;
	m_purchaseTimestamp = 0;
	m_originalPurchaseTimestamp = 0;
	m_subscriptionExpirationTimestamp = 0;
	m_cancellationTimestamp = 0;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AppStoreReceipt::AppStoreReceipt()
{
	m_receiptCreationTimestamp = 0;
	m_receiptExpirationTimestamp = 0;
}

void
AppStoreReceipt::clear()
{
	m_bundleId.clear();
	m_appVersion.clear();
	m_originalAppVersion.clear();
	m_receiptCreationDateString.clear();
	m_receiptExpirationDateString.clear();
	m_receiptCreationTimestamp = 0;
	m_receiptExpirationTimestamp = 0;
	m_opaque.clear();
	m_sha1Hash.clear();
}

//..............................................................................

AppStoreReceiptPayloadParser::AppStoreReceiptPayloadParser()
{
	m_receipt = NULL;
	m_iap = NULL;
	m_attributeId = AttributeId_Undefined;
	m_attributeString = NULL;
	m_attributeInteger = NULL;
	m_attributeIntegerSize = 0;
}

bool
AppStoreReceiptPayloadParser::parse(
	AppStoreReceipt* receipt,
	const void* p,
	size_t size
	)
{
	m_receipt = receipt;

	return decode(
		State_Set,
		V_ASN1_SET,
		"invalid app receipt (expected ASN1 SET)",
		(char*)p,
		size
		) != NULL;
}

const char*
AppStoreReceiptPayloadParser::decode(
	State state,
	int expectedTag,
	const char* unexpectedTagError,
	const char* p,
	size_t size
	)
{
	ASSERT(m_receipt);

	long length;
	int tag;
	int cls;

	int retCode = ASN1_get_object((const uchar_t**)&p, &length, &tag, &cls, size);
	if (retCode & 0x80)
	{
		setLastCryptoError();
		return NULL;
	}

	if (tag != expectedTag)
		return err::fail<const char*>(NULL, unexpectedTagError);

	const char* end = p + length;

	switch (state)
	{
	case State_Set:
		while (p < end)
		{
			p = decode(
				State_Attribute,
				V_ASN1_SEQUENCE,
				"invalid app receipt attribute (expected ASN1 SEQUENCE)",
				p,
				end - p
				);

			if (!p)
				return NULL;
		}

		break;

	case State_Attribute:
		m_attributeId = AttributeId_Undefined;
		m_attributeString = NULL;
		m_attributeInteger = &m_attributeId;
		m_attributeIntegerSize = sizeof(m_attributeId);

		p = decode(
			State_AttributeIntegerValue,
			V_ASN1_INTEGER,
			"invalid app receipt attribute type (expected ASN1 INTEGER)",
			p,
			end - p
			);

		if (!p)
			return NULL;

		m_attributeInteger = NULL; // ignore attribute version value

		p = decode(
			State_AttributeIntegerValue,
			V_ASN1_INTEGER,
			"invalid app receipt attribute version (expected ASN1 INTEGER)",
			p,
			end - p
			);

		if (!p)
			return NULL;

		p = decode(
			State_AttributeValue,
			V_ASN1_OCTET_STRING,
			"invalid app receipt attribute value (expected ASN1 OCTET STRING)",
			p,
			end - p
			);

		if (!p)
			return NULL;

		break;

	case State_AttributeValue:
		switch (m_attributeId)
		{
		case AttributeId_BundleId:
			m_receipt->m_rawBundleId.copy(p, length);
			m_attributeString = &m_receipt->m_bundleId;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_UTF8STRING,
				"invalid app receipt bundle ID (expected ASN1 UTF8 STRING)",
				p,
				length
				);

			break;

		case AttributeId_AppVersion:
			m_attributeString = &m_receipt->m_appVersion;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_UTF8STRING,
				"invalid app receipt version (expected ASN1 UTF8 STRING)",
				p,
				length
				);

			break;

		case AttributeId_OriginalAppVersion:
			m_attributeString = &m_receipt->m_originalAppVersion;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_UTF8STRING,
				"invalid app receipt original version (expected ASN1 UTF8 STRING)",
				p,
				length
				);

			break;

		case AttributeId_ReceiptCreationDate:
			m_attributeString = &m_receipt->m_receiptCreationDateString;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_IA5STRING,
				"invalid app receipt creation date (expected ASN1 IA5 STRING)",
				p,
				length
				);

			m_receipt->m_receiptCreationTimestamp = decodeRfc3339Timestamp(m_receipt->m_receiptCreationDateString);
			break;

		case AttributeId_Iap:
			m_iap = *m_receipt->m_iapList.insertTail(AXL_MEM_NEW(AppStoreIap));

			p = decode(
				State_Set,
				V_ASN1_SET,
				"invalid app receipt IAP (expected ASN1 SET)",
				p,
				end - p
				);

			m_iap = NULL;
			break;

		case AttributeId_Opaque:
			m_receipt->m_opaque.copy(p, length);
			break;

		case AttributeId_Sha1Hash:
			m_receipt->m_sha1Hash.copy(p, length);
			break;

		case AttributeId_IapQuantity:
			if (!m_iap)
				return err::fail<const char*>(NULL, "unexpected IAP attribute in app receipt");

			m_attributeInteger = &m_iap->m_quantity;
			m_attributeIntegerSize = sizeof(m_iap->m_quantity);

			p = decode(
				State_AttributeIntegerValue,
				V_ASN1_INTEGER,
				"invalid app receipt IAP quantity (expected ASN1 INTEGER)",
				p,
				length
				);
			break;

		case AttributeId_IapProductId:
			if (!m_iap)
				return err::fail<const char*>(NULL, "unexpected IAP attribute in app receipt");

			m_attributeString = &m_iap->m_productId;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_UTF8STRING,
				"invalid app receipt IAP product ID (expected ASN1 UTF8 STRING)",
				p,
				length
				);

			break;

		case AttributeId_IapTransactionId:
			if (!m_iap)
				return err::fail<const char*>(NULL, "unexpected IAP attribute in app receipt");

			m_attributeString = &m_iap->m_transactionId;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_UTF8STRING,
				"invalid app receipt IAP transaction ID (expected ASN1 UTF8 STRING)",
				p,
				length
				);

			break;

		case AttributeId_IapPurchaseDate:
			if (!m_iap)
				return err::fail<const char*>(NULL, "unexpected IAP attribute in app receipt");

			m_attributeString = &m_iap->m_purchaseDateString;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_IA5STRING,
				"invalid app receipt IAP purchase date (expected ASN1 IA5 STRING)",
				p,
				length
				);

			m_iap->m_purchaseTimestamp = decodeRfc3339Timestamp(m_iap->m_purchaseDateString);
			break;

		case AttributeId_IapOriginalTransactionId:
			if (!m_iap)
				return err::fail<const char*>(NULL, "unexpected IAP attribute in app receipt");

			m_attributeString = &m_iap->m_originalTransactionId;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_UTF8STRING,
				"invalid app receipt IAP original transaction ID (expected ASN1 UTF8 STRING)",
				p,
				length
				);

			break;

		case AttributeId_IapOriginalPurchaseDate:
			if (!m_iap)
				return err::fail<const char*>(NULL, "unexpected IAP attribute in app receipt");

			m_attributeString = &m_iap->m_originalPurchaseDateString;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_IA5STRING,
				"invalid app receipt IAP original purchase date (expected ASN1 IA5 STRING)",
				p,
				length
				);

			m_iap->m_originalPurchaseTimestamp = decodeRfc3339Timestamp(m_iap->m_originalPurchaseDateString);
			break;

		case AttributeId_IapSubscriptionExpirationDate:
			if (!m_iap)
				return err::fail<const char*>(NULL, "unexpected IAP attribute in app receipt");

			m_attributeString = &m_iap->m_subscriptionExpirationDateString;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_IA5STRING,
				"invalid app receipt IAP subscription expiration date (expected ASN1 IA5 STRING)",
				p,
				length
				);

			m_iap->m_subscriptionExpirationTimestamp = decodeRfc3339Timestamp(m_iap->m_subscriptionExpirationDateString);
			break;

		case AttributeId_IapWebOrderLineItemId:
			if (!m_iap)
				return err::fail<const char*>(NULL, "unexpected IAP attribute in app receipt");

			m_attributeInteger = &m_iap->m_webOrderLineItemId;
			m_attributeIntegerSize = sizeof(m_iap->m_webOrderLineItemId);

			p = decode(
				State_AttributeIntegerValue,
				V_ASN1_INTEGER,
				"invalid app receipt IAP web order line item ID (expected ASN1 INTEGER)",
				p,
				length
				);

			break;

		case AttributeId_IapCancellationDate:
			if (!m_iap)
				return err::fail<const char*>(NULL, "unexpected IAP attribute in app receipt");

			m_attributeString = &m_iap->m_cancellationDateString;

			p = decode(
				State_AttributeStringValue,
				V_ASN1_IA5STRING,
				"invalid app receipt IAP purchase date (expected ASN1 IA5 STRING)",
				p,
				length
				);

			m_iap->m_cancellationTimestamp = decodeRfc3339Timestamp(m_iap->m_cancellationDateString);
			break;

		default:
			AXL_TRACE("WARNING: unsupported AppStore Receipt attribute ID: 0x%x\n", m_attributeId);
		}

		if (!p)
			return NULL;

		break;

	case State_AttributeStringValue:
		ASSERT(m_attributeString);
		m_attributeString->copy(p, length);
		break;

	case State_AttributeIntegerValue:
		if (m_attributeInteger)
		{
			size_t size = AXL_MIN(length, m_attributeIntegerSize);
			sl::ArrayDetails<char>::copyReverse((char*)m_attributeInteger, p, size); // big-endian
		}

		break;

	default:
		ASSERT(false);
	}

	return end;
}

//..............................................................................

bool
verifyAppStoreReceipt(
	AppStoreReceipt* receipt,
	const void* p,
	size_t size,
	const void* computerGuid,
	size_t computerGuidSize,
	uint_t flags
	)
{
	static const char appleRootCertPem[] =
		"-----BEGIN CERTIFICATE-----\n"
		"MIIEuzCCA6OgAwIBAgIBAjANBgkqhkiG9w0BAQUFADBiMQswCQYDVQQGEwJVUzET\n"
		"MBEGA1UEChMKQXBwbGUgSW5jLjEmMCQGA1UECxMdQXBwbGUgQ2VydGlmaWNhdGlv\n"
		"biBBdXRob3JpdHkxFjAUBgNVBAMTDUFwcGxlIFJvb3QgQ0EwHhcNMDYwNDI1MjE0\n"
		"MDM2WhcNMzUwMjA5MjE0MDM2WjBiMQswCQYDVQQGEwJVUzETMBEGA1UEChMKQXBw\n"
		"bGUgSW5jLjEmMCQGA1UECxMdQXBwbGUgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkx\n"
		"FjAUBgNVBAMTDUFwcGxlIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAw\n"
		"ggEKAoIBAQDkkakJH5HbHkdQ6wXtXnmELes2oldMVeyLGYne+Uts9QerIjAC6Bg+\n"
		"+FAJ039BqJj50cpmnCRrEdCju+QbKsMflZ56DKRHi1vUFjczy8QPTc4UadHJGXL1\n"
		"XQ7Vf1+b8iUDulWPTV0N8WQ1IxVLFVkds5T39pyez1C6wVhQZ48ItCD3y6wsIG9w\n"
		"tj8BMIy3Q88PnT3zK0koGsj+zrW5DtleHNbLPbU6rfQPDgCSC7EhFi501TwN22IW\n"
		"q6NxkkdTVcGvL0Gz+PvjcM3mo0xFfh9Ma1CWQYnEdGILEINBhzOKgbEwWOxaBDKM\n"
		"aLOPHd5lc/9nXmW8Sdh2nzMUZaF3lMktAgMBAAGjggF6MIIBdjAOBgNVHQ8BAf8E\n"
		"BAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUK9BpR5R2Cf70a40uQKb3\n"
		"R01/CF4wHwYDVR0jBBgwFoAUK9BpR5R2Cf70a40uQKb3R01/CF4wggERBgNVHSAE\n"
		"ggEIMIIBBDCCAQAGCSqGSIb3Y2QFATCB8jAqBggrBgEFBQcCARYeaHR0cHM6Ly93\n"
		"d3cuYXBwbGUuY29tL2FwcGxlY2EvMIHDBggrBgEFBQcCAjCBthqBs1JlbGlhbmNl\n"
		"IG9uIHRoaXMgY2VydGlmaWNhdGUgYnkgYW55IHBhcnR5IGFzc3VtZXMgYWNjZXB0\n"
		"YW5jZSBvZiB0aGUgdGhlbiBhcHBsaWNhYmxlIHN0YW5kYXJkIHRlcm1zIGFuZCBj\n"
		"b25kaXRpb25zIG9mIHVzZSwgY2VydGlmaWNhdGUgcG9saWN5IGFuZCBjZXJ0aWZp\n"
		"Y2F0aW9uIHByYWN0aWNlIHN0YXRlbWVudHMuMA0GCSqGSIb3DQEBBQUAA4IBAQBc\n"
		"NplMLXi37Yyb3PN3m/J20ncwT8EfhYOFG5k9RzfyqZtAjizUsZAS2L70c5vu0mQP\n"
		"y3lPNNiiPvl4/2vIB+x9OYOLUyDTOMSxv5pPCmv/K/xZpwUJfBdAVhEedNO3iyM7\n"
		"R6PVbyTi69G3cN8PReEnyvFteO3ntRcXqNx+IjXKJdXZD9Zr1KIkIxH3oayPc4Fg\n"
		"xhtbCS+SsvhESPBgOJ4V9T0mZyCKM2r3DYLP3uujL/lTaltkwGMzd/c6ByxW69oP\n"
		"IQ7aunMZT7XZNn/Bh1XZp5m5MkL72NVxnn6hUrcbvZNCJBIqxw8dtk2cXmPIS4AX\n"
		"UKqK1drk/NAJBzewdXUh\n"
		"-----END CERTIFICATE-----\n";

	Pkcs7 pkcs7;
	bool result = pkcs7.loadDer(p, size);
	if (!result)
		return false;

	Bio bio;
	bio.createMem();

	if (flags & VerifyAppStoreReceiptFlag_SkipSignatureCheck)
	{
		result = pkcs7.verify(NULL, NULL, NULL, bio, PKCS7_NOVERIFY | PKCS7_NOSIGS);
	} else
	{
		X509Cert appleRootCert;
		X509Store store;

		result =
			appleRootCert.loadPem(sl::StringRef(appleRootCertPem, lengthof(appleRootCertPem))) &&
			store.create() &&
			store.addCert(appleRootCert) &&
			pkcs7.verify(NULL, store, NULL, bio, 0);
	}

	if (!result)
		return false;

	BUF_MEM* bufMem = bio.getBufMem();

	AppStoreReceiptPayloadParser parser;
	result = parser.parse(receipt, bufMem->data, bufMem->length);
	if (!result)
		return false;

	if (flags & VerifyAppStoreReceiptFlag_SkipHashCheck)
		return true;

	char buffer[256];
	sl::Array<char> message(ref::BufKind_Stack, buffer, sizeof(buffer));
	message.append((char*)computerGuid, computerGuidSize);
	message.append(receipt->m_opaque, receipt->m_opaque.getCount());
	message.append(receipt->m_rawBundleId, receipt->m_rawBundleId.getCount());

	uchar_t digest[20];
	EVP_Digest(message, message.getCount(), digest, NULL, EVP_sha1(), NULL);

	bool isMatch =
		receipt->m_sha1Hash.getCount() == sizeof(digest) &&
		memcmp(receipt->m_sha1Hash, digest, sizeof(digest)) == 0;

	if (!isMatch)
		return err::fail("app receipt SHA1 hash mismatch");

	return true;
}

bool
verifyAppStoreReceipt(
	AppStoreReceipt* receipt,
	const void* p,
	size_t size,
	uint_t flags
	)
{
	sl::Array<char> computerGuid;

#if (_AXL_OS_DARWIN)
	if (!(flags & VerifyAppStoreReceiptFlag_SkipSignatureCheck))
	{
		size_t result = iok::getComputerGuid(&computerGuid);
		if (result == -1)
			return false;
	}
#endif

	return verifyAppStoreReceipt(receipt, p, size, computerGuid, computerGuid.getCount(), flags);
}

//..............................................................................

} // namespace cry
} // namespace axl
