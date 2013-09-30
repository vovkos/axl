#include "pch.h"
#include "jancycolorizer.h"

//.............................................................................

%%{

machine jancy_lexer; 
write data;

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
nl     = '\n';
esc    = '\\' [^\n];
lit_dq = '"' ([^"\n\\] | esc)* (["\\] | nl);
lit_sq = "'" ([^'\n\\] | esc)* (['\\] | nl);

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# main machine 
#

main := |*
 
# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

(
'namespace'       |
'using'           |
'extend'          |
'pack'            |

'typedef'         |
'alias'           |
'static'          |
'thread'          |
'stack'           |
'heap'            |
'uheap'           |
'abstract'        |
'virtual'         |
'override'        |

'public'          |
'protected'       |
'friend'          |

'signed'          |
'unsigned'        |
'bigendian'       |
'nullable'        |
'const'           |
'pubconst'        |
'mutable'         |
'volatile'        |
'weak'            |
'thin'            |
'unsafe'          |
'cdecl'           |
'stdcall'         |
'function'        |
'property'        |
'bindable'        |
'autoget'         |
'indexed'         |
'multicast'       |
'event'           |
'pubevent'        |
'autoev'          |

'auto'            |
'void'            |
'object'          |
'variant'         |
'bool'            |
'int8'            |
'int16'           |
'int32'           |
'int64'           |
'float'           |
'double'          |
'char'            |
'int'             |
'intptr'          |

'enum'            |
'enumf'           |
'enumc'           |
'struct'          |
'union'           |
'class'           |

'get'             |
'set'             |
'this'            |
'basetype' [1-9]? |
'preconstruct'    |
'construct'       |
'destruct'        |
'operator'        |
'postfix'         |
'propvalue'       |
'onchange'        |

'if'              |
'else'            |
'for'             |
'while'           |
'do'              |
'break'           |
'break' [1-9]?    |
'continue' [1-9]? |
'return'          |
'switch'          |
'case'            |
'default'         |
'once'            |
'silent'          |

'true'            |
'false'           |
'null'            |

'new'             |
'delete'          |
'sizeof'          |
'countof'         |
'typeof'       

)                   { SetAttr (ts, te, gui::EStdColor_LightBlue); };		

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

id                  {  };
(lit_sq | lit_dq)   { SetAttr (ts, te, gui::EStdColor_Red); };
dec+                { SetAttr (ts, te, gui::EStdColor_Red); };
'0' [Xx] hex+       { SetAttr (ts, te, gui::EStdColor_Red); };
'0' [Xx] lit_dq     { SetAttr (ts, te, gui::EStdColor_Red); };
'$' lit_dq          { SetAttr (ts, te, gui::EStdColor_Red); };

'//'                { SetAttr (ts, te, gui::EStdColor_Green); fgoto comment_sl; };
'/*'                { SetAttr (ts, te, gui::EStdColor_Green); fgoto comment_ml; };

any                 ;

*|;

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# single-line comment machine 
#

comment_sl := |*

nl                  { SetAttr (ts, te, gui::EStdColor_Green); fgoto main; };
any                 { SetAttr (ts, te, gui::EStdColor_Green); };

*|;

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# multi-line comment machine 
#

comment_ml := |*

'*/'                { SetAttr (ts, te, gui::EStdColor_Green); fgoto main; };
any                 { SetAttr (ts, te, gui::EStdColor_Green); };

*|;

}%%

//.............................................................................

void JancyColorizer::Init()
{
	%% write init;
}

void JancyColorizer::Exec()
{
	%% write exec;
}

//.............................................................................
