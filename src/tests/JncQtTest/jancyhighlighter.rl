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
'import'       |
'namespace'    |
'using'        |
'pack'         |
'endian'       |

'typedef'      |
'static'       |

'public'       |
'private'      |

'const'        |
'volatile'     |
'signed'       |
'unsigned'     |
'littleendian' |
'bigendian'    |
'safe'         |
'unsafe'       |

'auto'         |
'void'         |
'variant'      |
'object'       |
'bool'         |
'int8'         |
'int16'        |
'int32'        |
'int64'        |
'float'        |
'double'       |
'int'          |
'char'         |
'short'        |
'long'         |

'enum'         |
'enumc'        |
'struct'       |
'union'        |
'class'        |
'interface'    |
'property'     |
'get'          |
'set'          |

'sizeof'       |
'typeof'       |

'if'           |
'else'         |
'for'          |
'while'        |
'do'           |
'break'        |
'continue'     |
'return'       |
'switch'       |
'case'         |
'default'      |

'true'         |
'false'        |
'null'         
)			   { colorize(ts, te, Qt::blue); };

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

id								{  };
(lit_sq | lit_dq)				{ colorize(ts, te, Qt::darkRed); };
dec+							{  };
'0' [Xx] hex+					{  };

'//' [^\n]*						{ colorize(ts, te, Qt::darkGreen); };

'/*'							{ colorize(ts, te, Qt::darkGreen); fgoto comment; }; 

ws | nl							;

any								{  };

*|;

# . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# comment machine 
#

comment := |*

'\n'		   { colorize(ts, te, Qt::darkGreen); };
any		       { colorize(ts, te, Qt::darkGreen); };
'*/'		   { colorize(ts, te, Qt::darkGreen); fgoto main; };

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