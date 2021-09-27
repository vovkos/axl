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
#include "axl_sys_win_Library.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
Library::loadLibrary(const sl::StringRef_w& fileName) {
	close();

	m_h = ::LoadLibraryW(fileName.sz());
	return err::complete(m_h != NULL);
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
