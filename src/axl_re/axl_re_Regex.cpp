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

Regex::SwitchCase::SwitchCase() {
	m_captureCount = 0;
	m_nfaMatchStartState = NULL;
	m_dfaMatchStartState = NULL;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Regex::Regex():
	m_dfaProgram(0),
	m_dfaReverseProgram(DfaStateFlag_Reverse) {
	m_regexKind = RegexKind_Undefined;
}

void
Regex::clear() {
	m_regexKind = RegexKind_Undefined;
	m_nfaProgram.clear();
	m_dfaProgram.clear();
	m_dfaReverseProgram.clear();
	m_switchCaseArray.clear();
}

size_t
Regex::load(
	const void* data,
	size_t size
) {
	if (size < sizeof(RegexStorageHdr))
		return err::fail<size_t>(-1, "regex storage buffer too small");

	clear();

	const RegexStorageHdr* hdr = (const RegexStorageHdr*)data;
	if (hdr->m_signature != RegexStorageSignature ||
		hdr->m_version != RegexStorageVersion_Current ||
		hdr->m_dataSize > size - sizeof(RegexStorageHdr) ||
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

	const char* p = (char*)(hdr + 1);
	const char* p0 = p;
	const char* end = p + hdr->m_dataSize;

	sl::Array<NfaState*> stateArray;
	stateArray.setCount(hdr->m_stateCount);

	for (size_t i = 0; i < hdr->m_stateCount; i++) {
		NfaState* state = AXL_MEM_NEW(NfaState);
		state->m_id = i;
		m_nfaProgram.m_stateList.insertTail(state);
		stateArray[i] = state;
	}

	m_nfaProgram.m_matchStartState = stateArray[hdr->m_matchStartStateId];
	m_nfaProgram.m_searchStartState = hdr->m_searchStartStateId != 1 ? stateArray[hdr->m_searchStartStateId] : NULL;

	bool result = m_switchCaseArray.setCount(hdr->m_switchCaseCount);
	if (!result)
		return false;

	for (size_t i = 0; i < hdr->m_switchCaseCount; i++) {
		const RegexSwitchCaseStorage* caseStorage = (RegexSwitchCaseStorage*)p;

		if (end - p < sizeof(RegexSwitchCaseStorage) ||
			caseStorage->m_matchStartStateId >= hdr->m_stateCount ||
			caseStorage->m_captureCount > hdr->m_captureCount)
			return err::fail("invalid regex switch-case storage");

		SwitchCase& scase = m_switchCaseArray[i];
		scase.m_captureCount = caseStorage->m_captureCount;
		scase.m_nfaMatchStartState = stateArray[caseStorage->m_matchStartStateId];
		p += sizeof(RegexSwitchCaseStorage);
	}

	for (size_t i = 0; i < hdr->m_stateCount; i++) {
		const NfaStateStorage* stateStorage = (NfaStateStorage*)p;

		if (end - p < sizeof(NfaStateStorage) ||
			!stateStorage->m_stateKind ||
			stateStorage->m_stateKind > NfaStateKind_Last)
			return err::fail("invalid regex NFA state storage");

		NfaState* state = stateArray[i];
		state->m_stateKind = (NfaStateKind)stateStorage->m_stateKind;
		state->m_unionData[0] = stateStorage->m_unionData[0];
		state->m_unionData[1] = stateStorage->m_unionData[1];
		state->m_unionData[2] = stateStorage->m_unionData[2];

		if (stateStorage->m_nextStateId == -1) {
			if (stateStorage->m_stateKind != NfaStateKind_Accept)
				return err::fail("missing regex NFA next state");

			state->m_nextState = NULL;
		} else if (stateStorage->m_nextStateId < hdr->m_stateCount)
			state->m_nextState = stateArray[stateStorage->m_nextStateId];
		else
			return err::fail("invalid regex NFA next state index");

		p += sizeof(NfaStateStorage);

		switch (stateStorage->m_stateKind) {
		case NfaStateKind_Split:
			if (stateStorage->m_splitStateId > hdr->m_stateCount)
				return err::fail("invalid split NFA regex state index");

			state->m_splitState = stateArray[stateStorage->m_splitStateId];
			break;

		case NfaStateKind_Link:
			if (stateStorage->m_opStateId > hdr->m_stateCount ||
				stateStorage->m_reverseStateId > hdr->m_stateCount)
				return err::fail("invalid regex NFA state index");

			state->m_opState = stateArray[stateStorage->m_opStateId];
			state->m_reverseState = stateArray[stateStorage->m_reverseStateId];
			break;

		case NfaStateKind_Sequence:
			if (stateStorage->m_tailStateId > hdr->m_stateCount)
				return err::fail("invalid regex NFA sequence state index");

			state->m_tailState = stateArray[stateStorage->m_tailStateId];
			break;

		case NfaStateKind_MatchCharSet:
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

size_t
Regex::save(sl::Array<char>* buffer) {
	buffer->clear();

	size_t switchCaseCount = m_switchCaseArray.getCount();
	size_t stateCount = m_nfaProgram.m_stateList.getCount();

	buffer->reserve(
		sizeof(RegexStorageHdr) +
		sizeof(NfaStateStorage) * stateCount +
		sizeof(RegexSwitchCaseStorage) * switchCaseCount
	);

	buffer->appendEmptySpace(sizeof(RegexStorageHdr));
	RegexStorageHdr* hdr = (RegexStorageHdr*)buffer->p();
	hdr->m_signature = RegexStorageSignature;
	hdr->m_version = RegexStorageVersion_Current;
	hdr->m_regexKind = m_regexKind;
	hdr->m_stateCount = stateCount;
	hdr->m_switchCaseCount = switchCaseCount;
	hdr->m_captureCount = m_nfaProgram.m_captureCount;
	hdr->m_matchStartStateId = m_nfaProgram.m_matchStartState ? m_nfaProgram.m_matchStartState->m_id : -1;
	hdr->m_searchStartStateId = m_nfaProgram.m_searchStartState ? m_nfaProgram.m_searchStartState->m_id : -1;
	hdr->m_dataSize = 0;

	size_t offset = sizeof(RegexStorageHdr);

	if (switchCaseCount) {
		size_t switchCaseSize = sizeof(RegexSwitchCaseStorage) * switchCaseCount;
		buffer->appendEmptySpace(switchCaseSize);
		RegexSwitchCaseStorage* caseStorage = (RegexSwitchCaseStorage*)(buffer->p() + offset);
		for (size_t i = 0; i < switchCaseCount; i++, caseStorage++) {
			const SwitchCase& scase = m_switchCaseArray[i];
			ASSERT(scase.m_nfaMatchStartState);

			caseStorage->m_captureCount = scase.m_captureCount;
			caseStorage->m_matchStartStateId = scase.m_nfaMatchStartState->m_id;
		}

		offset += switchCaseSize;
	}

	sl::ConstIterator<NfaState> it = m_nfaProgram.m_stateList.getHead();
	for (; it; it++) {
		const NfaState* state = *it;
		size_t stateSize = state->m_stateKind == NfaStateKind_MatchCharSet ?
			sizeof(NfaStateStorage) + sizeof(NfaCharRangeStorage) * state->m_charSet->getCount() :
			sizeof(NfaStateStorage);

		buffer->appendEmptySpace(stateSize);
		NfaStateStorage* stateStorage = (NfaStateStorage*)(buffer->p() + offset);
		stateStorage->m_stateKind = state->m_stateKind;
		stateStorage->m_unionData[0] = (uint32_t)state->m_unionData[0];
		stateStorage->m_unionData[1] = (uint32_t)state->m_unionData[1];
		stateStorage->m_unionData[2] = (uint32_t)state->m_unionData[2];
		stateStorage->m_nextStateId = state->m_nextState ? state->m_nextState->m_id : -1;

		switch (state->m_stateKind) {
		case NfaStateKind_Split:
			stateStorage->m_splitStateId = state->m_splitState->m_id;
			break;

		case NfaStateKind_Link:
			stateStorage->m_opStateId = state->m_opState->m_id;
			stateStorage->m_reverseStateId = state->m_reverseState->m_id;
			break;

		case NfaStateKind_Sequence:
			stateStorage->m_tailStateId = state->m_tailState->m_id;
			break;

		case NfaStateKind_MatchCharSet:
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

bool
Regex::compile(
	uint_t flags,
	const sl::StringRef& source
) {
	clear();
	m_regexKind = RegexKind_Single;

	RegexCompiler compiler(flags, &m_nfaProgram);
	bool result = compiler.compile(source, 0);
	if (!result)
		return false;

	m_nfaProgram.finalize((flags & RegexCompileFlag_MatchOnly) != 0);
	return true;
}

void
Regex::createSwitch(size_t caseCountHint) {
	clear();
	m_regexKind = RegexKind_Switch;
	m_switchCaseArray.reserve(caseCountHint);
}

size_t
Regex::compileSwitchCase(
	uint_t flags,
	const sl::StringRef& source
) {
	ASSERT(m_regexKind = RegexKind_Switch);
	ASSERT(flags ^ RegexCompileFlag_MatchOnly); // RegexCompileFlag_MatchOnly only for finalizeSwitch()

	SwitchCase scase;
	size_t id = m_switchCaseArray.getCount();
	size_t prevCaptureCount = m_nfaProgram.m_captureCount;
	m_nfaProgram.m_captureCount = 0;

	RegexCompiler compiler(flags, &m_nfaProgram);
	scase.m_nfaMatchStartState = compiler.compileSwitchCase(source, id);

	if (prevCaptureCount > m_nfaProgram.m_captureCount)
		m_nfaProgram.m_captureCount = prevCaptureCount;

	m_switchCaseArray.append(scase);
	return id;
}

void
Regex::buildFullDfa() {
	DfaBuilder builder(&m_dfaProgram);

	getDfaMatchStartState();
	while (!m_dfaProgram.m_preStateList.isEmpty())
		builder.buildTransitionMaps(*m_dfaProgram.m_preStateList.getHead());

	if (getDfaSearchStartState())
		while (!m_dfaProgram.m_preStateList.isEmpty())
			builder.buildTransitionMaps(*m_dfaProgram.m_preStateList.getHead());
}

void
Regex::buildFullReverseDfa() {
	DfaBuilder builder(&m_dfaReverseProgram);

	getDfaReverseMatchStartState();
	while (!m_dfaReverseProgram.m_preStateList.isEmpty())
		builder.buildTransitionMaps(*m_dfaProgram.m_preStateList.getHead());
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

//..............................................................................

} // namespace re
} // namespace axl
