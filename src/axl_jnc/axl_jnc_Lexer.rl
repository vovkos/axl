#include "pch.h"
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

# global declarations & pragmas

'namespace'    { CreateToken (EToken_Namespace); };
'using'        { CreateToken (EToken_Using); };
'extend'       { CreateToken (EToken_Extend); };
'pack'         { CreateToken (EToken_Pack); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# storage specifiers

'typedef'      { CreateToken (EToken_Typedef); };
'alias'        { CreateToken (EToken_Alias); };
'static'       { CreateToken (EToken_Static); };
'thread'       { CreateToken (EToken_Thread); };
'stack'        { CreateToken (EToken_Stack); };
'heap'         { CreateToken (EToken_Heap); };
'uheap'        { CreateToken (EToken_UHeap); };
'abstract'     { CreateToken (EToken_Abstract); };
'virtual'      { CreateToken (EToken_Virtual); };
'override'     { CreateToken (EToken_Override); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# access specifiers

'public'       { CreateToken (EToken_Public); };
'protected'    { CreateToken (EToken_Protected); };
'friend'       { CreateToken (EToken_Friend); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# type modifiers

'signed'       { CreateToken (EToken_Signed); };
'unsigned'     { CreateToken (EToken_Unsigned); };
'bigendian'    { CreateToken (EToken_BigEndian); };
'nullable'     { CreateToken (EToken_Nullable); };
'const'        { CreateToken (EToken_Const); };
'readonly'     { CreateToken (EToken_ReadOnly); };
'mutable'      { CreateToken (EToken_Mutable); };
'volatile'     { CreateToken (EToken_Volatile); };
'weak'         { CreateToken (EToken_Weak); };
'thin'         { CreateToken (EToken_Thin); };
'unsafe'       { CreateToken (EToken_Unsafe); };
'cdecl'        { CreateToken (EToken_Cdecl); };
'stdcall'      { CreateToken (EToken_Stdcall); };
'function'     { CreateToken (EToken_Function); };
'property'     { CreateToken (EToken_Property); };
'multicast'    { CreateToken (EToken_Multicast); };
'event'        { CreateToken (EToken_Event); };
'autoev'       { CreateToken (EToken_AutoEv); };
'bindable'     { CreateToken (EToken_Bindable); };
'autoget'      { CreateToken (EToken_AutoGet); };
'indexed'      { CreateToken (EToken_Indexed); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# type specifiers

'auto'         { CreateToken (EToken_Auto); };
'void'         { CreateToken (EToken_Void); };
'object'       { CreateToken (EToken_Object); };
'variant'      { CreateToken (EToken_Variant); };
'bool'         { CreateToken (EToken_Bool); };
'int8'         { CreateToken (EToken_Int8); };
'int16'        { CreateToken (EToken_Int16); };
'int32'        { CreateToken (EToken_Int32); };
'int64'        { CreateToken (EToken_Int64); };
'float'        { CreateToken (EToken_Float); };
'double'       { CreateToken (EToken_Double); };
'int'          { CreateToken (EToken_Int); };
'intptr'       { CreateToken (EToken_IntPtr); };
'char'         { CreateToken (EToken_Char); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# named type specifiers

'enum'         { CreateToken (EToken_Enum); };
'enumc'        { CreateToken (EToken_EnumC); };
'struct'       { CreateToken (EToken_Struct); };
'union'        { CreateToken (EToken_Union); };
'class'        { CreateToken (EToken_Class); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# special members

'get'          { CreateToken (EToken_Get); };
'set'          { CreateToken (EToken_Set); };
'this'         { CreateToken (EToken_This); };
'preconstruct' { CreateToken (EToken_PreConstruct); };
'construct'    { CreateToken (EToken_Construct); };
'destruct'     { CreateToken (EToken_Destruct); };
'operator'     { CreateToken (EToken_Operator); };
'postfix'      { CreateToken (EToken_Postfix); };
'propvalue'    { CreateToken (EToken_PropValue); };
'onchange'     { CreateToken (EToken_OnChange); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# operators

'new'          { CreateToken (EToken_New); };
'delete'       { CreateToken (EToken_Delete); };
'sizeof'       { CreateToken (EToken_SizeOf); };
'typeof'       { CreateToken (EToken_TypeOf); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# statements

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
'once'         { CreateToken (EToken_Once); };
'silent'       { CreateToken (EToken_Silent); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# constants

'true'         { CreateToken (EToken_True); };
'false'        { CreateToken (EToken_False); };
'null'         { CreateToken (EToken_Null); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# symbol tokens

'++'           { CreateToken (EToken_Inc); };
'--'           { CreateToken (EToken_Dec); };
'->'           { CreateToken (EToken_Ptr); };	
'<<'           { CreateToken (EToken_Shl); };
'>>'           { CreateToken (EToken_Shr); };
'&&'           { CreateToken (EToken_LogAnd); };	
'||'           { CreateToken (EToken_LogOr); };
'=='           { CreateToken (EToken_Eq); };
'!='           { CreateToken (EToken_Ne); };
'<='           { CreateToken (EToken_Le); };
'>='           { CreateToken (EToken_Ge); };	
':='           { CreateToken (EToken_RefAssign); };	
'+='           { CreateToken (EToken_AddAssign); };
'-='           { CreateToken (EToken_SubAssign); };
'*='           { CreateToken (EToken_MulAssign); };
'/='           { CreateToken (EToken_DivAssign); };
'%='           { CreateToken (EToken_ModAssign); };
'<<='          { CreateToken (EToken_ShlAssign); };
'>>='          { CreateToken (EToken_ShrAssign); };
'&='           { CreateToken (EToken_AndAssign); };
'^='           { CreateToken (EToken_XorAssign); };
'|='           { CreateToken (EToken_OrAssign); };	
'@='           { CreateToken (EToken_AtAssign); };	
'...'          { CreateToken (EToken_Ellipsis); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

# common tokens

id              { CreateStringToken (EToken_Identifier); };
lit_sq          { CreateCharToken (EToken_Integer); };
lit_dq          { CreateStringToken (EToken_Literal, 1, 1); };
dec+            { CreateIntegerToken (10); };
'0' [Xx] hex+   { CreateIntegerToken (16, 2); };
'0' [Xx] lit_dq { CreateHexLiteralToken (); };
dec+ ('.' dec+) | ([Ee] [+\-]? dec+)
		        { CreateFpToken (); };

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
		pToken->m_Data.m_Error.CreateStringError ("lexer error");
	}

	return Result;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
