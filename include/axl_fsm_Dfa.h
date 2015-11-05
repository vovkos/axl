// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_FSM_DFA_H

#include "axl_fsm_Nfa.h"

namespace axl {
namespace fsm {

struct DfaState;

//.............................................................................

struct DfaTransition: sl::ListLink
{
	MatchCondition m_matchCondition;
	DfaState* m_outState;
};

//.............................................................................

struct DfaState: sl::ListLink
{
	bool m_isAccept;
	uint_t m_id;
	uint_t m_acceptNfaStateId;
	void* m_acceptContext;
	sl::Array <size_t> m_openCaptureIdArray;
	sl::Array <size_t> m_closeCaptureIdArray;

	sl::StdList <DfaTransition> m_transitionList;

	NfaStateSet m_nfaStateSet;

	DfaState ();

	bool
	addNfaState (NfaState* state);	

	void
	makeEpsilonClosure ();
};

//.............................................................................

} // namespace fsm
} // namespace axl
