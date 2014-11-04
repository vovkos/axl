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

class EcPoint: public rtl::Handle <EC_POINT*, FreeEcPoint>
{
public:
	EcPoint ()
	{
	}

	EcPoint (EC_POINT* h):
		rtl::Handle <EC_POINT*, FreeEcPoint> (h)
	{
	}

	bool
	create (const EC_GROUP* group);

	bool
	createCopy (
		const EC_POINT* src,
		const EC_GROUP* group
		);

	bool
	copy (const EC_POINT* src)
	{
		int result = EC_POINT_copy (m_h, src);
		return completeWithLastCryptoError (result);
	}

	bool
	getData (
		rtl::Array <char>* data,
		const EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		);

	rtl::Array <char>
	getData (
		const EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		)
	{
		rtl::Array <char> data;
		getData (&data, group, form, ctx);
		return data;
	}

	bool
	setData (
		const EC_GROUP* group,
		const void* p,
		size_t size,
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_oct2point (group, m_h, (const uchar_t*) p, size, ctx);
		return completeWithLastCryptoError (result);
	}

	bool
	getHexString (
		rtl::String* string,
		const EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		);

	rtl::String
	getHexString (
		const EC_GROUP* group,
		point_conversion_form_t form = POINT_CONVERSION_COMPRESSED,
		BN_CTX* ctx = NULL
		)
	{
		rtl::String string;
		getHexString (&string, group, form, ctx);
		return string;
	}

	bool
	setHexString (
		const EC_GROUP* group,
		const char* string,
		BN_CTX* ctx = NULL
		)
	{
		EC_POINT* result = EC_POINT_hex2point (group, string, m_h, ctx);
		return completeWithLastCryptoError (result != NULL);
	}

	bool
	getBigNum (
		const EC_GROUP* group,
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
		const EC_GROUP* group,
		const BIGNUM* bigNum,
		BN_CTX* ctx = NULL
		)
	{
		EC_POINT* result = EC_POINT_bn2point (group, bigNum, m_h, ctx);
		return completeWithLastCryptoError (result != NULL);
	}

	bool
	isAtInfinity (const EC_GROUP* group)
	{
		return EC_POINT_is_at_infinity (group, m_h) == 1;
	}

	bool
	isOnCurve (
		const EC_GROUP* group,
		BN_CTX* ctx = NULL
		)
	{
		return EC_POINT_is_on_curve (group, m_h, ctx) == 1;
	}

	bool
	isEqual (
		const EC_GROUP* group,
		const EC_POINT* op, 
		BN_CTX* ctx = NULL
		)
	{
		return EC_POINT_cmp (group, m_h, op, ctx) == 1;
	}

	bool
	add (
		const EC_GROUP* group,
		const EC_POINT* op1, 
		const EC_POINT* op2, 
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_add (group, m_h, op1, op2, ctx);
		return completeWithLastCryptoError (result);
	}

	bool
	dbl (
		const EC_GROUP* group,
		const EC_POINT* op, 
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_dbl (group, m_h, op, ctx);
		return completeWithLastCryptoError (result);
	}

	bool
	invert (
		const EC_GROUP* group,
		BN_CTX* ctx = NULL
		)
	{
		int result = EC_POINT_invert (group, m_h, ctx);
		return completeWithLastCryptoError (result);
	}

	bool
	makeAffine (
		const EC_GROUP* group,
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
