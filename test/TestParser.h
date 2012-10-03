#pragma once



// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)





//.............................................................................

enum ESymbol
{
	ESymbol_global = 0, 
	ESymbol_declaration = 1, 
	ESymbol_type_specifier_modifier = 2, 
	ESymbol_type_identifier_rslv = 3, 
	ESymbol_qualified_identifier = 4, 
	
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
		SymbolCount        = 9,
		SequenceCount      = 6,
		ActionCount        = 2,
		ArgumentCount      = 0,
		BeaconCount        = 1,
		LaDfaCount         = 1,

		TotalCount         = 25,

		NamedSymbolCount   = 5,

		TokenFirst         = 0,
		TokenEnd           = 6,
		SymbolFirst        = 6,
		NamedSymbolEnd     = 11,
		SymbolEnd          = 15,
		SequenceFirst      = 15,
		SequenceEnd        = 21,
		ActionFirst        = 21,
		ActionEnd          = 23,
		ArgumentFirst      = 23,
		ArgumentEnd        = 23,
		BeaconFirst        = 23,
		BeaconEnd          = 24,
		LaDfaFirst         = 24,
		LaDfaEnd           = 25,
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
			0, -1, -1, -1, 15, 15, 
			-1, -1, -1, -1, 16, 16, 
			-1, -1, -1, -1, 13, 5, 
			-1, -1, -1, -1, 20, -1, 
			-1, -1, -1, -1, 19, -1, 
			0, -1, -1, -1, 15, 15, 
			-1, -1, -1, -1, 24, 17, 
			-1, -1, -1, -1, 10, -1, 
			-1, -1, -1, 18, 0, 0, 
			
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
			11, 7,  -1,
			21, 2, 23, 12, 8,  -1,
			12, 8,  -1,
			14, 4, 3,  -1,
			14, 4,  -1,
			22, 10,  -1,
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 9, 12, 16, 19, 
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

		
		case ';':
			return 2;
		
		case '.':
			return 3;
		
		case EToken_Identifier:
			return 4;
		
		case EToken_CHAR:
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
			';', 
			'.', 
			EToken_Identifier, 
			EToken_CHAR, 
			
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
			_T("declaration"),
			_T("type_specifier_modifier"),
			_T("type_identifier_rslv"),
			_T("qualified_identifier"),
			
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
			
			printf ("declare %s\n", (*GetTokenLocator (0)).m_Data.m_String);
		;
			}
			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			printf ("type_qual_identifier_rslv");
			return false;
		;
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
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 17;
				pTransition->m_ResolverIndex = 9;
				pTransition->m_ResolverElseIndex = 0;
						
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
