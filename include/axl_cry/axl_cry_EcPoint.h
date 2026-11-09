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

#define _AXL_CRY_ECPOINT_H

#include "axl_cry_BigNum.h"

namespace axl {
namespace cry {

//..............................................................................

class FreeEcPoint {
public:
	void
	operator () (EC_POINT* h) {
		EC_POINT_free(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class EcPoint: public sl::Handle<EC_POINT*, FreeEcPoint> {
public:
	EcPoint(const EC_GROUP* group) {
		create(group);
	}

	EcPoint(EC_POINT* h):
		sl::Handle<EC_POINT*, FreeEcPoint>(h) {}

	bool
	create(const EC_GROUP* group);

	bool
	createCopy(
		const EC_POINT* src,
		const EC_GROUP* group
	);

	bool
	copy(const EC_POINT* src) {
		int result = EC_POINT_copy(m_h, src);
		return completeWithLastCryptoError(result);
	}

	bool
	getData(
		sl::Array<char>* data,
		const EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
	) const;

	sl::Array<char>
	getData(
		const EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
	) const {
		sl::Array<char> data;
		getData(&data, group, form, ctx);
		return data;
	}

	bool
	setData(
		const EC_GROUP* group,
		const void* p,
		size_t size,
		BN_CTX* ctx = NULL
	) {
		int result = EC_POINT_oct2point(group, m_h, (const uchar_t*)p, size, ctx);
		return completeWithLastCryptoError(result);
	}

	bool
	getBigNum(
		const EC_GROUP* group,
		BIGNUM* bigNum,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
	) const {
		BIGNUM* result = EC_POINT_point2bn(group, m_h, form, bigNum, ctx);
		return completeWithLastCryptoError(result != NULL);
	}

	bool
	setBigNum(
		const EC_GROUP* group,
		const BIGNUM* bigNum,
		BN_CTX* ctx = NULL
	) {
		EC_POINT* result = EC_POINT_bn2point(group, bigNum, m_h, ctx);
		return completeWithLastCryptoError(result != NULL);
	}

	bool
	getDecString(
		sl::String* string,
		const EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
	) const {
		BigNum bigNum;
		return getBigNum(group, bigNum, form, ctx) && bigNum.getDecString(string);
	}

	sl::String
	getDecString(
		const EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
	) const {
		sl::String string;
		getDecString(&string, group, form, ctx);
		return string;
	}

	bool
	setDecString(
		const EC_GROUP* group,
		const sl::StringRef& string,
		BN_CTX* ctx = NULL
	) {
		BigNum bigNum;
		return bigNum.setDecString(string) && setBigNum(group, bigNum, ctx);
	}

	bool
	getHexString(
		sl::String* string,
		const EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
	) const;

	sl::String
	getHexString(
		const EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
	) const {
		sl::String string;
		getHexString(&string, group, form, ctx);
		return string;
	}

	bool
	setHexString(
		const EC_GROUP* group,
		const sl::StringRef& string,
		BN_CTX* ctx = NULL
	) {
		EC_POINT* result = EC_POINT_hex2point(group, string.sz(), m_h, ctx);
		return completeWithLastCryptoError(result != NULL);
	}

	bool
	isAtInfinity(const EC_GROUP* group) const {
		return EC_POINT_is_at_infinity(group, m_h) == 1;
	}

	bool
	isOnCurve(
		const EC_GROUP* group,
		BN_CTX* ctx = NULL
	) const {
		return EC_POINT_is_on_curve(group, m_h, ctx) == 1;
	}

	bool
	isEqual(
		const EC_GROUP* group,
		const EC_POINT* op,
		BN_CTX* ctx = NULL
	) const {
		return EC_POINT_cmp(group, m_h, op, ctx) == 1;
	}

	bool
	add(
		const EC_GROUP* group,
		const EC_POINT* op1,
		const EC_POINT* op2,
		BN_CTX* ctx = NULL
	) {
		int result = EC_POINT_add(group, m_h, op1, op2, ctx);
		return completeWithLastCryptoError(result);
	}

	bool
	mul(
		const EC_GROUP* group,
		const BIGNUM* op1,
		const EC_POINT* op2,
		const BIGNUM* op3,
		BN_CTX* ctx = NULL
	) {
		int result = EC_POINT_mul(group, m_h, op1, op2, op3, ctx);
		return completeWithLastCryptoError(result);
	}

	bool
	mul(
		const EC_GROUP* group,
		const BIGNUM* op,
		BN_CTX* ctx = NULL
	) {
		int result = EC_POINT_mul(group, m_h, op, NULL, NULL, ctx);
		return completeWithLastCryptoError(result);
	}

	bool
	dbl(
		const EC_GROUP* group,
		const EC_POINT* op,
		BN_CTX* ctx = NULL
	) {
		int result = EC_POINT_dbl(group, m_h, op, ctx);
		return completeWithLastCryptoError(result);
	}

	bool
	invert(
		const EC_GROUP* group,
		BN_CTX* ctx = NULL
	) {
		int result = EC_POINT_invert(group, m_h, ctx);
		return completeWithLastCryptoError(result);
	}

	bool
	makeAffine(
		const EC_GROUP* group,
		BN_CTX* ctx = NULL
	) {
		int result = EC_POINT_make_affine(group, m_h, ctx);
		return completeWithLastCryptoError(result);
	}
};

//..............................................................................

} // namespace cry
} // namespace axl
