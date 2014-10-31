#include "pch.h"
// #include "TestLexer.h"
// #include "TestParser.h"

//.............................................................................

namespace test_Parse
{

/*

void 
run (const char* fileName)
{
	bool result;

	io::MappedFile file;
	result = file.open (fileName, io::FileFlag_ReadOnly);
	if (!result)
	{
		printf ("%s\n", err::getError ()->getDescription ());
		return;
	}

	char* p = (char*) file.view ();
	size_t size = (size_t) file.getSize ();

	Lexer lexer;
	Parser parser;

	lexer.create (fileName, p, size);
	parser.create ();

	for (;;)
	{
		const Token* token = lexer.getToken (); 

		result = parser.parseToken (token);
		if (!result)
		{
			printf ("%s\n", err::getError ()->getDescription ());
			return;
		}

		if (!token->m_token)
			break;

		lexer.nextToken ();
	}
}

*/

} // test_Parse

//.............................................................................
