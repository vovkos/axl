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

#include "axl_re_CharSet.h"

namespace axl {
namespace re {

//..............................................................................

class StdCharClassDigit {
public:
	void
	operator () (CharSet* set) {
		set->add('0', '9');
	}
};

class StdCharClassHex {
public:
	void
	operator () (CharSet* set) {
		set->add('0', '9');
		set->add('a', 'f');
		set->add('A', 'F');
	}
};

class StdCharClassWord {
public:
	void
	operator () (CharSet* set) {
		set->add('a', 'z');
		set->add('A', 'Z');
		set->add('0', '9');
		set->add('_');
	}
};

class StdCharClassSpace {
public:
	void
	operator () (CharSet* set) {
		set->add(' ');
		set->add('\t');
		set->add('\r');
		set->add('\n');
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class StdCharClassNegation {
protected:
	class InvertedSet: public CharSet {
	public:
		InvertedSet() {
			T()(this);
			invert();
		}
	};

public:
	void
	operator () (CharSet* set) {
		set->add(*sl::getSingleton<InvertedSet>());
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef StdCharClassNegation<StdCharClassDigit> StdCharClassNonDigit;
typedef StdCharClassNegation<StdCharClassHex> StdCharClassNonHex;
typedef StdCharClassNegation<StdCharClassWord> StdCharClassNonWord;
typedef StdCharClassNegation<StdCharClassSpace> StdCharClassNonSpace;

//..............................................................................

} // namespace re
} // namespace axl
