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
#include "axl_re_Compiler.h"
#include "axl_re_Regex.h"
#include "axl_re_StdCharClass.h"

namespace axl {
namespace re {

//..............................................................................

NfaState*
Compiler::compileSwitchCase(
	const sl::StringRef& source,
	size_t acceptId
) {
	NfaState* oldStart = !m_program->m_stateList.isEmpty() ? *m_program->m_stateList.getHead() : NULL;
	NfaState* newStart = compile(source, acceptId);

	if (oldStart)
		m_program->insertSplitState(oldStart, newStart);

	return newStart;
}

NfaState*
Compiler::compile(
	const sl::StringRef& source,
	size_t acceptId
) {
	Lexer::create(source);

	NfaState* newStart = expression();
	if (!newStart)
		return NULL;

	Token token = getToken();
	if (token != TokenKind_Eof) {
		err::setError("invalid regexp syntax");
		return NULL;
	}

	NfaState* accept = m_program->getLastState();
	accept->createAccept(acceptId);
	return newStart;
}

inline
bool
Compiler::expectToken(TokenKind tokenKind) {
	Token token = getToken();
	if (token != tokenKind) {
		err::setFormatStringError("'%c' expected", getTokenString(tokenKind));
		return false;
	}

	nextToken();
	return true;
}

NfaState*
Compiler::expression() {
	NfaState* start = concat();
	if (!start)
		return NULL;

	Token token = getToken();
	while (token == TokenKind_Pipe) {
		nextToken();

		NfaState* accept1 = m_program->getLastState();
		NfaState* op = expression();
		if (!op)
			return NULL;

		NfaState* accept2 = m_program->getLastState();
		NfaState* split = m_program->insertSplitState(start, op);
		NfaState* accept = m_program->addState();
		accept1->createEpsilon(accept);
		accept2->createEpsilon(accept);
		split->m_flags |= start->m_flags | op->m_flags;
		start = split;

		token = getToken();
	}

	return start;
}

NfaState*
Compiler::concat() {
	NfaState* start = repeat();
	if (!start)
		return NULL;

	Token token = getToken();
	if (!token.isValidSingle())
		return start; // not a sequence

	uint_t flags = start->m_flags;
	NfaState* link = m_program->getLastState();
	NfaState* accept = AXL_MEM_NEW(NfaState);
	NfaState* head = link;
	head->createLink(start, accept);

	do {
		NfaState* op = repeat();
		if (!op) {
			AXL_MEM_DELETE(accept);
			return NULL;
		}

		if (!(op->m_flags & NfaStateFlag_Nullable))
			flags &= ~NfaStateFlag_Nullable;

		NfaState* link2 = m_program->getLastState();
		link->finalizeLink(link2);
		link2->createLink(op, link);
		link = link2;

		token = getToken();
	} while (token.isValidSingle());

	m_program->m_stateList.insertTail(accept);
	NfaState* sequence = m_program->insertState(start);
	sequence->createSequence(head, link);
	sequence->m_flags = flags;
	link->finalizeLink(accept);
	return sequence;
}


template <typename IsNonGreedy>
NfaState*
Compiler::question(NfaState* start) {
	Token token = nextToken();
	ASSERT(token == TokenKind_Question || token == TokenKind_NonGreedyQuestion);

	NfaState* oldAccept = m_program->getLastState();
	NfaState* newAccept = m_program->addState();
	NfaState* split = IsNonGreedy()() ?
		m_program->insertSplitState(newAccept, start) :
		m_program->insertSplitState(start, newAccept);

	oldAccept->createEpsilon(newAccept);
	split->m_flags |= NfaStateFlag_Nullable;
	return split;
}

template <typename IsNonGreedy>
NfaState*
Compiler::star(NfaState* start) {
	Token token = nextToken();
	ASSERT(token == TokenKind_Star || token == TokenKind_NonGreedyStar);

	NfaState* oldAccept = m_program->getLastState();
	NfaState* newAccept = m_program->addState();
	NfaState* split = IsNonGreedy()() ?
		m_program->insertSplitState(newAccept, start) :
		m_program->insertSplitState(start, newAccept);

	oldAccept->createEpsilon(split);
	split->m_flags |= NfaStateFlag_Nullable;
	return split;
}

template <typename IsNonGreedy>
NfaState*
Compiler::plus(NfaState* start) {
	Token token = nextToken();
	ASSERT(token == TokenKind_Plus || token == TokenKind_NonGreedyPlus);

	NfaState* oldAccept = m_program->getLastState();
	NfaState* newAccept = m_program->addState();
	if (IsNonGreedy()())
		oldAccept->createSplit(newAccept, start);
	else
		oldAccept->createSplit(start, newAccept);

	return start;
}


NfaState*
Compiler::repeat() {
	NfaState* start = single();
	if (!start)
		return NULL;

	Token token = getToken();
	switch (token.m_tokenKind) {
	case TokenKind_Question:
		return question<sl::False>(start);

	case TokenKind_NonGreedyQuestion:
		return question<sl::True>(start);

	case TokenKind_Star:
		return star<sl::False>(start);

	case TokenKind_NonGreedyStar:
		return star<sl::True>(start);

	case TokenKind_Plus:
		return plus<sl::False>(start);

	case TokenKind_NonGreedyPlus:
		return plus<sl::True>(start);

	case TokenKind_Quantifier:
		return quantifier(start);
	}

	return start;
}

NfaState*
Compiler::quantifier(NfaState* start) {
	Token token = nextToken();
	ASSERT(token == TokenKind_Quantifier);

	Token tokenCount = nextToken();
	token = nextToken();

	if (tokenCount != TokenKind_Number ||
		token != TokenKind_EndQuantifier
	) {
		err::setFormatStringError("invalid quantifier (only simple quantifiers are currently supported)");
		return NULL;
	}

	NfaState* originalStart = start;
	NfaState* accept = m_program->getLastState();
	for (size_t i = 1; i < tokenCount.m_number; i++) {
		start = clone(start, accept);
		accept->createEpsilon(start);
		accept = m_program->getLastState();
	}

	return originalStart;
}

NfaState*
Compiler::clone(
	NfaState* first,
	NfaState* last
) {
	sl::SimpleHashTable<NfaState*, NfaState*> stateMap;

	sl::Iterator<NfaState> end = m_program->m_stateList.getTail();
	sl::Iterator<NfaState> it = first;
	for (;;) {
		NfaState* oldState = *it;
		NfaState* newState = m_program->addState();
		newState->copy(*oldState);
		stateMap[oldState] = newState;

		if (it == last)
			break;

		it++;
	}

	it = end + 1;
	NfaState* result = *it;
	for (; it; it++) {
		NfaState* state = *it;

		sl::HashTableIterator<NfaState*, NfaState*> mapIt;
		if (state->m_nextState) {
			mapIt = stateMap.find(state->m_nextState);
			if (mapIt)
				state->m_nextState = mapIt->m_value;
		}

		if (state->m_splitState) {
			mapIt = stateMap.find(state->m_splitState);
			if (mapIt)
				state->m_splitState = mapIt->m_value;
		}
	}

	return result;
}

NfaState*
Compiler::single() {
	Token token = getToken();
	ASSERT(token.isValidSingle());

	switch (token.m_tokenKind) {
	case TokenKind_Group:
		return (m_flags & CompileFlag_DisableCapture) ?
			nonCapturingGroup() :
			capturingGroup();

	case TokenKind_NonCapturingGroup:
		return nonCapturingGroup();

	case TokenKind_CharClass:
		return charClass<sl::False>();

	case TokenKind_NegatedCharClass:
		return charClass<sl::True>();

	case TokenKind_StdCharClassDigit:
		return stdCharClass<StdCharClassDigit>();

	case TokenKind_StdCharClassHex:
		return stdCharClass<StdCharClassHex>();

	case TokenKind_StdCharClassWord:
		return stdCharClass<StdCharClassWord>();

	case TokenKind_StdCharClassSpace:
		return stdCharClass<StdCharClassSpace>();

	case TokenKind_StdCharClassNonDigit:
		return stdCharClass<StdCharClassNonDigit>();

	case TokenKind_StdCharClassNonHex:
		return stdCharClass<StdCharClassNonHex>();

	case TokenKind_StdCharClassNonWord:
		return stdCharClass<StdCharClassNonWord>();

	case TokenKind_StdCharClassNonSpace:
		return stdCharClass<StdCharClassNonSpace>();

	case TokenKind_AnchorBeginLine:
		return anchor(Anchor_BeginLine);

	case TokenKind_AnchorEndLine:
		return anchor(Anchor_EndLine);

	case TokenKind_AnchorBeginText:
		return anchor(Anchor_BeginText);

	case TokenKind_AnchorEndText:
		return anchor(Anchor_EndText);

	case TokenKind_AnchorWordBoundary:
		return anchor(Anchor_WordBoundary);

	case TokenKind_AnchorNotWordBoundary:
		return anchor(Anchor_NotWordBoundary);

	case TokenKind_Char:
		return ch(token.m_char);

	case TokenKind_AnyChar:
		return anyChar();

	default:
		ASSERT(false);
		err::setError("invalid regexp syntax");
		return NULL;
	}
}

NfaState*
Compiler::anchor(Anchor anchor) {
	Token token = nextToken();
	ASSERT(token >= TokenKind_FirstAnchor && token <= TokenKind_LastAnchor);

	NfaState* start = m_program->addState();
	NfaState* accept = m_program->addState();
	start->createMatchAnchor(anchor, accept);
	return start;
}

NfaState*
Compiler::ch(utf32_t c) {
	Token token = nextToken();
	ASSERT(token == TokenKind_Char && token.m_char == c);

	NfaState* start = m_program->addState();
	NfaState* accept = m_program->addState();
	start->createMatchChar(c, accept);
	return start;
}

NfaState*
Compiler::anyChar() {
	Token token = nextToken();
	ASSERT(token == TokenKind_AnyChar);

	NfaState* start = m_program->addState();
	NfaState* accept = m_program->addState();
	start->createMatchAnyChar(accept);
	return start;
}

template <typename IsNegated>
NfaState*
Compiler::charClass() {
	Token token = nextToken();
	ASSERT(
		IsNegated()() && token == TokenKind_NegatedCharClass ||
		!IsNegated()() && token == TokenKind_CharClass
	);

	NfaState* start = m_program->addState();
	NfaState* accept = m_program->addState();
	start->createMatchCharSet(accept);

	token = nextToken();
	while (token != TokenKind_EndCharClass) {
		switch (token) {
		case TokenKind_StdCharClassDigit:
			StdCharClassDigit()(start->m_charSet);
			break;

		case TokenKind_StdCharClassHex:
			StdCharClassHex()(start->m_charSet);
			break;

		case TokenKind_StdCharClassWord:
			StdCharClassWord()(start->m_charSet);
			break;

		case TokenKind_StdCharClassSpace:
			StdCharClassSpace()(start->m_charSet);
			break;

		case TokenKind_StdCharClassNonDigit:
			StdCharClassNonDigit()(start->m_charSet);
			break;

		case TokenKind_StdCharClassNonHex:
			StdCharClassNonHex()(start->m_charSet);
			break;

		case TokenKind_StdCharClassNonWord:
			StdCharClassNonWord()(start->m_charSet);
			break;

		case TokenKind_StdCharClassNonSpace:
			StdCharClassNonSpace()(start->m_charSet);
			break;

		case TokenKind_Dash: // dash NOT after a char
			start->m_charSet->add('-');
			break;

		case TokenKind_Char:
			utf32_t from;

			if (getToken() != TokenKind_Dash) {
				start->m_charSet->add(token.m_char);
				break;
			}

			from = token.m_char;
			nextToken();
			token = nextToken();
			if (token != TokenKind_Char) {
				err::setError("invalid character class");
				return NULL;
			}

			if (from <= token.m_char)
				start->m_charSet->add(from, token.m_char);
			else
				start->m_charSet->add(token.m_char, from);
			break;

		default:
			err::setError("invalid character class");
			return NULL;
		}

		token = nextToken();
	}

	if (IsNegated()())
		start->m_charSet->invert();

	if (start->m_charSet->isEmpty()) {
		err::setError("empty character class");
		return NULL;
	}

	return start;
}

template <typename StdCharClass>
NfaState*
Compiler::stdCharClass() {
	Token token = nextToken();
	ASSERT(token >= TokenKind_FirstStdCharClass && token <= TokenKind_LastStdCharClass);

	NfaState* start = m_program->addState();
	NfaState* accept = m_program->addState();
	start->createMatchCharSet(accept);
	StdCharClass()(start->m_charSet);
	return start;
}

NfaState*
Compiler::capturingGroup() {
	size_t captureId = m_program->m_captureCount++;

	NfaState* open = m_program->addState();
	NfaState* start = nonCapturingGroup();
	if (!start)
		return NULL;

	NfaState* innerAccept = m_program->getLastState();
	NfaState* close = m_program->addState();
	NfaState* outerAccept = m_program->addState();
	open->createOpenCapture(captureId, start);
	innerAccept->createEpsilon(close);
	close->createCloseCapture(captureId, outerAccept);
	return open;
}

NfaState*
Compiler::nonCapturingGroup() {
	Token token = nextToken();
	ASSERT(token == TokenKind_Group);

	NfaState* start = expression();
	if (!start)
		return NULL;

	bool result = expectToken(TokenKind_EndGroup);
	if (!result)
		return NULL;

	return start;
}

//..............................................................................

} // namespace re
} // namespace axl
