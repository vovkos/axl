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

#define _AXL_ENC_UTFDFA_H

#include "axl_g_Pch.h"

namespace axl {
namespace enc {

//..............................................................................

// common fields and methods for UTF-8, UTF-16 DFAs (forward and backward)

template <
	typename T,
	typename B
>
class UtfDfa: public B {
protected:
	uint_t m_state;
	utf32_t m_cp;

public:
	UtfDfa() {
		m_cp = 0;
		m_state = T::State_Start;
	}

	static
	bool
	isError(uint_t state)  {
		return (state & T::State_ErrorBit) != 0;
	}

	static
	bool
	isReady(uint_t state)  {
		return state >= T::State_Ready;
	}

	bool
	isError() const {
		return isError(m_state);
	}

	bool
	isReady() const  {
		return isReady(m_state);
	}

	uint_t
	getState() const {
		return m_state;
	}

	utf32_t
	getCp() const {
		return m_cp;
	}

	void
	reset() {
		m_state = T::State_Start;
	}

	void
	load(uint32_t storage) {
		m_state = (storage & 0xff000000) >> 21;
		m_cp = storage & 0x00ffffff;
	}

	uint32_t
	save() const {
		return (m_state << 21) | (m_cp & 0x00ffffff);
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
