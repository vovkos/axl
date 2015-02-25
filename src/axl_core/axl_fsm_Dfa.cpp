#include "pch.h"
#include "axl_fsm_Dfa.h"
#include "axl_err_Error.h"
#include "axl_rtl_String.h"

namespace axl {
namespace fsm {
	
//.............................................................................

DfaState::DfaState ()
{
	m_isAccept = false;
	m_id = -1;
	m_acceptContext = NULL;
}

bool
DfaState::addNfaState (NfaState* nfaState)
{
	bool result = m_nfaStateSet.addState (nfaState);
	if (!result)
		return false;

	if (nfaState->m_isAccept && !m_isAccept)
	{
		m_isAccept = true;
		m_acceptContext = nfaState->m_acceptContext;
	}

	return true;
}

void
DfaState::makeEpsilonClosure ()
{
	rtl::Array <NfaState*> workingSet = m_nfaStateSet.m_stateArray;
	while (!workingSet.isEmpty ())
	{
		NfaState* nfaState = workingSet.getBackAndPop ();
		if (nfaState->m_stateKind != NfaStateKind_EpsilonLink)
			continue;

		bool isAdded =  addNfaState (nfaState->m_outState);
		if (isAdded)
			workingSet.append (nfaState->m_outState);

		if (nfaState->m_outState2)
		{
			isAdded = addNfaState (nfaState->m_outState2);
			if (isAdded)
				workingSet.append (nfaState->m_outState2);
		}
	}
}

//.............................................................................

} // namespace fsm
} // namespace axl
