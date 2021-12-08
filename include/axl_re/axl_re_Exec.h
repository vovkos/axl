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

#define _AXL_RE_EXEC_H

#include "axl_re_Match.h"

namespace axl {
namespace re {

//..............................................................................

enum RegexExecFlag {
	RegexExecFlag_Dfa             = 0x0001, // default
	RegexExecFlag_NfaAuto         = 0x0002, // auto-select threading or single-pass NFA
	RegexExecFlag_NfaVm           = 0x0004, // force threading NFA
	RegexExecFlag_Nfa             = RegexExecFlag_NfaAuto,
	RegexExecFlag_EngineMask      = RegexExecFlag_Dfa | RegexExecFlag_NfaAuto | RegexExecFlag_NfaVm,

	RegexExecFlag_Stream          = 0x0010, // can feed data chunk-by-chunk
	RegexExecFlag_DisableCapture  = 0x0020, // capture by default for NFA-based engines
	RegexExecFlag_Multiline       = 0x0040, // ^ and $ match newline
	RegexExecFlag_AnchorDataBegin = 0x0100, // match must start on the first byte of data
	RegexExecFlag_AnchorDataEnd   = 0x0200, // match must end on the last byte of data
	RegexExecFlag_ExactMatch      = RegexExecFlag_AnchorDataBegin | RegexExecFlag_AnchorDataEnd,
};

//..............................................................................

class ExecEngine {
protected:
	enum ExecResult {
		ExecResult_Undefined = -1,
		ExecResult_False     = false,
		ExecResult_True      = true,
	};

protected:
	StateImpl* m_parent;
	ExecResult m_execResult;
	enc::DecoderState m_decoderState;

public:
	ExecEngine(StateImpl* parent) {
		m_parent = parent;
		m_execResult = ExecResult_Undefined;
		m_decoderState = 0;
	}

	virtual
	~ExecEngine() {}

	virtual
	ExecEngine*
	clone(StateImpl* parent) = 0;

	virtual
	void
	reset(size_t offset) = 0;

	virtual
	bool
	exec(
		const void* p,
		size_t size
	) = 0;

	virtual
	bool
	eof() = 0;
};

//..............................................................................

#define _AXL_RE_TRACE_CAPTURE 0

#if (_AXL_RE_TRACE_CAPTURE)
#	define AXL_RE_TRACE_CAPTURE AXL_TRACE
#else
#	define AXL_RE_TRACE_CAPTURE (void)
#endif

} // namespace re
} // namespace axl
