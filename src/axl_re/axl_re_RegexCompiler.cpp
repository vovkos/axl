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
#include "axl_re_RegexCompiler.h"
#include "axl_enc_EscapeEncoding.h"

namespace axl {
namespace re {

//..............................................................................

static
void
initValidSingleTable(bool* table) {
	table['.'] = true;
	table['['] = true;
	table['('] = true;
	table['^'] = true;
	table['$'] = true;
	table['d'] = true;
	table['D'] = true;
	table['h'] = true;
	table['H'] = true;
	table['w'] = true;
	table['W'] = true;
	table['s'] = true;
	table['S'] = true;
}

bool
RegexCompiler::Token::isValidSingle() {
	if (m_tokenKind != TokenKind_SpecialChar)
		return true;

	static bool validSingleTable[256] = { 0 };
	sl::callOnce(initValidSingleTable, validSingleTable);
	return validSingleTable[(uchar_t)m_char];
}

//..............................................................................

inline
NfaState*
RegexCompiler::addState() {
	NfaState* state = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertTail(state);
	return state;
}

inline
NfaState*
RegexCompiler::insertState(NfaState* beforeState) {
	NfaState* state = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertBefore(state, beforeState);
	return state;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
RegexCompiler::construct(
	uint_t flags,
	Regex* regex,
	RegexNameMgr* nameMgr
) {
	m_regex = regex;
	m_nameMgr = nameMgr;
	m_flags = flags;
	m_p = NULL;
	m_end = NULL;
	m_lastToken.m_tokenKind = TokenKind_Undefined;
	m_lastToken.m_char = 0;
}

NfaState*
RegexCompiler::compile(
	const sl::StringRef& source,
	size_t acceptId
) {
	m_p = source.cp();
	m_end = source.getEnd();
	m_lastToken.m_tokenKind = TokenKind_Undefined;

	NfaState* oldStart = !m_regex->m_nfaStateList.isEmpty() ? *m_regex->m_nfaStateList.getHead() : NULL;
	NfaState* body = expression();
	if (!body)
		return NULL;

	bool result = expectEof();
	if (!result)
		return NULL;

	NfaState* accept = *m_regex->m_nfaStateList.getTail();
	accept->createAccept(acceptId);

	if (oldStart) {
		NfaState* split = addState();
		split->createSplit(oldStart, body);
	}

	return body;
}

NfaState*
RegexCompiler::compileSwitchCase(
	const sl::StringRef& source,
	size_t acceptId
) {
	if (m_regex->m_nfaStateList.isEmpty())
		return compile(source, acceptId);

	NfaState* prevStart = *m_regex->m_nfaStateList.getHead();
	NfaState* caseStart = compile(source, acceptId);
	NfaState* split = insertState(prevStart);
	split->createSplit(prevStart, caseStart);
	return caseStart;
}

bool
RegexCompiler::readChar(utf32_t* c) {
	size_t length = enc::Utf8::getDecodeCodePointLength(*m_p);
	if (length > (size_t)(m_end - m_p)) {
		err::setError("incomplete UTF8 code-point");
		return false;
	}

	*c = enc::Utf8::decodeCodePoint(m_p);
	m_p += length;
	return true;
}

bool
RegexCompiler::readEscapeSequence(utf32_t* p) {
	ASSERT(*m_p == '\\');
	size_t length = enc::EscapeEncoding::decodeEscape(p, sl::StringRef(m_p, m_end - m_p));
	if (length == -1)
		return false;

	m_p += length;
	return true;
}

bool
RegexCompiler::readLiteral(sl::String* string) {
	char delimiter = *m_p++;
	ASSERT(delimiter == '"' || delimiter == '\'');

	for (const char* p = m_p;; p++) {
		if (p >= m_end) {
			err::setError("unclosed literal");
			return false;
		}

		switch (*p) {
		case '\\':
			if (++p >= m_end) {
				err::setError("invalid escape sequence");
				return false;
			}

			break;

		case '"':
		case '\'':
			if (*p == delimiter) {
				size_t size = enc::EscapeEncoding::decode(string, sl::StringRef(m_p, p - m_p));
				if (size == -1)
					return false;

				m_p = p + 1;
				return true;
			}
		}
	}
}

bool
RegexCompiler::readHexLiteral(sl::String* string) {
	ASSERT(m_p[0] == '0' && m_p[1] == 'x');
	m_p += 2;

	char delimiter = *m_p++;
	ASSERT(delimiter == '"' || delimiter == '\'');

	for (const char* p = m_p;; p++) {
		if (p >= m_end) {
			err::setError("unclosed literal");
			return false;
		}

		switch (*p) {
		case '"':
		case '\'':
			if (*p == delimiter) {
				sl::Array<char> buffer;
				size_t size = enc::HexEncoding::decode(&buffer, sl::StringRef(m_p, p - m_p));
				if (size == -1)
					return false;

				string->copy(buffer, size);
				m_p = p + 1;
				return true;
			}
		}
	}
}

bool
RegexCompiler::readIdentifier(sl::String* name) {
	ASSERT(isalpha((uchar_t)*m_p) || *m_p == '_');

	name->copy((uchar_t)*m_p++);

	while (m_p < m_end && isalnum((uchar_t)*m_p) || *m_p == '_')
		name->append((uchar_t)*m_p++);

	return true;
}

bool
RegexCompiler::readQuantifier(size_t* count) {
	char buffer[16] = { 0 };
	for (
		size_t i = 0;
		i < countof(buffer) - 1 && m_p < m_end && isdigit((uchar_t)*m_p);
		i++, m_p++
	) {
		buffer[i] = *m_p;
	}

	char* end;
	long n  = strtoul(buffer, &end, 10);
	bool result = n >= 1 && n <= MaxQuantifier;
	if (result)
		result = expectSpecialChar('}');

	if (!result) {
		err::setFormatStringError(
			"invalid quantifier; only simple quantifiers are supported: {n}, 1 <= n <= %d",
			MaxQuantifier
		);
		return false;
	}

	*count = n;
	return true;
}

bool
RegexCompiler::getToken(Token* token) {
	if (m_lastToken.m_tokenKind) {
		*token = m_lastToken;
		m_lastToken.m_tokenKind = TokenKind_Undefined;
		return true;
	}

	if (!(m_flags & RegexCompileFlag_SparseSyntax)) {
		for (;;) {
			if (m_p >= m_end) {
				token->m_tokenKind = TokenKind_SpecialChar;
				token->m_char = 0; // don't advance m_p
				return true;
			}

			switch (*m_p) {
			case '\\':
				if (m_p + 1 >= m_end) {
					err::setError("invalid escape sequence");
					return false;
				}

				switch (m_p[1]) {
				case 'b':
					token->m_tokenKind = TokenKind_WordBoundary;
					m_p += 2;
					return true;

				case 's':
				case 'S':
				case 'd':
				case 'D':
				case 'h':
				case 'H':
				case 'w':
				case 'W':
					token->m_tokenKind = TokenKind_SpecialChar;
					token->m_char = m_p[1];
					m_p += 2;
					return true;

				case '\n':
					m_p += 2; // ignore line continuation (LF)
					break;

				case '\r':
					if (m_p + 2 < m_end && m_p[2] == '\n') {
						m_p += 3; // line continuation (CR-LF)
						break;
					}

				default:
					token->m_tokenKind = TokenKind_Char;
					return readEscapeSequence(&token->m_char);
				}

				break;

			case '.':
			case '?':
			case '*':
			case '+':
			case '[':
			case ']':
			case '(':
			case ')':
			case '|':
			case '^':
			case '$':
			case '{':
			case '}':
				token->m_tokenKind = TokenKind_SpecialChar;
				token->m_char = *m_p++;
				return true;

			default:
				token->m_tokenKind = TokenKind_Char;
				return readChar(&token->m_char);
			}
		}
	} else {
		for (;;) {
			if (m_p >= m_end) {
				token->m_tokenKind = TokenKind_SpecialChar;
				token->m_char = 0; // don't advance m_p
				return true;
			}

			switch (*m_p) {
			case '\\':
				if (m_p + 1 >= m_end) {
					err::setError("invalid escape sequence");
					return false;
				}

				switch (m_p[1]) {
				case 's':
				case 'S':
				case 'd':
				case 'D':
				case 'h':
				case 'H':
				case 'w':
				case 'W':
					token->m_tokenKind = TokenKind_SpecialChar;
					token->m_char = m_p[1];
					m_p += 2;
					return true;

				case '\n':
					m_p += 2; // ignore line continuation (LF)
					break;

				case '\r':
					if (m_p + 2 < m_end && m_p[2] == '\n') {
						m_p += 3; // line continuation (CR-LF)
						break;
					}

				default:
					token->m_tokenKind = TokenKind_Char;
					return readEscapeSequence(&token->m_char);
				}

				break;

			case '"':
			case '\'':
				token->m_tokenKind = TokenKind_Literal;
				return readLiteral(&token->m_string);

			case ' ':
			case '\t':
			case '\r':
			case '\n':
				m_p++; // ignore whitespace
				break;

			case '.':
			case '?':
			case '*':
			case '+':
			case '[':
			case ']':
			case '(':
			case ')':
			case '|':
			case '^':
			case '$':
			case '{':
			case '}':
				token->m_tokenKind = TokenKind_SpecialChar;
				token->m_char = *m_p++;
				return true;

			case '0':
				if (m_p + 2 < m_end && m_p[1] == 'x' && (m_p[2] == '"' || m_p [2] == '\'')) {
					token->m_tokenKind = TokenKind_Literal;
					return readHexLiteral(&token->m_string);
				}

			default:
				if (isalpha((uchar_t)*m_p) || *m_p == '_') {
					token->m_tokenKind = TokenKind_Identifier;
					return readIdentifier(&token->m_string);
				}

				token->m_tokenKind = TokenKind_Char;
				return readChar(&token->m_char);
			}
		}
	}
}

bool
RegexCompiler::expectSpecialChar(char c) {
	Token token;
	bool result = getToken(&token);
	if (!result)
		return false;

	if (!token.isSpecialChar(c)) {
		err::setFormatStringError("'%c' expected", c);
		return false;
	}

	return true;
}

bool
RegexCompiler::expectEof() {
	Token token;
	bool result = getToken(&token);
	if (!result)
		return false;

	if (!token.isSpecialChar(0)) {
		err::setError("invalid regexp syntax");
		return false;
	}

	return true;
}

NfaState*
RegexCompiler::expression() {
	NfaState* op1 = concat();
	if (!op1)
		return NULL;

	Token token;
	bool result = getToken(&token);
	if (!result)
		return NULL;

	if (!token.isSpecialChar('|')) {
		m_lastToken = token; // unget token
		return op1;
	}

	NfaState* accept1 = *m_regex->m_nfaStateList.getTail();
	NfaState* op2 = expression();
	if (!op2)
		return NULL;

	NfaState* accept2 = *m_regex->m_nfaStateList.getTail();
	NfaState* start = insertState(op1);
	NfaState* accept = addState();
	start->createSplit(op1, op2);
	accept1->createEpsilon(accept);
	accept2->createEpsilon(accept);
	return start;
}

NfaState*
RegexCompiler::concat() {
	NfaState* start = repeat();
	if (!start)
		return NULL;

	for (;;) {
		Token token;
		bool result = getToken(&token);
		if (!result)
			return NULL;

		m_lastToken = token; // unget token

		if (!token.isValidSingle())
			break;

		NfaState* accept1 = *m_regex->m_nfaStateList.getTail();

		NfaState* op2 = repeat();
		if (!op2)
			return NULL;

		accept1->createEpsilon(op2);
	}

	return start;
}

NfaState*
RegexCompiler::repeat() {
	NfaState* start = single();
	if (!start)
		return NULL;

	Token token;
	bool result = getToken(&token);
	if (!result)
		return NULL;

	if (token.m_tokenKind == TokenKind_SpecialChar)
		switch (token.m_char) {
		case '?':
			return question(start);

		case '*':
			return star(start);

		case '+':
			return plus(start);

		case '{':
			size_t count;
			result = readQuantifier(&count);
			return result ? quantify(start, count) : NULL;
		}

	m_lastToken = token; // unget token
	return start;
}

NfaState*
RegexCompiler::question(NfaState* start) {
	NfaState* oldAccept = *m_regex->m_nfaStateList.getTail();
	NfaState* newAccept = addState();
	NfaState* split = insertState(start);
	split->createSplit(start, newAccept);
	oldAccept->createEpsilon(newAccept);
	return split;
}

NfaState*
RegexCompiler::star(NfaState* start) {
	NfaState* oldAccept = *m_regex->m_nfaStateList.getTail();
	NfaState* newAccept = addState();
	NfaState* split = insertState(start);
	split->createSplit(start, newAccept);
	oldAccept->createEpsilon(split);
	return split;
}

NfaState*
RegexCompiler::plus(NfaState* start) {
	NfaState* oldAccept = *m_regex->m_nfaStateList.getTail();
	NfaState* newAccept = addState();
	oldAccept->createSplit(start, newAccept);
	return start;
}

NfaState*
RegexCompiler::quantify(
	NfaState* start,
	size_t count
) {
	ASSERT(count <= MaxQuantifier);

	NfaState* originalStart = start;
	NfaState* accept = *m_regex->m_nfaStateList.getTail();
	for (size_t i = 1; i < count; i++) {
		start = clone(start, accept);
		accept->createEpsilon(start);
		accept = *m_regex->m_nfaStateList.getTail();
	}

	return originalStart;
}

NfaState*
RegexCompiler::clone(
	NfaState* first,
	NfaState* last
) {
	sl::SimpleHashTable<NfaState*, NfaState*> stateMap;

	sl::Iterator<NfaState> end = m_regex->m_nfaStateList.getTail();
	sl::Iterator<NfaState> it = first;
	for (;;) {
		NfaState* oldState = *it;
		NfaState* newState = addState();
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
RegexCompiler::single() {
	bool result;

	Token token;
	result = getToken(&token);
	if (!result)
		return NULL;

	switch (token.m_tokenKind) {
	case TokenKind_WordBoundary:
		return anchor(Anchor_Word);

	case TokenKind_SpecialChar:
		switch (token.m_char) {
		case '(':
			if (m_p + 1 < m_end && m_p[0] == '?' && m_p[1] == ':') {
				m_p += 2;
				return nonCapturingGroup();
			} else if (m_flags & RegexCompileFlag_DisableCapture) {
				return nonCapturingGroup();
			} else {
				return capturingGroup();
			}

		case '[':
			return charClass();

		case 'd':
		case 'D':
		case 'h':
		case 'H':
		case 'w':
		case 'W':
		case 's':
		case 'S':
			return stdCharClass(token.m_char);

		case '.':
			return any();

		case '^':
			return anchor(Anchor_Begin);

		case '$':
			return anchor(Anchor_End);

		default:
			err::setError("invalid regexp syntax");
			return NULL;
		}

	case TokenKind_Char:
		return ch(token.m_char);

	case TokenKind_Literal:
		return literal(token.m_string);

	case TokenKind_Identifier:
		return namedRegex(token.m_string);

	default:
		err::setError("invalid regexp syntax");
		return NULL;
	}
}

NfaState*
RegexCompiler::literal(const sl::StringRef& string) {
	NfaState* start = addState();
	NfaState* mid = start;

	const char* p = string.cp();
	const char* end = string.getEnd();
	for (; p < end; p++) {
		ch((uchar_t)*p, mid);
		mid = *m_regex->m_nfaStateList.getTail();
	}

	return start;
}

NfaState*
RegexCompiler::anchor(Anchor anchor) {
	NfaState* start = addState();
	NfaState* accept = addState();
	start->createMatchAnchor(anchor, accept);
	return start;
}

NfaState*
RegexCompiler::ch(uint_t c) {
	NfaState* start = addState();
	ch(c, start);
	return start;
}

void
RegexCompiler::ch(
	uint_t c,
	NfaState* start
) {
	NfaState* accept = addState();
	start->createMatchChar(c, accept);
}

NfaState*
RegexCompiler::charClass() {
	bool result;

	bool isInversed = false;

	if (m_p < m_end && *m_p == '^') {
		isInversed = true;
		m_p++;
	}

	NfaState* start = addState();
	NfaState* accept = addState();
	start->createMatchCharSet(accept);

	size_t count = 0;

	for (;;) {
		if (m_p >= m_end) {
			err::setError("invalid character class");
			return NULL;
		}

		if (*m_p == ']') {
			m_p++;
			break;
		}

		result = charClassItem(start->m_charSet);
		if (!result)
			return NULL;

		count++;
	}

	if (!count) {
		err::setError("empty character class");
		return NULL;
	}

	if (isInversed)
		start->m_charSet->invert();

	return start;
}

bool
RegexCompiler::charClassItem(CharSet* charSet) {
	ASSERT(m_p < m_end);

	bool result;

	utf32_t c1;
	utf32_t c2;

	switch (*m_p) {
	case '\\':
		if (m_p + 1 >= m_end) {
			err::setError("invalid escape sequence");
			return false;
		}

		switch (m_p[1]) {
		case 'd':
		case 'D':
		case 'h':
		case 'H':
		case 'w':
		case 'W':
		case 's':
		case 'S':
			stdCharClass(m_p[1], charSet);
			m_p += 2;
			return true;
		}

		result = readEscapeSequence(&c1);
		if (!result)
			return false;

		break;

	default:
		result = readChar(&c1);
		if (!result)
			return false;
	}

	if (m_p >= m_end || *m_p != '-') {
		charSet->add(c1);
		return true;
	}

	m_p++;

	if (m_p >= m_end) {
		err::setError("invalid char class");
		return false;
	}

	switch (*m_p) {
	case '\\':
		result = readEscapeSequence(&c2);
		if (!result)
			return false;

		break;

	default:
		result = readChar(&c2);
		if (!result)
			return false;
	}

	if (c2 < c1) {
		char t = c2;
		c2 = c1;
		c1 = t;
	}

	charSet->add(c1, c2);
	return true;
}

NfaState*
RegexCompiler::stdCharClass(char c) {
	NfaState* start = addState();
	NfaState* accept = addState();
	start->createMatchCharSet(accept);
	stdCharClass(c, start->m_charSet);
	return start;
}

void
RegexCompiler::stdCharClass(
	char c,
	CharSet* charSet
) {
	CharSet invertedCharSet;

	switch (c) {
	case 'D':
		invertedCharSet.add('0', '9');
		break;

	case 'd':
		charSet->add('0', '9');
		break;

	case 'H':
		invertedCharSet.add('0', '9');
		invertedCharSet.add('a', 'f');
		invertedCharSet.add('A', 'F');
		break;

	case 'h':
		charSet->add('0', '9');
		charSet->add('a', 'f');
		charSet->add('A', 'F');
		break;

	case 'W':
		invertedCharSet.add('a', 'z');
		invertedCharSet.add('A', 'Z');
		invertedCharSet.add('0', '9');
		invertedCharSet.add('_');
		break;

	case 'w':
		charSet->add('a', 'z');
		charSet->add('A', 'Z');
		charSet->add('0', '9');
		charSet->add('_');
		break;

	case 'S':
		invertedCharSet.add(' ');
		invertedCharSet.add('\t');
		invertedCharSet.add('\r');
		invertedCharSet.add('\n');
		break;

	case 's':
		charSet->add(' ');
		charSet->add('\t');
		charSet->add('\r');
		charSet->add('\n');
		break;

	default:
		ASSERT(false);
	}

	if (!invertedCharSet.isEmpty()) {
		invertedCharSet.invert();
		charSet->add(invertedCharSet);
	}
}

NfaState*
RegexCompiler::any() {
	NfaState* start = addState();
	NfaState* accept = addState();
	start->createMatchAnyChar(accept);
	return start;
}

NfaState*
RegexCompiler::capturingGroup() {
	size_t captureId = m_regex->m_captureCount++;

	NfaState* open = addState();
	NfaState* start = nonCapturingGroup();
	if (!start)
		return NULL;

	NfaState* innerAccept = *m_regex->m_nfaStateList.getTail();
	NfaState* close = addState();
	NfaState* outerAccept = addState();
	open->createOpenCapture(captureId, start);
	innerAccept->createEpsilon(close);
	close->createCloseCapture(captureId, outerAccept);
	return open;
}

NfaState*
RegexCompiler::nonCapturingGroup() {
	NfaState* start = expression();
	if (!start)
		return NULL;

	bool result = expectSpecialChar(')');
	if (!result)
		return NULL;

	return start;
}

NfaState*
RegexCompiler::namedRegex(const sl::StringRef& name) {
	if (!m_nameMgr) {
		err::setFormatStringError("named regexp '%s' is used but name manager is not set", name.sz());
		return NULL;
	}

	sl::StringRef source = m_nameMgr->findName(name);
	if (source.isEmpty()) {
		err::setFormatStringError("named regexp '%s' is not defined", name.sz());
		return NULL;
	}

	Regex subRegex;
	RegexCompiler subRegexCompiler(RegexCompileFlag_DisableCapture, &subRegex, m_nameMgr);
	bool result = subRegexCompiler.compile(source, NULL);
	if (!result)
		return NULL;

	NfaState* start = *subRegexCompiler.m_regex->m_nfaStateList.getHead();
	NfaState* accept = *subRegexCompiler.m_regex->m_nfaStateList.getTail();

	ASSERT(accept->m_stateKind == NfaStateKind_Accept);
	accept->m_stateKind = NfaStateKind_Undefined;

	m_regex->m_nfaStateList.insertListTail(&subRegex.m_nfaStateList);
	return start;
}

//..............................................................................

} // namespace re
} // namespace axl
