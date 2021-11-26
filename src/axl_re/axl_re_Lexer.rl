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

// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)

namespace axl {
namespace re {

//..............................................................................

%%{

machine axl_re;
write data;

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
#  standard definitions
#

oct = [0-7];
dec = [0-9];
hex = [0-9a-fA-F];
ws  = [ \t\r];

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
#  common tokens
#

esc_char_token =
	'\\x' hex{2}  @{ createHexCharToken_2(ts + 2); } |
	'\\u' hex{4}  @{ createHexCharToken_4(ts + 2); } |
	'\\U' hex{8}  @{ createHexCharToken_8(ts + 2); } |
    '\\'  oct{3}  @{ createOctCharToken(ts + 1); } |
	'\\0'         @{ createCharToken(0); } |
	'\\a'         @{ createCharToken('\a'); } |
	'\\b'         @{ createCharToken('\b'); } |
	'\\e'         @{ createCharToken('\x1b'); } |
	'\\f'         @{ createCharToken('\f'); } |
	'\\n'         @{ createCharToken('\n'); } |
	'\\r'         @{ createCharToken('\r'); } |
	'\\t'         @{ createCharToken('\t'); } |
	'\\v'         @{ createCharToken('\v'); } |
	'\\' any      @{ createCharToken(ts[1]); }
	;

std_char_class_token =
	'\\d'  @{ createToken(TokenKind_StdCharClassDigit); } |
	'\\D'  @{ createToken(TokenKind_StdCharClassNonDigit); } |
	'\\h'  @{ createToken(TokenKind_StdCharClassHex); } |
	'\\H'  @{ createToken(TokenKind_StdCharClassNonHex); } |
	'\\w'  @{ createToken(TokenKind_StdCharClassWord); } |
	'\\W'  @{ createToken(TokenKind_StdCharClassNonWord); } |
	'\\s'  @{ createToken(TokenKind_StdCharClassSpace); } |
	'\\S'  @{ createToken(TokenKind_StdCharClassNonSpace); }
	;

utf8_char_token =
	[\x00-\x7f]         @{ createCharToken(ts[0]); } |
	[\xc0-\xdf] any     @{ createUtf8CharToken_2(ts); } |
	[\xe0-\xef] any{2}  @{ createUtf8CharToken_3(ts); } |
	[\xf0-\xff] any{3}  @{ createUtf8CharToken_4(ts); }
	;

any_char_token = any @{ createCharToken(ts[0]); };

common_token =
	esc_char_token |
	std_char_class_token |
	utf8_char_token |
	any_char_token
	;

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
#  main machine
#

main := |*

'^'    { createToken(TokenKind_AnchorBeginLine); };
'$'    { createToken(TokenKind_AnchorEndLine); };
'\\A'  { createToken(TokenKind_AnchorBeginText); };
'\\z'  { createToken(TokenKind_AnchorEndText); };
'\\b'  { createToken(TokenKind_AnchorWordBoundary); };
'\\B'  { createToken(TokenKind_AnchorNotWordBoundary); };

'[^'   { createToken(TokenKind_NegatedCharClass); fgoto char_class; };
'['    { createToken(TokenKind_CharClass); fgoto char_class; };
'(?:'  { createToken(TokenKind_NonCapturingGroup); };
'('    { createToken(TokenKind_Group); };
')'    { createToken(TokenKind_EndGroup); };
'??'   { createToken(TokenKind_NonGreedyQuestion); };
'?'    { createToken(TokenKind_Question); };
'*?'   { createToken(TokenKind_NonGreedyStar); };
'*'    { createToken(TokenKind_Star); };
'+?'   { createToken(TokenKind_NonGreedyPlus); };
'+'    { createToken(TokenKind_Plus); };
'|'    { createToken(TokenKind_Pipe); };
'{'    { createToken(TokenKind_Quantifier); fgoto quantifier; };
'.'    { createToken(TokenKind_AnyChar); };

common_token;

*|;

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
#  char class machine
#

char_class := |*

'-'  { createToken(TokenKind_Dash); };
']'  { createToken(TokenKind_EndCharClass); fgoto main; };

common_token;

*|;

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
#  quantifier machine
#

quantifier := |*

dec+  { createNumberToken(atoi(ts)); };
','   { createToken(TokenKind_Comma); };
'}'   { createToken(TokenKind_EndQuantifier); fgoto main; };

ws;

*|;

}%%

//..............................................................................

void
Lexer::init () {
	%% write init;
}

bool
Lexer::exec () {
	%% write exec;
	return cs != axl_re_error;
}

//..............................................................................

} // namespace re
} // namespace axl
