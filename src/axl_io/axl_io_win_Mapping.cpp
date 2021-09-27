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
#include "axl_io_win_Mapping.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

bool
Mapping::create(
	HANDLE hFile,
	SECURITY_ATTRIBUTES* secAttr,
	uint_t pageProtection,
	uint64_t _MaxSize,
	const sl::StringRef_w& name
) {
	close();

	ULARGE_INTEGER maxSize;
	maxSize.QuadPart = _MaxSize;

	m_h = ::CreateFileMappingW(
		hFile,
		secAttr,
		pageProtection,
		maxSize.HighPart,
		maxSize.LowPart,
		name.sz()
	);

	return err::complete(m_h != NULL);
}

bool
Mapping::open(
	uint_t access,
	bool doInheritHandle,
	const sl::StringRef_w& name
) {
	close();

	m_h = ::OpenFileMappingW(access, doInheritHandle, name.sz());
	return err::complete(m_h != NULL);
}

//..............................................................................

void*
MappedView::view(
	HANDLE hMapping,
	uint_t access,
	uint64_t _offset,
	uint32_t size
) {
	close();

	ULARGE_INTEGER offset;
	offset.QuadPart = _offset;

	m_h = ::MapViewOfFile(hMapping, access, offset.HighPart, offset.LowPart, size);
	if (!m_h) {
		err::failWithLastSystemError();
		return NULL;
	}

	return m_h;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
