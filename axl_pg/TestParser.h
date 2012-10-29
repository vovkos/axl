#pragma once



// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)





//.............................................................................

enum ESymbol
{
	ESymbol_expression_pass1 = 0, 
	ESymbol_primary_expr_pass1 = 1, 
	
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

		TokenCount         = 4,
		SymbolCount        = 4,
		SequenceCount      = 4,
		ActionCount        = 0,
		ArgumentCount      = 0,
		BeaconCount        = 0,
		LaDfaCount         = 0,

		TotalCount         = 12,

		NamedSymbolCount   = 2,

		TokenFirst         = 0,
		TokenEnd           = 4,
		SymbolFirst        = 4,
		NamedSymbolEnd     = 6,
		SymbolEnd          = 8,
		SequenceFirst      = 8,
		SequenceEnd        = 12,
		ActionFirst        = 12,
		ActionEnd          = 12,
		ArgumentFirst      = 12,
		ArgumentEnd        = 12,
		BeaconFirst        = 12,
		BeaconEnd          = 12,
		LaDfaFirst         = 12,
		LaDfaEnd           = 12,
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
			-1, 8, 8, 8, 
			-1, 1, 11, 1, 
			0, 9, 9, 9, 
			-1, 10, 10, 0, 
			
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
			6, 5,  -1,
			6, 5,  -1,
			7, 5,  -1,
			3, 7, 2,  -1,
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 6, 9, 
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
			_T("expression_pass1"),
			_T("primary_expr_pass1"),
			
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
		
		default:
			return ELaDfaResult_Fail;
		}
	}
};

//.............................................................................



#pragma warning (default: 4065)
