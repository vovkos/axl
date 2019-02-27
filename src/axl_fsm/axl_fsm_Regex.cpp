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
#include "axl_fsm_Regex.h"

namespace axl {
namespace fsm {

//..............................................................................

static
sl::String
getCharSetString(const sl::BitMap* charSet)
{
	sl::String string;

	uint_t c1 = -1;

	for (uint_t i = 0; i < 256; i++)
	{
		bool isSet = charSet->getBit(i);
		if (isSet)
		{
			if (c1 == -1)
			{
				c1 = i;
				string.appendFormat(isprint(c1) ? "%c" : "\\x%02x", c1);
			}
		}
		else if (c1 != -1)
		{
			uint_t c2 = i - 1;

			if (c2 > c1)
			{
				if (c2 > c1 + 1)
					string += '-';

				string.appendFormat(isprint(c2) ? "%c" : "\\x%02x", c2);
			}

			c1 = -1;
		}
	}

	if (c1 != -1)
	{
		uint_t c2 = 255;

		if (c2 > c1)
		{
			if (c2 > c1 + 1)
				string += '-';

			string.appendFormat(isprint(c2) ? "%c" : "\\x%02x", c2);
		}
	}

	return string;
}

static
sl::String
getMatchConditionString(const MatchCondition* condition)
{
	sl::String string;

	switch(condition->m_conditionKind)
	{
	case MatchConditionKind_Char:
		string.format(isprint(condition->m_char) ? "'%c'" : "'\\x%02x'", condition->m_char);
		break;

	case MatchConditionKind_CharSet:
		string.format("[%s]", getCharSetString (&condition->m_charSet).sz ());
		break;

	case MatchConditionKind_Any:
		string = "any";
		break;

	default:
		ASSERT(false);
	}

	return string;
}

//..............................................................................

void
Regex::clear()
{
	m_groupCount = 0;
	m_nfaStateList.clear();
	m_nfaStateArray.clear();
	m_dfaStateList.clear();
	m_dfaStateArray.clear();
}

bool
Regex::match(const sl::StringRef& string)
{
	return false;
}

bool
Regex::compile(const sl::StringRef& source)
{
	RegexCompiler compiler(this);
	return compiler.compile(source);
}

void
Regex::print() const
{
	printf("NFA\n");
	printf("==================\n");

	sl::ConstIterator<NfaState> nfaIt = m_nfaStateList.getHead();
	for (; nfaIt; nfaIt++)
	{
		const NfaState* nfaState = *nfaIt;
		printf(
			"%c %02d ",
			(nfaState->m_flags & NfaStateFlag_Accept) ? '*' : ' ',
			nfaState->m_id
			);

		if (nfaState->m_flags & NfaStateFlag_OpenCapture)
			printf("open (%d) ", nfaState->m_captureId);

		if (nfaState->m_flags & NfaStateFlag_CloseCapture)
			printf("close (%d) ", nfaState->m_captureId);

		if (nfaState->m_flags & NfaStateFlag_Match)
		{
			ASSERT(nfaState->m_outState && !nfaState->m_outState2);
			printf("%s -> %02d", getMatchConditionString (&nfaState->m_matchCondition).sz (), nfaState->m_outState->m_id);
		}
		else if (nfaState->m_flags & NfaStateFlag_EpsilonLink)
		{
			ASSERT(nfaState->m_outState);
			printf("eps -> %02d", nfaState->m_outState->m_id);

			if (nfaState->m_outState2)
				printf(", %02d", nfaState->m_outState2->m_id);
		}

		printf("\n");
	}

	printf("\nDFA\n");
	printf("==================\n");

	sl::ConstIterator<DfaState> dfaIt = m_dfaStateList.getHead();
	for (; dfaIt; dfaIt++)
	{
		const DfaState* dfaState = *dfaIt;
		printf(
			"%c %02d = { ",
			(dfaState->m_isAccept) ? '*' : ' ',
			dfaState->m_id
			);

		size_t count = dfaState->m_nfaStateSet.m_stateArray.getCount();
		for (size_t i = 0; i < count; i++)
		{
			NfaState* nfaState = dfaState->m_nfaStateSet.m_stateArray[i];
			printf("%02d ", nfaState->m_id);
		}

		printf("}");

		size_t i = dfaState->m_openCaptureIdSet.findBit(0);
		if (i != -1)
		{
			printf(" open ( ");

			do
			{
				printf("%d ", i);
				i = dfaState->m_openCaptureIdSet.findBit(i + 1);
			} while (i != -1);

			printf(")");
		}

		i = dfaState->m_closeCaptureIdSet.findBit(0);
		if (i != -1)
		{
			printf(" close ( ");

			do
			{
				printf("%d ", i);
				i = dfaState->m_closeCaptureIdSet.findBit(i + 1);
			} while (i != -1);

			printf(")");
		}

		printf("\n");


		sl::ConstIterator<DfaTransition> dfaTransitionIt = dfaState->m_transitionList.getHead();
		for (; dfaTransitionIt; dfaTransitionIt++)
		{
			const DfaTransition* transition = *dfaTransitionIt;
			printf("    %s -> %02d\n", getMatchConditionString (&transition->m_matchCondition).sz (), transition->m_outState->m_id);
		}
	}
}

//..............................................................................

static
void
initValidSingleTable(bool* table)
{
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
RegexCompiler::Token::isValidSingle()
{
	if (m_tokenKind != TokenKind_SpecialChar)
		return true;

	static bool validSingleTable[256] = { 0 };
	sl::callOnce(initValidSingleTable, validSingleTable);
	return validSingleTable[(uchar_t)m_char];
}

//..............................................................................

void
RegexCompiler::construct(
	uint_t flags,
	Regex* regex,
	RegexNameMgr* nameMgr
	)
{
	m_regex = regex;
	m_nameMgr = nameMgr;
	m_flags = flags;
	m_p = NULL;
	m_end = NULL;
	m_lastToken.m_tokenKind = TokenKind_Undefined;
	m_lastToken.m_char = 0;
}

bool
RegexCompiler::compile(
	const sl::StringRef& source,
	void* acceptContext
	)
{
	m_regex->clear();

	bool result = incrementalCompile(source, acceptContext);
	if (!result)
		return false;

	finalize();
	return true;
}

bool
RegexCompiler::incrementalCompile(
	const sl::StringRef& source,
	void* acceptContext
	)
{
	m_p = source.cp();
	m_end = source.getEnd();
	m_lastToken.m_tokenKind = TokenKind_Undefined;

	NfaState* oldStart = !m_regex->m_nfaStateList.isEmpty() ?
		*m_regex->m_nfaStateList.getHead() :
		NULL;

	NfaState* body = expression();
	if (!body)
		return false;

	bool result = expectEof();
	if (!result)
		return false;

	NfaState* accept = *m_regex->m_nfaStateList.getTail();
	accept->m_flags |= NfaStateFlag_Accept;
	accept->m_acceptContext = acceptContext;

	if (oldStart)
	{
		NfaState* split = AXL_MEM_NEW(NfaState);
		split->createEpsilonLink(oldStart, body);
		m_regex->m_nfaStateList.insertHead(split);
	}

	return true;
}

void
RegexCompiler::finalize()
{
	assignNfaIds();
	makeDfa();
	minimizeDfa();
	assignDfaIds();
}

void
RegexCompiler::assignNfaIds()
{
	size_t nfaStateCount = m_regex->m_nfaStateList.getCount();
	m_regex->m_nfaStateArray.setCount(nfaStateCount);

	sl::Iterator<NfaState> nfaIt = m_regex->m_nfaStateList.getHead();
	for (uint_t i = 0; nfaIt; i++, nfaIt++)
	{
		NfaState* nfaState = *nfaIt;
		nfaState->m_id = i;
		m_regex->m_nfaStateArray[i] = nfaState;
	}
}

void
RegexCompiler::assignDfaIds()
{
	size_t dfaStateCount = m_regex->m_dfaStateList.getCount();
	m_regex->m_dfaStateArray.setCount(dfaStateCount);

	sl::Iterator<DfaState> dfaIt = m_regex->m_dfaStateList.getHead();
	for (uint_t i = 0; dfaIt; i++, dfaIt++)
	{
		DfaState* state = *dfaIt;
		state->m_id = i;
		m_regex->m_dfaStateArray[i] = state;
	}
}

void
RegexCompiler::makeDfa()
{
	m_regex->m_dfaStateList.clear();

	if (m_regex->m_nfaStateList.isEmpty())
		return;

	sl::Array<DfaState*> workingSet;
	NfaStateSetMap<DfaState*> dfaStateMap;

	DfaState* dfaState = AXL_MEM_NEW(DfaState);
	dfaState->addNfaState(*m_regex->m_nfaStateList.getHead());
	dfaState->makeEpsilonClosure();

	m_regex->m_dfaStateList.insertTail(dfaState);
	workingSet.append(dfaState);
	dfaStateMap[&dfaState->m_nfaStateSet] = dfaState;

	NfaTransitionMgr nfaTransitionMgr;
	sl::BitMap tmpSet;

	while (!workingSet.isEmpty())
	{
		DfaState* dfaState = workingSet.getBackAndPop();

		nfaTransitionMgr.clear();

		size_t nfaStateCount = dfaState->m_nfaStateSet.m_stateArray.getCount();
		for (size_t i = 0; i < nfaStateCount; i++)
		{
			NfaState* nfaState = dfaState->m_nfaStateSet.m_stateArray[i];
			if (nfaState->m_flags & NfaStateFlag_Match)
				nfaTransitionMgr.addMatchState(nfaState);
		}

		nfaTransitionMgr.finalize();

		sl::ConstList<NfaTransition> nfaTransitionList = nfaTransitionMgr.getTransitionList();
		sl::ConstIterator<NfaTransition> nfaTransitionIt = nfaTransitionList.getHead();
		for (; nfaTransitionIt; nfaTransitionIt++)
		{
			const NfaTransition* nfaTransition = *nfaTransitionIt;

			DfaState* dfaState2 = AXL_MEM_NEW(DfaState);

			size_t outCount = nfaTransition->m_outStateSet.m_stateArray.getCount();
			for (size_t i = 0; i < outCount; i++)
			{
				NfaState* nfaState = nfaTransition->m_outStateSet.m_stateArray[i];
				dfaState2->addNfaState(nfaState);
			}

			dfaState2->makeEpsilonClosure();

			NfaStateSetMap<DfaState*>::Iterator mapIt = dfaStateMap.visit(&dfaState2->m_nfaStateSet);
			if (mapIt->m_value)
			{
				AXL_MEM_DELETE(dfaState2);
				dfaState2 = mapIt->m_value;
			}
			else
			{
				m_regex->m_dfaStateList.insertTail(dfaState2);
				workingSet.append(dfaState2);
				mapIt->m_value = dfaState2;
			}

			DfaTransition* dfaTransition = AXL_MEM_NEW(DfaTransition);
			dfaTransition->m_matchCondition = nfaTransition->m_matchCondition;
			dfaTransition->m_outState = dfaState2;
			dfaState->m_transitionList.insertTail(dfaTransition);
		}
	}
}

void
RegexCompiler::minimizeDfa()
{
}

bool
getHexValue(
	uint_t c,
	uint_t* value
	)
{
	if (c >= '0' && c <= '9')
		*value = c - '0';
	else if (c >= 'a' && c <= 'f')
		*value = 10 + c - 'a';
	else if (c >= 'A' && c <= 'F')
		*value = 10 + c - 'A';
	else
		return false;

	return true;
}

bool
RegexCompiler::readHexEscapeSequence(uchar_t* c)
{
	ASSERT(*m_p == 'x');
	m_p++;

	uint_t x1;
	uint_t x2;

	bool result =
		m_p + 1 < m_end &&
		getHexValue(m_p[0], &x1) &&
		getHexValue(m_p[1], &x2);

	if (!result)
	{
		err::setFormatStringError("invalid hex escape sequence");
		return false;
	}

	*c = x2 | (x1 << 4);
	m_p += 2;
	return true;
}

bool
RegexCompiler::readEscapeSequence(uchar_t* c)
{
	ASSERT(*m_p == '\\');
	m_p++;

	if (m_p >= m_end)
	{
		err::setError("invalid escape sequence");
		return false;
	}

	switch(*m_p)
	{
	case '0':
		*c = '\0';
		break;

	case 'a':
		*c = '\a';
		break; // 07

	case 'b':
		*c = '\b';
		break; // 08

	case 't':
		*c = '\t';
		break; // 09

	case 'n':
		*c = '\n';
		break; // 0A

	case 'v':
		*c = '\v';
		break; // 0B

	case 'f':
		*c = '\f';
		break; // 0C

	case 'r':
		*c = '\r';
		break; // 0D

	case 'e':
		*c = 0x1b;
		break;

	case 'x':
		return readHexEscapeSequence(c);

	default:
		*c = *m_p;
	}

	m_p++;
	return true;
}

bool
RegexCompiler::readLiteral(sl::String* string)
{
	char delimiter = *m_p++;
	ASSERT(delimiter == '"' || delimiter == '\'');

	for (const char* p = m_p;; p++)
	{
		if (p >= m_end)
		{
			err::setError("unclosed literal");
			return false;
		}

		switch(*p)
		{
		case '\\':
			if (++p >= m_end)
			{
				err::setError("invalid escape sequence");
				return false;
			}

			break;

		case '"':
		case '\'':
			if (*p == delimiter)
			{
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
RegexCompiler::readHexLiteral(sl::String* string)
{
	ASSERT(m_p[0] == '0' && m_p[1] == 'x');
	m_p += 2;

	char delimiter = *m_p++;
	ASSERT(delimiter == '"' || delimiter == '\'');

	for (const char* p = m_p;; p++)
	{
		if (p >= m_end)
		{
			err::setError("unclosed literal");
			return false;
		}

		switch(*p)
		{
		case '"':
		case '\'':
			if (*p == delimiter)
			{
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
RegexCompiler::readIdentifier(sl::String* name)
{
	ASSERT(isalpha((uchar_t)*m_p) || *m_p == '_');

	name->copy((uchar_t)*m_p++);

	while (m_p < m_end && isalnum((uchar_t)*m_p) || *m_p == '_')
	{
		name->append((uchar_t)*m_p);
		m_p++;
	}

	return true;
}

bool
RegexCompiler::readQuantifier(size_t* count)
{
	char buffer[16] = { 0 };
	for (
		size_t i = 0;
		i < countof(buffer) - 1 && m_p < m_end && isdigit((uchar_t)*m_p);
		i++, m_p++
		)
	{
		buffer[i] = *m_p;
	}

	char* end;
	long n  = strtoul(buffer, &end, 10);
	bool result = n >= 1 && n <= Const_MaxQuantifier;
	if (result)
		result = expectSpecialChar('}');

	if (!result)
	{
		err::setFormatStringError(
			"invalid quantifier; only simple quantifiers are supported: {n}, 1 <= n <= %d",
			Const_MaxQuantifier
			);
		return false;
	}

	*count = n;
	return true;
}

bool
RegexCompiler::getToken(Token* token)
{
	if (m_lastToken.m_tokenKind)
	{
		*token = m_lastToken;
		m_lastToken.m_tokenKind = TokenKind_Undefined;
		return true;
	}

	if (!(m_flags & Flag_SparseSyntax))
	{
		for (;;)
		{
			if (m_p >= m_end)
			{
				token->m_tokenKind = TokenKind_SpecialChar;
				token->m_char = 0; // don't advance m_p
				return true;
			}

			switch(*m_p)
			{
			case '\\':
				if (m_p + 1 >= m_end)
				{
					err::setError("invalid escape sequence");
					return false;
				}

				switch(m_p[1])
				{
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
					if (m_p + 2 < m_end && m_p[2] == '\n')
					{
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
				token->m_char = *m_p++;
				return true;
			}
		}
	}
	else
		for (;;)
		{
			if (m_p >= m_end)
			{
				token->m_tokenKind = TokenKind_SpecialChar;
				token->m_char = 0; // don't advance m_p
				return true;
			}

			switch(*m_p)
			{
			case '\\':
				if (m_p + 1 >= m_end)
				{
					err::setError("invalid escape sequence");
					return false;
				}

				switch(m_p[1])
				{
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
					if (m_p + 2 < m_end && m_p[2] == '\n')
					{
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
				if (m_p + 2 < m_end && m_p[1] == 'x' && (m_p[2] == '"' || m_p [2] == '\''))
				{
					token->m_tokenKind = TokenKind_Literal;
					return readHexLiteral(&token->m_string);
				}

			default:
				if (isalpha((uchar_t)*m_p) || *m_p == '_')
				{
					token->m_tokenKind = TokenKind_Identifier;
					return readIdentifier(&token->m_string);
				}

				token->m_tokenKind = TokenKind_Char;
				token->m_char = *m_p++;
				return true;
			}
		}
}

bool
RegexCompiler::expectSpecialChar(char c)
{
	Token token;
	bool result = getToken(&token);
	if (!result)
		return false;

	if (!token.isSpecialChar(c))
	{
		err::setFormatStringError("'%c' expected", c);
		return false;
	}

	return true;
}

bool
RegexCompiler::expectEof()
{
	Token token;
	bool result = getToken(&token);
	if (!result)
		return false;

	if (!token.isSpecialChar(0))
	{
		err::setError("invalid regexp syntax");
		return false;
	}

	return true;
}

NfaState*
RegexCompiler::expression()
{
	NfaState* op1 = concat();
	if (!op1)
		return NULL;

	Token token;
	bool result = getToken(&token);
	if (!result)
		return NULL;

	if (!token.isSpecialChar('|'))
	{
		m_lastToken = token; // unget token
		return op1;
	}

	NfaState* accept1 = *m_regex->m_nfaStateList.getTail();

	NfaState* op2 = expression();
	if (!op2)
		return NULL;

	NfaState* accept2 = *m_regex->m_nfaStateList.getTail();

	NfaState* start = AXL_MEM_NEW(NfaState);
	NfaState* accept = AXL_MEM_NEW(NfaState);

	// connect

	m_regex->m_nfaStateList.insertBefore(start, op1);
	m_regex->m_nfaStateList.insertTail(accept);

	start->createEpsilonLink(op1, op2);
	accept1->createEpsilonLink(accept);
	accept2->createEpsilonLink(accept);

	return start;
}

NfaState*
RegexCompiler::concat()
{
	NfaState* start = repeat();
	if (!start)
		return NULL;

	for (;;)
	{
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

		accept1->createEpsilonLink(op2);
	}

	return start;
}

NfaState*
RegexCompiler::repeat()
{
	NfaState* start = single();
	if (!start)
		return NULL;

	Token token;
	bool result = getToken(&token);
	if (!result)
		return NULL;

	if (token.m_tokenKind == TokenKind_SpecialChar)
		switch(token.m_char)
		{
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
RegexCompiler::question(NfaState* start)
{
	NfaState* accept = *m_regex->m_nfaStateList.getTail();

	NfaState* split = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertBefore(split, start);
	split->createEpsilonLink(start, accept);

	return split;
}

NfaState*
RegexCompiler::star(NfaState* start)
{
	NfaState* oldAccept = *m_regex->m_nfaStateList.getTail();

	NfaState* newAccept = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertTail(newAccept);

	NfaState* split = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertBefore(split, start);
	split->createEpsilonLink(start, newAccept);
	oldAccept->createEpsilonLink(start, newAccept);

	return split;
}

NfaState*
RegexCompiler::plus(NfaState* start)
{
	NfaState* oldAccept = *m_regex->m_nfaStateList.getTail();

	NfaState* newAccept = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertTail(newAccept);

	oldAccept->createEpsilonLink(start, newAccept);

	return start;
}

NfaState*
RegexCompiler::quantify(
	NfaState* start,
	size_t count
	)
{
	ASSERT(count <= Const_MaxQuantifier);

	NfaState* originalStart = start;
	NfaState* accept = *m_regex->m_nfaStateList.getTail();
	for (size_t i = 1; i < count; i++)
	{
		start = clone(start, accept);
		accept->createEpsilonLink(start);
		accept = *m_regex->m_nfaStateList.getTail();
	}

	return originalStart;
}

NfaState*
RegexCompiler::clone(
	NfaState* first,
	NfaState* last
	)
{
	sl::SimpleHashTable<NfaState*, NfaState*> stateMap;

	sl::Iterator<NfaState> end = m_regex->m_nfaStateList.getTail();
	sl::Iterator<NfaState> it = first;
	for (;;)
	{
		NfaState* oldState = *it;
		NfaState* newState = AXL_MEM_NEW(NfaState);
		*newState = *oldState;
		stateMap[oldState] = newState;
		m_regex->m_nfaStateList.insertTail(newState);

		if (it == last)
			break;

		it++;
	}

	it = end + 1;
	NfaState* result = *it;
	for (; it; it++)
	{
		NfaState* state = *it;

		sl::HashTableIterator<NfaState*, NfaState*> mapIt;
		if (state->m_outState)
		{
			mapIt = stateMap.find(state->m_outState);
			if (mapIt)
				state->m_outState = mapIt->m_value;
		}

		if (state->m_outState2)
		{
			mapIt = stateMap.find(state->m_outState2);
			if (mapIt)
				state->m_outState2 = mapIt->m_value;
		}
	}

	return result;
}

NfaState*
RegexCompiler::single()
{
	bool result;

	Token token;
	result = getToken(&token);
	if (!result)
		return NULL;

	switch(token.m_tokenKind)
	{
	case TokenKind_SpecialChar:
		switch(token.m_char)
		{
		case '(':
			if (m_p + 1 < m_end && m_p[0] == '?' && m_p[1] == ':')
			{
				m_p += 2;
				return nonCapturingGroup();
			}
			else if (m_flags & Flag_NonCapturingGroups)
			{
				return nonCapturingGroup();
			}
			else
			{
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
RegexCompiler::literal(const sl::StringRef& string)
{
	NfaState* start = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertTail(start);

	NfaState* mid = start;
	const char* p = string.cp();
	const char* end = string.getEnd();
	for (; p < end; p++)
	{
		ch((uchar_t)*p, mid);
		mid = *m_regex->m_nfaStateList.getTail();
	}

	return start;
}

NfaState*
RegexCompiler::ch(uint_t c)
{
	NfaState* start = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertTail(start);
	ch(c, start);
	return start;
}

void
RegexCompiler::ch(
	uint_t c,
	NfaState* start
	)
{
	NfaState* accept = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertTail(accept);
	start->createCharMatch(c, accept);
}

NfaState*
RegexCompiler::charClass()
{
	bool result;

	bool isInversed = false;

	if (m_p < m_end && *m_p == '^')
	{
		isInversed = true;
		m_p++;
	}

	NfaState* start = AXL_MEM_NEW(NfaState);
	start->m_flags |= NfaStateFlag_Match;
	start->m_matchCondition.m_conditionKind = MatchConditionKind_CharSet;
	start->m_matchCondition.m_charSet.setBitCount(256);
	m_regex->m_nfaStateList.insertTail(start);

	size_t count = 0;

	for (;;)
	{
		if (m_p >= m_end)
		{
			err::setError("invalid character class");
			return NULL;
		}

		if (*m_p == ']')
		{
			m_p++;
			break;
		}

		result = charClassItem(&start->m_matchCondition.m_charSet);
		if (!result)
			return NULL;

		count++;
	}

	if (!count)
	{
		err::setError("empty character class");
		return NULL;
	}

	if (isInversed) // invert range
		start->m_matchCondition.m_charSet.inverse();

	NfaState* accept = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertTail(accept);

	start->m_outState = accept;
	return start;
}

bool
RegexCompiler::charClassItem(sl::BitMap* charSet)
{
	ASSERT(m_p < m_end);

	bool result;

	uchar_t c1;
	uchar_t c2;

	switch(*m_p)
	{
	case '\\':
		if (m_p + 1 >= m_end)
		{
			err::setError("invalid escape sequence");
			return false;
		}

		switch(m_p[1])
		{
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
		c1 = *m_p++;
	}

	if (m_p >= m_end || *m_p != '-')
	{
		charSet->setBit(c1);
		return true;
	}

	m_p++;

	if (m_p >= m_end)
	{
		err::setError("invalid char class");
		return false;
	}

	switch(*m_p)
	{
	case '\\':
		result = readEscapeSequence(&c2);
		if (!result)
			return false;

		break;

	default:
		c2 = *m_p++;
	}

	if (c2 < c1)
	{
		char t = c2;
		c2 = c1;
		c1 = t;
	}

	charSet->setBitRange(c1, c2 + 1);
	return true;
}

NfaState*
RegexCompiler::stdCharClass(uint_t c)
{
	NfaState* start = AXL_MEM_NEW(NfaState);
	start->m_flags |= NfaStateFlag_Match;
	start->m_matchCondition.m_conditionKind = MatchConditionKind_CharSet;
	start->m_matchCondition.m_charSet.setBitCount(256);
	m_regex->m_nfaStateList.insertTail(start);

	stdCharClass(c, &start->m_matchCondition.m_charSet);

	NfaState* accept = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertTail(accept);

	start->m_outState = accept;
	return start;
}

void
RegexCompiler::stdCharClass(
	uint_t c,
	sl::BitMap* charSet
	)
{
	bool isInverted = false;

	switch(c)
	{
	case 'D':
		isInverted = true;
		// and fall through

	case 'd':
		charSet->setBitRange('0', '9' + 1);
		break;

	case 'H':
		isInverted = true;
		// and fall through

	case 'h':
		charSet->setBitRange('0', '9' + 1);
		charSet->setBitRange('a', 'f' + 1);
		charSet->setBitRange('A', 'F' + 1);
		break;

	case 'W':
		isInverted = true;
		// and fall through

	case 'w':
		charSet->setBitRange('a', 'z' + 1);
		charSet->setBitRange('A', 'Z' + 1);
		charSet->setBitRange('0', '9' + 1);
		charSet->setBit('_');
		break;

	case 'S':
		isInverted = true;
		// and fall through

	case 's':
		charSet->setBit(' ');
		charSet->setBit('\t');
		charSet->setBit('\r');
		charSet->setBit('\n');
		break;

	default:
		ASSERT(false);
	}

	if (isInverted)
		charSet->inverse();
}

NfaState*
RegexCompiler::any()
{
	NfaState* start = AXL_MEM_NEW(NfaState);
	start->m_flags |= NfaStateFlag_Match;
	start->m_matchCondition.m_conditionKind = MatchConditionKind_Any;
	m_regex->m_nfaStateList.insertTail(start);

	NfaState* accept = AXL_MEM_NEW(NfaState);
	m_regex->m_nfaStateList.insertTail(accept);

	start->m_outState = accept;
	return start;
}

NfaState*
RegexCompiler::capturingGroup()
{
	size_t captureId = m_regex->m_groupCount++;

	NfaState* open = AXL_MEM_NEW(NfaState);
	open->m_flags |= NfaStateFlag_OpenCapture;
	open->m_captureId = captureId;
	m_regex->m_nfaStateList.insertTail(open);

	NfaState* start = nonCapturingGroup();
	if (!start)
		return NULL;

	NfaState* accept = *m_regex->m_nfaStateList.getTail();

	NfaState* close = AXL_MEM_NEW(NfaState);
	close->m_flags |= NfaStateFlag_CloseCapture;
	close->m_captureId = captureId;
	m_regex->m_nfaStateList.insertTail(close);

	open->createEpsilonLink(start);
	accept->createEpsilonLink(close);

	return open;
}

NfaState*
RegexCompiler::nonCapturingGroup()
{
	NfaState* start = expression();
	if (!start)
		return NULL;

	bool result = expectSpecialChar(')');
	if (!result)
		return NULL;

	return start;
}

NfaState*
RegexCompiler::namedRegex(const sl::StringRef& name)
{
	if (!m_nameMgr)
	{
		err::setFormatStringError("named regexp '%s' is used but name manager is not set", name.sz ());
		return NULL;
	}

	sl::StringRef source = m_nameMgr->findName(name);
	if (source.isEmpty())
	{
		err::setFormatStringError("named regexp '%s' is not defined", name.sz ());
		return NULL;
	}

	Regex subRegex;
	RegexCompiler subRegexCompiler(Flag_NonCapturingGroups, &subRegex, m_nameMgr);

	bool result = subRegexCompiler.compile(source);
	if (!result)
		return NULL;

	NfaState* start = *subRegex.m_nfaStateList.getHead();
	NfaState* accept = *subRegex.m_nfaStateList.getTail();

	ASSERT(accept->m_flags & NfaStateFlag_Accept);
	accept->m_flags &= ~NfaStateFlag_Accept;

	m_regex->m_nfaStateList.insertListTail(&subRegex.m_nfaStateList);
	return start;
}

//..............................................................................

} // namespace fsm
} // namespace axl
