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

#include "pch.h"
#include "axl_re_Dfa.h"

namespace axl {
namespace re {

//..............................................................................

DfaState::DfaState() {
	m_id = -1;
	m_flags = false;
	m_acceptNfaStateId = -1;
	m_acceptContext = NULL;
}

bool
DfaState::addNfaState(NfaState* nfaState) {
	bool result = m_nfaStateSet.addState(nfaState);
	if (!result)
		return false;

	switch (nfaState->m_stateKind) {
	case NfaStateKind_Accept:
		if (!(m_flags & DfaStateFlag_Accept) || nfaState->m_id < m_acceptNfaStateId) {
			m_acceptContext = nfaState->m_acceptContext;
			m_acceptNfaStateId = nfaState->m_id;
		}

		m_flags |= DfaStateFlag_Accept;
		break;

	case NfaStateKind_OpenCapture:
		ASSERT(nfaState->m_captureId != -1);
		m_openCaptureIdSet.setBitResize(nfaState->m_captureId);
		break;

	case NfaStateKind_CloseCapture:
		ASSERT(nfaState->m_captureId != -1);
		m_closeCaptureIdSet.setBitResize(nfaState->m_captureId);
		break;

	default:
		ASSERT(false);
	}

	return true;
}

void
DfaState::makeEpsilonClosure() {
	sl::Array<NfaState*> workingSet = m_nfaStateSet.m_stateArray;
	while (!workingSet.isEmpty()) {
		NfaState* nfaState = workingSet.getBackAndPop();
		ASSERT(nfaState->m_stateKind != NfaStateKind_Epsilon);

		bool isAdded;

		switch (nfaState->m_stateKind) {
		case NfaStateKind_Split:
			isAdded = addNfaState(nfaState->m_nextState);
			if (isAdded)
				workingSet.append(nfaState->m_nextState);

			isAdded = addNfaState(nfaState->m_splitState);
			if (isAdded)
				workingSet.append(nfaState->m_splitState);

			break;

		case NfaStateKind_OpenCapture:
		case NfaStateKind_CloseCapture:
			isAdded = addNfaState(nfaState->m_nextState);
			if (isAdded)
				workingSet.append(nfaState->m_nextState);

			break;
		}
	}
}

//..............................................................................

} // namespace re
} // namespace axl
