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

#define _AXL_CRY_ECGROUP_H

#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//..............................................................................

class FreeEcGroup {
public:
	void
	operator () (EC_GROUP* h) {
		EC_GROUP_free(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class EcGroup: public sl::Handle<EC_GROUP*, FreeEcGroup> {
public:
	EcGroup() {}

	EcGroup(uint_t curveId) {
		create(curveId);
	}

	EcGroup(EC_GROUP* h):
		sl::Handle<EC_GROUP*, FreeEcGroup>(h) {}

	bool
	create(uint_t curveId);

	bool
	createCopy(const EC_GROUP* src);

	uint_t
	getCurveId() const {
		ASSERT(m_h);
		return EC_GROUP_get_curve_name(m_h);
	}

	const char*
	getCurveName() const {
		return getCurveName(getCurveId());
	}

	size_t
	getDegree() const { // in bits
		ASSERT(m_h);
		return EC_GROUP_get_degree(m_h);
	}

	static
	const char*
	getCurveName(uint_t curveId) { // the NIST name (P-256) if any, the short name (prime256v1, X25519) otherwise
		const char* name = EC_curve_nid2nist(curveId);
		return name ? name : OBJ_nid2sn(curveId);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
EcGroup::create(uint_t curveId) {
	close();

	m_h = EC_GROUP_new_by_curve_name(curveId);
	return completeWithLastCryptoError(m_h != NULL);
}

inline
bool
EcGroup::createCopy(const EC_GROUP* src) {
	close();

	m_h = EC_GROUP_dup(src);
	return completeWithLastCryptoError(m_h != NULL);
}

//..............................................................................

} // namespace cry
} // namespace axl
