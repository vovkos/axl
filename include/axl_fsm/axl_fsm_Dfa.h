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

#define _AXL_FSM_DFA_H

#include "axl_fsm_Nfa.h"

namespace axl {
namespace fsm {

struct DfaState;

//..............................................................................

struct DfaTransition: sl::ListLink
{
	MatchCondition m_matchCondition;
	DfaState* m_outState;
};

//..............................................................................

struct DfaState: sl::ListLink
{
	bool m_isAccept;
	uint_t m_id;
	uint_t m_acceptNfaStateId;
	void* m_acceptContext;

	sl::List<DfaTransition> m_transitionList;

	NfaStateSet m_nfaStateSet;
	sl::BitMap m_openCaptureIdSet;
	sl::BitMap m_closeCaptureIdSet;

	DfaState();

	bool
	addNfaState(NfaState* state);

	void
	makeEpsilonClosure();
};

//..............................................................................

} // namespace fsm
} // namespace axl
