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
#include "axl_re_State.h"

namespace axl {
namespace re {

//..............................................................................

ExecEngine::ExecEngine(
	ExecEngineKind engineKind,
	StateImpl* parent
) {
	m_engineKind = engineKind;
	m_parent = parent;
	m_lastExecData = NULL;
	m_lastExecOffset = 0;
	m_lastExecEndOffset = 0;
	m_prevChar = 0;
	m_prevCharFlags = 0;
	m_offset = parent->m_init.m_offset;
	m_decoderState = parent->m_init.m_decoderState;
	m_execFlags = parent->m_init.m_execFlags; // cache to save one indirection
	m_execResult = ExecResult_Continue;
}

void
ExecEngine::reset(size_t offset) {
	m_offset = offset;
	m_decoderState = 0;
	m_prevCharFlags = Anchor_BeginText | Anchor_BeginLine | Anchor_WordBoundary;
	m_execResult = ExecResult_Continue;
}

void
ExecEngine::copy(const ExecEngine* src) {
	m_lastExecData = src->m_lastExecData;
	m_lastExecOffset = src->m_lastExecOffset;
	m_lastExecEndOffset = src->m_lastExecEndOffset;
	m_offset = src->m_offset;
	m_decoderState = src->m_decoderState;
	m_prevChar = src->m_prevChar;
	m_prevCharFlags = src->m_prevCharFlags;
}

//..............................................................................

} // namespace re
} // namespace axl
