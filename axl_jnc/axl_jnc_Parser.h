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
	ESymbol_type_name_list = 13, 
	ESymbol_generic_formal_argument_list = 14, 
	ESymbol_generic_formal_argument = 15, 
	ESymbol_pragma_pack = 16, 
	ESymbol_struct_pack_factor_value = 17, 
	ESymbol_expression_pass1 = 18, 
	ESymbol_primary_expr_pass1 = 19, 
	ESymbol_expression = 20, 
	ESymbol_conditional_expr = 21, 
	ESymbol_constant_expr = 22, 
	ESymbol_constant_integer_expr = 23, 
	ESymbol_logical_or_expr = 24, 
	ESymbol_logical_and_expr = 25, 
	ESymbol_inclusive_or_expr = 26, 
	ESymbol_exclusive_or_expr = 27, 
	ESymbol_and_expr = 28, 
	ESymbol_equality_expr = 29, 
	ESymbol_relational_expr = 30, 
	ESymbol_equality_operator = 31, 
	ESymbol_shift_expr = 32, 
	ESymbol_relational_operator = 33, 
	ESymbol_additive_expr = 34, 
	ESymbol_shift_operator = 35, 
	ESymbol_multiplicative_expr = 36, 
	ESymbol_additivie_operator = 37, 
	ESymbol_assignment_expr = 38, 
	ESymbol_multiplicative_operator = 39, 
	ESymbol_unary_expr = 40, 
	ESymbol_assignment_operator = 41, 
	ESymbol_postfix_expr = 42, 
	ESymbol_cast_operator_rslv = 43, 
	ESymbol_type_specifier_modifier = 44, 
	ESymbol_primary_expr = 45, 
	ESymbol_postfix_operator = 46, 
	ESymbol_call_operator = 47, 
	ESymbol_literal = 48, 
	ESymbol_type_name_or_expr = 49, 
	ESymbol_compound_stmt_pass1 = 50, 
	ESymbol_statement_pass1 = 51, 
	ESymbol_compound_stmt = 52, 
	ESymbol_expression_stmt = 53, 
	ESymbol_if_stmt = 54, 
	ESymbol_switch_stmt = 55, 
	ESymbol_while_stmt = 56, 
	ESymbol_do_stmt = 57, 
	ESymbol_for_stmt = 58, 
	ESymbol_break_stmt = 59, 
	ESymbol_continue_stmt = 60, 
	ESymbol_return_stmt = 61, 
	ESymbol_switch_block_stmt = 62, 
	ESymbol_loop_stmt = 63, 
	ESymbol_attribute_block = 64, 
	ESymbol_attribute_declarator_list = 65, 
	ESymbol_attribute_declarator = 66, 
	ESymbol_declaration_specifier_list = 67, 
	ESymbol_init_declarator_list = 68, 
	ESymbol_declaration_terminator_2 = 69, 
	ESymbol_init_declarator = 70, 
	ESymbol_declarator = 71, 
	ESymbol_initializer = 72, 
	ESymbol_initializer_list = 73, 
	ESymbol_initializer_list_entry = 74, 
	ESymbol_declaration_terminator = 75, 
	ESymbol_declaration_specifier = 76, 
	ESymbol_access_specifier = 77, 
	ESymbol_storage_class_specifier = 78, 
	ESymbol_type_specifier = 79, 
	ESymbol_type_modifier = 80, 
	ESymbol_enum_specifier = 81, 
	ESymbol_struct_specifier = 82, 
	ESymbol_union_specifier = 83, 
	ESymbol_class_specifier = 84, 
	ESymbol_property_specifier = 85, 
	ESymbol_qualified_type_name_rslv = 86, 
	ESymbol_qualified_type_name = 87, 
	ESymbol_generic_actual_argument_list = 88, 
	ESymbol_property_accessor_kind = 89, 
	ESymbol_pointer_kind = 90, 
	ESymbol_array_suffix = 91, 
	ESymbol_function_suffix = 92, 
	ESymbol_function_formal_argument_list = 93, 
	ESymbol_function_formal_argument = 94, 
	ESymbol_pragma_endian = 95, 
	ESymbol_pragma_endian_value = 96, 
	ESymbol_enum_kind = 97, 
	ESymbol_enum_block = 98, 
	ESymbol_enum_member_list = 99, 
	ESymbol_enum_member = 100, 
	ESymbol_struct_pack_factor = 101, 
	ESymbol_struct_block = 102, 
	ESymbol_struct_member_declaration = 103, 
	ESymbol_struct_member_declarator_list = 104, 
	ESymbol_struct_member_declarator = 105, 
	ESymbol_struct_member_bit_field = 106, 
	ESymbol_union_block = 107, 
	ESymbol_union_member_declaration = 108, 
	ESymbol_union_member_declarator_list = 109, 
	ESymbol_union_member_declarator = 110, 
	ESymbol_union_member_bit_field = 111, 
	ESymbol_class_kind = 112, 
	ESymbol_class_block = 113, 
	ESymbol_class_member_declaration = 114, 
	ESymbol_class_member_declarator_list = 115, 
	ESymbol_class_member_declarator = 116, 
	ESymbol_property_block = 117, 
	ESymbol_property_accessor_declaration = 118, 
	ESymbol_property_declarator = 119, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 2,

		StartSymbol        = 0,
		StartPragmaSymbol  = 207,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 110,
		SymbolCount        = 208,
		SequenceCount      = 271,
		ActionCount        = 189,
		ArgumentCount      = 104,
		BeaconCount        = 94,
		LaDfaCount         = 43,

		TotalCount         = 1019,

		NamedSymbolCount   = 120,

		TokenFirst         = 0,
		TokenEnd           = 110,
		SymbolFirst        = 110,
		NamedSymbolEnd     = 230,
		SymbolEnd          = 318,
		SequenceFirst      = 318,
		SequenceEnd        = 589,
		ActionFirst        = 589,
		ActionEnd          = 778,
		ArgumentFirst      = 778,
		ArgumentEnd        = 882,
		BeaconFirst        = 882,
		BeaconEnd          = 976,
		LaDfaFirst         = 976,
		LaDfaEnd           = 1019,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class _cls1: public CAstNode
	{
	public:
#line 20 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CQualifiedName m_Name;
;
#line 202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls2: public CAstNode
	{
	public:
#line 38 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
#line 212 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls3: public CAstNode
	{
	public:
#line 55 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	rtl::CBoxListT <CType*> m_TypeList;
;
#line 222 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls4: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
#line 232 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls5: public CAstNode
	{
	public:
#line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
#line 242 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls6: public CAstNode
	{
	public:
#line 34 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
#line 252 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls7: public CAstNode
	{
	public:
#line 50 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	intptr_t m_Value;
;
#line 262 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls8: public CAstNode
	{
	public:
#line 163 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 272 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls9: public CAstNode
	{
	public:
#line 192 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls10: public CAstNode
	{
	public:
#line 229 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 292 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls11: public CAstNode
	{
	public:
#line 258 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls12: public CAstNode
	{
	public:
#line 287 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 312 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls13: public CAstNode
	{
	public:
#line 320 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 322 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls14: public CAstNode
	{
	public:
#line 516 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CString m_String;
;
#line 332 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls15: public CAstNode
	{
	public:
#line 532 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CType* m_pType;
;
#line 342 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls16: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
#line 352 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class CCompoundStmt: public CAstNode
	{
	public:
#line 36 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
		
	CScope* m_pScope;
;
#line 362 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls18: public CAstNode
	{
	public:
#line 11 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CModuleItem* m_pItem;
;
#line 372 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls19: public CAstNode
	{
	public:
#line 27 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CModuleItem* m_pItem;
;
#line 382 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls20: public CAstNode
	{
	public:
#line 56 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CValue m_Value;
	rtl::CBoxListT <CToken> m_TokenList;
;
#line 393 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls21: public CAstNode
	{
	public:
#line 103 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CDeclSpecifiers m_DeclSpecifiers;
;
#line 403 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls22: public CAstNode
	{
	public:
#line 119 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CTypeSpecifierModifiers m_TypeSpecifier;
;
#line 413 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls23: public CAstNode
	{
	public:
#line 316 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CType* m_pType;
;
#line 423 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls24: public CAstNode
	{
	public:
#line 340 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CDeclarator m_Declarator;
;
#line 433 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls25: public CAstNode
	{
	public:
#line 374 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	EType m_TypeKind;
;
#line 443 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls26: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
		
	CEnumType* m_pType;
;
#line 453 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls27: public CAstNode
	{
	public:
#line 31 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
		
	EType m_TypeKind;
;
#line 463 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls28: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
		
	CStructType* m_pType;
;
#line 473 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls29: public CAstNode
	{
	public:
#line 27 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
		
	size_t m_PackFactor;
;
#line 483 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls30: public CAstNode
	{
	public:
#line 38 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
		
	size_t m_Value;
;
#line 493 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls31: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
		
	CUnionType* m_pType;
;
#line 503 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls32: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
		
	CClassType* m_pType;
;
#line 513 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls33: public CAstNode
	{
	public:
#line 28 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
		
	EType m_TypeKind;
;
#line 523 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls34: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	CProperty* m_pProperty;
;
#line 533 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 549 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls36: public CAstNode
	{
	public:
#line 39 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	CDeclarator m_Declarator;
;
#line 559 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls37: public CAstNode
	{
	public:
#line 53 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	EPropertyAccessor m_PropertyAccessorKind;
;
#line 569 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 595 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 361 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 616 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 363 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		CDeclPointer* pPointer;
	;
#line 625 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 391 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 645 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_declarator_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_declarator_suffix;
			
		}
	};
		
	
	
	
	
	
	
	
	class CSymbolNode_primary_expr_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
#line 12 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			rtl::CBoxListT <CToken>* pTokenList;
#line 671 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 692 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 65 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue TrueValue;
	CValue FalseValue;
;
#line 702 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 724 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 80 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 733 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 753 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 94 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 762 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 782 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 108 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 791 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 811 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 122 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 820 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 840 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 136 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 849 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 869 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 150 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 878 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 898 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 179 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 907 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 928 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 216 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 937 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 958 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 245 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 967 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 988 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue OpValue2;
;
#line 997 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1018 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 307 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue RValue;
;
#line 1027 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1048 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1069 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1091 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1111 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 475 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1131 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 424 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1151 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 448 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1171 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 450 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue Arg;
	rtl::CBoxListT <CValue> ArgList;
;
#line 1181 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 537 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
	CValue Value;
;
#line 1206 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
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
#line 1226 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1246 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_statement_pass1 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_statement_pass1;
			
		}
	};
		
	
	
	
	class CSymbolNode_if_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
#line 72 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
	CBasicBlock* pThenBlock;
	CBasicBlock* pElseBlock;
	CBasicBlock* pFollowBlock;
;
#line 1272 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_if_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_if_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	class CSymbolNode_while_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
#line 113 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
	CBasicBlock* pConditionBlock;
	CBasicBlock* pBodyBlock;
	CBasicBlock* pFollowBlock;
;
#line 1299 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_while_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_while_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_do_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
#line 137 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
	CBasicBlock* pConditionBlock;
	CBasicBlock* pBodyBlock;
	CBasicBlock* pFollowBlock;
;
#line 1325 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_do_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_do_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_for_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
#line 165 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
	CBasicBlock* pConditionBlock;
	CBasicBlock* pBodyBlock;
	CBasicBlock* pLoopBlock;
	CBasicBlock* pFollowBlock;
;
#line 1352 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_for_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_for_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	
	
	
	class CSymbolNode_loop_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
#line 208 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			const CToken::CPos* pPos; CBasicBlock* pBreakBlock; CBasicBlock* pContinueBlock;
#line 1378 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_loop_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_loop_stmt;
			
		}
	};
		
	
	
	
	
	
	class CSymbolNode_init_declarator_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 14 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclSpecifiers* pDeclSpecifiers;
#line 1402 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1422 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1442 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1463 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1483 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1503 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1524 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1544 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1564 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 211 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CTypeSpecifier* pTypeSpecifier;
#line 1584 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1604 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 396 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 1634 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 407 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 1654 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 409 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
		CDeclFunctionSuffix* pFunctionSuffix;
	;
#line 1663 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 420 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
#line 1685 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 425 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
#line 1705 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1728 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1748 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 1768 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_enum_member ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_enum_member;
			
		}
	};
		
	
	
	class CSymbolNode_struct_block: public CSymbolNode
	{
	public:
			
		struct
		{
#line 52 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CStructType* pType;
#line 1789 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 64 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CStructType* pType;
#line 1809 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 72 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CStructType* pType; CTypeSpecifierModifiers* pTypeSpecifier;
#line 1829 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 76 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CStructType* pType; CTypeSpecifierModifiers* pTypeSpecifier;
#line 1849 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_struct_member_declarator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_struct_member_declarator;
			
		}
	};
		
	
	
	class CSymbolNode_union_block: public CSymbolNode
	{
	public:
			
		struct
		{
#line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			CUnionType* pType;
#line 1870 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_union_block ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_union_block;
			
		}
	};
		
	
	class CSymbolNode_union_member_declaration: public CSymbolNode
	{
	public:
			
		struct
		{
#line 31 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			CUnionType* pType;
#line 1890 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_union_member_declaration ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_union_member_declaration;
			
		}
	};
		
	
	class CSymbolNode_union_member_declarator_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 39 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			CUnionType* pType; CTypeSpecifierModifiers* pTypeSpecifier;
#line 1910 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_union_member_declarator_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_union_member_declarator_list;
			
		}
	};
		
	
	class CSymbolNode_union_member_declarator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 43 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			CUnionType* pType; CTypeSpecifierModifiers* pTypeSpecifier;
#line 1930 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_union_member_declarator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_union_member_declarator;
			
		}
	};
		
	
	
	
	class CSymbolNode_class_block: public CSymbolNode
	{
	public:
			
		struct
		{
#line 42 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CClassType* pType;
#line 1952 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_class_block ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_class_block;
			
		}
	};
		
	
	class CSymbolNode_class_member_declaration: public CSymbolNode
	{
	public:
			
		struct
		{
#line 54 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CClassType* pType;
#line 1972 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 63 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CDeclSpecifiers* pDeclSpecifiers; CClassType* pType;
#line 1992 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 67 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CDeclSpecifiers* pDeclSpecifiers; CClassType* pType;
#line 2012 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 69 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
	CModuleItem* pMember;
;
#line 2021 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 2042 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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

	CEnumType*
	DeclareEnumType (
		EType TypeKind,
		rtl::CString& Name
		);

	CStructType*
	DeclareStructType (
		rtl::CString& Name,
		rtl::CBoxListT <CType*>* pBaseTypeList,
		size_t PackFactor
		);

	CStructMember*
	DeclareStructMember (
		CStructType* pStructType,
		CTypeSpecifierModifiers* pTypeSpecifier,
		CDeclarator* pDeclarator
		);

	CUnionType*
	DeclareUnionType (rtl::CString& Name);

	CUnionMember*
	DeclareUnionMember (
		CUnionType* pStructType,
		CTypeSpecifierModifiers* pTypeSpecifier,
		CDeclarator* pDeclarator
		);

	CClassType*
	DeclareClassType (
		EType TypeKind,
		rtl::CString& Name,
		rtl::CBoxListT <CType*>* pBaseTypeList,
		size_t PackFactor
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
			0, -1, 318, -1, -1, -1, -1, -1, -1, 318, -1, 318, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 318, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, -1, -1, 318, 318, 318, 318, 318, 318, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, 115, -1, 114, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 114, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 112, 113, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, -1, -1, 114, 114, 114, 114, 114, 114, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 454, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 456, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, -1, -1, 457, 457, 457, 457, 457, 457, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 426, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 117, -1, -1, -1, -1, -1, -1, 117, -1, 114, -1, 117, -1, -1, -1, -1, 117, 117, 117, 117, -1, -1, -1, 117, 117, 976, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, -1, 117, 117, 117, 117, 117, 117, 117, 117, 117, -1, 117, -1, -1, 117, 117, 117, 117, 117, 117, -1, 113, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, -1, -1, 114, 114, 114, 114, 114, 114, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, 162, -1, -1, -1, 163, -1, -1, -1, -1, 163, 163, 163, 163, -1, -1, -1, 163, 163, 163, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 163, 163, -1, 163, 163, 163, 163, 163, 163, 163, 163, 164, -1, 165, -1, -1, 166, 167, 168, 169, 170, 171, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 319, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 322, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, 322, -1, -1, 322, 322, 322, 322, 322, 322, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 472, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, -1, -1, 472, 472, 472, 472, 472, 472, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, -1, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 526, -1, 527, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 324, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, 324, -1, -1, 324, 324, 324, 324, 324, 324, -1, -1, 
			-1, -1, -1, -1, -1, 326, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 27, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 328, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 551, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 552, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 329, 
			-1, 337, 337, 337, 337, 337, 337, 337, 337, 332, 337, 334, 337, 336, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 337, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 338, -1, -1, -1, -1, 338, 338, 338, 338, -1, -1, -1, 338, 338, 338, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 338, 338, -1, 338, 338, 338, 338, 338, 338, 338, 338, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 341, -1, -1, -1, -1, 341, 341, 341, 341, -1, -1, -1, 341, 341, 341, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 341, 341, -1, 341, 341, 341, 341, 341, 341, 341, 341, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 339, -1, -1, -1, -1, 339, 339, 339, 339, -1, -1, -1, 339, 339, 339, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 339, 339, -1, 339, 339, 339, 339, 339, 339, 339, 339, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 340, -1, -1, -1, -1, 340, 340, 340, 340, -1, -1, -1, 340, 340, 340, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 340, 340, -1, 340, 340, 340, 340, 340, 340, 340, 340, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 343, -1, -1, -1, -1, 343, 343, 343, 343, -1, -1, -1, 343, 343, 343, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 343, 343, -1, 343, 343, 343, 343, 343, 343, 343, 343, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 345, -1, -1, -1, -1, 345, 345, 345, 345, -1, -1, -1, 345, 345, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 345, 345, -1, 345, 345, 345, 345, 345, 345, 345, 345, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 347, -1, -1, -1, -1, 347, 347, 347, 347, -1, -1, -1, 347, 347, 347, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 347, 347, -1, 347, 347, 347, 347, 347, 347, 347, 347, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 349, -1, -1, -1, -1, 349, 349, 349, 349, -1, -1, -1, 349, 349, 349, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 349, 349, -1, 349, 349, 349, 349, 349, 349, 349, 349, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 351, -1, -1, -1, -1, 351, 351, 351, 351, -1, -1, -1, 351, 351, 351, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 351, 351, -1, 351, 351, 351, 351, 351, 351, 351, 351, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 353, -1, -1, -1, -1, 353, 353, 353, 353, -1, -1, -1, 353, 353, 353, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 353, 353, -1, 353, 353, 353, 353, 353, 353, 353, 353, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 357, -1, -1, -1, -1, 357, 357, 357, 357, -1, -1, -1, 357, 357, 357, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 357, 357, -1, 357, 357, 357, 357, 357, 357, 357, 357, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 355, 356, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 363, -1, -1, -1, -1, 363, 363, 363, 363, -1, -1, -1, 363, 363, 363, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 363, 363, -1, 363, 363, 363, 363, 363, 363, 363, 363, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 359, 360, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 361, 362, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 367, -1, -1, -1, -1, 367, 367, 367, 367, -1, -1, -1, 367, 367, 367, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 367, 367, -1, 367, 367, 367, 367, 367, 367, 367, 367, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 365, 366, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 371, -1, -1, -1, -1, 371, 371, 371, 371, -1, -1, -1, 371, 371, 371, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 371, 371, -1, 371, 371, 371, 371, 371, 371, 371, 371, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 369, 370, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 376, -1, -1, -1, -1, 376, 376, 376, 376, -1, -1, -1, 376, 376, 376, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 376, 376, -1, 376, 376, 376, 376, 376, 376, 376, 376, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 373, 374, 375, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 977, -1, -1, -1, -1, 390, 392, 393, 391, -1, -1, -1, 394, 395, 389, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 396, 397, -1, 389, 389, 389, 389, 389, 389, 389, 389, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 378, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 400, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 400, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 400, 400, 400, 400, 400, 400, 400, 400, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 399, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 477, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 478, 478, 478, 478, 478, 478, 478, 478, 478, 978, 477, 477, 477, 477, 477, 477, 477, 477, 477, 477, 477, 477, 477, 477, 477, -1, -1, 477, 477, 477, 477, 477, 477, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 420, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 411, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 412, 413, 415, 416, 417, 418, 419, 414, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 403, -1, -1, -1, -1, -1, -1, -1, 405, -1, 402, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 406, 407, 404, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 980, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 421, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 982, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 984, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 425, 425, 425, 425, 425, 425, 425, 425, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, 423, -1, -1, 423, 423, 423, 423, 423, 423, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 427, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 430, 430, 430, 430, 430, 430, 430, 430, 429, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 430, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 431, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 433, -1, -1, -1, -1, 433, 433, 433, 433, -1, -1, -1, 433, 433, 433, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 433, 433, -1, 433, 433, 433, 433, 433, 433, 433, 433, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 434, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 436, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 440, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 441, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 442, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 446, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 447, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 448, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 117, -1, -1, -1, -1, -1, -1, 117, -1, -1, -1, 117, -1, -1, -1, -1, 117, 117, 117, 117, -1, -1, -1, 117, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, -1, 117, 117, 117, 117, 117, 117, 117, 117, 117, -1, 117, 438, 439, 117, 117, 117, 117, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 445, -1, -1, -1, -1, -1, -1, 445, -1, -1, -1, 445, -1, -1, -1, -1, 445, 445, 445, 445, -1, -1, -1, 445, 445, 445, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 445, 445, -1, 445, 445, 445, 445, 445, 445, 445, 445, 445, -1, 445, -1, -1, 445, 445, 445, 445, 445, 445, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 449, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 451, -1, -1, -1, -1, -1, -1, -1, 451, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 451, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 453, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 470, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, -1, -1, 470, 470, 470, 470, 470, 470, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 459, -1, -1, 459, -1, -1, -1, -1, -1, 459, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, 469, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 461, -1, -1, 461, -1, -1, -1, -1, -1, 461, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 519, -1, -1, 519, -1, -1, -1, -1, -1, 519, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 273, 273, 273, 273, 273, 273, 273, 273, 987, 273, 273, 273, 985, 273, 273, 273, 273, 1000, 1001, 1002, 1003, 273, 273, 273, 1004, 1005, 1006, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 1007, 1008, 273, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 
			-1, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 
			-1, 468, 468, 468, 1017, 468, 468, 468, 468, 468, 1018, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 468, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 474, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 475, 475, 476, 476, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, 474, -1, -1, 474, 474, 474, 474, 474, 474, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 479, 480, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 481, 482, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 278, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 512, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, -1, -1, 508, 508, 509, 510, 511, 511, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 540, 540, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 560, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 568, 568, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 514, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 515, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 587, 588, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 525, -1, -1, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 988, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 990, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, -1, 532, 532, 532, 532, 532, 532, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 534, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 534, 534, 534, 534, 534, 534, 992, 994, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 536, -1, 534, 534, 534, 534, 534, 534, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 537, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 538, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 538, 539, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 541, 542, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 543, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 544, -1, -1, -1, -1, -1, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 550, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, -1, -1, 555, 555, 555, 555, 555, 555, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 556, -1, -1, 556, -1, -1, -1, -1, -1, 556, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 559, -1, -1, 559, -1, -1, -1, -1, -1, 559, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 133, -1, -1, -1, -1, 133, 133, 133, 133, -1, -1, -1, 133, 133, 133, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 133, 133, -1, 133, 133, 133, 133, 133, 133, 133, 133, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, -1, -1, 563, 563, 563, 563, 563, 563, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 564, -1, -1, 564, -1, -1, -1, -1, -1, 564, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 567, -1, -1, 567, -1, -1, -1, -1, -1, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 133, -1, -1, -1, -1, 133, 133, 133, 133, -1, -1, -1, 133, 133, 133, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 133, 133, -1, 133, 133, 133, 133, 133, 133, 133, 133, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 570, 571, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 572, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, 115, -1, 574, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 574, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, 574, -1, -1, 574, 574, 574, 574, 574, 574, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 576, -1, -1, 576, -1, -1, -1, -1, -1, 576, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 579, -1, -1, 579, -1, -1, -1, -1, -1, 579, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 583, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 584, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, 584, -1, -1, 584, 584, 584, 584, 584, 584, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 586, -1, -1, 586, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 586, 586, 
			0, -1, 318, -1, -1, -1, -1, -1, -1, 318, -1, 318, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 318, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, 318, -1, -1, 318, 318, 318, 318, 318, 318, -1, -1, 
			-1, -1, 0, 320, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 321, 0, 0, 321, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 323, 0, 323, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 325, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 327, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 330, 0, 330, 0, 330, 330, 330, 330, 330, 0, 330, 0, 330, 0, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 
			-1, 331, 331, 331, 331, 331, 331, 331, 331, 331, 0, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 
			-1, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 0, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 333, 
			-1, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 0, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 342, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 344, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 346, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 348, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 350, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 352, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 354, 354, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 358, 358, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 358, 358, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 364, 364, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 368, 368, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 372, 372, 372, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 377, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 398, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, 401, 0, 0, 0, -1, 0, 0, 0, 401, 0, 401, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 401, 401, 401, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 409, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 422, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 428, 428, 428, 428, 428, 428, 428, 428, 428, 0, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 
			-1, -1, 432, -1, -1, -1, -1, -1, -1, 432, 0, 432, -1, 432, -1, -1, -1, -1, 432, 432, 432, 432, -1, -1, -1, 432, 432, 432, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 432, 432, -1, 432, 432, 432, 432, 432, 432, 432, 432, 432, -1, 432, -1, -1, 432, 432, 432, 432, 432, 432, -1, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, 432, -1, -1, 432, 432, 432, 432, 432, 432, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, 0, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 435, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 437, -1, -1, -1, -1, -1, -1, 437, 0, -1, -1, 437, -1, -1, -1, -1, 437, 437, 437, 437, -1, -1, -1, 437, 437, 437, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 437, 437, -1, 437, 437, 437, 437, 437, 437, 437, 437, 437, -1, 437, 437, 437, 437, 437, 437, 437, 437, 437, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 130, -1, -1, -1, -1, 130, 130, 130, 130, -1, -1, -1, 130, 130, 130, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 130, 130, -1, 130, 130, 130, 130, 130, 130, 130, 130, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 443, -1, -1, -1, -1, 443, 443, 443, 443, -1, -1, -1, 443, 443, 443, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 443, 443, -1, 443, 443, 443, 443, 443, 443, 443, 443, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 444, 0, -1, -1, -1, 444, 444, 444, 444, -1, -1, -1, 444, 444, 444, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 444, 444, -1, 444, 444, 444, 444, 444, 444, 444, 444, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 922, -1, -1, -1, -1, 922, 922, 922, 922, -1, -1, -1, 922, 922, 922, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 922, 922, -1, 922, 922, 922, 922, 922, 922, 922, 922, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 923, -1, -1, -1, -1, 923, 923, 923, 923, -1, -1, -1, 923, 923, 923, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 923, 923, -1, 923, 923, 923, 923, 923, 923, 923, 923, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 924, -1, -1, -1, -1, 924, 924, 924, 924, -1, -1, -1, 924, 924, 924, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 924, 924, -1, 924, 924, 924, 924, 924, 924, 924, 924, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 450, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 452, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 455, -1, -1, -1, -1, -1, -1, 455, 0, 455, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 455, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, -1, -1, 455, 455, 455, 455, 455, 455, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 174, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 458, -1, -1, 458, -1, -1, -1, -1, -1, 458, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 460, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 462, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 
			-1, -1, -1, -1, 467, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, 996, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, 471, -1, -1, 471, 471, 471, 471, 471, 471, -1, -1, 
			-1, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 997, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, -1, -1, 473, 473, 473, 473, 473, 473, 0, 0, 
			-1, -1, 0, 0, 0, 198, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 513, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 517, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 518, -1, -1, 518, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, 520, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, -1, -1, -1, 0, 0, -1, 521, -1, 521, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 
			-1, -1, -1, -1, 533, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 535, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 81, 951, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 79, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 953, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 545, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 956, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 957, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 124, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 549, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 554, -1, -1, -1, -1, -1, -1, -1, 0, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, -1, -1, 554, 554, 554, 554, 554, 554, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 174, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 0, -1, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, -1, -1, -1, 558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 963, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 124, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 562, -1, -1, -1, -1, -1, -1, -1, 0, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, -1, -1, 562, 562, 562, 562, 562, 562, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 174, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 0, -1, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, -1, -1, -1, 566, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 967, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 968, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 124, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 569, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 573, -1, -1, -1, -1, -1, -1, 573, 0, 573, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 573, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, -1, -1, 573, 573, 573, 573, 573, 573, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 174, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 575, -1, -1, 575, -1, -1, -1, -1, -1, 575, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 577, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, -1, -1, -1, 578, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 580, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 582, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 582, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, -1, -1, 582, 582, 582, 582, 582, 582, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 585, -1, -1, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, 998, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			
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
			230, 111,  -1,
			231, 589, 882,  -1,
			231, 590, 883, 3,  -1,
			232, 778, 121,  -1,
			591, 233, 232, 884,  -1,
			233, 779, 122,  -1,
			234, 592, 885,  -1,
			234, 593, 886, 4,  -1,
			6, 235, 27, 5,  -1,
			235, 125, 4,  -1,
			594, 887, 8, 28, 7,  -1,
			236, 780, 129,  -1,
			236, 780, 129,  -1,
			237, 781, 129,  -1,
			10, 237, 781, 129, 9,  -1,
			238, 782, 129,  -1,
			12, 238, 782, 129, 11,  -1,
			239, 783, 129,  -1,
			14, 239, 13,  -1,
			595, 888,  -1,
			784, 131,  -1,
			596, 785, 131,  -1,
			597, 130,  -1,
			240, 786, 134,  -1,
			598, 788, 131, 8, 787, 131, 15,  -1,
			241, 789, 135,  -1,
			241, 599, 790, 135, 29,  -1,
			242, 791, 136,  -1,
			242, 600, 792, 136, 30,  -1,
			243, 793, 137,  -1,
			243, 601, 794, 137, 16,  -1,
			244, 795, 138,  -1,
			244, 602, 796, 138, 17,  -1,
			245, 797, 139,  -1,
			245, 603, 798, 139, 18,  -1,
			246, 799, 140,  -1,
			246, 604, 800, 140, 889,  -1,
			605, 31,  -1,
			606, 32,  -1,
			247, 801, 142,  -1,
			247, 607, 802, 142, 890,  -1,
			608, 5,  -1,
			609, 6,  -1,
			610, 33,  -1,
			611, 34,  -1,
			248, 803, 144,  -1,
			248, 612, 804, 144, 891,  -1,
			613, 35,  -1,
			614, 36,  -1,
			249, 805, 146,  -1,
			249, 615, 806, 146, 892,  -1,
			616, 19,  -1,
			617, 20,  -1,
			250, 807, 148,  -1,
			250, 618, 808, 148, 893,  -1,
			619, 21,  -1,
			620, 22,  -1,
			621, 23,  -1,
			251, 809, 150,  -1,
			622, 810, 131, 894,  -1,
			623, 24,  -1,
			624, 37,  -1,
			625, 38,  -1,
			626, 39,  -1,
			627, 40,  -1,
			628, 41,  -1,
			629, 42,  -1,
			630, 43,  -1,
			631, 44,  -1,
			632, 45,  -1,
			633, 46,  -1,
			811, 152,  -1,
			634, 812, 150, 18,  -1,
			635, 813, 150, 21,  -1,
			814, 150, 19,  -1,
			636, 815, 150, 20,  -1,
			637, 816, 150, 25,  -1,
			638, 817, 150, 26,  -1,
			639, 818, 150, 47,  -1,
			640, 819, 150, 48,  -1,
			641, 820, 150, 14, 895, 13,  -1,
			821, 154, 13,  -1,
			253, 822, 155,  -1,
			253, 823, 156,  -1,
			824, 157,  -1,
			642, 896, 3,  -1,
			643, 897, 49,  -1,
			644, 12, 898, 11,  -1,
			645, 47,  -1,
			646, 48,  -1,
			14, 254, 647, 825, 131, 13,  -1,
			254, 648, 826, 131, 4,  -1,
			14, 13,  -1,
			649, 899,  -1,
			650, 900,  -1,
			651, 901,  -1,
			652, 902,  -1,
			653, 52,  -1,
			654, 53,  -1,
			655, 54,  -1,
			656, 903, 55,  -1,
			657, 904, 56,  -1,
			14, 827, 131, 13,  -1,
			255, 658, 905,  -1,
			255, 659, 906,  -1,
			660, 907,  -1,
			661, 14, 908, 13,  -1,
			662, 828, 155,  -1,
			829, 160,  -1,
			664, 910, 256, 663, 909,  -1,
			256, 830, 161,  -1,
			831, 160,  -1,
			665, 911,  -1,
			667, 913, 257, 666, 912,  -1,
			257, 116,  -1,
			2, 130,  -1,
			670, 258, 117, 668, 14, 914, 13, 58,  -1,
			117, 669, 59,  -1,
			10, 259, 9, 14, 130, 13, 60,  -1,
			259, 172,  -1,
			8, 133, 61,  -1,
			8, 62,  -1,
			672, 832, 173, 671, 916, 915, 13, 63,  -1,
			675, 2, 674, 14, 918, 13, 63, 673, 833, 173, 917,  -1,
			680, 834, 173, 920, 262, 2, 261, 2, 260, 13, 65,  -1,
			677, 919, 676,  -1,
			679, 130, 678,  -1,
			682, 921, 681,  -1,
			683, 2, 263, 66,  -1,
			684, 2, 264, 67,  -1,
			685, 2, 265, 68,  -1,
			12, 175, 11,  -1,
			266, 176, 4,  -1,
			266, 176,  -1,
			128, 24,  -1,
			686, 267, 925,  -1,
			688, 10, 268, 9, 687, 927, 926,  -1,
			268, 111,  -1,
			2, 118, 70,  -1,
			836, 179, 270, 928, 269,  -1,
			835, 929,  -1,
			271, 689, 837, 930,  -1,
			271, 690, 838, 931, 4,  -1,
			272, 691, 932,  -1,
			693, 839, 933, 692, 24,  -1,
			695, 934,  -1,
			696, 935,  -1,
			697, 10, 840, 183, 9,  -1,
			274, 841, 184,  -1,
			274, 842, 184, 4,  -1,
			843, 182,  -1,
			698, 936,  -1,
			275, 844, 186,  -1,
			275, 844, 186,  -1,
			276, 845, 154,  -1,
			276, 845, 154,  -1,
			846, 154,  -1,
			847, 187,  -1,
			848, 188,  -1,
			849, 189,  -1,
			850, 190,  -1,
			699, 71,  -1,
			700, 72,  -1,
			701, 73,  -1,
			702, 74,  -1,
			703, 75,  -1,
			704, 76,  -1,
			705, 77,  -1,
			706, 78,  -1,
			707, 79,  -1,
			708, 80,  -1,
			709, 81,  -1,
			710, 82,  -1,
			711, 83,  -1,
			712, 84,  -1,
			713, 85,  -1,
			714, 86,  -1,
			715, 87,  -1,
			716, 88,  -1,
			717, 89,  -1,
			718, 90,  -1,
			719, 91,  -1,
			720, 92,  -1,
			721, 93,  -1,
			722, 94,  -1,
			723, 95,  -1,
			724, 96,  -1,
			725, 97,  -1,
			726, 98,  -1,
			727, 99,  -1,
			728, 937,  -1,
			729, 938,  -1,
			730, 939,  -1,
			731, 940,  -1,
			732, 941,  -1,
			733, 277, 942,  -1,
			735, 943, 734,  -1,
			736, 944,  -1,
			6, 279, 119, 5,  -1,
			279, 119, 4,  -1,
			280, 851, 121,  -1,
			282, 281, 737, 945, 280,  -1,
			738, 946, 3,  -1,
			282, 852, 122,  -1,
			283, 739, 947,  -1,
			283, 853, 190,  -1,
			740, 21,  -1,
			741, 18,  -1,
			854, 201,  -1,
			855, 202,  -1,
			742, 12, 948, 11,  -1,
			743, 12, 11,  -1,
			14, 856, 203, 13,  -1,
			14, 13,  -1,
			284, 857, 204,  -1,
			284, 858, 204, 4,  -1,
			744, 285, 950, 949,  -1,
			128, 24,  -1,
			745, 100, 286,  -1,
			206, 8, 101, 7,  -1,
			746, 287,  -1,
			747, 80,  -1,
			859, 208, 748, 288, 952,  -1,
			749, 102,  -1,
			750, 103,  -1,
			752, 10, 860, 209, 751, 9,  -1,
			289, 861, 210,  -1,
			289, 862, 210, 4,  -1,
			955, 24,  -1,
			753, 290, 954,  -1,
			863, 212, 754, 294, 293, 292, 291, 104,  -1,
			958, 8,  -1,
			755, 14, 959, 13, 28,  -1,
			756, 960,  -1,
			757, 62,  -1,
			759, 10, 295, 758, 9,  -1,
			295, 864, 213,  -1,
			2, 865, 214, 961, 296,  -1,
			297, 866, 215,  -1,
			297, 867, 215, 4,  -1,
			216, 8,  -1,
			760, 298, 962,  -1,
			868, 217, 300, 761, 299, 105,  -1,
			763, 10, 301, 762, 9,  -1,
			301, 869, 218,  -1,
			2, 870, 219, 964, 302,  -1,
			303, 871, 220,  -1,
			303, 872, 220, 4,  -1,
			221, 8,  -1,
			764, 304, 965,  -1,
			873, 223, 765, 308, 307, 306, 305, 966,  -1,
			969, 8,  -1,
			766, 106,  -1,
			767, 107,  -1,
			769, 10, 309, 768, 9,  -1,
			309, 874, 224,  -1,
			185, 311, 970, 310,  -1,
			875, 225,  -1,
			312, 876, 226,  -1,
			312, 877, 226, 4,  -1,
			216, 8,  -1,
			314, 770, 313, 971,  -1,
			772, 878, 182, 771, 24,  -1,
			773, 972, 84,  -1,
			315, 879, 228,  -1,
			10, 315, 879, 228, 9,  -1,
			774, 185, 974, 973,  -1,
			316, 880, 121,  -1,
			775, 881, 202, 975, 316,  -1,
			776, 108,  -1,
			777, 109,  -1,
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 7, 12, 16, 21, 25, 29, 34, 39, 43, 49, 53, 57, 61, 67, 71, 77, 81, 85, 88, 91, 95, 98, 102, 110, 114, 120, 124, 130, 134, 140, 144, 150, 154, 160, 164, 170, 173, 176, 180, 186, 189, 192, 195, 198, 202, 208, 211, 214, 218, 224, 227, 230, 234, 240, 243, 246, 249, 253, 258, 261, 264, 267, 270, 273, 276, 279, 282, 285, 288, 291, 294, 299, 304, 308, 313, 318, 323, 328, 333, 340, 344, 348, 352, 355, 359, 363, 368, 371, 374, 381, 387, 390, 393, 396, 399, 402, 405, 408, 411, 415, 419, 424, 428, 432, 435, 440, 444, 447, 453, 457, 460, 463, 469, 472, 475, 484, 488, 496, 499, 503, 506, 515, 527, 539, 543, 547, 551, 556, 561, 566, 570, 574, 577, 580, 584, 592, 595, 599, 605, 608, 613, 619, 623, 629, 632, 635, 641, 645, 650, 653, 656, 660, 664, 668, 672, 675, 678, 681, 684, 687, 690, 693, 696, 699, 702, 705, 708, 711, 714, 717, 720, 723, 726, 729, 732, 735, 738, 741, 744, 747, 750, 753, 756, 759, 762, 765, 768, 771, 774, 777, 780, 783, 786, 789, 793, 797, 800, 805, 809, 813, 819, 823, 827, 831, 835, 838, 841, 844, 847, 852, 856, 861, 864, 868, 873, 878, 881, 885, 890, 893, 896, 902, 905, 908, 915, 919, 924, 927, 931, 940, 943, 949, 952, 955, 961, 965, 971, 975, 980, 983, 987, 994, 1000, 1004, 1010, 1014, 1019, 1022, 1026, 1035, 1038, 1041, 1044, 1050, 1054, 1059, 1062, 1066, 1071, 1074, 1079, 1085, 1089, 1093, 1099, 1104, 1108, 1114, 1117, 
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
		
		case ',':
			return 4;
		
		case '<':
			return 5;
		
		case '>':
			return 6;
		
		case '#':
			return 7;
		
		case ':':
			return 8;
		
		case '{':
			return 9;
		
		case '}':
			return 10;
		
		case '[':
			return 11;
		
		case ']':
			return 12;
		
		case '(':
			return 13;
		
		case ')':
			return 14;
		
		case '?':
			return 15;
		
		case '|':
			return 16;
		
		case '^':
			return 17;
		
		case '&':
			return 18;
		
		case '+':
			return 19;
		
		case '-':
			return 20;
		
		case '*':
			return 21;
		
		case '/':
			return 22;
		
		case '%':
			return 23;
		
		case '=':
			return 24;
		
		case '~':
			return 25;
		
		case '!':
			return 26;
		
		case EToken_Identifier:
			return 27;
		
		case EToken_Pack:
			return 28;
		
		case EToken_LogicalOr:
			return 29;
		
		case EToken_LogicalAnd:
			return 30;
		
		case EToken_Eq:
			return 31;
		
		case EToken_Ne:
			return 32;
		
		case EToken_Le:
			return 33;
		
		case EToken_Ge:
			return 34;
		
		case EToken_Shl:
			return 35;
		
		case EToken_Shr:
			return 36;
		
		case EToken_AddAssign:
			return 37;
		
		case EToken_SubAssign:
			return 38;
		
		case EToken_MulAssign:
			return 39;
		
		case EToken_DivAssign:
			return 40;
		
		case EToken_ModAssign:
			return 41;
		
		case EToken_ShlAssign:
			return 42;
		
		case EToken_ShrAssign:
			return 43;
		
		case EToken_AndAssign:
			return 44;
		
		case EToken_XorAssign:
			return 45;
		
		case EToken_OrAssign:
			return 46;
		
		case EToken_Inc:
			return 47;
		
		case EToken_Dec:
			return 48;
		
		case EToken_Ptr:
			return 49;
		
		case EToken_Integer:
			return 50;
		
		case EToken_Fp:
			return 51;
		
		case EToken_True:
			return 52;
		
		case EToken_False:
			return 53;
		
		case EToken_Null:
			return 54;
		
		case EToken_SizeOf:
			return 55;
		
		case EToken_TypeOf:
			return 56;
		
		case EToken_Literal:
			return 57;
		
		case EToken_If:
			return 58;
		
		case EToken_Else:
			return 59;
		
		case EToken_Switch:
			return 60;
		
		case EToken_Case:
			return 61;
		
		case EToken_Default:
			return 62;
		
		case EToken_While:
			return 63;
		
		case EToken_Do:
			return 64;
		
		case EToken_For:
			return 65;
		
		case EToken_Break:
			return 66;
		
		case EToken_Continue:
			return 67;
		
		case EToken_Return:
			return 68;
		
		case EToken_Namespace:
			return 69;
		
		case EToken_Using:
			return 70;
		
		case EToken_Public:
			return 71;
		
		case EToken_Private:
			return 72;
		
		case EToken_Static:
			return 73;
		
		case EToken_Typedef:
			return 74;
		
		case EToken_Const:
			return 75;
		
		case EToken_Volatile:
			return 76;
		
		case EToken_Signed:
			return 77;
		
		case EToken_Unsigned:
			return 78;
		
		case EToken_LittleEndian:
			return 79;
		
		case EToken_BigEndian:
			return 80;
		
		case EToken_Safe:
			return 81;
		
		case EToken_Unsafe:
			return 82;
		
		case EToken_NoNull:
			return 83;
		
		case EToken_Property:
			return 84;
		
		case EToken_Void:
			return 85;
		
		case EToken_Variant:
			return 86;
		
		case EToken_Bool:
			return 87;
		
		case EToken_Int:
			return 88;
		
		case EToken_Char:
			return 89;
		
		case EToken_Short:
			return 90;
		
		case EToken_Long:
			return 91;
		
		case EToken_Float:
			return 92;
		
		case EToken_Double:
			return 93;
		
		case EToken_Int8:
			return 94;
		
		case EToken_Int16:
			return 95;
		
		case EToken_Int32:
			return 96;
		
		case EToken_Int64:
			return 97;
		
		case EToken_WChar:
			return 98;
		
		case EToken_Auto:
			return 99;
		
		case EToken_Ellipsis:
			return 100;
		
		case EToken_Endian:
			return 101;
		
		case EToken_Enum:
			return 102;
		
		case EToken_EnumC:
			return 103;
		
		case EToken_Struct:
			return 104;
		
		case EToken_Union:
			return 105;
		
		case EToken_Interface:
			return 106;
		
		case EToken_Class:
			return 107;
		
		case EToken_Get:
			return 108;
		
		case EToken_Set:
			return 109;
		
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
			',', 
			'<', 
			'>', 
			'#', 
			':', 
			'{', 
			'}', 
			'[', 
			']', 
			'(', 
			')', 
			'?', 
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
			EToken_Identifier, 
			EToken_Pack, 
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
			EToken_Ptr, 
			EToken_Integer, 
			EToken_Fp, 
			EToken_True, 
			EToken_False, 
			EToken_Null, 
			EToken_SizeOf, 
			EToken_TypeOf, 
			EToken_Literal, 
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
			EToken_NoNull, 
			EToken_Property, 
			EToken_Void, 
			EToken_Variant, 
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
			EToken_Auto, 
			EToken_Ellipsis, 
			EToken_Endian, 
			EToken_Enum, 
			EToken_EnumC, 
			EToken_Struct, 
			EToken_Union, 
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
			_T("type_name_list"),
			_T("generic_formal_argument_list"),
			_T("generic_formal_argument"),
			_T("pragma_pack"),
			_T("struct_pack_factor_value"),
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
			_T("call_operator"),
			_T("literal"),
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
			_T("loop_stmt"),
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
			_T("union_specifier"),
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
			_T("struct_pack_factor"),
			_T("struct_block"),
			_T("struct_member_declaration"),
			_T("struct_member_declarator_list"),
			_T("struct_member_declarator"),
			_T("struct_member_bit_field"),
			_T("union_block"),
			_T("union_member_declaration"),
			_T("union_member_declarator_list"),
			_T("union_member_declarator"),
			_T("union_member_bit_field"),
			_T("class_kind"),
			_T("class_block"),
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls16);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls22);
				
			break;
			
		
		case ESymbol_pointer:
			pNode = AXL_MEM_NEW (CSymbolNode_pointer);
			
			break;
			
		
		case ESymbol_declarator_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_suffix);
			
			break;
			
		
		case ESymbol_type_name_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls3);
				
			break;
			
		
		
		
		
		case ESymbol_struct_pack_factor_value:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls30);
				
			break;
			
		
		case ESymbol_expression_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls4);
				
			break;
			
		
		case ESymbol_primary_expr_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_primary_expr_pass1);
			
			break;
			
		
		case ESymbol_expression:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls5);
				
			break;
			
		
		case ESymbol_conditional_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_conditional_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_constant_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls6);
				
			break;
			
		
		case ESymbol_constant_integer_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls7);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls8);
				
			break;
			
		
		case ESymbol_shift_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_shift_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_relational_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls9);
				
			break;
			
		
		case ESymbol_additive_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_additive_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_shift_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls10);
				
			break;
			
		
		case ESymbol_multiplicative_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_multiplicative_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_additivie_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls11);
				
			break;
			
		
		case ESymbol_assignment_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_assignment_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_multiplicative_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls12);
				
			break;
			
		
		case ESymbol_unary_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_unary_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_assignment_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls13);
				
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
			
		
		case ESymbol_call_operator:
			pNode = AXL_MEM_NEW (CSymbolNode_call_operator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_literal:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls14);
				
			break;
			
		
		case ESymbol_type_name_or_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_type_name_or_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls15);
			
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
			
		
		
		case ESymbol_if_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_if_stmt);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		case ESymbol_while_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_while_stmt);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_do_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_do_stmt);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_for_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_for_stmt);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		
		
		
		case ESymbol_loop_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_loop_stmt);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls21);
				
			break;
			
		
		case ESymbol_init_declarator_list:
			pNode = AXL_MEM_NEW (CSymbolNode_init_declarator_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls18);
			
			break;
			
		
		case ESymbol_declaration_terminator_2:
			pNode = AXL_MEM_NEW (CSymbolNode_declaration_terminator_2);
			
			break;
			
		
		case ESymbol_init_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_init_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls19);
			
			break;
			
		
		case ESymbol_declarator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls24);
				
			break;
			
		
		case ESymbol_initializer:
			pNode = AXL_MEM_NEW (CSymbolNode_initializer);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls20);
			
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls26);
				
			break;
			
		
		case ESymbol_struct_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls28);
				
			break;
			
		
		case ESymbol_union_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls31);
				
			break;
			
		
		case ESymbol_class_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls32);
				
			break;
			
		
		case ESymbol_property_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls34);
				
			break;
			
		
		case ESymbol_qualified_type_name_rslv:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			break;
			
		
		case ESymbol_qualified_type_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls23);
				
			break;
			
		
		
		case ESymbol_property_accessor_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls37);
				
			break;
			
		
		case ESymbol_pointer_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls25);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls27);
				
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
			
		
		case ESymbol_struct_pack_factor:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls29);
				
			break;
			
		
		case ESymbol_struct_block:
			pNode = AXL_MEM_NEW (CSymbolNode_struct_block);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
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
			
		
		
		case ESymbol_union_block:
			pNode = AXL_MEM_NEW (CSymbolNode_union_block);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_union_member_declaration:
			pNode = AXL_MEM_NEW (CSymbolNode_union_member_declaration);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_union_member_declarator_list:
			pNode = AXL_MEM_NEW (CSymbolNode_union_member_declarator_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_union_member_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_union_member_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		case ESymbol_class_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls33);
				
			break;
			
		
		case ESymbol_class_block:
			pNode = AXL_MEM_NEW (CSymbolNode_class_block);
			
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls36);
				
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
			
			{ 0, 120 },
			
			{ 0, 119 },
			
			{ 1, 119 },
			
			{ 0, 127 },
			
			{ 0, 1 },
			
			{ 0, 141 },
			
			{ 0, 143 },
			
			{ 0, 145 },
			
			{ 0, 147 },
			
			{ 0, 149 },
			
			{ 0, 151 },
			
			{ 0, 119 },
			
			{ 0, 27 },
			
			{ 0, 27 },
			
			{ 0, 130 },
			
			{ 0, 27 },
			
			{ 0, 50 },
			
			{ 0, 51 },
			
			{ 0, 158 },
			
			{ 0, 159 },
			
			{ 0, 159 },
			
			{ 0, 57 },
			
			{ 1, 57 },
			
			{ 0, 119 },
			
			{ 0, 119 },
			
			{ 0, 9 },
			
			{ 1, 10 },
			
			{ 0, 1 },
			
			{ 0, 9 },
			
			{ 1, 10 },
			
			{ 0, 130 },
			
			{ 0, 130 },
			
			{ 1, 14 },
			
			{ 1, 64 },
			
			{ 0, 130 },
			
			{ 0, 130 },
			
			{ 1, 14 },
			
			{ 0, 117 },
			
			{ 0, 133 },
			
			{ 0, 133 },
			
			{ 0, 130 },
			
			{ 0, 27 },
			
			{ 0, 69 },
			
			{ 1, 118 },
			
			{ 0, 177 },
			
			{ 1, 178 },
			
			{ 0, 180 },
			
			{ 1, 180 },
			
			{ 0, 181 },
			
			{ 1, 182 },
			
			{ 0, 128 },
			
			{ 0, 130 },
			
			{ 0, 115 },
			
			{ 0, 191 },
			
			{ 0, 192 },
			
			{ 0, 193 },
			
			{ 0, 194 },
			
			{ 0, 195 },
			
			{ 0, 197 },
			
			{ 0, 118 },
			
			{ 0, 118 },
			
			{ 0, 27 },
			
			{ 1, 199 },
			
			{ 0, 200 },
			
			{ 0, 50 },
			
			{ 0, 120 },
			
			{ 1, 181 },
			
			{ 0, 82 },
			
			{ 0, 207 },
			
			{ 1, 27 },
			
			{ 0, 27 },
			
			{ 1, 128 },
			
			{ 2, 211 },
			
			{ 0, 27 },
			
			{ 1, 123 },
			
			{ 0, 127 },
			
			{ 0, 50 },
			
			{ 0, 120 },
			
			{ 0, 181 },
			
			{ 0, 27 },
			
			{ 0, 120 },
			
			{ 0, 181 },
			
			{ 0, 222 },
			
			{ 3, 211 },
			
			{ 1, 27 },
			
			{ 2, 123 },
			
			{ 0, 177 },
			
			{ 0, 181 },
			
			{ 0, 227 },
			
			{ 0, 120 },
			
			{ 1, 229 },
			
			{ 0, 199 },
			
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
#line 4370 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls1* __pAstNode = (_cls1*) __pSymbol->m_pAstNode;
#line 29 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			(*__pAstNode).m_Name.m_List.InsertTail ((*GetTokenLocator (1)).m_Data.m_String);
		;
#line 4383 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
#line 49 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_pType = __pSymbol->m_Local.Declarator.GetType (&(*(_cls22*) GetAstLocator (0)).m_TypeSpecifier);
		;
#line 4396 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
#line 60 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_TypeList.InsertTail ((*(_cls2*) GetAstLocator (0)).m_pType);
		;
#line 4409 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
#line 64 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_TypeList.InsertTail ((*(_cls2*) GetAstLocator (1)).m_pType);
		;
#line 4422 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 5:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 91 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			m_StructPackFactor = (*(_cls30*) GetAstLocator (0)).m_Value;
		;
#line 4435 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 6:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
#line 4448 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 7:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
#line 40 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			if ((*__pAstNode).m_Value.GetValueKind () != EValue_Const)
			{
				err::SetFormatStringError (_T("not a constant expression"));
				return false;
			}
		;
#line 4465 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 8:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
#line 56 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_Value = 10;
		;
#line 4478 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 9:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 72 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.ConditionalOperator (__pSymbol->m_Arg.pValue, __pSymbol->m_Local.TrueValue, __pSymbol->m_Local.FalseValue);
			;
#line 4491 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 10:
			{
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 86 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogicalAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4504 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 11:
			{
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 100 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogicalOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4517 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 12:
			{
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 114 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BitwiseOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4530 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 13:
			{
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 128 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BitwiseXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4543 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 14:
			{
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BitwiseAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4556 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 15:
			{
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 156 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls8*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4569 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 16:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 168 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Eq;
		;
#line 4582 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 17:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 172 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ne;
		;
#line 4595 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 18:
			{
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 185 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls9*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4608 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 19:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 197 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Lt;
		;
#line 4621 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 20:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 201 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Gt;
		;
#line 4634 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 21:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 205 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Le;
		;
#line 4647 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 22:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 209 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ge;
		;
#line 4660 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 23:
			{
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 222 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls10*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4673 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 24:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
#line 234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shl;
		;
#line 4686 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 25:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
#line 238 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shr;
		;
#line 4699 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 26:
			{
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 251 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls11*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4712 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 27:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
#line 263 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Add;
		;
#line 4725 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 28:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
#line 267 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Sub;
		;
#line 4738 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 29:
			{
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 280 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls12*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 4751 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 30:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 292 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mul;
		;
#line 4764 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 31:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 296 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Div;
		;
#line 4777 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 32:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
#line 300 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mod;
		;
#line 4790 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 33:
			{
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 313 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.MoveOperator (__pSymbol->m_Local.RValue, *__pSymbol->m_Arg.pValue, (*(_cls13*) GetAstLocator (0)).m_OpKind);
			;
#line 4803 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 34:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 325 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_None;
		;
#line 4816 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 35:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 329 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Add;
		;
#line 4829 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 36:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 333 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Sub;
		;
#line 4842 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 37:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 337 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mul;
		;
#line 4855 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 38:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 341 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Div;
		;
#line 4868 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 39:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 345 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mod;
		;
#line 4881 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 40:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 349 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shl;
		;
#line 4894 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 41:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 353 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shr;
		;
#line 4907 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 42:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 357 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_BitwiseAnd;
		;
#line 4920 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 43:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 361 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_BitwiseXor;
		;
#line 4933 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 44:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 365 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_BitwiseOr;
		;
#line 4946 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 45:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 373 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, __pSymbol->m_Arg.pValue);
		;
#line 4959 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 46:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 377 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, __pSymbol->m_Arg.pValue);
		;
#line 4972 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 47:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 382 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Minus, __pSymbol->m_Arg.pValue);
		;
#line 4985 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 48:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 386 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_BitwiseNot, __pSymbol->m_Arg.pValue);
		;
#line 4998 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 49:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 390 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_LogicalNot, __pSymbol->m_Arg.pValue);
		;
#line 5011 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 50:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 394 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
		;
#line 5024 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 51:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 398 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
		;
#line 5037 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 52:
			{
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 403 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.CastOperator (__pSymbol->m_Arg.pValue, (*(_cls2*) GetAstLocator (0)).m_pType);
		;
#line 5050 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 53:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 427 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.MemberOperator (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
		;
#line 5063 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 54:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 431 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.PointerToMemberOperator (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
		;
#line 5076 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 55:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 435 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls5*) GetAstLocator (0)).m_Value);
		;
#line 5089 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 56:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 439 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
		;
#line 5102 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 57:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 443 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
		;
#line 5115 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 58:
			{
			CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 460 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Local.ArgList.InsertTail (__pSymbol->m_Local.Arg);
		;
#line 5128 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 59:
			{
			CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 466 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Local.ArgList.InsertTail (__pSymbol->m_Local.Arg);
			;
#line 5141 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 60:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 477 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return LookupIdentifier ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
		;
#line 5154 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 61:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 481 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstUInt64 ((*GetTokenLocator (0)).m_Data.m_UInt64);
		;
#line 5167 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 62:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 485 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstDouble ((*GetTokenLocator (0)).m_Data.m_Double);
		;
#line 5180 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 63:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 489 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetLiteral ((*(_cls14*) GetAstLocator (0)).m_String);
		;
#line 5193 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 64:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 493 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstBool (true);
		;
#line 5206 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 65:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 497 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstBool (false);
		;
#line 5219 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 66:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 501 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetNull ();
		;
#line 5232 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 67:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 505 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstSizeT ((*(_cls15*) GetAstLocator (0)).m_pType->GetSize ());
		;
#line 5245 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 68:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 509 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetType ((*(_cls15*) GetAstLocator (0)).m_pType);
		;
#line 5258 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 69:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
#line 521 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_String = (*GetTokenLocator (0)).m_Data.m_String;
		;
#line 5271 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 70:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
#line 525 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_String.Append ((*GetTokenLocator (1)).m_Data.m_String);
		;
#line 5284 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 71:
			{
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
#line 540 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_pType = (*(_cls2*) GetAstLocator (0)).m_pType;
		;
#line 5297 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 72:
			{
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
#line 544 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_pType = (*(_cls2*) GetAstLocator (0)).m_pType;
		;
#line 5310 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 73:
			{
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
#line 548 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			CType* pType = __pSymbol->m_Local.Value.GetType ();
			(*__pAstNode).m_pType = pType->IsReferenceType () ? ((CPointerType*) pType)->GetBaseType () : pType;
		;
#line 5324 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 74:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 14 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
#line 5337 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 75:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
		;
#line 5350 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 76:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 28 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
#line 5363 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 77:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CCompoundStmt* __pAstNode = (CCompoundStmt*) __pSymbol->m_pAstNode;
#line 42 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			(*__pAstNode).m_pScope = m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
		;
#line 5376 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 78:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CCompoundStmt* __pAstNode = (CCompoundStmt*) __pSymbol->m_pAstNode;
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
		;
#line 5389 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 79:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 84 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.ConditionalJump ((*(_cls5*) GetAstLocator (0)).m_Value, __pSymbol->m_Local.pThenBlock, __pSymbol->m_Local.pElseBlock);
		;
#line 5402 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 80:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 90 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				__pSymbol->m_Local.pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("if_follow"));
				m_pModule->m_ControlFlowMgr.Jump (__pSymbol->m_Local.pFollowBlock, __pSymbol->m_Local.pElseBlock);
			;
#line 5416 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 81:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 96 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
			m_pModule->m_ControlFlowMgr.Follow (__pSymbol->m_Local.pFollowBlock);
		;
#line 5429 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 82:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 126 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.ConditionalJump ((*(_cls5*) GetAstLocator (0)).m_Value, __pSymbol->m_Local.pBodyBlock, __pSymbol->m_Local.pFollowBlock);
		;
#line 5442 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 83:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.Jump (__pSymbol->m_Local.pConditionBlock, __pSymbol->m_Local.pFollowBlock);
		;
#line 5455 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 84:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 150 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.Follow (__pSymbol->m_Local.pConditionBlock);
		;
#line 5468 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 85:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 154 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.ConditionalJump ((*(_cls5*) GetAstLocator (0)).m_Value, __pSymbol->m_Local.pBodyBlock, __pSymbol->m_Local.pFollowBlock);
		;
#line 5481 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 86:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 158 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
			m_pModule->m_ControlFlowMgr.SetCurrentBlock (__pSymbol->m_Local.pFollowBlock);
		;
#line 5494 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 87:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 181 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				__pSymbol->m_Local.pConditionBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("for_condition"));
				__pSymbol->m_Local.pLoopBlock = __pSymbol->m_Local.pConditionBlock;
				m_pModule->m_ControlFlowMgr.Follow (__pSymbol->m_Local.pConditionBlock);
			;
#line 5509 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 88:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 187 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.ConditionalJump ((*(_cls5*) GetAstLocator (0)).m_Value, __pSymbol->m_Local.pBodyBlock, __pSymbol->m_Local.pFollowBlock);
			;
#line 5522 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 89:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 192 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				__pSymbol->m_Local.pLoopBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("for_loop"));
				m_pModule->m_ControlFlowMgr.SetCurrentBlock (__pSymbol->m_Local.pLoopBlock);
			;
#line 5536 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 90:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 197 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.Jump (__pSymbol->m_Local.pConditionBlock, __pSymbol->m_Local.pBodyBlock);
			;
#line 5549 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 91:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 203 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.Jump (__pSymbol->m_Local.pLoopBlock, __pSymbol->m_Local.pFollowBlock);
		;
#line 5562 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 92:
			{
			CSymbolNode_loop_stmt* __pSymbol = (CSymbolNode_loop_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 210 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
						
			m_pModule->m_NamespaceMgr.OpenScope (*__pSymbol->m_Arg.pPos, __pSymbol->m_Arg.pBreakBlock, __pSymbol->m_Arg.pContinueBlock);
		;
#line 5575 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 93:
			{
			CSymbolNode_loop_stmt* __pSymbol = (CSymbolNode_loop_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 214 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.CloseScope ((*GetAstLocator (0)).m_LastToken.m_Pos);
		;
#line 5588 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 94:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.Break (IsValidLocator ((*(_cls7*) GetAstLocator (0))) ? (*(_cls7*) GetAstLocator (0)).m_Value : 1);
		;
#line 5601 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 95:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 228 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.Continue (IsValidLocator ((*(_cls7*) GetAstLocator (0))) ? (*(_cls7*) GetAstLocator (0)).m_Value : 1);
		;
#line 5614 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 96:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 235 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.Return (IsValidLocator ((*(_cls5*) GetAstLocator (0))) ? (*(_cls5*) GetAstLocator (0)).m_Value : CValue ());
		;
#line 5627 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 97:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_AttributeBlock.llk"
			
			CAttribute* pAttribute = m_pModule->m_AttributeMgr.CreateAttribute ((*GetTokenLocator (0)).m_Data.m_String, NULL);
			if (!pAttribute)
				return false;

			pAttribute->m_Pos = (*GetTokenLocator (0)).m_Pos;
		;
#line 5644 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 98:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 5 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamespaceDeclaration.llk"
			 
			CNamespace* pNamespace = m_pModule->m_NamespaceMgr.OpenNamespace ((*GetTokenLocator (0)).m_Pos, (*(_cls1*) GetAstLocator (1)).m_Name);
			if (!pNamespace)
				return false;
		;
#line 5659 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 99:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 11 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamespaceDeclaration.llk"
			 
			m_pModule->m_NamespaceMgr.CloseNamespace ((*(_cls1*) GetAstLocator (1)).m_Name.m_List.GetCount () + 1);
		;
#line 5672 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 100:
			{
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 16 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_pItem = (*(_cls19*) GetAstLocator (0)).m_pItem;
		;
#line 5685 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 101:
			{
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 20 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_pItem = (*(_cls19*) GetAstLocator (1)).m_pItem;
		;
#line 5698 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 102:
			{
			CSymbolNode_init_declarator* __pSymbol = (CSymbolNode_init_declarator*) GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
#line 32 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_pItem = Declare (__pSymbol->m_Arg.pDeclSpecifiers, &(*(_cls24*) GetAstLocator (0)).m_Declarator);
			if (!(*__pAstNode).m_pItem)
				return false;
		;
#line 5713 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 103:
			{
			CSymbolNode_init_declarator* __pSymbol = (CSymbolNode_init_declarator*) GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
#line 38 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			if ((*__pAstNode).m_pItem->GetItemKind () != EModuleItem_Variable)
			{
				err::SetFormatStringError (_T("cannot apply initializer to '%s'"), (*__pAstNode).m_pItem->GetItemKindString ());
				return false;
			}
		;
#line 5730 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 104:
			{
			CSymbolNode_init_declarator* __pSymbol = (CSymbolNode_init_declarator*) GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
#line 46 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			if (m_Stage == EStage_Pass2)
				return m_pModule->m_OperatorMgr.MoveOperator ((*(_cls20*) GetAstLocator (1)).m_Value, (CVariable*) (*__pAstNode).m_pItem);
		;
#line 5744 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 105:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
#line 61 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			 return m_Stage == EStage_Pass1 ;
#line 5755 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 106:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
#line 62 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_TokenList.TakeOver (&(*(_cls4*) GetAstLocator (0)).m_TokenList);
		;
#line 5768 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 107:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
#line 66 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_Value = (*(_cls5*) GetAstLocator (0)).m_Value;
		;
#line 5781 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 108:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
#line 70 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
		;
#line 5793 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 109:
			{
			CSymbolNode_declaration_terminator_2* __pSymbol = (CSymbolNode_declaration_terminator_2*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 89 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return SetFunctionBody (__pSymbol->m_Arg.pItem, &(*(_cls16*) GetAstLocator (0)).m_TokenList);
		;
#line 5806 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 110:
			{
			CSymbolNode_access_specifier* __pSymbol = (CSymbolNode_access_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 148 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pAccessSpecifier->SetAccess (EAccess_Public);
		;
#line 5819 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 111:
			{
			CSymbolNode_access_specifier* __pSymbol = (CSymbolNode_access_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 152 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pAccessSpecifier->SetAccess (EAccess_Private);
		;
#line 5832 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 112:
			{
			CSymbolNode_storage_class_specifier* __pSymbol = (CSymbolNode_storage_class_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 159 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pDeclSpecifiers->SetStorageClass (EStorageClass_Static);
		;
#line 5845 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 113:
			{
			CSymbolNode_storage_class_specifier* __pSymbol = (CSymbolNode_storage_class_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 163 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pDeclSpecifiers->SetStorageClass (EStorageClass_Typedef);
		;
#line 5858 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 114:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 170 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Const);
		;
#line 5871 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 115:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 174 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Volatile);
		;
#line 5884 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 116:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 178 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Signed);
		;
#line 5897 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 117:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Unsigned);
		;
#line 5910 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 118:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 186 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_LittleEndian);
		;
#line 5923 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 119:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 190 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_BigEndian);
		;
#line 5936 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 120:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 194 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Safe);
		;
#line 5949 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 121:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 198 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Unsafe);
		;
#line 5962 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 122:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_NoNull);
		;
#line 5975 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 123:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 206 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Property);
		;
#line 5988 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 124:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 213 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Void));
		;
#line 6001 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 125:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 217 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Variant));
		;
#line 6014 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 126:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Bool));
		;
#line 6027 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 127:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 225 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int32_be : EType_Int));
		;
#line 6040 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 128:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 229 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Char));
		;
#line 6053 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 129:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 233 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int16_be : EType_Short));
		;
#line 6066 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 130:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 237 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int32_be : EType_Long));
		;
#line 6079 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 131:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 241 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Float));
		;
#line 6092 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 132:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 245 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Double));
		;
#line 6105 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 133:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 249 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int8));
		;
#line 6118 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 134:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 253 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int16_be : EType_Int16));
		;
#line 6131 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 135:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 257 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int32_be : EType_Int32));
		;
#line 6144 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 136:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 261 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (m_Endianness == EEndianness_BigEndian ? EType_Int64_be : EType_Int64));
		;
#line 6157 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 137:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 265 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_WChar));
		;
#line 6170 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 138:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 269 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			err::SetStringError (_T("'auto' type specifier is not yet supported"));
			return false; 
		;
#line 6184 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 139:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls26*) GetAstLocator (0)).m_pType);
		;
#line 6197 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 140:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 278 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls28*) GetAstLocator (0)).m_pType);
		;
#line 6210 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 141:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls31*) GetAstLocator (0)).m_pType);
		;
#line 6223 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 142:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 286 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls32*) GetAstLocator (0)).m_pType);
		;
#line 6236 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 143:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 290 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetProperty ((*(_cls34*) GetAstLocator (0)).m_pProperty);
		;
#line 6249 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 144:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 296 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls23*) GetAstLocator (0)).m_pType);
		;
#line 6262 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 145:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 305 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			if (IsTypeSpecified ())
				return false;
		;
#line 6276 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 146:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 310 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return m_Stage == EStage_Pass1 || FindType ((*(_cls1*) GetAstLocator (0)).m_Name) != NULL;
		;
#line 6289 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 147:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls23* __pAstNode = (_cls23*) __pSymbol->m_pAstNode;
#line 321 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			CType* pType = FindType ((*(_cls1*) GetAstLocator (0)).m_Name);
			if (!pType)
			{
				err::SetFormatStringError (_T("'%s' is not a type"), (*(_cls1*) GetAstLocator (0)).m_Name.GetFullName ());
				return false;
			}

			(*__pAstNode).m_pType = pType;
		;
#line 6309 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 148:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
#line 346 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_Declarator.m_Pos = (*GetTokenLocator (0)).m_Pos;
			(*__pAstNode).m_Declarator.m_Name = (*GetTokenLocator (0)).m_Data.m_String;
		;
#line 6323 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 149:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
#line 351 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_Declarator.m_PropertyAccessorKind = (*(_cls37*) GetAstLocator (1)).m_PropertyAccessorKind;
		;
#line 6336 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 150:
			{
			CSymbolNode_pointer* __pSymbol = (CSymbolNode_pointer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 367 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Local.pPointer = __pSymbol->m_Arg.pDeclarator->AddPointer ((*(_cls25*) GetAstLocator (0)).m_TypeKind);
		;
#line 6349 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 151:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
#line 379 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_TypeKind = EType_Pointer;
		;
#line 6362 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 152:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
#line 383 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAstNode).m_TypeKind = EType_Reference;
		;
#line 6375 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 153:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 398 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
		;
#line 6388 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 154:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 402 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix (0);
		;
#line 6401 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 155:
			{
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 428 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return DeclareFormalArg (__pSymbol->m_Arg.pFunctionSuffix, &(*(_cls22*) GetAstLocator (0)).m_TypeSpecifier, &(*(_cls24*) GetAstLocator (1)).m_Declarator, CValue ()) != NULL;
		;
#line 6414 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 156:
			{
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 432 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_IsVarArg;
			if (IsValidLocator ((*GetTokenLocator (0))))
				__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_IsUnsafeVarArg;
		;
#line 6429 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 157:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 448 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			m_Endianness = EEndianness_LittleEndian;
		;
#line 6442 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 158:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 452 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			m_Endianness = EEndianness_BigEndian;
		;
#line 6455 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 159:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
#line 18 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			(*__pAstNode).m_pType = DeclareEnumType (
				(*(_cls27*) GetAstLocator (0)).m_TypeKind,
				IsValidLocator ((*GetTokenLocator (1))) ? (*GetTokenLocator (1)).m_Data.m_String : rtl::CString ()
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 6474 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 160:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
#line 36 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			(*__pAstNode).m_TypeKind = EType_Enum;
		;
#line 6487 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 161:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
#line 40 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			(*__pAstNode).m_TypeKind = EType_Enum_c;
		;
#line 6500 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 162:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
		;
#line 6513 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 163:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 52 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();	
		;
#line 6526 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 164:
			{
			CSymbolNode_enum_member* __pSymbol = (CSymbolNode_enum_member*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 63 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			CEnumMember* pMember = __pSymbol->m_Arg.pType->CreateMember ((*GetTokenLocator (0)).m_Data.m_String);
			if (!pMember)
				return false;

			pMember->m_Pos = (*GetTokenLocator (0)).m_Pos;
			
			if (IsValidLocator ((*(_cls4*) GetAstLocator (1)))) 
				pMember->SetExpression (&(*(_cls4*) GetAstLocator (1)).m_TokenList);
		;
#line 6546 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 165:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
#line 13 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAstNode).m_pType = DeclareStructType (
				IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
				IsValidLocator ((*(_cls3*) GetAstLocator (1))) ? &(*(_cls3*) GetAstLocator (1)).m_TypeList : NULL,
				IsValidLocator ((*(_cls29*) GetAstLocator (2))) ? (*(_cls29*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 6566 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 166:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 32 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAstNode).m_PackFactor = (*(_cls30*) GetAstLocator (0)).m_Value;
		;
#line 6579 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 167:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
#line 43 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAstNode).m_Value = (*GetTokenLocator (0)).m_Data.m_Integer;
		;
#line 6592 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 168:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAstNode).m_Value = m_DefaultStructPackFactor;
		;
#line 6605 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 169:
			{
			CSymbolNode_struct_block* __pSymbol = (CSymbolNode_struct_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 54 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
		;
#line 6618 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 170:
			{
			CSymbolNode_struct_block* __pSymbol = (CSymbolNode_struct_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 59 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();
		;
#line 6631 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 171:
			{
			CSymbolNode_struct_member_declarator* __pSymbol = (CSymbolNode_struct_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 78 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			return DeclareStructMember (__pSymbol->m_Arg.pType, __pSymbol->m_Arg.pTypeSpecifier, &(*(_cls24*) GetAstLocator (0)).m_Declarator) != NULL;
		;
#line 6644 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 172:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 10 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			
			(*__pAstNode).m_pType = DeclareUnionType (IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString ());
			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 6659 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 173:
			{
			CSymbolNode_union_block* __pSymbol = (CSymbolNode_union_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 21 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
		;
#line 6672 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 174:
			{
			CSymbolNode_union_block* __pSymbol = (CSymbolNode_union_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();
		;
#line 6685 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 175:
			{
			CSymbolNode_union_member_declarator* __pSymbol = (CSymbolNode_union_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 45 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			
			return DeclareUnionMember (__pSymbol->m_Arg.pType, __pSymbol->m_Arg.pTypeSpecifier, &(*(_cls24*) GetAstLocator (0)).m_Declarator) != NULL;
		;
#line 6698 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 176:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
#line 13 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			(*__pAstNode).m_pType = DeclareClassType (
				(*(_cls33*) GetAstLocator (0)).m_TypeKind,
				IsValidLocator ((*GetTokenLocator (1))) ? (*GetTokenLocator (1)).m_Data.m_String : rtl::CString (),
				IsValidLocator ((*(_cls3*) GetAstLocator (2))) ? &(*(_cls3*) GetAstLocator (2)).m_TypeList : NULL,
				IsValidLocator ((*(_cls29*) GetAstLocator (3))) ? (*(_cls29*) GetAstLocator (3)).m_PackFactor : m_StructPackFactor
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 6719 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 177:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
#line 33 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			(*__pAstNode).m_TypeKind = EType_Interface;
		;
#line 6732 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 178:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
#line 37 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			(*__pAstNode).m_TypeKind = EType_Class;
		;
#line 6745 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 179:
			{
			CSymbolNode_class_block* __pSymbol = (CSymbolNode_class_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 44 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
		;
#line 6758 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 180:
			{
			CSymbolNode_class_block* __pSymbol = (CSymbolNode_class_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 49 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();
		;
#line 6771 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 181:
			{
			CSymbolNode_class_member_declarator* __pSymbol = (CSymbolNode_class_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 73 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			__pSymbol->m_Local.pMember = DeclareClassMember (__pSymbol->m_Arg.pType, __pSymbol->m_Arg.pDeclSpecifiers, &(*(_cls24*) GetAstLocator (0)).m_Declarator);
			if (!__pSymbol->m_Local.pMember)
				return false;
		;
#line 6786 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 182:
			{
			CSymbolNode_class_member_declarator* __pSymbol = (CSymbolNode_class_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 79 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			if (__pSymbol->m_Local.pMember->GetItemKind () != EModuleItem_ClassMember)
			{
				err::SetFormatStringError (_T("cannot apply initializer to '%s'"), __pSymbol->m_Local.pMember->GetItemKindString ());
				return false;
			}			
		;
#line 6803 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 183:
			{
			CSymbolNode_class_member_declarator* __pSymbol = (CSymbolNode_class_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 87 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
		;
#line 6815 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 184:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 9 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAstNode).m_pProperty = CreateProperty ((*(CPropertyBlock*) GetAstLocator (0)).m_pGetter, (*(CPropertyBlock*) GetAstLocator (0)).m_Setter);
		;
#line 6828 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 185:
			{
			CSymbolNode_property_accessor_declaration* __pSymbol = (CSymbolNode_property_accessor_declaration*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 32 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			return DeclarePropertyAccessor (__pSymbol->m_Arg.pBlock, &(*(_cls22*) GetAstLocator (0)).m_TypeSpecifier, &(*(_cls36*) GetAstLocator (1)).m_Declarator) != NULL;
		;
#line 6841 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 186:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
#line 46 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAstNode).m_Declarator.m_PropertyAccessorKind = (*(_cls37*) GetAstLocator (0)).m_PropertyAccessorKind;
			(*__pAstNode).m_Declarator.m_Pos = (*(_cls37*) GetAstLocator (0)).m_FirstToken.m_Pos;
		;
#line 6855 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 187:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
#line 58 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAstNode).m_PropertyAccessorKind = EPropertyAccessor_Get;
		;
#line 6868 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 188:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
#line 62 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAstNode).m_PropertyAccessorKind = EPropertyAccessor_Set;
		;
#line 6881 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 6909 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
#line 48 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
#line 6922 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
			
#line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
#line 6935 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 13 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 6948 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 14 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 6961 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 15 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 6974 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
			
#line 30 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
#line 6987 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
			
#line 39 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
#line 7000 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_logical_or_expr* __pTarget = (CSymbolNode_logical_or_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 69 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7013 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 71 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.TrueValue;
#line 7026 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 71 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.FalseValue;
#line 7039 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 83 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7052 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 85 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 7065 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 97 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7078 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 99 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 7091 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 111 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7104 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 113 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 7117 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 125 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 127 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 7143 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 139 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7156 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 141 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 7169 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 153 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 155 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 7195 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7208 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 184 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 7221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 219 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 7247 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 248 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7260 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 250 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 7273 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 277 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7286 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 279 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 7299 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 310 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7312 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 312 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.RValue;
#line 7325 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_postfix_expr* __pTarget = (CSymbolNode_postfix_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 371 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7338 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 372 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7351 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 376 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7364 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 380 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7377 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 381 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7390 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 385 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7403 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 389 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7416 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 393 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7429 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 41:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 397 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7442 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 42:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 402 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7455 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 43:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_cast_operator_rslv* __pSymbol = (CSymbolNode_cast_operator_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 416 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTypeSpecifier = &__pSymbol->m_Local.TypeSpecifier;
#line 7468 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 44:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 420 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7481 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 45:
			{
			CSymbolNode_postfix_operator* __pTarget = (CSymbolNode_postfix_operator*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 421 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7494 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 46:
			{
			CSymbolNode_call_operator* __pTarget = (CSymbolNode_call_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 425 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7507 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 47:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 459 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.Arg;
#line 7520 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 48:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 465 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.Arg;
#line 7533 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 49:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 512 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 7546 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 50:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
#line 547 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.Value;
#line 7559 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 51:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
			
#line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
#line 7572 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 52:
			{
			CSymbolNode_statement_pass1* __pTarget = (CSymbolNode_statement_pass1*) pSymbol;
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 7585 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 53:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 7598 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 54:
			{
			CSymbolNode_loop_stmt* __pTarget = (CSymbolNode_loop_stmt*) pSymbol;
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 129 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pPos = &(*GetTokenLocator (1)).m_Pos;
#line 7611 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 129 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pBreakBlock =  __pSymbol->m_Local.pFollowBlock;
#line 7615 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 129 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pContinueBlock =  __pSymbol->m_Local.pConditionBlock;
#line 7619 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 55:
			{
			CSymbolNode_loop_stmt* __pTarget = (CSymbolNode_loop_stmt*) pSymbol;
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 149 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pPos = &(*GetTokenLocator (1)).m_Pos;
#line 7632 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 149 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pBreakBlock =  __pSymbol->m_Local.pFollowBlock;
#line 7636 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 149 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pContinueBlock =  __pSymbol->m_Local.pConditionBlock;
#line 7640 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 56:
			{
			CSymbolNode_loop_stmt* __pTarget = (CSymbolNode_loop_stmt*) pSymbol;
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pPos = &(*GetTokenLocator (1)).m_Pos;
#line 7653 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pBreakBlock =  __pSymbol->m_Local.pFollowBlock;
#line 7657 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pContinueBlock =  __pSymbol->m_Local.pConditionBlock;
#line 7661 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 57:
			{
			CSymbolNode_init_declarator_list* __pTarget = (CSymbolNode_init_declarator_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 6 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = &(*(_cls21*) GetAstLocator (0)).m_DeclSpecifiers;
#line 7674 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 58:
			{
			CSymbolNode_declaration_terminator_2* __pTarget = (CSymbolNode_declaration_terminator_2*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 7 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pItem = IsValidLocator ((*(_cls18*) GetAstLocator (1))) ? (*(_cls18*) GetAstLocator (1)).m_pItem : NULL;
#line 7687 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 59:
			{
			CSymbolNode_init_declarator* __pTarget = (CSymbolNode_init_declarator*) pSymbol;
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
			
#line 15 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7700 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 60:
			{
			CSymbolNode_init_declarator* __pTarget = (CSymbolNode_init_declarator*) pSymbol;
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
			
#line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7713 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 61:
			{
			CSymbolNode_initializer* __pTarget = (CSymbolNode_initializer*) pSymbol;
			CSymbolNode_init_declarator* __pSymbol = (CSymbolNode_init_declarator*) GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
			
#line 45 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pType = ((CVariable*) (*__pAstNode).m_pItem)->GetType ();
#line 7726 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 62:
			{
			CSymbolNode_initializer_list* __pTarget = (CSymbolNode_initializer_list*) pSymbol;
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
			
#line 69 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7739 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 63:
			{
			CSymbolNode_initializer_list_entry* __pTarget = (CSymbolNode_initializer_list_entry*) pSymbol;
			CSymbolNode_initializer_list* __pSymbol = (CSymbolNode_initializer_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 75 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7752 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 64:
			{
			CSymbolNode_initializer_list_entry* __pTarget = (CSymbolNode_initializer_list_entry*) pSymbol;
			CSymbolNode_initializer_list* __pSymbol = (CSymbolNode_initializer_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 75 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7765 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 65:
			{
			CSymbolNode_initializer* __pTarget = (CSymbolNode_initializer*) pSymbol;
			CSymbolNode_initializer_list_entry* __pSymbol = (CSymbolNode_initializer_list_entry*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 79 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7778 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 66:
			{
			CSymbolNode_declaration_specifier* __pTarget = (CSymbolNode_declaration_specifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
			
#line 115 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = &(*__pAstNode).m_DeclSpecifiers;
#line 7791 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 67:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
			
#line 131 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
#line 7804 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 68:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 135 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7817 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 69:
			{
			CSymbolNode_access_specifier* __pTarget = (CSymbolNode_access_specifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 136 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pAccessSpecifier = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7830 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 70:
			{
			CSymbolNode_storage_class_specifier* __pTarget = (CSymbolNode_storage_class_specifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 137 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 7843 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 71:
			{
			CSymbolNode_type_specifier* __pTarget = (CSymbolNode_type_specifier*) pSymbol;
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
#line 7856 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 72:
			{
			CSymbolNode_type_modifier* __pTarget = (CSymbolNode_type_modifier*) pSymbol;
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 143 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeModifiers = __pSymbol->m_Arg.pTypeSpecifier;
#line 7869 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 73:
			{
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
			
#line 344 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 7882 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 74:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
			
#line 355 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 7895 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 75:
			{
			CSymbolNode_type_modifier* __pTarget = (CSymbolNode_type_modifier*) pSymbol;
			CSymbolNode_pointer* __pSymbol = (CSymbolNode_pointer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 370 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeModifiers = __pSymbol->m_Local.pPointer;
#line 7908 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 76:
			{
			CSymbolNode_array_suffix* __pTarget = (CSymbolNode_array_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 392 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 7921 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 77:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 393 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 7934 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 78:
			{
			CSymbolNode_function_formal_argument_list* __pTarget = (CSymbolNode_function_formal_argument_list*) pSymbol;
			CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 416 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Local.pFunctionSuffix;
#line 7947 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 79:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 421 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
#line 7960 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 80:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 422 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
#line 7973 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 81:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
			
#line 27 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 7986 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 82:
			{
			CSymbolNode_enum_member_list* __pTarget = (CSymbolNode_enum_member_list*) pSymbol;
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 50 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 7999 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 83:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 58 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8012 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 84:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 58 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8025 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 85:
			{
			CSymbolNode_struct_block* __pTarget = (CSymbolNode_struct_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
			
#line 23 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 8038 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 86:
			{
			CSymbolNode_struct_member_declaration* __pTarget = (CSymbolNode_struct_member_declaration*) pSymbol;
			CSymbolNode_struct_block* __pSymbol = (CSymbolNode_struct_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8051 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 87:
			{
			CSymbolNode_struct_member_declarator_list* __pTarget = (CSymbolNode_struct_member_declarator_list*) pSymbol;
			CSymbolNode_struct_member_declaration* __pSymbol = (CSymbolNode_struct_member_declaration*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 67 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8064 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 67 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier =  &(*(_cls22*) GetAstLocator (0)).m_TypeSpecifier;
#line 8068 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 88:
			{
			CSymbolNode_struct_member_declarator* __pTarget = (CSymbolNode_struct_member_declarator*) pSymbol;
			CSymbolNode_struct_member_declarator_list* __pSymbol = (CSymbolNode_struct_member_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 73 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8081 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 73 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier =  __pSymbol->m_Arg.pTypeSpecifier;
#line 8085 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 89:
			{
			CSymbolNode_struct_member_declarator* __pTarget = (CSymbolNode_struct_member_declarator*) pSymbol;
			CSymbolNode_struct_member_declarator_list* __pSymbol = (CSymbolNode_struct_member_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 73 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8098 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 73 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier =  __pSymbol->m_Arg.pTypeSpecifier;
#line 8102 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 90:
			{
			CSymbolNode_union_block* __pTarget = (CSymbolNode_union_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
			
#line 16 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 8115 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 91:
			{
			CSymbolNode_union_member_declaration* __pTarget = (CSymbolNode_union_member_declaration*) pSymbol;
			CSymbolNode_union_block* __pSymbol = (CSymbolNode_union_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 24 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8128 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 92:
			{
			CSymbolNode_union_member_declarator_list* __pTarget = (CSymbolNode_union_member_declarator_list*) pSymbol;
			CSymbolNode_union_member_declaration* __pSymbol = (CSymbolNode_union_member_declaration*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 34 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8141 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 34 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier =  &(*(_cls22*) GetAstLocator (0)).m_TypeSpecifier;
#line 8145 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 93:
			{
			CSymbolNode_union_member_declarator* __pTarget = (CSymbolNode_union_member_declarator*) pSymbol;
			CSymbolNode_union_member_declarator_list* __pSymbol = (CSymbolNode_union_member_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 40 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8158 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 40 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier =  __pSymbol->m_Arg.pTypeSpecifier;
#line 8162 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 94:
			{
			CSymbolNode_union_member_declarator* __pTarget = (CSymbolNode_union_member_declarator*) pSymbol;
			CSymbolNode_union_member_declarator_list* __pSymbol = (CSymbolNode_union_member_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 40 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8175 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 40 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_UnionSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier =  __pSymbol->m_Arg.pTypeSpecifier;
#line 8179 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 95:
			{
			CSymbolNode_class_block* __pTarget = (CSymbolNode_class_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
			
#line 24 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 8192 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 96:
			{
			CSymbolNode_class_member_declaration* __pTarget = (CSymbolNode_class_member_declaration*) pSymbol;
			CSymbolNode_class_block* __pSymbol = (CSymbolNode_class_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 8205 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 97:
			{
			CSymbolNode_class_member_declarator_list* __pTarget = (CSymbolNode_class_member_declarator_list*) pSymbol;
			CSymbolNode_class_member_declaration* __pSymbol = (CSymbolNode_class_member_declaration*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pDeclSpecifiers = &(*(_cls21*) GetAstLocator (0)).m_DeclSpecifiers;
#line 8218 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 8222 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 98:
			{
			CSymbolNode_class_member_declarator* __pTarget = (CSymbolNode_class_member_declarator*) pSymbol;
			CSymbolNode_class_member_declarator_list* __pSymbol = (CSymbolNode_class_member_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 64 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 8235 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 64 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 8239 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 99:
			{
			CSymbolNode_class_member_declarator* __pTarget = (CSymbolNode_class_member_declarator*) pSymbol;
			CSymbolNode_class_member_declarator_list* __pSymbol = (CSymbolNode_class_member_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 64 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 8252 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 64 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 8256 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 100:
			{
			CSymbolNode_initializer* __pTarget = (CSymbolNode_initializer*) pSymbol;
			CSymbolNode_class_member_declarator* __pSymbol = (CSymbolNode_class_member_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 86 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = ((CClassFieldMember*) __pSymbol->m_Local.pMember)->GetType ();
#line 8269 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 101:
			{
			CSymbolNode_property_accessor_declaration* __pTarget = (CSymbolNode_property_accessor_declaration*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CPropertyBlock* __pAstNode = (CPropertyBlock*) __pSymbol->m_pAstNode;
			
#line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			__pTarget->m_Arg.pBlock = &(*__pAstNode);
#line 8282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 102:
			{
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
			
#line 43 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 8295 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 103:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
			
#line 45 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 8308 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
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
				_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
#line 124 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
#line 8342 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_if_stmt:
			{
				CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 78 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
	__pSymbol->m_Local.pThenBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("if_then"));
	__pSymbol->m_Local.pElseBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("if_else"));
	__pSymbol->m_Local.pFollowBlock = __pSymbol->m_Local.pElseBlock;
;
#line 8400 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		case ESymbol_while_stmt:
			{
				CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 119 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
	__pSymbol->m_Local.pConditionBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("while_condition"));
	__pSymbol->m_Local.pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("while_body"));
	__pSymbol->m_Local.pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("while_follow"));
	m_pModule->m_ControlFlowMgr.Follow (__pSymbol->m_Local.pConditionBlock);
;
#line 8417 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_do_stmt:
			{
				CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 143 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
	__pSymbol->m_Local.pConditionBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("do_condition"));
	__pSymbol->m_Local.pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("do_body"));
	__pSymbol->m_Local.pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("do_follow"));
	m_pModule->m_ControlFlowMgr.Follow (__pSymbol->m_Local.pBodyBlock);
;
#line 8433 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_for_stmt:
			{
				CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 172 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
	__pSymbol->m_Local.pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("for_body"));
	__pSymbol->m_Local.pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("for_follow"));
	__pSymbol->m_Local.pConditionBlock = __pSymbol->m_Local.pBodyBlock;
	__pSymbol->m_Local.pLoopBlock = __pSymbol->m_Local.pBodyBlock;
;
#line 8449 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_attribute_block:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 5 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_AttributeBlock.llk"
				
		m_pModule->m_AttributeMgr.CreateAttributeSet ();
	;
#line 8467 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
#line 108 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_DeclSpecifiers);
	;
#line 8482 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_function_suffix:
			{
				CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 413 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		__pSymbol->m_Local.pFunctionSuffix = __pSymbol->m_Arg.pDeclarator->AddFormalArgSuffix ();
	;
#line 8519 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
				_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
#line 128 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
#line 8580 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_call_operator:
			{
				CSymbolNode_call_operator* __pSymbol = (CSymbolNode_call_operator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 455 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
	return m_pModule->m_OperatorMgr.CallOperator (__pSymbol->m_Arg.pValue, &__pSymbol->m_Local.ArgList);
;
#line 8629 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
#line 112 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_DeclSpecifiers);
	;
#line 8661 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_enum_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
#line 9 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
				
		if ((*__pAstNode).m_pType->GetTypeKind () == EType_Enum_c ||
			(*__pAstNode).m_pType->GetName ().IsEmpty ())
		{
			return m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->ExposeEnumMembers ((*__pAstNode).m_pType);
		}
	;
#line 8691 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
					
				pTransition->m_ProductionIndex = 114;
				pTransition->m_ResolverIndex = 196;
				pTransition->m_ResolverElseIndex = 117;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 252;
				pTransition->m_ResolverIndex = 153;
				pTransition->m_ResolverElseIndex = 389;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 979;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 3:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 477;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 478;
				return ELaDfaResult_Production;
					
			}
			
		
		case 4:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 981;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 5:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 408;
				pTransition->m_ResolverIndex = 153;
				pTransition->m_ResolverElseIndex = 408;
						
				return ELaDfaResult_Resolver;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
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
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
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
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 408;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 6:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 983;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 7:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 425;
				pTransition->m_ResolverIndex = 153;
				pTransition->m_ResolverElseIndex = 425;
						
				return ELaDfaResult_Resolver;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 424;
				pTransition->m_ResolverIndex = 196;
				pTransition->m_ResolverElseIndex = 425;
						
				return ELaDfaResult_Resolver;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 424;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 8:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 423;
				pTransition->m_ResolverIndex = 196;
				pTransition->m_ResolverElseIndex = 425;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 9:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 986;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 10:
			
			pTransition->m_ProductionIndex = 464;
			pTransition->m_ResolverIndex = 153;
			pTransition->m_ResolverElseIndex = 464;
				
			return ELaDfaResult_Resolver;
			
		
		case 11:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 465;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 12:
			
			switch (LookaheadToken)
			{
			
			case '[':
					
				pTransition->m_ProductionIndex = 989;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 13:
			
			switch (LookaheadToken)
			{
			
			case ']':
					
				pTransition->m_ProductionIndex = 529;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 528;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 14:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 991;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 15:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 531;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 530;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 16:
			
			switch (LookaheadToken)
			{
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 993;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 17:
			
			switch (LookaheadToken)
			{
			
			case '&':
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 534;
				pTransition->m_ResolverIndex = 196;
				pTransition->m_ResolverElseIndex = 534;
						
				return ELaDfaResult_Resolver;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 536;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 18:
			
			switch (LookaheadToken)
			{
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 995;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 19:
			
			switch (LookaheadToken)
			{
			
			case '&':
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 534;
				pTransition->m_ResolverIndex = 196;
				pTransition->m_ResolverElseIndex = 534;
						
				return ELaDfaResult_Resolver;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 536;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 534;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 20:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 471;
				pTransition->m_ResolverIndex = 196;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 21:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 473;
				pTransition->m_ResolverIndex = 196;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 22:
			
			switch (LookaheadToken)
			{
			
			case '#':
					
				pTransition->m_ProductionIndex = 999;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 23:
			
			switch (LookaheadToken)
			{
			
			case EToken_Pack:
					
				pTransition->m_ProductionIndex = 126;
				return ELaDfaResult_Production;
					
			
			case EToken_Endian:
					
				pTransition->m_ProductionIndex = 205;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 24:
			
			switch (LookaheadToken)
			{
			
			case '&':
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 25:
			
			switch (LookaheadToken)
			{
			
			case '+':
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 26:
			
			switch (LookaheadToken)
			{
			
			case '-':
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 27:
			
			switch (LookaheadToken)
			{
			
			case '*':
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 28:
			
			switch (LookaheadToken)
			{
			
			case '~':
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 29:
			
			switch (LookaheadToken)
			{
			
			case '!':
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 30:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 31:
			
			switch (LookaheadToken)
			{
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 32:
			
			switch (LookaheadToken)
			{
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 33:
			
			switch (LookaheadToken)
			{
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 34:
			
			switch (LookaheadToken)
			{
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 35:
			
			switch (LookaheadToken)
			{
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 36:
			
			switch (LookaheadToken)
			{
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 37:
			
			switch (LookaheadToken)
			{
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 38:
			
			switch (LookaheadToken)
			{
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 39:
			
			switch (LookaheadToken)
			{
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 40:
			
			switch (LookaheadToken)
			{
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 273;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 464;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 41:
			
			switch (LookaheadToken)
			{
			
			case ',':
					
				pTransition->m_ProductionIndex = 468;
				pTransition->m_ResolverIndex = 694;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 42:
			
			switch (LookaheadToken)
			{
			
			case '}':
					
				pTransition->m_ProductionIndex = 468;
				pTransition->m_ResolverIndex = 694;
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
