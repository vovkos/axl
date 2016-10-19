//..............................................................................

%%{

machine parse_test;
write data nofinal;

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# prepush / postpop (for fcall/fret)
#

prepush
{
	stack = prePush ();
}

postpop
{
	postPop ();
}

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# standard definitions
#

dec    = [0-9];
hex    = [0-9a-fA-F];
oct    = [0-7];
bin    = [01];
id     = [!~$a-zA-Z_] [!~$a-zA-Z0-9_]*;
ws     = [ \t\r]+;
nl     = '\n' @{ newLine (p + 1); };
esc    = '\\' [^\n];
lit_dq = '"' ([^"\n\\] | esc)* (["\\] | nl);
lit_sq = "'" ([^'\n\\] | esc)* (['\\] | nl);

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# main machine of BASIC lexer
#

main := |*

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# keywords
#

'integer'i       { createToken (TokenKind_SHORT); };
'string'i        { createToken (TokenKind_STRING); };
'wstring'i       { createToken (TokenKind_WSTRING); };
'boolean'i       { createToken (TokenKind_BOOLEAN); };
'byte'i          { createToken (TokenKind_BYTE); };
'char'i          { createToken (TokenKind_CHAR); };
'word'i          { createToken (TokenKind_WORD); };
'short'i         { createToken (TokenKind_SHORT); };
'dword'i         { createToken (TokenKind_DWORD); };
'long'i          { createToken (TokenKind_LONG); };
'real'i          { createToken (TokenKind_REAL); };
'float'i         { createToken (TokenKind_REAL); };
'enum'i          { createToken (TokenKind_ENUM); };
'type'i          { createToken (TokenKind_STRUCT); };
'doevents'i      { createToken (TokenKind_DOEVENTS); };
'const'i         { createToken (TokenKind_CONST); };
'dim'i           { createToken (TokenKind_DIM); };
'as'i            { createToken (TokenKind_AS); };
'end'i           { createToken (TokenKind_END); };
'declare'i       { createToken (TokenKind_DECLARE); };
'function'i      { createToken (TokenKind_FUNCTION); };
'sub'i           { createToken (TokenKind_SUB); };
'goto'i          { createToken (TokenKind_GOTO); };
'gosub'i         { createToken (TokenKind_GOSUB); };
'return'i        { createToken (TokenKind_RETURN); };
'select'i        { createToken (TokenKind_SELECT); };
'case'i          { createToken (TokenKind_CASE); };
'if'i            { createToken (TokenKind_IF); };
'then'i          { createToken (TokenKind_THEN); };
'else'i          { createToken (TokenKind_ELSE); };
'for'i           { createToken (TokenKind_FOR); };
'to'i            { createToken (TokenKind_TO); };
'step'i          { createToken (TokenKind_STEP); };
'next'i          { createToken (TokenKind_NEXT); };
'exit'i          { createToken (TokenKind_EXIT); };
'while'i         { createToken (TokenKind_WHILE); };
'wend'i          { createToken (TokenKind_WEND); };
'do'i            { createToken (TokenKind_DO); };
'loop'i          { createToken (TokenKind_LOOP); };
'until'i         { createToken (TokenKind_UNTIL); };
'byref'i         { createToken (TokenKind_BYREF); };
'byval'i         { createToken (TokenKind_BYVAL); };
'xor'i           { createToken (TokenKind_XOR); };
'or'i            { createToken (TokenKind_OR); };
'and'i           { createToken (TokenKind_AND); };
'not'i           { createToken (TokenKind_NOT); };
'mod'i           { createToken (TokenKind_MOD); };
'object'i        { createToken (TokenKind_OBJECT); };
'syscall'i       { createToken (TokenKind_SYSCALL); };
'event'i         { createToken (TokenKind_EVENT); };
'property'i      { createToken (TokenKind_PROPERTY); };
'get'i           { createToken (TokenKind_GET); };
'set'i           { createToken (TokenKind_SET); };
'ref'i           { createToken (TokenKind_REF); };
'sizeof'i        { createToken (TokenKind_SIZEOF); };
'include'i       { createToken (TokenKind_INCLUDE); };
'public'i        { createToken (TokenKind_PUBLIC); };
'static'i        { createToken (TokenKind_STATIC); };
'library'i       { createToken (TokenKind_LIBRARY); };

'message'i       { createToken (TokenKind_Message); };


#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# operators
#

'<>'   { createToken (TokenKind_Ne); };
'<='   { createToken (TokenKind_Le); };
'>='   { createToken (TokenKind_Ge); };
'or'i  { createToken (TokenKind_Or); };
'and'i { createToken (TokenKind_And); };
'xor'i { createToken (TokenKind_Xor); };

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# other tokens
#

'true'i        { createIntegerConstToken (1); };
'false'i       { createIntegerConstToken (0); };

'&' [hh] hex+  { createIntegerToken (2, 16); };
'&' [bb] bin+  { createIntegerToken (2, 2); };
dec+           { createIntegerToken (0, 10); };
dec+ ('.' dec+) | ([ee] [+\-]? dec+)
		       { createDoubleToken (); };

"'" [^\n]*     { createStringToken (1, 0, TokenKind_Comment, 1); };
lit_dq         { createStringToken (1, 1, TokenKind_Literal); };
lit_sq         { createCharConstToken(1, 1); };
id             { createStringToken (0, 0, TokenKind_Identifier); };
ws             ;
nl             ;
any            { createToken ((TokenKind) ts [0]); };

*|;

}%%

//..............................................................................

void
Lexer::init ()
{
	%% write init;
}

bool
Lexer::exec ()
{
	%% write exec;

	bool result = cs != parse_test_error;
	if (!result)
	{
		Token* token = createToken (TokenKind_Error);
		token->m_data.m_error.createStringError ("lexer error");
	}

	return result;
}

//..............................................................................
