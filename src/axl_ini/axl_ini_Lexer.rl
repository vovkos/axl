// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)

namespace axl {
namespace ini {

//.............................................................................

%%{

machine axl_ini; 
write data;

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# standard definitions
#

ws     = [ \t\r]+;
nl     = '\n' @{ newLine (p + 1); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# main machine 
#

main := |*

ws | nl               ;
[;#] [^\n]*           ;
'//' [^\n]*           ;

'[' [^\n\]]+ ']'      { parseSection (ts, te); };
[^;# \t\r\n\[] [^\n]* { parseKeyValue (ts, te); };

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
	return cs != axl_ini_error;
}

//.............................................................................

} // namespace ini {
} // namespace axl {
