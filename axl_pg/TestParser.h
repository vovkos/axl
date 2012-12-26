#pragma once



// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)





//.............................................................................

enum ESymbol
{
	ESymbol_program = 0, 
	ESymbol_expression = 1, 
	ESymbol_conditional_expr = 2, 
	ESymbol_logical_or_expr = 3, 
	ESymbol_logical_and_expr = 4, 
	ESymbol_inclusive_or_expr = 5, 
	ESymbol_exclusive_or_expr = 6, 
	ESymbol_and_expr = 7, 
	ESymbol_equality_expr = 8, 
	ESymbol_relational_expr = 9, 
	ESymbol_equality_operator = 10, 
	ESymbol_shift_expr = 11, 
	ESymbol_relational_operator = 12, 
	ESymbol_additive_expr = 13, 
	ESymbol_shift_operator = 14, 
	ESymbol_multiplicative_expr = 15, 
	ESymbol_additivie_operator = 16, 
	ESymbol_assignment_expr = 17, 
	ESymbol_multiplicative_operator = 18, 
	ESymbol_unary_expr = 19, 
	ESymbol_assignment_operator = 20, 
	ESymbol_binop_assignment_operator = 21, 
	ESymbol_postfix_expr = 22, 
	ESymbol_cast_operator_rslv = 23, 
	ESymbol_type_name = 24, 
	ESymbol_primary_expr = 25, 
	ESymbol_postfix_operator = 26, 
	ESymbol_call_operator = 27, 
	
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

		TokenCount         = 45,
		SymbolCount        = 43,
		SequenceCount      = 69,
		ActionCount        = 52,
		ArgumentCount      = 41,
		BeaconCount        = 9,
		LaDfaCount         = 3,

		TotalCount         = 262,

		NamedSymbolCount   = 28,

		TokenFirst         = 0,
		TokenEnd           = 45,
		SymbolFirst        = 45,
		NamedSymbolEnd     = 73,
		SymbolEnd          = 88,
		SequenceFirst      = 88,
		SequenceEnd        = 157,
		ActionFirst        = 157,
		ActionEnd          = 209,
		ArgumentFirst      = 209,
		ArgumentEnd        = 250,
		BeaconFirst        = 250,
		BeaconEnd          = 259,
		LaDfaFirst         = 259,
		LaDfaEnd           = 262,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class _cls1: public CAstNode
	{
	public:
#line 12 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
		
	CValue m_Value;	
;
#line 106 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
	};
	
	class CConditionalExpr: public CAstNode
	{
	public:
#line 22 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
		
	CValue m_TrueValue;
	CValue m_FalseValue;
	CBasicBlock* m_pThenBlock;
	CBasicBlock* m_pElseBlock;
	CBasicBlock* m_pPhiBlock;
;
#line 120 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
	};
	
	class _cls3: public CAstNode
	{
	public:
#line 136 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
		
	EBinOp m_OpKind;
;
#line 130 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
	};
	
	class _cls4: public CAstNode
	{
	public:
#line 165 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
		
	EBinOp m_OpKind;
;
#line 140 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
	};
	
	class _cls5: public CAstNode
	{
	public:
#line 202 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
		
	EBinOp m_OpKind;
;
#line 150 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
	};
	
	class _cls6: public CAstNode
	{
	public:
#line 231 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
		
	EBinOp m_OpKind;
;
#line 160 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
	};
	
	class _cls7: public CAstNode
	{
	public:
#line 260 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
		
	EBinOp m_OpKind;
;
#line 170 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
	};
	
	class _cls8: public CAstNode
	{
	public:
#line 303 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
		
	EAssign m_AssignKind;
	EBinOp m_OpKind;
;
#line 181 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
	};
	
	class _cls9: public CAstNode
	{
	public:
#line 324 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
		
	EBinOp m_OpKind;
;
#line 191 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
	};
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	class CSymbolNode_conditional_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 29 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 208 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		CSymbolNode_conditional_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_conditional_expr;
			
		}
	};
		
	
	class CSymbolNode_logical_or_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 51 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 228 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 53 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue OpValue2;
	;
#line 237 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_logical_or_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_logical_or_expr;
			
		}
	};
		
	
	class CSymbolNode_logical_and_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 65 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 257 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 67 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue OpValue2;
	;
#line 266 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_logical_and_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_logical_and_expr;
			
		}
	};
		
	
	class CSymbolNode_inclusive_or_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 79 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 286 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 81 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue OpValue2;
	;
#line 295 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_inclusive_or_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_inclusive_or_expr;
			
		}
	};
		
	
	class CSymbolNode_exclusive_or_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 93 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 315 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 95 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue OpValue2;
	;
#line 324 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_exclusive_or_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_exclusive_or_expr;
			
		}
	};
		
	
	class CSymbolNode_and_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 107 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 344 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 109 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue OpValue2;
	;
#line 353 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_and_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_and_expr;
			
		}
	};
		
	
	class CSymbolNode_equality_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 121 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 373 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 123 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue OpValue2;
	;
#line 382 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_equality_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_equality_expr;
			
		}
	};
		
	
	class CSymbolNode_relational_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 150 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 402 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 152 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue OpValue2;
	;
#line 411 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_relational_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_relational_expr;
			
		}
	};
		
	
	
	class CSymbolNode_shift_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 187 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 432 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 189 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue OpValue2;
	;
#line 441 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_shift_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_shift_expr;
			
		}
	};
		
	
	
	class CSymbolNode_additive_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 216 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 462 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 218 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue OpValue2;
	;
#line 471 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_additive_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_additive_expr;
			
		}
	};
		
	
	
	class CSymbolNode_multiplicative_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 245 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 492 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 247 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue OpValue2;
	;
#line 501 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_multiplicative_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_multiplicative_expr;
			
		}
	};
		
	
	
	class CSymbolNode_assignment_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 278 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 522 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 280 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue RValue;
	;
#line 531 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_assignment_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_assignment_expr;
			
		}
	};
		
	
	
	class CSymbolNode_unary_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 370 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 552 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		CSymbolNode_unary_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_unary_expr;
			
		}
	};
		
	
	
	
	class CSymbolNode_postfix_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 429 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 574 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		CSymbolNode_postfix_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_postfix_expr;
			
		}
	};
		
	
	
	
	class CSymbolNode_primary_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 458 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 596 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		CSymbolNode_primary_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_primary_expr;
			
		}
	};
		
	
	class CSymbolNode_postfix_operator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 434 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 616 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		CSymbolNode_postfix_operator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_postfix_operator;
			
		}
	};
		
	
	class CSymbolNode_call_operator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 438 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			CValue* pValue;
#line 636 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Arg;	
			
		struct
		{
#line 440 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
		CValue Arg;
		rtl::CBoxListT <CValue> ArgList;
	;
#line 646 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
		} m_Local;	
			
		CSymbolNode_call_operator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_call_operator;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
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
			-1, -1, -1, -1, -1, -1, 46, -1, -1, 46, 46, 46, -1, -1, -1, 46, 46, 46, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 46, 46, 46, 46, 46, -1, 46, 
			-1, -1, -1, -1, -1, -1, 88, -1, -1, 88, 88, 88, -1, -1, -1, 88, 88, 88, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 88, 88, 88, 88, 88, -1, 88, 
			-1, -1, -1, -1, -1, -1, 89, -1, -1, 89, 89, 89, -1, -1, -1, 89, 89, 89, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 89, 89, 89, 89, 89, -1, 89, 
			-1, -1, -1, -1, -1, -1, 91, -1, -1, 91, 91, 91, -1, -1, -1, 91, 91, 91, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 91, 91, 91, 91, 91, -1, 91, 
			-1, -1, -1, -1, -1, -1, 93, -1, -1, 93, 93, 93, -1, -1, -1, 93, 93, 93, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 93, 93, 93, 93, 93, -1, 93, 
			-1, -1, -1, -1, -1, -1, 95, -1, -1, 95, 95, 95, -1, -1, -1, 95, 95, 95, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 95, 95, 95, 95, 95, -1, 95, 
			-1, -1, -1, -1, -1, -1, 97, -1, -1, 97, 97, 97, -1, -1, -1, 97, 97, 97, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 97, 97, 97, 97, 97, -1, 97, 
			-1, -1, -1, -1, -1, -1, 99, -1, -1, 99, 99, 99, -1, -1, -1, 99, 99, 99, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 99, 99, 99, 99, 99, -1, 99, 
			-1, -1, -1, -1, -1, -1, 101, -1, -1, 101, 101, 101, -1, -1, -1, 101, 101, 101, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 101, 101, 101, 101, 101, -1, 101, 
			-1, -1, -1, -1, -1, -1, 105, -1, -1, 105, 105, 105, -1, -1, -1, 105, 105, 105, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 105, 105, 105, 105, 105, -1, 105, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 103, 104, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 111, -1, -1, 111, 111, 111, -1, -1, -1, 111, 111, 111, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 111, 111, 111, 111, 111, -1, 111, 
			-1, -1, -1, -1, -1, -1, -1, 107, 108, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 109, 110, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 115, -1, -1, 115, 115, 115, -1, -1, -1, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, 115, 115, -1, 115, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 113, 114, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 119, -1, -1, 119, 119, 119, -1, -1, -1, 119, 119, 119, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 119, 119, 119, 119, 119, -1, 119, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 117, 118, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 124, -1, -1, 124, 124, 124, -1, -1, -1, 124, 124, 124, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 124, 124, 124, 124, 124, -1, 124, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 121, 122, 123, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 140, -1, -1, 142, 143, 141, -1, -1, -1, 144, 145, 259, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 146, 147, 149, 149, 149, -1, 139, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 126, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 128, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 151, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 151, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 150, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 43, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 44, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 153, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 260, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 92, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 94, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 96, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 0, 98, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 102, 102, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 0, 0, 0, 106, 106, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 106, 106, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 112, 112, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 0, 0, 0, 0, 0, 116, 116, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, 120, 120, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 125, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 148, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 257, 41, 0, -1, -1, 
			0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 152, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 
			
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
			209, 47,  -1, // 0
			73, 210, 48,  -1, // 1
			159, 212, 47, 3, 158, 211, 47, 2, 157,  -1, // 2
			74, 213, 49,  -1, // 3
			74, 160, 214, 49, 19,  -1, // 4
			75, 215, 50,  -1, // 5
			75, 161, 216, 50, 20,  -1, // 6
			76, 217, 51,  -1, // 7
			76, 162, 218, 51, 4,  -1, // 8
			77, 219, 52,  -1, // 9
			77, 163, 220, 52, 5,  -1, // 10
			78, 221, 53,  -1, // 11
			78, 164, 222, 53, 6,  -1, // 12
			79, 223, 54,  -1, // 13
			79, 165, 224, 54, 250,  -1, // 14
			166, 21,  -1, // 15
			167, 22,  -1, // 16
			80, 225, 56,  -1, // 17
			80, 168, 226, 56, 251,  -1, // 18
			169, 7,  -1, // 19
			170, 8,  -1, // 20
			171, 23,  -1, // 21
			172, 24,  -1, // 22
			81, 227, 58,  -1, // 23
			81, 173, 228, 58, 252,  -1, // 24
			174, 25,  -1, // 25
			175, 26,  -1, // 26
			82, 229, 60,  -1, // 27
			82, 176, 230, 60, 253,  -1, // 28
			177, 9,  -1, // 29
			178, 10,  -1, // 30
			83, 231, 62,  -1, // 31
			83, 179, 232, 62, 254,  -1, // 32
			180, 11,  -1, // 33
			181, 12,  -1, // 34
			182, 13,  -1, // 35
			84, 233, 64,  -1, // 36
			183, 234, 47, 255,  -1, // 37
			184, 14,  -1, // 38
			185, 66,  -1, // 39
			186, 27,  -1, // 40
			187, 28,  -1, // 41
			188, 29,  -1, // 42
			189, 30,  -1, // 43
			190, 31,  -1, // 44
			191, 32,  -1, // 45
			192, 33,  -1, // 46
			193, 34,  -1, // 47
			194, 35,  -1, // 48
			195, 36,  -1, // 49
			196, 37,  -1, // 50
			235, 67,  -1, // 51
			197, 236, 64, 6,  -1, // 52
			198, 237, 64, 11,  -1, // 53
			238, 64, 9,  -1, // 54
			199, 239, 64, 10,  -1, // 55
			200, 240, 64, 15,  -1, // 56
			201, 241, 64, 16,  -1, // 57
			202, 242, 64, 38,  -1, // 58
			203, 243, 64, 39,  -1, // 59
			205, 244, 64, 18, 256, 204, 17,  -1, // 60
			206, 258, 42, 86,  -1, // 61
			207, 43, 17,  -1, // 62
			87, 245, 70,  -1, // 63
			87, 246, 71,  -1, // 64
			247, 72,  -1, // 65
			18, 208, 248, 47, 17,  -1, // 66
			18, 17,  -1, // 67
			18, 249, 47, 17,  -1, // 68
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 7, 17, 21, 27, 31, 37, 41, 47, 51, 57, 61, 67, 71, 77, 80, 83, 87, 93, 96, 99, 102, 105, 109, 115, 118, 121, 125, 131, 134, 137, 141, 147, 150, 153, 156, 160, 165, 168, 171, 174, 177, 180, 183, 186, 189, 192, 195, 198, 201, 204, 207, 212, 217, 221, 226, 231, 236, 241, 246, 254, 259, 263, 267, 271, 274, 280, 283, 
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

		
		case '?':
			return 2;
		
		case ':':
			return 3;
		
		case '|':
			return 4;
		
		case '^':
			return 5;
		
		case '&':
			return 6;
		
		case '<':
			return 7;
		
		case '>':
			return 8;
		
		case '+':
			return 9;
		
		case '-':
			return 10;
		
		case '*':
			return 11;
		
		case '/':
			return 12;
		
		case '%':
			return 13;
		
		case '=':
			return 14;
		
		case '~':
			return 15;
		
		case '!':
			return 16;
		
		case '(':
			return 17;
		
		case ')':
			return 18;
		
		case EToken_LogicalOr:
			return 19;
		
		case EToken_LogicalAnd:
			return 20;
		
		case EToken_Eq:
			return 21;
		
		case EToken_Ne:
			return 22;
		
		case EToken_Le:
			return 23;
		
		case EToken_Ge:
			return 24;
		
		case EToken_Shl:
			return 25;
		
		case EToken_Shr:
			return 26;
		
		case EToken_RefAssign:
			return 27;
		
		case EToken_AddAssign:
			return 28;
		
		case EToken_SubAssign:
			return 29;
		
		case EToken_MulAssign:
			return 30;
		
		case EToken_DivAssign:
			return 31;
		
		case EToken_ModAssign:
			return 32;
		
		case EToken_ShlAssign:
			return 33;
		
		case EToken_ShrAssign:
			return 34;
		
		case EToken_AndAssign:
			return 35;
		
		case EToken_XorAssign:
			return 36;
		
		case EToken_OrAssign:
			return 37;
		
		case EToken_Inc:
			return 38;
		
		case EToken_Dec:
			return 39;
		
		case EToken_Stack:
			return 40;
		
		case EToken_Heap:
			return 41;
		
		case EToken_New:
			return 42;
		
		case Token_Float:
			return 43;
		
		case EToken_Identifier:
			return 44;
		
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
			'?', 
			':', 
			'|', 
			'^', 
			'&', 
			'<', 
			'>', 
			'+', 
			'-', 
			'*', 
			'/', 
			'%', 
			'=', 
			'~', 
			'!', 
			'(', 
			')', 
			EToken_LogicalOr, 
			EToken_LogicalAnd, 
			EToken_Eq, 
			EToken_Ne, 
			EToken_Le, 
			EToken_Ge, 
			EToken_Shl, 
			EToken_Shr, 
			EToken_RefAssign, 
			EToken_AddAssign, 
			EToken_SubAssign, 
			EToken_MulAssign, 
			EToken_DivAssign, 
			EToken_ModAssign, 
			EToken_ShlAssign, 
			EToken_ShrAssign, 
			EToken_AndAssign, 
			EToken_XorAssign, 
			EToken_OrAssign, 
			EToken_Inc, 
			EToken_Dec, 
			EToken_Stack, 
			EToken_Heap, 
			EToken_New, 
			Token_Float, 
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
			_T("expression"),
			_T("conditional_expr"),
			_T("logical_or_expr"),
			_T("logical_and_expr"),
			_T("inclusive_or_expr"),
			_T("exclusive_or_expr"),
			_T("and_expr"),
			_T("equality_expr"),
			_T("relational_expr"),
			_T("equality_operator"),
			_T("shift_expr"),
			_T("relational_operator"),
			_T("additive_expr"),
			_T("shift_operator"),
			_T("multiplicative_expr"),
			_T("additivie_operator"),
			_T("assignment_expr"),
			_T("multiplicative_operator"),
			_T("unary_expr"),
			_T("assignment_operator"),
			_T("binop_assignment_operator"),
			_T("postfix_expr"),
			_T("cast_operator_rslv"),
			_T("type_name"),
			_T("primary_expr"),
			_T("postfix_operator"),
			_T("call_operator"),
			
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
		
		
		case ESymbol_expression:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls1);
				
			break;
			
		
		case ESymbol_conditional_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_conditional_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CConditionalExpr);
			
			break;
			
		
		case ESymbol_logical_or_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_logical_or_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_logical_and_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_logical_and_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_inclusive_or_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_inclusive_or_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_exclusive_or_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_exclusive_or_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_and_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_and_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_equality_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_equality_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_relational_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_relational_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_equality_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls3);
				
			break;
			
		
		case ESymbol_shift_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_shift_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_relational_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls4);
				
			break;
			
		
		case ESymbol_additive_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_additive_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_shift_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls5);
				
			break;
			
		
		case ESymbol_multiplicative_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_multiplicative_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_additivie_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls6);
				
			break;
			
		
		case ESymbol_assignment_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_assignment_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_multiplicative_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls7);
				
			break;
			
		
		case ESymbol_unary_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_unary_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_assignment_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls8);
				
			break;
			
		
		case ESymbol_binop_assignment_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls9);
				
			break;
			
		
		case ESymbol_postfix_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_postfix_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		
		case ESymbol_primary_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_primary_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_postfix_operator:
			pNode = AXL_MEM_NEW (CSymbolNode_postfix_operator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_call_operator:
			pNode = AXL_MEM_NEW (CSymbolNode_call_operator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
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
			
			{ 0, 55 },
			
			{ 0, 57 },
			
			{ 0, 59 },
			
			{ 0, 61 },
			
			{ 0, 63 },
			
			{ 0, 65 },
			
			{ 0, 69 },
			
			{ 0, 40 },
			
			{ 1, 69 },
			
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
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
#line 32 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("cond_then"));
			(*__pAstNode).m_pElseBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("cond_else"));
			(*__pAstNode).m_pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("cond_phi"));			
			m_pModule->m_ControlFlowMgr.ConditionalJump (*__pSymbol->m_Arg.pValue, (*__pAstNode).m_pThenBlock, (*__pAstNode).m_pElseBlock);
		;
#line 1342 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
#line 40 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock ((*__pAstNode).m_pElseBlock); // might have changed				
			;
#line 1355 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
#line 45 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				FinalizeConditionalExpr (&(*__pAstNode), __pSymbol->m_Arg.pValue);
			;
#line 1368 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 59 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogicalAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 1381 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 73 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogicalOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 1394 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 5:
			{
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 87 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BitwiseOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 1407 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 6:
			{
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 101 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BitwiseXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 1420 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 7:
			{
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 115 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BitwiseAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 1433 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 8:
			{
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 129 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls3*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 1446 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 9:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
#line 141 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Eq;
		;
#line 1459 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 10:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
#line 145 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ne;
		;
#line 1472 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 11:
			{
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 158 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls4*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 1485 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 12:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
#line 170 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Lt;
		;
#line 1498 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 13:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
#line 174 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Gt;
		;
#line 1511 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 14:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
#line 178 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Le;
		;
#line 1524 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 15:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
#line 182 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ge;
		;
#line 1537 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 16:
			{
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 195 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls5*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 1550 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 17:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
#line 207 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shl;
		;
#line 1563 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 18:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
#line 211 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shr;
		;
#line 1576 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 19:
			{
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 224 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls6*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 1589 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 20:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
#line 236 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Add;
		;
#line 1602 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 21:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
#line 240 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Sub;
		;
#line 1615 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 22:
			{
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 253 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls7*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 1628 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 23:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
#line 265 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mul;
		;
#line 1641 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 24:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
#line 269 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Div;
		;
#line 1654 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 25:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
#line 273 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mod;
		;
#line 1667 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 26:
			{
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 286 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
				switch ((*(_cls8*) GetAstLocator (0)).m_AssignKind)
				{
				case EAssign_Normal:
					return m_pModule->m_OperatorMgr.MoveOperator (__pSymbol->m_Local.RValue, *__pSymbol->m_Arg.pValue);

				case EAssign_BinOp:
					return m_pModule->m_OperatorMgr.BinOpMoveOperator (__pSymbol->m_Local.RValue, *__pSymbol->m_Arg.pValue, (*(_cls8*) GetAstLocator (0)).m_OpKind);

				case EAssign_Ref:
					return m_pModule->m_OperatorMgr.RefMoveOperator (__pSymbol->m_Local.RValue, *__pSymbol->m_Arg.pValue);
				}
			;
#line 1690 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 27:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 309 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_AssignKind = EAssign_Normal;
		;
#line 1703 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 28:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 313 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_AssignKind = EAssign_BinOp;
			(*__pAstNode).m_OpKind = EBinOp_None;
		;
#line 1717 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 29:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 318 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_AssignKind = EAssign_Ref;
		;
#line 1730 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 30:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 329 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Add;
		;
#line 1743 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 31:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 333 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Sub;
		;
#line 1756 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 32:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 337 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mul;
		;
#line 1769 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 33:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 341 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Div;
		;
#line 1782 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 34:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 345 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mod;
		;
#line 1795 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 35:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 349 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shl;
		;
#line 1808 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 36:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 353 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shr;
		;
#line 1821 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 37:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 357 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_BitwiseAnd;
		;
#line 1834 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 38:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 361 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_BitwiseXor;
		;
#line 1847 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 39:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 365 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_BitwiseOr;
		;
#line 1860 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 40:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 373 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, __pSymbol->m_Arg.pValue);
		;
#line 1873 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 41:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 377 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, __pSymbol->m_Arg.pValue);
		;
#line 1886 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 42:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 382 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Minus, __pSymbol->m_Arg.pValue);
		;
#line 1899 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 43:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 386 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_BitwiseNot, __pSymbol->m_Arg.pValue);
		;
#line 1912 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 44:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 390 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_LogicalNot, __pSymbol->m_Arg.pValue);
		;
#line 1925 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 45:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 394 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
		;
#line 1938 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 46:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 398 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
		;
#line 1951 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 47:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 403 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			_asm int 3
		;
#line 1964 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 48:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 408 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			return m_pModule->m_OperatorMgr.CastOperator (__pSymbol->m_Arg.pValue, (*GetAstLocator (0)).m_pType);
		;
#line 1977 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 49:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 412 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			return IsValidLocator ((*GetTokenLocator (0))) ? 
				m_pModule->m_OperatorMgr.StackNewOperator ((*GetAstLocator (1)).m_pType, __pSymbol->m_Arg.pValue) : 
				m_pModule->m_OperatorMgr.HeapNewOperator ((*GetAstLocator (1)).m_pType, __pSymbol->m_Arg.pValue);
		;
#line 1992 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 50:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 424 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			_asm int 3
		;
#line 2005 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}

			return true;
		
		case 51:
			{
			CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 450 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			
			__pSymbol->m_Local.ArgList.InsertTail (__pSymbol->m_Local.Arg);
		;
#line 2018 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
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
		
		case 0:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls1* __pAstNode = (_cls1*) __pSymbol->m_pAstNode;
			
#line 16 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
#line 2046 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_logical_or_expr* __pTarget = (CSymbolNode_logical_or_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
			
#line 30 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2059 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
			
#line 39 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_TrueValue;
#line 2072 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
			
#line 44 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_FalseValue;
#line 2085 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 56 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2098 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 58 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 2111 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 70 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2124 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 72 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 2137 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 84 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2150 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 86 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 2163 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 98 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2176 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 100 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 2189 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 112 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2202 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 114 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 2215 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 126 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2228 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 128 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 2241 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 155 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2254 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 157 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 2267 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 192 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2280 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 194 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 2293 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 221 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2306 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 223 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 2319 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 250 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2332 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 252 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 2345 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 283 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2358 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 285 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.RValue;
#line 2371 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_postfix_expr* __pTarget = (CSymbolNode_postfix_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 371 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2384 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 372 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2397 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 376 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2410 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 380 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2423 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 381 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2436 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 385 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2449 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 389 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2462 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 393 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2475 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 397 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2488 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 407 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2501 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 430 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2514 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_postfix_operator* __pTarget = (CSymbolNode_postfix_operator*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 431 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2527 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_call_operator* __pTarget = (CSymbolNode_call_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 435 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2540 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 449 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.Arg;
#line 2553 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 460 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 2566 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			
			}
			break;
		
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
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_call_operator:
			{
				CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 445 "D:/Prj/Ninja/axl3/axl_pg/TestParser.llk"
				
		return m_pModule->m_OperatorMgr.CallOperator (__pSymbol->m_Arg.pValue, &__pSymbol->m_Local.ArgList);
	;
#line 2656 "D:/Prj/Ninja/axl3/axl_pg/TestParser.h"
			}
			return true;
			
		
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
					
				pTransition->m_ProductionIndex = 148;
				pTransition->m_ResolverIndex = 68;
				pTransition->m_ResolverElseIndex = 139;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 261;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case '&':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '(':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 155;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
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
