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

#define _AXL_CRY_DH_H

#include "axl_cry_Bio.h"
#include "axl_cry_CryptoError.h"

namespace axl {
namespace cry {

//..............................................................................

class FreeDh {
public:
	void
	operator () (DH* h) {
		DH_free(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Dh: public sl::Handle<DH*, FreeDh> {
public:
	Dh() {}

	bool
	create();

	bool
	create1024x160();

	bool
	create2048x224();

	bool
	create2048x256();

	size_t
	getSize() {
		return DH_size(m_h);
	}

	bool
	readParameters(BIO* bio);

	bool
	readParameters(
		const void* pem,
		size_t size
	) {
		Bio bio;
		return bio.createMemBuf(pem, size) && readParameters(bio);
	}
};

//..............................................................................

} // namespace cry
} // namespace axl
