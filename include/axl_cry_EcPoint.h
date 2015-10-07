// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_CRY_ECPOINT_H

#include "axl_cry_BigNum.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//.............................................................................

class FreeEcPoint
{
public:
	void 
	operator () (EC_POINT* h)
	{
		EC_POINT_free (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class EcPoint: public sl::Handle <EC_POINT*, FreeEcPoint>
{
public:
	EcPoint (EC_GROUP* group)
	{
		create (group);
	}

	EcPoint (EC_POINT* h):
		sl::Handle <EC_POINT*, FreeEcPoint> (h)
	{
	}

	bool
	create (EC_GROUP* group);

	bool
	createCopy (
		EC_POINT* src,
		EC_GROUP* group
		);

	bool
	copy (EC_POINT* src)
	{
		int result = EC_POINT_copy (m_h, src);
		return completeWithLastCryptoError (result);
	}

	bool
	getData (
		sl::Array <char>* data,
		EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		);

	sl::Array <char>
	getData (
		EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		)
	{
		sl::Array <char> data;
		getData (&data, group, form, ctx);
		return data;
	}

	bool
	setData (
		EC_GROUP* group,
		const void* p,
		size_t size,
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_oct2point (group, m_h, (const uchar_t*) p, size, ctx);
		return completeWithLastCryptoError (result);
	}

	bool
	getBigNum (
		EC_GROUP* group,
		BIGNUM* bigNum,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		)
	{
		BIGNUM* result = EC_POINT_point2bn (group, m_h, form, bigNum, ctx);
		return completeWithLastCryptoError (result != NULL);
	}

	bool
	setBigNum (
		EC_GROUP* group,
		const BIGNUM* bigNum,
		BN_CTX* ctx = NULL
		)
	{
		EC_POINT* result = EC_POINT_bn2point (group, bigNum, m_h, ctx);
		return completeWithLastCryptoError (result != NULL);
	}

	bool
	getDecString (
		sl::String* string,
		EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		)
	{
		BigNum bigNum;
		return getBigNum (group, bigNum, form, ctx) && bigNum.getDecString (string);
	}

	sl::String
	getDecString (
		EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		)
	{
		sl::String string;
		getDecString (&string, group, form, ctx);
		return string;
	}

	bool
	setDecString (
		EC_GROUP* group,
		const char* string,
		BN_CTX* ctx = NULL
		)
	{
		BigNum bigNum;
		return bigNum.setDecString (string) && setBigNum (group, bigNum, ctx);
	}

	bool
	getHexString (
		sl::String* string,
		EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		);

	sl::String
	getHexString (
		EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		)
	{
		sl::String string;
		getHexString (&string, group, form, ctx);
		return string;
	}

	bool
	setHexString (
		EC_GROUP* group,
		const char* string,
		BN_CTX* ctx = NULL
		)
	{
		EC_POINT* result = EC_POINT_hex2point (group, string, m_h, ctx);
		return completeWithLastCryptoError (result != NULL);
	}

	bool
	isAtInfinity (EC_GROUP* group)
	{
		return EC_POINT_is_at_infinity (group, m_h) == 1;
	}

	bool
	isOnCurve (
		EC_GROUP* group,
		BN_CTX* ctx = NULL
		)
	{
		return EC_POINT_is_on_curve (group, m_h, ctx) == 1;
	}

	bool
	isEqual (
		EC_GROUP* group,
		EC_POINT* op, 
		BN_CTX* ctx = NULL
		)
	{
		return EC_POINT_cmp (group, m_h, op, ctx) == 1;
	}

	bool
	add (
		EC_GROUP* group,
		EC_POINT* op1, 
		EC_POINT* op2, 
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_add (group, m_h, op1, op2, ctx);
		return completeWithLastCryptoError (result);
	}

	bool
	mul (
		EC_GROUP* group,
		BIGNUM* op1, 
		EC_POINT* op2, 
		BIGNUM* op3, 
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_mul (group, m_h, op1, op2, op3, ctx);
		return completeWithLastCryptoError (result);
	}

	bool
	mul (
		EC_GROUP* group,
		BIGNUM* op, 
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_mul (group, m_h, op, NULL, NULL, ctx);
		return completeWithLastCryptoError (result);
	}

	bool
	dbl (
		EC_GROUP* group,
		EC_POINT* op, 
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_dbl (group, m_h, op, ctx);
		return completeWithLastCryptoError (result);
	}

	bool
	invert (
		EC_GROUP* group,
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_invert (group, m_h, ctx);
		return completeWithLastCryptoError (result);
	}

	bool
	makeAffine (
		EC_GROUP* group,
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_make_affine (group, m_h, ctx);
		return completeWithLastCryptoError (result);
	}
};

//.............................................................................

} // namespace cry
} // namespace axl
