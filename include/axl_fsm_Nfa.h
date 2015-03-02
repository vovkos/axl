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
	char m_char;
	
	MatchCondition ();
	
	void
	copy (const MatchCondition& src);	

	void
	addChar (char c);
};
	
//.............................................................................

enum NfaStateFlag
{
	NfaStateFlag_Match        = 0x01,
	NfaStateFlag_EpsilonLink  = 0x02,
	NfaStateFlag_Accept       = 0x04,
	NfaStateFlag_OpenCapture  = 0x10,
	NfaStateFlag_CloseCapture = 0x20,
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
	createEpsilonLink (NfaState* outState);

	void
	createEpsilonLink (
		NfaState* outState,
		NfaState* outState2
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
	NfaTransition* m_transitionMap [256]; // for UTF regexps this should be replaced with interval tree
	
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
		uchar_t c, 
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
