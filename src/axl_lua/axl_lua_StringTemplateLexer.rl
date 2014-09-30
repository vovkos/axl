// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)

namespace axl { 
namespace lua { 

//.............................................................................

%%{

machine axl_lua_st; 
write data;

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# standard definitions
#

dec    = [0-9];
hex    = [0-9a-fA-F];
oct    = [0-7];
bin    = [01];
id     = [a-zA-Z_] [a-zA-Z0-9_]*;
ws     = [ \t\r]+;
nl     = '\n' @{ newLine (p + 1); };
esc    = '\\' [^\n];
lit_dq = '"' ([^"\n\\] | esc)* (["\\] | nl);
lit_sq = "'" ([^'\n\\] | esc)* (['\\] | nl);

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# main machine 
#

user_code := |*

[{}]         { createToken (ts [0]); };
nl           ;
any          ;

*|; 

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

user_data := |*

[()]         { createToken (ts [0]); };
nl           ;
any          ;
   
*|;

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

main := |*

'%{'         { createToken (EToken_OpenCode); };
'$('         { createToken (EToken_OpenData); };
'$' id       { createStringToken (EToken_Data, 1); };
nl           ;
any          ;

*|;

}%%

//.............................................................................

void 
CLexer::init ()
{
	%% write init;
}

void
CLexer::exec ()
{
	%% write exec;
}

int
CLexer::getMachineState (ELexerMachine machine)
{
	switch (machine)
	{
	case ELexerMachine_Main:
		return axl_lua_st_en_main;

	case ELexerMachine_UserCode:
		return axl_lua_st_en_user_code;

	case ELexerMachine_UserData:
		return axl_lua_st_en_user_data;

	default:
		ASSERT (false);
		return axl_lua_st_en_main;
	}
}

//.............................................................................

} // namespace lua { 
} // namespace axl { 
