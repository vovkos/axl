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

#include "axl_re_Pch.h"
#include "axl_sl_Array.h"

#include <re2/sm.h>

//..............................................................................

// convenient AXL <-> RE2 string conversions

enum ToAxl {
	toAxl
};

enum ToRe2 {
	toRe2
};

inline
re2::StringPiece
operator >> (const axl::sl::StringRef& src, ToRe2) {
	return re2::StringPiece(src.cp(), src.getLength());
}

inline
axl::sl::StringRef
operator >> (const re2::StringPiece& src, ToAxl) {
	return axl::sl::StringRef(src.data(), src.length());
}

inline
axl::sl::StringRef
operator >> (const std::string& src, ToAxl) {
	return axl::sl::StringRef(src.data(), src.length());
}

//..............................................................................
