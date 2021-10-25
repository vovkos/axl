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

namespace axl {
namespace re {

//..............................................................................

static
inline
void
appendChar(
	sl::String* string,
	utf32_t c
) {
	if (enc::utfIsPrintable(c) && c < 0x80)
		string->append(c);
	else if (c <= 0xffff)
		string->appendFormat("\\u%04x", c);
	else
		string->appendFormat("\\U%08x", c);
}

static
sl::String
getCharSetString(const CharSet* charSet) {
	sl::String string;

	uint_t c1 = -1;

	CharSet::ConstIterator it = charSet->getHead();
	for (; it != 0; it++) {
		utf32_t from = it->getKey();
		utf32_t to = it->m_value;

		if (from == to)
			appendChar(&string, from);
 		else {
			appendChar(&string, from);
			string += '-';
			appendChar(&string, to);
		}
	}

	return string;
}

//..............................................................................

Regex::Regex() {
	m_regexKind = RegexKind_Undefined;
	m_groupCount = 0;
	m_maxSubMatchCount = 0;
}

void
Regex::clear() {
	m_regexKind = RegexKind_Undefined;
	m_groupCount = 0;
	m_maxSubMatchCount = 0;
	m_stateList.clear();
	m_stateArray.clear();
	m_caseContextList.clear();
}

bool
Regex::compile(const sl::StringRef& source) {
	clear();
	m_regexKind = RegexKind_Normal;

	RegexCompiler compiler(this);
	bool result = compiler.compile(source);
	if (!result)
		return false;

	compiler.finalize();
	return true;
}

void
Regex::createSwitch() {
	clear();
	m_regexKind = RegexKind_Switch;
}

bool
Regex::compileSwitchCase(
	const sl::StringRef& source,
	const rc::Ptr<void>& caseContext
) {
	ASSERT(m_regexKind = RegexKind_Switch);

	SwitchCaseContext* context = AXL_MEM_NEW(SwitchCaseContext);
	context->m_index = m_caseContextList.getCount();
	context->m_context = caseContext;
	context->m_firstGroupId = m_groupCount;
	m_caseContextList.insertTail(context);

	RegexCompiler compiler(this);
	bool result = compiler.compile(source, context);
	if (!result)
		return false;

	context->m_groupCount = m_groupCount - context->m_firstGroupId;
	return true;
}

bool
Regex::finalizeSwitch() {
	ASSERT(m_regexKind = RegexKind_Switch);
	RegexCompiler compiler(this);
	compiler.finalize();
	return true;
}

bool
Regex::match(
	enc::CharCodec* codec,
	const void* p,
	size_t size
) {
	if (isEmpty())
		return true;

	RegexState state(this, codec);
	state.exec(p, size);
	state.eof();
	return true;
}

#if (_AXL_DEBUG)
void
Regex::print() const {
	printf("NFA\n");
	printf("==================\n");

	sl::String string;

	sl::ConstIterator<NfaState> it = m_stateList.getHead();
	for (; it; it++) {
		const NfaState* state = *it;
		printf("%02d: ", state->m_id);

		switch (state->m_stateKind) {
		case NfaStateKind_Accept:
			if (state->m_nextState)
				printf("accept -> %02d", state->m_nextState->m_id);
			else
				printf("accept (final)");
			break;

		case NfaStateKind_Epsilon:
			ASSERT(state->m_nextState && !state->m_splitState);
			printf("eps -> %02d", state->m_nextState->m_id);
			break;

		case NfaStateKind_Split:
			ASSERT(state->m_nextState && state->m_splitState);
			printf("split -> %02d : %02d", state->m_nextState->m_id, state->m_splitState->m_id);
			break;

		case NfaStateKind_MatchChar:
			ASSERT(state->m_nextState && !state->m_splitState);

			string.clear();
			appendChar(&string, state->m_char);

			printf("'%s' -> %02d", string.sz(), state->m_nextState->m_id);
			break;

		case NfaStateKind_MatchCharSet:
			ASSERT(state->m_nextState && !state->m_splitState);
			printf("[%s] -> %02d", getCharSetString(state->m_charSet).sz(), state->m_nextState->m_id);
			break;

		case NfaStateKind_MatchAnyChar:
			ASSERT(state->m_nextState && !state->m_splitState);
			printf(". -> %02d", state->m_nextState->m_id);
			break;

		case NfaStateKind_MatchAnchor:
			ASSERT(state->m_nextState && !state->m_splitState);

			switch (state->m_anchor)
			{
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
			ASSERT(state->m_nextState && !state->m_splitState);
			printf("open(%d) -> %02d", state->m_captureId, state->m_nextState->m_id);
			break;

		case NfaStateKind_CloseCapture:
			ASSERT(state->m_nextState && !state->m_splitState);
			printf("close(%d) -> %02d", state->m_captureId, state->m_nextState->m_id);
			break;

		default:
			ASSERT(false);
		}

		printf("\n");
	}
}
#endif

//..............................................................................

} // namespace re
} // namespace axl
