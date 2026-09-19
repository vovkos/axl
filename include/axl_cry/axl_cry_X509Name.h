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

#define _AXL_CRY_X509NAME_H

#include "axl_cry_CryptoError.h"

namespace axl {

namespace cry {

//..............................................................................

class FreeX509Name {
public:
	void
	operator () (X509_NAME* name) {
		::X509_NAME_free(name);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class X509Name: public sl::Handle<X509_NAME*, FreeX509Name> {
public:
	X509Name() {}

	X509Name(X509_NAME* name) {
		attach(name);
	}

	bool
	create();

	size_t
	getEntryCount() const {
		ASSERT(m_h);
		return ::X509_NAME_entry_count(m_h);
	}

	bool
	addEntry(
		const sl::StringRef& field, // "CN", "O", "C", ...
		const sl::StringRef& value
	);

	size_t
	getString(sl::String* string) const;

	inline
	sl::String
	getString() const {
		sl::String string;
		getString(&string);
		return string;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
X509Name::create() {
	close();
	m_h = ::X509_NAME_new();
	return completeWithLastCryptoError(m_h != NULL);
}

//..............................................................................

} // namespace cry
} // namespace axl
