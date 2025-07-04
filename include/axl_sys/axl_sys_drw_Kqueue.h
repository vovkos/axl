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

#define _AXL_SYS_DRW_KQUEUE_H

#include "axl_io_psx_File.h"

namespace axl {
namespace sys {
namespace drw {

//..............................................................................

class Kqueue: public sl::Handle<int, io::psx::CloseFile, sl::MinusOne<int> > {
public:
	Kqueue() {
		m_h = kqueue();
	}

	bool
	change(
		const struct kevent* changeList,
		size_t changeCount = 1
	) const {
		return wait(changeList, changeCount, NULL, 0) != -1;
	}

	size_t
	wait(
		struct kevent* eventList,
		size_t eventCount,
		const timespec* timeout = NULL
	) const {
		return wait(NULL, 0, eventList, eventCount, timeout);
	}

	size_t
	wait(
		const struct kevent* changeList,
		size_t changeCount,
		struct kevent* eventList,
		size_t eventCount,
		const timespec* timeout = NULL
	) const {
		int result = kevent(m_h, changeList, changeCount, eventList, eventCount, timeout);
		return result == -1 ? err::failWithLastSystemError<size_t>(-1) : result;
	}
};

//..............................................................................

} // namespace drw
} // namespace sys
} // namespace axl
