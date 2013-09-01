#include "pch.h"
#include "jancyhighlighter.h"
#include "moc_jancyhighlighter.cpp"

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

)                   { colorize(ts, te, Qt::blue); };					

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

id                  {  };
(lit_sq | lit_dq)   { colorize(ts, te, Qt::darkRed); };
dec+                { colorize(ts, te, Qt::darkRed); };
'0' [Xx] hex+       { colorize(ts, te, Qt::darkRed); };
'0' [Xx] lit_dq     { colorize(ts, te, Qt::darkRed); };
'$' lit_dq          { colorize(ts, te, Qt::darkRed); };

'//' [^\n]*         { colorize(ts, te, Qt::darkGreen); };

'/*'                { colorize(ts, te, Qt::darkGreen); fgoto comment; }; 

ws | nl             ;

any                 {  };

*|;

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# comment machine 
#

comment := |*

'\n'                { colorize(ts, te, Qt::darkGreen); };
any                 { colorize(ts, te, Qt::darkGreen); };
'*/'                { colorize(ts, te, Qt::darkGreen); fgoto main; };

*|;

}%%

//.............................................................................

#define BLOCK_STATE_NONE	0
#define BLOCK_STATE_COMMENT 1

void JancyHighlighter::ragelInit()
{
	%% write init;
}

void JancyHighlighter::ragelExec()
{
	%% write exec;
}

int JancyHighlighter::getRagelState(int blockState)
{
	switch (blockState)
	{
		case 1:
			return jancy_lexer_en_comment;
	}

	return jancy_lexer_en_main;
}

void JancyHighlighter::ragelExecPreEvent(int &ragelState)
{
	setCurrentBlockState(BLOCK_STATE_NONE);

	if (previousBlockState() == BLOCK_STATE_COMMENT)
		ragelState = jancy_lexer_en_comment;
}

void JancyHighlighter::ragelExecPostEvent(int ragelState)
{
	if (ragelState == jancy_lexer_en_comment)
		setCurrentBlockState(BLOCK_STATE_COMMENT);
}

//.............................................................................
