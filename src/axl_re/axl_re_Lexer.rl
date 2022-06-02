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

utf8_1 = 0x00 .. 0x7f;
utf8_2 = 0xc0 .. 0xdf;
utf8_3 = 0xe0 .. 0xef;
utf8_4 = 0xf0 .. 0xf7;
utf8_c = 0x80 .. 0xbf;

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

'\\d'  { createToken(TokenKind_StdCharClassDigit); };
'\\D'  { createToken(TokenKind_StdCharClassNonDigit); };
'\\h'  { createToken(TokenKind_StdCharClassHex); };
'\\H'  { createToken(TokenKind_StdCharClassNonHex); };
'\\w'  { createToken(TokenKind_StdCharClassWord); };
'\\W'  { createToken(TokenKind_StdCharClassNonWord); };
'\\s'  { createToken(TokenKind_StdCharClassSpace); };
'\\S'  { createToken(TokenKind_StdCharClassNonSpace); };

'\\0'  { createCharToken(0); };
'\\a'  { createCharToken('\a'); };
'\\b'  { createCharToken('\b'); };
'\\e'  { createCharToken('\x1b'); };
'\\f'  { createCharToken('\f'); };
'\\n'  { createCharToken('\n'); };
'\\r'  { createCharToken('\r'); };
'\\t'  { createCharToken('\t'); };
'\\v'  { createCharToken('\v'); };

'\\x' hex{2}   { createHexCharToken_2(ts + 2); };
'\\u' hex{4}   { createHexCharToken_4(ts + 2); };
'\\U' hex{8}   { createHexCharToken_8(ts + 2); };
'\\'  oct{3}   { createOctCharToken(ts + 1); };
'\\'  any      { createCharToken(ts[1]); };

utf8_1            { createCharToken(ts[0]); };
utf8_2 utf8_c     { createUtf8CharToken_2(ts); };
utf8_3 utf8_c{2}  { createUtf8CharToken_3(ts); };
utf8_4 utf8_c{3}  { createUtf8CharToken_4(ts); };

any               { createCharToken((uchar_t)ts[0]); };

*|;

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
#  char class machine
#

char_class := |*

'-'    { createToken(TokenKind_Dash); };
']'    { createToken(TokenKind_EndCharClass); fgoto main; };

# alas, Ragel doesn't allow injecting sub-scanners, hence, copy-paste...

'\\d'  { createToken(TokenKind_StdCharClassDigit); };
'\\D'  { createToken(TokenKind_StdCharClassNonDigit); };
'\\h'  { createToken(TokenKind_StdCharClassHex); };
'\\H'  { createToken(TokenKind_StdCharClassNonHex); };
'\\w'  { createToken(TokenKind_StdCharClassWord); };
'\\W'  { createToken(TokenKind_StdCharClassNonWord); };
'\\s'  { createToken(TokenKind_StdCharClassSpace); };
'\\S'  { createToken(TokenKind_StdCharClassNonSpace); };

'\\0'  { createCharToken(0); };
'\\a'  { createCharToken('\a'); };
'\\b'  { createCharToken('\b'); };
'\\e'  { createCharToken('\x1b'); };
'\\f'  { createCharToken('\f'); };
'\\n'  { createCharToken('\n'); };
'\\r'  { createCharToken('\r'); };
'\\t'  { createCharToken('\t'); };
'\\v'  { createCharToken('\v'); };

'\\x' hex{2}   { createHexCharToken_2(ts + 2); };
'\\u' hex{4}   { createHexCharToken_4(ts + 2); };
'\\U' hex{8}   { createHexCharToken_8(ts + 2); };
'\\'  oct{3}   { createOctCharToken(ts + 1); };
'\\'  any      { createCharToken(ts[1]); };

utf8_1            { createCharToken(ts[0]); };
utf8_2 utf8_c     { createUtf8CharToken_2(ts); };
utf8_3 utf8_c{2}  { createUtf8CharToken_3(ts); };
utf8_4 utf8_c{3}  { createUtf8CharToken_4(ts); };

any               { createCharToken(ts[0]); };

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
