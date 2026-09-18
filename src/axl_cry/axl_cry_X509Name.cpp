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
#include "axl_cry_X509Name.h"
#include "axl_cry_Bio.h"

namespace axl {
namespace cry {

//..............................................................................

bool
X509Name::addEntry(
	const sl::StringRef& field,
	const sl::StringRef& value
) {
	ASSERT(m_h);

	int result = ::X509_NAME_add_entry_by_txt(
		m_h,
		field.sz(),
		MBSTRING_UTF8,
		(const uchar_t*)value.cp(),
		(int)value.getLength(),
		-1, // append
		0   // not multi-valued
	);

	return completeWithLastCryptoError(result);
}

size_t
X509Name::getString(sl::String* string) const {
	ASSERT(m_h);

	Bio bio;
	bool result = bio.createMem();
	if (!result)
		return -1;

	// RFC 2253 -- X509_NAME_oneline's format is legacy and ambiguous

	if (::X509_NAME_print_ex(bio, m_h, 0, XN_FLAG_RFC2253) < 0)
		return failWithLastCryptoError<size_t>(-1);

	BUF_MEM* mem = bio.getBufMem();
	return string->copy(mem->data, mem->length);
}

//

//..............................................................................

} // namespace cry
} // namespace axl
