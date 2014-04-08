#include "axl_ini_Lexer.h"

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
nl     = '\n' @{ NewLine (p + 1); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# main machine 
#

main := |*

ws | nl               ;
[;#] [^\n]*           ;
'//' [^\n]*           ;

'[' [^\n\]]+ ']'      { ParseSection (ts, te); };
[^;# \t\r\n\[] [^\n]* { ParseKeyValue (ts, te); };

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
	return cs != axl_ini_error;
}

//.............................................................................

} // namespace ini {
} // namespace axl {


