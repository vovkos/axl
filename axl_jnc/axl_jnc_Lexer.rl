#include "stdafx.h"
#include "axl_jnc_Lexer.h"

// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)

namespace axl {
namespace jnc {

//.............................................................................

%%{

machine axl_jnc; 
write data;

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
id     = [_a-zA-Z] [_a-zA-Z0-9]*;
ws     = [ \t\r]+;
nl     = '\n' @{ NewLine (p + 1); };
esc    = '\\' [^\n];
lit_dq = '"' ([^"\n\\] | esc)* (["\\] | nl);
lit_sq = "'" ([^'\n\\] | esc)* (['\\] | nl);

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# main machine 
#

main := |*

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

'namespace'    { CreateToken (EToken_Namespace); };
'using'        { CreateToken (EToken_Using); };
'pack'         { CreateToken (EToken_Pack); };
'endian'       { CreateToken (EToken_Endian); };

'typedef'      { CreateToken (EToken_Typedef); };
'static'       { CreateToken (EToken_Static); };

'public'       { CreateToken (EToken_Public); };
'private'      { CreateToken (EToken_Private); };

'const'        { CreateToken (EToken_Const); };
'volatile'     { CreateToken (EToken_Volatile); };
'signed'       { CreateToken (EToken_Signed); };
'unsigned'     { CreateToken (EToken_Unsigned); };
'littleendian' { CreateToken (EToken_LittleEndian); };
'bigendian'    { CreateToken (EToken_BigEndian); };
'safe'         { CreateToken (EToken_Safe); };
'unsafe'       { CreateToken (EToken_Unsafe); };
'dynamic'      { CreateToken (EToken_Dynamic); };
'nonull'       { CreateToken (EToken_NoNull); };

'auto'         { CreateToken (EToken_Auto); };
'void'         { CreateToken (EToken_Void); };
'bool'         { CreateToken (EToken_Bool); };
'int8'         { CreateToken (EToken_Int8); };
'int16'        { CreateToken (EToken_Int16); };
'int32'        { CreateToken (EToken_Int32); };
'int64'        { CreateToken (EToken_Int64); };
'float'        { CreateToken (EToken_Float); };
'double'       { CreateToken (EToken_Double); };
'int'          { CreateToken (EToken_Int); };
'char'         { CreateToken (EToken_Char); };
'short'        { CreateToken (EToken_Short); };
'long'         { CreateToken (EToken_Long); };
'wchar_t'      { CreateToken (EToken_WChar); };

'enum'         { CreateToken (EToken_Enum); };
'enumc'        { CreateToken (EToken_EnumC); };
'struct'       { CreateToken (EToken_Struct); };
'union'        { CreateToken (EToken_Union); };
'class'        { CreateToken (EToken_Class); };
'interface'    { CreateToken (EToken_Interface); };
'property'     { CreateToken (EToken_Property); };
'get'          { CreateToken (EToken_Get); };
'set'          { CreateToken (EToken_Set); };

'sizeof'       { CreateToken (EToken_SizeOf); };
'typeof'       { CreateToken (EToken_TypeOf); };
'dynasizeof'   { CreateToken (EToken_DynaSizeOf); };
'dynatypeof'   { CreateToken (EToken_DynaTypeOf); };

'if'           { CreateToken (EToken_If); };
'else'         { CreateToken (EToken_Else); };
'for'          { CreateToken (EToken_For); };
'while'        { CreateToken (EToken_While); };
'do'           { CreateToken (EToken_Do); };
'break'        { CreateToken (EToken_Break); };
'continue'     { CreateToken (EToken_Continue); };
'return'       { CreateToken (EToken_Return); };
'switch'       { CreateToken (EToken_Switch); };
'case'         { CreateToken (EToken_Case); };
'default'      { CreateToken (EToken_Default); };

'true'         { CreateToken (EToken_True); };
'false'        { CreateToken (EToken_False); };
'null'         { CreateToken (EToken_Null); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

'=='           { CreateToken (EToken_Eq); };
'!='           { CreateToken (EToken_Ne); };
'<='           { CreateToken (EToken_Le); };
'>='           { CreateToken (EToken_Ge); };	
'||'           { CreateToken (EToken_LogicalOr); };
'&&'           { CreateToken (EToken_LogicalAnd); };	
'<<'           { CreateToken (EToken_Shl); };
'>>'           { CreateToken (EToken_Shr); };
'++'           { CreateToken (EToken_Inc); };
'--'           { CreateToken (EToken_Dec); };
'->'           { CreateToken (EToken_Ptr); };	
'*='           { CreateToken (EToken_MulAssign); };
'/='           { CreateToken (EToken_DivAssign); };
'%='           { CreateToken (EToken_ModAssign); };
'+='           { CreateToken (EToken_AddAssign); };
'-='           { CreateToken (EToken_SubAssign); };
'<<='          { CreateToken (EToken_ShlAssign); };
'>>='          { CreateToken (EToken_ShrAssign); };
'&='           { CreateToken (EToken_AndAssign); };
'^='           { CreateToken (EToken_XorAssign); };
'|='           { CreateToken (EToken_OrAssign); };	
':='           { CreateToken (EToken_RefAssign); };	
'...'          { CreateToken (EToken_Ellipsis); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

id             { CreateStringToken (EToken_Identifier); };
lit_sq         { CreateCharToken (EToken_Integer); };
lit_dq         { CreateStringToken (EToken_Literal, 1, 1); };
dec+           { CreateIntegerToken (10); };
'0' [Xx] hex+  { CreateIntegerToken (16, 2); };

'//' [^\n]*    ;
'/*' (any | nl)* :>> '*/' ;

ws | nl        ;

any            { CreateToken (ts [0]); };

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

	bool Result = cs != axl_jnc_error;
	if (!Result)
	{
		CToken* pToken = CreateToken (EToken_Error);
		pToken->m_Data.m_Error = err::CError ("lexer error");
	}

	return Result;
}


//.............................................................................

} // namespace axl {
} // namespace jnc {
