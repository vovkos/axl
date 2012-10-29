#pragma once



// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)





//.............................................................................

enum ESymbol
{
	ESymbol_program = 0, 
	ESymbol_statement = 1, 
	ESymbol_initializer = 2, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 1,
		PragmaNestingLimit = 1,

		StartSymbol        = 0,
		StartPragmaSymbol  = -1,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 6,
		SymbolCount        = 5,
		SequenceCount      = 4,
		ActionCount        = 4,
		ArgumentCount      = 0,
		BeaconCount        = 1,
		LaDfaCount         = 0,

		TotalCount         = 20,

		NamedSymbolCount   = 3,

		TokenFirst         = 0,
		TokenEnd           = 6,
		SymbolFirst        = 6,
		NamedSymbolEnd     = 9,
		SymbolEnd          = 11,
		SequenceFirst      = 11,
		SequenceEnd        = 15,
		ActionFirst        = 15,
		ActionEnd          = 19,
		ArgumentFirst      = 19,
		ArgumentEnd        = 19,
		BeaconFirst        = 19,
		BeaconEnd          = 20,
		LaDfaFirst         = 20,
		LaDfaEnd           = 20,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	
	size_t m_Stage;


	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	static
	size_t*
	GetParseTable ()
	{
		static
		size_t _ParseTable [] = 
		{
			0, -1, -1, -1, 11, -1, 
			-1, -1, -1, -1, 12, -1, 
			-1, 14, 14, 14, 14, 10, 
			0, -1, -1, -1, 11, -1, 
			-1, -1, -1, -1, -1, 13, 
			
			-1
		};

		return _ParseTable;
	}

	static
	size_t*
	GetSequence (size_t Index)
	{
		static
		size_t _SequenceTable [] = 
		{
			9, 7,  -1,
			3, 8, 2, 15, 19,  -1,
			17, 5,  -1,
			18, 1,  -1,
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 9, 12, 
			-1
		};

		ASSERT (Index < SequenceCount);
		return _SequenceTable + _SequenceIndexTable [Index];
	}

	static
	size_t
	GetTokenIndex (int Token)
	{
		switch (Token)
		{
		case 0:
			return EofToken;

		
		case '=':
			return 2;
		
		case ';':
			return 3;
		
		case EToken_Identifier:
			return 4;
		
		case EToken_Integer:
			return 5;
		
		default:
			return AnyToken;
		}
	}

	static
	int
	GetTokenFromIndex (size_t Index)
	{
		static
		const int _TokenTable [] = 
		{
			0,  // eof
			0,  // any token
			'=', 
			';', 
			EToken_Identifier, 
			EToken_Integer, 
			
			0
		};

		ASSERT (Index < TokenCount);
		return _TokenTable [Index];
	}

	static
	const tchar_t*
	GetSymbolName (size_t Index)
	{
		static
		const tchar_t* _SymbolNameTable [NamedSymbolCount] = 
		{
			_T("program"),
			_T("statement"),
			_T("initializer"),
			
		};

		ASSERT (Index < NamedSymbolCount);
		return _SymbolNameTable [Index];
	}

	static
	CSymbolNode*
	CreateSymbolNode (size_t Index)
	{
		CSymbolNode* pNode;

		switch (Index)
		{
		
		
		
		
		default:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_pAst = AXL_MEM_NEW (CAst);
		};

		if (pNode->m_pAst)
			pNode->m_pAst->m_Kind = (int) Index;

		return pNode;
	}

	static
	size_t*
	GetBeacon (size_t Index)
	{
		static
		size_t _BeaconTable [] [2] = 
		{
			
			{ 0, 4 },
			
			{ 0 }
		};

		ASSERT (Index < BeaconCount);
		return _BeaconTable [Index];
	}

	bool
	Action (size_t Index)
	{
		switch (Index)
		{
		
		case 0:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 16 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			m_Stage = (*GetTokenLocator (0)).m_Data.m_String == "hui" ? 1 : 2;			
		;
#line 251 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 23 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			return m_Stage == 1;
#line 262 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 25 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			printf ("actual expression");
		;
#line 275 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 29 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			printf ("any");
		;
#line 288 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		default:
			ASSERT (false);
			return true;
		}
	}

	void
	Argument (
		size_t Index,
		CSymbolNode* pSymbol
		)
	{
		switch (Index)
		{
		
		default:
			ASSERT (false);
		}
	}

	bool
	Enter (size_t Index)
	{
		switch (Index)
		{
		
		
		
		
		default:
			return true;
		}
	}

	bool
	Leave (size_t Index)
	{
		switch (Index)
		{
		
		
		
		
		default:
			return true;
		}
	}

	ELaDfaResult
	LaDfa (
		size_t Index,
		int LookaheadToken,
		TLaDfaTransition* pTransition
		)
	{
		switch (Index)
		{
		
		default:
			return ELaDfaResult_Fail;
		}
	}
};

//.............................................................................



#pragma warning (default: 4065)
