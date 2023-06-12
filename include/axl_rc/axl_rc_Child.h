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

#define _AXL_RC_CHILD_H

#include "axl_rc_RefCount.h"

namespace axl {
namespace rc {

//..............................................................................

// creatable child ref-counted object

template <
	typename T,
	size_t extra = 0
>
class Child {
	AXL_DISABLE_COPY(Child)

protected:
	union {
		size_t m_parentOffset;
		uint64_t _m_align;
	};

	char m_buffer[sizeof(T) + extra];

public:
	Child(RefCount* parent) {
		m_parentOffset = m_buffer - (char*)parent;
		AXL_RC_NEW_INPLACE(T, m_buffer, weakReleaseParent);
		parent->addWeakRef();
		p()->addRef();
	}

	~Child() {
		p()->release();
	}

	operator T* () {
		return p();
	}

	T*
	operator & () {
		return p();
	}

	T*
	operator -> () {
		return p();
	}

	T* p() {
		return (T*)m_buffer;
	}

protected:
	static void weakReleaseParent(void* p) {
		size_t parentOffset = *(size_t*)((uint64_t*)p - 1);
		((RefCount*)((char*)p - parentOffset))->weakRelease();
	}
};

//..............................................................................

} // namespace rc
} // namespace axl
