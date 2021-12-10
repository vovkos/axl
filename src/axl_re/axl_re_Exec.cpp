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
#include "axl_re_Exec.h"

namespace axl {
namespace re {

//..............................................................................

ExecEngine::ExecEngine(StateImpl* parent) {
	m_parent = parent;
	m_execResult = ExecResult_Undefined;
	m_decoderState = 0;
}

//..............................................................................

} // namespace re
} // namespace axl
