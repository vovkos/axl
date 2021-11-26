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
	RegexExecFlag_NfaThreading    = 0x0001,
	RegexExecFlag_NfaSinglePass   = 0x0002,
	RegexExecFlag_Dfa             = 0x0004,
	RegexExecFlag_NfaVm           = RegexExecFlag_NfaThreading,
	RegexExecFlag_NfaSp           = RegexExecFlag_NfaSinglePass,
	RegexExecFlag_Nfa             = RegexExecFlag_NfaVm, // default
	RegexExecFlag_EngineMask      = RegexExecFlag_NfaVm | RegexExecFlag_NfaSp | RegexExecFlag_Dfa,

	RegexExecFlag_Stream          = 0x0010,
	RegexExecFlag_DisableCapture  = 0x0020, // capture by default for all NFA-based engines
	RegexExecFlag_Multiline       = 0x0040,
	RegexExecFlag_AnchorDataBegin = 0x0100,
	RegexExecFlag_AnchorDataEnd   = 0x0200,
	RegexExecFlag_ExactMatch      = RegexExecFlag_AnchorDataBegin | RegexExecFlag_AnchorDataEnd,
};

//..............................................................................

class ExecEngine {
protected:
	enum {
		DecodeBufferSize = 64
	};

protected:
	StateImpl* m_parent;

public:
	ExecEngine(StateImpl* parent) {
		m_parent = parent;
	}

	virtual
	~ExecEngine() {}

	virtual
	void
	reset() = 0;

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
