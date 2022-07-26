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

#define _AXL_LEX_RAGEL_H

#include "axl_lex_Lexer.h"

namespace axl {
namespace lex {

//..............................................................................

class RagelRoot {
protected:
	sl::Array<int> m_stack; // stack of states

	// ragel interface variables

	int* stack;
	int act;
	int cs;
	int top;

public:
	RagelRoot();

protected:
	int*
	prePush();

	void
	postPop();

	// to be implemented:

	// void
	// init () {
	// }

	// void
	// exec () {
	// }
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
RagelRoot::RagelRoot() {
	stack = NULL;
	act = 0;
	cs = 0;
	top = 0;
}

inline
int*
RagelRoot::prePush() {
	size_t count = m_stack.getCount();
	m_stack.setCount(count + 1);
	stack = m_stack;
	return stack;
}

inline
void
RagelRoot::postPop() {
	size_t count = m_stack.getCount();
	if (count)
		m_stack.setCount(count - 1);
}

//..............................................................................

template <typename T>
class RagelBase: public RagelRoot {
public:
	typedef T C;
	typedef typename sl::StringBase<T>::Encoding Encoding;

protected:
	T* m_begin;

	// ragel interface variables

	T* p;
	T* pe;
	T* eof;
	T* ts;
	T* te;

public:
	RagelBase() {
		clear();
	}

	void
	setSource(
		const sl::StringRefBase<T>& source,
		bool isBomNeeded = false
	) {
		setSourceRaw(source);

		if (!isBomNeeded) {
			size_t bomLength = Encoding::getBomLength();
			if (source.getLength() >= bomLength && memcmp(p, Encoding::getBom(), bomLength) == 0)
				p += bomLength;
		}
	}

	void
	setSourceRaw(const sl::StringRefBase<T>& source) {
		m_begin = p = (T*)source.cp();
		pe = eof = (T*)source.getEnd();
	}

protected:
	void
	stop() {
		pe = p + 1;
	}

	void
	clear() {
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

	// to be implemented:

	// void
	// init () {
	// }

	// void
	// exec () {
	// }
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef RagelBase<char>    Ragel;
typedef RagelBase<wchar_t> Ragel_w;
typedef RagelBase<utf8_t>  Ragel_utf8;
typedef RagelBase<utf16_t> Ragel_utf16;
typedef RagelBase<utf32_t> Ragel_utf32;

//..............................................................................

} // namespace lex
} // namespace axl
