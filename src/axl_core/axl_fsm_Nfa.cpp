#include "pch.h"
#include "axl_fsm_Nfa.h"
#include "axl_err_Error.h"
#include "axl_rtl_String.h"

namespace axl {
namespace fsm {
	
//.............................................................................

MatchCondition::MatchCondition ()
{
	m_conditionKind = MatchConditionKind_Undefined;
	m_char = 0;
}

void
MatchCondition::copy (const MatchCondition& src)
{
	m_conditionKind = src.m_conditionKind;
	m_char = src.m_char;
	m_charSet.copy (src.m_charSet);	
}

void
MatchCondition::addChar (char c)
{
	switch (m_conditionKind)
	{
	case MatchConditionKind_Char:
		if (m_char == c) 
			break; // no change
			
		m_conditionKind = MatchConditionKind_CharSet;
		m_charSet.setBitCount (256);
		m_charSet.setBit ((uchar_t) m_char);
		
		// and fall through

	case MatchConditionKind_CharSet:
		m_charSet.setBit ((uchar_t) c);
		break;

	case MatchConditionKind_Any:
		break; // no change
	}	
}

//.............................................................................

NfaState::NfaState ()
{
	m_flags = 0;
	m_id = -1;
	m_acceptContext = NULL;
	m_captureId = -1;
	m_outState = NULL;
	m_outState2 = NULL;
}

void
NfaState::createEpsilonLink (NfaState* outState)
{
	ASSERT (!m_flags && !m_outState && !m_outState2);

	m_flags |= NfaStateFlag_EpsilonLink;
	m_outState = outState;
}

void
NfaState::createEpsilonLink (
	NfaState* outState,
	NfaState* outState2
	)
{
	ASSERT (!m_flags && !m_outState && !m_outState2);
	
	m_flags |= NfaStateFlag_EpsilonLink;
	m_outState = outState;
	m_outState2 = outState2;
}

//.............................................................................

bool
NfaStateSet::addState (NfaState* state)
{
	if (m_stateSet.getBit (state->m_id))
		return false;

	m_stateArray.append (state);
	
	if (state->m_id >= m_stateSet.getBitCount ())
		m_stateSet.setBitCount (state->m_id + 1);

	m_stateSet.setBit (state->m_id);
	return true;	
}

//.............................................................................

void 
NfaTransitionMgr::clear ()
{
	m_transitionList.clear ();
	memset (m_transitionMap, 0, sizeof (m_transitionMap));
}
	
void
NfaTransitionMgr::addMatchState (NfaState* state)
{
	ASSERT (state->m_flags & NfaStateFlag_Match);
	
	switch (state->m_matchCondition.m_conditionKind)
	{
	case MatchConditionKind_Char:
		addMatchCharTransition (state->m_matchCondition.m_char, state->m_outState);
		break;

	case MatchConditionKind_CharSet:
		for (size_t i = 0; i < 256; i++)
		{
			if (state->m_matchCondition.m_charSet.getBit (i))
				addMatchCharTransition ((char) i, state->m_outState);
		}
		break;

	case MatchConditionKind_Any:
		for (size_t i = 0; i < 256; i++)
			addMatchCharTransition ((char) i, state->m_outState);
		break;	
	}
}

void
NfaTransitionMgr::finalize ()
{
	NfaStateSetMap <NfaTransition*> transitionMap;
	
	rtl::Iterator <NfaTransition> transitionIt = m_transitionList.getHead ();
	while (transitionIt)
	{
		NfaTransition* transition = *transitionIt++;
		ASSERT (transition->m_matchCondition.m_conditionKind == MatchConditionKind_Char);
		
		NfaStateSetMap <NfaTransition*>::Iterator mapIt = transitionMap.visit (&transition->m_outStateSet);
		if (!mapIt->m_value)
		{
			mapIt->m_value = transition;
			continue;
		}

		NfaTransition* prevTransition = mapIt->m_value;
		prevTransition->m_matchCondition.addChar (transition->m_matchCondition.m_char);
		m_transitionList.erase (transition);
	}
}

void	
NfaTransitionMgr::addMatchCharTransition (
	uchar_t c, 
	NfaState* outState
	)
{
	NfaTransition* transition = m_transitionMap [c];
	if (!transition)
	{
		transition = AXL_MEM_NEW (NfaTransition);
		transition->m_matchCondition.m_conditionKind = MatchConditionKind_Char;	
		transition->m_matchCondition.m_char = c;
		m_transitionList.insertTail (transition);
		m_transitionMap [c] = transition;
	}

	transition->m_outStateSet.addState (outState);
}

//.............................................................................

} // namespace fsm
} // namespace axl
