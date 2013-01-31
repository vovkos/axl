#pragma once



// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)





//.............................................................................

enum ESymbol
{
	ESymbol_function_actual_argument_list = 0, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 1,

		StartSymbol        = 0,
		StartPragmaSymbol  = -1,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 6,
		SymbolCount        = 2,
		SequenceCount      = 2,
		ActionCount        = 1,
		ArgumentCount      = 0,
		BeaconCount        = 1,
		LaDfaCount         = 0,

		TotalCount         = 12,

		NamedSymbolCount   = 1,

		TokenFirst         = 0,
		TokenEnd           = 6,
		SymbolFirst        = 6,
		NamedSymbolEnd     = 7,
		SymbolEnd          = 8,
		SequenceFirst      = 8,
		SequenceEnd        = 10,
		ActionFirst        = 10,
		ActionEnd          = 11,
		ArgumentFirst      = 11,
		ArgumentEnd        = 11,
		BeaconFirst        = 11,
		BeaconEnd          = 12,
		LaDfaFirst         = 12,
		LaDfaEnd           = 12,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class hhhhhhh: public CAstNode
	{
	public:
#line 6 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
		
	int m_x;
	int m_y;
;
#line 80 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
	};
	

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
			-1, -1, 9, -1, -1, -1, 
			-1, -1, -1, 8, -1, 0, 
			
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
			7, 10, 4, 11,  -1, // 0
			5, 7, 10, 4, 11, 2,  -1, // 1
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 5, 
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
		
		case ',':
			return 3;
		
		case ':':
			return 4;
		
		case ')':
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
			',', 
			':', 
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
			_T("function_actual_argument_list"),
			
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
		
		case ESymbol_function_actual_argument_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (hhhhhhh);
				
			break;
			
		
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
			
			{ 0, 3 },
			
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
			hhhhhhh* __pAstNode = (hhhhhhh*) __pSymbol->m_pAstNode;
#line 13 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				(*__pAstNode).m_y = (*GetTokenLocator (0)).m_Value;
			;
#line 258 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
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
