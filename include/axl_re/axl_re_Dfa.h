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

#define _AXL_RE_DFA_H

#include "axl_re_Nfa.h"

namespace axl {
namespace re {

struct DfaState;

//..............................................................................

enum DfaStateFlag {
	DfaStateFlag_Ready  = 0x01,
	DfaStateFlag_Accept = 0x02,
};

//..............................................................................

struct DfaTransition: sl::ListLink {
	FsmMatchCondition m_matchCondition;
	DfaState* m_nextState;
};

//..............................................................................

struct DfaState: sl::ListLink {
	uint_t m_id;
	uint_t m_flags;
	uint_t m_acceptNfaStateId;
	void* m_acceptContext;

	NfaStateSet m_nfaStateSet;
	sl::BitMap m_openCaptureIdSet;
	sl::BitMap m_closeCaptureIdSet;

	sl::List<DfaTransition> m_transitionList;

	DfaState();

	bool
	addNfaState(NfaState* state);

	void
	makeEpsilonClosure();
};

//..............................................................................

} // namespace re
} // namespace axl
