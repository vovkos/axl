#include "pch.h"
// #include "TestLexer.h"
// #include "TestParser.h"

using namespace axl;

//.............................................................................

namespace test_Parse
{

/*

void 
Run (const char* pFileName)
{
	bool Result;

	io::CMappedFile File;
	Result = File.Open (pFileName, io::EFileFlag_ReadOnly);
	if (!Result)
	{
		printf ("%s\n", err::GetError ()->GetDescription ());
		return;
	}

	char* p = (char*) File.View ();
	size_t Size = (size_t) File.GetSize ();

	CLexer Lexer;
	CParser Parser;

	Lexer.Create (pFileName, p, Size);
	Parser.Create ();

	for (;;)
	{
		const CToken* pToken = Lexer.GetToken (); 

		Result = Parser.ParseToken (pToken);
		if (!Result)
		{
			printf ("%s\n", err::GetError ()->GetDescription ());
			return;
		}

		if (!pToken->m_Token)
			break;

		Lexer.NextToken ();
	}
}

*/

} // test_Parse

//.............................................................................
