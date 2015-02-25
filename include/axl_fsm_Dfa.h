// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_FSM_DFA_H

#include "axl_fsm_Nfa.h"

namespace axl {
namespace fsm {

struct DfaState;

//.............................................................................

struct DfaTransition: rtl::ListLink
{
	MatchCondition m_matchCondition;
	DfaState* m_outState;
};

//.............................................................................

struct DfaState: rtl::ListLink
{
	bool m_isAccept;
	uint_t m_id;
	void* m_acceptContext;

	rtl::StdList <DfaTransition> m_transitionList;	

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
