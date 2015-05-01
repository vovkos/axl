// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_FSM_NFA_H

#include "axl_rtl_List.h"
#include "axl_rtl_Array.h"
#include "axl_rtl_BitMap.h"
#include "axl_rtl_HashTable.h"

namespace axl {
namespace fsm {

//.............................................................................

enum PseudoChar
{
	PseudoChar_StartOfLine = 256,
	PseudoChar_EndOfLine,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum 
{
	TransitionTableCharCount = 256 + 2
};

//.............................................................................

enum MatchConditionKind
{
	MatchConditionKind_Undefined,
	MatchConditionKind_Char,
	MatchConditionKind_CharSet,
	MatchConditionKind_Any,
};
	
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct MatchCondition
{
	MatchConditionKind m_conditionKind;
	rtl::BitMap m_charSet;
	uint_t m_char;
	
	MatchCondition ();
	
	void
	copy (const MatchCondition& src);	

	void
	addChar (uchar_t c);
};

//.............................................................................

enum NfaStateFlag
{
	NfaStateFlag_Match        = 0x0001,
	NfaStateFlag_EpsilonLink  = 0x0002,
	NfaStateFlag_Accept       = 0x0004,
	NfaStateFlag_OpenCapture  = 0x0010,
	NfaStateFlag_CloseCapture = 0x0020,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct NfaState: rtl::ListLink
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

//.............................................................................

struct NfaStateSet
{
	rtl::Array <NfaState*> m_stateArray;
	rtl::BitMap m_stateSet;

	bool
	addState (NfaState* state);	

	int 
	cmp (const NfaStateSet& set) const
	{
		return m_stateSet.cmp (set.m_stateSet);
	}
	
	intptr_t
	getHash () const
	{
		return m_stateSet.getHash ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class NfaStateSetMap: public rtl::HashTableMap <
	NfaStateSet*, 
	T, 
	rtl::HashDuckType <NfaStateSet>, 
	rtl::CmpDuckType <NfaStateSet> 
	>
{
};

//.............................................................................

struct NfaTransition: rtl::ListLink 
{
	MatchCondition m_matchCondition;
	NfaStateSet m_outStateSet;
};

//.............................................................................

class NfaTransitionMgr
{
protected:
	rtl::StdList <NfaTransition> m_transitionList;
	NfaTransition* m_transitionMap [TransitionTableCharCount];

	// for UTF regexps m_transitionMap should be replaced with interval tree

public:
	void 
	clear ();
	
	rtl::ConstList <NfaTransition> 
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
		const rtl::BitMap* charSet, 
		NfaState* outState
		);
	
	void
	addMatchAnyTransition (NfaState* outState);
};

//.............................................................................

} // namespace fsm
} // namespace axl
