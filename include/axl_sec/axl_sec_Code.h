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

#define _AXL_SEC_CODE_H

#include "axl_sec_Pch.h"
#include "axl_cf_Type.h"

namespace axl {
namespace sec {

//..............................................................................

class Code: public cf::TypeBase<SecCodeRef> {
public:
	Code() {}

	Code(const Code& src):
		cf::TypeBase<SecCodeRef>(src) {}

#if (_AXL_CPP_HAS_RVALUE_REF)
	Code(Code&& src):
		cf::TypeBase<SecCodeRef>(std::move(src)) {}
#endif

	Code(
		SecCodeRef p,
		bool isAttach = false
	):
		cf::TypeBase<SecCodeRef>(p, isAttach) {}
};

//..............................................................................

} // namespace sec
} // namespace axl
