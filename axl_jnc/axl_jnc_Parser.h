#pragma once


	#include "axl_jnc_Lexer.h"
	#include "axl_jnc_Module.h"
	#include "axl_jnc_Decl.h"
	#include "axl_llk_Parser.h"


// warning C4065: switch statement contains 'default' but no 'case' labels

#pragma warning (disable: 4065)




	namespace axl {
	namespace jnc {


//.............................................................................

enum ESymbol
{
	ESymbol_compilation_unit = 0, 
	ESymbol_global_declaration = 1, 
	ESymbol_namespace_declaration = 2, 
	ESymbol_using_declaration = 3, 
	ESymbol_common_declaration = 4, 
	ESymbol_function_body_pass1 = 5, 
	ESymbol_local_declaration = 6, 
	ESymbol_statement = 7, 
	ESymbol_qualified_name = 8, 
	ESymbol_type_name = 9, 
	ESymbol_type_specifier_modifier_list = 10, 
	ESymbol_pointer = 11, 
	ESymbol_declarator_suffix = 12, 
	ESymbol_generic_formal_argument_list = 13, 
	ESymbol_generic_formal_argument = 14, 
	ESymbol_expression_pass1 = 15, 
	ESymbol_primary_expr_pass1 = 16, 
	ESymbol_expression = 17, 
	ESymbol_conditional_expr = 18, 
	ESymbol_constant_expr = 19, 
	ESymbol_constant_integer_expr = 20, 
	ESymbol_logical_or_expr = 21, 
	ESymbol_logical_and_expr = 22, 
	ESymbol_inclusive_or_expr = 23, 
	ESymbol_exclusive_or_expr = 24, 
	ESymbol_and_expr = 25, 
	ESymbol_equality_expr = 26, 
	ESymbol_relational_expr = 27, 
	ESymbol_equality_operator = 28, 
	ESymbol_shift_expr = 29, 
	ESymbol_relational_operator = 30, 
	ESymbol_additive_expr = 31, 
	ESymbol_shift_operator = 32, 
	ESymbol_multiplicative_expr = 33, 
	ESymbol_additivie_operator = 34, 
	ESymbol_assignment_expr = 35, 
	ESymbol_multiplicative_operator = 36, 
	ESymbol_unary_expr = 37, 
	ESymbol_assignment_operator = 38, 
	ESymbol_postfix_expr = 39, 
	ESymbol_cast_operator_rslv = 40, 
	ESymbol_type_specifier_modifier = 41, 
	ESymbol_primary_expr = 42, 
	ESymbol_postfix_operator = 43, 
	ESymbol_member_operator = 44, 
	ESymbol_index_operator = 45, 
	ESymbol_call_operator = 46, 
	ESymbol_type_name_or_expr = 47, 
	ESymbol_compound_stmt_pass1 = 48, 
	ESymbol_statement_pass1 = 49, 
	ESymbol_compound_stmt = 50, 
	ESymbol_expression_stmt = 51, 
	ESymbol_if_stmt = 52, 
	ESymbol_switch_stmt = 53, 
	ESymbol_while_stmt = 54, 
	ESymbol_do_stmt = 55, 
	ESymbol_for_stmt = 56, 
	ESymbol_break_stmt = 57, 
	ESymbol_continue_stmt = 58, 
	ESymbol_return_stmt = 59, 
	ESymbol_switch_block_stmt = 60, 
	ESymbol_attribute_block = 61, 
	ESymbol_attribute_declarator_list = 62, 
	ESymbol_attribute_declarator = 63, 
	ESymbol_declaration_specifier_list = 64, 
	ESymbol_init_declarator_list = 65, 
	ESymbol_declaration_terminator_2 = 66, 
	ESymbol_init_declarator = 67, 
	ESymbol_declarator = 68, 
	ESymbol_initializer = 69, 
	ESymbol_initializer_list = 70, 
	ESymbol_initializer_list_entry = 71, 
	ESymbol_declaration_terminator = 72, 
	ESymbol_declaration_specifier = 73, 
	ESymbol_access_specifier = 74, 
	ESymbol_storage_class_specifier = 75, 
	ESymbol_type_specifier = 76, 
	ESymbol_type_modifier = 77, 
	ESymbol_enum_specifier = 78, 
	ESymbol_struct_specifier = 79, 
	ESymbol_class_specifier = 80, 
	ESymbol_property_specifier = 81, 
	ESymbol_qualified_type_name_rslv = 82, 
	ESymbol_qualified_type_name = 83, 
	ESymbol_generic_actual_argument_list = 84, 
	ESymbol_property_accessor_kind = 85, 
	ESymbol_pointer_kind = 86, 
	ESymbol_array_suffix = 87, 
	ESymbol_function_suffix = 88, 
	ESymbol_function_formal_argument_list = 89, 
	ESymbol_function_formal_argument = 90, 
	ESymbol_pragma_endian = 91, 
	ESymbol_pragma_endian_value = 92, 
	ESymbol_enum_kind = 93, 
	ESymbol_enum_block = 94, 
	ESymbol_enum_member_list = 95, 
	ESymbol_enum_member = 96, 
	ESymbol_struct_kind = 97, 
	ESymbol_struct_pack_factor = 98, 
	ESymbol_inheritance_list = 99, 
	ESymbol_struct_block = 100, 
	ESymbol_struct_pack_factor_value = 101, 
	ESymbol_struct_member_declaration = 102, 
	ESymbol_struct_member_declarator_list = 103, 
	ESymbol_struct_member_declarator = 104, 
	ESymbol_struct_member_bit_field = 105, 
	ESymbol_pragma_pack = 106, 
	ESymbol_class_kind = 107, 
	ESymbol_class_block = 108, 
	ESymbol_base_class = 109, 
	ESymbol_class_member_declaration = 110, 
	ESymbol_class_member_declarator_list = 111, 
	ESymbol_class_member_declarator = 112, 
	ESymbol_property_block = 113, 
	ESymbol_property_accessor_declaration = 114, 
	ESymbol_property_declarator = 115, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 2,

		StartSymbol        = 0,
		StartPragmaSymbol  = 195,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 108,
		SymbolCount        = 196,
		SequenceCount      = 264,
		ActionCount        = 165,
		ArgumentCount      = 109,
		BeaconCount        = 71,
		LaDfaCount         = 39,

		TotalCount         = 952,

		NamedSymbolCount   = 116,

		TokenFirst         = 0,
		TokenEnd           = 108,
		SymbolFirst        = 108,
		NamedSymbolEnd     = 224,
		SymbolEnd          = 304,
		SequenceFirst      = 304,
		SequenceEnd        = 568,
		ActionFirst        = 568,
		ActionEnd          = 733,
		ArgumentFirst      = 733,
		ArgumentEnd        = 842,
		BeaconFirst        = 842,
		BeaconEnd          = 913,
		LaDfaFirst         = 913,
		LaDfaEnd           = 952,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class _cls1: public CAstNode
	{
	public:
#line 20 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CQualifiedName m_Name;
;
#line 198 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls2: public CAstNode
	{
	public:
#line 38 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
#line 208 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls3: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
#line 218 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls4: public CAstNode
	{
	public:
#line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
#line 228 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls5: public CAstNode
	{
	public:
#line 34 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
#line 238 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls6: public CAstNode
	{
	public:
#line 50 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	intptr_t m_Value;
;
#line 248 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls7: public CAstNode
	{
	public:
#line 163 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 258 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls8: public CAstNode
	{
	public:
#line 192 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 268 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls9: public CAstNode
	{
	public:
#line 229 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 278 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls10: public CAstNode
	{
	public:
#line 258 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 288 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls11: public CAstNode
	{
	public:
#line 287 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 298 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls12: public CAstNode
	{
	public:
#line 320 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 308 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls13: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
#line 318 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class CCompoundStmt: public CAstNode
	{
	public:
#line 36 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
		
	CScope* m_pScope;
;
#line 328 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls15: public CAstNode
	{
	public:
#line 11 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CModuleItem* m_pItem;
;
#line 338 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls16: public CAstNode
	{
	public:
#line 27 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CModuleItem* m_pItem;
;
#line 348 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls17: public CAstNode
	{
	public:
#line 56 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CValue m_Value;
	rtl::CBoxListT <CToken> m_TokenList;
;
#line 359 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls18: public CAstNode
	{
	public:
#line 103 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CDeclSpecifiers m_DeclSpecifiers;
;
#line 369 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls19: public CAstNode
	{
	public:
#line 119 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CTypeSpecifierModifiers m_TypeSpecifier;
;
#line 379 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls20: public CAstNode
	{
	public:
#line 308 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CType* m_pType;
;
#line 389 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls21: public CAstNode
	{
	public:
#line 332 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CDeclarator m_Declarator;
;
#line 399 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls22: public CAstNode
	{
	public:
#line 366 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	EType m_TypeKind;
;
#line 409 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls23: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
		
	CEnumType* m_pType;
;
#line 419 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls24: public CAstNode
	{
	public:
#line 31 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
		
	EType m_TypeKind;
;
#line 429 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls25: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
		
	CStructType* m_pType;
;
#line 439 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls26: public CAstNode
	{
	public:
#line 27 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
		
	EType m_TypeKind;
;
#line 449 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls27: public CAstNode
	{
	public:
#line 42 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
		
	size_t m_PackFactor;
;
#line 459 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls28: public CAstNode
	{
	public:
#line 53 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
		
	size_t m_Value;
;
#line 469 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls29: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
		
	CClassType* m_pType;
;
#line 479 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls30: public CAstNode
	{
	public:
#line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
		
	EType m_TypeKind;
;
#line 489 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls31: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	CProperty* m_pProperty;
;
#line 499 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class CPropertyBlock: public CAstNode
	{
	public:
#line 15 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	CFunction* m_pGetter;
	CFunctionOverload m_Setter;

	CPropertyBlock ()
	{
		m_pGetter = NULL;
	}
;
#line 515 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls33: public CAstNode
	{
	public:
#line 39 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	CDeclarator m_Declarator;
;
#line 525 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls34: public CAstNode
	{
	public:
#line 53 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	EPropertyAccessor m_PropertyAccessorKind;
;
#line 535 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_type_name: public CSymbolNode
	{
	public:
			
		struct
		{
#line 43 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
				
		CDeclarator Declarator;
	;
#line 561 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_type_name ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_type_name;
			
		}
	};
		
	
	
	class CSymbolNode_pointer: public CSymbolNode
	{
	public:
			
		struct
		{
#line 353 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 582 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 355 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		CDeclPointer* pPointer;
	;
#line 591 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_pointer ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_pointer;
			
		}
	};
		
	
	class CSymbolNode_declarator_suffix: public CSymbolNode
	{
	public:
			
		struct
		{
#line 383 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 611 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_declarator_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_declarator_suffix;
			
		}
	};
		
	
	class CSymbolNode_generic_formal_argument_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 56 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			CStructClassType* pType;
#line 631 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_generic_formal_argument_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_generic_formal_argument_list;
			
		}
	};
		
	
	class CSymbolNode_generic_formal_argument: public CSymbolNode
	{
	public:
			
		struct
		{
#line 60 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			CStructClassType* pType;
#line 651 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_generic_formal_argument ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_generic_formal_argument;
			
		}
	};
		
	
	
	class CSymbolNode_primary_expr_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
#line 12 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			rtl::CBoxListT <CToken>* pTokenList;
#line 672 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_primary_expr_pass1 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_primary_expr_pass1;
			
		}
	};
		
	
	
	class CSymbolNode_conditional_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 63 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 693 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 65 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue TrueValue;
	CValue FalseValue;
;
#line 703 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
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
#line 78 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 725 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 80 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 734 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 92 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 754 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 94 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 763 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 106 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 783 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 108 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 792 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 120 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 812 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 122 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 821 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 134 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 841 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 136 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 850 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 148 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 870 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 150 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 879 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 177 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 899 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 179 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 908 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 214 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 929 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 216 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 938 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 243 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 959 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 245 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 968 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 272 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 989 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 998 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 305 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1019 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 307 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue RValue;
;
#line 1028 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 370 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1049 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 419 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1070 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_postfix_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_postfix_expr;
			
		}
	};
		
	
	class CSymbolNode_cast_operator_rslv: public CSymbolNode
	{
	public:
			
		struct
		{
#line 413 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CTypeSpecifierModifiers TypeSpecifier;
;
#line 1092 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_cast_operator_rslv ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_cast_operator_rslv;
			
		}
	};
		
	
	class CSymbolNode_type_specifier_modifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 141 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CTypeSpecifierModifiers* pTypeSpecifier;
#line 1112 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_type_specifier_modifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_type_specifier_modifier;
			
		}
	};
		
	
	class CSymbolNode_primary_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 483 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1132 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 485 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue;
;
#line 1141 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
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
#line 424 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1161 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_postfix_operator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_postfix_operator;
			
		}
	};
		
	
	class CSymbolNode_member_operator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 438 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1181 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_member_operator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_member_operator;
			
		}
	};
		
	
	class CSymbolNode_index_operator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 445 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1201 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 447 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue IndexValue;
;
#line 1210 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_index_operator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_index_operator;
			
		}
	};
		
	
	class CSymbolNode_call_operator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 456 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1230 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 458 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue Arg;
	rtl::CBoxListT <CValue> ArgList;
;
#line 1240 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_call_operator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_call_operator;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_type_name_or_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 527 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1262 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_type_name_or_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_type_name_or_expr;
			
		}
	};
		
	
	class CSymbolNode_compound_stmt_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
#line 12 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			rtl::CBoxListT <CToken>* pTokenList;
#line 1282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_compound_stmt_pass1 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_compound_stmt_pass1;
			
		}
	};
		
	
	class CSymbolNode_statement_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
#line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			rtl::CBoxListT <CToken>* pTokenList;
#line 1302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_statement_pass1 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_statement_pass1;
			
		}
	};
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_init_declarator_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 14 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclSpecifiers* pDeclSpecifiers;
#line 1337 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_init_declarator_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_init_declarator_list;
			
		}
	};
		
	
	class CSymbolNode_declaration_terminator_2: public CSymbolNode
	{
	public:
			
		struct
		{
#line 86 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CModuleItem* pItem;
#line 1357 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_declaration_terminator_2 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_declaration_terminator_2;
			
		}
	};
		
	
	class CSymbolNode_init_declarator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 30 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclSpecifiers* pDeclSpecifiers;
#line 1377 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_init_declarator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_init_declarator;
			
		}
	};
		
	
	
	class CSymbolNode_initializer: public CSymbolNode
	{
	public:
			
		struct
		{
#line 60 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CType* pType;
#line 1398 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_initializer ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_initializer;
			
		}
	};
		
	
	class CSymbolNode_initializer_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 74 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CType* pType;
#line 1418 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_initializer_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_initializer_list;
			
		}
	};
		
	
	class CSymbolNode_initializer_list_entry: public CSymbolNode
	{
	public:
			
		struct
		{
#line 78 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CType* pType;
#line 1438 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_initializer_list_entry ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_initializer_list_entry;
			
		}
	};
		
	
	
	class CSymbolNode_declaration_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 134 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclSpecifiers* pDeclSpecifiers;
#line 1459 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_declaration_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_declaration_specifier;
			
		}
	};
		
	
	class CSymbolNode_access_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 146 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CAccessSpecifier* pAccessSpecifier;
#line 1479 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_access_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_access_specifier;
			
		}
	};
		
	
	class CSymbolNode_storage_class_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 157 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclSpecifiers* pDeclSpecifiers;
#line 1499 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_storage_class_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_storage_class_specifier;
			
		}
	};
		
	
	class CSymbolNode_type_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 207 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CTypeSpecifier* pTypeSpecifier;
#line 1519 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_type_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_type_specifier;
			
		}
	};
		
	
	class CSymbolNode_type_modifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 168 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CTypeModifiers* pTypeModifiers;
#line 1539 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_type_modifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_type_modifier;
			
		}
	};
		
	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_array_suffix: public CSymbolNode
	{
	public:
			
		struct
		{
#line 388 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 1568 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_array_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_array_suffix;
			
		}
	};
		
	
	class CSymbolNode_function_suffix: public CSymbolNode
	{
	public:
			
		struct
		{
#line 399 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 1588 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 401 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
		CDeclFunctionSuffix* pFunctionSuffix;
	;
#line 1597 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_function_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_function_suffix;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_function_formal_argument_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 412 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
#line 1619 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_function_formal_argument_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_function_formal_argument_list;
			
		}
	};
		
	
	class CSymbolNode_function_formal_argument: public CSymbolNode
	{
	public:
			
		struct
		{
#line 417 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
#line 1639 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_function_formal_argument ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_function_formal_argument;
			
		}
	};
		
	
	
	
	
	class CSymbolNode_enum_block: public CSymbolNode
	{
	public:
			
		struct
		{
#line 45 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			CEnumType* pType;
#line 1662 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_enum_block ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_enum_block;
			
		}
	};
		
	
	class CSymbolNode_enum_member_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			CEnumType* pType;
#line 1682 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_enum_member_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_enum_member_list;
			
		}
	};
		
	
	class CSymbolNode_enum_member: public CSymbolNode
	{
	public:
			
		struct
		{
#line 61 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			CEnumType* pType;
#line 1702 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_enum_member ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_enum_member;
			
		}
	};
		
	
	
	
	class CSymbolNode_inheritance_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 39 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CStructClassType* pType;
#line 1724 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_inheritance_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_inheritance_list;
			
		}
	};
		
	
	class CSymbolNode_struct_block: public CSymbolNode
	{
	public:
			
		struct
		{
#line 67 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CStructType* pType;
#line 1744 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_struct_block ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_struct_block;
			
		}
	};
		
	
	
	class CSymbolNode_struct_member_declaration: public CSymbolNode
	{
	public:
			
		struct
		{
#line 79 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CStructType* pType;
#line 1765 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_struct_member_declaration ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_struct_member_declaration;
			
		}
	};
		
	
	class CSymbolNode_struct_member_declarator_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 88 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CTypeSpecifierModifiers* pTypeSpecifier; CStructType* pType;
#line 1785 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_struct_member_declarator_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_struct_member_declarator_list;
			
		}
	};
		
	
	class CSymbolNode_struct_member_declarator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 92 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CTypeSpecifierModifiers* pTypeSpecifier; CStructType* pType;
#line 1805 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 94 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
	CStructMember* pMember;
;
#line 1814 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_struct_member_declarator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_struct_member_declarator;
			
		}
	};
		
	
	
	
	
	class CSymbolNode_class_block: public CSymbolNode
	{
	public:
			
		struct
		{
#line 50 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CClassType* pType;
#line 1837 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_class_block ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_class_block;
			
		}
	};
		
	
	class CSymbolNode_base_class: public CSymbolNode
	{
	public:
			
		struct
		{
#line 43 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CStructClassType* pType;
#line 1857 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_base_class ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_base_class;
			
		}
	};
		
	
	class CSymbolNode_class_member_declaration: public CSymbolNode
	{
	public:
			
		struct
		{
#line 62 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CClassType* pType;
#line 1877 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_class_member_declaration ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_class_member_declaration;
			
		}
	};
		
	
	class CSymbolNode_class_member_declarator_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 71 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CDeclSpecifiers* pDeclSpecifiers; CClassType* pType;
#line 1897 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_class_member_declarator_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_class_member_declarator_list;
			
		}
	};
		
	
	class CSymbolNode_class_member_declarator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 75 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CDeclSpecifiers* pDeclSpecifiers; CClassType* pType;
#line 1917 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 77 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
	CModuleItem* pMember;
;
#line 1926 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_class_member_declarator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_class_member_declarator;
			
		}
	};
		
	
	
	class CSymbolNode_property_accessor_declaration: public CSymbolNode
	{
	public:
			
		struct
		{
#line 28 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			CPropertyBlock* pBlock;
#line 1947 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_property_accessor_declaration ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_property_accessor_declaration;
			
		}
	};
		
	
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	
	enum EStage
	{
		EStage_Pass1, // global declarations (before linkage)
		EStage_Pass2, // statements and expressions (after linkage)
	};

	enum EEndianness
	{
		EEndianness_LittleEndian,
		EEndianness_BigEndian,
	};

	CModule* m_pModule;
	size_t m_StructPackFactor;
	size_t m_DefaultStructPackFactor;
	EEndianness m_Endianness;
	EStage m_Stage;

	rtl::CArrayT <CTypeSpecifier*> m_TypeSpecifierStack;

	CParser ();

	CType*
	FindType (const CQualifiedName& Name);

	void
	PushTypeSpecifier (CTypeSpecifier* pTypeSpecifier)
	{
		m_TypeSpecifierStack.Append (pTypeSpecifier);
	}

	void
	PopTypeSpecifier (CTypeSpecifier* pTypeSpecifier)
	{
		m_TypeSpecifierStack.Pop ();
	}

	bool
	IsTypeSpecified ();

	CModuleItem*
	Declare (
		CDeclSpecifiers* pDeclSpecifiers,
		CDeclarator* pDeclarator
		);

	CClassType*
	DeclareClassType (
		EType TypeKind,
		rtl::CString& Name
		);

	CStructType*
	DeclareStructType (
		EType TypeKind,
		rtl::CString& Name,
		size_t PackFactor
		);

	CEnumType*
	DeclareEnumType (
		EType TypeKind,
		rtl::CString& Name
		);

	CStructMember*
	DeclareStructMember (
		CStructType* pStructType,
		CTypeSpecifierModifiers* pTypeSpecifier,
		CDeclarator* pDeclarator
		);

	CModuleItem*
	DeclareClassMember (
		CClassType* pClassType,
		CDeclSpecifiers* pDeclSpecifiers,
		CDeclarator* pDeclarator
		);

	CFunctionFormalArg*
	DeclareFormalArg (
		CDeclFunctionSuffix* pArgSuffix,
		CTypeSpecifierModifiers* pTypeSpecifier,
		CDeclarator* pDeclarator,
		const CValue& DefaultValue
		);

	CFunction*
	DeclarePropertyAccessor (
		CPropertyBlock* pBlock,
		CTypeSpecifierModifiers* pTypeSpecifier,
		CDeclarator* pDeclarator
		);

	CProperty*
	CreateProperty (
		CFunction* pGetter,
		const CFunctionOverload& Setter
		);

	void
	SetSimplePropertyPos (
		CProperty* pProperty,
		const CToken::CPos& Pos
		);

	bool
	SetFunctionBody (
		CModuleItem* pItem,
		rtl::CBoxListT <CToken>* pBody
		);

	bool
	ReturnStmt (const CValue& Value);

	bool
	LookupIdentifier (
		const rtl::CString& Name,
		CValue* pValue
		);


	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	static
	size_t*
	GetParseTable ()
	{
		static
		size_t _ParseTable [] = 
		{
			0, -1, 304, -1, -1, -1, -1, 304, -1, 304, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 304, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, -1, -1, 304, 304, 304, 304, -1, 304, 304, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, 113, -1, 112, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 112, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 110, 111, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, -1, -1, 112, 112, 112, 112, -1, 112, 112, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 434, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 436, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 437, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 437, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, 437, -1, -1, 437, 437, 437, 437, -1, 437, 437, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 409, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 115, -1, -1, -1, -1, 115, -1, 112, -1, 115, -1, -1, -1, -1, -1, 115, 115, 115, 115, -1, -1, -1, 115, 115, -1, 913, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, -1, 115, -1, -1, 115, 115, 115, 115, 115, 115, -1, 111, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, -1, -1, 112, 112, 112, 112, -1, 112, 112, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, 158, -1, -1, -1, 159, -1, -1, -1, -1, -1, 159, 159, 159, 159, -1, -1, -1, 159, 159, -1, 159, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 160, -1, 161, -1, -1, 162, 163, 164, 165, 166, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 305, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 308, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, -1, -1, 308, 308, 308, 308, -1, 308, 308, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 452, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, 452, -1, -1, 452, 452, 452, 452, -1, 452, 452, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, -1, -1, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 504, -1, 505, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 310, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 312, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 313, 
			-1, 321, 321, 321, 321, 321, 321, 316, 321, 318, 321, 320, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 321, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 322, -1, -1, -1, -1, -1, 322, 322, 322, 322, -1, -1, -1, 322, 322, -1, 322, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 325, -1, -1, -1, -1, -1, 325, 325, 325, 325, -1, -1, -1, 325, 325, -1, 325, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 325, 325, 325, 325, 325, 325, 325, 325, 325, 325, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 323, -1, -1, -1, -1, -1, 323, 323, 323, 323, -1, -1, -1, 323, 323, -1, 323, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 323, 323, 323, 323, 323, 323, 323, 323, 323, 323, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 324, -1, -1, -1, -1, -1, 324, 324, 324, 324, -1, -1, -1, 324, 324, -1, 324, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 327, -1, -1, -1, -1, -1, 327, 327, 327, 327, -1, -1, -1, 327, 327, -1, 327, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 329, -1, -1, -1, -1, -1, 329, 329, 329, 329, -1, -1, -1, 329, 329, -1, 329, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 331, -1, -1, -1, -1, -1, 331, 331, 331, 331, -1, -1, -1, 331, 331, -1, 331, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 333, -1, -1, -1, -1, -1, 333, 333, 333, 333, -1, -1, -1, 333, 333, -1, 333, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 335, -1, -1, -1, -1, -1, 335, 335, 335, 335, -1, -1, -1, 335, 335, -1, 335, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 337, -1, -1, -1, -1, -1, 337, 337, 337, 337, -1, -1, -1, 337, 337, -1, 337, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 341, -1, -1, -1, -1, -1, 341, 341, 341, 341, -1, -1, -1, 341, 341, -1, 341, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 341, 341, 341, 341, 341, 341, 341, 341, 341, 341, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 339, 340, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 347, -1, -1, -1, -1, -1, 347, 347, 347, 347, -1, -1, -1, 347, 347, -1, 347, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 343, -1, 344, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 345, 346, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 351, -1, -1, -1, -1, -1, 351, 351, 351, 351, -1, -1, -1, 351, 351, -1, 351, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 349, 350, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 355, -1, -1, -1, -1, -1, 355, 355, 355, 355, -1, -1, -1, 355, 355, -1, 355, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 355, 355, 355, 355, 355, 355, 355, 355, 355, 355, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 353, 354, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 360, -1, -1, -1, -1, -1, 360, 360, 360, 360, -1, -1, -1, 360, 360, -1, 360, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 357, 358, 359, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 914, -1, -1, -1, -1, -1, 374, 376, 377, 375, -1, -1, -1, 378, 379, -1, 373, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 380, 381, 373, 373, 373, 373, 373, 373, 373, 373, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 362, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 384, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 384, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 384, 384, 384, 384, 384, 384, 384, 384, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 383, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 458, 458, 458, 458, 458, 458, 458, 458, 915, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, -1, -1, 457, 457, 457, 457, -1, 457, 457, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 405, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 396, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 397, 398, 399, 400, 401, 402, 403, 404, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 386, -1, -1, -1, -1, -1, 387, -1, 388, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 389, 390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 391, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 392, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 917, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 919, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 921, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 408, 408, 408, 408, 408, 408, 408, 408, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, -1, -1, 406, 406, 406, 406, -1, 406, 406, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 410, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 413, 413, 413, 413, 413, 413, 412, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 
			-1, -1, -1, -1, -1, -1, -1, 414, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 416, -1, -1, -1, -1, -1, 416, 416, 416, 416, -1, -1, -1, 416, 416, -1, 416, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 416, 416, 416, 416, 416, 416, 416, 416, 416, 416, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 417, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 419, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 423, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 424, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 425, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 426, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 427, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 428, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 115, -1, -1, -1, -1, 115, -1, -1, -1, 115, -1, -1, -1, -1, -1, 115, 115, 115, 115, -1, -1, -1, 115, 115, -1, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, -1, 115, 421, 422, 115, 115, 115, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 429, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 431, -1, -1, -1, -1, 431, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 431, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 433, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 450, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, 450, -1, -1, 450, 450, 450, 450, -1, 450, 450, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 439, -1, -1, 439, -1, -1, -1, -1, -1, -1, 439, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, 449, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 441, -1, -1, 441, -1, -1, -1, -1, -1, -1, 441, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 497, -1, -1, 497, -1, -1, -1, -1, -1, -1, 497, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 265, 265, 265, 265, 265, 265, 924, 265, 265, 265, 922, 265, 265, 265, 265, 265, 933, 934, 935, 936, 265, 265, 265, 937, 938, 265, 939, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 940, 941, 942, 943, 944, 945, 946, 947, 948, 949, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 265, 
			-1, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 446, 
			-1, 448, 448, 448, 448, 950, 448, 448, 951, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 448, 
			-1, -1, 2, -1, -1, -1, -1, 113, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 454, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 455, 455, 456, 456, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, 454, -1, -1, 454, 454, 454, 454, -1, 454, 454, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 459, 460, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 461, 462, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 270, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, -1, -1, 487, 487, 488, 488, -1, 489, 489, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 463, 464, 465, 466, 467, 468, 469, 470, 471, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 518, 518, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 526, 526, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 543, 543, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 560, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 494, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 566, 567, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 925, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 927, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 510, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, -1, 510, 510, 510, 510, -1, 510, 510, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 512, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 514, -1, 512, 512, 512, 512, -1, 512, 512, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 515, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 516, 517, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 519, 520, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 522, -1, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 525, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 529, 530, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 531, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 534, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 533, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, 156, -1, 536, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 536, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, -1, -1, 536, 536, 536, 536, -1, 536, 536, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 537, -1, -1, 537, -1, -1, -1, -1, -1, -1, 537, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 540, -1, -1, 540, -1, -1, -1, -1, -1, -1, 540, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 128, -1, -1, -1, -1, -1, 128, 128, 128, 128, -1, -1, -1, 128, 128, -1, 128, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 542, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, 547, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 551, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 550, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, -1, -1, 550, 550, 550, 550, -1, 550, 550, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, 113, -1, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, -1, -1, 553, 553, 553, 553, -1, 553, 553, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, -1, -1, 555, -1, -1, -1, -1, -1, -1, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 558, -1, -1, 558, -1, -1, -1, -1, -1, -1, 558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, -1, -1, 563, 563, 563, 563, -1, 563, 563, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 565, -1, -1, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 565, 565, 
			0, -1, 304, -1, -1, -1, -1, 304, -1, 304, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 304, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, 304, -1, -1, 304, 304, 304, 304, -1, 304, 304, -1, -1, 
			-1, -1, 0, 306, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 
			-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 307, 0, 0, 307, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, 0, 0, 0, 0, 0, 0, 309, 0, 309, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 311, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 314, 0, 314, 314, 0, 314, 314, 0, 314, 0, 314, 0, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 314, 
			-1, 315, 315, 315, 315, 315, 315, 315, 0, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 
			-1, 317, 317, 317, 317, 317, 317, 317, 317, 317, 0, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 317, 
			-1, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 0, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 319, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 326, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 328, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 330, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 332, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 334, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 336, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 338, 338, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 342, 0, 342, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 342, 342, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 348, 348, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 352, 352, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 356, 356, 356, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 361, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 382, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, 385, 0, 0, 0, 0, 0, 385, 0, 385, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 385, 385, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 394, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 411, 411, 411, 411, 411, 411, 411, 0, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 
			-1, -1, 415, -1, -1, -1, -1, 415, 0, 415, -1, 415, -1, -1, -1, -1, -1, 415, 415, 415, 415, -1, -1, -1, 415, 415, -1, 415, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, -1, 415, -1, -1, 415, 415, 415, 415, 415, 415, -1, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, -1, -1, 415, 415, 415, 415, -1, 415, 415, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, 0, 0, 0, -1, 0, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 418, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, -1, -1, 
			-1, -1, 420, -1, -1, -1, -1, 420, 0, -1, -1, 420, -1, -1, -1, -1, -1, 420, 420, 420, 420, -1, -1, -1, 420, 420, -1, 420, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 420, 420, 420, 420, 420, 420, 420, 420, 420, 420, 420, -1, 420, 420, 420, 420, 420, 420, 420, 420, 420, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 125, -1, -1, -1, -1, -1, 125, 125, 125, 125, -1, -1, -1, 125, 125, -1, 125, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 125, -1, -1, -1, -1, -1, 125, 125, 125, 125, -1, -1, -1, 125, 125, -1, 125, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 125, 0, -1, -1, -1, -1, 125, 125, 125, 125, -1, -1, -1, 125, 125, -1, 125, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 128, -1, -1, -1, -1, -1, 128, 128, 128, 128, -1, -1, -1, 128, 128, -1, 128, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 128, -1, -1, -1, -1, -1, 128, 128, 128, 128, -1, -1, -1, 128, 128, -1, 128, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 866, -1, -1, -1, -1, -1, 866, 866, 866, 866, -1, -1, -1, 866, 866, -1, 866, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 866, 866, 866, 866, 866, 866, 866, 866, 866, 866, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 430, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 432, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 435, -1, -1, -1, -1, 435, 0, 435, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 435, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, 435, -1, -1, 435, 435, 435, 435, -1, 435, 435, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 169, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 438, -1, -1, 438, -1, -1, -1, -1, -1, -1, 438, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 440, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 442, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 
			-1, -1, -1, -1, -1, 447, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, 929, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, -1, -1, 451, 451, 451, 451, -1, 451, 451, -1, -1, 
			-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 930, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, -1, -1, 453, 453, 453, 453, -1, 453, 453, 0, 0, 
			-1, -1, 0, 0, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 495, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 496, -1, -1, 496, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, 498, -1, 0, -1, 0, -1, 0, -1, 0, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, -1, 499, -1, 499, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, 501, 501, 501, 501, 501, 501, 501, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 
			-1, -1, -1, -1, -1, 511, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 513, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 77, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 893, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 523, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 897, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 898, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 527, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 528, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 535, -1, -1, -1, -1, 535, 0, 535, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 535, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, -1, -1, 535, 535, 535, 535, -1, 535, 535, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 169, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, 538, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 539, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 541, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 905, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 544, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 549, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 552, -1, -1, -1, -1, 552, 0, 552, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 552, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, -1, -1, 552, 552, 552, 552, -1, 552, 552, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 169, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, -1, -1, 554, -1, -1, -1, -1, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 556, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, 557, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 559, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 561, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, -1, -1, 561, 561, 561, 561, -1, 561, 561, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 564, -1, -1, 564, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 931, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			
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
			224, 109,  -1,
			225, 568, 842,  -1,
			225, 569, 843, 3,  -1,
			226, 733, 119,  -1,
			570, 227, 226, 844,  -1,
			227, 734, 120,  -1,
			6, 228, 735, 122, 4,  -1,
			228, 736, 122, 5,  -1,
			571, 845,  -1,
			229, 737, 124,  -1,
			229, 737, 124,  -1,
			230, 738, 124,  -1,
			8, 230, 738, 124, 7,  -1,
			231, 739, 124,  -1,
			10, 231, 739, 124, 9,  -1,
			232, 740, 124,  -1,
			12, 232, 11,  -1,
			572, 846,  -1,
			741, 126,  -1,
			573, 742, 126,  -1,
			574, 125,  -1,
			233, 743, 129,  -1,
			575, 745, 126, 14, 744, 126, 13,  -1,
			234, 746, 130,  -1,
			234, 576, 747, 130, 28,  -1,
			235, 748, 131,  -1,
			235, 577, 749, 131, 29,  -1,
			236, 750, 132,  -1,
			236, 578, 751, 132, 15,  -1,
			237, 752, 133,  -1,
			237, 579, 753, 133, 16,  -1,
			238, 754, 134,  -1,
			238, 580, 755, 134, 17,  -1,
			239, 756, 135,  -1,
			239, 581, 757, 135, 847,  -1,
			582, 30,  -1,
			583, 31,  -1,
			240, 758, 137,  -1,
			240, 584, 759, 137, 848,  -1,
			585, 4,  -1,
			586, 6,  -1,
			587, 32,  -1,
			588, 33,  -1,
			241, 760, 139,  -1,
			241, 589, 761, 139, 849,  -1,
			590, 34,  -1,
			591, 35,  -1,
			242, 762, 141,  -1,
			242, 592, 763, 141, 850,  -1,
			593, 18,  -1,
			594, 19,  -1,
			243, 764, 143,  -1,
			243, 595, 765, 143, 851,  -1,
			596, 20,  -1,
			597, 21,  -1,
			598, 22,  -1,
			244, 766, 145,  -1,
			599, 767, 126, 852,  -1,
			600, 23,  -1,
			601, 36,  -1,
			602, 37,  -1,
			603, 38,  -1,
			604, 39,  -1,
			605, 40,  -1,
			606, 41,  -1,
			607, 42,  -1,
			608, 43,  -1,
			609, 44,  -1,
			610, 45,  -1,
			768, 147,  -1,
			611, 769, 145, 17,  -1,
			612, 770, 145, 20,  -1,
			771, 145, 18,  -1,
			613, 772, 145, 19,  -1,
			614, 773, 145, 24,  -1,
			615, 774, 145, 25,  -1,
			616, 775, 145, 46,  -1,
			617, 776, 145, 47,  -1,
			618, 777, 145, 12, 853, 11,  -1,
			778, 149, 11,  -1,
			246, 779, 150,  -1,
			246, 780, 151,  -1,
			781, 152,  -1,
			782, 153,  -1,
			783, 154,  -1,
			619, 46,  -1,
			620, 47,  -1,
			621, 854, 3,  -1,
			622, 10, 125, 9,  -1,
			12, 247, 623, 784, 126, 11,  -1,
			247, 624, 785, 126, 5,  -1,
			12, 11,  -1,
			625, 855,  -1,
			626, 856,  -1,
			627, 857,  -1,
			628, 858,  -1,
			629, 51,  -1,
			630, 52,  -1,
			631, 53,  -1,
			632, 786, 155, 54,  -1,
			633, 787, 155, 55,  -1,
			12, 788, 126, 11,  -1,
			634, 859,  -1,
			635, 12, 860, 11,  -1,
			789, 150,  -1,
			790, 156,  -1,
			637, 862, 248, 636, 861,  -1,
			248, 791, 157,  -1,
			792, 156,  -1,
			638, 863,  -1,
			640, 865, 249, 639, 864,  -1,
			249, 114,  -1,
			2, 125,  -1,
			250, 115, 12, 125, 11, 56,  -1,
			115, 57,  -1,
			8, 251, 7, 12, 125, 11, 58,  -1,
			251, 168,  -1,
			14, 128, 59,  -1,
			14, 60,  -1,
			115, 12, 125, 11, 61,  -1,
			2, 12, 125, 11, 61, 115, 62,  -1,
			115, 12, 254, 2, 253, 2, 252, 11, 63,  -1,
			2, 255, 64,  -1,
			2, 256, 65,  -1,
			641, 2, 257, 66,  -1,
			10, 170, 9,  -1,
			258, 171, 5,  -1,
			258, 171,  -1,
			123, 23,  -1,
			642, 259, 867,  -1,
			644, 8, 260, 7, 643, 869, 868,  -1,
			260, 109,  -1,
			2, 116, 68,  -1,
			794, 174, 262, 870, 261,  -1,
			793, 871,  -1,
			263, 645, 795, 872,  -1,
			263, 646, 796, 873, 5,  -1,
			264, 647, 874,  -1,
			649, 797, 875, 648, 23,  -1,
			651, 876,  -1,
			652, 877,  -1,
			653, 8, 798, 178, 7,  -1,
			266, 799, 179,  -1,
			266, 800, 179, 5,  -1,
			801, 177,  -1,
			654, 878,  -1,
			267, 802, 181,  -1,
			267, 802, 181,  -1,
			268, 803, 149,  -1,
			268, 803, 149,  -1,
			804, 149,  -1,
			805, 182,  -1,
			806, 183,  -1,
			807, 184,  -1,
			808, 185,  -1,
			655, 69,  -1,
			656, 70,  -1,
			657, 71,  -1,
			658, 72,  -1,
			659, 73,  -1,
			660, 74,  -1,
			661, 75,  -1,
			662, 76,  -1,
			663, 77,  -1,
			664, 78,  -1,
			665, 79,  -1,
			666, 80,  -1,
			667, 81,  -1,
			668, 82,  -1,
			669, 83,  -1,
			670, 84,  -1,
			671, 85,  -1,
			672, 86,  -1,
			673, 87,  -1,
			674, 88,  -1,
			675, 89,  -1,
			676, 90,  -1,
			677, 91,  -1,
			678, 92,  -1,
			679, 93,  -1,
			680, 94,  -1,
			681, 95,  -1,
			682, 96,  -1,
			683, 879,  -1,
			684, 880,  -1,
			685, 881,  -1,
			686, 882,  -1,
			687, 269, 883,  -1,
			689, 884, 688,  -1,
			690, 885,  -1,
			6, 271, 117, 4,  -1,
			271, 117, 5,  -1,
			272, 809, 119,  -1,
			274, 273, 691, 886, 272,  -1,
			692, 887, 3,  -1,
			274, 810, 120,  -1,
			275, 693, 888,  -1,
			275, 811, 185,  -1,
			694, 20,  -1,
			695, 17,  -1,
			812, 195,  -1,
			813, 196,  -1,
			696, 10, 889, 9,  -1,
			697, 10, 9,  -1,
			12, 814, 197, 11,  -1,
			12, 11,  -1,
			276, 815, 198,  -1,
			276, 816, 198, 5,  -1,
			698, 277, 891, 890,  -1,
			123, 23,  -1,
			699, 97,  -1,
			200, 14, 98, 26,  -1,
			700, 278,  -1,
			701, 78,  -1,
			817, 202, 702, 279, 892,  -1,
			703, 99,  -1,
			704, 100,  -1,
			706, 8, 818, 203, 705, 7,  -1,
			280, 819, 204,  -1,
			280, 820, 204, 5,  -1,
			895, 23,  -1,
			707, 281, 894,  -1,
			823, 208, 285, 284, 708, 283, 282, 896,  -1,
			821, 121,  -1,
			822, 207,  -1,
			709, 101,  -1,
			710, 102,  -1,
			711, 12, 899, 11, 103,  -1,
			712, 900,  -1,
			713, 60,  -1,
			715, 8, 286, 714, 7,  -1,
			286, 824, 210,  -1,
			2, 825, 211, 901, 287,  -1,
			288, 826, 212,  -1,
			288, 827, 212, 5,  -1,
			213, 14,  -1,
			290, 716, 289, 902,  -1,
			717, 828, 177, 23,  -1,
			718, 903, 14, 103, 26,  -1,
			831, 216, 293, 292, 719, 291, 904,  -1,
			829, 121,  -1,
			830, 207,  -1,
			720, 104,  -1,
			721, 105,  -1,
			294, 832, 217, 14,  -1,
			294, 833, 217, 5,  -1,
			722, 906,  -1,
			724, 8, 295, 723, 7,  -1,
			295, 834, 218,  -1,
			180, 297, 907, 296,  -1,
			835, 219,  -1,
			298, 836, 220,  -1,
			298, 837, 220, 5,  -1,
			213, 14,  -1,
			300, 725, 299, 908,  -1,
			727, 838, 177, 726, 23,  -1,
			728, 909, 81,  -1,
			301, 839, 222,  -1,
			8, 301, 839, 222, 7,  -1,
			729, 180, 911, 910,  -1,
			302, 840, 119,  -1,
			730, 841, 196, 912, 302,  -1,
			731, 106,  -1,
			732, 107,  -1,
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 7, 12, 16, 21, 25, 31, 36, 39, 43, 47, 51, 57, 61, 67, 71, 75, 78, 81, 85, 88, 92, 100, 104, 110, 114, 120, 124, 130, 134, 140, 144, 150, 154, 160, 163, 166, 170, 176, 179, 182, 185, 188, 192, 198, 201, 204, 208, 214, 217, 220, 224, 230, 233, 236, 239, 243, 248, 251, 254, 257, 260, 263, 266, 269, 272, 275, 278, 281, 284, 289, 294, 298, 303, 308, 313, 318, 323, 330, 334, 338, 342, 345, 348, 351, 354, 357, 361, 366, 373, 379, 382, 385, 388, 391, 394, 397, 400, 403, 408, 413, 418, 421, 426, 429, 432, 438, 442, 445, 448, 454, 457, 460, 467, 470, 478, 481, 485, 488, 494, 502, 512, 516, 520, 525, 529, 533, 536, 539, 543, 551, 554, 558, 564, 567, 572, 578, 582, 588, 591, 594, 600, 604, 609, 612, 615, 619, 623, 627, 631, 634, 637, 640, 643, 646, 649, 652, 655, 658, 661, 664, 667, 670, 673, 676, 679, 682, 685, 688, 691, 694, 697, 700, 703, 706, 709, 712, 715, 718, 721, 724, 727, 730, 733, 736, 739, 742, 746, 750, 753, 758, 762, 766, 772, 776, 780, 784, 788, 791, 794, 797, 800, 805, 809, 814, 817, 821, 826, 831, 834, 837, 842, 845, 848, 854, 857, 860, 867, 871, 876, 879, 883, 892, 895, 898, 901, 904, 910, 913, 916, 922, 926, 932, 936, 941, 944, 949, 954, 960, 968, 971, 974, 977, 980, 985, 990, 993, 999, 1003, 1008, 1011, 1015, 1020, 1023, 1028, 1034, 1038, 1042, 1048, 1053, 1057, 1063, 1066, 
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
		
		case '<':
			return 4;
		
		case ',':
			return 5;
		
		case '>':
			return 6;
		
		case '{':
			return 7;
		
		case '}':
			return 8;
		
		case '[':
			return 9;
		
		case ']':
			return 10;
		
		case '(':
			return 11;
		
		case ')':
			return 12;
		
		case '?':
			return 13;
		
		case ':':
			return 14;
		
		case '|':
			return 15;
		
		case '^':
			return 16;
		
		case '&':
			return 17;
		
		case '+':
			return 18;
		
		case '-':
			return 19;
		
		case '*':
			return 20;
		
		case '/':
			return 21;
		
		case '%':
			return 22;
		
		case '=':
			return 23;
		
		case '~':
			return 24;
		
		case '!':
			return 25;
		
		case '#':
			return 26;
		
		case EToken_Identifier:
			return 27;
		
		case EToken_LogicalOr:
			return 28;
		
		case EToken_LogicalAnd:
			return 29;
		
		case EToken_Eq:
			return 30;
		
		case EToken_Ne:
			return 31;
		
		case EToken_Le:
			return 32;
		
		case EToken_Ge:
			return 33;
		
		case EToken_Shl:
			return 34;
		
		case EToken_Shr:
			return 35;
		
		case EToken_AddAssign:
			return 36;
		
		case EToken_SubAssign:
			return 37;
		
		case EToken_MulAssign:
			return 38;
		
		case EToken_DivAssign:
			return 39;
		
		case EToken_ModAssign:
			return 40;
		
		case EToken_ShlAssign:
			return 41;
		
		case EToken_ShrAssign:
			return 42;
		
		case EToken_AndAssign:
			return 43;
		
		case EToken_XorAssign:
			return 44;
		
		case EToken_OrAssign:
			return 45;
		
		case EToken_Inc:
			return 46;
		
		case EToken_Dec:
			return 47;
		
		case EToken_Integer:
			return 48;
		
		case EToken_Fp:
			return 49;
		
		case EToken_Literal:
			return 50;
		
		case EToken_True:
			return 51;
		
		case EToken_False:
			return 52;
		
		case EToken_Null:
			return 53;
		
		case EToken_SizeOf:
			return 54;
		
		case EToken_TypeOf:
			return 55;
		
		case EToken_If:
			return 56;
		
		case EToken_Else:
			return 57;
		
		case EToken_Switch:
			return 58;
		
		case EToken_Case:
			return 59;
		
		case EToken_Default:
			return 60;
		
		case EToken_While:
			return 61;
		
		case EToken_Do:
			return 62;
		
		case EToken_For:
			return 63;
		
		case EToken_Break:
			return 64;
		
		case EToken_Continue:
			return 65;
		
		case EToken_Return:
			return 66;
		
		case EToken_Namespace:
			return 67;
		
		case EToken_Using:
			return 68;
		
		case EToken_Public:
			return 69;
		
		case EToken_Private:
			return 70;
		
		case EToken_Static:
			return 71;
		
		case EToken_Typedef:
			return 72;
		
		case EToken_Const:
			return 73;
		
		case EToken_Volatile:
			return 74;
		
		case EToken_Signed:
			return 75;
		
		case EToken_Unsigned:
			return 76;
		
		case EToken_LittleEndian:
			return 77;
		
		case EToken_BigEndian:
			return 78;
		
		case EToken_Safe:
			return 79;
		
		case EToken_Unsafe:
			return 80;
		
		case EToken_Property:
			return 81;
		
		case EToken_Void:
			return 82;
		
		case EToken_Bool:
			return 83;
		
		case EToken_Int:
			return 84;
		
		case EToken_Char:
			return 85;
		
		case EToken_Short:
			return 86;
		
		case EToken_Long:
			return 87;
		
		case EToken_Float:
			return 88;
		
		case EToken_Double:
			return 89;
		
		case EToken_Int8:
			return 90;
		
		case EToken_Int16:
			return 91;
		
		case EToken_Int32:
			return 92;
		
		case EToken_Int64:
			return 93;
		
		case EToken_WChar:
			return 94;
		
		case EToken_Variant:
			return 95;
		
		case EToken_Auto:
			return 96;
		
		case EToken_Ellipsis:
			return 97;
		
		case EToken_Endian:
			return 98;
		
		case EToken_Enum:
			return 99;
		
		case EToken_EnumC:
			return 100;
		
		case EToken_Struct:
			return 101;
		
		case EToken_Union:
			return 102;
		
		case EToken_Pack:
			return 103;
		
		case EToken_Interface:
			return 104;
		
		case EToken_Class:
			return 105;
		
		case EToken_Get:
			return 106;
		
		case EToken_Set:
			return 107;
		
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
			'<', 
			',', 
			'>', 
			'{', 
			'}', 
			'[', 
			']', 
			'(', 
			')', 
			'?', 
			':', 
			'|', 
			'^', 
			'&', 
			'+', 
			'-', 
			'*', 
			'/', 
			'%', 
			'=', 
			'~', 
			'!', 
			'#', 
			EToken_Identifier, 
			EToken_LogicalOr, 
			EToken_LogicalAnd, 
			EToken_Eq, 
			EToken_Ne, 
			EToken_Le, 
			EToken_Ge, 
			EToken_Shl, 
			EToken_Shr, 
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
			EToken_Integer, 
			EToken_Fp, 
			EToken_Literal, 
			EToken_True, 
			EToken_False, 
			EToken_Null, 
			EToken_SizeOf, 
			EToken_TypeOf, 
			EToken_If, 
			EToken_Else, 
			EToken_Switch, 
			EToken_Case, 
			EToken_Default, 
			EToken_While, 
			EToken_Do, 
			EToken_For, 
			EToken_Break, 
			EToken_Continue, 
			EToken_Return, 
			EToken_Namespace, 
			EToken_Using, 
			EToken_Public, 
			EToken_Private, 
			EToken_Static, 
			EToken_Typedef, 
			EToken_Const, 
			EToken_Volatile, 
			EToken_Signed, 
			EToken_Unsigned, 
			EToken_LittleEndian, 
			EToken_BigEndian, 
			EToken_Safe, 
			EToken_Unsafe, 
			EToken_Property, 
			EToken_Void, 
			EToken_Bool, 
			EToken_Int, 
			EToken_Char, 
			EToken_Short, 
			EToken_Long, 
			EToken_Float, 
			EToken_Double, 
			EToken_Int8, 
			EToken_Int16, 
			EToken_Int32, 
			EToken_Int64, 
			EToken_WChar, 
			EToken_Variant, 
			EToken_Auto, 
			EToken_Ellipsis, 
			EToken_Endian, 
			EToken_Enum, 
			EToken_EnumC, 
			EToken_Struct, 
			EToken_Union, 
			EToken_Pack, 
			EToken_Interface, 
			EToken_Class, 
			EToken_Get, 
			EToken_Set, 
			
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
			_T("compilation_unit"),
			_T("global_declaration"),
			_T("namespace_declaration"),
			_T("using_declaration"),
			_T("common_declaration"),
			_T("function_body_pass1"),
			_T("local_declaration"),
			_T("statement"),
			_T("qualified_name"),
			_T("type_name"),
			_T("type_specifier_modifier_list"),
			_T("pointer"),
			_T("declarator_suffix"),
			_T("generic_formal_argument_list"),
			_T("generic_formal_argument"),
			_T("expression_pass1"),
			_T("primary_expr_pass1"),
			_T("expression"),
			_T("conditional_expr"),
			_T("constant_expr"),
			_T("constant_integer_expr"),
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
			_T("postfix_expr"),
			_T("cast_operator_rslv"),
			_T("type_specifier_modifier"),
			_T("primary_expr"),
			_T("postfix_operator"),
			_T("member_operator"),
			_T("index_operator"),
			_T("call_operator"),
			_T("type_name_or_expr"),
			_T("compound_stmt_pass1"),
			_T("statement_pass1"),
			_T("compound_stmt"),
			_T("expression_stmt"),
			_T("if_stmt"),
			_T("switch_stmt"),
			_T("while_stmt"),
			_T("do_stmt"),
			_T("for_stmt"),
			_T("break_stmt"),
			_T("continue_stmt"),
			_T("return_stmt"),
			_T("switch_block_stmt"),
			_T("attribute_block"),
			_T("attribute_declarator_list"),
			_T("attribute_declarator"),
			_T("declaration_specifier_list"),
			_T("init_declarator_list"),
			_T("declaration_terminator_2"),
			_T("init_declarator"),
			_T("declarator"),
			_T("initializer"),
			_T("initializer_list"),
			_T("initializer_list_entry"),
			_T("declaration_terminator"),
			_T("declaration_specifier"),
			_T("access_specifier"),
			_T("storage_class_specifier"),
			_T("type_specifier"),
			_T("type_modifier"),
			_T("enum_specifier"),
			_T("struct_specifier"),
			_T("class_specifier"),
			_T("property_specifier"),
			_T("qualified_type_name_rslv"),
			_T("qualified_type_name"),
			_T("generic_actual_argument_list"),
			_T("property_accessor_kind"),
			_T("pointer_kind"),
			_T("array_suffix"),
			_T("function_suffix"),
			_T("function_formal_argument_list"),
			_T("function_formal_argument"),
			_T("pragma_endian"),
			_T("pragma_endian_value"),
			_T("enum_kind"),
			_T("enum_block"),
			_T("enum_member_list"),
			_T("enum_member"),
			_T("struct_kind"),
			_T("struct_pack_factor"),
			_T("inheritance_list"),
			_T("struct_block"),
			_T("struct_pack_factor_value"),
			_T("struct_member_declaration"),
			_T("struct_member_declarator_list"),
			_T("struct_member_declarator"),
			_T("struct_member_bit_field"),
			_T("pragma_pack"),
			_T("class_kind"),
			_T("class_block"),
			_T("base_class"),
			_T("class_member_declaration"),
			_T("class_member_declarator_list"),
			_T("class_member_declarator"),
			_T("property_block"),
			_T("property_accessor_declaration"),
			_T("property_declarator"),
			
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
		
		
		
		
		
		
		case ESymbol_function_body_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls13);
				
			break;
			
		
		
		case ESymbol_statement:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			break;
			
		
		case ESymbol_qualified_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls1);
				
			break;
			
		
		case ESymbol_type_name:
			pNode = AXL_MEM_NEW (CSymbolNode_type_name);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls2);
			
			break;
			
		
		case ESymbol_type_specifier_modifier_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls19);
				
			break;
			
		
		case ESymbol_pointer:
			pNode = AXL_MEM_NEW (CSymbolNode_pointer);
			
			break;
			
		
		case ESymbol_declarator_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_suffix);
			
			break;
			
		
		case ESymbol_generic_formal_argument_list:
			pNode = AXL_MEM_NEW (CSymbolNode_generic_formal_argument_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_generic_formal_argument:
			pNode = AXL_MEM_NEW (CSymbolNode_generic_formal_argument);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_expression_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls3);
				
			break;
			
		
		case ESymbol_primary_expr_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_primary_expr_pass1);
			
			break;
			
		
		case ESymbol_expression:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls4);
				
			break;
			
		
		case ESymbol_conditional_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_conditional_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_constant_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls5);
				
			break;
			
		
		case ESymbol_constant_integer_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls6);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls7);
				
			break;
			
		
		case ESymbol_shift_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_shift_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_relational_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls8);
				
			break;
			
		
		case ESymbol_additive_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_additive_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_shift_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls9);
				
			break;
			
		
		case ESymbol_multiplicative_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_multiplicative_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_additivie_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls10);
				
			break;
			
		
		case ESymbol_assignment_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_assignment_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_multiplicative_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls11);
				
			break;
			
		
		case ESymbol_unary_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_unary_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_assignment_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls12);
				
			break;
			
		
		case ESymbol_postfix_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_postfix_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_cast_operator_rslv:
			pNode = AXL_MEM_NEW (CSymbolNode_cast_operator_rslv);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_type_specifier_modifier:
			pNode = AXL_MEM_NEW (CSymbolNode_type_specifier_modifier);
			
			break;
			
		
		case ESymbol_primary_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_primary_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_postfix_operator:
			pNode = AXL_MEM_NEW (CSymbolNode_postfix_operator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_member_operator:
			pNode = AXL_MEM_NEW (CSymbolNode_member_operator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_index_operator:
			pNode = AXL_MEM_NEW (CSymbolNode_index_operator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_call_operator:
			pNode = AXL_MEM_NEW (CSymbolNode_call_operator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_type_name_or_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_type_name_or_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_compound_stmt_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_compound_stmt_pass1);
			
			break;
			
		
		case ESymbol_statement_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_statement_pass1);
			
			break;
			
		
		case ESymbol_compound_stmt:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (CCompoundStmt);
				
			break;
			
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_attribute_block:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
				
			break;
			
		
		
		
		case ESymbol_declaration_specifier_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls18);
				
			break;
			
		
		case ESymbol_init_declarator_list:
			pNode = AXL_MEM_NEW (CSymbolNode_init_declarator_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls15);
			
			break;
			
		
		case ESymbol_declaration_terminator_2:
			pNode = AXL_MEM_NEW (CSymbolNode_declaration_terminator_2);
			
			break;
			
		
		case ESymbol_init_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_init_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls16);
			
			break;
			
		
		case ESymbol_declarator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls21);
				
			break;
			
		
		case ESymbol_initializer:
			pNode = AXL_MEM_NEW (CSymbolNode_initializer);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls17);
			
			break;
			
		
		case ESymbol_initializer_list:
			pNode = AXL_MEM_NEW (CSymbolNode_initializer_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_initializer_list_entry:
			pNode = AXL_MEM_NEW (CSymbolNode_initializer_list_entry);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declaration_terminator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			break;
			
		
		case ESymbol_declaration_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_declaration_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_access_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_access_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_storage_class_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_storage_class_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_type_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_type_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_type_modifier:
			pNode = AXL_MEM_NEW (CSymbolNode_type_modifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_enum_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls23);
				
			break;
			
		
		case ESymbol_struct_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls25);
				
			break;
			
		
		case ESymbol_class_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls29);
				
			break;
			
		
		case ESymbol_property_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls31);
				
			break;
			
		
		case ESymbol_qualified_type_name_rslv:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			break;
			
		
		case ESymbol_qualified_type_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls20);
				
			break;
			
		
		
		case ESymbol_property_accessor_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls34);
				
			break;
			
		
		case ESymbol_pointer_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls22);
				
			break;
			
		
		case ESymbol_array_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_array_suffix);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_function_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_function_suffix);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_function_formal_argument_list:
			pNode = AXL_MEM_NEW (CSymbolNode_function_formal_argument_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_function_formal_argument:
			pNode = AXL_MEM_NEW (CSymbolNode_function_formal_argument);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		
		case ESymbol_enum_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls24);
				
			break;
			
		
		case ESymbol_enum_block:
			pNode = AXL_MEM_NEW (CSymbolNode_enum_block);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_enum_member_list:
			pNode = AXL_MEM_NEW (CSymbolNode_enum_member_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_enum_member:
			pNode = AXL_MEM_NEW (CSymbolNode_enum_member);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_struct_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls26);
				
			break;
			
		
		case ESymbol_struct_pack_factor:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls27);
				
			break;
			
		
		case ESymbol_inheritance_list:
			pNode = AXL_MEM_NEW (CSymbolNode_inheritance_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_struct_block:
			pNode = AXL_MEM_NEW (CSymbolNode_struct_block);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_struct_pack_factor_value:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls28);
				
			break;
			
		
		case ESymbol_struct_member_declaration:
			pNode = AXL_MEM_NEW (CSymbolNode_struct_member_declaration);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_struct_member_declarator_list:
			pNode = AXL_MEM_NEW (CSymbolNode_struct_member_declarator_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_struct_member_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_struct_member_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		
		case ESymbol_class_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls30);
				
			break;
			
		
		case ESymbol_class_block:
			pNode = AXL_MEM_NEW (CSymbolNode_class_block);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_base_class:
			pNode = AXL_MEM_NEW (CSymbolNode_base_class);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_class_member_declaration:
			pNode = AXL_MEM_NEW (CSymbolNode_class_member_declaration);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_class_member_declarator_list:
			pNode = AXL_MEM_NEW (CSymbolNode_class_member_declarator_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_class_member_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_class_member_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_property_block:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (CPropertyBlock);
				
			break;
			
		
		case ESymbol_property_accessor_declaration:
			pNode = AXL_MEM_NEW (CSymbolNode_property_accessor_declaration);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_property_declarator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls33);
				
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
			
			{ 0, 27 },
			
			{ 1, 27 },
			
			{ 0, 118 },
			
			{ 0, 27 },
			
			{ 0, 1 },
			
			{ 0, 136 },
			
			{ 0, 138 },
			
			{ 0, 140 },
			
			{ 0, 142 },
			
			{ 0, 144 },
			
			{ 0, 146 },
			
			{ 0, 117 },
			
			{ 0, 27 },
			
			{ 0, 27 },
			
			{ 0, 48 },
			
			{ 0, 49 },
			
			{ 0, 50 },
			
			{ 0, 117 },
			
			{ 0, 117 },
			
			{ 0, 7 },
			
			{ 1, 8 },
			
			{ 0, 1 },
			
			{ 0, 7 },
			
			{ 1, 8 },
			
			{ 0, 125 },
			
			{ 0, 27 },
			
			{ 0, 67 },
			
			{ 1, 116 },
			
			{ 0, 172 },
			
			{ 1, 173 },
			
			{ 0, 175 },
			
			{ 1, 175 },
			
			{ 0, 176 },
			
			{ 1, 177 },
			
			{ 0, 123 },
			
			{ 0, 125 },
			
			{ 0, 113 },
			
			{ 0, 186 },
			
			{ 0, 187 },
			
			{ 0, 188 },
			
			{ 0, 189 },
			
			{ 0, 191 },
			
			{ 0, 116 },
			
			{ 0, 116 },
			
			{ 0, 27 },
			
			{ 1, 193 },
			
			{ 0, 194 },
			
			{ 0, 48 },
			
			{ 0, 118 },
			
			{ 1, 176 },
			
			{ 0, 201 },
			
			{ 1, 27 },
			
			{ 0, 27 },
			
			{ 1, 123 },
			
			{ 0, 205 },
			
			{ 2, 206 },
			
			{ 1, 27 },
			
			{ 0, 209 },
			
			{ 0, 48 },
			
			{ 0, 118 },
			
			{ 0, 176 },
			
			{ 0, 209 },
			
			{ 0, 215 },
			
			{ 1, 27 },
			
			{ 0, 117 },
			
			{ 0, 172 },
			
			{ 0, 176 },
			
			{ 0, 221 },
			
			{ 0, 118 },
			
			{ 1, 223 },
			
			{ 0, 193 },
			
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
			_cls1* __pAstNode = (_cls1*) __pSymbol->m_pAstNode;
#line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			  
			(*__pAstNode).m_Name.m_First = (*GetTokenLocator (0)).m_Data.m_String; 
		;
#line 4154 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls1* __pAstNode = (_cls1*) __pSymbol->m_pAstNode;
#line 29 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			(*__pAstNode).m_Name.m_List.InsertTail ((*GetTokenLocator (1)).m_Data.m_String);
		;
#line 4167 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
#line 49 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_pType = __pSymbol->m_Local.Declarator.GetType (&(*(_cls19*) GetAstLocator (0)).m_TypeSpecifier);
		;
#line 4180 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode_generic_formal_argument* __pSymbol = (CSymbolNode_generic_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 62 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
									
			CImportType* pArgumentType = m_pModule->m_TypeMgr.GetImportType ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pType);
			return __pSymbol->m_Arg.pType->AddGenericArgument (pArgumentType);
		;
#line 4194 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
#line 4207 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 5:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
#line 40 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			if ((*__pAstNode).m_Value.GetValueKind () != EValue_Const)
			{
				err::SetFormatStringError (_T("not a constant expression"));
				return false;
			}
		;
#line 4224 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 6:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
#line 56 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_Value = 10;
		;
#line 4237 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 7:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 72 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.ConditionalOperator (__pSymbol->m_Arg.pValue, __pSymbol->m_Local.TrueValue, __pSymbol->m_Local.FalseValue);
			;
#line 4250 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 8:
			{
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 86 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogicalAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4263 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 9:
			{
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 100 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogicalOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4276 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 10:
			{
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 114 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BitwiseOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4289 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 11:
			{
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 128 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BitwiseXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 12:
			{
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BitwiseAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4315 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 13:
			{
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 156 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls7*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4328 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 14:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
#line 168 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Eq;
		;
#line 4341 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 15:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
#line 172 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ne;
		;
#line 4354 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 16:
			{
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 185 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls8*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4367 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 17:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 197 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Lt;
		;
#line 4380 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 18:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 201 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Gt;
		;
#line 4393 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 19:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 205 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Le;
		;
#line 4406 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 20:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 209 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ge;
		;
#line 4419 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 21:
			{
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 222 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls9*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4432 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 22:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shl;
		;
#line 4445 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 23:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 238 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shr;
		;
#line 4458 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 24:
			{
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 251 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls10*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4471 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 25:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
#line 263 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Add;
		;
#line 4484 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 26:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
#line 267 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Sub;
		;
#line 4497 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 27:
			{
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 280 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls11*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4510 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 28:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
#line 292 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mul;
		;
#line 4523 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 29:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
#line 296 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Div;
		;
#line 4536 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 30:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
#line 300 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mod;
		;
#line 4549 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 31:
			{
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 313 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.MoveOperator (__pSymbol->m_Local.RValue, *__pSymbol->m_Arg.pValue, (*(_cls12*) GetAstLocator (0)).m_OpKind);
			;
#line 4562 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 32:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 325 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_None;
		;
#line 4575 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 33:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 329 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Add;
		;
#line 4588 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 34:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 333 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Sub;
		;
#line 4601 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 35:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 337 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mul;
		;
#line 4614 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 36:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 341 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Div;
		;
#line 4627 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 37:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 345 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mod;
		;
#line 4640 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 38:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 349 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shl;
		;
#line 4653 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 39:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 353 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shr;
		;
#line 4666 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 40:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 357 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_BitwiseAnd;
		;
#line 4679 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 41:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 361 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_BitwiseXor;
		;
#line 4692 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 42:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 365 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_BitwiseOr;
		;
#line 4705 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 43:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 373 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, __pSymbol->m_Arg.pValue);
		;
#line 4718 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 44:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 377 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, __pSymbol->m_Arg.pValue);
		;
#line 4731 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 45:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 382 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Minus, __pSymbol->m_Arg.pValue);
		;
#line 4744 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 46:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 386 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_BitwiseNot, __pSymbol->m_Arg.pValue);
		;
#line 4757 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 47:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 390 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_LogicalNot, __pSymbol->m_Arg.pValue);
		;
#line 4770 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 48:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 394 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Inc, __pSymbol->m_Arg.pValue);
		;
#line 4783 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 49:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 398 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Dec, __pSymbol->m_Arg.pValue);
		;
#line 4796 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 50:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 403 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.CastOperator (__pSymbol->m_Arg.pValue, (*(_cls2*) GetAstLocator (0)).m_pType);
		;
#line 4809 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 51:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 429 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.PostfixIncOperator (__pSymbol->m_Arg.pValue);
		;
#line 4822 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 52:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 433 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.PostfixDecOperator (__pSymbol->m_Arg.pValue);
		;
#line 4835 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 53:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 440 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.MemberOperator (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
		;
#line 4848 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 54:
			{
			CSymbolNode_index_operator* __pSymbol = (CSymbolNode_index_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 451 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.IndexValue);
		;
#line 4861 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 55:
			{
			CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 468 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Local.ArgList.InsertTail (__pSymbol->m_Local.Arg);
		;
#line 4874 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 56:
			{
			CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 474 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Local.ArgList.InsertTail (__pSymbol->m_Local.Arg);
			;
#line 4887 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 57:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 489 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return LookupIdentifier ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
		;
#line 4900 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 58:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 493 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstUInt64 ((*GetTokenLocator (0)).m_Data.m_UInt64);
		;
#line 4913 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 59:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 497 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstDouble ((*GetTokenLocator (0)).m_Data.m_Double);
		;
#line 4926 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 60:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 501 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetLiteral ((*GetTokenLocator (0)).m_Data.m_String);
		;
#line 4939 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 61:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 505 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstBool (true);
		;
#line 4952 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 62:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 509 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstBool (false);
		;
#line 4965 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 63:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 513 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetNull ();
		;
#line 4978 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 64:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 517 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstUInt32 (__pSymbol->m_Local.OpValue.GetType ()->GetSize ());
		;
#line 4991 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 65:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 521 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetType (__pSymbol->m_Local.OpValue.GetType ());
		;
#line 5004 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 66:
			{
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 529 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetType ((*(_cls2*) GetAstLocator (0)).m_pType);
		;
#line 5017 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 67:
			{
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 533 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetType ((*(_cls2*) GetAstLocator (0)).m_pType);
		;
#line 5030 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 68:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 14 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
#line 5043 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 69:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
		;
#line 5056 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 70:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 28 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
#line 5069 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 71:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CCompoundStmt* __pAstNode = (CCompoundStmt*) __pSymbol->m_pAstNode;
#line 42 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			(*__pAstNode).m_pScope = m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
		;
#line 5082 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 72:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CCompoundStmt* __pAstNode = (CCompoundStmt*) __pSymbol->m_pAstNode;
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
		;
#line 5095 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 73:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 107 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return ReturnStmt (IsValidLocator ((*(_cls4*) GetAstLocator (0))) ? (*(_cls4*) GetAstLocator (0)).m_Value : CValue ());
		;
#line 5108 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 74:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_AttributeBlock.llk"
			
			CAttribute* pAttribute = m_pModule->m_AttributeMgr.CreateAttribute ((*GetTokenLocator (0)).m_Data.m_String, NULL);
			if (!pAttribute)
				return false;

			pAttribute->m_Pos = (*GetTokenLocator (0)).m_Pos;
		;
#line 5125 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 75:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 5 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamespaceDeclaration.llk"
			 
			CNamespace* pNamespace = m_pModule->m_NamespaceMgr.OpenNamespace ((*GetTokenLocator (0)).m_Pos, (*(_cls1*) GetAstLocator (1)).m_Name);
			if (!pNamespace)
				return false;
		;
#line 5140 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 76:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 11 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamespaceDeclaration.llk"
			 
			m_pModule->m_NamespaceMgr.CloseNamespace ((*(_cls1*) GetAstLocator (1)).m_Name.m_List.GetCount () + 1);
		;
#line 5153 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 77:
			{
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
#line 16 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_pItem = (*(_cls16*) GetAstLocator (0)).m_pItem;
		;
#line 5166 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 78:
			{
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
#line 20 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_pItem = (*(_cls16*) GetAstLocator (1)).m_pItem;
		;
#line 5179 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 79:
			{
			CSymbolNode_init_declarator* __pSymbol = (CSymbolNode_init_declarator*) GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
#line 32 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_pItem = Declare (__pSymbol->m_Arg.pDeclSpecifiers, &(*(_cls21*) GetAstLocator (0)).m_Declarator);
			if (!(*__pAstNode).m_pItem)
				return false;
		;
#line 5194 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 80:
			{
			CSymbolNode_init_declarator* __pSymbol = (CSymbolNode_init_declarator*) GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
#line 38 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			if ((*__pAstNode).m_pItem->GetItemKind () != EModuleItem_Variable)
			{
				err::SetFormatStringError (_T("cannot apply initializer to '%s'"), (*__pAstNode).m_pItem->GetItemKindString ());
				return false;
			}
		;
#line 5211 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 81:
			{
			CSymbolNode_init_declarator* __pSymbol = (CSymbolNode_init_declarator*) GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
#line 46 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			if (m_Stage == EStage_Pass2)
				return m_pModule->m_OperatorMgr.MoveOperator ((*(_cls17*) GetAstLocator (1)).m_Value, (CVariable*) (*__pAstNode).m_pItem);
		;
#line 5225 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 82:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
#line 61 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			 return m_Stage == EStage_Pass1 ;
#line 5236 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 83:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
#line 62 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_TokenList.TakeOver (&(*(_cls3*) GetAstLocator (0)).m_TokenList);
		;
#line 5249 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 84:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
#line 66 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_Value = (*(_cls4*) GetAstLocator (0)).m_Value;
		;
#line 5262 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 85:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
#line 70 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
		;
#line 5274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 86:
			{
			CSymbolNode_declaration_terminator_2* __pSymbol = (CSymbolNode_declaration_terminator_2*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 89 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return SetFunctionBody (__pSymbol->m_Arg.pItem, &(*(_cls13*) GetAstLocator (0)).m_TokenList);
		;
#line 5287 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 87:
			{
			CSymbolNode_access_specifier* __pSymbol = (CSymbolNode_access_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 148 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pAccessSpecifier->SetAccess (EAccess_Public);
		;
#line 5300 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 88:
			{
			CSymbolNode_access_specifier* __pSymbol = (CSymbolNode_access_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 152 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pAccessSpecifier->SetAccess (EAccess_Private);
		;
#line 5313 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 89:
			{
			CSymbolNode_storage_class_specifier* __pSymbol = (CSymbolNode_storage_class_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 159 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pDeclSpecifiers->SetStorageClass (EStorageClass_Static);
		;
#line 5326 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 90:
			{
			CSymbolNode_storage_class_specifier* __pSymbol = (CSymbolNode_storage_class_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 163 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pDeclSpecifiers->SetStorageClass (EStorageClass_Typedef);
		;
#line 5339 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 91:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 170 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Const);
		;
#line 5352 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 92:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 174 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Volatile);
		;
#line 5365 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 93:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 178 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Signed);
		;
#line 5378 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 94:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Unsigned);
		;
#line 5391 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 95:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 186 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_LittleEndian);
		;
#line 5404 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 96:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 190 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_BigEndian);
		;
#line 5417 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 97:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 194 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Safe);
		;
#line 5430 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 98:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 198 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Unsafe);
		;
#line 5443 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 99:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Property);
		;
#line 5456 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 100:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 209 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Void));
		;
#line 5469 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 101:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 213 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Bool));
		;
#line 5482 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 102:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 217 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int32_be : EType_Int));
		;
#line 5495 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 103:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Char));
		;
#line 5508 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 104:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 225 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int16_be : EType_Short));
		;
#line 5521 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 105:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 229 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int32_be : EType_Long));
		;
#line 5534 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 106:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 233 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Float));
		;
#line 5547 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 107:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 237 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Double));
		;
#line 5560 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 108:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 241 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int8));
		;
#line 5573 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 109:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 245 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int16_be : EType_Int16));
		;
#line 5586 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 110:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 249 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int32_be : EType_Int32));
		;
#line 5599 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 111:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 253 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int64_be : EType_Int64));
		;
#line 5612 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 112:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 257 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_WChar));
		;
#line 5625 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 113:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 261 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Variant));
		;
#line 5638 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 114:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 265 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			err::SetStringError (_T("'auto' type specifier is not yet supported"));
			return false; 
		;
#line 5652 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 115:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 270 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls23*) GetAstLocator (0)).m_pType);
		;
#line 5665 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 116:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls25*) GetAstLocator (0)).m_pType);
		;
#line 5678 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 117:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 278 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls29*) GetAstLocator (0)).m_pType);
		;
#line 5691 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 118:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetProperty ((*(_cls31*) GetAstLocator (0)).m_pProperty);
		;
#line 5704 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 119:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 288 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls20*) GetAstLocator (0)).m_pType);
		;
#line 5717 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 120:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 297 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			if (IsTypeSpecified ())
				return false;
		;
#line 5731 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 121:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return m_Stage == EStage_Pass1 || FindType ((*(_cls1*) GetAstLocator (0)).m_Name) != NULL;
		;
#line 5744 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 122:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
#line 313 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			CType* pType = FindType ((*(_cls1*) GetAstLocator (0)).m_Name);
			if (!pType)
			{
				err::SetFormatStringError (_T("'%s' is not a type"), (*(_cls1*) GetAstLocator (0)).m_Name.GetFullName ());
				return false;
			}

			(*__pAstNode).m_pType = pType;
		;
#line 5764 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 123:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
#line 338 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_Declarator.m_Pos = (*GetTokenLocator (0)).m_Pos;
			(*__pAstNode).m_Declarator.m_Name = (*GetTokenLocator (0)).m_Data.m_String;
		;
#line 5778 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 124:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
#line 343 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_Declarator.m_PropertyAccessorKind = (*(_cls34*) GetAstLocator (1)).m_PropertyAccessorKind;
		;
#line 5791 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 125:
			{
			CSymbolNode_pointer* __pSymbol = (CSymbolNode_pointer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 359 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Local.pPointer = __pSymbol->m_Arg.pDeclarator->AddPointer ((*(_cls22*) GetAstLocator (0)).m_TypeKind);
		;
#line 5804 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 126:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
#line 371 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_TypeKind = EType_Pointer;
		;
#line 5817 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 127:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
#line 375 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_TypeKind = EType_Reference;
		;
#line 5830 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 128:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 390 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
		;
#line 5843 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 129:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 394 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix (0);
		;
#line 5856 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 130:
			{
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 420 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return DeclareFormalArg (__pSymbol->m_Arg.pFunctionSuffix, &(*(_cls19*) GetAstLocator (0)).m_TypeSpecifier, &(*(_cls21*) GetAstLocator (1)).m_Declarator, CValue ()) != NULL;
		;
#line 5869 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 131:
			{
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 424 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_IsVarArg;
		;
#line 5882 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 132:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 438 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			m_Endianness = EEndianness_LittleEndian;
		;
#line 5895 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 133:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 442 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			m_Endianness = EEndianness_BigEndian;
		;
#line 5908 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 134:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls23* __pAstNode = (_cls23*) __pSymbol->m_pAstNode;
#line 18 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			(*__pAstNode).m_pType = DeclareEnumType (
				(*(_cls24*) GetAstLocator (0)).m_TypeKind,
				IsValidLocator ((*GetTokenLocator (1))) ? (*GetTokenLocator (1)).m_Data.m_String : rtl::CString ()
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 5927 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 135:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
#line 36 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			(*__pAstNode).m_TypeKind = EType_Enum;
		;
#line 5940 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 136:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
#line 40 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			(*__pAstNode).m_TypeKind = EType_Enum_c;
		;
#line 5953 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 137:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
		;
#line 5966 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 138:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 52 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();	
		;
#line 5979 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 139:
			{
			CSymbolNode_enum_member* __pSymbol = (CSymbolNode_enum_member*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 63 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			CEnumMember* pMember = __pSymbol->m_Arg.pType->CreateMember ((*GetTokenLocator (0)).m_Data.m_String);
			if (!pMember)
				return false;

			pMember->m_Pos = (*GetTokenLocator (0)).m_Pos;
			
			if (IsValidLocator ((*(_cls3*) GetAstLocator (1)))) 
				pMember->SetExpression (&(*(_cls3*) GetAstLocator (1)).m_TokenList);
		;
#line 5999 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 140:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
#line 11 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAstNode).m_pType = DeclareStructType (
				(*(_cls26*) GetAstLocator (0)).m_TypeKind,
				IsValidLocator ((*GetTokenLocator (1))) ? (*GetTokenLocator (1)).m_Data.m_String : rtl::CString (),
				IsValidLocator ((*(_cls27*) GetAstLocator (2))) ? (*(_cls27*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 6019 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 141:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
#line 32 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAstNode).m_TypeKind = EType_Struct;
		;
#line 6032 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 142:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
#line 36 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAstNode).m_TypeKind = EType_Union;
		;
#line 6045 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 143:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAstNode).m_PackFactor = (*(_cls28*) GetAstLocator (0)).m_Value;
		;
#line 6058 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 144:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
#line 58 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAstNode).m_Value = (*GetTokenLocator (0)).m_Data.m_Integer;
		;
#line 6071 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 145:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
#line 62 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAstNode).m_Value = m_DefaultStructPackFactor;
		;
#line 6084 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 146:
			{
			CSymbolNode_struct_block* __pSymbol = (CSymbolNode_struct_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 69 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
		;
#line 6097 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 147:
			{
			CSymbolNode_struct_block* __pSymbol = (CSymbolNode_struct_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 74 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();
		;
#line 6110 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 148:
			{
			CSymbolNode_struct_member_declarator* __pSymbol = (CSymbolNode_struct_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 98 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			__pSymbol->m_Local.pMember = DeclareStructMember (__pSymbol->m_Arg.pType, __pSymbol->m_Arg.pTypeSpecifier, &(*(_cls21*) GetAstLocator (0)).m_Declarator);
			if (!__pSymbol->m_Local.pMember)
				return false;
		;
#line 6125 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 149:
			{
			CSymbolNode_struct_member_declarator* __pSymbol = (CSymbolNode_struct_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 104 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
		;
#line 6137 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 150:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 118 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			m_StructPackFactor = (*(_cls28*) GetAstLocator (0)).m_Value;
		;
#line 6150 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 151:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 10 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			(*__pAstNode).m_pType = DeclareClassType (
				(*(_cls30*) GetAstLocator (0)).m_TypeKind,
				IsValidLocator ((*GetTokenLocator (1))) ? (*GetTokenLocator (1)).m_Data.m_String : rtl::CString ()
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 6169 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 152:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
#line 30 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			(*__pAstNode).m_TypeKind = EType_Interface;
		;
#line 6182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 153:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
#line 34 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			(*__pAstNode).m_TypeKind = EType_Class;
		;
#line 6195 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 154:
			{
			CSymbolNode_base_class* __pSymbol = (CSymbolNode_base_class*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 45 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			return __pSymbol->m_Arg.pType->AddBaseType ((*(_cls2*) GetAstLocator (0)).m_pType);
		;
#line 6208 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 155:
			{
			CSymbolNode_class_block* __pSymbol = (CSymbolNode_class_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 52 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
		;
#line 6221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 156:
			{
			CSymbolNode_class_block* __pSymbol = (CSymbolNode_class_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();
		;
#line 6234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 157:
			{
			CSymbolNode_class_member_declarator* __pSymbol = (CSymbolNode_class_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 81 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			__pSymbol->m_Local.pMember = DeclareClassMember (__pSymbol->m_Arg.pType, __pSymbol->m_Arg.pDeclSpecifiers, &(*(_cls21*) GetAstLocator (0)).m_Declarator);
			if (!__pSymbol->m_Local.pMember)
				return false;
		;
#line 6249 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 158:
			{
			CSymbolNode_class_member_declarator* __pSymbol = (CSymbolNode_class_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 87 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			if (__pSymbol->m_Local.pMember->GetItemKind () != EModuleItem_ClassMember)
			{
				err::SetFormatStringError (_T("cannot apply initializer to '%s'"), __pSymbol->m_Local.pMember->GetItemKindString ());
				return false;
			}			
		;
#line 6266 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 159:
			{
			CSymbolNode_class_member_declarator* __pSymbol = (CSymbolNode_class_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 95 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
		;
#line 6278 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 160:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 9 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAstNode).m_pProperty = CreateProperty ((*(CPropertyBlock*) GetAstLocator (0)).m_pGetter, (*(CPropertyBlock*) GetAstLocator (0)).m_Setter);
		;
#line 6291 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 161:
			{
			CSymbolNode_property_accessor_declaration* __pSymbol = (CSymbolNode_property_accessor_declaration*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 32 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			return DeclarePropertyAccessor (__pSymbol->m_Arg.pBlock, &(*(_cls19*) GetAstLocator (0)).m_TypeSpecifier, &(*(_cls33*) GetAstLocator (1)).m_Declarator) != NULL;
		;
#line 6304 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 162:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
#line 46 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAstNode).m_Declarator.m_PropertyAccessorKind = (*(_cls34*) GetAstLocator (0)).m_PropertyAccessorKind;
			(*__pAstNode).m_Declarator.m_Pos = (*(_cls34*) GetAstLocator (0)).m_FirstToken.m_Pos;
		;
#line 6318 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 163:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 58 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAstNode).m_PropertyAccessorKind = EPropertyAccessor_Get;
		;
#line 6331 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 164:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 62 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAstNode).m_PropertyAccessorKind = EPropertyAccessor_Set;
		;
#line 6344 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
#line 6372 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
#line 48 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
#line 6385 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_generic_formal_argument* __pTarget = (CSymbolNode_generic_formal_argument*) pSymbol;
			CSymbolNode_generic_formal_argument_list* __pSymbol = (CSymbolNode_generic_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 6398 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_generic_formal_argument* __pTarget = (CSymbolNode_generic_formal_argument*) pSymbol;
			CSymbolNode_generic_formal_argument_list* __pSymbol = (CSymbolNode_generic_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 6411 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
			
#line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
#line 6424 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 13 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 6437 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 14 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 6450 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 15 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 6463 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
			
#line 30 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
#line 6476 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
			
#line 39 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
#line 6489 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_logical_or_expr* __pTarget = (CSymbolNode_logical_or_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 69 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6502 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 71 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.TrueValue;
#line 6515 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 71 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.FalseValue;
#line 6528 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 83 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6541 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 85 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 6554 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 97 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6567 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 99 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 6580 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 111 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6593 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 113 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 6606 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 125 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6619 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 127 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 6632 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 139 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6645 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 141 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 6658 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 153 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6671 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 155 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 6684 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6697 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 184 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 6710 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 219 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6723 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 6736 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 248 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6749 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 250 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 6762 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 277 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6775 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 279 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 6788 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 310 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6801 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 312 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.RValue;
#line 6814 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_postfix_expr* __pTarget = (CSymbolNode_postfix_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 371 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6827 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 372 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6840 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 376 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6853 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 380 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6866 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 381 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6879 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 385 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6892 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 41:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 389 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6905 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 42:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 393 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6918 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 43:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 397 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6931 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 44:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 402 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6944 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 45:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_cast_operator_rslv* __pSymbol = (CSymbolNode_cast_operator_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 416 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTypeSpecifier = &__pSymbol->m_Local.TypeSpecifier;
#line 6957 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 46:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 420 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6970 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 47:
			{
			CSymbolNode_postfix_operator* __pTarget = (CSymbolNode_postfix_operator*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 421 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6983 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 48:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 425 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 6996 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 49:
			{
			CSymbolNode_index_operator* __pTarget = (CSymbolNode_index_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 426 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7009 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 50:
			{
			CSymbolNode_call_operator* __pTarget = (CSymbolNode_call_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 427 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7022 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 51:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 467 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.Arg;
#line 7035 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 52:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 473 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.Arg;
#line 7048 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 53:
			{
			CSymbolNode_type_name_or_expr* __pTarget = (CSymbolNode_type_name_or_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 516 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue;
#line 7061 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 54:
			{
			CSymbolNode_type_name_or_expr* __pTarget = (CSymbolNode_type_name_or_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 520 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue;
#line 7074 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 55:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 524 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7087 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 56:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 536 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7100 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 57:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
			
#line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
#line 7113 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 58:
			{
			CSymbolNode_statement_pass1* __pTarget = (CSymbolNode_statement_pass1*) pSymbol;
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 7126 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 59:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 7139 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 60:
			{
			CSymbolNode_init_declarator_list* __pTarget = (CSymbolNode_init_declarator_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 6 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = &(*(_cls18*) GetAstLocator (0)).m_DeclSpecifiers;
#line 7152 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 61:
			{
			CSymbolNode_declaration_terminator_2* __pTarget = (CSymbolNode_declaration_terminator_2*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 7 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pItem = IsValidLocator ((*(_cls15*) GetAstLocator (1))) ? (*(_cls15*) GetAstLocator (1)).m_pItem : NULL;
#line 7165 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 62:
			{
			CSymbolNode_init_declarator* __pTarget = (CSymbolNode_init_declarator*) pSymbol;
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
#line 15 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7178 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 63:
			{
			CSymbolNode_init_declarator* __pTarget = (CSymbolNode_init_declarator*) pSymbol;
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
#line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7191 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 64:
			{
			CSymbolNode_initializer* __pTarget = (CSymbolNode_initializer*) pSymbol;
			CSymbolNode_init_declarator* __pSymbol = (CSymbolNode_init_declarator*) GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
			
#line 45 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pType = ((CVariable*) (*__pAstNode).m_pItem)->GetType ();
#line 7204 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 65:
			{
			CSymbolNode_initializer_list* __pTarget = (CSymbolNode_initializer_list*) pSymbol;
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
			
#line 69 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7217 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 66:
			{
			CSymbolNode_initializer_list_entry* __pTarget = (CSymbolNode_initializer_list_entry*) pSymbol;
			CSymbolNode_initializer_list* __pSymbol = (CSymbolNode_initializer_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 75 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7230 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 67:
			{
			CSymbolNode_initializer_list_entry* __pTarget = (CSymbolNode_initializer_list_entry*) pSymbol;
			CSymbolNode_initializer_list* __pSymbol = (CSymbolNode_initializer_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 75 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7243 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 68:
			{
			CSymbolNode_initializer* __pTarget = (CSymbolNode_initializer*) pSymbol;
			CSymbolNode_initializer_list_entry* __pSymbol = (CSymbolNode_initializer_list_entry*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 79 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7256 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 69:
			{
			CSymbolNode_declaration_specifier* __pTarget = (CSymbolNode_declaration_specifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
			
#line 115 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = &(*__pAstNode).m_DeclSpecifiers;
#line 7269 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 70:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
			
#line 131 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
#line 7282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 71:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 135 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7295 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 72:
			{
			CSymbolNode_access_specifier* __pTarget = (CSymbolNode_access_specifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 136 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pAccessSpecifier = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7308 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 73:
			{
			CSymbolNode_storage_class_specifier* __pTarget = (CSymbolNode_storage_class_specifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 137 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7321 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 74:
			{
			CSymbolNode_type_specifier* __pTarget = (CSymbolNode_type_specifier*) pSymbol;
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
#line 7334 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 75:
			{
			CSymbolNode_type_modifier* __pTarget = (CSymbolNode_type_modifier*) pSymbol;
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 143 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeModifiers = __pSymbol->m_Arg.pTypeSpecifier;
#line 7347 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 76:
			{
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
			
#line 336 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 7360 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 77:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
			
#line 347 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 7373 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 78:
			{
			CSymbolNode_type_modifier* __pTarget = (CSymbolNode_type_modifier*) pSymbol;
			CSymbolNode_pointer* __pSymbol = (CSymbolNode_pointer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 362 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeModifiers = __pSymbol->m_Local.pPointer;
#line 7386 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 79:
			{
			CSymbolNode_array_suffix* __pTarget = (CSymbolNode_array_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 384 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 7399 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 80:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 385 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 7412 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 81:
			{
			CSymbolNode_function_formal_argument_list* __pTarget = (CSymbolNode_function_formal_argument_list*) pSymbol;
			CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 408 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Local.pFunctionSuffix;
#line 7425 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 82:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 413 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
#line 7438 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 83:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 414 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
#line 7451 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 84:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls23* __pAstNode = (_cls23*) __pSymbol->m_pAstNode;
			
#line 27 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 7464 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 85:
			{
			CSymbolNode_enum_member_list* __pTarget = (CSymbolNode_enum_member_list*) pSymbol;
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 50 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7477 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 86:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 58 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7490 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 87:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 58 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7503 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 88:
			{
			CSymbolNode_generic_formal_argument_list* __pTarget = (CSymbolNode_generic_formal_argument_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
			
#line 21 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 7516 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 89:
			{
			CSymbolNode_inheritance_list* __pTarget = (CSymbolNode_inheritance_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
			
#line 22 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 7529 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 90:
			{
			CSymbolNode_struct_block* __pTarget = (CSymbolNode_struct_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
			
#line 23 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 7542 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 91:
			{
			CSymbolNode_struct_member_declaration* __pTarget = (CSymbolNode_struct_member_declaration*) pSymbol;
			CSymbolNode_struct_block* __pSymbol = (CSymbolNode_struct_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 72 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7555 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 92:
			{
			CSymbolNode_struct_member_declarator_list* __pTarget = (CSymbolNode_struct_member_declarator_list*) pSymbol;
			CSymbolNode_struct_member_declaration* __pSymbol = (CSymbolNode_struct_member_declaration*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 82 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls19*) GetAstLocator (0)).m_TypeSpecifier;
#line 7568 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 82 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 7572 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 93:
			{
			CSymbolNode_struct_member_declarator* __pTarget = (CSymbolNode_struct_member_declarator*) pSymbol;
			CSymbolNode_struct_member_declarator_list* __pSymbol = (CSymbolNode_struct_member_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 89 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
#line 7585 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 89 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 7589 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 94:
			{
			CSymbolNode_struct_member_declarator* __pTarget = (CSymbolNode_struct_member_declarator*) pSymbol;
			CSymbolNode_struct_member_declarator_list* __pSymbol = (CSymbolNode_struct_member_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 89 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
#line 7602 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 89 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 7606 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 95:
			{
			CSymbolNode_initializer* __pTarget = (CSymbolNode_initializer*) pSymbol;
			CSymbolNode_struct_member_declarator* __pSymbol = (CSymbolNode_struct_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 103 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Local.pMember->GetType ();
#line 7619 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 96:
			{
			CSymbolNode_generic_formal_argument_list* __pTarget = (CSymbolNode_generic_formal_argument_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
			
#line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 7632 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 97:
			{
			CSymbolNode_inheritance_list* __pTarget = (CSymbolNode_inheritance_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
			
#line 20 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 7645 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 98:
			{
			CSymbolNode_class_block* __pTarget = (CSymbolNode_class_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
			
#line 21 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 7658 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 99:
			{
			CSymbolNode_base_class* __pTarget = (CSymbolNode_base_class*) pSymbol;
			CSymbolNode_inheritance_list* __pSymbol = (CSymbolNode_inheritance_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 40 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7671 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 100:
			{
			CSymbolNode_base_class* __pTarget = (CSymbolNode_base_class*) pSymbol;
			CSymbolNode_inheritance_list* __pSymbol = (CSymbolNode_inheritance_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 40 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7684 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 101:
			{
			CSymbolNode_class_member_declaration* __pTarget = (CSymbolNode_class_member_declaration*) pSymbol;
			CSymbolNode_class_block* __pSymbol = (CSymbolNode_class_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 55 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7697 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 102:
			{
			CSymbolNode_class_member_declarator_list* __pTarget = (CSymbolNode_class_member_declarator_list*) pSymbol;
			CSymbolNode_class_member_declaration* __pSymbol = (CSymbolNode_class_member_declaration*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 65 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pDeclSpecifiers = &(*(_cls18*) GetAstLocator (0)).m_DeclSpecifiers;
#line 7710 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 65 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 7714 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 103:
			{
			CSymbolNode_class_member_declarator* __pTarget = (CSymbolNode_class_member_declarator*) pSymbol;
			CSymbolNode_class_member_declarator_list* __pSymbol = (CSymbolNode_class_member_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 72 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7727 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 72 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 7731 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 104:
			{
			CSymbolNode_class_member_declarator* __pTarget = (CSymbolNode_class_member_declarator*) pSymbol;
			CSymbolNode_class_member_declarator_list* __pSymbol = (CSymbolNode_class_member_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 72 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7744 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 72 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 7748 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 105:
			{
			CSymbolNode_initializer* __pTarget = (CSymbolNode_initializer*) pSymbol;
			CSymbolNode_class_member_declarator* __pSymbol = (CSymbolNode_class_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 94 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = ((CClassFieldMember*) __pSymbol->m_Local.pMember)->GetField ()->GetType ();
#line 7761 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 106:
			{
			CSymbolNode_property_accessor_declaration* __pTarget = (CSymbolNode_property_accessor_declaration*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CPropertyBlock* __pAstNode = (CPropertyBlock*) __pSymbol->m_pAstNode;
			
#line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			__pTarget->m_Arg.pBlock = &(*__pAstNode);
#line 7774 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 107:
			{
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
			
#line 43 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 7787 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 108:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
			
#line 45 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 7800 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
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
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
#line 124 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
#line 7834 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_attribute_block:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 5 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_AttributeBlock.llk"
				
		m_pModule->m_AttributeMgr.CreateAttributeSet ();
	;
#line 7897 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 108 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_DeclSpecifiers);
	;
#line 7912 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_function_suffix:
			{
				CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 405 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		__pSymbol->m_Local.pFunctionSuffix = __pSymbol->m_Arg.pDeclarator->AddFormalArgSuffix ();
	;
#line 7948 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		default:
			return true;
		}
	}

	bool
	Leave (size_t Index)
	{
		switch (Index)
		{
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
#line 128 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
#line 8009 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_call_operator:
			{
				CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 463 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
	return m_pModule->m_OperatorMgr.CallOperator (__pSymbol->m_Arg.pValue, &__pSymbol->m_Local.ArgList);
;
#line 8057 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 112 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_DeclSpecifiers);
	;
#line 8087 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_enum_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls23* __pAstNode = (_cls23*) __pSymbol->m_pAstNode;
#line 9 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
				
		if ((*__pAstNode).m_pType->GetTypeKind () == EType_Enum_c ||
			(*__pAstNode).m_pType->GetName ().IsEmpty ())
		{
			return m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->ExposeEnumMembers ((*__pAstNode).m_pType);
		}
	;
#line 8117 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 112;
				pTransition->m_ResolverIndex = 190;
				pTransition->m_ResolverElseIndex = 115;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 245;
				pTransition->m_ResolverIndex = 148;
				pTransition->m_ResolverElseIndex = 373;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 916;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 3:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 457;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 458;
				return ELaDfaResult_Production;
					
			}
			
		
		case 4:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 918;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 5:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 393;
				pTransition->m_ResolverIndex = 148;
				pTransition->m_ResolverElseIndex = 393;
						
				return ELaDfaResult_Resolver;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 395;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 393;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 6:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 920;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 7:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 408;
				pTransition->m_ResolverIndex = 148;
				pTransition->m_ResolverElseIndex = 408;
						
				return ELaDfaResult_Resolver;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 407;
				pTransition->m_ResolverIndex = 190;
				pTransition->m_ResolverElseIndex = 408;
						
				return ELaDfaResult_Resolver;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 407;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 8:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 406;
				pTransition->m_ResolverIndex = 190;
				pTransition->m_ResolverElseIndex = 408;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 9:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 923;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 10:
			
			pTransition->m_ProductionIndex = 444;
			pTransition->m_ResolverIndex = 148;
			pTransition->m_ResolverElseIndex = 444;
				
			return ELaDfaResult_Resolver;
			
		
		case 11:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 445;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 12:
			
			switch (LookaheadToken)
			{
			
			case '[':
					
				pTransition->m_ProductionIndex = 926;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 13:
			
			switch (LookaheadToken)
			{
			
			case ']':
					
				pTransition->m_ProductionIndex = 507;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 506;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 14:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 928;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 15:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 509;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 508;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 16:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 451;
				pTransition->m_ResolverIndex = 190;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 17:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 453;
				pTransition->m_ResolverIndex = 190;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 18:
			
			switch (LookaheadToken)
			{
			
			case '#':
					
				pTransition->m_ProductionIndex = 932;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 19:
			
			switch (LookaheadToken)
			{
			
			case EToken_Endian:
					
				pTransition->m_ProductionIndex = 199;
				return ELaDfaResult_Production;
					
			
			case EToken_Pack:
					
				pTransition->m_ProductionIndex = 214;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 20:
			
			switch (LookaheadToken)
			{
			
			case '&':
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 21:
			
			switch (LookaheadToken)
			{
			
			case '+':
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 22:
			
			switch (LookaheadToken)
			{
			
			case '-':
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 23:
			
			switch (LookaheadToken)
			{
			
			case '*':
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 24:
			
			switch (LookaheadToken)
			{
			
			case '~':
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 25:
			
			switch (LookaheadToken)
			{
			
			case '!':
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 26:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 27:
			
			switch (LookaheadToken)
			{
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 28:
			
			switch (LookaheadToken)
			{
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 29:
			
			switch (LookaheadToken)
			{
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 30:
			
			switch (LookaheadToken)
			{
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 31:
			
			switch (LookaheadToken)
			{
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 32:
			
			switch (LookaheadToken)
			{
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 33:
			
			switch (LookaheadToken)
			{
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 34:
			
			switch (LookaheadToken)
			{
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 35:
			
			switch (LookaheadToken)
			{
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 36:
			
			switch (LookaheadToken)
			{
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 265;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 444;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 37:
			
			switch (LookaheadToken)
			{
			
			case ',':
					
				pTransition->m_ProductionIndex = 448;
				pTransition->m_ResolverIndex = 650;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 38:
			
			switch (LookaheadToken)
			{
			
			case '}':
					
				pTransition->m_ProductionIndex = 448;
				pTransition->m_ResolverIndex = 650;
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


	} // namespace axl {
	} // namespace jnc {


#pragma warning (default: 4065)
