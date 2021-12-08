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
#include "axl_re_ExecNfaVm.h"
#include "axl_enc_Utf16s.h"
#include "axl_enc_Utf32s.h"

namespace axl {
namespace re {

//..............................................................................

ExecEngine*
ExecNfaVm::clone(StateImpl* parent) {
	ExecNfaVm* exec = AXL_MEM_NEW_ARGS(ExecNfaVm, (parent));
	return exec;
}

void
ExecNfaVm::reset(size_t offset) {
}

bool
ExecNfaVm::exec(
	const void* p,
	size_t size
) {
	return true;
}

bool
ExecNfaVm::eof() {
	return true;
}

//..............................................................................

ExecEngine*
createExecNfaVm(
	StateImpl* parent,
	enc::CharCodecKind codecKind
) {
/*	switch (codecKind) {
	case enc::CharCodecKind_Ascii:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Ascii>, (parent));
	case enc::CharCodecKind_Utf8:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Utf8>, (parent));
	case enc::CharCodecKind_Utf16:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Utf16s>, (parent));
	case enc::CharCodecKind_Utf16_be:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Utf16s_be>, (parent));
	case enc::CharCodecKind_Utf32:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Utf32s>, (parent));
	case enc::CharCodecKind_Utf32_be:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Utf32s_be>, (parent));
	default:
		ASSERT(false);
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Ascii>, (parent));
	} */

	return NULL;
}

//..............................................................................

} // namespace re
} // namespace axl
