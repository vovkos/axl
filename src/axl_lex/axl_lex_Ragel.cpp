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
#include "axl_lex_Ragel.h"

namespace axl {
namespace lex {

//..............................................................................

void
Ragel::setSource(
	const sl::StringRef& source,
	bool isBomNeeded
) {
	m_begin = p = (char*)source.cp();
	pe = eof = (char*)source.getEnd();

	if (!isBomNeeded) {
		size_t bomLength = enc::Utf8::getBomLength();
		if (source.getLength() >= bomLength && memcmp(p, enc::Utf8::getBom(), bomLength) == 0)
			p += bomLength;
	}
}

int*
Ragel::prePush() {
	size_t count = m_stack.getCount();
	m_stack.setCount(count + 1);
	stack = m_stack;
	return stack;
}

void
Ragel::postPop() {
	size_t count = m_stack.getCount();
	if (count)
		m_stack.setCount(count - 1);
}

void
Ragel::clear() {
	m_stack.clear();
	m_begin = NULL;

	p = NULL;
	pe = NULL;
	eof = NULL;
	ts = NULL;
	te = NULL;
	act = 0;
	cs = 0;
	top = 0;
	stack = NULL;
}

//..............................................................................

} // namespace lex
} // namespace axl
