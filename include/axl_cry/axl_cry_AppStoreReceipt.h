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

struct AppStoreReceipt
{
	sl::String m_bundleId;
	sl::String m_appVersion;
	sl::String m_originalAppVersion;
	sl::String m_receiptCreationDate;
	sl::String m_receiptExpirationDate;

	sl::Array<char> m_rawBundleId;
	sl::Array<char> m_opaque;
	sl::Array<char> m_sha1Hash;

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
		State_Receipt,
		State_Attribute,
		State_AttributeType,
		State_AttributeVersion,
		State_AttributeValue,
		State_AttributeStringValue,
	};

	enum AttributeId
	{
		AttributeId_Undefined             = 0,
		AttributeId_BundleId              = 2,
		AttributeId_AppVersion            = 3,
		AttributeId_Opaque                = 4,
		AttributeId_Sha1Hash              = 5,
		AttributeId_ReceiptCreationDate   = 12,
		AttributeId_OriginalAppVersion    = 19,
		AttributeId_ReceiptExpirationDate = 21,
	};

	AppStoreReceipt* m_receipt;
	AttributeId m_attributeId;
	sl::String* m_attributeString;

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
