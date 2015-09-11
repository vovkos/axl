#include "pch.h"
#include "axl_fsm_RegExp.h"
#include "axl_err_Error.h"
#include "axl_rtl_String.h"
#include "axl_mt_CallOnce.h"

namespace axl {
namespace fsm {
	
//.............................................................................

static
rtl::String 
getCharSetString (const rtl::BitMap* charSet)
{
	rtl::String string;

	uint_t c1 = -1;

	for (uint_t i = 0; i < 256; i++)
	{
		bool isSet = charSet->getBit (i);
		if (isSet)
		{
			if (c1 == -1)
			{
				c1 = i;
				string.appendFormat (isprint (c1) ? "%c" : "\\x%02x", c1);
			}
		}
		else if (c1 != -1)
		{
			uint_t c2 = i - 1;

			if (c2 > c1)
			{
				if (c2 > c1 + 1)
					string += '-';

				string.appendFormat (isprint (c2) ? "%c" : "\\x%02x", c2);
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

			string.appendFormat (isprint (c2) ? "%c" : "\\x%02x", c2);
		}
	}

	return string;
}

static
rtl::String 
getMatchConditionString (const MatchCondition* condition)
{
	rtl::String string;
	
	switch (condition->m_conditionKind)
	{
	case MatchConditionKind_Char:
		string.format (isprint (condition->m_char) ? "'%c'" : "'\\x%02x'", condition->m_char);
		break;

	case MatchConditionKind_CharSet:
		string.format ("[%s]", getCharSetString (&condition->m_charSet).cc ());
		break;

	case MatchConditionKind_Any:
		string = "any";
		break;

	default:
		ASSERT (false);
	}

	return string;
}

//.............................................................................

void
RegExp::clear ()
{
	m_nfaStateList.clear ();
	m_nfaStateArray.clear ();
	m_dfaStateList.clear ();
	m_dfaStateArray.clear ();
}

bool 
RegExp::match (char const* string)
{
	return false;
}

bool 
RegExp::compile (char const* source)
{
	RegExpCompiler compiler (this);
	return compiler.compile (source);
}

void
RegExp::print () const
{
	printf ("NFA\n");
	printf ("==================\n");

	rtl::Iterator <NfaState> nfaIt = m_nfaStateList.getHead ();
	for (; nfaIt; nfaIt++)
	{
		NfaState* nfaState = *nfaIt;
		printf (
			"%c %02d ", 
			(nfaState->m_flags & NfaStateFlag_Accept) ? '*' : ' ',
			nfaState->m_id
			);

		if (nfaState->m_flags & NfaStateFlag_Match)
		{
			ASSERT (nfaState->m_outState && !nfaState->m_outState2);
			printf ("%s -> %02d", getMatchConditionString (&nfaState->m_matchCondition).cc (), nfaState->m_outState->m_id);
		}
		else if (nfaState->m_flags & NfaStateFlag_EpsilonLink)
		{
			ASSERT (nfaState->m_outState);
			printf ("eps -> %02d", nfaState->m_outState->m_id);

			if (nfaState->m_outState2)
				printf (", %02d", nfaState->m_outState2->m_id);
		}

		printf ("\n");
	}

	printf ("\nDFA\n");
	printf ("==================\n");

	rtl::Iterator <DfaState> dfaIt = m_dfaStateList.getHead ();
	for (; dfaIt; dfaIt++)
	{
		DfaState* dfaState = *dfaIt;
		printf (
			"%c %02d = { ", 
			(dfaState->m_isAccept) ? '*' : ' ',
			dfaState->m_id
			);

		size_t nfaStateCount = dfaState->m_nfaStateSet.m_stateArray.getCount ();
		for (size_t i = 0; i < nfaStateCount; i++)
		{
			NfaState* nfaState = dfaState->m_nfaStateSet.m_stateArray [i];
			printf ("%02d ", nfaState->m_id);
		}

		printf ("}\n");

		rtl::Iterator <DfaTransition> dfaTransitionIt = dfaState->m_transitionList.getHead ();
		for (; dfaTransitionIt; dfaTransitionIt++)
		{
			DfaTransition* transition = *dfaTransitionIt;
			printf ("    %s -> %02d\n", getMatchConditionString (&transition->m_matchCondition).cc (), transition->m_outState->m_id);
		}
	}
}

//.............................................................................

static
void
initValidSingleTable (bool* table)
{
	table ['.'] = true;
	table ['['] = true;
	table ['('] = true;
	table ['{'] = true;
	table ['^'] = true;
	table ['$'] = true;
	table ['d'] = true;
	table ['D'] = true;
	table ['h'] = true;
	table ['H'] = true;
	table ['w'] = true;
	table ['W'] = true;
	table ['s'] = true;
	table ['S'] = true;
}

bool
RegExpCompiler::Token::isValidSingle ()
{
	if (m_tokenKind != TokenKind_SpecialChar)
		return true;

	static bool validSingleTable [256] = { 0 };
	mt::callOnce (initValidSingleTable, validSingleTable);
	return validSingleTable [(uchar_t) m_char];
}

//.............................................................................

RegExpCompiler::RegExpCompiler (
	RegExp* regExp,
	RegExpNameMgr* nameMgr
	)
{
	m_regExp = regExp;
	m_nameMgr = nameMgr;
	m_p = NULL;
	m_lastToken.m_tokenKind = TokenKind_Undefined;
	m_lastToken.m_char = 0;
	m_captureId = 0;
}

bool 
RegExpCompiler::compile (
	char const* source,
	void* acceptContext
	)
{
	m_regExp->clear ();
	m_captureId = 0;
	
	bool result = incrementalCompile (source, acceptContext);
	if (!result)
		return false;
	
	finalize ();
	return true;
}

bool 
RegExpCompiler::incrementalCompile (
	char const* source,
	void* acceptContext
	)
{
	m_p = source;
	m_lastToken.m_tokenKind = TokenKind_Undefined;

	NfaState* oldStart = !m_regExp->m_nfaStateList.isEmpty () ? 
		*m_regExp->m_nfaStateList.getHead () :
		NULL;

	NfaState* body = expression ();
	if (!body)
		return false;

	bool result = expectEof ();
	if (!result)
		return false;	

	NfaState* accept = *m_regExp->m_nfaStateList.getTail ();
	accept->m_flags |= NfaStateFlag_Accept;
	accept->m_acceptContext = acceptContext;

	if (oldStart)
	{
		NfaState* split = AXL_MEM_NEW (NfaState);
		split->createEpsilonLink (oldStart, body);
		m_regExp->m_nfaStateList.insertHead (split);
	}

	return true;
}

void
RegExpCompiler::finalize ()
{
	assignNfaIds ();
	makeDfa ();
	minimizeDfa ();
	assignDfaIds ();
}

void 
RegExpCompiler::assignNfaIds ()
{
	size_t nfaStateCount = m_regExp->m_nfaStateList.getCount ();
	m_regExp->m_nfaStateArray.setCount (nfaStateCount);

	rtl::Iterator <NfaState> nfaIt = m_regExp->m_nfaStateList.getHead ();
	for (uint_t i = 0; nfaIt; i++, nfaIt++)
	{
		NfaState* nfaState = *nfaIt;
		nfaState->m_id = i;
		m_regExp->m_nfaStateArray [i] = nfaState;
	}
}

void 
RegExpCompiler::assignDfaIds ()
{
	size_t dfaStateCount = m_regExp->m_dfaStateList.getCount ();
	m_regExp->m_dfaStateArray.setCount (dfaStateCount);

	rtl::Iterator <DfaState> dfaIt = m_regExp->m_dfaStateList.getHead ();
	for (uint_t i = 0; dfaIt; i++, dfaIt++)
	{
		DfaState* state = *dfaIt;
		state->m_id = i;
		m_regExp->m_dfaStateArray [i] = state;
	}
}

void 
RegExpCompiler::makeDfa ()
{
	rtl::Array <DfaState*> workingSet;
	NfaStateSetMap <DfaState*> dfaStateMap;

	DfaState* dfaState = AXL_MEM_NEW (DfaState);
	dfaState->addNfaState (*m_regExp->m_nfaStateList.getHead ()); 
	dfaState->makeEpsilonClosure ();

	m_regExp->m_dfaStateList.insertTail (dfaState);
	workingSet.append (dfaState);
	dfaStateMap [&dfaState->m_nfaStateSet] = dfaState;
	
	NfaTransitionMgr nfaTransitionMgr;
	
	while (!workingSet.isEmpty ())
	{
		DfaState* dfaState = workingSet.getBackAndPop ();

		nfaTransitionMgr.clear ();

		size_t nfaStateCount = dfaState->m_nfaStateSet.m_stateArray.getCount ();
		for (size_t i = 0; i < nfaStateCount; i++)
		{
			NfaState* nfaState = dfaState->m_nfaStateSet.m_stateArray [i];			
			if (nfaState->m_flags & NfaStateFlag_Match)
				nfaTransitionMgr.addMatchState (nfaState);			
		}
		
		nfaTransitionMgr.finalize ();

		rtl::ConstList <NfaTransition> nfaTransitionList = nfaTransitionMgr.getTransitionList ();		
		rtl::Iterator <NfaTransition> nfaTransitionIt = nfaTransitionList.getHead ();
		for (; nfaTransitionIt; nfaTransitionIt++)
		{
			NfaTransition* nfaTransition = *nfaTransitionIt;

			DfaState* dfaState2 = AXL_MEM_NEW (DfaState);

			size_t outCount = nfaTransition->m_outStateSet.m_stateArray.getCount ();
			for (size_t i = 0; i < outCount; i++)
			{				
				NfaState* nfaState = nfaTransition->m_outStateSet.m_stateArray [i];
				dfaState2->addNfaState (nfaState);
			}

			dfaState2->makeEpsilonClosure ();

			NfaStateSetMap <DfaState*>::Iterator mapIt = dfaStateMap.visit (&dfaState2->m_nfaStateSet);
			if (mapIt->m_value)
			{
				AXL_MEM_DELETE (dfaState2);
				dfaState2 = mapIt->m_value;
			}
			else
			{
				m_regExp->m_dfaStateList.insertTail (dfaState2);
				workingSet.append (dfaState2);
				mapIt->m_value = dfaState2;
			}

			DfaTransition* dfaTransition = AXL_MEM_NEW (DfaTransition);
			dfaTransition->m_matchCondition.copy (nfaTransition->m_matchCondition);
			dfaTransition->m_outState = dfaState2;
			dfaState->m_transitionList.insertTail (dfaTransition);
		}
	}
}

void 
RegExpCompiler::minimizeDfa ()
{
}

bool
getHexValue (
	uint_t c,
	uint_t* value
	)
{
	if (c >= '0' && c <= '9')
		*value = c - '0';
	else if (c >= 'a' && c <= 'f')
		*value = c - 'a';
	else if (c >= 'A' && c <= 'F')
		*value = c - 'A';
	else
		return false;

	return true;
}

bool
RegExpCompiler::readHexEscapeSequence (char* c)
{
	ASSERT (*m_p == 'x');
	m_p++;

	uint_t x1;
	uint_t x2;

	bool result = 
		getHexValue (m_p [0], &x1) &&
		getHexValue (m_p [1], &x2);

	if (!result)
	{
		err::setFormatStringError ("invalid hex escape sequence");
		return false;
	}

	*c = x2 | (x1 << 4);
	m_p += 2;
	return true;
}

bool
RegExpCompiler::readEscapeSequence (char* c)
{
	ASSERT (*m_p == '\\');
	m_p++;

	switch (*m_p)
	{
	case 0:
		err::setStringError ("invalid escape sequence");
		return false;

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
		return readHexEscapeSequence (c);

	default:
		*c = *m_p;
	}

	m_p++;
	return true;
}

bool
RegExpCompiler::readLiteral (rtl::String* literal)
{
	bool result;

	char delimiter = *m_p++;
	ASSERT (delimiter == '"' || delimiter == '\'');

	for (;;)
	{
		char c;

		switch (*m_p)
		{
		case 0:
			err::setStringError ("unclosed literal");
			return false;

		case '\\':
			result = readEscapeSequence (&c);
			if (!result)
				return false;

			literal->append (c);
			break;

		case '"':
		case '\'':			
			if (*m_p == delimiter)
			{
				m_p++;
				return true;
			}

		default:
			c = *m_p++;
			literal->append (c);
		}
	}
}

bool
RegExpCompiler::readIdentifier (rtl::String* name)
{
	ASSERT (isalpha ((uchar_t) *m_p) || *m_p == '_');

	name->copy ((uchar_t) *m_p++);
	
	while (isalnum ((uchar_t) *m_p) || *m_p == '_')
	{
		name->append ((uchar_t) *m_p);
		m_p++;
	}

	return true;
}

bool
RegExpCompiler::getToken (Token* token)
{
	if (m_lastToken.m_tokenKind)
	{
		*token = m_lastToken;
		m_lastToken.m_tokenKind = TokenKind_Undefined;
		return true;
	}

	for (;;)
	{
		switch (*m_p)
		{
		case '\\':
			switch (m_p [1])
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
				token->m_char = m_p [1];
				m_p += 2;
				return true;

			case '\n':				
				m_p += 2; // ignore line continuation (LF)
				break;

			case '\r':
				if (m_p [2] == '\n')
				{					
					m_p += 3; // line continuation (CR-LF)
					break;
				}

			default:
				token->m_tokenKind = TokenKind_Char;
				return readEscapeSequence (&token->m_char);
			}

			break;

		case '"':
		case '\'':
			token->m_tokenKind = TokenKind_Literal;
			return readLiteral (&token->m_string);

		case 0:
			token->m_tokenKind = TokenKind_SpecialChar;
			token->m_char = 0; // don't advance m_p
			return true;

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
		case '{':
		case '}':
		case '|':
		case '^':
		case '$':
			token->m_tokenKind = TokenKind_SpecialChar;
			token->m_char = *m_p++;
			return true;
			
		default:
			if (isalpha ((uchar_t) *m_p) || *m_p == '_')
			{
				token->m_tokenKind = TokenKind_Identifier;
				return readIdentifier (&token->m_string);
			}

			token->m_tokenKind = TokenKind_Char;
			token->m_char = *m_p++;
			return true;
		}
	}
}

bool
RegExpCompiler::expectSpecialChar (char c)
{
	Token token;
	bool result = getToken (&token);
	if (!result)
		return false;

	if (!token.isSpecialChar (c))
	{
		err::setFormatStringError ("'%c' expected", c);
		return false;
	}

	return true;
}

bool
RegExpCompiler::expectEof ()
{
	Token token;
	bool result = getToken (&token);
	if (!result)
		return false;

	if (!token.isSpecialChar (0))
	{
		err::setStringError ("invalid regexp syntax");
		return false;
	}

	return true;
}

NfaState*
RegExpCompiler::expression ()
{
	NfaState* op1 = concat ();
	if (!op1)
		return NULL;

	Token token;
	bool result = getToken (&token);
	if (!result)
		return NULL;

	if (!token.isSpecialChar ('|'))
	{
		m_lastToken = token; // unget token
		return op1;
	}

	m_p++;

	NfaState* accept1 = *m_regExp->m_nfaStateList.getTail ();

	NfaState* op2 = expression ();
	if (!op2)
		return NULL;
		
	NfaState* accept2 = *m_regExp->m_nfaStateList.getTail ();

	NfaState* start = AXL_MEM_NEW (NfaState);
	NfaState* accept = AXL_MEM_NEW (NfaState);

	// connect

	m_regExp->m_nfaStateList.insertBefore (start, op1);
	m_regExp->m_nfaStateList.insertTail (accept);

	start->createEpsilonLink (op1, op2);
	accept1->createEpsilonLink (accept);
	accept2->createEpsilonLink (accept);

	return start;
}

NfaState*
RegExpCompiler::concat ()
{
	NfaState* start = repeat ();
	if (!start)
		return NULL;

	for (;;)
	{
		Token token;
		bool result = getToken (&token);
		if (!result)
			return NULL;

		m_lastToken = token; // unget token

		if (!token.isValidSingle ())
			break;

		NfaState* accept1 = *m_regExp->m_nfaStateList.getTail ();

		NfaState* op2 = repeat ();
		if (!op2)
			return NULL;

		accept1->createEpsilonLink (op2);
	}

	return start;
}

NfaState*
RegExpCompiler::repeat ()
{
	NfaState* start = single ();
	if (!start)
		return NULL;

	Token token;
	bool result = getToken (&token);
	if (!result)
		return NULL;

	if (token.m_tokenKind == TokenKind_SpecialChar)
		switch (token.m_char)
		{
		case '?':
			return question (start);

		case '*':
			return star (start);

		case '+':
			return plus (start);
		}
	
	m_lastToken = token; // unget token
	return start;
}

NfaState*
RegExpCompiler::question (NfaState* start)
{
	NfaState* accept = *m_regExp->m_nfaStateList.getTail ();

	NfaState* split = AXL_MEM_NEW (NfaState);
	m_regExp->m_nfaStateList.insertBefore (split, start);
	split->createEpsilonLink (start, accept);

	return split;
}

NfaState*
RegExpCompiler::star (NfaState* start)
{
	NfaState* oldAccept = *m_regExp->m_nfaStateList.getTail ();

	NfaState* newAccept = AXL_MEM_NEW (NfaState);
	m_regExp->m_nfaStateList.insertTail (newAccept);

	NfaState* split = AXL_MEM_NEW (NfaState);
	m_regExp->m_nfaStateList.insertBefore (split, start);
	split->createEpsilonLink (start, newAccept);
	oldAccept->createEpsilonLink (start, newAccept);

	return split;
}

NfaState*
RegExpCompiler::plus (NfaState* start)
{
	NfaState* oldAccept = *m_regExp->m_nfaStateList.getTail ();

	NfaState* newAccept = AXL_MEM_NEW (NfaState);
	m_regExp->m_nfaStateList.insertTail (newAccept);

	oldAccept->createEpsilonLink (start, newAccept);

	return start;
}

NfaState*
RegExpCompiler::single ()
{
	bool result;

	Token token;
	result = getToken (&token);
	if (!result)
		return NULL;

	switch (token.m_tokenKind)
	{
	case TokenKind_SpecialChar:
		switch (token.m_char)
		{
		case '(':
			return group ();

		case '{': 
			return capturingGroup ();

		case '[':
			return charClass ();

		case 'd':
		case 'D':
		case 'h':
		case 'H':
		case 'w':
		case 'W':
		case 's':
		case 'S':
			return stdCharClass (token.m_char);

		case '.':
			return any ();

		default:
			err::setStringError ("invalid regexp syntax");
			return NULL;
		}

	case TokenKind_Char:
		return ch (token.m_char);

	case TokenKind_Literal:
		return literal (token.m_string, token.m_string.getLength ());

	case TokenKind_Identifier:
		return namedRegExp (token.m_string);

	default:
		err::setStringError ("invalid regexp syntax");
		return NULL;
	}	
}

NfaState*
RegExpCompiler::literal (
	const char* p,
	size_t length
	)
{
	NfaState* start = AXL_MEM_NEW (NfaState);
	m_regExp->m_nfaStateList.insertTail (start);

	NfaState* mid = start;
	const char* end = p + length;
	for (; p < end; p++)
	{
		ch ((uchar_t) *p, mid);
		mid = *m_regExp->m_nfaStateList.getTail ();
	}

	return start;
}

NfaState*
RegExpCompiler::ch (uint_t c)
{
	NfaState* start = AXL_MEM_NEW (NfaState);
	m_regExp->m_nfaStateList.insertTail (start);
	ch (c, start);
	return start;
}

void
RegExpCompiler::ch (
	uint_t c,
	NfaState* start
	)
{
	NfaState* accept = AXL_MEM_NEW (NfaState);
	m_regExp->m_nfaStateList.insertTail (accept);
	start->createCharMatch (c, accept);
}

NfaState*
RegExpCompiler::charClass ()
{
	bool result;

	bool isInversed = false;
	
	if (*m_p == '^')
	{
		isInversed = true;
		m_p++;
	}

	NfaState* start = AXL_MEM_NEW (NfaState);
	start->m_flags |= NfaStateFlag_Match;
	start->m_matchCondition.m_conditionKind = MatchConditionKind_CharSet;
	start->m_matchCondition.m_charSet.setBitCount (256);
	m_regExp->m_nfaStateList.insertTail (start);

	size_t count = 0;

	for (;;)
	{
		if (!*m_p)
		{
			err::setStringError ("invalid character class");
			return NULL;
		}	
		
		if (*m_p == ']')
		{
			m_p++;
			break;
		}

		result = charClassItem (&start->m_matchCondition.m_charSet);
		if (!result)
			return NULL;

		count++;
	}

	if (!count)
	{
		err::setStringError ("empty character class");
		return NULL;
	}	

	if (isInversed) // invert range
		start->m_matchCondition.m_charSet.inverse ();

	NfaState* accept = AXL_MEM_NEW (NfaState);
	m_regExp->m_nfaStateList.insertTail (accept);

	start->m_outState = accept;
	return start;
}

bool
RegExpCompiler::charClassItem (rtl::BitMap* charSet)
{
	bool result;

	char c1;
	char c2;

	switch (*m_p)
	{
	case 0:
		err::setStringError ("invalid character class");
		return false;

	case '\\':
		switch (m_p [1])
		{
		case 'd':
		case 'D':
		case 'h':
		case 'H':
		case 'w':
		case 'W':
		case 's':
		case 'S':			
			stdCharClass (m_p [1], charSet);
			m_p += 2;
			return true;
		}

		result = readEscapeSequence (&c1);
		if (!result)
			return false;
		
		break;

	default:
		c1 = *m_p++;
	}

	if (*m_p != '-')
	{
		charSet->setBit (c1);
		return true;
	}

	m_p++;

	switch (*m_p)
	{
	case 0:
		err::setStringError ("invalid char class");
		return false;

	case '\\':
		result = readEscapeSequence (&c2);
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

	charSet->setBitRange (c1, c2 + 1);
	return true;
}

NfaState*
RegExpCompiler::stdCharClass (uint_t c)
{
	NfaState* start = AXL_MEM_NEW (NfaState);
	start->m_flags |= NfaStateFlag_Match;
	start->m_matchCondition.m_conditionKind = MatchConditionKind_CharSet;
	start->m_matchCondition.m_charSet.setBitCount (256);
	m_regExp->m_nfaStateList.insertTail (start);

	stdCharClass (c, &start->m_matchCondition.m_charSet);

	NfaState* accept = AXL_MEM_NEW (NfaState);
	m_regExp->m_nfaStateList.insertTail (accept);

	start->m_outState = accept;
	return start;
}

void
RegExpCompiler::stdCharClass (
	uint_t c,
	rtl::BitMap* charSet
	)
{
	bool isInverted = false;

	switch (c)
	{
	case 'D':
		isInverted = true;
		// and fall through

	case 'd':
		charSet->setBitRange ('0', '9' + 1);
		break;

	case 'H':
		isInverted = true;
		// and fall through

	case 'h':
		charSet->setBitRange ('0', '9' + 1);
		charSet->setBitRange ('a', 'f' + 1);
		charSet->setBitRange ('A', 'F' + 1);
		break;

	case 'W':
		isInverted = true;
		// and fall through

	case 'w':
		charSet->setBitRange ('a', 'z' + 1);
		charSet->setBitRange ('A', 'Z' + 1);
		charSet->setBitRange ('0', '9' + 1);
		charSet->setBit ('_');
		break;

	case 'S':
		isInverted = true;
		// and fall through

	case 's':
		charSet->setBit (' ');
		charSet->setBit ('\t');
		charSet->setBit ('\r');
		charSet->setBit ('\n');
		break;

	default:
		ASSERT (false);
	}

	if (isInverted)
		charSet->inverse ();
}

NfaState*
RegExpCompiler::any ()
{
	NfaState* start = AXL_MEM_NEW (NfaState);
	start->m_flags |= NfaStateFlag_Match;
	start->m_matchCondition.m_conditionKind = MatchConditionKind_Any;
	m_regExp->m_nfaStateList.insertTail (start);

	NfaState* accept = AXL_MEM_NEW (NfaState);
	m_regExp->m_nfaStateList.insertTail (accept);
	
	start->m_outState = accept;
	return start;
}

NfaState*
RegExpCompiler::group ()
{
	NfaState* start = expression ();
	if (!start)
		return NULL;

	bool result = expectSpecialChar (')');
	if (!result)
		return NULL;

	return start;
}

NfaState*
RegExpCompiler::capturingGroup ()
{
	size_t captureId = m_captureId++;

	NfaState* start = group ();
	if (!start)
		return NULL;

	NfaState* accept = *m_regExp->m_nfaStateList.getTail ();
	ASSERT (accept);

	start->m_flags |= NfaStateFlag_OpenCapture;
	start->m_captureId = captureId;

	accept->m_flags |= NfaStateFlag_CloseCapture;
	accept->m_captureId = captureId;

	return start;
}

NfaState*
RegExpCompiler::namedRegExp (const char* name)
{
	if (!m_nameMgr)	
	{
		err::setFormatStringError ("named regexp '%s' is used but name manager is not set", name);
		return NULL;
	}

	const char* source = m_nameMgr->findName (name);
	if (!source)
	{
		err::setFormatStringError ("named regexp '%s' is not defined", name);
		return NULL;
	}

	RegExp subRegExp;
	RegExpCompiler subRegExpCompiler (&subRegExp, m_nameMgr);

	bool result = subRegExpCompiler.compile (source);
	if (!result)
		return NULL;
	
	NfaState* start = *subRegExp.m_nfaStateList.getHead ();
	NfaState* accept = *subRegExp.m_nfaStateList.getTail ();

	ASSERT (accept->m_flags & NfaStateFlag_Accept);
	accept->m_flags &= ~NfaStateFlag_Accept;

	m_regExp->m_nfaStateList.insertListTail (&subRegExp.m_nfaStateList);
	return start;
}

//.............................................................................

} // namespace fsm
} // namespace axl
