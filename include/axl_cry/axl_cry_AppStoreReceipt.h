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

#define _AXL_CRY_APPSTORERECEIPT_H

#include "axl_cry_AppStoreReceipt.h"
#include "axl_err_Error.h"

namespace axl {
namespace cry {

//..............................................................................

struct AppStoreIap: sl::ListLink
{
	size_t m_quantity;
	uint_t m_webOrderLineItemId;

	sl::String m_productId;
	sl::String m_transactionId;
	sl::String m_originalTransactionId;
	sl::String m_purchaseDateString;
	sl::String m_originalPurchaseDateString;
	sl::String m_subscriptionExpirationDateString;
	sl::String m_cancellationDateString;

	uint64_t m_purchaseTimestamp;
	uint64_t m_originalPurchaseTimestamp;
	uint64_t m_subscriptionExpirationTimestamp;
	uint64_t m_cancellationTimestamp;

	AppStoreIap();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct AppStoreReceipt
{
	sl::String m_bundleId;
	sl::String m_appVersion;
	sl::String m_originalAppVersion;
	sl::String m_receiptCreationDateString;
	sl::String m_receiptExpirationDateString;

	uint64_t m_receiptCreationTimestamp;
	uint64_t m_receiptExpirationTimestamp;

	sl::Array<char> m_rawBundleId;
	sl::Array<char> m_opaque;
	sl::Array<char> m_sha1Hash;

	sl::List<AppStoreIap> m_iapList;

	AppStoreReceipt();

	void
	clear();
};

//..............................................................................

class AppStoreReceiptPayloadParser
{
protected:
	enum State
	{
		State_Idle = 0,
		State_Set,
		State_Attribute,
		State_AttributeValue,
		State_AttributeStringValue,
		State_AttributeIntegerValue,
	};

	enum AttributeId
	{
		AttributeId_Undefined             = 0,
		AttributeId_BundleId              = 2,
		AttributeId_AppVersion            = 3,
		AttributeId_Opaque                = 4,
		AttributeId_Sha1Hash              = 5,
		AttributeId_ReceiptCreationDate   = 12,
		AttributeId_Iap                   = 17,
		AttributeId_OriginalAppVersion    = 19,
		AttributeId_ReceiptExpirationDate = 21,

		AttributeId_IapQuantity                   = 1701,
		AttributeId_IapProductId                  = 1702,
		AttributeId_IapTransactionId              = 1703,
		AttributeId_IapPurchaseDate               = 1704,
		AttributeId_IapOriginalTransactionId      = 1705,
		AttributeId_IapOriginalPurchaseDate       = 1706,
		AttributeId_IapSubscriptionExpirationDate = 1708,
		AttributeId_IapWebOrderLineItemId         = 1711,
		AttributeId_IapCancellationDate           = 1712,
	};

	AppStoreReceipt* m_receipt;
	AppStoreIap* m_iap;
	AttributeId m_attributeId;
	sl::String* m_attributeString;
	void* m_attributeInteger;
	size_t m_attributeIntegerSize;

public:
	AppStoreReceiptPayloadParser();

	bool
	parse(
		AppStoreReceipt* receipt,
		const void* p,
		size_t size
		);

protected:
	const char*
	decode(
		State state,
		int expectedTag,
		const char* unexpectedTagError,
		const char* p,
		size_t size
		);
};

//..............................................................................

enum VerifyAppStoreReceiptFlag
{
	VerifyAppStoreReceiptFlag_SkipSignatureCheck = 0x01,
	VerifyAppStoreReceiptFlag_SkipHashCheck      = 0x02,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
verifyAppStoreReceipt(
	AppStoreReceipt* receipt,
	const void* p,
	size_t size,
	const void* computerGuid,
	size_t computerGuidSize,
	uint_t flags = 0
	);

bool
verifyAppStoreReceipt(
	AppStoreReceipt* receipt,
	const void* p,
	size_t size,
	uint_t flags = 0
	);

//..............................................................................

} // namespace cry
} // namespace axl
