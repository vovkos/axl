//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning(disable: 4065)

namespace axl {
namespace dox {

//..............................................................................

%%{

machine dox;
write data;

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# definitions
#

# whitespace

ws = [ \t\r]+;

# non-whitespace

nws = [^ \t\r\n]+;

# regular char (non-whitespace and non-escape)

rc = [^ \t\r\n\\];

#. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#
# main machine
#

main := |*

'\\headerfile'    { createToken(TokenKind_Import); };
'\\importfile'    { createToken(TokenKind_Import); };
'\\import'        { createToken(TokenKind_Import); };
'\\enum'          { createToken(TokenKind_Enum); };
'\\enumvalue'     { createToken(TokenKind_EnumValue); };
'\\struct'        { createToken(TokenKind_Struct); };
'\\union'         { createToken(TokenKind_Union); };
'\\class'         { createToken(TokenKind_Class); };
'\\alias'         { createToken(TokenKind_Alias); };
'\\var'           { createToken(TokenKind_Variable); };
'\\variable'      { createToken(TokenKind_Variable); };
'\\field'         { createToken(TokenKind_Field); };
'\\fn'            { createToken(TokenKind_Function); };
'\\function'      { createToken(TokenKind_Function); };
'\\method'        { createToken(TokenKind_Function); };
'\\overload'      { createToken(TokenKind_Overload); };
'\\prop'          { createToken(TokenKind_Property); };
'\\property'      { createToken(TokenKind_Property); };
'\\event'         { createToken(TokenKind_Event); };
'\\typedef'       { createToken(TokenKind_Typedef); };
'\\namespace'     { createToken(TokenKind_Namespace); };
'\\group'         { createToken(TokenKind_Group); };
'\\defgroup'      { createToken(TokenKind_Group); };
'\\addtogroup'    { createToken(TokenKind_Group); };
'\\ingroup'       { createToken(TokenKind_InGroup); };
'\\subgroup'      { createToken(TokenKind_SubGroup); };
'\\title'         { createToken(TokenKind_Title); };
'\\brief'         { createToken(TokenKind_Brief); };
'\\see'           { createToken(TokenKind_SeeAlso); };
'\\seealso'       { createToken(TokenKind_SeeAlso); };
'\\sa'            { createToken(TokenKind_SeeAlso); };
'\\footnote'      { createToken(TokenKind_Footnote); };

'\\' rc*          { createTextToken(TokenKind_OtherCommand); };

'@{'              { createToken(TokenKind_OpeningBrace); };
'@}'              { createToken(TokenKind_ClosingBrace); };

rc ([^\n]* nws)?  { createTextToken(TokenKind_Text); };

'\n' ws?          { createNewLineToken(), newLine(ts + 1); };
ws                ;

*|;

}%%

//..............................................................................

void
Lexer::init()
{
	%% write init;
}

void
Lexer::exec()
{
	%% write exec;
}

//..............................................................................

} // namespace dox
} // namespace axl
