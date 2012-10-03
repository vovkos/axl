#include "stdafx.h"
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
	stack = PrePush ();
}

postpop
{
	PostPop ();
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
nl     = '\n' @{ NewLine (p + 1); };
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

'integer'i       { CreateToken (EToken_SHORT); };
'string'i        { CreateToken (EToken_STRING); };
'wstring'i       { CreateToken (EToken_WSTRING); };
'boolean'i       { CreateToken (EToken_BOOLEAN); };
'byte'i          { CreateToken (EToken_BYTE); };
'char'i          { CreateToken (EToken_CHAR); };
'word'i          { CreateToken (EToken_WORD); };
'short'i         { CreateToken (EToken_SHORT); };
'dword'i         { CreateToken (EToken_DWORD); };
'long'i          { CreateToken (EToken_LONG); };
'real'i          { CreateToken (EToken_REAL); };
'float'i         { CreateToken (EToken_REAL); };
'enum'i          { CreateToken (EToken_ENUM); };
'type'i          { CreateToken (EToken_STRUCT); };
'doevents'i      { CreateToken (EToken_DOEVENTS); };
'const'i         { CreateToken (EToken_CONST); };
'dim'i           { CreateToken (EToken_DIM); };
'as'i            { CreateToken (EToken_AS); };
'end'i           { CreateToken (EToken_END); };
'declare'i       { CreateToken (EToken_DECLARE); };
'function'i      { CreateToken (EToken_FUNCTION); };
'sub'i           { CreateToken (EToken_SUB); };
'goto'i          { CreateToken (EToken_GOTO); };
'gosub'i         { CreateToken (EToken_GOSUB); };
'return'i        { CreateToken (EToken_RETURN); };
'select'i        { CreateToken (EToken_SELECT); };
'case'i          { CreateToken (EToken_CASE); };
'if'i            { CreateToken (EToken_IF); };
'then'i          { CreateToken (EToken_THEN); };
'else'i          { CreateToken (EToken_ELSE); };
'for'i           { CreateToken (EToken_FOR); };
'to'i            { CreateToken (EToken_TO); };
'step'i          { CreateToken (EToken_STEP); };
'next'i          { CreateToken (EToken_NEXT); };
'exit'i          { CreateToken (EToken_EXIT); };
'while'i         { CreateToken (EToken_WHILE); };
'wend'i          { CreateToken (EToken_WEND); };
'do'i            { CreateToken (EToken_DO); };
'loop'i          { CreateToken (EToken_LOOP); };
'until'i         { CreateToken (EToken_UNTIL); };
'byref'i         { CreateToken (EToken_BYREF); };
'byval'i         { CreateToken (EToken_BYVAL); };
'xor'i           { CreateToken (EToken_XOR); };
'or'i            { CreateToken (EToken_OR); };
'and'i           { CreateToken (EToken_AND); };
'not'i           { CreateToken (EToken_NOT); };
'mod'i           { CreateToken (EToken_MOD); };
'object'i        { CreateToken (EToken_OBJECT); };
'syscall'i       { CreateToken (EToken_SYSCALL); };
'event'i         { CreateToken (EToken_EVENT); };
'property'i      { CreateToken (EToken_PROPERTY); };
'get'i           { CreateToken (EToken_GET); };
'set'i           { CreateToken (EToken_SET); };
'ref'i           { CreateToken (EToken_REF); };
'sizeof'i        { CreateToken (EToken_SIZEOF); };
'include'i       { CreateToken (EToken_INCLUDE); };
'public'i        { CreateToken (EToken_PUBLIC); };
'static'i        { CreateToken (EToken_STATIC); };
'library'i       { CreateToken (EToken_LIBRARY); };

'message'i       { CreateToken (EToken_Message); };


# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# operators
#

'<>'   { CreateToken (EToken_Ne); };
'<='   { CreateToken (EToken_Le); };
'>='   { CreateToken (EToken_Ge); };	
'or'i  { CreateToken (EToken_Or); };
'and'i { CreateToken (EToken_And); };	
'xor'i { CreateToken (EToken_Xor); };	

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# other tokens
#

'true'i        { CreateIntegerConstToken (1); };
'false'i       { CreateIntegerConstToken (0); };

'&' [Hh] hex+  { CreateIntegerToken (2, 16); };
'&' [Bb] bin+  { CreateIntegerToken (2, 2); };
dec+           { CreateIntegerToken (0, 10); };
dec+ ('.' dec+) | ([Ee] [+\-]? dec+)
		       { CreateDoubleToken (); };

"'" [^\n]*     { CreateStringToken (1, 0, EToken_Comment, 1); };
lit_dq         { CreateStringToken (1, 1, EToken_Literal); };
lit_sq         { CreateCharConstToken(1, 1); };
id             { CreateStringToken (0, 0, EToken_Identifier); };
ws             ;
nl             ;
any            { CreateToken ((EToken) ts [0]); };
	
*|;

}%%

//.............................................................................

void 
CLexer::Init ()
{
	%% write init;
}

bool
CLexer::Exec ()
{
	%% write exec;

	bool Result = cs != parse_test_error;
	if (!Result)
	{
		CToken* pToken = CreateToken (EToken_Error);
		pToken->m_Data.m_Error = axl::err::CError ("lexer error");
	}

	return Result;
}

//.............................................................................
