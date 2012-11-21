#pragma once



// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)





//.............................................................................

enum ESymbol
{
	ESymbol_program = 0, 
	ESymbol_type_name_or_expr = 1, 
	ESymbol_type_name = 2, 
	ESymbol_primary_expr = 3, 
	ESymbol_type_name_rslv = 4, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 2,

		StartSymbol        = 0,
		StartPragmaSymbol  = -1,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 6,
		SymbolCount        = 6,
		SequenceCount      = 6,
		ActionCount        = 3,
		ArgumentCount      = 0,
		BeaconCount        = 1,
		LaDfaCount         = 2,

		TotalCount         = 24,

		NamedSymbolCount   = 5,

		TokenFirst         = 0,
		TokenEnd           = 6,
		SymbolFirst        = 6,
		NamedSymbolEnd     = 11,
		SymbolEnd          = 12,
		SequenceFirst      = 12,
		SequenceEnd        = 18,
		ActionFirst        = 18,
		ActionEnd          = 21,
		ArgumentFirst      = 21,
		ArgumentEnd        = 21,
		BeaconFirst        = 21,
		BeaconEnd          = 22,
		LaDfaFirst         = 22,
		LaDfaEnd           = 24,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	
	
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	static
	size_t*
	GetParseTable ()
	{
		static
		size_t _ParseTable [] = 
		{
			-1, -1, -1, -1, 12, -1, 
			-1, -1, 22, -1, -1, 9, 
			-1, -1, -1, -1, -1, 11, 
			-1, -1, 15, -1, -1, 14, 
			-1, -1, -1, -1, -1, 17, 
			-1, -1, -1, -1, -1, 16, 
			
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
			7, 4,  -1,
			3, 8, 2,  -1,
			18, 5,  -1,
			3, 9, 2,  -1,
			19, 5,  -1,
			20, 21,  -1,
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 7, 10, 14, 17, 
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

		
		case '(':
			return 2;
		
		case ')':
			return 3;
		
		case EToken_SIZEOF:
			return 4;
		
		case EToken_Identifier:
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
			'(', 
			')', 
			EToken_SIZEOF, 
			EToken_Identifier, 
			
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
			_T("type_name_or_expr"),
			_T("type_name"),
			_T("primary_expr"),
			_T("type_name_rslv"),
			
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
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
		};

		if (pNode->m_pAstNode)
			pNode->m_pAstNode->m_Kind = (int) Index;

		return pNode;
	}

	static
	size_t*
	GetBeacon (size_t Index)
	{
		static
		size_t _BeaconTable [] [2] = 
		{
			
			{ 0, 5 },
			
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
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 16 "D:/Prj/Ninja/axl3/test/TestParser.llk"
			
			printf ("primary_expr:EToken_Identifier\n");
		;
#line 259 "D:/Prj/Ninja/axl3/test/TestParser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 26 "D:/Prj/Ninja/axl3/test/TestParser.llk"
			
			printf ("type_name:EToken_Identifier\n");
		;
#line 272 "D:/Prj/Ninja/axl3/test/TestParser.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 33 "D:/Prj/Ninja/axl3/test/TestParser.llk"
			
			return (*GetTokenLocator (0)).m_Data.m_String == "TStruct";
		;
#line 285 "D:/Prj/Ninja/axl3/test/TestParser.h"
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
		
		case 0:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 23;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 9;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 13;
				pTransition->m_ResolverIndex = 10;
				pTransition->m_ResolverElseIndex = 9;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		default:
			return ELaDfaResult_Fail;
		}
	}
};

//.............................................................................



#pragma warning (default: 4065)
