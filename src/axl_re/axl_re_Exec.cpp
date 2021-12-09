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
	m_p = NULL;
	m_matchEnd = NULL;
	m_matchEndOffset = -1;
	m_matchAcceptId = -1;
}

void
ExecEngine::reset(size_t offset) {
	m_decoderState = 0;
	m_matchEnd = NULL;
	m_matchEndOffset = -1;
	m_matchAcceptId = -1;
}

void
ExecEngine::copy(const ExecEngine* src) {
	m_decoderState = src->m_decoderState;
	m_p = src->m_p;
	m_matchEnd = src->m_matchEnd;
	m_matchEndOffset = src->m_matchEndOffset;
	m_matchAcceptId = src->m_matchAcceptId;
}

//..............................................................................

} // namespace re
} // namespace axl
