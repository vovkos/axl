// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_FSM_NFA_H

#include "axl_fsm_Pch.h"

namespace axl {
namespace fsm {

//..............................................................................

enum MatchConditionKind
{
	MatchConditionKind_Undefined,
	MatchConditionKind_Char,
	MatchConditionKind_CharSet,
	MatchConditionKind_Any,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct MatchCondition
{
	MatchConditionKind m_conditionKind;
	sl::BitMap m_charSet;
	uint_t m_char;

	MatchCondition ();

	void
	copy (const MatchCondition& src);

	void
	addChar (uchar_t c);
};

//..............................................................................

enum NfaStateFlag
{
	NfaStateFlag_Match        = 0x0001,
	NfaStateFlag_EpsilonLink  = 0x0002,
	NfaStateFlag_Accept       = 0x0004,
	NfaStateFlag_OpenCapture  = 0x0010,
	NfaStateFlag_CloseCapture = 0x0020,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct NfaState: sl::ListLink
{
	uint_t m_id;
	uint_t m_flags;
	void* m_acceptContext;
	size_t m_captureId;

	MatchCondition m_matchCondition;
	NfaState* m_outState;
	NfaState* m_outState2;

	NfaState ();

	void
	copy (const NfaState* srcState);

	void
	createEpsilonLink (NfaState* outState);

	void
	createEpsilonLink (
		NfaState* outState,
		NfaState* outState2
		);

	void
	createCharMatch (
		uint_t c,
		NfaState* outState
		);
};

//..............................................................................

struct NfaStateSet
{
	sl::Array <NfaState*> m_stateArray;
	sl::BitMap m_stateSet;

	bool
	addState (NfaState* state);

	int
	cmp (const NfaStateSet& set) const
	{
		return m_stateSet.cmp (set.m_stateSet);
	}

	bool
	isEqual (const NfaStateSet& set) const
	{
		return m_stateSet.isEqual (set.m_stateSet);
	}

	size_t
	hash () const
	{
		return m_stateSet.hash ();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class NfaStateSetMap: public sl::HashTableMap <
	NfaStateSet*,
	T,
	sl::HashDuckType <NfaStateSet>,
	sl::EqDuckType <NfaStateSet>
	>
{
};

//..............................................................................

struct NfaTransition: sl::ListLink
{
	MatchCondition m_matchCondition;
	NfaStateSet m_outStateSet;
};

//..............................................................................

class NfaTransitionMgr
{
protected:
	sl::StdList <NfaTransition> m_transitionList;
	NfaTransition* m_transitionMap [256];

	// for UTF regexps m_transitionMap should be replaced with interval tree

public:
	void
	clear ();

	sl::ConstList <NfaTransition>
	getTransitionList ()
	{
		return m_transitionList;
	}

	void
	addMatchState (NfaState* state);

	void
	finalize ();

protected:
	void
	addMatchCharTransition (
		uint_t c,
		NfaState* outState
		);

	void
	addMatchCharSetTransition (
		const sl::BitMap* charSet,
		NfaState* outState
		);

	void
	addMatchAnyTransition (NfaState* outState);
};

//..............................................................................

} // namespace fsm
} // namespace axl
