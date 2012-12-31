#pragma once



// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)





//.............................................................................

enum ESymbol
{
	ESymbol_program = 0, 
	ESymbol_common_declaration = 1, 
	ESymbol_declaration = 2, 
	ESymbol_expr = 3, 
	ESymbol_type_specifier = 4, 
	ESymbol_declarator = 5, 
	ESymbol_declaration_terminator = 6, 
	ESymbol_pointer = 7, 
	ESymbol_function_suffix = 8, 
	ESymbol_formal_arg = 9, 
	ESymbol_mul_expr = 10, 
	ESymbol_primary_expr = 11, 
	
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

		TokenCount         = 13,
		SymbolCount        = 22,
		SequenceCount      = 17,
		ActionCount        = 2,
		ArgumentCount      = 0,
		BeaconCount        = 0,
		LaDfaCount         = 6,

		TotalCount         = 60,

		NamedSymbolCount   = 12,

		TokenFirst         = 0,
		TokenEnd           = 13,
		SymbolFirst        = 13,
		NamedSymbolEnd     = 25,
		SymbolEnd          = 35,
		SequenceFirst      = 35,
		SequenceEnd        = 52,
		ActionFirst        = 52,
		ActionEnd          = 54,
		ArgumentFirst      = 54,
		ArgumentEnd        = 54,
		BeaconFirst        = 54,
		BeaconEnd          = 54,
		LaDfaFirst         = 54,
		LaDfaEnd           = 60,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	

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
			-1, -1, 14, 14, 14, -1, -1, 14, -1, 14, 14, 14, -1, 
			-1, -1, 37, 37, 37, -1, -1, 37, -1, 37, 37, 37, -1, 
			-1, -1, 14, 54, 55, 35, -1, 14, -1, 14, 14, 56, 35, 
			-1, -1, -1, 45, 45, 45, -1, -1, -1, -1, -1, 45, 45, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 9, 10, 27, -1, 
			-1, -1, -1, 30, 30, -1, -1, -1, -1, -1, -1, 30, -1, 
			-1, -1, 2, -1, -1, -1, -1, 51, -1, -1, -1, -1, -1, 
			-1, -1, -1, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 41, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 43, 43, -1, -1, -1, -1, 43, 43, 43, -1, 
			-1, -1, -1, 47, 47, 47, -1, -1, -1, -1, -1, 47, 47, 
			-1, -1, -1, 49, 50, 48, -1, -1, -1, -1, -1, 11, 12, 
			-1, -1, 0, 0, 0, -1, -1, 0, -1, 36, 36, 58, -1, 
			-1, -1, 0, 18, 18, -1, -1, 0, -1, -1, -1, 18, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 11, -1, 
			-1, -1, -1, 38, 38, -1, -1, -1, -1, -1, -1, 0, -1, 
			-1, -1, 0, 0, 0, 21, 0, 0, -1, 0, 0, 0, -1, 
			-1, -1, -1, 39, 39, -1, -1, -1, -1, -1, -1, 39, -1, 
			-1, -1, -1, 40, 40, -1, 0, -1, -1, 40, 40, 40, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, -1, 42, 42, 59, -1, 
			-1, -1, 0, 44, 0, -1, 0, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, 0, 46, -1, 0, -1, -1, -1, -1, -1, -1, 
			
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
			2, 16,  -1, // 0
			25, 17,  -1, // 1
			19, 26, 25,  -1, // 2
			28, 20,  -1, // 3
			29, 11, 28,  -1, // 4
			31, 22,  -1, // 5
			6, 31, 5,  -1, // 6
			32, 17,  -1, // 7
			18, 32,  -1, // 8
			33, 23, 3,  -1, // 9
			33, 23,  -1, // 10
			34, 24, 4,  -1, // 11
			34, 24,  -1, // 12
			6, 16, 5,  -1, // 13
			16, 3,  -1, // 14
			16, 4,  -1, // 15
			8, 7,  -1, // 16
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 6, 10, 13, 17, 20, 24, 27, 30, 34, 37, 41, 44, 48, 51, 54, 
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
		
		case '*':
			return 3;
		
		case '&':
			return 4;
		
		case '(':
			return 5;
		
		case ')':
			return 6;
		
		case '{':
			return 7;
		
		case '}':
			return 8;
		
		case EToken_Int:
			return 9;
		
		case EToken_Float:
			return 10;
		
		case EToken_Identifier:
			return 11;
		
		case EToken_Integer:
			return 12;
		
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
			'*', 
			'&', 
			'(', 
			')', 
			'{', 
			'}', 
			EToken_Int, 
			EToken_Float, 
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
			_T("common_declaration"),
			_T("declaration"),
			_T("expr"),
			_T("type_specifier"),
			_T("declarator"),
			_T("declaration_terminator"),
			_T("pointer"),
			_T("function_suffix"),
			_T("formal_arg"),
			_T("mul_expr"),
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
#line 21 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			 return IsTypeName; ;
#line 342 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 26 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			 return false; ;
#line 353 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
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
					
				pTransition->m_ProductionIndex = 14;
				pTransition->m_ResolverIndex = 53;
				pTransition->m_ResolverElseIndex = 35;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case '&':
					
				pTransition->m_ProductionIndex = 14;
				pTransition->m_ResolverIndex = 53;
				pTransition->m_ResolverElseIndex = 35;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 14;
				pTransition->m_ResolverIndex = 52;
				pTransition->m_ResolverElseIndex = 57;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 3:
			
			pTransition->m_ProductionIndex = 14;
			pTransition->m_ResolverIndex = 53;
			pTransition->m_ResolverElseIndex = 35;
				
			return ELaDfaResult_Resolver;
			
		
		case 4:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 36;
				pTransition->m_ResolverIndex = 52;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 5:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 42;
				pTransition->m_ResolverIndex = 52;
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
