#pragma once



// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)





//.............................................................................

enum ESymbol
{
	ESymbol_global = 0, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 3,
		PragmaNestingLimit = 1,

		StartSymbol        = 0,
		StartPragmaSymbol  = -1,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 5,
		SymbolCount        = 2,
		SequenceCount      = 5,
		ActionCount        = 4,
		ArgumentCount      = 0,
		BeaconCount        = 4,
		LaDfaCount         = 3,

		TotalCount         = 23,

		NamedSymbolCount   = 1,

		TokenFirst         = 0,
		TokenEnd           = 5,
		SymbolFirst        = 5,
		NamedSymbolEnd     = 6,
		SymbolEnd          = 7,
		SequenceFirst      = 7,
		SequenceEnd        = 12,
		ActionFirst        = 12,
		ActionEnd          = 16,
		ArgumentFirst      = 16,
		ArgumentEnd        = 16,
		BeaconFirst        = 16,
		BeaconEnd          = 20,
		LaDfaFirst         = 20,
		LaDfaEnd           = 23,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class CSymbolNode_global: public CSymbolNode
	{
	public:
			
		struct
		{
#line 5 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			std::list <int>* p;
#line 81 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		CSymbolNode_global ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_global;
			
		}
	};
		
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	static
	size_t*
	GetParseTable ()
	{
		static
		size_t _ParseTable [] = 
		{
			-1, -1, 20, -1, -1, 
			-1, -1, -1, -1, 8, 
			
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
			12, 16,  -1,
			13, 17,  -1,
			6, 2,  -1,
			14, 3, 18, 2,  -1,
			15, 19, 2,  -1,
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 6, 9, 14, 
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

		
		case '*':
			return 2;
		
		case '+':
			return 3;
		
		case EToken_Identifier:
			return 4;
		
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
			'*', 
			'+', 
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
			_T("global"),
			
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
		
		case ESymbol_global:
			pNode = AXL_MEM_NEW (CSymbolNode_global);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
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
			
			{ 1, 4 },
			
			{ 0, 4 },
			
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
			CSymbolNode_global* __pSymbol = (CSymbolNode_global*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 6 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			 return (*GetTokenLocator (0)).m_Data.m_String == "hui"; ;
#line 265 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode_global* __pSymbol = (CSymbolNode_global*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 7 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			 
			printf ("A %s\n", (*GetTokenLocator (1)).m_Data.m_String); 
		;
#line 278 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode_global* __pSymbol = (CSymbolNode_global*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 12 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			 
			printf ("C+ %s\n", (*GetTokenLocator (0)).m_Data.m_String); 
		;
#line 291 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode_global* __pSymbol = (CSymbolNode_global*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 17 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			 
			printf ("C %s\n", (*GetTokenLocator (0)).m_Data.m_String); 
		;
#line 304 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
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
			
			case '*':
					
				pTransition->m_ProductionIndex = 21;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 9;
				pTransition->m_ResolverIndex = 7;
				pTransition->m_ResolverElseIndex = 22;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case '+':
					
				pTransition->m_ProductionIndex = 10;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 11;
				return ELaDfaResult_Production;
					
			}
			
		
		default:
			return ELaDfaResult_Fail;
		}
	}
};

//.............................................................................



#pragma warning (default: 4065)
