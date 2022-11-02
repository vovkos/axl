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

#define _AXL_IO_SHMTWRITER_H

#include "axl_io_ShmtBase.h"

namespace axl {
namespace io {

//..............................................................................

class ShmtWriter: public ShmtBase {
protected:
	size_t m_sizeLimitHint;
	sys::Lock m_writeLock; // make write operations atomic

public:
	ShmtWriter() {
		m_sizeLimitHint = ShmtConst_DefSizeLimitHint;
	}

	bool
	open(
		const sl::StringRef& fileName,
		const sl::StringRef& readEventName,
		const sl::StringRef& writeEventName,
		uint_t flags,
		size_t sizeLimitHint = ShmtConst_DefSizeLimitHint
	);

	bool
	attach(
		File::Handle fileHandle,
		const sl::StringRef& readEventName,
		const sl::StringRef& writeEventName,
		uint_t flags,
		size_t sizeLimitHint = ShmtConst_DefSizeLimitHint
	);

	size_t
	write(
		const void* p,
		size_t size
	) {
		return write(&p, &size, 1);
	}

	size_t
	write(
		const void* const* blockArray,
		const size_t* sizeArray,
		size_t count
	);

protected:
	static
	void
	copyWriteChain(
		void* dst,
		const void* const* blockArray,
		const size_t* sizeArray,
		size_t count
	);
};
//..............................................................................

} // namespace io
} // namespace axl
