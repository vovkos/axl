#pragma once



// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)





//.............................................................................

enum ESymbol
{
	ESymbol_unary_expr = 0, 
	ESymbol_unary_operator = 1, 
	ESymbol_postfix_expr = 2, 
	ESymbol_cast_operator_rslv = 3, 
	ESymbol_primary_expr = 4, 
	
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

		TokenCount         = 8,
		SymbolCount        = 7,
		SequenceCount      = 5,
		ActionCount        = 0,
		ArgumentCount      = 0,
		BeaconCount        = 0,
		LaDfaCount         = 1,

		TotalCount         = 21,

		NamedSymbolCount   = 5,

		TokenFirst         = 0,
		TokenEnd           = 8,
		SymbolFirst        = 8,
		NamedSymbolEnd     = 13,
		SymbolEnd          = 15,
		SequenceFirst      = 15,
		SequenceEnd        = 20,
		ActionFirst        = 20,
		ActionEnd          = 20,
		ArgumentFirst      = 20,
		ArgumentEnd        = 20,
		BeaconFirst        = 20,
		BeaconEnd          = 20,
		LaDfaFirst         = 20,
		LaDfaEnd           = 21,
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
			-1, -1, 16, 16, 16, -1, 16, 16, 
			-1, -1, 2, 3, 14, -1, -1, -1, 
			-1, -1, -1, -1, 12, -1, 12, 12, 
			-1, -1, -1, -1, 18, -1, -1, -1, 
			-1, -1, -1, -1, 19, -1, 6, 7, 
			-1, -1, 15, 15, 20, -1, 0, 0, 
			-1, -1, -1, -1, 17, -1, -1, -1, 
			
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
			13, 9,  -1,
			10, 13,  -1,
			5, 6, 4,  -1,
			5, 6, 4,  -1,
			5, 8, 4,  -1,
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 6, 10, 14, 
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

		
		case '-':
			return 2;
		
		case '~':
			return 3;
		
		case '(':
			return 4;
		
		case ')':
			return 5;
		
		case EToken_Identifier:
			return 6;
		
		case EToken_Integer:
			return 7;
		
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
			'-', 
			'~', 
			'(', 
			')', 
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
			_T("unary_expr"),
			_T("unary_operator"),
			_T("postfix_expr"),
			_T("cast_operator_rslv"),
			_T("primary_expr"),
			
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
					
				pTransition->m_ProductionIndex = 15;
				pTransition->m_ResolverIndex = 11;
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
