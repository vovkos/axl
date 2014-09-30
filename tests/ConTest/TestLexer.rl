#include "TestLexer.h"

//.............................................................................

%%{

machine parse_test; 
write data nofinal;

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
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

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
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

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# main machine of BASIC lexer
#

main := |*
	
# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# keywords
#

'integer'i       { createToken (EToken_SHORT); };
'string'i        { createToken (EToken_STRING); };
'wstring'i       { createToken (EToken_WSTRING); };
'boolean'i       { createToken (EToken_BOOLEAN); };
'byte'i          { createToken (EToken_BYTE); };
'char'i          { createToken (EToken_CHAR); };
'word'i          { createToken (EToken_WORD); };
'short'i         { createToken (EToken_SHORT); };
'dword'i         { createToken (EToken_DWORD); };
'long'i          { createToken (EToken_LONG); };
'real'i          { createToken (EToken_REAL); };
'float'i         { createToken (EToken_REAL); };
'enum'i          { createToken (EToken_ENUM); };
'type'i          { createToken (EToken_STRUCT); };
'doevents'i      { createToken (EToken_DOEVENTS); };
'const'i         { createToken (EToken_CONST); };
'dim'i           { createToken (EToken_DIM); };
'as'i            { createToken (EToken_AS); };
'end'i           { createToken (EToken_END); };
'declare'i       { createToken (EToken_DECLARE); };
'function'i      { createToken (EToken_FUNCTION); };
'sub'i           { createToken (EToken_SUB); };
'goto'i          { createToken (EToken_GOTO); };
'gosub'i         { createToken (EToken_GOSUB); };
'return'i        { createToken (EToken_RETURN); };
'select'i        { createToken (EToken_SELECT); };
'case'i          { createToken (EToken_CASE); };
'if'i            { createToken (EToken_IF); };
'then'i          { createToken (EToken_THEN); };
'else'i          { createToken (EToken_ELSE); };
'for'i           { createToken (EToken_FOR); };
'to'i            { createToken (EToken_TO); };
'step'i          { createToken (EToken_STEP); };
'next'i          { createToken (EToken_NEXT); };
'exit'i          { createToken (EToken_EXIT); };
'while'i         { createToken (EToken_WHILE); };
'wend'i          { createToken (EToken_WEND); };
'do'i            { createToken (EToken_DO); };
'loop'i          { createToken (EToken_LOOP); };
'until'i         { createToken (EToken_UNTIL); };
'byref'i         { createToken (EToken_BYREF); };
'byval'i         { createToken (EToken_BYVAL); };
'xor'i           { createToken (EToken_XOR); };
'or'i            { createToken (EToken_OR); };
'and'i           { createToken (EToken_AND); };
'not'i           { createToken (EToken_NOT); };
'mod'i           { createToken (EToken_MOD); };
'object'i        { createToken (EToken_OBJECT); };
'syscall'i       { createToken (EToken_SYSCALL); };
'event'i         { createToken (EToken_EVENT); };
'property'i      { createToken (EToken_PROPERTY); };
'get'i           { createToken (EToken_GET); };
'set'i           { createToken (EToken_SET); };
'ref'i           { createToken (EToken_REF); };
'sizeof'i        { createToken (EToken_SIZEOF); };
'include'i       { createToken (EToken_INCLUDE); };
'public'i        { createToken (EToken_PUBLIC); };
'static'i        { createToken (EToken_STATIC); };
'library'i       { createToken (EToken_LIBRARY); };

'message'i       { createToken (EToken_Message); };


# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# operators
#

'<>'   { createToken (EToken_Ne); };
'<='   { createToken (EToken_Le); };
'>='   { createToken (EToken_Ge); };	
'or'i  { createToken (EToken_Or); };
'and'i { createToken (EToken_And); };	
'xor'i { createToken (EToken_Xor); };	

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
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

"'" [^\n]*     { createStringToken (1, 0, EToken_Comment, 1); };
lit_dq         { createStringToken (1, 1, EToken_Literal); };
lit_sq         { createCharConstToken(1, 1); };
id             { createStringToken (0, 0, EToken_Identifier); };
ws             ;
nl             ;
any            { createToken ((EToken) ts [0]); };
	
*|;

}%%

//.............................................................................

void 
CLexer::init ()
{
	%% write init;
}

bool
CLexer::exec ()
{
	%% write exec;

	bool result = cs != parse_test_error;
	if (!result)
	{
		CToken* token = createToken (EToken_Error);
		token->m_data.m_error.createStringError ("lexer error");
	}

	return result;
}

//.............................................................................
