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
#include "axl_re_Regex.h"
#include "axl_re_RegexCompiler.h"
#include "axl_re_RegexStorage.h"

namespace axl {
namespace re {

//..............................................................................

Regex::Regex() {
	m_regexKind = RegexKind_Undefined;
	m_startCount = 0;
	m_captureCount = 0;
	m_nfaMatchStartState = NULL;
	m_nfaSearchStartState = NULL;
	m_dfaMatchStartState = NULL;
	m_dfaSearchStartState = NULL;
}

void
Regex::clear() {
	m_regexKind = RegexKind_Undefined;
	m_startCount = 0;
	m_captureCount = 0;
	m_nfaMatchStartState = NULL;
	m_nfaSearchStartState = NULL;
	m_dfaMatchStartState = NULL;
	m_dfaSearchStartState = NULL;
	m_nfaStateList.clear();
	m_dfaStateList.clear();
	m_dfaStateMap.clear();
	m_switchCaseContextList.clear();
	m_switchCaseContextArray.clear();
}

size_t
Regex::load(
	const void* p,
	size_t size
) {
	if (size < sizeof(RegexStorageHdr))
		return err::fail<size_t>(-1, "regex storage buffer too small");

	clear();

	const RegexStorageHdr* hdr = (const RegexStorageHdr*)p;
	if (hdr->m_dataSize > size - sizeof(RegexStorageHdr) ||
		hdr->m_matchStartStateId >= hdr->m_stateCount ||
		hdr->m_searchStartStateId != -1 && hdr->m_searchStartStateId >= hdr->m_stateCount)
		return err::fail<size_t>(-1, "invalid regex storage");

	switch (hdr->m_regexKind) {
	case RegexKind_Single:
		if (hdr->m_switchCaseCount)
			return err::fail<size_t>(-1, "regex kind mismatch");
		break;

	case RegexKind_Switch:
		break;

	default:
		return err::fail<size_t>(-1, "invalid regex kind");
	}

	clear();
	m_regexKind = (RegexKind)hdr->m_regexKind;

	switch (hdr->m_storageKind) {
	case RegexStorageKind_Nfa:
		return loadNfa(hdr) ? sizeof(RegexStorageHdr) + hdr->m_dataSize : -1;

	case RegexStorageKind_Dfa:
		return loadDfa(hdr) ? sizeof(RegexStorageHdr) + hdr->m_dataSize : -1;

	default:
		return err::fail<size_t>(-1, "invalid regex storage");
	}
}

bool
Regex::loadNfa(const RegexStorageHdr* hdr) {
	const char* p = (char*)(hdr + 1);
	const char* p0 = p;
	const char* end = p + hdr->m_dataSize;

	sl::Array<NfaState*> stateArray;
	stateArray.setCount(hdr->m_stateCount);

	for (size_t i = 0; i < hdr->m_stateCount; i++) {
		NfaState* state = AXL_MEM_NEW(NfaState);
		state->m_id = i;
		m_nfaStateList.insertTail(state);
		stateArray[i] = state;
	}

	m_nfaMatchStartState = stateArray[hdr->m_matchStartStateId];
	m_nfaSearchStartState = hdr->m_searchStartStateId != 1 ? stateArray[hdr->m_searchStartStateId] : NULL;

	bool result = m_switchCaseContextArray.setCount(hdr->m_switchCaseCount);
	if (!result)
		return false;

	for (size_t i = 0; i < hdr->m_switchCaseCount; i++) {
		const RegexSwitchCaseStorage* caseStorage = (RegexSwitchCaseStorage*)p;

		if (end - p < sizeof(RegexSwitchCaseStorage) ||
			caseStorage->m_matchStartStateId >= hdr->m_stateCount ||
			caseStorage->m_captureCount && caseStorage->m_baseCaptureId >= hdr->m_captureCount ||
			caseStorage->m_captureCount > hdr->m_captureCount - caseStorage->m_baseCaptureId)
			return err::fail("invalid regex switch-case storage");

		SwitchCaseContext* context = AXL_MEM_NEW(SwitchCaseContext);
		context->m_baseCaptureId = caseStorage->m_baseCaptureId;
		context->m_captureCount = caseStorage->m_captureCount;
		context->m_nfaMatchStartState = stateArray[caseStorage->m_matchStartStateId];
		m_switchCaseContextList.insertTail(context);
		m_switchCaseContextArray[i] = context;

		p += sizeof(RegexSwitchCaseStorage);
	}

	for (size_t i = 0; i < hdr->m_stateCount; i++) {
		const NfaStateStorage* stateStorage = (NfaStateStorage*)p;

		if (end - p < sizeof(NfaStateStorage) ||
			stateStorage->m_stateKind < NfaStateKind_FirstStorable ||
			stateStorage->m_stateKind > NfaStateKind_LastStorable)
			return err::fail("invalid regex NFA state storage");

		NfaState* state = stateArray[i];
		state->m_stateKind = (NfaStateKind)stateStorage->m_stateKind;
		state->m_unionData = stateStorage->m_unionData;

		if (stateStorage->m_nextStateId == -1) {
			if (stateStorage->m_stateKind != NfaStateKind_Accept)
				return err::fail("missing next regex NFA state");

			state->m_nextState = NULL;
		} else if (stateStorage->m_nextStateId < hdr->m_stateCount)
			state->m_nextState = stateArray[stateStorage->m_nextStateId];
		else
			return err::fail("invalid next regex NFA state index");

		p += sizeof(NfaStateStorage);

		if (stateStorage->m_stateKind == NfaStateKind_Split) {
			if (stateStorage->m_splitStateId > hdr->m_stateCount)
				return err::fail("invalid split regex NFA state index");

			state->m_splitState = stateArray[stateStorage->m_splitStateId];
		} else if (stateStorage->m_stateKind == NfaStateKind_MatchCharSet) {
			state->m_charSet = AXL_MEM_NEW(CharSet);

			for (size_t j = 0; j < stateStorage->m_charRangeCount; j++) {
				const NfaCharRangeStorage* rangeStorage = (NfaCharRangeStorage*)p;
				if (end - p < sizeof(NfaCharRangeStorage))
					return err::fail("invalid regex char set storage");

				state->m_charSet->add(rangeStorage->m_charFrom, rangeStorage->m_charTo);
				p += sizeof(NfaCharRangeStorage);
			}
		}
	}

	if (p - p0 != hdr->m_dataSize)
		return err::fail("regex storage data size mismatch");

	return true;
}

bool
Regex::loadDfa(const RegexStorageHdr* hdr) {
	const char* p = (char*)(hdr + 1);
	const char* p0 = p;
	const char* end = p + hdr->m_dataSize;

	sl::Array<DfaState*> stateArray;
	stateArray.setCount(hdr->m_stateCount);

	for (size_t i = 0; i < hdr->m_stateCount; i++) {
		DfaState* state = AXL_MEM_NEW(DfaState);
		state->m_id = i;
		m_dfaStateList.insertTail(state);
		stateArray[i] = state;
	}

	m_dfaMatchStartState = stateArray[hdr->m_matchStartStateId];
	m_dfaSearchStartState = hdr->m_searchStartStateId != 1 ? stateArray[hdr->m_searchStartStateId] : NULL;

	bool result = m_switchCaseContextArray.setCount(hdr->m_switchCaseCount);
	if (!result)
		return false;

	for (size_t i = 0; i < hdr->m_switchCaseCount; i++) {
		const RegexSwitchCaseStorage* caseStorage = (RegexSwitchCaseStorage*)p;

		if (end - p < sizeof(RegexSwitchCaseStorage) ||
			caseStorage->m_captureCount && caseStorage->m_baseCaptureId >= hdr->m_captureCount ||
			caseStorage->m_captureCount > hdr->m_captureCount - caseStorage->m_baseCaptureId)
			return err::fail("invalid regex switch-case storage");

		SwitchCaseContext* context = AXL_MEM_NEW(SwitchCaseContext);
		context->m_baseCaptureId = caseStorage->m_baseCaptureId;
		context->m_captureCount = caseStorage->m_captureCount;
		context->m_nfaMatchStartState = NULL;
		m_switchCaseContextList.insertTail(context);
		m_switchCaseContextArray[i] = context;

		p += sizeof(RegexSwitchCaseStorage);
	}

	for (size_t i = 0; i < hdr->m_stateCount; i++) {
		const DfaStateStorage* stateStorage = (DfaStateStorage*)p;

		if (end - p < sizeof(DfaStateStorage) ||
			stateStorage->m_anchorMask >= Anchor__TransitionMapSize ||
			(stateStorage->m_flags & DfaStateFlag_Accept) && hdr->m_switchCaseCount && stateStorage->m_acceptId >= hdr->m_switchCaseCount)
			return err::fail("invalid regex DFA state storage");

		DfaState* state = stateArray[i];
		state->m_flags = stateStorage->m_flags;
		state->m_anchorMask = stateStorage->m_anchorMask;
		state->m_acceptId = stateStorage->m_acceptId;
		state->m_anchorTransitionMap.setCountZeroConstruct(state->m_anchorMask + 1);

		p += sizeof(DfaStateStorage);

		for (size_t i = 1; i <= stateStorage->m_anchorMask; i++) {
			const uint32_t* id = (uint32_t*)p;
			if (end - p < sizeof(uint32_t) || *id >= hdr->m_stateCount)
				return err::fail("invalid regex anchor transition storage");

			state->m_anchorTransitionMap[i] = stateArray[*id];
			p += sizeof(uint32_t);
		}

		for (size_t i = 1; i <= stateStorage->m_charTransitionCount; i++) {
			const DfaCharTransitionStorage* transitionStorage = (DfaCharTransitionStorage*)p;
			if (end - p < sizeof(DfaCharTransitionStorage) ||
				transitionStorage->m_stateId >= hdr->m_stateCount)
				return err::fail("invalid regex char transition storage");

			state->m_charTransitionMap.add(
				transitionStorage->m_charFrom,
				transitionStorage->m_charTo,
				stateArray[transitionStorage->m_stateId],
				transitionStorage->m_flags
			);

			p += sizeof(DfaCharTransitionStorage);
		}
	}

	if (p - p0 != hdr->m_dataSize)
		return err::fail("regex storage data size mismatch");

	return true;
}

size_t
Regex::saveNfa(sl::Array<char>* buffer) {
	buffer->clear();

	size_t switchCaseCount = m_switchCaseContextArray.getCount();
	size_t stateCount = m_nfaStateList.getCount();

	buffer->reserve(
		sizeof(RegexStorageHdr) +
		sizeof(NfaStateStorage) * stateCount +
		sizeof(RegexSwitchCaseStorage) * switchCaseCount
	);

	buffer->appendEmptySpace(sizeof(RegexStorageHdr));
	RegexStorageHdr* hdr = (RegexStorageHdr*)buffer->p();
	hdr->m_storageKind = RegexStorageKind_Nfa;
	hdr->m_regexKind = m_regexKind;
	hdr->m_stateCount = stateCount;
	hdr->m_switchCaseCount = switchCaseCount;
	hdr->m_captureCount = m_captureCount;
	hdr->m_matchStartStateId = m_nfaMatchStartState ? m_nfaMatchStartState->m_id : -1;
	hdr->m_searchStartStateId = m_nfaSearchStartState ? m_nfaSearchStartState->m_id : -1;
	hdr->m_dataSize = 0;

	size_t offset = sizeof(RegexStorageHdr);

	if (switchCaseCount) {
		size_t switchCaseSize = sizeof(RegexSwitchCaseStorage) * switchCaseCount;
		buffer->appendEmptySpace(switchCaseSize);
		RegexSwitchCaseStorage* caseStorage = (RegexSwitchCaseStorage*)(buffer->p() + offset);
		for (size_t i = 0; i < switchCaseCount; i++, caseStorage++) {
			const SwitchCaseContext* context = m_switchCaseContextArray[i];
			ASSERT(context->m_nfaMatchStartState);

			caseStorage->m_baseCaptureId = context->m_baseCaptureId;
			caseStorage->m_captureCount = context->m_captureCount;
			caseStorage->m_matchStartStateId = context->m_nfaMatchStartState->m_id;
		}

		offset += switchCaseSize;
	}

	sl::ConstIterator<NfaState> it = m_nfaStateList.getHead();
	for (; it; it++) {
		const NfaState* state = *it;
		size_t stateSize = state->m_stateKind == NfaStateKind_MatchCharSet ?
			sizeof(NfaStateStorage) + sizeof(NfaCharRangeStorage) * state->m_charSet->getCount() :
			sizeof(NfaStateStorage);

		buffer->appendEmptySpace(stateSize);
		NfaStateStorage* stateStorage = (NfaStateStorage*)(buffer->p() + offset);
		stateStorage->m_stateKind = state->m_stateKind;
		stateStorage->m_unionData = (uint32_t)state->m_unionData;
		stateStorage->m_nextStateId = state->m_nextState ? state->m_nextState->m_id : -1;

		if (state->m_stateKind == NfaStateKind_Split)
			stateStorage->m_splitStateId = state->m_splitState->m_id;
		else if (state->m_stateKind == NfaStateKind_MatchCharSet) {
			stateStorage->m_charRangeCount = state->m_charSet->getCount();

			NfaCharRangeStorage* rangeStorage = (NfaCharRangeStorage*)(stateStorage + 1);
			CharSet::ConstIterator it = state->m_charSet->getHead();
			for (; it; it++, rangeStorage++) {
				rangeStorage->m_charFrom = it->getKey();
				rangeStorage->m_charTo = it->m_value;
			}

			ASSERT((char*)rangeStorage == buffer->getEnd());
		}

		offset += stateSize;
	}

	ASSERT(offset == buffer->getCount());

	hdr = (RegexStorageHdr*)buffer->p();
	hdr->m_dataSize = offset - sizeof(RegexStorageHdr);
	return offset;
}

size_t
Regex::saveDfa(sl::Array<char>* buffer) {
	buffer->clear();

	buildFullDfa();

	size_t switchCaseCount = m_switchCaseContextArray.getCount();
	size_t stateCount = m_dfaStateList.getCount();

	buffer->reserve(
		sizeof(RegexStorageHdr) +
		sizeof(DfaStateStorage) * stateCount +
		sizeof(RegexSwitchCaseStorage) * switchCaseCount
	);

	buffer->appendEmptySpace(sizeof(RegexStorageHdr));
	RegexStorageHdr* hdr = (RegexStorageHdr*)buffer->p();
	hdr->m_storageKind = RegexStorageKind_Dfa;
	hdr->m_regexKind = m_regexKind;
	hdr->m_stateCount = stateCount;
	hdr->m_switchCaseCount = switchCaseCount;
	hdr->m_captureCount = m_captureCount;
	hdr->m_matchStartStateId = m_dfaMatchStartState ? m_dfaMatchStartState->m_id : -1;
	hdr->m_searchStartStateId = m_dfaSearchStartState ? m_dfaSearchStartState->m_id : -1;
	hdr->m_dataSize = 0;

	size_t offset = sizeof(RegexStorageHdr);

	if (switchCaseCount) {
		size_t switchCaseSize = sizeof(RegexSwitchCaseStorage) * switchCaseCount;
		buffer->appendEmptySpace(switchCaseSize);
		RegexSwitchCaseStorage* caseStorage = (RegexSwitchCaseStorage*)(buffer->p() + offset);
		for (size_t i = 0; i < switchCaseCount; i++, caseStorage++) {
			const SwitchCaseContext* context = m_switchCaseContextArray[i];
			ASSERT(context->m_nfaMatchStartState);

			caseStorage->m_baseCaptureId = context->m_baseCaptureId;
			caseStorage->m_captureCount = context->m_captureCount;
			caseStorage->m_matchStartStateId = -1;
		}

		buildFullDfa(); // ensure switch-case DFA match states are built
		offset += switchCaseSize;
	}

	sl::ConstIterator<DfaState> it = m_dfaStateList.getHead();
	for (; it; it++) {
		const DfaState* state = *it;
		size_t charTransitionCount = state->m_charTransitionMap.getCount();

		size_t stateSize =
			sizeof(DfaStateStorage) +
			state->m_anchorMask * sizeof(uint32_t) +
			charTransitionCount * sizeof(DfaCharTransitionStorage);

		buffer->appendEmptySpace(stateSize);
		DfaStateStorage* stateStorage = (DfaStateStorage*)(buffer->p() + offset);
		stateStorage->m_flags = state->m_flags;
		stateStorage->m_anchorMask = state->m_anchorMask;
		stateStorage->m_acceptId = state->m_acceptId;
		stateStorage->m_charTransitionCount = charTransitionCount;

		uint32_t* idStorage = (uint32_t*)(stateStorage + 1);

		for (size_t i = 1; i <= state->m_anchorMask; i++) {
			const DfaState* transitionState = state->m_anchorTransitionMap[i];
			*idStorage = transitionState ? transitionState->m_id : -1;
		}

		DfaCharTransitionStorage* transitionStorage = (DfaCharTransitionStorage*)idStorage;

		DfaCharTransitionMap::ConstIterator it2 = state->m_charTransitionMap.getHead();
		for (; it2; it2++, transitionStorage++) {
			transitionStorage->m_charFrom = it2->getKey();
			transitionStorage->m_charTo = it2->m_value.m_last;
			transitionStorage->m_stateId = it2->m_value.m_state->m_id;
			transitionStorage->m_flags = it2->m_value.m_flags;
		}

		ASSERT((char*)transitionStorage == buffer->getEnd());
		offset += stateSize;
	}

	ASSERT(offset == buffer->getCount());

	hdr = (RegexStorageHdr*)buffer->p();
	hdr->m_dataSize = offset - sizeof(RegexStorageHdr);
	hdr->m_stateCount = m_dfaStateList.getCount();
	return offset;
}

bool
Regex::compile(
	uint_t flags,
	const sl::StringRef& source
) {
	clear();
	m_regexKind = RegexKind_Single;

	RegexCompiler compiler(flags, this);
	bool result = compiler.compile(source);
	if (!result)
		return false;

	finalize(flags);
	return true;
}

void
Regex::buildFullDfa() {
	DfaBuilder builder(this);
	builder.buildFullDfa();
}

void
Regex::createSwitch() {
	clear();
	m_regexKind = RegexKind_Switch;
}

size_t
Regex::compileSwitchCase(
	uint_t flags,
	const sl::StringRef& source
) {
	ASSERT(m_regexKind = RegexKind_Switch);
	ASSERT(flags ^ RegexCompileFlag_MatchOnly); // RegexCompileFlag_MatchOnly only for finalizeSwitch()

	size_t id = m_switchCaseContextList.getCount();

	SwitchCaseContext* context = AXL_MEM_NEW(SwitchCaseContext);
	context->m_baseCaptureId = m_captureCount;
	context->m_dfaMatchStartState = NULL;
	m_switchCaseContextList.insertTail(context);
	m_switchCaseContextArray.append(context);

	RegexCompiler compiler(flags, this);
	context->m_nfaMatchStartState = compiler.compileSwitchCase(source, id);
	context->m_captureCount = m_captureCount - context->m_baseCaptureId;
	return id;
}

bool
Regex::exec(
	RegexState* state,
	const void* p,
	size_t size
) {
	if (!*state)
		state->initialize(this);
	else if (!state->getRegex())
		state->postInitialize(this);
	else
		ASSERT(state->getRegex() == this);

	bool result = state->exec(p, size);
	return result ? (state->getExecFlags() & RegexExecFlag_Stream) ? true : state->eof() : false;
}

void
Regex::finalize(uint_t flags) {
	if (isEmpty())
		return;

	m_nfaMatchStartState = *m_nfaStateList.getHead();

	// insert search prefix (.*)

	if (!(flags & RegexCompileFlag_MatchOnly)) {
		NfaState* matchAny = AXL_MEM_NEW(NfaState);
		NfaState* split = AXL_MEM_NEW(NfaState);
		split->createSplit(matchAny, m_nfaMatchStartState);
		matchAny->createMatchAnyChar(split);
		m_nfaStateList.insertHead(matchAny);
		m_nfaStateList.insertHead(split);
		m_nfaSearchStartState = split;
	}

	// get rid of epsilon transitions and assign NFA state IDs

	sl::List<NfaState> epsilonList; // must keep epsilon states alive while finalizing

	sl::Iterator<NfaState> it = m_nfaStateList.getHead();
	for (uint_t i = 0; it; ) {
		NfaState* state = *it++;
		if (state->m_stateKind == NfaStateKind_Epsilon) {
			m_nfaStateList.remove(state);
			epsilonList.insertTail(state);
		} else {
			state->m_id = i++;
			state->resolveOutStates();
		}
	}

	// demux the epsilon closure of the search prefix

	if (!(flags & RegexCompileFlag_MatchOnly)) {
		NfaDemuxer demuxer(this);
		demuxer.demux();
	}
}

DfaState*
Regex::createDfaStartState(const NfaState* nfaState) {
	DfaState* dfaState = AXL_MEM_NEW(DfaState);
	dfaState->m_nfaStateSet.add(nfaState);
	dfaState->m_nfaStateSet.buildEpsilonClosure();
	return addDfaState(dfaState);
}

DfaState*
Regex::addDfaState(DfaState* state) {
	NfaStateSetMap<DfaState*>::Iterator it = m_dfaStateMap.visit(&state->m_nfaStateSet);
	if (it->m_value) {
		AXL_MEM_DELETE(state);
		return it->m_value;
	}

	state->finalize();
	m_preDfaStateList.insertTail(state);
	it->m_value = state;
	return state;
}

#if (_AXL_DEBUG)

void
Regex::printNfa() const {
	sl::String string;

	sl::ConstIterator<NfaState> it = m_nfaStateList.getHead();
	for (; it; it++) {
		const NfaState* state = *it;

		printf(
			"%c %c (%02d): ",
			state == m_nfaMatchStartState ? 'M' :
			state == m_nfaSearchStartState ? 'S' : ' ',
			(state->m_flags & NfaStateFlag_StartEpsilonClosure) ? '*' :
			(state->m_flags & NfaStateFlag_Demux) ? '+' : ' ',
			state->m_id
		);

		switch (state->m_stateKind) {
		case NfaStateKind_Accept:
			if (m_regexKind == RegexKind_Switch)
				printf("accept #%0d", state->m_acceptId);
			else
				printf("accept");
			break;

		case NfaStateKind_Epsilon:
			ASSERT(state->m_nextState);
			printf("eps -> %02d", state->m_nextState->m_id);
			break;

		case NfaStateKind_Split:
			ASSERT(state->m_nextState && state->m_splitState);
			printf("split -> %02d : %02d", state->m_nextState->m_id, state->m_splitState->m_id);
			break;

		case NfaStateKind_MatchChar:
			ASSERT(state->m_nextState);
			getCharString(&string, state->m_char);
			printf("'%s' -> %02d", string.sz(), state->m_nextState->m_id);
			break;

		case NfaStateKind_MatchCharSet:
			ASSERT(state->m_nextState);
			state->m_charSet->getString(&string);
			printf("[%s] -> %02d", string.sz(), state->m_nextState->m_id);
			break;

		case NfaStateKind_MatchAnyChar:
			ASSERT(state->m_nextState);
			printf(". -> %02d", state->m_nextState->m_id);
			break;

		case NfaStateKind_MatchAnchor:
			ASSERT(state->m_nextState);
			switch (state->m_anchor) {
			case Anchor_Begin:
				printf("^ -> %02d", state->m_nextState->m_id);
				break;

			case Anchor_End:
				printf("$ -> %02d", state->m_nextState->m_id);
				break;

			case Anchor_Word:
				printf("\\b -> %02d", state->m_nextState->m_id);
				break;

			default:
					ASSERT(false);
			}

			break;

		case NfaStateKind_OpenCapture:
			ASSERT(state->m_nextState);
			printf("open(%d) -> %02d", state->m_captureId, state->m_nextState->m_id);
			break;

		case NfaStateKind_CloseCapture:
			ASSERT(state->m_nextState);
			printf("close(%d) -> %02d", state->m_captureId, state->m_nextState->m_id);
			break;

		default:
			ASSERT(false);
		}

		if (state->m_demuxState && state->m_demuxState != (NfaState*)-1)
			printf(" demux(%02d)", state->m_demuxState->m_id);

		size_t i = state->m_startIdSet.findBit(0);
		if (i != -1) {
			printf(" start({");

			do {
				printf(" %02d", i);
				i = state->m_startIdSet.findBit(++i);
			} while (i != -1);

			printf(" })");
		}

		printf("\n");
	}
}

void
Regex::printDfa() const {
	sl::String string;

	sl::ConstIterator<DfaState> it = m_dfaStateList.getHead();
	for (; it; it++) {
		const DfaState* state = *it;
		printf(
			"%c %02d: nfa({ ",
			state == m_dfaMatchStartState ? 'M' :
			state == m_dfaSearchStartState ? 'S' : ' ',
			state->m_id
		);

#define INDENT "      "

		size_t count = state->m_nfaStateSet.getCount();
		for (size_t i = 0; i < count; i++) {
			const NfaState* nfaState = state->m_nfaStateSet[i];
			printf("%02d ", nfaState->m_id);
		}

		printf("})\n");

		if (state->m_flags & DfaStateFlag_Accept)
			if (m_regexKind == RegexKind_Switch)
				printf(INDENT "accept #%0d\n", state->m_acceptId);
			else
				printf(INDENT "accept\n");

		count = state->m_anchorTransitionMap.getCount();
		for (size_t i = 0; i < count; i++) {
			const DfaState* state2 = state->m_anchorTransitionMap[i];
			if (state2)
				printf(INDENT "%s -> %02d\n", getAnchorString(i), state2->m_id);
		}

		DfaCharTransitionMap::ConstIterator it2 = state->m_charTransitionMap.getHead();
		for (; it2; it2++) {
			getCharRangeString(&string, it2->getKey(), it2->m_value.m_last);
			printf(
				INDENT "%s %c> %02d\n",
				string.sz(),
				(it2->m_value.m_flags & DfaTransitionFlag_Alive) ? '-' : '~',
				it2->m_value.m_state->m_id
			);
		}
	}
}

#endif

//..............................................................................

} // namespace re
} // namespace axl
