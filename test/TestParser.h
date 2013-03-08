#pragma once



// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)





//.............................................................................

enum ESymbol
{
	ESymbol_declaration = 0, 
	ESymbol_type_specifier = 1, 
	ESymbol_type_modifier = 2, 
	ESymbol_declarator = 3, 
	ESymbol_type_property_specifier_rslv = 4, 
	
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

		TokenCount         = 10,
		SymbolCount        = 9,
		SequenceCount      = 9,
		ActionCount        = 5,
		ArgumentCount      = 0,
		BeaconCount        = 2,
		LaDfaCount         = 3,

		TotalCount         = 38,

		NamedSymbolCount   = 5,

		TokenFirst         = 0,
		TokenEnd           = 10,
		SymbolFirst        = 10,
		NamedSymbolEnd     = 15,
		SymbolEnd          = 19,
		SequenceFirst      = 19,
		SequenceEnd        = 28,
		ActionFirst        = 28,
		ActionEnd          = 33,
		ArgumentFirst      = 33,
		ArgumentEnd        = 33,
		BeaconFirst        = 33,
		BeaconEnd          = 35,
		LaDfaFirst         = 35,
		LaDfaEnd           = 38,
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
			-1, -1, -1, -1, -1, 20, 20, -1, 20, 20, 
			-1, -1, -1, -1, -1, 21, 18, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 23, -1, -1, 24, 
			-1, -1, -1, -1, -1, -1, -1, -1, 27, -1, 
			-1, -1, -1, -1, -1, -1, 35, -1, -1, -1, 
			-1, -1, -1, -1, -1, 11, 37, -1, -1, 12, 
			-1, -1, -1, -1, -1, 19, 19, -1, 0, 19, 
			-1, -1, -1, -1, -1, -1, -1, 7, 0, -1, 
			-1, -1, -1, -1, -1, -1, 22, -1, -1, -1, 
			
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
			16, 15,  -1, // 0
			2, 13, 16,  -1, // 1
			28, 5,  -1, // 2
			29, 4, 3, 33, 17, 6,  -1, // 3
			30, 6,  -1, // 4
			31, 9,  -1, // 5
			3, 8, 6,  -1, // 6
			7, 6,  -1, // 7
			32, 34,  -1, // 8
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 7, 10, 17, 20, 23, 27, 30, 
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
		
		case '{':
			return 3;
		
		case '}':
			return 4;
		
		case EToken_BYTE:
			return 5;
		
		case EToken_PROPERTY:
			return 6;
		
		case EToken_Integer:
			return 7;
		
		case EToken_Identifier:
			return 8;
		
		case EToken_CONST:
			return 9;
		
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
			'{', 
			'}', 
			EToken_BYTE, 
			EToken_PROPERTY, 
			EToken_Integer, 
			EToken_Identifier, 
			EToken_CONST, 
			
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
			_T("declaration"),
			_T("type_specifier"),
			_T("type_modifier"),
			_T("declarator"),
			_T("type_property_specifier_rslv"),
			
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
			
			{ 0, 8 },
			
			{ 0, 8 },
			
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
 #line 11 "D:/Prj/Ninja/axl3/test/TestParser.llk"
			
			printf ("spec:byte\n");
		;
 #line 287 "D:/Prj/Ninja/axl3/test/TestParser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
 #line 16 "D:/Prj/Ninja/axl3/test/TestParser.llk"
			
			printf ("spec:property %s {}\n", (*GetTokenLocator (0)).m_Data.m_String);
		;
 #line 300 "D:/Prj/Ninja/axl3/test/TestParser.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
 #line 23 "D:/Prj/Ninja/axl3/test/TestParser.llk"
			
			printf ("mod:property\n");
		;
 #line 313 "D:/Prj/Ninja/axl3/test/TestParser.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
 #line 27 "D:/Prj/Ninja/axl3/test/TestParser.llk"
			
			printf ("mod:const\n");
		;
 #line 326 "D:/Prj/Ninja/axl3/test/TestParser.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
 #line 39 "D:/Prj/Ninja/axl3/test/TestParser.llk"
			
			printf ("decl:id %s\n", (*GetTokenLocator (0)).m_Data.m_String);
		;
 #line 339 "D:/Prj/Ninja/axl3/test/TestParser.h"
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
			
			case EToken_PROPERTY:
					
				pTransition->m_ProductionIndex = 36;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 26;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 25;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case EToken_PROPERTY:
					
				pTransition->m_ProductionIndex = 11;
				pTransition->m_ResolverIndex = 14;
				pTransition->m_ResolverElseIndex = 12;
						
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
