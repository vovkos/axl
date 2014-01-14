#pragma once


	#define _AXL_JNC_PARSER_H

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
	ESymbol_pragma_pack = 2, 
	ESymbol_struct_pack_factor_value = 3, 
	ESymbol_namespace_declaration = 4, 
	ESymbol_extend_declaration = 5, 
	ESymbol_using_declaration = 6, 
	ESymbol_friend_declaration = 7, 
	ESymbol_access_declaration = 8, 
	ESymbol_named_type_specifier = 9, 
	ESymbol_item_declaration_w_specifier = 10, 
	ESymbol_item_declaration_no_specifier_rslv = 11, 
	ESymbol_item_declaration_no_specifier = 12, 
	ESymbol_named_type_block_declaration = 13, 
	ESymbol_local_declaration = 14, 
	ESymbol_statement = 15, 
	ESymbol_catch_label = 16, 
	ESymbol_finally_label = 17, 
	ESymbol_qualified_name = 18, 
	ESymbol_access_specifier = 19, 
	ESymbol_qualified_name_impl = 20, 
	ESymbol_declaration_specifier_list = 21, 
	ESymbol_declarator_list = 22, 
	ESymbol_declaration_terminator = 23, 
	ESymbol_declarator_prefix = 24, 
	ESymbol_declarator_name = 25, 
	ESymbol_function_suffix = 26, 
	ESymbol_declarator_suffix = 27, 
	ESymbol_post_declarator_modifier = 28, 
	ESymbol_declaration_body_pass1 = 29, 
	ESymbol_compound_stmt_pass1 = 30, 
	ESymbol_statement_pass1 = 31, 
	ESymbol_type_name_impl = 32, 
	ESymbol_type_specifier_modifier_list = 33, 
	ESymbol_type_name = 34, 
	ESymbol_type_name_list = 35, 
	ESymbol_type_name_or_expr = 36, 
	ESymbol_expression_s = 37, 
	ESymbol_attribute_block = 38, 
	ESymbol_attribute_declarator_list = 39, 
	ESymbol_attribute_declarator = 40, 
	ESymbol_expression_pass1 = 41, 
	ESymbol_primary_expr_pass1 = 42, 
	ESymbol_arg_list_pass1 = 43, 
	ESymbol_arg_pass1 = 44, 
	ESymbol_expression_save_value = 45, 
	ESymbol_expression = 46, 
	ESymbol_expression_or_empty_list_save_list = 47, 
	ESymbol_expression_or_empty_list = 48, 
	ESymbol_conditional_expr = 49, 
	ESymbol_expression_or_empty = 50, 
	ESymbol_expression_list = 51, 
	ESymbol_constant_expr = 52, 
	ESymbol_constant_integer_expr = 53, 
	ESymbol_logical_or_expr = 54, 
	ESymbol_logical_and_expr = 55, 
	ESymbol_inclusive_or_expr = 56, 
	ESymbol_exclusive_or_expr = 57, 
	ESymbol_and_expr = 58, 
	ESymbol_equality_expr = 59, 
	ESymbol_relational_expr = 60, 
	ESymbol_equality_operator = 61, 
	ESymbol_shift_expr = 62, 
	ESymbol_relational_operator = 63, 
	ESymbol_additive_expr = 64, 
	ESymbol_shift_operator = 65, 
	ESymbol_multiplicative_expr = 66, 
	ESymbol_additive_operator = 67, 
	ESymbol_at_expr = 68, 
	ESymbol_multiplicative_operator = 69, 
	ESymbol_assignment_expr = 70, 
	ESymbol_unary_expr = 71, 
	ESymbol_assignment_operator_expr = 72, 
	ESymbol_assignment_operator = 73, 
	ESymbol_curly_initializer = 74, 
	ESymbol_postfix_expr = 75, 
	ESymbol_unary_operator_expr = 76, 
	ESymbol_cast_operator_rslv = 77, 
	ESymbol_storage_specifier = 78, 
	ESymbol_new_operator_type = 79, 
	ESymbol_new_operator_curly_initializer = 80, 
	ESymbol_type_name_w_constructor_rslv = 81, 
	ESymbol_qualified_type_name = 82, 
	ESymbol_type_specifier_modifier = 83, 
	ESymbol_primary_expr = 84, 
	ESymbol_postfix_operator = 85, 
	ESymbol_member_operator = 86, 
	ESymbol_literal = 87, 
	ESymbol_fmt_literal_value = 88, 
	ESymbol_curly_initializer_item = 89, 
	ESymbol_expression_save_value_s = 90, 
	ESymbol_conditional_expr_s = 91, 
	ESymbol_expression_or_empty_s = 92, 
	ESymbol_expression_list_s = 93, 
	ESymbol_expression_or_empty_list_s = 94, 
	ESymbol_logical_or_expr_s = 95, 
	ESymbol_logical_and_expr_s = 96, 
	ESymbol_inclusive_or_expr_s = 97, 
	ESymbol_exclusive_or_expr_s = 98, 
	ESymbol_and_expr_s = 99, 
	ESymbol_equality_expr_s = 100, 
	ESymbol_relational_expr_s = 101, 
	ESymbol_shift_expr_s = 102, 
	ESymbol_additive_expr_s = 103, 
	ESymbol_multiplicative_expr_s = 104, 
	ESymbol_at_expr_s = 105, 
	ESymbol_assignment_expr_s = 106, 
	ESymbol_unary_expr_s = 107, 
	ESymbol_postfix_expr_s = 108, 
	ESymbol_unary_operator_expr_s = 109, 
	ESymbol_new_operator_sype_s = 110, 
	ESymbol_primary_expr_s = 111, 
	ESymbol_postfix_operator_s = 112, 
	ESymbol_member_operator_s = 113, 
	ESymbol_literal_s = 114, 
	ESymbol_compound_stmt = 115, 
	ESymbol_constructor_compound_stmt = 116, 
	ESymbol_btm_construct_stmt = 117, 
	ESymbol_btm_construct_stmt_rslv = 118, 
	ESymbol_btm_construct_name = 119, 
	ESymbol_expression_stmt = 120, 
	ESymbol_if_stmt = 121, 
	ESymbol_switch_stmt = 122, 
	ESymbol_while_stmt = 123, 
	ESymbol_do_stmt = 124, 
	ESymbol_for_stmt = 125, 
	ESymbol_break_stmt = 126, 
	ESymbol_continue_stmt = 127, 
	ESymbol_return_stmt = 128, 
	ESymbol_once_stmt = 129, 
	ESymbol_try_stmt = 130, 
	ESymbol_switch_block_stmt = 131, 
	ESymbol_reactor_body = 132, 
	ESymbol_reactor_stmt = 133, 
	ESymbol_reactor_onchange_stmt = 134, 
	ESymbol_reactor_expression_stmt = 135, 
	ESymbol_statement_0 = 136, 
	ESymbol_compound_stmt_0 = 137, 
	ESymbol_expression_stmt_0 = 138, 
	ESymbol_if_stmt_0 = 139, 
	ESymbol_switch_stmt_0 = 140, 
	ESymbol_while_stmt_0 = 141, 
	ESymbol_do_stmt_0 = 142, 
	ESymbol_for_stmt_0 = 143, 
	ESymbol_break_stmt_0 = 144, 
	ESymbol_continue_stmt_0 = 145, 
	ESymbol_return_stmt_0 = 146, 
	ESymbol_once_stmt_0 = 147, 
	ESymbol_expression_0 = 148, 
	ESymbol_primary_expr_0 = 149, 
	ESymbol_reactor_body_0 = 150, 
	ESymbol_reactor_stmt_0 = 151, 
	ESymbol_reactor_onchange_stmt_0 = 152, 
	ESymbol_reactor_expression_stmt_0 = 153, 
	ESymbol_declaration_specifier = 154, 
	ESymbol_type_specifier = 155, 
	ESymbol_type_modifier = 156, 
	ESymbol_pitcher_modifier = 157, 
	ESymbol_property_template_specifier = 158, 
	ESymbol_qualified_type_name_rslv = 159, 
	ESymbol_named_type_block = 160, 
	ESymbol_full_declarator = 161, 
	ESymbol_declarator = 162, 
	ESymbol_declarator_qualifier = 163, 
	ESymbol_operator_name = 164, 
	ESymbol_array_suffix = 165, 
	ESymbol_bitfield_suffix = 166, 
	ESymbol_function_formal_argument_list = 167, 
	ESymbol_function_formal_argument = 168, 
	ESymbol_function_formal_argument_w_specifier = 169, 
	ESymbol_function_formal_argument_no_specifier = 170, 
	ESymbol_enum_specifier = 171, 
	ESymbol_struct_specifier = 172, 
	ESymbol_union_specifier = 173, 
	ESymbol_class_specifier = 174, 
	ESymbol_enum_type_kind = 175, 
	ESymbol_enum_block = 176, 
	ESymbol_enum_member_list = 177, 
	ESymbol_enum_member = 178, 
	ESymbol_struct_pack_factor = 179, 
	ESymbol_named_type_block_impl = 180, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 2,

		StartSymbol        = 0,
		StartPragmaSymbol  = 330,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 155,
		SymbolCount        = 331,
		SequenceCount      = 507,
		ActionCount        = 390,
		ArgumentCount      = 163,
		BeaconCount        = 166,
		LaDfaCount         = 118,

		TotalCount         = 1830,

		NamedSymbolCount   = 181,

		TokenFirst         = 0,
		TokenEnd           = 155,
		SymbolFirst        = 155,
		NamedSymbolEnd     = 336,
		SymbolEnd          = 486,
		SequenceFirst      = 486,
		SequenceEnd        = 993,
		ActionFirst        = 993,
		ActionEnd          = 1383,
		ArgumentFirst      = 1383,
		ArgumentEnd        = 1546,
		BeaconFirst        = 1546,
		BeaconEnd          = 1712,
		LaDfaFirst         = 1712,
		LaDfaEnd           = 1830,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	
	enum EStage
	{
		EStage_Pass1, // global declarations (before linkage)
		EStage_Pass2, // statements and expressions (after linkage & calclayout)
		EStage_ReactorScan, // scan reactor body to build bind site map (after linkage but before calclayout)
	};

	enum EFlag
	{
		EFlag_ConstExpression = 0x01,
	};

	enum EAssign
	{
		EAssign_Normal,
		EAssign_BinOp,
	};

	enum ELandingPadFlag
	{
		ELandingPadFlag_Catch   = 0x01,
		ELandingPadFlag_Finally = 0x02,
	};

	struct TCurlyInitializer
	{
		CValue m_TargetValue;
		CValue m_MemberValue;
		size_t m_Index;
		size_t m_Count;
	};

	struct TLiteral
	{
		CValue m_FmtLiteralValue;
		rtl::CArrayT <uchar_t> m_BinData;
		EToken m_LastToken;
	};

	CModule* m_pModule;
	EStage m_Stage;
	uint_t m_Flags;
	size_t m_StructPackFactor;
	size_t m_DefaultStructPackFactor;
	EStorage m_StorageKind;
	EAccess m_AccessKind;
	CAttributeBlock* m_pAttributeBlock;
	CModuleItem* m_pLastDeclaredItem;
	rtl::CArrayT <CTypeSpecifier*> m_TypeSpecifierStack;

	CFunctionType* m_pLastPropertyGetterType;
	CTypeModifiers m_LastPropertyTypeModifiers;

	CReactorClassType* m_pReactorType;
	rtl::CStdListT <TReaction> m_ReactionList;
	rtl::CBoxListT <CValue> m_ReactorBindSiteList;
	size_t m_ReactorBindSiteCount;
	size_t m_ReactorBindSiteTotalCount;

	CDerivableType* m_pConstructorType;
	CProperty* m_pConstructorProperty;

	CValue m_CurlyInitializerTargetValue;
	CValue m_PitcherReturnValue;

	// saved attributes

	CValue m_ExpressionValue;
	rtl::CBoxListT <CValue> m_ExpressionValueList;


	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class _cls1: public CAstNode
	{
	public:
// #line 98 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CQualifiedName m_Name
;
// #line 339 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls2: public CAstNode
	{
	public:
// #line 193 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 349 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls3: public CAstNode
	{
	public:
// #line 201 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CToken* m_pAnchorToken;
;
// #line 359 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls4: public CAstNode
	{
	public:
// #line 261 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
// #line 369 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls5: public CAstNode
	{
	public:
// #line 269 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	rtl::CBoxListT <CType*> m_TypeList;
;
// #line 379 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls6: public CAstNode
	{
	public:
// #line 285 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
// #line 389 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls7: public CAstNode
	{
	public:
// #line 4 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 399 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls8: public CAstNode
	{
	public:
// #line 51 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 409 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls9: public CAstNode
	{
	public:
// #line 91 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;
;
// #line 419 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls10: public CAstNode
	{
	public:
// #line 104 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;
;
// #line 429 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls11: public CAstNode
	{
	public:
// #line 114 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 439 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls12: public CAstNode
	{
	public:
// #line 133 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 449 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls13: public CAstNode
	{
	public:
// #line 159 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;
;
// #line 459 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls14: public CAstNode
	{
	public:
// #line 175 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	intptr_t m_Value;
;
// #line 469 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls15: public CAstNode
	{
	public:
// #line 200 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_TrueValue;
	CValue m_FalseValue;
	CBasicBlock* m_pThenBlock;
	CBasicBlock* m_pElseBlock;
	CBasicBlock* m_pPhiBlock;
;
// #line 483 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls16: public CAstNode
	{
	public:
// #line 314 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 493 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls17: public CAstNode
	{
	public:
// #line 342 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 503 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls18: public CAstNode
	{
	public:
// #line 378 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 513 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls19: public CAstNode
	{
	public:
// #line 406 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 523 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls20: public CAstNode
	{
	public:
// #line 434 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 533 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls21: public CAstNode
	{
	public:
// #line 491 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 543 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls22: public CAstNode
	{
	public:
// #line 622 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CType* m_pType;
	CValue m_ElementCountValue;
	rtl::CBoxListT <CValue> m_ArgList;
;
// #line 555 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls23: public CAstNode
	{
	public:
// #line 12 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	CValue m_Value;	
;
// #line 565 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls24: public CAstNode
	{
	public:
// #line 21 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	CValue m_Value;
;
// #line 575 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls25: public CAstNode
	{
	public:
// #line 31 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 585 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls26: public CAstNode
	{
	public:
// #line 50 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 595 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls27: public CAstNode
	{
	public:
// #line 336 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	CType* m_pType;
;
// #line 605 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls28: public CAstNode
	{
	public:
// #line 57 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
		
	CQualifiedName m_Name;
;
// #line 615 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls29: public CAstNode
	{
	public:
// #line 4 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CTypeSpecifier m_TypeSpecifier;
;
// #line 625 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls30: public CAstNode
	{
	public:
// #line 43 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CTypeSpecifier m_TypeSpecifier;
;
// #line 635 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls31: public CAstNode
	{
	public:
// #line 72 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EStorage m_StorageKind;
;
// #line 645 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls32: public CAstNode
	{
	public:
// #line 131 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EAccess m_AccessKind;
;
// #line 655 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls33: public CAstNode
	{
	public:
// #line 148 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
// #line 665 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls34: public CAstNode
	{
	public:
// #line 245 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
// #line 675 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls35: public CAstNode
	{
	public:
// #line 263 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CPropertyType* m_pType;
;
// #line 685 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls36: public CAstNode
	{
	public:
// #line 285 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	ETypeModifier m_TypeModifier;
;
// #line 695 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls37: public CAstNode
	{
	public:
// #line 31 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
		
	CDeclarator m_Declarator;
;
// #line 705 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls38: public CAstNode
	{
	public:
// #line 105 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
		
	EUnOp m_UnOpKind;
	EBinOp m_BinOpKind;
;
// #line 716 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls39: public CAstNode
	{
	public:
// #line 6 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CType* m_pType;
;
// #line 726 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls40: public CAstNode
	{
	public:
// #line 33 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CEnumType* m_pType;
;
// #line 736 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls41: public CAstNode
	{
	public:
// #line 62 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	EEnumType m_EnumTypeKind;
	uint_t m_Flags;
;
// #line 747 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls42: public CAstNode
	{
	public:
// #line 125 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CStructType* m_pType;
;
// #line 757 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls43: public CAstNode
	{
	public:
// #line 154 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_PackFactor;
;
// #line 767 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls44: public CAstNode
	{
	public:
// #line 165 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_Value;
;
// #line 777 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls45: public CAstNode
	{
	public:
// #line 184 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CUnionType* m_pType;
;
// #line 787 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls46: public CAstNode
	{
	public:
// #line 210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CClassType* m_pType;
;
// #line 797 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_item_declaration_no_specifier_rslv: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 163 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
		CDeclarator Declarator;
	;
// #line 825 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_item_declaration_no_specifier_rslv ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_item_declaration_no_specifier_rslv;
			
		}
	};
		
	
	class CSymbolNode_item_declaration_no_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 136 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
		CDeclarator Declarator;
	;
// #line 847 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_item_declaration_no_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_item_declaration_no_specifier;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	
	
	
	
	
	
	class CSymbolNode_qualified_name_impl: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 105 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			CQualifiedName* pName;
// #line 876 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_qualified_name_impl ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_qualified_name_impl;
			
		}
	};
		
	
	
	class CSymbolNode_declarator_list: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 3 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 897 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_declarator_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declarator_list;
			
		}
	};
		
	
	class CSymbolNode_declaration_terminator: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 171 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 917 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_declaration_terminator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declaration_terminator;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_declarator_prefix: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 286 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 939 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_declarator_prefix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declarator_prefix;
			
		}
	};
		
	
	class CSymbolNode_declarator_name: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 47 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 959 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_declarator_name ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declarator_name;
			
		}
	};
		
	
	class CSymbolNode_function_suffix: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 327 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 979 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 329 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclFunctionSuffix* pFunctionSuffix;
		EStorage PrevStorageKind;
	;
// #line 989 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_function_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_suffix;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_declarator_suffix: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 301 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 1013 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_declarator_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declarator_suffix;
			
		}
	};
		
	
	class CSymbolNode_post_declarator_modifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 420 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 1033 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_post_declarator_modifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_post_declarator_modifier;
			
		}
	};
		
	
	
	class CSymbolNode_compound_stmt_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 204 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 1054 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_compound_stmt_pass1 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_compound_stmt_pass1;
			
		}
	};
		
	
	class CSymbolNode_statement_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 218 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			rtl::CBoxListT <CToken>* pTokenList; CToken* pAnchorToken;
// #line 1074 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_statement_pass1 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_statement_pass1;
			
		}
	};
		
	
	class CSymbolNode_type_name_impl: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 240 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			CType** ppType; CValue* pCountValue;
// #line 1094 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 242 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
		CDeclarator Declarator;
	;
// #line 1103 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_type_name_impl ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_type_name_impl;
			
		}
	};
		
	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_primary_expr_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 12 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 1132 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_primary_expr_pass1 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_primary_expr_pass1;
			
		}
	};
		
	
	
	class CSymbolNode_arg_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 59 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 1153 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_arg_pass1 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_arg_pass1;
			
		}
	};
		
	
	
	
	
	
	class CSymbolNode_conditional_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 207 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1177 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_conditional_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_conditional_expr;
			
		}
	};
		
	
	
	
	
	
	class CSymbolNode_logical_or_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 235 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1201 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 237 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_logical_or_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_logical_or_expr;
			
		}
	};
		
	
	class CSymbolNode_logical_and_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 248 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1230 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 250 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1239 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_logical_and_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_logical_and_expr;
			
		}
	};
		
	
	class CSymbolNode_inclusive_or_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 261 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1259 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 263 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1268 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_inclusive_or_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_inclusive_or_expr;
			
		}
	};
		
	
	class CSymbolNode_exclusive_or_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 274 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1288 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 276 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1297 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_exclusive_or_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_exclusive_or_expr;
			
		}
	};
		
	
	class CSymbolNode_and_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 287 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1317 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 289 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1326 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_and_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_and_expr;
			
		}
	};
		
	
	class CSymbolNode_equality_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 300 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1346 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 302 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1355 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_equality_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_equality_expr;
			
		}
	};
		
	
	class CSymbolNode_relational_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 328 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1375 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 330 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1384 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_relational_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_relational_expr;
			
		}
	};
		
	
	
	class CSymbolNode_shift_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 364 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1405 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 366 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1414 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_shift_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_shift_expr;
			
		}
	};
		
	
	
	class CSymbolNode_additive_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 392 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1435 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 394 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1444 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_additive_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_additive_expr;
			
		}
	};
		
	
	
	class CSymbolNode_multiplicative_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 420 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1465 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 422 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1474 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_multiplicative_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_multiplicative_expr;
			
		}
	};
		
	
	
	class CSymbolNode_at_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 452 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1495 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 454 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1504 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_at_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_at_expr;
			
		}
	};
		
	
	
	class CSymbolNode_assignment_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 465 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1525 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_assignment_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_assignment_expr;
			
		}
	};
		
	
	class CSymbolNode_unary_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 545 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1545 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_unary_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_unary_expr;
			
		}
	};
		
	
	class CSymbolNode_assignment_operator_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 469 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1565 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 471 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1574 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_assignment_operator_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_assignment_operator_expr;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	class CSymbolNode_curly_initializer: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 829 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		TCurlyInitializer Initializer;
	;
// #line 1599 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_curly_initializer ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_curly_initializer;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_postfix_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 660 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1621 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_postfix_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_postfix_expr;
			
		}
	};
		
	
	class CSymbolNode_unary_operator_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 550 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1641 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_unary_operator_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_unary_operator_expr;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_cast_operator_rslv: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 654 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CTypeSpecifier TypeSpecifier;
	;
// #line 1665 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_cast_operator_rslv ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_cast_operator_rslv;
			
		}
	};
		
	
	
	
	class CSymbolNode_new_operator_curly_initializer: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 641 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1687 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_new_operator_curly_initializer ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_new_operator_curly_initializer;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	
	class CSymbolNode_type_specifier_modifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 58 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 1711 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_type_specifier_modifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_type_specifier_modifier;
			
		}
	};
		
	
	class CSymbolNode_primary_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 723 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1731 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_primary_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_primary_expr;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_postfix_operator: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 665 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1753 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_postfix_operator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_postfix_operator;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_member_operator: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 705 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1775 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_member_operator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_member_operator;
			
		}
	};
		
	
	class CSymbolNode_literal: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 789 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1795 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 791 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		TLiteral Literal;
	;
// #line 1804 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_literal ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_literal;
			
		}
	};
		
	
	class CSymbolNode_fmt_literal_value: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 815 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			TLiteral* pLiteral;
// #line 1824 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_fmt_literal_value ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_fmt_literal_value;
			
		}
	};
		
	
	class CSymbolNode_curly_initializer_item: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 851 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			TCurlyInitializer* pInitializer;
// #line 1844 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 853 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue PrevCurlyInitializerTargetValue;
	;
// #line 1853 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_curly_initializer_item ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_curly_initializer_item;
			
		}
	};
		
	
	
	class CSymbolNode_conditional_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 77 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1874 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 79 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue TrueValue;
		CValue FalseValue;
	;
// #line 1884 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_conditional_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_conditional_expr_s;
			
		}
	};
		
	
	
	
	
	class CSymbolNode_logical_or_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 91 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1907 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 93 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1916 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_logical_or_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_logical_or_expr_s;
			
		}
	};
		
	
	class CSymbolNode_logical_and_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 104 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1936 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 106 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1945 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_logical_and_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_logical_and_expr_s;
			
		}
	};
		
	
	class CSymbolNode_inclusive_or_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 117 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1965 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1974 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_inclusive_or_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_inclusive_or_expr_s;
			
		}
	};
		
	
	class CSymbolNode_exclusive_or_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 130 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1994 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 132 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2003 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_exclusive_or_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_exclusive_or_expr_s;
			
		}
	};
		
	
	class CSymbolNode_and_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 143 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2023 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 145 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2032 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_and_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_and_expr_s;
			
		}
	};
		
	
	class CSymbolNode_equality_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 156 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2052 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 158 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2061 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_equality_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_equality_expr_s;
			
		}
	};
		
	
	class CSymbolNode_relational_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 169 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2081 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 171 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2090 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_relational_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_relational_expr_s;
			
		}
	};
		
	
	class CSymbolNode_shift_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 182 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2110 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 184 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_shift_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_shift_expr_s;
			
		}
	};
		
	
	class CSymbolNode_additive_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 195 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2139 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 197 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2148 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_additive_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_additive_expr_s;
			
		}
	};
		
	
	class CSymbolNode_multiplicative_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 208 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2168 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2177 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_multiplicative_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_multiplicative_expr_s;
			
		}
	};
		
	
	class CSymbolNode_at_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 221 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2197 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 223 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2206 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_at_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_at_expr_s;
			
		}
	};
		
	
	class CSymbolNode_assignment_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 234 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2226 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 236 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2235 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_assignment_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_assignment_expr_s;
			
		}
	};
		
	
	class CSymbolNode_unary_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 252 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2255 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_unary_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_unary_expr_s;
			
		}
	};
		
	
	class CSymbolNode_postfix_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 351 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2275 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_postfix_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_postfix_expr_s;
			
		}
	};
		
	
	class CSymbolNode_unary_operator_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 257 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2295 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_unary_operator_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_unary_operator_expr_s;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	
	class CSymbolNode_primary_expr_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 411 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2318 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_primary_expr_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_primary_expr_s;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_postfix_operator_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 356 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2340 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_postfix_operator_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_postfix_operator_s;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_member_operator_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 396 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2362 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_member_operator_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_member_operator_s;
			
		}
	};
		
	
	class CSymbolNode_literal_s: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 453 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2382 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 455 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		TLiteral Literal;
	;
// #line 2391 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_literal_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_literal_s;
			
		}
	};
		
	
	
	
	
	
	
	
	class CSymbolNode_if_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 104 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TIfStmt Stmt;
	;
// #line 2419 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_if_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_if_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_switch_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 130 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TSwitchStmt Stmt;
	;
// #line 2443 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_switch_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_switch_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_while_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 163 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TWhileStmt Stmt;
	;
// #line 2467 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_while_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_while_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_do_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 182 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TDoStmt Stmt;
	;
// #line 2491 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_do_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_do_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_for_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 205 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TForStmt Stmt;
	;
// #line 2515 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_for_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_for_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	
	
	class CSymbolNode_once_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 267 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TOnceStmt Stmt;
	;
// #line 2542 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_once_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_once_stmt;
			
		}
	};
		
	
	
	class CSymbolNode_switch_block_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 149 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			TSwitchStmt* pStmt;
// #line 2563 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_switch_block_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_switch_block_stmt;
			
		}
	};
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_declaration_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 19 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2605 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_declaration_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declaration_specifier;
			
		}
	};
		
	
	
	
	class CSymbolNode_pitcher_modifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 373 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2627 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_pitcher_modifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_pitcher_modifier;
			
		}
	};
		
	
	class CSymbolNode_property_template_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 268 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
		CPropertyTemplate* pTemplate;
	;
// #line 2649 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_property_template_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_property_template_specifier;
			
		}
	};
		
	
	
	class CSymbolNode_named_type_block: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 237 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CNamespace* pNamespace;
// #line 2670 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_named_type_block ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_named_type_block;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_full_declarator: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 7 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2694 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_full_declarator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_full_declarator;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_declarator: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 34 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2716 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_declarator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declarator;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_declarator_qualifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 55 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2738 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_declarator_qualifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declarator_qualifier;
			
		}
	};
		
	
	
	class CSymbolNode_array_suffix: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 310 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2759 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_array_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_array_suffix;
			
		}
	};
		
	
	class CSymbolNode_bitfield_suffix: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 411 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2779 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_bitfield_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_bitfield_suffix;
			
		}
	};
		
	
	class CSymbolNode_function_formal_argument_list: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 346 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2799 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_function_formal_argument_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_formal_argument_list;
			
		}
	};
		
	
	class CSymbolNode_function_formal_argument: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 351 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2819 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_function_formal_argument ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_formal_argument;
			
		}
	};
		
	
	class CSymbolNode_function_formal_argument_w_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 360 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2839 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 362 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclarator Declarator;
	;
// #line 2848 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_function_formal_argument_w_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_formal_argument_w_specifier;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_function_formal_argument_no_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 386 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2870 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 388 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclarator Declarator;
	;
// #line 2879 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_function_formal_argument_no_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_formal_argument_no_specifier;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	
	
	
	
	class CSymbolNode_enum_block: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 86 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2906 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_enum_block ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_enum_block;
			
		}
	};
		
	
	class CSymbolNode_enum_member_list: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 99 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2926 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_enum_member_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_enum_member_list;
			
		}
	};
		
	
	class CSymbolNode_enum_member: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 104 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2946 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_enum_member ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_enum_member;
			
		}
	};
		
	
	
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	
	CParser ();

	bool
	ParseTokenList (
		ESymbol Symbol,
		const rtl::CConstBoxListT <CToken>& TokenList,
		bool IsBuildingAst = false
		);

	bool
	PreCreateLandingPads (uint_t Flags);

	CNamedImportType*
	GetNamedImportType (
		const CQualifiedName& Name,
		const CToken::CPos& Pos
		);

	CType*
	FindType (
		const CQualifiedName& Name,
		const CToken::CPos& Pos
		);

	bool
	SetStorageKind (EStorage StorageKind);

	bool
	SetAccessKind (EAccess AccessKind);

	void
	PushTypeSpecifier (CTypeSpecifier* pTypeSpecifier)
	{
		m_TypeSpecifierStack.Append (pTypeSpecifier);
	}

	CTypeSpecifier*
	GetTypeSpecifier ()
	{
		return m_TypeSpecifierStack.GetBack ();
	}

	void
	PopTypeSpecifier (CTypeSpecifier* pTypeSpecifier)
	{
		m_TypeSpecifierStack.Pop ();
	}

	bool
	IsReactor ()
	{
		return m_pReactorType != NULL;
	}

	bool
	IsTypeSpecified ();

	bool
	IsEmptyDeclarationTerminatorAllowed (CTypeSpecifier* pTypeSpecifier);

	bool
	SetDeclarationBody (rtl::CBoxListT <CToken>* pTokenList);

	void
	AssignDeclarationAttributes (
		CModuleItem* pItem,
		const CToken::CPos& Pos
		);

	CGlobalNamespace*
	OpenGlobalNamespace (
		const CQualifiedName& Name,
		const CToken::CPos& Pos
		);

	CGlobalNamespace*
	GetGlobalNamespace (
		CGlobalNamespace* pParentNamespace,
		const rtl::CString& Name,
		const CToken::CPos& Pos
		);

	bool
	OpenTypeExtension (
		const CQualifiedName& Name,
		const CToken::CPos& Pos
		);

	bool
	PreDeclare ();

	bool
	Declare (CDeclarator* pDeclarator);

	bool
	DeclareTypedef (
		CDeclarator* pDeclarator,
		CType* pType
		);

	bool
	DeclareAlias (
		CDeclarator* pDeclarator,
		CType* pType
		);

	bool
	DeclareFunction (
		CDeclarator* pDeclarator,
		CFunctionType* pType
		);

	bool
	DeclareProperty (
		CDeclarator* pDeclarator,
		CPropertyType* pType,
		uint_t Flags
		);

	bool
	DeclareData (
		CDeclarator* pDeclarator,
		CType* pType,
		uint_t PtrTypeFlags
		);

	bool
	DeclareUnnamedStructOrUnion (CDerivableType* pType);

	bool
	DeclareReactor (
		CDeclarator* pDeclarator,
		CClassType* pType,
		uint_t PtrTypeFlags
		);

	CFunctionArg*
	CreateFormalArg (
		CDeclFunctionSuffix* pArgSuffix,
		CDeclarator* pDeclarator
		);

	CEnumType*
	CreateEnumType (
		EEnumType EnumTypeKind,
		const rtl::CString& Name,
		CType* pBaseType,
		uint_t Flags
		);

	CStructType*
	CreateStructType (
		const rtl::CString& Name,
		rtl::CBoxListT <CType*>* pBaseTypeList,
		size_t PackFactor
		);

	CUnionType*
	CreateUnionType (const rtl::CString& Name);

	CClassType*
	CreateClassType (
		const rtl::CString& Name,
		rtl::CBoxListT <CType*>* pBaseTypeList,
		size_t PackFactor,
		uint_t Flags
		);

	CProperty*
	CreateProperty (
		const rtl::CString& Name,
		const CToken::CPos& Pos
		);

	CPropertyTemplate*
	CreatePropertyTemplate ();

	bool
	FinalizeLastProperty (bool HasBody);

	bool
	ParseLastPropertyBody (const rtl::CBoxListT <CToken>& Body);

	bool
	NewOperator_s (
		EStorage StorageKind,
		CType* pType,
		CValue* pResultValue
		);

	bool
	FinalizeReactor ();

	bool
	FinalizeReactorOnChangedClause ();

	bool
	FinalizeReactorOnChangedStmt ()
	{
		return m_pModule->m_FunctionMgr.Epilogue ();
	}

	bool
	ReactorExpressionStmt (rtl::CBoxListT <CToken>* pTokenList);

	bool
	CountReactorBindSites ();

	bool
	CallBaseTypeMemberConstructor (
		const CQualifiedName& Name,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	CallBaseTypeConstructor (
		size_t BaseTypeIdx,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	CallBaseTypeConstructor (
		CType* pType,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	CallBaseTypeConstructorImpl (
		CBaseTypeSlot* pBaseTypeSlot,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	CallFieldConstructor (
		CStructField* pField,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	FinalizeBaseTypeMemberConstructBlock ();

	bool
	LookupIdentifier (
		const rtl::CString& Name,
		const CToken::CPos& Pos,
		CValue* pValue
		);

	bool
	LookupIdentifierType (
		const rtl::CString& Name,
		const CToken::CPos& Pos,
		CValue* pValue
		);

	bool
	CreateMemberClosure (CValue* pValue);

	bool
	GetCountOf (
		CType* pType,
		CValue* pValue
		);

	bool
	GetThisValue (CValue* pValue);

	bool
	GetThisValueType (CValue* pValue);

	bool
	GetPitcherReturnValue (CValue* pValue);

	bool
	GetPitcherReturnValueType (CValue* pValue);

	bool
	PrepareCurlyInitializerNamedItem (
		TCurlyInitializer* pInitializer,
		const char* pName
		);

	bool
	PrepareCurlyInitializerIndexedItem (TCurlyInitializer* pInitializer);

	bool
	SkipCurlyInitializerItem (TCurlyInitializer* pInitializer);

	bool
	AppendFmtLiteral (
		TLiteral* pLiteral,
		const void* p,
		size_t Length
		);

	bool
	AppendFmtLiteralValue (
		TLiteral* pLiteral,
		const CValue& SrcValue,
		const rtl::CString& FmtSpecifierString
		);

	bool
	AppendFmtLiteralBinValue (
		TLiteral* pLiteral,
		const CValue& SrcValue
		);

	bool
	FinalizeLiteral (
		TLiteral* pLiteral,
		CValue* pResultValue
		);

	bool
	FinalizeLiteral_s (
		TLiteral* pLiteral,
		CValue* pResultValue
		);


	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	static
	size_t*
	GetParseTable ()
	{
		static
		size_t _ParseTable [] = 
		{
			0, -1, -1, -1, -1, 486, -1, -1, -1, -1, -1, 486, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 486, -1, -1, -1, -1, -1, -1, 486, 486, 486, 486, 486, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 486, 486, -1, 486, 486, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, -1, -1, -1, 486, 486, 486, 486, 486, 486, 486, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, 165, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 337, -1, -1, -1, -1, -1, -1, 159, 160, 161, 162, 1716, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 337, 165, -1, 337, 337, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 165, 165, 1717, 165, 165, 165, 165, 165, 165, 165, 165, 165, 1712, 1714, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 337, 337, 337, -1, -1, -1, 164, 164, 164, 164, 164, 164, 164, 
			-1, -1, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 986, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 987, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 488, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 494, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 496, 496, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 969, 969, 969, 970, 971, 972, 972, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 506, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 506, -1, -1, 506, 506, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 506, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 506, 506, 506, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, 503, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, 503, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, 165, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 338, -1, -1, -1, -1, -1, -1, -1, -1, -1, 162, 1722, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 338, 165, -1, 338, 338, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 165, 165, 1723, 165, 165, 165, 165, 165, 165, 165, 165, 165, 1718, 1720, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 338, 338, 338, -1, -1, -1, 164, 164, 164, 164, 164, 164, 164, 
			-1, -1, -1, 170, -1, 170, 170, -1, -1, -1, -1, 165, -1, -1, -1, -1, -1, 170, -1, -1, 170, 170, 1750, -1, -1, -1, 170, 170, -1, -1, -1, 161, -1, 1724, 171, 172, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 170, 170, 170, 170, 170, 339, 165, -1, 339, 339, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, -1, -1, 170, -1, 170, -1, -1, 170, 170, 170, 170, 170, 170, 170, -1, 1726, 1728, 1730, 1732, 1734, 1736, 1738, 1740, 1742, 1744, 1746, 1748, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 339, 339, 339, -1, -1, -1, 164, 164, 164, 164, 164, 164, 164, 
			-1, -1, -1, 275, -1, 5, 270, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 275, -1, -1, 275, 275, 275, -1, -1, -1, 275, 275, -1, -1, -1, -1, -1, 275, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 275, 275, 1775, 275, 275, -1, -1, -1, -1, -1, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, -1, -1, 276, -1, 277, -1, -1, 278, 279, 280, 281, 282, 283, 284, -1, 1751, 1753, 1755, 1757, 1759, 1761, 1763, 1765, 1767, 1769, 1771, 1773, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 783, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 784, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 497, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 835, 836, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 498, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 812, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 812, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 812, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, -1, -1, 880, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, 880, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 507, 508, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 942, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 889, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 889, -1, -1, 889, 889, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 889, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 889, 889, 889, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1777, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 945, -1, -1, -1, -1, -1, 463, -1, 944, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 968, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 509, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 510, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 515, 515, 515, 515, 515, 512, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 513, 514, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 519, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 519, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 523, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 523, -1, -1, 523, 523, 523, -1, -1, -1, 523, 523, -1, -1, -1, -1, -1, 1779, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 523, 523, 523, 523, 523, -1, 522, -1, -1, -1, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, -1, -1, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, -1, -1, 670, 670, 670, -1, -1, -1, 670, 670, -1, -1, -1, -1, -1, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 526, -1, -1, -1, 526, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 526, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 528, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 
			-1, 537, 537, 535, 537, 537, 531, 537, 537, 537, 537, 533, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 
			-1, -1, -1, 539, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 542, 542, 541, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 
			-1, -1, -1, 543, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 543, -1, -1, 543, 543, 543, -1, -1, -1, 543, 543, -1, -1, -1, -1, -1, 543, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 543, 543, 543, 543, 543, -1, -1, -1, -1, -1, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 545, -1, -1, 545, 545, 545, -1, -1, -1, 545, 545, -1, -1, -1, -1, -1, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 545, 545, 545, 545, 545, -1, -1, -1, -1, -1, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			544, -1, -1, 544, -1, -1, -1, -1, 544, -1, -1, -1, -1, -1, -1, -1, -1, 544, -1, -1, 544, 544, 544, -1, -1, -1, 544, 544, -1, -1, -1, -1, -1, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, 544, 544, 544, 544, -1, -1, -1, -1, -1, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 549, 0, 0, -1, -1, 552, -1, -1, -1, -1, -1, -1, -1, -1, 549, -1, -1, 549, 549, 549, -1, -1, -1, 549, 549, -1, -1, -1, -1, -1, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 549, 549, 549, 549, 549, -1, -1, -1, -1, -1, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, -1, -1, 555, 555, 555, -1, -1, -1, 555, 555, -1, -1, -1, -1, -1, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 546, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 546, -1, -1, 546, 546, 546, -1, -1, -1, 546, 546, -1, -1, -1, -1, -1, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, 546, 546, 546, 546, -1, -1, -1, -1, -1, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, -1, -1, 547, 547, 547, -1, -1, -1, 547, 547, -1, -1, -1, -1, -1, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 553, -1, -1, 553, 553, 553, -1, -1, -1, 553, 553, -1, -1, -1, -1, -1, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 553, 553, 553, 553, 553, -1, -1, -1, -1, -1, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, -1, -1, 554, 554, 554, -1, -1, -1, 554, 554, -1, -1, -1, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 557, -1, -1, 557, 557, 557, -1, -1, -1, 557, 557, -1, -1, -1, -1, -1, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 557, 557, 557, 557, 557, -1, -1, -1, -1, -1, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 559, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 559, -1, -1, 559, 559, 559, -1, -1, -1, 559, 559, -1, -1, -1, -1, -1, 559, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 559, 559, 559, 559, 559, -1, -1, -1, -1, -1, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 561, -1, -1, 561, 561, 561, -1, -1, -1, 561, 561, -1, -1, -1, -1, -1, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 561, 561, 561, 561, 561, -1, -1, -1, -1, -1, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 563, -1, -1, 563, 563, 563, -1, -1, -1, 563, 563, -1, -1, -1, -1, -1, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 563, 563, 563, 563, 563, -1, -1, -1, -1, -1, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 565, -1, -1, 565, 565, 565, -1, -1, -1, 565, 565, -1, -1, -1, -1, -1, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 565, 565, 565, 565, 565, -1, -1, -1, -1, -1, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 567, -1, -1, 567, 567, 567, -1, -1, -1, 567, 567, -1, -1, -1, -1, -1, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 567, 567, 567, 567, 567, -1, -1, -1, -1, -1, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 571, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 571, -1, -1, 571, 571, 571, -1, -1, -1, 571, 571, -1, -1, -1, -1, -1, 571, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 571, 571, 571, 571, 571, -1, -1, -1, -1, -1, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 569, 570, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 577, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 577, -1, -1, 577, 577, 577, -1, -1, -1, 577, 577, -1, -1, -1, -1, -1, 577, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 577, 577, 577, 577, 577, -1, -1, -1, -1, -1, 577, 577, 577, 577, 577, 577, 577, 577, 577, 577, 577, 577, 577, 577, 577, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 577, 577, 577, 577, 577, 577, 577, 577, 577, 577, 577, 577, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 573, 574, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 575, 576, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 581, -1, -1, 581, 581, 581, -1, -1, -1, 581, 581, -1, -1, -1, -1, -1, 581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 581, 581, 581, 581, 581, -1, -1, -1, -1, -1, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 579, 580, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 585, -1, -1, 585, 585, 585, -1, -1, -1, 585, 585, -1, -1, -1, -1, -1, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 585, 585, 585, 585, 585, -1, -1, -1, -1, -1, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 583, 584, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 590, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 590, -1, -1, 590, 590, 590, -1, -1, -1, 590, 590, -1, -1, -1, -1, -1, 590, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 590, 590, 590, 590, 590, -1, -1, -1, -1, -1, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, 590, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 587, 588, 589, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 592, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 592, -1, -1, 592, 592, 592, -1, -1, -1, 592, 592, -1, -1, -1, -1, -1, 592, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 592, 592, 592, 592, 592, -1, -1, -1, -1, -1, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1780, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 609, -1, -1, 609, 609, 609, -1, -1, -1, 609, 609, -1, -1, -1, -1, -1, 608, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 609, 609, 609, 609, 609, -1, -1, -1, -1, -1, 608, 608, 608, 608, 608, 608, 608, 608, 608, 608, 608, 608, 608, 608, 608, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 609, 609, 609, 609, 609, 609, 609, 609, 609, 609, 609, 609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1781, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 596, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 661, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 627, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 627, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 627, 627, 627, 627, 627, 627, 627, 627, 627, 627, 627, 627, 627, 627, 627, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 378, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 613, -1, -1, 610, 611, 614, -1, -1, -1, 612, 615, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 616, 617, 618, 620, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 626, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 834, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1783, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 623, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, 623, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 229, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 625, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 857, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 821, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 822, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 821, 821, 821, 821, 821, 821, 821, 821, 821, 821, 821, 821, 821, 821, 1784, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 640, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654, 641, 641, 641, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 629, -1, -1, -1, -1, -1, -1, 635, 631, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 630, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 632, 633, -1, -1, -1, -1, -1, 636, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 637, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 638, 639, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 658, 658, 658, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 660, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 660, -1, -1, 660, 660, 660, -1, -1, -1, 660, 660, -1, -1, -1, -1, -1, 660, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 660, 660, 660, 660, 660, -1, -1, -1, -1, -1, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 666, -1, -1, 667, 668, 668, -1, -1, -1, -1, -1, -1, -1, -1, 666, -1, -1, 666, 666, 666, -1, -1, -1, 666, 666, -1, -1, -1, -1, -1, 1785, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 666, 666, 666, 666, 666, -1, -1, -1, -1, -1, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 669, -1, -1, 669, 669, 669, -1, -1, -1, 669, 669, -1, -1, -1, -1, -1, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 669, 669, 669, 669, 669, -1, -1, -1, -1, -1, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 678, -1, -1, 678, 678, 678, -1, -1, -1, 678, 678, -1, -1, -1, -1, -1, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 678, 678, 678, 678, 678, -1, -1, -1, -1, -1, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 671, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 671, -1, -1, 671, 671, 671, -1, -1, -1, 671, 671, -1, -1, -1, -1, -1, 671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 671, 671, 671, 671, 671, -1, -1, -1, -1, -1, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, -1, -1, 672, 672, 672, -1, -1, -1, 672, 672, -1, -1, -1, -1, -1, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 674, 0, -1, -1, -1, 677, -1, -1, -1, -1, -1, -1, -1, -1, 674, -1, -1, 674, 674, 674, -1, -1, -1, 674, 674, -1, -1, -1, -1, -1, 674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 674, 674, 674, 674, 674, -1, -1, -1, -1, -1, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 680, -1, -1, 680, 680, 680, -1, -1, -1, 680, 680, -1, -1, -1, -1, -1, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 680, 680, 680, 680, 680, -1, -1, -1, -1, -1, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, -1, -1, 682, 682, 682, -1, -1, -1, 682, 682, -1, -1, -1, -1, -1, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, 682, 682, 682, 682, -1, -1, -1, -1, -1, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 684, -1, -1, 684, 684, 684, -1, -1, -1, 684, 684, -1, -1, -1, -1, -1, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 684, 684, 684, 684, 684, -1, -1, -1, -1, -1, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, -1, -1, 686, 686, 686, -1, -1, -1, 686, 686, -1, -1, -1, -1, -1, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, 686, 686, 686, 686, -1, -1, -1, -1, -1, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 688, -1, -1, 688, 688, 688, -1, -1, -1, 688, 688, -1, -1, -1, -1, -1, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 688, 688, 688, 688, 688, -1, -1, -1, -1, -1, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 690, -1, -1, 690, 690, 690, -1, -1, -1, 690, 690, -1, -1, -1, -1, -1, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 690, 690, 690, 690, 690, -1, -1, -1, -1, -1, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 692, -1, -1, 692, 692, 692, -1, -1, -1, 692, 692, -1, -1, -1, -1, -1, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 692, 692, 692, 692, 692, -1, -1, -1, -1, -1, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, -1, -1, 694, 694, 694, -1, -1, -1, 694, 694, -1, -1, -1, -1, -1, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, 694, 694, 694, 694, -1, -1, -1, -1, -1, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, -1, -1, 696, 696, 696, -1, -1, -1, 696, 696, -1, -1, -1, -1, -1, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, 696, 696, 696, 696, -1, -1, -1, -1, -1, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, -1, -1, 698, 698, 698, -1, -1, -1, 698, 698, -1, -1, -1, -1, -1, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, 698, 698, 698, 698, -1, -1, -1, -1, -1, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 700, -1, -1, 700, 700, 700, -1, -1, -1, 700, 700, -1, -1, -1, -1, -1, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 700, 700, 700, 700, 700, -1, -1, -1, -1, -1, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 702, -1, -1, 702, 702, 702, -1, -1, -1, 702, 702, -1, -1, -1, -1, -1, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 702, 702, 702, 702, 702, -1, -1, -1, -1, -1, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1786, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 705, -1, -1, 705, 705, 705, -1, -1, -1, 705, 705, -1, -1, -1, -1, -1, 704, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 705, 705, 705, 705, 705, -1, -1, -1, -1, -1, 704, 704, 704, 704, 704, 704, 704, 705, 705, 705, 705, 705, 704, 704, 704, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 725, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 725, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 725, 725, 725, 725, 725, 725, 725, -1, -1, -1, -1, -1, 725, 725, 725, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 410, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 709, -1, -1, 706, 707, 710, -1, -1, -1, 708, 711, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 712, 713, 714, 721, 722, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 715, 716, 717, 718, 719, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1787, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 723, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 747, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 738, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 739, 740, 741, 742, 743, 744, 745, -1, -1, -1, -1, -1, 746, 746, 746, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 727, -1, -1, -1, -1, -1, -1, 733, 729, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 728, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 730, 731, -1, -1, -1, -1, -1, 734, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 735, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 736, 737, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 751, 751, 751, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 753, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 755, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 422, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 759, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 760, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 762, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 764, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 764, -1, -1, 764, 764, 764, -1, -1, -1, 764, 764, -1, -1, -1, -1, -1, 764, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 764, 764, 764, 764, 764, -1, -1, -1, -1, -1, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 765, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 767, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 771, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 772, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 773, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 777, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 778, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 779, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 780, -1, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 781, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 169, -1, 169, 169, -1, -1, -1, -1, 169, -1, -1, -1, -1, -1, 169, -1, -1, 169, 169, 169, -1, -1, -1, 169, 169, -1, -1, -1, 169, -1, 169, 169, 169, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 169, 169, 169, 169, 169, 169, 169, -1, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, -1, -1, 169, -1, 169, 769, 770, 169, 169, 169, 169, 169, 169, 169, -1, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, -1, -1, -1, 169, 169, 169, 169, 169, 169, 169, 
			-1, -1, -1, -1, -1, -1, 786, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 290, 290, 290, 290, 5, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 1829, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 787, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 
			-1, 293, 293, 293, 293, 5, 292, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 294, 293, 295, 293, 293, 296, 297, 298, 299, 300, 301, 302, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 
			-1, -1, -1, -1, -1, -1, 790, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 798, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 800, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 801, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 802, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 803, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 804, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 805, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 806, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 807, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 
			-1, 1, 1, 797, 1, 1, 1, 1, 1, 1, 1, 795, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, -1, -1, -1, -1, -1, 809, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 308, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 308, -1, -1, 308, 308, 308, -1, -1, -1, 308, 308, -1, -1, -1, -1, -1, 308, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 308, 308, 308, 308, 308, -1, -1, -1, -1, -1, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 448, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, 308, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 810, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 811, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 811, -1, -1, 811, 811, 811, -1, -1, -1, 811, 811, -1, -1, -1, -1, -1, 811, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 811, 811, 811, 811, 811, -1, -1, -1, -1, -1, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 193, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 816, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 817, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 814, 814, 814, 814, 814, 814, 814, 814, 814, 814, 814, 814, 815, 815, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 1788, 817, 817, 817, 817, 817, 817, 1789, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 452, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 837, 838, 1791, 841, 842, 843, 844, 845, 846, 847, 848, 849, 850, 851, 451, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 864, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 871, 859, 860, 861, 862, 863, 865, 866, 867, 868, 869, 870, 872, 873, 874, 875, 876, 877, 878, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 879, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 858, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 855, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 335, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, -1, -1, 882, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, 882, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, 886, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, 886, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 891, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 895, -1, -1, 892, 893, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1793, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 894, 896, 1795, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 940, -1, 925, -1, 918, 917, 916, 921, 923, 909, 910, 911, 912, 913, 941, 902, 903, -1, -1, -1, -1, -1, -1, -1, -1, 939, 938, 919, 920, 922, 924, 914, 915, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 904, 905, -1, -1, -1, -1, -1, 908, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1797, 937, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1799, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 967, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 955, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, -1, -1, -1, -1, -1, 956, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 957, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 957, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 957, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 962, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 973, 973, 973, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 983, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 988, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 989, 989, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 975, 976, 977, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 978, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 979, 979, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 979, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 982, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 985, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 992, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, -1, 486, -1, -1, -1, -1, -1, 486, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 486, -1, -1, -1, -1, -1, -1, 486, 486, 486, 486, 486, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 486, 486, -1, 486, 486, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, -1, -1, -1, 486, 486, 486, 486, 486, 486, 486, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 489, -1, 0, -1, -1, -1, 489, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 489, -1, -1, -1, -1, -1, -1, 489, 489, 489, 489, 489, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 489, 489, -1, 489, 489, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, -1, -1, -1, 489, 489, 489, 489, 489, 489, 489, 
			-1, -1, -1, -1, -1, 491, -1, 0, -1, -1, -1, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, 491, -1, 491, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, -1, -1, -1, 491, 491, 491, 491, 491, 491, 491, 
			-1, -1, -1, -1, -1, 0, -1, -1, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 495, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 499, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, -1, -1, 500, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, 500, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 504, -1, 0, 0, -1, -1, 504, -1, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 505, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 511, 511, 511, 511, 511, 511, 0, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 517, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 518, 0, 0, 0, 0, 0, 1801, 0, 518, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 525, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, 527, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 530, 530, 530, 0, 0, 530, 0, 0, 530, 530, 530, 0, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 
			-1, 532, 532, 532, 532, 532, 532, 0, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 
			-1, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 0, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 
			-1, 536, 536, 536, 0, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 
			-1, 538, 538, 538, 0, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 
			-1, 540, 540, 540, 0, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 
			-1, -1, -1, -1, 0, -1, -1, -1, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 550, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 551, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 556, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 558, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 560, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 562, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 564, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 566, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 568, 568, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 572, 572, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 572, 572, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 578, 578, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 582, 582, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 586, 586, 586, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 591, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 593, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 593, 593, 593, 593, 593, 593, 593, 593, 593, 593, 593, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1587, 1587, 1587, 1587, 1587, 1587, 1587, 1587, 1587, 1587, 1587, 1587, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1589, 1589, 1589, 1589, 1589, 1589, 1589, 1589, 1589, 1589, 1589, 1589, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 621, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 624, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 628, 0, 0, -1, 0, 0, 0, 628, 628, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 628, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 628, 628, -1, -1, -1, -1, -1, 628, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 634, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 655, 656, 657, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 659, 659, 659, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 1607, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 662, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 665, -1, -1, 229, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 665, -1, -1, 665, 665, 665, -1, -1, -1, 665, 665, -1, -1, -1, -1, -1, 665, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 665, 665, 665, 665, 665, -1, -1, -1, -1, -1, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 664, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 673, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 675, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 676, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 679, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 681, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 683, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 685, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 687, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 689, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 691, 691, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 693, 693, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 693, 693, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 695, 695, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 697, 697, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 699, 699, 699, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 701, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 703, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 703, 703, 703, 703, 703, 703, 703, 703, 703, 703, 703, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 720, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1625, 1625, 1625, 1625, 1625, 1625, 1625, 1625, 1625, 1625, 1625, 1625, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 724, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 726, 0, 0, -1, -1, 0, 0, 726, 726, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 726, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 726, 726, -1, -1, -1, -1, -1, 726, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 732, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 748, 749, 750, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 80, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 752, 752, 752, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 754, -1, 754, 754, 0, -1, -1, -1, 754, -1, -1, -1, -1, -1, 754, -1, -1, 754, 754, 754, -1, -1, -1, 754, 754, -1, -1, -1, 754, -1, 754, 754, 754, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 754, 754, 754, 754, 754, 754, 754, -1, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, -1, -1, 754, -1, 754, -1, -1, 754, 754, 754, 754, 754, 754, 754, -1, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, -1, -1, -1, 754, 754, 754, 754, 754, 754, 754, 
			-1, -1, -1, 0, -1, 756, 0, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, 0, -1, 1802, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 756, 0, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 757, -1, 757, 757, 0, -1, -1, -1, 757, -1, -1, -1, -1, -1, 757, -1, -1, 757, 757, 757, -1, -1, -1, 757, 757, -1, -1, -1, 757, -1, 757, 757, 757, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 757, 757, 757, 757, 757, 757, 757, -1, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, -1, -1, 757, -1, 757, -1, -1, 757, 757, 757, 757, 757, 757, 757, -1, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, -1, -1, -1, 757, 757, 757, 757, 757, 757, 757, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 758, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 761, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 763, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 766, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 768, -1, 768, 768, 0, -1, -1, -1, 768, -1, -1, -1, -1, -1, 768, -1, -1, 768, 768, 768, -1, -1, -1, 768, 768, -1, -1, -1, 768, -1, 768, 768, 768, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 768, 768, 768, 768, 768, 768, 768, -1, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, -1, -1, 768, -1, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, -1, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, -1, -1, -1, 768, 768, 768, 768, 768, 768, 768, 
			-1, -1, -1, 774, -1, 774, -1, -1, 774, -1, -1, 165, -1, -1, -1, -1, -1, 774, -1, -1, 774, 774, 774, -1, -1, -1, 774, 774, -1, -1, -1, -1, -1, 1803, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 774, 774, 774, 774, 774, -1, 165, -1, -1, -1, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1804, 1806, 1808, 1810, 1812, 1814, 1816, 1818, 1820, 1822, 1824, 1826, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 775, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, -1, -1, 775, 775, 775, -1, -1, -1, 775, 775, -1, -1, -1, -1, -1, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, 775, 775, 775, 775, -1, -1, -1, -1, -1, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 776, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 776, -1, -1, 776, 776, 776, -1, -1, -1, 776, 776, -1, -1, -1, -1, -1, 776, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 776, 776, 776, 776, 776, -1, -1, -1, -1, -1, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1661, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1661, -1, -1, 1661, 1661, 1661, -1, -1, -1, 1661, 1661, -1, -1, -1, -1, -1, 1661, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1661, 1661, 1661, 1661, 1661, -1, -1, -1, -1, -1, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, 1661, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 1662, 1662, 1662, 1662, 1662, 1662, 1662, 1662, 1662, 1662, 1662, 1662, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 782, -1, 782, 782, 0, -1, -1, -1, 782, -1, -1, -1, -1, -1, 782, -1, -1, 782, 782, 782, -1, -1, -1, 782, 782, -1, -1, -1, 782, -1, 782, 782, 782, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 782, 782, 782, 782, 782, 782, 782, -1, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, -1, -1, 782, -1, 782, -1, -1, 782, 782, 782, 782, 782, 782, 782, -1, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, -1, -1, -1, 782, 782, 782, 782, 782, 782, 782, 
			-1, 785, 785, 785, 785, 785, 785, 0, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 289, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 1, 1, 1, 1, 1, 292, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, 789, 789, 789, 789, 789, 789, 0, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 
			-1, 793, 793, 793, 0, 0, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 
			-1, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 0, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 
			-1, 796, 796, 796, 0, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 
			-1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 799, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 303, 303, 303, 303, 0, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 
			-1, 303, 303, 303, 303, 0, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 
			-1, 303, 303, 303, 0, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 
			-1, 303, 303, 303, 303, 0, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 
			-1, 303, 303, 303, 303, 0, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 
			-1, 303, 303, 303, 303, 0, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 303, 
			-1, -1, -1, 808, -1, 808, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 808, -1, -1, 808, 808, 808, -1, -1, -1, 808, 808, -1, -1, -1, -1, -1, 808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 808, 808, 808, 808, 808, -1, -1, -1, -1, -1, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 307, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, -1, 0, 0, -1, 813, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1828, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 813, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 820, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 820, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 853, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 854, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 856, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, 0, -1, 881, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 883, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 884, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 885, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 887, -1, 0, 0, -1, 0, 887, -1, 887, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 888, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, -1, 0, 0, 890, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 145, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 943, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 946, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 953, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 958, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 959, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 959, -1, -1, 959, 959, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 959, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 959, 959, 959, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 960, 0, -1, -1, -1, 0, 960, -1, 960, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 961, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 963, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 964, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 964, -1, -1, 964, 964, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 964, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 964, 964, 964, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 965, 0, -1, -1, -1, 0, 965, -1, 965, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 966, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 974, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 980, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, 981, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1702, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1703, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 984, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1707, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1708, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1709, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 990, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 991, -1, 0, -1, -1, -1, 991, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 991, -1, -1, -1, -1, -1, -1, -1, -1, -1, 991, 991, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 991, 991, -1, 991, 991, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, 991, -1, -1, -1, 991, 991, 991, 991, 991, 991, 991, 
			-1, -1, 157, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			
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
			336, 156,  -1, // 0
			993, 4, 1546, 3, 28, 2,  -1, // 1
			996, 7, 340, 6, 995, 1547, 29,  -1, // 2
			340, 156,  -1, // 3
			998, 7, 341, 6, 997, 1548, 30,  -1, // 4
			341, 168,  -1, // 5
			5, 342, 173, 31,  -1, // 6
			342, 173, 8,  -1, // 7
			5, 343, 173, 32,  -1, // 8
			343, 173, 8,  -1, // 9
			999, 9, 1549,  -1, // 10
			1383, 175,  -1, // 11
			344, 1000, 1550,  -1, // 12
			344, 1001, 1551, 10,  -1, // 13
			1384, 177,  -1, // 14
			1385, 178, 345, 1552,  -1, // 15
			346, 1386, 179,  -1, // 16
			1391, 178, 1002, 348, 347, 1388, 181, 1387, 180, 346,  -1, // 17
			347, 1389, 182,  -1, // 18
			348, 1390, 183,  -1, // 19
			3, 1392, 180,  -1, // 20
			1003, 5,  -1, // 21
			1004, 1553,  -1, // 22
			1393, 185,  -1, // 23
			1006, 1555, 349, 1005, 1554,  -1, // 24
			349, 1394, 186,  -1, // 25
			1395, 185,  -1, // 26
			1007, 1556,  -1, // 27
			1008, 1557,  -1, // 28
			1009, 1558,  -1, // 29
			1011, 351, 350, 1010, 1559,  -1, // 30
			350, 1396, 179,  -1, // 31
			351, 1397, 182,  -1, // 32
			1398, 187,  -1, // 33
			352, 1012, 1560,  -1, // 34
			352, 1013, 1561, 8,  -1, // 35
			1014, 1562,  -1, // 36
			1015, 1563,  -1, // 37
			12, 194, 1016, 11,  -1, // 38
			353, 195, 8,  -1, // 39
			353, 195,  -1, // 40
			196, 13,  -1, // 41
			1017, 354, 1564,  -1, // 42
			355, 1399, 197,  -1, // 43
			355, 1399, 197,  -1, // 44
			1019, 1566, 356, 1018, 1565,  -1, // 45
			356, 1400, 197,  -1, // 46
			1021, 1568, 357, 1020, 1567,  -1, // 47
			357, 1401, 197,  -1, // 48
			1023, 1570, 358, 1022, 1569,  -1, // 49
			358, 1402, 197,  -1, // 50
			1024, 1571,  -1, // 51
			359, 1403, 199,  -1, // 52
			4, 359, 3,  -1, // 53
			360, 1404, 199,  -1, // 54
			4, 360, 3,  -1, // 55
			1025, 1572,  -1, // 56
			1026, 1573,  -1, // 57
			1027, 1574,  -1, // 58
			1405, 204,  -1, // 59
			1406, 204,  -1, // 60
			361, 1028, 1575,  -1, // 61
			361, 1029, 1576, 8,  -1, // 62
			362, 1030, 1577,  -1, // 63
			362, 1031, 1578, 8,  -1, // 64
			363, 1033, 1579, 8,  -1, // 65
			363, 1033, 1579, 8, 1032,  -1, // 66
			1034, 1407, 204,  -1, // 67
			1035, 1580,  -1, // 68
			364, 1408, 209,  -1, // 69
			1038, 1410, 204, 9, 1037, 1409, 204, 14, 1036,  -1, // 70
			365, 1411, 210,  -1, // 71
			365, 1039, 1412, 210, 36,  -1, // 72
			366, 1413, 211,  -1, // 73
			366, 1040, 1414, 211, 37,  -1, // 74
			367, 1415, 212,  -1, // 75
			367, 1041, 1416, 212, 15,  -1, // 76
			368, 1417, 213,  -1, // 77
			368, 1042, 1418, 213, 16,  -1, // 78
			369, 1419, 214,  -1, // 79
			369, 1043, 1420, 214, 17,  -1, // 80
			370, 1421, 215,  -1, // 81
			370, 1044, 1422, 215, 1581,  -1, // 82
			1045, 38,  -1, // 83
			1046, 39,  -1, // 84
			371, 1423, 217,  -1, // 85
			371, 1047, 1424, 217, 1582,  -1, // 86
			1048, 18,  -1, // 87
			1049, 19,  -1, // 88
			1050, 40,  -1, // 89
			1051, 41,  -1, // 90
			372, 1425, 219,  -1, // 91
			372, 1052, 1426, 219, 1583,  -1, // 92
			1053, 42,  -1, // 93
			1054, 43,  -1, // 94
			373, 1427, 221,  -1, // 95
			373, 1055, 1428, 221, 1584,  -1, // 96
			1056, 20,  -1, // 97
			1057, 21,  -1, // 98
			374, 1429, 223,  -1, // 99
			374, 1058, 1430, 223, 1585,  -1, // 100
			1059, 22,  -1, // 101
			1060, 23,  -1, // 102
			1061, 24,  -1, // 103
			375, 1431, 225,  -1, // 104
			375, 1062, 1432, 225, 25,  -1, // 105
			376, 1433, 226,  -1, // 106
			1434, 227,  -1, // 107
			1063, 1435, 204, 1586,  -1, // 108
			229, 1064, 13,  -1, // 109
			1065, 13,  -1, // 110
			1066, 44,  -1, // 111
			1067, 45,  -1, // 112
			1068, 46,  -1, // 113
			1069, 47,  -1, // 114
			1070, 48,  -1, // 115
			1071, 49,  -1, // 116
			1072, 50,  -1, // 117
			1073, 51,  -1, // 118
			1074, 52,  -1, // 119
			1075, 53,  -1, // 120
			1076, 54,  -1, // 121
			1436, 230,  -1, // 122
			1437, 231,  -1, // 123
			1077, 1438, 226, 20,  -1, // 124
			1078, 1439, 226, 21,  -1, // 125
			1079, 1440, 226, 26,  -1, // 126
			1080, 1441, 226, 17,  -1, // 127
			1081, 1442, 226, 22,  -1, // 128
			1082, 1443, 226, 27,  -1, // 129
			1083, 1444, 226, 55,  -1, // 130
			1084, 1445, 226, 56,  -1, // 131
			1086, 1446, 226, 1085, 57,  -1, // 132
			1087, 1447, 226, 4, 1588, 377, 3,  -1, // 133
			380, 1088, 1590, 58, 379,  -1, // 134
			1448, 235,  -1, // 135
			1089, 1449, 226, 59,  -1, // 136
			1450, 187,  -1, // 137
			1090, 4, 1592, 3, 381, 1591,  -1, // 138
			3, 383, 173,  -1, // 139
			1451, 238, 384, 3,  -1, // 140
			385, 1452, 239,  -1, // 141
			385, 1453, 240,  -1, // 142
			1091, 4, 1593, 3,  -1, // 143
			1092, 4, 1594, 3, 26,  -1, // 144
			1093, 12, 1595, 11,  -1, // 145
			1094, 55,  -1, // 146
			1095, 56,  -1, // 147
			1096, 61,  -1, // 148
			1454, 241, 386, 10,  -1, // 149
			1455, 241, 1097, 62,  -1, // 150
			1098, 1596,  -1, // 151
			1099, 63,  -1, // 152
			1100, 64,  -1, // 153
			4, 1456, 204, 3,  -1, // 154
			1457, 242,  -1, // 155
			1101, 1597,  -1, // 156
			1102, 65,  -1, // 157
			1103, 66,  -1, // 158
			1104, 1598,  -1, // 159
			1105, 1599,  -1, // 160
			1106, 69,  -1, // 161
			1107, 70,  -1, // 162
			1108, 71,  -1, // 163
			1109, 4, 1600, 3, 72,  -1, // 164
			1110, 4, 1601, 3, 73,  -1, // 165
			1111, 4, 1602, 3, 74,  -1, // 166
			1112, 4, 33, 8, 189, 3, 75,  -1, // 167
			1113, 4, 1458, 204, 3, 76,  -1, // 168
			1114, 1603,  -1, // 169
			1115, 1604,  -1, // 170
			1459, 243, 1116, 1605,  -1, // 171
			1117, 388, 387,  -1, // 172
			388, 387,  -1, // 173
			1118, 389, 1606,  -1, // 174
			1119, 7, 390, 1460, 244, 6,  -1, // 175
			390, 1461, 244, 8,  -1, // 176
			13, 33,  -1, // 177
			391, 1120, 13, 1608,  -1, // 178
			1121, 1609,  -1, // 179
			1123, 1610, 1122,  -1, // 180
			229, 1124,  -1, // 181
			1125, 0,  -1, // 182
			1126, 1611,  -1, // 183
			1462, 246,  -1, // 184
			1463, 246,  -1, // 185
			393, 1127, 1612,  -1, // 186
			393, 1128, 1613, 8,  -1, // 187
			394, 1129, 1614,  -1, // 188
			394, 1130, 1615, 8,  -1, // 189
			395, 1132, 1616, 8,  -1, // 190
			395, 1132, 1616, 8, 1131,  -1, // 191
			396, 1464, 250,  -1, // 192
			1133, 1466, 246, 9, 1465, 246, 14,  -1, // 193
			397, 1467, 251,  -1, // 194
			397, 1134, 1468, 251, 36,  -1, // 195
			398, 1469, 252,  -1, // 196
			398, 1135, 1470, 252, 37,  -1, // 197
			399, 1471, 253,  -1, // 198
			399, 1136, 1472, 253, 15,  -1, // 199
			400, 1473, 254,  -1, // 200
			400, 1137, 1474, 254, 16,  -1, // 201
			401, 1475, 255,  -1, // 202
			401, 1138, 1476, 255, 17,  -1, // 203
			402, 1477, 256,  -1, // 204
			402, 1139, 1478, 256, 1617,  -1, // 205
			403, 1479, 257,  -1, // 206
			403, 1140, 1480, 257, 1618,  -1, // 207
			404, 1481, 258,  -1, // 208
			404, 1141, 1482, 258, 1619,  -1, // 209
			405, 1483, 259,  -1, // 210
			405, 1142, 1484, 259, 1620,  -1, // 211
			406, 1485, 260,  -1, // 212
			406, 1143, 1486, 260, 1621,  -1, // 213
			407, 1487, 261,  -1, // 214
			407, 1144, 1488, 261, 25,  -1, // 215
			408, 1489, 262,  -1, // 216
			1146, 1490, 246, 1622, 1145,  -1, // 217
			1491, 263,  -1, // 218
			1492, 264,  -1, // 219
			1147, 1493, 262, 20,  -1, // 220
			1148, 1494, 262, 21,  -1, // 221
			1149, 1495, 262, 26,  -1, // 222
			1150, 1496, 262, 17,  -1, // 223
			1151, 1497, 262, 22,  -1, // 224
			1152, 1498, 262, 27,  -1, // 225
			1153, 1499, 262, 55,  -1, // 226
			1154, 1500, 262, 56,  -1, // 227
			1155, 1501, 262, 57,  -1, // 228
			1156, 4, 191, 3, 72,  -1, // 229
			1157, 4, 191, 3, 73,  -1, // 230
			1158, 4, 1623, 3, 74,  -1, // 231
			1159, 4, 33, 8, 189, 3, 75,  -1, // 232
			1160, 4, 1502, 246, 3, 76,  -1, // 233
			1161, 1503, 262, 4, 1624, 409, 3,  -1, // 234
			1162, 1626, 58, 411,  -1, // 235
			1163, 1504, 262, 59,  -1, // 236
			1164, 1627,  -1, // 237
			1165, 4, 249, 3, 412, 1628,  -1, // 238
			414, 1505, 266,  -1, // 239
			414, 1506, 267,  -1, // 240
			1166, 4, 1629, 3,  -1, // 241
			1167, 4, 1630, 3, 26,  -1, // 242
			1168, 12, 1631, 11,  -1, // 243
			1169, 55,  -1, // 244
			1170, 56,  -1, // 245
			1171, 61,  -1, // 246
			1507, 268, 415, 10,  -1, // 247
			1508, 268, 1172, 62,  -1, // 248
			1173, 1632,  -1, // 249
			1174, 63,  -1, // 250
			1175, 64,  -1, // 251
			1176, 1633,  -1, // 252
			1177, 65,  -1, // 253
			1178, 66,  -1, // 254
			1179, 1634,  -1, // 255
			1180, 68,  -1, // 256
			1181, 69,  -1, // 257
			1182, 70,  -1, // 258
			1183, 71,  -1, // 259
			1509, 269,  -1, // 260
			4, 1510, 246, 3,  -1, // 261
			1184, 1635,  -1, // 262
			1185, 1636,  -1, // 263
			1186, 417, 201, 79,  -1, // 264
			1187, 418, 416,  -1, // 265
			418, 416,  -1, // 266
			1189, 7, 419, 1188, 1637,  -1, // 267
			419, 169,  -1, // 268
			1192, 7, 421, 1191, 420, 1190, 1638,  -1, // 269
			420, 272,  -1, // 270
			421, 169,  -1, // 271
			1193, 4, 1640, 3, 1639,  -1, // 272
			1194, 4, 1642, 3, 60, 10, 1641,  -1, // 273
			60, 423, 10, 33,  -1, // 274
			423, 10, 33,  -1, // 275
			60, 424, 1195, 10, 1643,  -1, // 276
			424, 1196, 10, 1644,  -1, // 277
			5, 201,  -1, // 278
			1199, 425, 170, 1197, 1646, 1645, 3, 82,  -1, // 279
			170, 1198, 1647,  -1, // 280
			1201, 7, 426, 1200, 1649, 4, 1648, 3, 84,  -1, // 281
			426, 1511, 286,  -1, // 282
			1202, 9, 1651, 1650,  -1, // 283
			1203, 9, 1652,  -1, // 284
			1205, 170, 1204, 1654, 1653, 3, 87,  -1, // 285
			1208, 5, 4, 1656, 3, 87, 1207, 170, 1206, 1655,  -1, // 286
			1215, 170, 1214, 4, 429, 5, 428, 427, 1209, 3, 1657,  -1, // 287
			5, 203,  -1, // 288
			1211, 1658, 1210,  -1, // 289
			1213, 206, 1212,  -1, // 290
			1216, 5, 1659,  -1, // 291
			1217, 5, 1660,  -1, // 292
			1218, 5, 430, 92,  -1, // 293
			1220, 1664, 1219, 1663, 431,  -1, // 294
			1222, 7, 432, 1221, 6, 1665,  -1, // 295
			432, 169,  -1, // 296
			1223, 9, 34,  -1, // 297
			1224, 9, 35,  -1, // 298
			433, 288,  -1, // 299
			1225, 7, 433, 288, 6,  -1, // 300
			1228, 170, 1227, 4, 206, 3, 94,  -1, // 301
			1229, 5, 1666,  -1, // 302
			436, 435,  -1, // 303
			7, 436, 6,  -1, // 304
			5, 303,  -1, // 305
			437, 304,  -1, // 306
			437, 304,  -1, // 307
			438, 304,  -1, // 308
			12, 438, 304, 11,  -1, // 309
			439, 304,  -1, // 310
			4, 439, 3,  -1, // 311
			440, 291, 4, 303, 3, 82,  -1, // 312
			291, 83,  -1, // 313
			292, 4, 303, 3, 84,  -1, // 314
			291, 4, 303, 3, 87,  -1, // 315
			5, 4, 303, 3, 87, 291, 88,  -1, // 316
			291, 4, 443, 5, 442, 5, 441, 3, 89,  -1, // 317
			5, 444, 90,  -1, // 318
			5, 445, 91,  -1, // 319
			5, 446, 92,  -1, // 320
			291, 93,  -1, // 321
			447, 306,  -1, // 322
			7, 447, 306, 6,  -1, // 323
			291, 1231, 4, 248, 3, 94,  -1, // 324
			1232, 5, 192,  -1, // 325
			449, 1512, 309,  -1, // 326
			449, 1512, 309,  -1, // 327
			1233, 1667,  -1, // 328
			1234, 1668,  -1, // 329
			1235, 1669,  -1, // 330
			1236, 1670,  -1, // 331
			1513, 312,  -1, // 332
			450, 1514, 238,  -1, // 333
			450, 1514, 238,  -1, // 334
			1237, 1671,  -1, // 335
			1238, 1672,  -1, // 336
			1239, 95,  -1, // 337
			1240, 96,  -1, // 338
			1241, 97,  -1, // 339
			1242, 98,  -1, // 340
			1243, 99,  -1, // 341
			1244, 100,  -1, // 342
			1245, 101,  -1, // 343
			1246, 102,  -1, // 344
			1247, 103,  -1, // 345
			1248, 104,  -1, // 346
			1249, 105,  -1, // 347
			1250, 106,  -1, // 348
			1251, 107,  -1, // 349
			1252, 108,  -1, // 350
			1253, 109,  -1, // 351
			1254, 110,  -1, // 352
			1255, 111,  -1, // 353
			1256, 19, 1673, 18, 111,  -1, // 354
			1257, 112,  -1, // 355
			1258, 113,  -1, // 356
			1259, 114,  -1, // 357
			1260, 115,  -1, // 358
			1261, 116,  -1, // 359
			1262, 117,  -1, // 360
			1263, 118,  -1, // 361
			1264, 119,  -1, // 362
			1265, 120,  -1, // 363
			1266, 121,  -1, // 364
			1267, 122,  -1, // 365
			6, 123,  -1, // 366
			1268, 1674,  -1, // 367
			1269, 1675,  -1, // 368
			453, 1271, 1676, 1270,  -1, // 369
			1272, 3,  -1, // 370
			1273, 1677,  -1, // 371
			1275, 1515, 315, 1274, 123,  -1, // 372
			1276, 124,  -1, // 373
			1277, 125,  -1, // 374
			1278, 126,  -1, // 375
			1279, 127,  -1, // 376
			1280, 128,  -1, // 377
			1281, 61,  -1, // 378
			1282, 129,  -1, // 379
			1283, 130,  -1, // 380
			1284, 131,  -1, // 381
			1285, 132,  -1, // 382
			1286, 133,  -1, // 383
			1287, 134,  -1, // 384
			1288, 123,  -1, // 385
			1289, 135,  -1, // 386
			1290, 136,  -1, // 387
			1291, 137,  -1, // 388
			1292, 138,  -1, // 389
			1293, 139,  -1, // 390
			1294, 140,  -1, // 391
			1295, 141,  -1, // 392
			1296, 4, 1678, 3, 130,  -1, // 393
			454, 1516, 316,  -1, // 394
			454, 1517, 316, 8,  -1, // 395
			1299, 456, 455, 1518, 1679,  -1, // 396
			1297, 1680, 60,  -1, // 397
			1298, 1681, 13,  -1, // 398
			457, 1519, 179,  -1, // 399
			459, 458, 1520, 180, 457,  -1, // 400
			458, 1521, 182,  -1, // 401
			459, 1522, 183,  -1, // 402
			460, 1300, 1523, 1682,  -1, // 403
			460, 1524, 318, 10,  -1, // 404
			1301, 1683,  -1, // 405
			1302, 63,  -1, // 406
			1303, 64,  -1, // 407
			1304, 142,  -1, // 408
			1305, 60,  -1, // 409
			1306, 143,  -1, // 410
			1307, 60, 97,  -1, // 411
			1308, 143, 97,  -1, // 412
			1309, 4, 3, 144,  -1, // 413
			1310, 1684, 144,  -1, // 414
			1311, 1685, 144,  -1, // 415
			1312, 26,  -1, // 416
			1313, 27,  -1, // 417
			1314, 55,  -1, // 418
			1315, 56,  -1, // 419
			1316, 55, 145,  -1, // 420
			1317, 56, 145,  -1, // 421
			1318, 62, 461,  -1, // 422
			1319, 20,  -1, // 423
			1320, 21,  -1, // 424
			1321, 22,  -1, // 425
			1322, 23,  -1, // 426
			1323, 24,  -1, // 427
			1324, 42,  -1, // 428
			1325, 43,  -1, // 429
			1326, 17,  -1, // 430
			1327, 16,  -1, // 431
			1328, 15,  -1, // 432
			1329, 38,  -1, // 433
			1330, 39,  -1, // 434
			1331, 18,  -1, // 435
			1332, 40,  -1, // 436
			1333, 19,  -1, // 437
			1334, 41,  -1, // 438
			1335, 13,  -1, // 439
			1336, 44,  -1, // 440
			1337, 45,  -1, // 441
			1338, 46,  -1, // 442
			1339, 47,  -1, // 443
			1340, 48,  -1, // 444
			1341, 49,  -1, // 445
			1342, 50,  -1, // 446
			1343, 51,  -1, // 447
			1344, 52,  -1, // 448
			1345, 53,  -1, // 449
			1346, 54,  -1, // 450
			1347, 146,  -1, // 451
			1348, 37,  -1, // 452
			1349, 36,  -1, // 453
			1350, 12, 11,  -1, // 454
			1351, 25,  -1, // 455
			462, 1352, 22,  -1, // 456
			462, 1353, 1686,  -1, // 457
			1525, 320,  -1, // 458
			1526, 181,  -1, // 459
			1527, 321,  -1, // 460
			1355, 12, 1687, 11,  -1, // 461
			1356, 12, 1688, 11,  -1, // 462
			1357, 12, 11,  -1, // 463
			4, 1528, 322, 3,  -1, // 464
			4, 3,  -1, // 465
			464, 1529, 323,  -1, // 466
			464, 1530, 323, 8,  -1, // 467
			1531, 324,  -1, // 468
			1532, 325,  -1, // 469
			1358, 147,  -1, // 470
			1361, 468, 467, 466, 465, 1359, 1689,  -1, // 471
			465, 1533, 179,  -1, // 472
			1534, 180,  -1, // 473
			467, 1535, 182,  -1, // 474
			1360, 1690, 13,  -1, // 475
			1363, 472, 471, 470, 469, 1536, 179,  -1, // 476
			469, 1536, 179,  -1, // 477
			1537, 180,  -1, // 478
			471, 1538, 182,  -1, // 479
			1362, 1691, 13,  -1, // 480
			1364, 1692, 9,  -1, // 481
			1365, 126,  -1, // 482
			1366, 1693,  -1, // 483
			1367, 1694,  -1, // 484
			1368, 1695,  -1, // 485
			1369, 1696,  -1, // 486
			1539, 331, 1370, 474, 473, 1697,  -1, // 487
			1699, 9,  -1, // 488
			1371, 148,  -1, // 489
			1372, 149,  -1, // 490
			1373, 150,  -1, // 491
			1375, 7, 1540, 332, 1374, 6,  -1, // 492
			475, 1541, 333,  -1, // 493
			475, 1542, 333, 8,  -1, // 494
			1701, 13,  -1, // 495
			1376, 476, 1700,  -1, // 496
			1543, 315, 1377, 479, 478, 477, 151,  -1, // 497
			1704, 9,  -1, // 498
			1378, 4, 1705, 3, 28,  -1, // 499
			1379, 1706,  -1, // 500
			1380, 86,  -1, // 501
			1544, 315, 1381, 480, 152,  -1, // 502
			1545, 315, 1382, 483, 1710, 482, 154, 481,  -1, // 503
			1711, 9,  -1, // 504
			484, 168,  -1, // 505
			7, 484, 6,  -1, // 506
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 10, 18, 21, 29, 32, 37, 41, 46, 50, 54, 57, 61, 66, 69, 74, 78, 89, 93, 97, 101, 104, 107, 110, 116, 120, 123, 126, 129, 132, 138, 142, 146, 149, 153, 158, 161, 164, 169, 173, 176, 179, 183, 187, 191, 197, 201, 207, 211, 217, 221, 224, 228, 232, 236, 240, 243, 246, 249, 252, 255, 259, 264, 268, 273, 278, 284, 288, 291, 295, 305, 309, 315, 319, 325, 329, 335, 339, 345, 349, 355, 359, 365, 368, 371, 375, 381, 384, 387, 390, 393, 397, 403, 406, 409, 413, 419, 422, 425, 429, 435, 438, 441, 444, 448, 454, 458, 461, 466, 470, 473, 476, 479, 482, 485, 488, 491, 494, 497, 500, 503, 506, 509, 512, 517, 522, 527, 532, 537, 542, 547, 552, 558, 566, 572, 575, 580, 583, 590, 594, 599, 603, 607, 612, 618, 623, 626, 629, 632, 637, 642, 645, 648, 651, 656, 659, 662, 665, 668, 671, 674, 677, 680, 683, 689, 695, 701, 709, 716, 719, 722, 727, 731, 734, 738, 745, 750, 753, 758, 761, 765, 768, 771, 774, 777, 780, 784, 789, 793, 798, 803, 809, 813, 821, 825, 831, 835, 841, 845, 851, 855, 861, 865, 871, 875, 881, 885, 891, 895, 901, 905, 911, 915, 921, 925, 931, 935, 941, 944, 947, 952, 957, 962, 967, 972, 977, 982, 987, 992, 998, 1004, 1010, 1018, 1025, 1033, 1038, 1043, 1046, 1053, 1057, 1061, 1066, 1072, 1077, 1080, 1083, 1086, 1091, 1096, 1099, 1102, 1105, 1108, 1111, 1114, 1117, 1120, 1123, 1126, 1129, 1132, 1137, 1140, 1143, 1148, 1152, 1155, 1161, 1164, 1172, 1175, 1178, 1184, 1192, 1197, 1201, 1207, 1212, 1215, 1224, 1228, 1238, 1242, 1247, 1251, 1259, 1270, 1282, 1285, 1289, 1293, 1297, 1301, 1306, 1312, 1319, 1322, 1326, 1330, 1333, 1339, 1347, 1351, 1354, 1358, 1361, 1364, 1367, 1370, 1375, 1378, 1382, 1389, 1392, 1398, 1404, 1412, 1422, 1426, 1430, 1434, 1437, 1440, 1445, 1452, 1456, 1460, 1464, 1467, 1470, 1473, 1476, 1479, 1483, 1487, 1490, 1493, 1496, 1499, 1502, 1505, 1508, 1511, 1514, 1517, 1520, 1523, 1526, 1529, 1532, 1535, 1538, 1541, 1544, 1550, 1553, 1556, 1559, 1562, 1565, 1568, 1571, 1574, 1577, 1580, 1583, 1586, 1589, 1592, 1597, 1600, 1603, 1609, 1612, 1615, 1618, 1621, 1624, 1627, 1630, 1633, 1636, 1639, 1642, 1645, 1648, 1651, 1654, 1657, 1660, 1663, 1666, 1669, 1675, 1679, 1684, 1690, 1694, 1698, 1702, 1708, 1712, 1716, 1721, 1726, 1729, 1732, 1735, 1738, 1741, 1744, 1748, 1752, 1757, 1761, 1765, 1768, 1771, 1774, 1777, 1781, 1785, 1789, 1792, 1795, 1798, 1801, 1804, 1807, 1810, 1813, 1816, 1819, 1822, 1825, 1828, 1831, 1834, 1837, 1840, 1843, 1846, 1849, 1852, 1855, 1858, 1861, 1864, 1867, 1870, 1873, 1876, 1879, 1882, 1886, 1889, 1893, 1897, 1900, 1903, 1906, 1911, 1916, 1920, 1925, 1928, 1932, 1937, 1940, 1943, 1946, 1954, 1958, 1961, 1965, 1969, 1977, 1981, 1984, 1988, 1992, 1996, 1999, 2002, 2005, 2008, 2011, 2018, 2021, 2024, 2027, 2030, 2037, 2041, 2046, 2049, 2053, 2061, 2064, 2070, 2073, 2076, 2082, 2091, 2094, 2097, 
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

		
		case '#':
			return 2;
		
		case '(':
			return 3;
		
		case ')':
			return 4;
		
		case ';':
			return 5;
		
		case '{':
			return 6;
		
		case '}':
			return 7;
		
		case ',':
			return 8;
		
		case ':':
			return 9;
		
		case '.':
			return 10;
		
		case '[':
			return 11;
		
		case ']':
			return 12;
		
		case '=':
			return 13;
		
		case '?':
			return 14;
		
		case '|':
			return 15;
		
		case '^':
			return 16;
		
		case '&':
			return 17;
		
		case '<':
			return 18;
		
		case '>':
			return 19;
		
		case '+':
			return 20;
		
		case '-':
			return 21;
		
		case '*':
			return 22;
		
		case '/':
			return 23;
		
		case '%':
			return 24;
		
		case '@':
			return 25;
		
		case '~':
			return 26;
		
		case '!':
			return 27;
		
		case EToken_Pack:
			return 28;
		
		case EToken_Namespace:
			return 29;
		
		case EToken_Extend:
			return 30;
		
		case EToken_Using:
			return 31;
		
		case EToken_Friend:
			return 32;
		
		case EToken_Identifier:
			return 33;
		
		case EToken_Catch:
			return 34;
		
		case EToken_Finally:
			return 35;
		
		case EToken_LogOr:
			return 36;
		
		case EToken_LogAnd:
			return 37;
		
		case EToken_Eq:
			return 38;
		
		case EToken_Ne:
			return 39;
		
		case EToken_Le:
			return 40;
		
		case EToken_Ge:
			return 41;
		
		case EToken_Shl:
			return 42;
		
		case EToken_Shr:
			return 43;
		
		case EToken_RefAssign:
			return 44;
		
		case EToken_AddAssign:
			return 45;
		
		case EToken_SubAssign:
			return 46;
		
		case EToken_MulAssign:
			return 47;
		
		case EToken_DivAssign:
			return 48;
		
		case EToken_ModAssign:
			return 49;
		
		case EToken_ShlAssign:
			return 50;
		
		case EToken_ShrAssign:
			return 51;
		
		case EToken_AndAssign:
			return 52;
		
		case EToken_XorAssign:
			return 53;
		
		case EToken_OrAssign:
			return 54;
		
		case EToken_Inc:
			return 55;
		
		case EToken_Dec:
			return 56;
		
		case EToken_Try:
			return 57;
		
		case EToken_New:
			return 58;
		
		case EToken_Delete:
			return 59;
		
		case EToken_Construct:
			return 60;
		
		case EToken_Weak:
			return 61;
		
		case EToken_Ptr:
			return 62;
		
		case EToken_Get:
			return 63;
		
		case EToken_Set:
			return 64;
		
		case EToken_This:
			return 65;
		
		case EToken_RetVal:
			return 66;
		
		case EToken_Integer:
			return 67;
		
		case EToken_Fp:
			return 68;
		
		case EToken_True:
			return 69;
		
		case EToken_False:
			return 70;
		
		case EToken_Null:
			return 71;
		
		case EToken_SizeOf:
			return 72;
		
		case EToken_CountOf:
			return 73;
		
		case EToken_TypeOf:
			return 74;
		
		case EToken_OffsetOf:
			return 75;
		
		case EToken_BindingOf:
			return 76;
		
		case EToken_Literal:
			return 77;
		
		case EToken_HexLiteral:
			return 78;
		
		case EToken_FmtLiteral:
			return 79;
		
		case EToken_FmtSpecifier:
			return 80;
		
		case EToken_BaseType:
			return 81;
		
		case EToken_If:
			return 82;
		
		case EToken_Else:
			return 83;
		
		case EToken_Switch:
			return 84;
		
		case EToken_Case:
			return 85;
		
		case EToken_Default:
			return 86;
		
		case EToken_While:
			return 87;
		
		case EToken_Do:
			return 88;
		
		case EToken_For:
			return 89;
		
		case EToken_Break:
			return 90;
		
		case EToken_Continue:
			return 91;
		
		case EToken_Return:
			return 92;
		
		case EToken_Once:
			return 93;
		
		case EToken_OnChanged:
			return 94;
		
		case EToken_Typedef:
			return 95;
		
		case EToken_Alias:
			return 96;
		
		case EToken_Static:
			return 97;
		
		case EToken_Thread:
			return 98;
		
		case EToken_Stack:
			return 99;
		
		case EToken_Heap:
			return 100;
		
		case EToken_UHeap:
			return 101;
		
		case EToken_Abstract:
			return 102;
		
		case EToken_Virtual:
			return 103;
		
		case EToken_Override:
			return 104;
		
		case EToken_Mutable:
			return 105;
		
		case EToken_Nullable:
			return 106;
		
		case EToken_Public:
			return 107;
		
		case EToken_Protected:
			return 108;
		
		case EToken_Void:
			return 109;
		
		case EToken_Variant:
			return 110;
		
		case EToken_Object:
			return 111;
		
		case EToken_Bool:
			return 112;
		
		case EToken_Int8:
			return 113;
		
		case EToken_Int16:
			return 114;
		
		case EToken_Int32:
			return 115;
		
		case EToken_Int64:
			return 116;
		
		case EToken_Int:
			return 117;
		
		case EToken_IntPtr:
			return 118;
		
		case EToken_Char:
			return 119;
		
		case EToken_Float:
			return 120;
		
		case EToken_Double:
			return 121;
		
		case EToken_Auto:
			return 122;
		
		case EToken_Property:
			return 123;
		
		case EToken_Unsigned:
			return 124;
		
		case EToken_BigEndian:
			return 125;
		
		case EToken_Const:
			return 126;
		
		case EToken_DConst:
			return 127;
		
		case EToken_Volatile:
			return 128;
		
		case EToken_Thin:
			return 129;
		
		case EToken_Pitcher:
			return 130;
		
		case EToken_Cdecl:
			return 131;
		
		case EToken_Stdcall:
			return 132;
		
		case EToken_Array:
			return 133;
		
		case EToken_Function:
			return 134;
		
		case EToken_Bindable:
			return 135;
		
		case EToken_AutoGet:
			return 136;
		
		case EToken_Indexed:
			return 137;
		
		case EToken_Multicast:
			return 138;
		
		case EToken_Event:
			return 139;
		
		case EToken_DEvent:
			return 140;
		
		case EToken_Reactor:
			return 141;
		
		case EToken_PreConstruct:
			return 142;
		
		case EToken_Destruct:
			return 143;
		
		case EToken_Operator:
			return 144;
		
		case EToken_Postfix:
			return 145;
		
		case EToken_AtAssign:
			return 146;
		
		case EToken_Ellipsis:
			return 147;
		
		case EToken_Enum:
			return 148;
		
		case EToken_FEnum:
			return 149;
		
		case EToken_CEnum:
			return 150;
		
		case EToken_Struct:
			return 151;
		
		case EToken_Union:
			return 152;
		
		case EToken_Opaque:
			return 153;
		
		case EToken_Class:
			return 154;
		
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
			'#', 
			'(', 
			')', 
			';', 
			'{', 
			'}', 
			',', 
			':', 
			'.', 
			'[', 
			']', 
			'=', 
			'?', 
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
			'@', 
			'~', 
			'!', 
			EToken_Pack, 
			EToken_Namespace, 
			EToken_Extend, 
			EToken_Using, 
			EToken_Friend, 
			EToken_Identifier, 
			EToken_Catch, 
			EToken_Finally, 
			EToken_LogOr, 
			EToken_LogAnd, 
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
			EToken_Try, 
			EToken_New, 
			EToken_Delete, 
			EToken_Construct, 
			EToken_Weak, 
			EToken_Ptr, 
			EToken_Get, 
			EToken_Set, 
			EToken_This, 
			EToken_RetVal, 
			EToken_Integer, 
			EToken_Fp, 
			EToken_True, 
			EToken_False, 
			EToken_Null, 
			EToken_SizeOf, 
			EToken_CountOf, 
			EToken_TypeOf, 
			EToken_OffsetOf, 
			EToken_BindingOf, 
			EToken_Literal, 
			EToken_HexLiteral, 
			EToken_FmtLiteral, 
			EToken_FmtSpecifier, 
			EToken_BaseType, 
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
			EToken_Once, 
			EToken_OnChanged, 
			EToken_Typedef, 
			EToken_Alias, 
			EToken_Static, 
			EToken_Thread, 
			EToken_Stack, 
			EToken_Heap, 
			EToken_UHeap, 
			EToken_Abstract, 
			EToken_Virtual, 
			EToken_Override, 
			EToken_Mutable, 
			EToken_Nullable, 
			EToken_Public, 
			EToken_Protected, 
			EToken_Void, 
			EToken_Variant, 
			EToken_Object, 
			EToken_Bool, 
			EToken_Int8, 
			EToken_Int16, 
			EToken_Int32, 
			EToken_Int64, 
			EToken_Int, 
			EToken_IntPtr, 
			EToken_Char, 
			EToken_Float, 
			EToken_Double, 
			EToken_Auto, 
			EToken_Property, 
			EToken_Unsigned, 
			EToken_BigEndian, 
			EToken_Const, 
			EToken_DConst, 
			EToken_Volatile, 
			EToken_Thin, 
			EToken_Pitcher, 
			EToken_Cdecl, 
			EToken_Stdcall, 
			EToken_Array, 
			EToken_Function, 
			EToken_Bindable, 
			EToken_AutoGet, 
			EToken_Indexed, 
			EToken_Multicast, 
			EToken_Event, 
			EToken_DEvent, 
			EToken_Reactor, 
			EToken_PreConstruct, 
			EToken_Destruct, 
			EToken_Operator, 
			EToken_Postfix, 
			EToken_AtAssign, 
			EToken_Ellipsis, 
			EToken_Enum, 
			EToken_FEnum, 
			EToken_CEnum, 
			EToken_Struct, 
			EToken_Union, 
			EToken_Opaque, 
			EToken_Class, 
			
			0
		};

		ASSERT (Index < TokenCount);
		return _TokenTable [Index];
	}

	static
	const char*
	GetSymbolName (size_t Index)
	{
		static
		const char* _SymbolNameTable [NamedSymbolCount] = 
		{
			"compilation_unit",
			"global_declaration",
			"pragma_pack",
			"struct_pack_factor_value",
			"namespace_declaration",
			"extend_declaration",
			"using_declaration",
			"friend_declaration",
			"access_declaration",
			"named_type_specifier",
			"item_declaration_w_specifier",
			"item_declaration_no_specifier_rslv",
			"item_declaration_no_specifier",
			"named_type_block_declaration",
			"local_declaration",
			"statement",
			"catch_label",
			"finally_label",
			"qualified_name",
			"access_specifier",
			"qualified_name_impl",
			"declaration_specifier_list",
			"declarator_list",
			"declaration_terminator",
			"declarator_prefix",
			"declarator_name",
			"function_suffix",
			"declarator_suffix",
			"post_declarator_modifier",
			"declaration_body_pass1",
			"compound_stmt_pass1",
			"statement_pass1",
			"type_name_impl",
			"type_specifier_modifier_list",
			"type_name",
			"type_name_list",
			"type_name_or_expr",
			"expression_s",
			"attribute_block",
			"attribute_declarator_list",
			"attribute_declarator",
			"expression_pass1",
			"primary_expr_pass1",
			"arg_list_pass1",
			"arg_pass1",
			"expression_save_value",
			"expression",
			"expression_or_empty_list_save_list",
			"expression_or_empty_list",
			"conditional_expr",
			"expression_or_empty",
			"expression_list",
			"constant_expr",
			"constant_integer_expr",
			"logical_or_expr",
			"logical_and_expr",
			"inclusive_or_expr",
			"exclusive_or_expr",
			"and_expr",
			"equality_expr",
			"relational_expr",
			"equality_operator",
			"shift_expr",
			"relational_operator",
			"additive_expr",
			"shift_operator",
			"multiplicative_expr",
			"additive_operator",
			"at_expr",
			"multiplicative_operator",
			"assignment_expr",
			"unary_expr",
			"assignment_operator_expr",
			"assignment_operator",
			"curly_initializer",
			"postfix_expr",
			"unary_operator_expr",
			"cast_operator_rslv",
			"storage_specifier",
			"new_operator_type",
			"new_operator_curly_initializer",
			"type_name_w_constructor_rslv",
			"qualified_type_name",
			"type_specifier_modifier",
			"primary_expr",
			"postfix_operator",
			"member_operator",
			"literal",
			"fmt_literal_value",
			"curly_initializer_item",
			"expression_save_value_s",
			"conditional_expr_s",
			"expression_or_empty_s",
			"expression_list_s",
			"expression_or_empty_list_s",
			"logical_or_expr_s",
			"logical_and_expr_s",
			"inclusive_or_expr_s",
			"exclusive_or_expr_s",
			"and_expr_s",
			"equality_expr_s",
			"relational_expr_s",
			"shift_expr_s",
			"additive_expr_s",
			"multiplicative_expr_s",
			"at_expr_s",
			"assignment_expr_s",
			"unary_expr_s",
			"postfix_expr_s",
			"unary_operator_expr_s",
			"new_operator_sype_s",
			"primary_expr_s",
			"postfix_operator_s",
			"member_operator_s",
			"literal_s",
			"compound_stmt",
			"constructor_compound_stmt",
			"btm_construct_stmt",
			"btm_construct_stmt_rslv",
			"btm_construct_name",
			"expression_stmt",
			"if_stmt",
			"switch_stmt",
			"while_stmt",
			"do_stmt",
			"for_stmt",
			"break_stmt",
			"continue_stmt",
			"return_stmt",
			"once_stmt",
			"try_stmt",
			"switch_block_stmt",
			"reactor_body",
			"reactor_stmt",
			"reactor_onchange_stmt",
			"reactor_expression_stmt",
			"statement_0",
			"compound_stmt_0",
			"expression_stmt_0",
			"if_stmt_0",
			"switch_stmt_0",
			"while_stmt_0",
			"do_stmt_0",
			"for_stmt_0",
			"break_stmt_0",
			"continue_stmt_0",
			"return_stmt_0",
			"once_stmt_0",
			"expression_0",
			"primary_expr_0",
			"reactor_body_0",
			"reactor_stmt_0",
			"reactor_onchange_stmt_0",
			"reactor_expression_stmt_0",
			"declaration_specifier",
			"type_specifier",
			"type_modifier",
			"pitcher_modifier",
			"property_template_specifier",
			"qualified_type_name_rslv",
			"named_type_block",
			"full_declarator",
			"declarator",
			"declarator_qualifier",
			"operator_name",
			"array_suffix",
			"bitfield_suffix",
			"function_formal_argument_list",
			"function_formal_argument",
			"function_formal_argument_w_specifier",
			"function_formal_argument_no_specifier",
			"enum_specifier",
			"struct_specifier",
			"union_specifier",
			"class_specifier",
			"enum_type_kind",
			"enum_block",
			"enum_member_list",
			"enum_member",
			"struct_pack_factor",
			"named_type_block_impl",
			
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
		
		
		
		
		case ESymbol_struct_pack_factor_value:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls44);
				
			break;
			
		
		
		
		
		
		
		case ESymbol_named_type_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls39);
				
			break;
			
		
		case ESymbol_item_declaration_w_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
				
			break;
			
		
		case ESymbol_item_declaration_no_specifier_rslv:
			pNode = AXL_MEM_NEW (CSymbolNode_item_declaration_no_specifier_rslv);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_item_declaration_no_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_item_declaration_no_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		case ESymbol_local_declaration:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
				
			break;
			
		
		case ESymbol_statement:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
				
			break;
			
		
		
		
		case ESymbol_qualified_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls1);
				
			break;
			
		
		case ESymbol_access_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls32);
				
			break;
			
		
		case ESymbol_qualified_name_impl:
			pNode = AXL_MEM_NEW (CSymbolNode_qualified_name_impl);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declaration_specifier_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls29);
				
			break;
			
		
		case ESymbol_declarator_list:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declaration_terminator:
			pNode = AXL_MEM_NEW (CSymbolNode_declaration_terminator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declarator_prefix:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_prefix);
			
			break;
			
		
		case ESymbol_declarator_name:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_name);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_function_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_function_suffix);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declarator_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_suffix);
			
			break;
			
		
		case ESymbol_post_declarator_modifier:
			pNode = AXL_MEM_NEW (CSymbolNode_post_declarator_modifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declaration_body_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls2);
				
			break;
			
		
		case ESymbol_compound_stmt_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_compound_stmt_pass1);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls3);
			
			break;
			
		
		case ESymbol_statement_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_statement_pass1);
			
			break;
			
		
		case ESymbol_type_name_impl:
			pNode = AXL_MEM_NEW (CSymbolNode_type_name_impl);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_type_specifier_modifier_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls30);
				
			break;
			
		
		case ESymbol_type_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls4);
				
			break;
			
		
		case ESymbol_type_name_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls5);
				
			break;
			
		
		case ESymbol_type_name_or_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls6);
				
			break;
			
		
		case ESymbol_expression_s:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls23);
				
			break;
			
		
		
		
		
		case ESymbol_expression_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls7);
				
			break;
			
		
		case ESymbol_primary_expr_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_primary_expr_pass1);
			
			break;
			
		
		case ESymbol_arg_list_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls8);
				
			break;
			
		
		case ESymbol_arg_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_arg_pass1);
			
			break;
			
		
		
		case ESymbol_expression:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls9);
				
			break;
			
		
		
		case ESymbol_expression_or_empty_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls12);
				
			break;
			
		
		case ESymbol_conditional_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_conditional_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls15);
			
			break;
			
		
		case ESymbol_expression_or_empty:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls10);
				
			break;
			
		
		case ESymbol_expression_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls11);
				
			break;
			
		
		case ESymbol_constant_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls13);
				
			break;
			
		
		case ESymbol_constant_integer_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls14);
				
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
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls16);
				
			break;
			
		
		case ESymbol_shift_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_shift_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_relational_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls17);
				
			break;
			
		
		case ESymbol_additive_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_additive_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_shift_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls18);
				
			break;
			
		
		case ESymbol_multiplicative_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_multiplicative_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_additive_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls19);
				
			break;
			
		
		case ESymbol_at_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_at_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_multiplicative_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls20);
				
			break;
			
		
		case ESymbol_assignment_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_assignment_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_unary_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_unary_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_assignment_operator_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_assignment_operator_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_assignment_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls21);
				
			break;
			
		
		case ESymbol_curly_initializer:
			pNode = AXL_MEM_NEW (CSymbolNode_curly_initializer);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_postfix_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_postfix_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_unary_operator_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_unary_operator_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_cast_operator_rslv:
			pNode = AXL_MEM_NEW (CSymbolNode_cast_operator_rslv);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_storage_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls31);
				
			break;
			
		
		case ESymbol_new_operator_type:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls22);
				
			break;
			
		
		case ESymbol_new_operator_curly_initializer:
			pNode = AXL_MEM_NEW (CSymbolNode_new_operator_curly_initializer);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		case ESymbol_qualified_type_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls34);
				
			break;
			
		
		case ESymbol_type_specifier_modifier:
			pNode = AXL_MEM_NEW (CSymbolNode_type_specifier_modifier);
			
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
			
		
		case ESymbol_member_operator:
			pNode = AXL_MEM_NEW (CSymbolNode_member_operator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_literal:
			pNode = AXL_MEM_NEW (CSymbolNode_literal);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_fmt_literal_value:
			pNode = AXL_MEM_NEW (CSymbolNode_fmt_literal_value);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_curly_initializer_item:
			pNode = AXL_MEM_NEW (CSymbolNode_curly_initializer_item);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		case ESymbol_conditional_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_conditional_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_expression_or_empty_s:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls24);
				
			break;
			
		
		case ESymbol_expression_list_s:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls25);
				
			break;
			
		
		case ESymbol_expression_or_empty_list_s:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls26);
				
			break;
			
		
		case ESymbol_logical_or_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_logical_or_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_logical_and_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_logical_and_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_inclusive_or_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_inclusive_or_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_exclusive_or_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_exclusive_or_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_and_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_and_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_equality_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_equality_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_relational_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_relational_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_shift_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_shift_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_additive_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_additive_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_multiplicative_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_multiplicative_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_at_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_at_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_assignment_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_assignment_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_unary_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_unary_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_postfix_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_postfix_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_unary_operator_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_unary_operator_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_new_operator_sype_s:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls27);
				
			break;
			
		
		case ESymbol_primary_expr_s:
			pNode = AXL_MEM_NEW (CSymbolNode_primary_expr_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_postfix_operator_s:
			pNode = AXL_MEM_NEW (CSymbolNode_postfix_operator_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_member_operator_s:
			pNode = AXL_MEM_NEW (CSymbolNode_member_operator_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_literal_s:
			pNode = AXL_MEM_NEW (CSymbolNode_literal_s);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		
		
		
		case ESymbol_btm_construct_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls28);
				
			break;
			
		
		
		case ESymbol_if_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_if_stmt);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_switch_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_switch_stmt);
			
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
			
		
		
		
		
		case ESymbol_once_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_once_stmt);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		case ESymbol_switch_block_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_switch_block_stmt);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		
		case ESymbol_reactor_onchange_stmt:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
				
			break;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_reactor_stmt_0:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
				
			break;
			
		
		
		
		case ESymbol_declaration_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_declaration_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_type_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls33);
				
			break;
			
		
		case ESymbol_type_modifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls36);
				
			break;
			
		
		case ESymbol_pitcher_modifier:
			pNode = AXL_MEM_NEW (CSymbolNode_pitcher_modifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_property_template_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_property_template_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls35);
			
			break;
			
		
		
		case ESymbol_named_type_block:
			pNode = AXL_MEM_NEW (CSymbolNode_named_type_block);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_full_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_full_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls37);
			
			break;
			
		
		case ESymbol_declarator_qualifier:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_qualifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_operator_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls38);
				
			break;
			
		
		case ESymbol_array_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_array_suffix);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_bitfield_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_bitfield_suffix);
			
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
			
		
		case ESymbol_function_formal_argument_w_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_function_formal_argument_w_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_function_formal_argument_no_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_function_formal_argument_no_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_enum_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls40);
				
			break;
			
		
		case ESymbol_struct_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls42);
				
			break;
			
		
		case ESymbol_union_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls45);
				
			break;
			
		
		case ESymbol_class_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls46);
				
			break;
			
		
		case ESymbol_enum_type_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls41);
				
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
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls43);
				
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
			
			{ 0, 158 },
			
			{ 0, 173 },
			
			{ 0, 173 },
			
			{ 0, 174 },
			
			{ 0, 33 },
			
			{ 1, 33 },
			
			{ 0, 176 },
			
			{ 0, 184 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 34 },
			
			{ 0, 35 },
			
			{ 0, 1 },
			
			{ 0, 188 },
			
			{ 0, 189 },
			
			{ 1, 189 },
			
			{ 0, 189 },
			
			{ 0, 192 },
			
			{ 0, 33 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 11 },
			
			{ 1, 12 },
			
			{ 0, 3 },
			
			{ 1, 4 },
			
			{ 0, 1 },
			
			{ 0, 1 },
			
			{ 0, 201 },
			
			{ 0, 203 },
			
			{ 0, 201 },
			
			{ 1, 201 },
			
			{ 0, 201 },
			
			{ 1, 205 },
			
			{ 0, 205 },
			
			{ 0, 201 },
			
			{ 0, 216 },
			
			{ 0, 218 },
			
			{ 0, 220 },
			
			{ 0, 222 },
			
			{ 0, 224 },
			
			{ 0, 228 },
			
			{ 0, 233 },
			
			{ 1, 189 },
			
			{ 0, 233 },
			
			{ 1, 234 },
			
			{ 0, 237 },
			
			{ 1, 203 },
			
			{ 0, 203 },
			
			{ 0, 203 },
			
			{ 0, 205 },
			
			{ 0, 33 },
			
			{ 0, 33 },
			
			{ 0, 67 },
			
			{ 0, 68 },
			
			{ 0, 191 },
			
			{ 0, 191 },
			
			{ 0, 191 },
			
			{ 0, 77 },
			
			{ 1, 78 },
			
			{ 2, 79 },
			
			{ 1, 201 },
			
			{ 0, 80 },
			
			{ 0, 33 },
			
			{ 1, 201 },
			
			{ 0, 201 },
			
			{ 0, 192 },
			
			{ 0, 192 },
			
			{ 1, 192 },
			
			{ 0, 192 },
			
			{ 1, 247 },
			
			{ 0, 247 },
			
			{ 0, 216 },
			
			{ 0, 218 },
			
			{ 0, 220 },
			
			{ 0, 222 },
			
			{ 0, 224 },
			
			{ 0, 228 },
			
			{ 0, 191 },
			
			{ 0, 189 },
			
			{ 0, 233 },
			
			{ 1, 265 },
			
			{ 0, 189 },
			
			{ 0, 237 },
			
			{ 0, 249 },
			
			{ 0, 249 },
			
			{ 0, 247 },
			
			{ 0, 33 },
			
			{ 0, 33 },
			
			{ 0, 67 },
			
			{ 0, 77 },
			
			{ 1, 78 },
			
			{ 0, 6 },
			
			{ 0, 6 },
			
			{ 0, 274 },
			
			{ 1, 203 },
			
			{ 0, 81 },
			
			{ 1, 203 },
			
			{ 0, 33 },
			
			{ 1, 33 },
			
			{ 0, 201 },
			
			{ 1, 4 },
			
			{ 2, 83 },
			
			{ 0, 201 },
			
			{ 1, 6 },
			
			{ 1, 85 },
			
			{ 0, 208 },
			
			{ 0, 86 },
			
			{ 0, 201 },
			
			{ 1, 4 },
			
			{ 0, 88 },
			
			{ 1, 201 },
			
			{ 0, 89 },
			
			{ 1, 201 },
			
			{ 0, 90 },
			
			{ 0, 91 },
			
			{ 0, 201 },
			
			{ 0, 233 },
			
			{ 1, 93 },
			
			{ 2, 170 },
			
			{ 0, 57 },
			
			{ 0, 196 },
			
			{ 0, 233 },
			
			{ 0, 174 },
			
			{ 0, 310 },
			
			{ 0, 311 },
			
			{ 0, 310 },
			
			{ 0, 311 },
			
			{ 0, 189 },
			
			{ 0, 313 },
			
			{ 0, 237 },
			
			{ 0, 173 },
			
			{ 0, 173 },
			
			{ 0, 196 },
			
			{ 0, 317 },
			
			{ 1, 198 },
			
			{ 2, 196 },
			
			{ 0, 318 },
			
			{ 0, 33 },
			
			{ 0, 189 },
			
			{ 0, 319 },
			
			{ 0, 311 },
			
			{ 0, 196 },
			
			{ 0, 67 },
			
			{ 0, 176 },
			
			{ 1, 196 },
			
			{ 0, 196 },
			
			{ 0, 67 },
			
			{ 0, 326 },
			
			{ 0, 327 },
			
			{ 0, 328 },
			
			{ 0, 329 },
			
			{ 0, 330 },
			
			{ 1, 33 },
			
			{ 2, 189 },
			
			{ 0, 33 },
			
			{ 1, 196 },
			
			{ 2, 334 },
			
			{ 0, 33 },
			
			{ 1, 190 },
			
			{ 0, 158 },
			
			{ 0, 67 },
			
			{ 0, 33 },
			
			{ 3, 153 },
			
			{ 2, 334 },
			
			{ 0, 33 },
			
			{ 1, 190 },
			
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
// #line 6 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
			m_StructPackFactor = (*(_cls44*) GetAstLocator (0)).m_Value;
		;
// #line 6553 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 48 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 return false; ;
// #line 6564 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 60 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return OpenGlobalNamespace ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos) != NULL;
			;
// #line 6577 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pModule->m_NamespaceMgr.CloseNamespace ();
			;
// #line 6590 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 71 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return OpenTypeExtension ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos);
			;
// #line 6603 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 5:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 75 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pModule->m_NamespaceMgr.CloseNamespace ();
			;
// #line 6616 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 6:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 90 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pModule->m_NamespaceMgr.m_CurrentAccessKind = (*(_cls32*) GetAstLocator (0)).m_AccessKind;
			;
// #line 6629 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 7:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 107 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pName->AddName ((*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 6642 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 8:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 111 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pName->AddName ((*GetTokenLocator (1)).m_Data.m_String);
			;
// #line 6655 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 9:
			{
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 149 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return Declare (&__pSymbol->m_Local.Declarator);
			;
// #line 6668 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 10:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 183 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return IsEmptyDeclarationTerminatorAllowed (__pSymbol->m_Arg.pTypeSpecifier);
			;
// #line 6681 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 11:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 187 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return SetDeclarationBody (&(*(_cls2*) GetAstLocator (0)).m_TokenList);
			;
// #line 6694 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 12:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
// #line 206 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_pAnchorToken = &*__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
				(*__pAstNode).m_pAnchorToken->m_Data.m_Integer = 0; // tokens can be reused, ensure cleanup
			;
// #line 6708 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 13:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
// #line 212 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6721 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 14:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 221 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
				__pSymbol->m_Arg.pAnchorToken->m_Data.m_Integer |= ELandingPadFlag_Catch;
			;
// #line 6735 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 15:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 226 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
				__pSymbol->m_Arg.pAnchorToken->m_Data.m_Integer |= ELandingPadFlag_Finally;
			;
// #line 6749 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 16:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 231 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6762 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 17:
			{
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 246 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls30*) GetAstLocator (0)).m_TypeSpecifier);
			;
// #line 6775 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 18:
			{
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 251 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				CType* pType = __pSymbol->m_Local.Declarator.CalcType (__pSymbol->m_Arg.pCountValue);
				if (!pType)
					return false;

				*__pSymbol->m_Arg.ppType = pType;
			;
// #line 6792 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 19:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
// #line 274 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_TypeList.InsertTail ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 6805 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 20:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
// #line 278 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_TypeList.InsertTail ((*(_cls4*) GetAstLocator (1)).m_pType);
			;
// #line 6818 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 21:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
// #line 290 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_pType = (*(_cls4*) GetAstLocator (0)).m_pType;
			;
// #line 6831 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 22:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
// #line 294 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_pType = (*(_cls23*) GetAstLocator (0)).m_Value.GetType ();
			;
// #line 6844 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 23:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 305 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pAttributeBlock = m_pModule->m_AttributeMgr.CreateAttributeBlock ();
			;
// #line 6857 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 24:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 320 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				CAttribute* pAttribute = m_pAttributeBlock->CreateAttribute ((*GetTokenLocator (0)).m_Data.m_String, NULL);
				if (!pAttribute)
					return false;

				pAttribute->m_Pos = (*GetTokenLocator (0)).m_Pos;
			;
// #line 6874 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 25:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 14 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6887 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 26:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6900 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 27:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 23 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6913 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 28:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 28 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6926 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 29:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 32 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6939 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 30:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 37 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6952 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 31:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 41 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6965 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 32:
			{
			CSymbolNode_arg_pass1* __pSymbol = (CSymbolNode_arg_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6978 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 33:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 74 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_ExpressionValue = (*(_cls9*) GetAstLocator (0)).m_Value;
			;
// #line 6991 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 34:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 83 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_ExpressionValueList.TakeOver (&(*(_cls12*) GetAstLocator (0)).m_ValueList);
			;
// #line 7004 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 35:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
// #line 119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls9*) GetAstLocator (0)).m_Value);
			;
// #line 7017 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 36:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
// #line 123 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls9*) GetAstLocator (1)).m_Value);
			;
// #line 7030 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 37:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 139 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls9*) GetAstLocator (0)).m_Value);
			;
// #line 7043 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 38:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 143 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls10*) GetAstLocator (1)).m_Value);
			;
// #line 7056 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 39:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 147 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail (CValue ());
			;
// #line 7069 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 40:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 151 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls10*) GetAstLocator (0)).m_Value);
			;
// #line 7082 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 41:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
// #line 165 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				if ((*__pAstNode).m_Value.GetValueKind () != EValue_Const)
				{
					err::SetFormatStringError ("not a constant expression");
					return false;
				}
			;
// #line 7099 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 42:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
// #line 181 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				if ((*(_cls9*) GetAstLocator (0)).m_Value.GetValueKind () != EValue_Const ||
					!((*(_cls9*) GetAstLocator (0)).m_Value.GetType ()->GetTypeKindFlags () & ETypeKindFlag_Integer))
				{
					err::SetFormatStringError ("not a constant integer expression");
					return false;
				}

				bool Result = m_pModule->m_OperatorMgr.CastOperator (&(*(_cls9*) GetAstLocator (0)).m_Value, EType_Int32);
				if (!Result)
					return false;

				(*__pAstNode).m_Value = (*(_cls9*) GetAstLocator (0)).m_Value.GetInt32 ();
			;
// #line 7123 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 43:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("cond_then");
					(*__pAstNode).m_pElseBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("cond_else");
					(*__pAstNode).m_pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("cond_phi");
					m_pModule->m_ControlFlowMgr.ConditionalJump (*__pSymbol->m_Arg.pValue, (*__pAstNode).m_pThenBlock, (*__pAstNode).m_pElseBlock);
				;
// #line 7139 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 44:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 218 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock ((*__pAstNode).m_pElseBlock); // might have changed
				;
// #line 7152 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 45:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 223 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return m_pModule->m_OperatorMgr.ConditionalOperator (
						(*__pAstNode).m_TrueValue,
						(*__pAstNode).m_FalseValue,
						(*__pAstNode).m_pThenBlock,
						(*__pAstNode).m_pPhiBlock,
						__pSymbol->m_Arg.pValue
						);
				;
// #line 7171 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 46:
			{
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 242 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7184 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 47:
			{
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 255 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7197 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 48:
			{
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 268 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 49:
			{
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 281 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7223 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 50:
			{
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 294 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7236 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 51:
			{
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 307 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7249 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 52:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
// #line 319 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Eq;
		;
// #line 7262 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 53:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
// #line 323 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ne;
		;
// #line 7275 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 54:
			{
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 335 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7288 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 55:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 347 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Lt;
			;
// #line 7301 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 56:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 351 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Gt;
			;
// #line 7314 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 57:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 355 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Le;
			;
// #line 7327 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 58:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 359 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Ge;
			;
// #line 7340 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 59:
			{
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 371 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7353 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 60:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 383 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Shl;
			;
// #line 7366 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 61:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 387 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Shr;
			;
// #line 7379 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 62:
			{
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 399 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls19*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7392 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 63:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 411 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Add;
			;
// #line 7405 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 64:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 415 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Sub;
			;
// #line 7418 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 65:
			{
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 427 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls20*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7431 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 66:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 439 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Mul;
			;
// #line 7444 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 67:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 443 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Div;
			;
// #line 7457 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 68:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 447 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Mod;
			;
// #line 7470 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 69:
			{
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 459 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7483 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 70:
			{
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 480 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls21*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7496 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 71:
			{
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 484 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_CurlyInitializerTargetValue = *__pSymbol->m_Arg.pValue;
			;
// #line 7509 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 72:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 496 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Assign;
			;
// #line 7522 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 73:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 500 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_RefAssign;
			;
// #line 7535 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 74:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 504 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_AddAssign;
			;
// #line 7548 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 75:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 508 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_SubAssign;
			;
// #line 7561 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 76:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 512 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_MulAssign;
			;
// #line 7574 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 77:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 516 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_DivAssign;
			;
// #line 7587 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 78:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 520 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ModAssign;
			;
// #line 7600 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 79:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 524 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ShlAssign;
			;
// #line 7613 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 80:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 528 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ShrAssign;
			;
// #line 7626 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 81:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 532 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_AndAssign;
			;
// #line 7639 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 82:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 536 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_XorAssign;
			;
// #line 7652 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 83:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 540 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_OrAssign;
			;
// #line 7665 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 84:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 557 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Plus, __pSymbol->m_Arg.pValue);
			;
// #line 7678 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 85:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 561 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Minus, __pSymbol->m_Arg.pValue);
			;
// #line 7691 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 86:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 565 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
			;
// #line 7704 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 87:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 569 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, __pSymbol->m_Arg.pValue);
			;
// #line 7717 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 88:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 573 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, __pSymbol->m_Arg.pValue);
			;
// #line 7730 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 89:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 577 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
			;
// #line 7743 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 90:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 581 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
			;
// #line 7756 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 91:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 585 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
			;
// #line 7769 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 92:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 589 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_pModule->m_ControlFlowMgr.LockThrow ();
			;
// #line 7782 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 93:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 593 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_pModule->m_ControlFlowMgr.UnlockThrow ();
			;
// #line 7795 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 94:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 598 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
				return m_pModule->m_OperatorMgr.CastOperator (StorageKind, __pSymbol->m_Arg.pValue, (*(_cls4*) GetAstLocator (1)).m_pType);
			;
// #line 7809 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 95:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 603 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
				return m_pModule->m_OperatorMgr.NewOperator (
					StorageKind,
					(*(_cls22*) GetAstLocator (1)).m_pType,
					(*(_cls22*) GetAstLocator (1)).m_ElementCountValue,
					&(*(_cls22*) GetAstLocator (1)).m_ArgList,
					__pSymbol->m_Arg.pValue
					);
			;
// #line 7829 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 96:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 615 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				err::SetFormatStringError ("manual memory management is not supported yet");
				return false;
			;
// #line 7843 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 97:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
// #line 631 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
				(*__pAstNode).m_ArgList.TakeOver (&(*(_cls12*) GetAstLocator (1)).m_ValueList);
			;
// #line 7857 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 98:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 672 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.CallOperator (__pSymbol->m_Arg.pValue, &(*(_cls12*) GetAstLocator (0)).m_ValueList);
			;
// #line 7870 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 99:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 676 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.ClosureOperator (__pSymbol->m_Arg.pValue, &(*(_cls12*) GetAstLocator (0)).m_ValueList);
			;
// #line 7883 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 100:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 680 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls10*) GetAstLocator (0)).m_Value);
			;
// #line 7896 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 101:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 684 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
			;
// #line 7909 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 102:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 688 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
			;
// #line 7922 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 103:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 693 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.WeakenOperator (__pSymbol->m_Arg.pValue);
			;
// #line 7935 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 104:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 699 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
			;
// #line 7948 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 105:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 707 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.MemberOperator (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 7961 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 106:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 711 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyGetter (__pSymbol->m_Arg.pValue);
			;
// #line 7974 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 107:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 715 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertySetter (__pSymbol->m_Arg.pValue);
			;
// #line 7987 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 108:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 732 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return LookupIdentifier ((*GetTokenLocator (0)).m_Data.m_String, (*GetTokenLocator (0)).m_Pos, __pSymbol->m_Arg.pValue);
			;
// #line 8000 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 109:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 736 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return GetThisValue (__pSymbol->m_Arg.pValue);
			;
// #line 8013 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 110:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 740 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return GetPitcherReturnValue (__pSymbol->m_Arg.pValue);
			;
// #line 8026 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 111:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 744 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstInt64_u ((*GetTokenLocator (0)).m_Data.m_Int64_u);
			;
// #line 8039 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 112:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 748 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstDouble ((*GetTokenLocator (0)).m_Data.m_Double);
			;
// #line 8052 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 113:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 752 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstBool (true);
			;
// #line 8065 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 114:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 756 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstBool (false);
			;
// #line 8078 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 115:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 760 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetNull ();
			;
// #line 8091 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 116:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 764 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				__pSymbol->m_Arg.pValue->SetConstSizeT (pType->GetSize ());
			;
// #line 8105 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 117:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 769 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				return GetCountOf (pType, __pSymbol->m_Arg.pValue);
			;
// #line 8119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 118:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 774 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				__pSymbol->m_Arg.pValue->SetType ((*(_cls6*) GetAstLocator (0)).m_pType);
			;
// #line 8133 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 119:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 779 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			;
// #line 8145 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 120:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 782 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyOnChanged (__pSymbol->m_Arg.pValue);
			;
// #line 8158 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 121:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 795 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					__pSymbol->m_Local.Literal.m_BinData.Append ((const uchar_t*) (*GetTokenLocator (0)).m_Data.m_String.cc (), (*GetTokenLocator (0)).m_Data.m_String.GetLength ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_Literal;
				;
// #line 8172 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 122:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 800 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					__pSymbol->m_Local.Literal.m_BinData.Append ((*GetTokenLocator (1)).m_Data.m_BinData, (*GetTokenLocator (1)).m_Data.m_BinData.GetCount ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_HexLiteral;
				;
// #line 8186 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 123:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 805 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return AppendFmtLiteral (&__pSymbol->m_Local.Literal, (*GetTokenLocator (2)).m_Data.m_String.cc (), (*GetTokenLocator (2)).m_Data.m_String.GetLength ());
				;
// #line 8199 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 124:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 810 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return FinalizeLiteral (&__pSymbol->m_Local.Literal, __pSymbol->m_Arg.pValue);
			;
// #line 8212 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 125:
			{
			CSymbolNode_fmt_literal_value* __pSymbol = (CSymbolNode_fmt_literal_value*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 817 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return IsValidLocator ((*GetTokenLocator (0))) ?
					AppendFmtLiteralValue (__pSymbol->m_Arg.pLiteral, (*(_cls9*) GetAstLocator (1)).m_Value, (*GetTokenLocator (0)).m_Data.m_String) :
					AppendFmtLiteralValue (__pSymbol->m_Arg.pLiteral, (*(_cls9*) GetAstLocator (1)).m_Value, rtl::CString ());
			;
// #line 8227 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 126:
			{
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 841 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				if (!__pSymbol->m_Local.Initializer.m_Count)
				{
					err::SetFormatStringError ("empty curly initializer");
					return false;
				}
			;
// #line 8244 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 127:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 858 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerNamedItem (__pSymbol->m_Arg.pInitializer, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 8257 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 128:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 863 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, __pSymbol->m_Arg.pInitializer->m_MemberValue, (*(_cls9*) GetAstLocator (1)).m_Value);
				;
// #line 8270 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 129:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 867 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerIndexedItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 8283 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 130:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 871 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, __pSymbol->m_Arg.pInitializer->m_MemberValue, (*(_cls9*) GetAstLocator (0)).m_Value);
			;
// #line 8296 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 131:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 874 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerIndexedItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 8309 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 132:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 879 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return SkipCurlyInitializerItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 8322 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 133:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 6 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				m_ExpressionValue = (*(_cls23*) GetAstLocator (0)).m_Value;
			;
// #line 8335 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 134:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 36 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (0)).m_Value);
			;
// #line 8348 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 135:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 40 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (1)).m_Value);
			;
// #line 8361 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 136:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 56 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (0)).m_Value);
			;
// #line 8374 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 137:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 60 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls24*) GetAstLocator (1)).m_Value);
			;
// #line 8387 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 138:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail (CValue ());
			;
// #line 8400 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 139:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 68 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls24*) GetAstLocator (0)).m_Value);
			;
// #line 8413 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 140:
			{
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 85 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetConditionalOperatorResultType (__pSymbol->m_Local.TrueValue, __pSymbol->m_Local.FalseValue, __pSymbol->m_Arg.pValue);
			;
// #line 8426 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 141:
			{
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 98 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8439 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 142:
			{
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 111 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8452 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 143:
			{
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 124 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8465 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 144:
			{
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 137 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8478 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 145:
			{
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 150 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8491 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 146:
			{
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 163 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8504 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 147:
			{
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 176 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8517 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 148:
			{
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 189 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8530 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 149:
			{
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 202 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls19*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8543 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 150:
			{
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 215 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls20*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8556 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 151:
			{
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 228 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8569 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 152:
			{
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 241 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					if (IsReactor ())
						m_ReactorBindSiteCount = 0; // reset on assignment
				;
// #line 8583 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 153:
			{
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 246 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls21*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
				;
// #line 8596 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 154:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 264 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Plus, __pSymbol->m_Arg.pValue);
			;
// #line 8609 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 155:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 268 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Minus, __pSymbol->m_Arg.pValue);
			;
// #line 8622 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 156:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 272 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
			;
// #line 8635 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 157:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 276 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Addr, __pSymbol->m_Arg.pValue);
			;
// #line 8648 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 158:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 280 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Indir, __pSymbol->m_Arg.pValue);
			;
// #line 8661 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 159:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 284 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
			;
// #line 8674 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 160:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 288 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
			;
// #line 8687 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 161:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
			;
// #line 8700 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 162:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 296 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				// try operator does not change type of value				
			;
// #line 8713 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 163:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 300 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_SizeT);
			;
// #line 8726 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 164:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 304 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_SizeT);
			;
// #line 8739 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 165:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 308 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				__pSymbol->m_Arg.pValue->SetType ((*(_cls6*) GetAstLocator (0)).m_pType);
			;
// #line 8753 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 166:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 313 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
			;
// #line 8765 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 167:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 316 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyOnChangedType (__pSymbol->m_Arg.pValue);
			;
// #line 8778 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 168:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 321 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 8791 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 169:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 325 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
				return NewOperator_s (StorageKind, (*(_cls27*) GetAstLocator (1)).m_pType, __pSymbol->m_Arg.pValue);			
			;
// #line 8805 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 170:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 330 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetVoid ();
			;
// #line 8818 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 171:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
// #line 341 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_pType = (*(_cls4*) GetAstLocator (0)).m_pType;
			;
// #line 8831 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 172:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
// #line 346 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
			;
// #line 8844 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 173:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 363 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetCallOperatorResultType (__pSymbol->m_Arg.pValue, &(*(_cls26*) GetAstLocator (0)).m_ValueList);
			;
// #line 8857 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 174:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 367 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetClosureOperatorResultType (__pSymbol->m_Arg.pValue, &(*(_cls26*) GetAstLocator (0)).m_ValueList);
			;
// #line 8870 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 175:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 371 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls24*) GetAstLocator (0)).m_Value);
			;
// #line 8883 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 176:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 375 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
			;
// #line 8896 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 177:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 379 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
			;
// #line 8909 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 178:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 384 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetWeakenOperatorResultType (__pSymbol->m_Arg.pValue);
			;
// #line 8922 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 179:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 390 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
			;
// #line 8935 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 180:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 398 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetMemberOperatorResultType (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 8948 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 181:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 402 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyGetterType (__pSymbol->m_Arg.pValue);
			;
// #line 8961 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 182:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 406 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertySetterType (__pSymbol->m_Arg.pValue);
			;
// #line 8974 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 183:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 418 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return LookupIdentifierType ((*GetTokenLocator (0)).m_Data.m_String, (*GetTokenLocator (0)).m_Pos, __pSymbol->m_Arg.pValue);
			;
// #line 8987 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 184:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 422 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return GetThisValueType (__pSymbol->m_Arg.pValue);
			;
// #line 9000 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 185:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 426 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return GetPitcherReturnValueType (__pSymbol->m_Arg.pValue);
			;
// #line 9013 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 186:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 430 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (GetInt64TypeKind ((*GetTokenLocator (0)).m_Data.m_Int64));
			;
// #line 9026 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 187:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 434 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Double);
			;
// #line 9039 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 188:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 438 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Bool);
			;
// #line 9052 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 189:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 442 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Bool);
			;
// #line 9065 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 190:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 446 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetNull ();
			;
// #line 9078 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 191:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 459 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
						
					__pSymbol->m_Local.Literal.m_BinData.Append ((const uchar_t*) (*GetTokenLocator (0)).m_Data.m_String.cc (), (*GetTokenLocator (0)).m_Data.m_String.GetLength ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_Literal;
				;
// #line 9092 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 192:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 464 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
						
					__pSymbol->m_Local.Literal.m_BinData.Append ((*GetTokenLocator (1)).m_Data.m_BinData, (*GetTokenLocator (1)).m_Data.m_BinData.GetCount ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_HexLiteral;
				;
// #line 9106 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 193:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 469 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					// mark we have a formatting literal from now on

					if (!__pSymbol->m_Local.Literal.m_FmtLiteralValue)
						__pSymbol->m_Local.Literal.m_FmtLiteralValue.SetConstBool (true); 
				;
// #line 9122 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 194:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 476 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return FinalizeLiteral_s (&__pSymbol->m_Local.Literal, __pSymbol->m_Arg.pValue);
			;
// #line 9135 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 195:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 6 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
				PreCreateLandingPads ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9149 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 196:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 12 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.CloseScope ();
			;
// #line 9162 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 197:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 20 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
				PreCreateLandingPads ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9176 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 198:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 25 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeBaseTypeMemberConstructBlock ();
			;
// #line 9189 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 199:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 30 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.CloseScope ();
			;
// #line 9202 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 200:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 42 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return CallBaseTypeMemberConstructor ((*(_cls28*) GetAstLocator (0)).m_Name, &(*(_cls12*) GetAstLocator (1)).m_ValueList);
			;
// #line 9215 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 201:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 46 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return CallBaseTypeConstructor ((*GetTokenLocator (0)).m_Data.m_Integer - 1, &(*(_cls12*) GetAstLocator (1)).m_ValueList);
			;
// #line 9228 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 202:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
// #line 62 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				(*__pAstNode).m_Name.AddName ((*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 9241 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 203:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
// #line 66 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				(*__pAstNode).m_Name.AddName ((*GetTokenLocator (1)).m_Data.m_String);
			;
// #line 9254 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 204:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 112 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.IfStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9267 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 205:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 118 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.IfStmt_Else (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (2)).m_Pos);
				;
// #line 9280 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 206:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 123 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.IfStmt_Follow (&__pSymbol->m_Local.Stmt);
			;
// #line 9293 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 207:
			{
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 138 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return
					m_pModule->m_ControlFlowMgr.SwitchStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos) &&
					PreCreateLandingPads ((*GetTokenLocator (1)).m_Data.m_Integer);
			;
// #line 9308 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 208:
			{
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 144 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.SwitchStmt_Follow (&__pSymbol->m_Local.Stmt);
			;
// #line 9321 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 209:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 151 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.SwitchStmt_Case (__pSymbol->m_Arg.pStmt, (*(_cls14*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9334 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 210:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 155 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.SwitchStmt_Default (__pSymbol->m_Arg.pStmt, (*GetTokenLocator (0)).m_Pos);
			;
// #line 9347 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 211:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 171 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.WhileStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9360 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 212:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 175 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.WhileStmt_Follow (&__pSymbol->m_Local.Stmt);
			;
// #line 9373 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 213:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 190 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.DoStmt_PreBody (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
			;
// #line 9386 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 214:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 194 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.DoStmt_PostBody (&__pSymbol->m_Local.Stmt);
			;
// #line 9399 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 215:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 198 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.DoStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (1)).m_Value);
			;
// #line 9412 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 216:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 213 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PreInit (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
			;
// #line 9425 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 217:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 218 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PreCondition (&__pSymbol->m_Local.Stmt);
				;
// #line 9438 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 218:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 222 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					return m_pModule->m_ControlFlowMgr.ForStmt_PostCondition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (1)).m_Value);
				;
// #line 9451 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 219:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 227 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PreLoop (&__pSymbol->m_Local.Stmt);
				;
// #line 9464 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 220:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 231 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PostLoop (&__pSymbol->m_Local.Stmt);
				;
// #line 9477 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 221:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 235 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PreBody (&__pSymbol->m_Local.Stmt);
			;
// #line 9490 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 222:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 239 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PostBody (&__pSymbol->m_Local.Stmt);
			;
// #line 9503 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 223:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 246 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Break ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9516 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 224:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 253 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Continue ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9529 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 225:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 260 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Return (IsValidLocator ((*(_cls9*) GetAstLocator (0))) ? (*(_cls9*) GetAstLocator (0)).m_Value : CValue ());
			;
// #line 9542 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 226:
			{
			CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 271 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Static;
				return
					m_pModule->m_ControlFlowMgr.OnceStmt_Create (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (1)).m_Pos, StorageKind) &&
					m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9558 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 227:
			{
			CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 278 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.OnceStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
			;
// #line 9571 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 228:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 286 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
				PreCreateLandingPads ((*GetTokenLocator (0)).m_Data.m_Integer | ELandingPadFlag_Catch); // always catch
			;
// #line 9585 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 229:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.EndTry ();
				m_pModule->m_NamespaceMgr.CloseScope ();
			;
// #line 9599 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 230:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 301 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Catch ();
			;
// #line 9612 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 231:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 308 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Finally ();
			;
// #line 9625 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 232:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 318 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
			return FinalizeReactor ();
		;
// #line 9638 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 233:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 324 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			 return true; ;
// #line 9649 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 234:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 336 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeReactorOnChangedClause ();
			;
// #line 9662 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 235:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 340 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeReactorOnChangedStmt ();
			;
// #line 9675 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 236:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 347 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return ReactorExpressionStmt (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 9688 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 237:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 84 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			 return true; ;
// #line 9699 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 238:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 92 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			
				return CountReactorBindSites ();
			;
// #line 9712 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 239:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 100 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			
				return CountReactorBindSites ();
			;
// #line 9725 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 240:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 22 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return SetStorageKind ((*(_cls31*) GetAstLocator (0)).m_StorageKind);
			;
// #line 9738 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 241:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 26 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return SetAccessKind ((*(_cls32*) GetAstLocator (0)).m_AccessKind);
			;
// #line 9751 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 242:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 30 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls33*) GetAstLocator (0)).m_pType);
			;
// #line 9764 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 243:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 34 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls36*) GetAstLocator (0)).m_TypeModifier);
			;
// #line 9777 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 244:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 60 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls33*) GetAstLocator (0)).m_pType);
			;
// #line 9790 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 245:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls36*) GetAstLocator (0)).m_TypeModifier);
			;
// #line 9803 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 246:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 77 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Typedef;
			;
// #line 9816 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 247:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 81 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Alias;
			;
// #line 9829 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 248:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 85 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Static;
			;
// #line 9842 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 249:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 89 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Thread;
			;
// #line 9855 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 250:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 93 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Stack;
			;
// #line 9868 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 251:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 97 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Heap;
			;
// #line 9881 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 252:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 101 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				// (*__pAstNode).m_StorageKind = EStorage_UHeap;
				err::SetFormatStringError ("manual memory management is not supported yet");
				return false;
			;
// #line 9896 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 253:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 107 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Abstract;
			;
// #line 9909 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 254:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 111 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Virtual;
			;
// #line 9922 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 255:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 115 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Override;
			;
// #line 9935 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 256:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Mutable;
			;
// #line 9948 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 257:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 123 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Nullable;
			;
// #line 9961 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 258:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
// #line 136 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_AccessKind = EAccess_Public;
			;
// #line 9974 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 259:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
// #line 140 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_AccessKind = EAccess_Protected;
			;
// #line 9987 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 260:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 153 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
			;
// #line 10000 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 261:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 157 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				err::SetStringError ("'variant' type specifier is not yet supported");
				return false;
			;
// #line 10014 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 262:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 162 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectClass);
			;
// #line 10027 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 263:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 166 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetBoxClassType ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 10040 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 264:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 170 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Bool);
			;
// #line 10053 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 265:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 174 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int8);
			;
// #line 10066 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 266:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 178 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int16);
			;
// #line 10079 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 267:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 182 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32);
			;
// #line 10092 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 268:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 186 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64);
			;
// #line 10105 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 269:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 190 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int);
			;
// #line 10118 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 270:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 194 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
			;
// #line 10131 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 271:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 198 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char);
			;
// #line 10144 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 272:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 202 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Float);
			;
// #line 10157 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 273:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 206 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Double);
			;
// #line 10170 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 274:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				err::SetStringError ("'auto' type specifier is not yet supported");
				return false;
			;
// #line 10184 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 275:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 216 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls35*) GetAstLocator (0)).m_pType;
			;
// #line 10197 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 276:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 221 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
			;
// #line 10210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 277:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 229 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				if (IsTypeSpecified ())
					return false;
			;
// #line 10224 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 278:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 234 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return m_Stage == EStage_Pass1 || FindType ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos) != NULL;
			;
// #line 10237 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 279:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 238 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return false;
			;
// #line 10250 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 280:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
// #line 250 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				CType* pType = FindType ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos);
				if (!pType)
				{
					err::SetFormatStringError ("'%s' is not a type", (*(_cls1*) GetAstLocator (0)).m_Name.GetFullName ().cc ());
					return false;
				}

				(*__pAstNode).m_pType = pType;
			;
// #line 10270 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 281:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 272 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				__pSymbol->m_Local.pTemplate = CreatePropertyTemplate ();
			;
// #line 10283 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 282:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 276 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = __pSymbol->m_Local.pTemplate->CalcType ();
				return (*__pAstNode).m_pType != NULL;
			;
// #line 10297 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 283:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 290 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Unsigned;
			;
// #line 10310 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 284:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 294 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_BigEndian;
			;
// #line 10323 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 285:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 298 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Const;
			;
// #line 10336 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 286:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 302 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_DConst;
			;
// #line 10349 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 287:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 306 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Volatile;
			;
// #line 10362 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 288:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 310 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Weak;
			;
// #line 10375 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 289:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 314 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Thin;
			;
// #line 10388 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 290:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 318 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Pitcher;
			;
// #line 10401 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 291:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 322 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Cdecl;
			;
// #line 10414 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 292:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 326 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Stdcall;
			;
// #line 10427 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 293:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 330 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Array;
			;
// #line 10440 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 294:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 334 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Function;
			;
// #line 10453 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 295:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 338 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Property;
			;
// #line 10466 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 296:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 342 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Bindable;
			;
// #line 10479 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 297:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 346 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_AutoGet;
			;
// #line 10492 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 298:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 350 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Indexed;
			;
// #line 10505 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 299:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 354 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Multicast;
			;
// #line 10518 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 300:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 358 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Event;
			;
// #line 10531 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 301:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 362 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_DEvent;
			;
// #line 10544 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 302:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 366 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Reactor;
			;
// #line 10557 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 303:
			{
			CSymbolNode_pitcher_modifier* __pSymbol = (CSymbolNode_pitcher_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 375 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetPitcherCondition (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 10570 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 304:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 14 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*(_cls37*) GetAstLocator (0)).m_Declarator.m_Constructor.TakeOver (&(*(_cls8*) GetAstLocator (1)).m_TokenList);
			;
// #line 10583 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 305:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*(_cls37*) GetAstLocator (0)).m_Declarator.m_Initializer.TakeOver (&(*(_cls7*) GetAstLocator (2)).m_TokenList);
			;
// #line 10596 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 306:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 23 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return Declare (&(*(_cls37*) GetAstLocator (0)).m_Declarator);
			;
// #line 10609 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 307:
			{
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 49 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->m_Pos = (*GetAstLocator (0)).m_FirstToken.m_Pos;
			;
// #line 10622 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 308:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 57 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddName ((*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 10635 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 309:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 61 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Getter);
			;
// #line 10648 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 310:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 65 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Setter);
			;
// #line 10661 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 311:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 69 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_PreConstructor);
			;
// #line 10674 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 312:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 73 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Constructor);
			;
// #line 10687 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 313:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 77 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Destructor);
			;
// #line 10700 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 314:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 81 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticConstructor);
			;
// #line 10713 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 315:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 85 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticDestructor);
			;
// #line 10726 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 316:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 89 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_CallOperator);
			;
// #line 10739 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 317:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 93 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddCastOperator ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 10752 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 318:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 97 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnaryBinaryOperator ((*(_cls38*) GetAstLocator (0)).m_UnOpKind, (*(_cls38*) GetAstLocator (0)).m_BinOpKind);
			;
// #line 10765 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 319:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 116 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_BwNot;
			;
// #line 10778 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 320:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 120 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_LogNot;
			;
// #line 10791 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 321:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 124 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PreInc;
			;
// #line 10804 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 322:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 128 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PreDec;
			;
// #line 10817 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 323:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 132 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PostInc;
			;
// #line 10830 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 324:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 136 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PostDec;
			;
// #line 10843 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 325:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 140 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Ptr;
			;
// #line 10856 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 326:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 144 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Plus;
				(*__pAstNode).m_BinOpKind = EBinOp_Add;
			;
// #line 10870 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 327:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 149 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Minus;
				(*__pAstNode).m_BinOpKind = EBinOp_Sub;
			;
// #line 10884 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 328:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 154 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Indir;
				(*__pAstNode).m_BinOpKind = EBinOp_Mul;
			;
// #line 10898 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 329:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 159 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Div;
			;
// #line 10911 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 330:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 163 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Mod;
			;
// #line 10924 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 331:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 167 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Shl;
			;
// #line 10937 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 332:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 171 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Shr;
			;
// #line 10950 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 333:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 175 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Addr;
				(*__pAstNode).m_BinOpKind = EBinOp_BwAnd;
			;
// #line 10964 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 334:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 180 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_BwXor;
			;
// #line 10977 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 335:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 184 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_BwOr;
			;
// #line 10990 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 336:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 188 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Eq;
			;
// #line 11003 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 337:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 192 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Ne;
			;
// #line 11016 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 338:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 196 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Lt;
			;
// #line 11029 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 339:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 200 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Le;
			;
// #line 11042 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 340:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 204 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Gt;
			;
// #line 11055 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 341:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 208 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Ge;
			;
// #line 11068 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 342:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 212 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Assign;
			;
// #line 11081 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 343:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 216 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_RefAssign;
			;
// #line 11094 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 344:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 220 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AddAssign;
			;
// #line 11107 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 345:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 224 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_SubAssign;
			;
// #line 11120 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 346:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 228 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_MulAssign;
			;
// #line 11133 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 347:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 232 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_DivAssign;
			;
// #line 11146 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 348:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 236 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ModAssign;
			;
// #line 11159 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 349:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 240 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ShlAssign;
			;
// #line 11172 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 350:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 244 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ShrAssign;
			;
// #line 11185 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 351:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 248 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AndAssign;
			;
// #line 11198 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 352:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 252 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_XorAssign;
			;
// #line 11211 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 353:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 256 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_OrAssign;
			;
// #line 11224 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 354:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 260 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AtAssign;
			;
// #line 11237 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 355:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 264 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_LogAnd;
			;
// #line 11250 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 356:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 268 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_LogOr;
			;
// #line 11263 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 357:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 272 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Idx;
			;
// #line 11276 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 358:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 276 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_At;
			;
// #line 11289 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 359:
			{
			CSymbolNode_declarator_prefix* __pSymbol = (CSymbolNode_declarator_prefix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 288 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddPointerPrefix ();
			;
// #line 11302 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 360:
			{
			CSymbolNode_declarator_prefix* __pSymbol = (CSymbolNode_declarator_prefix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->SetTypeModifier ((*(_cls36*) GetAstLocator (0)).m_TypeModifier);
			;
// #line 11315 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 361:
			{
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 304 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			 return false; ;
// #line 11326 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 362:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 312 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 11339 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 363:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 316 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 11352 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 364:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 320 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix (-1);
			;
// #line 11365 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 365:
			{
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 355 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_VarArg;
			;
// #line 11378 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 366:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 370 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier);
			;
// #line 11391 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 367:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 377 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.m_Initializer.TakeOver (&(*(_cls7*) GetAstLocator (1)).m_TokenList);
			;
// #line 11404 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 368:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 381 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return CreateFormalArg (__pSymbol->m_Arg.pFunctionSuffix, &__pSymbol->m_Local.Declarator) != NULL;
			;
// #line 11417 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 369:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 400 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.m_Initializer.TakeOver (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 11430 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 370:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 404 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return CreateFormalArg (__pSymbol->m_Arg.pFunctionSuffix, &__pSymbol->m_Local.Declarator) != NULL;
			;
// #line 11443 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 371:
			{
			CSymbolNode_bitfield_suffix* __pSymbol = (CSymbolNode_bitfield_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 413 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddBitFieldSuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 11456 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 372:
			{
			CSymbolNode_post_declarator_modifier* __pSymbol = (CSymbolNode_post_declarator_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 422 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->SetPostDeclaratorModifier (EPostDeclaratorModifier_Const);
			;
// #line 11469 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 373:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 11 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls40*) GetAstLocator (0)).m_pType;
			;
// #line 11482 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 374:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 15 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls42*) GetAstLocator (0)).m_pType;
			;
// #line 11495 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 375:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 19 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls45*) GetAstLocator (0)).m_pType;
			;
// #line 11508 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 376:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 23 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls46*) GetAstLocator (0)).m_pType;
			;
// #line 11521 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 377:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
// #line 45 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateEnumType (
					(*(_cls41*) GetAstLocator (0)).m_EnumTypeKind,
					IsValidLocator ((*GetTokenLocator (1))) ? (*GetTokenLocator (1)).m_Data.m_String : rtl::CString (),
					IsValidLocator ((*(_cls4*) GetAstLocator (2))) ? (*(_cls4*) GetAstLocator (2)).m_pType : NULL,
					(*(_cls41*) GetAstLocator (0)).m_Flags
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11542 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 378:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 68 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_EnumTypeKind = EEnumType_Normal;
				(*__pAstNode).m_Flags = 0;
			;
// #line 11556 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 379:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 73 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_EnumTypeKind = EEnumType_Flag;
				(*__pAstNode).m_Flags = 0;
			;
// #line 11570 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 380:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 78 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_EnumTypeKind = EEnumType_Normal;
				(*__pAstNode).m_Flags = EEnumTypeFlag_Exposed;
			;
// #line 11584 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 381:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 88 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);
			;
// #line 11597 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 382:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 93 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pModule->m_NamespaceMgr.CloseNamespace ();
			;
// #line 11610 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 383:
			{
			CSymbolNode_enum_member* __pSymbol = (CSymbolNode_enum_member*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 106 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				CEnumConst* pMember = __pSymbol->m_Arg.pType->CreateConst (
					(*GetTokenLocator (0)).m_Data.m_String,
					IsValidLocator ((*(_cls7*) GetAstLocator (1))) ? &(*(_cls7*) GetAstLocator (1)).m_TokenList : NULL
					);

				if (!pMember)
					return false;

				pMember->m_Pos = (*GetTokenLocator (0)).m_Pos;
			;
// #line 11631 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 384:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
// #line 138 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateStructType (
					IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
					IsValidLocator ((*(_cls5*) GetAstLocator (1))) ? &(*(_cls5*) GetAstLocator (1)).m_TypeList : NULL,
					IsValidLocator ((*(_cls43*) GetAstLocator (2))) ? (*(_cls43*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11651 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 385:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls43* __pAstNode = (_cls43*) __pSymbol->m_pAstNode;
// #line 159 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_PackFactor = (*(_cls44*) GetAstLocator (0)).m_Value;
			;
// #line 11664 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 386:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
// #line 170 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_Value = (*GetTokenLocator (0)).m_Data.m_Integer;
			;
// #line 11677 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 387:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
// #line 174 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_Value = m_DefaultStructPackFactor;
			;
// #line 11690 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 388:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
// #line 195 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateUnionType (IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString ());
				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11705 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 389:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
// #line 219 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateClassType (
					(*GetTokenLocator (0)).m_Data.m_String,
					IsValidLocator ((*(_cls5*) GetAstLocator (1))) ? &(*(_cls5*) GetAstLocator (1)).m_TypeList : NULL,
					IsValidLocator ((*(_cls43*) GetAstLocator (2))) ? (*(_cls43*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor,
					IsValidLocator ((*GetTokenLocator (3))) ? EClassTypeFlag_Opaque : 0
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11726 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
			CSymbolNode_qualified_name_impl* __pTarget = (CSymbolNode_qualified_name_impl*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls1* __pAstNode = (_cls1*) __pSymbol->m_pAstNode;
			
// #line 102 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pName = &(*__pAstNode).m_Name;
// #line 11754 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_declarator_list* __pTarget = (CSymbolNode_declarator_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 128 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier;
// #line 11767 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 129 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier;
// #line 11780 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 145 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11793 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 146 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11806 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 147 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11819 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 147 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11832 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 148 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11845 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 152 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = NULL;
// #line 11858 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier_rslv* __pSymbol = (CSymbolNode_item_declaration_no_specifier_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 166 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11871 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
// #line 197 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 11884 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_statement_pass1* __pTarget = (CSymbolNode_statement_pass1*) pSymbol;
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
			
// #line 210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11897 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pAnchorToken =  (*__pAstNode).m_pAnchorToken;
// #line 11901 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 219 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11914 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 249 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11927 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 250 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11940 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_type_name_impl* __pTarget = (CSymbolNode_type_name_impl*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
			
// #line 265 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.ppType = &(*__pAstNode).m_pType;
// #line 11953 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 265 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pCountValue =  NULL;
// #line 11957 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
			
// #line 8 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 11970 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 17 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11983 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 26 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11996 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 35 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12009 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_arg_pass1* __pTarget = (CSymbolNode_arg_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
			
// #line 55 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 12022 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_arg_pass1* __pTarget = (CSymbolNode_arg_pass1*) pSymbol;
			CSymbolNode_arg_pass1* __pSymbol = (CSymbolNode_arg_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 61 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12035 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
			
// #line 100 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12048 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
			
// #line 109 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12061 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
			
// #line 164 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12074 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_logical_or_expr* __pTarget = (CSymbolNode_logical_or_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
// #line 208 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12087 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
// #line 217 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_TrueValue;
// #line 12100 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
// #line 222 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_FalseValue;
// #line 12113 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 240 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12126 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 241 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12139 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 253 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12152 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 254 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12165 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 266 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12178 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 267 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12191 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 279 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12204 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 280 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12217 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 292 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12230 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 293 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12243 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 305 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12256 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 306 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12269 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 333 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12282 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 41:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 334 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12295 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 42:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 369 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12308 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 43:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 370 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12321 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 44:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 397 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12334 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 45:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 398 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12347 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 46:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 425 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12360 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 47:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 426 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12373 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 48:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 457 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12386 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 49:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 458 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12399 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 50:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 466 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12412 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 51:
			{
			CSymbolNode_assignment_operator_expr* __pTarget = (CSymbolNode_assignment_operator_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 466 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12425 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 52:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 479 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12438 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 53:
			{
			CSymbolNode_postfix_expr* __pTarget = (CSymbolNode_postfix_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 546 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12451 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 54:
			{
			CSymbolNode_unary_operator_expr* __pTarget = (CSymbolNode_unary_operator_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 547 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12464 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 55:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 556 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12477 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 56:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 560 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12490 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 57:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 564 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12503 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 58:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 568 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12516 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 59:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 572 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12529 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 60:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 576 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12542 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 61:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 580 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12555 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 62:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 584 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12568 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 63:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 592 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12581 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 64:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 597 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12594 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 65:
			{
			CSymbolNode_new_operator_curly_initializer* __pTarget = (CSymbolNode_new_operator_curly_initializer*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 613 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12607 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 66:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 614 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12620 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 67:
			{
			CSymbolNode_type_name_impl* __pTarget = (CSymbolNode_type_name_impl*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
			
// #line 628 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.ppType = &(*__pAstNode).m_pType;
// #line 12633 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 628 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pCountValue =  &(*__pAstNode).m_ElementCountValue;
// #line 12637 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 68:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_cast_operator_rslv* __pSymbol = (CSymbolNode_cast_operator_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 657 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTypeSpecifier = &__pSymbol->m_Local.TypeSpecifier;
// #line 12650 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 69:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 661 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12663 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 70:
			{
			CSymbolNode_postfix_operator* __pTarget = (CSymbolNode_postfix_operator*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 662 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12676 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 71:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 697 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12689 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 72:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 702 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12702 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 73:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 729 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12715 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 74:
			{
			CSymbolNode_literal* __pTarget = (CSymbolNode_literal*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 730 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12728 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 75:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 781 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12741 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 76:
			{
			CSymbolNode_fmt_literal_value* __pTarget = (CSymbolNode_fmt_literal_value*) pSymbol;
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 808 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pLiteral = &__pSymbol->m_Local.Literal;
// #line 12754 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 77:
			{
			CSymbolNode_curly_initializer_item* __pTarget = (CSymbolNode_curly_initializer_item*) pSymbol;
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 840 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pInitializer = &__pSymbol->m_Local.Initializer;
// #line 12767 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 78:
			{
			CSymbolNode_curly_initializer_item* __pTarget = (CSymbolNode_curly_initializer_item*) pSymbol;
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 840 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pInitializer = &__pSymbol->m_Local.Initializer;
// #line 12780 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 79:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls23* __pAstNode = (_cls23*) __pSymbol->m_pAstNode;
			
// #line 17 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12793 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 80:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
			
// #line 26 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12806 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 81:
			{
			CSymbolNode_logical_or_expr_s* __pTarget = (CSymbolNode_logical_or_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 83 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12819 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 82:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 84 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.TrueValue;
// #line 12832 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 83:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 84 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.FalseValue;
// #line 12845 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 84:
			{
			CSymbolNode_logical_and_expr_s* __pTarget = (CSymbolNode_logical_and_expr_s*) pSymbol;
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 96 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12858 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 85:
			{
			CSymbolNode_logical_and_expr_s* __pTarget = (CSymbolNode_logical_and_expr_s*) pSymbol;
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 97 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12871 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 86:
			{
			CSymbolNode_inclusive_or_expr_s* __pTarget = (CSymbolNode_inclusive_or_expr_s*) pSymbol;
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 109 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12884 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 87:
			{
			CSymbolNode_inclusive_or_expr_s* __pTarget = (CSymbolNode_inclusive_or_expr_s*) pSymbol;
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 110 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12897 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 88:
			{
			CSymbolNode_exclusive_or_expr_s* __pTarget = (CSymbolNode_exclusive_or_expr_s*) pSymbol;
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 122 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12910 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 89:
			{
			CSymbolNode_exclusive_or_expr_s* __pTarget = (CSymbolNode_exclusive_or_expr_s*) pSymbol;
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 123 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12923 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 90:
			{
			CSymbolNode_and_expr_s* __pTarget = (CSymbolNode_and_expr_s*) pSymbol;
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 135 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12936 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 91:
			{
			CSymbolNode_and_expr_s* __pTarget = (CSymbolNode_and_expr_s*) pSymbol;
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 136 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12949 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 92:
			{
			CSymbolNode_equality_expr_s* __pTarget = (CSymbolNode_equality_expr_s*) pSymbol;
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 148 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12962 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 93:
			{
			CSymbolNode_equality_expr_s* __pTarget = (CSymbolNode_equality_expr_s*) pSymbol;
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 149 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12975 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 94:
			{
			CSymbolNode_relational_expr_s* __pTarget = (CSymbolNode_relational_expr_s*) pSymbol;
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 161 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12988 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 95:
			{
			CSymbolNode_relational_expr_s* __pTarget = (CSymbolNode_relational_expr_s*) pSymbol;
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 162 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13001 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 96:
			{
			CSymbolNode_shift_expr_s* __pTarget = (CSymbolNode_shift_expr_s*) pSymbol;
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 174 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13014 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 97:
			{
			CSymbolNode_shift_expr_s* __pTarget = (CSymbolNode_shift_expr_s*) pSymbol;
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 175 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13027 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 98:
			{
			CSymbolNode_additive_expr_s* __pTarget = (CSymbolNode_additive_expr_s*) pSymbol;
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 187 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13040 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 99:
			{
			CSymbolNode_additive_expr_s* __pTarget = (CSymbolNode_additive_expr_s*) pSymbol;
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 188 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13053 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 100:
			{
			CSymbolNode_multiplicative_expr_s* __pTarget = (CSymbolNode_multiplicative_expr_s*) pSymbol;
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 200 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13066 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 101:
			{
			CSymbolNode_multiplicative_expr_s* __pTarget = (CSymbolNode_multiplicative_expr_s*) pSymbol;
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 201 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13079 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 102:
			{
			CSymbolNode_at_expr_s* __pTarget = (CSymbolNode_at_expr_s*) pSymbol;
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 213 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13092 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 103:
			{
			CSymbolNode_at_expr_s* __pTarget = (CSymbolNode_at_expr_s*) pSymbol;
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 214 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13105 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 104:
			{
			CSymbolNode_assignment_expr_s* __pTarget = (CSymbolNode_assignment_expr_s*) pSymbol;
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 226 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13118 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 105:
			{
			CSymbolNode_assignment_expr_s* __pTarget = (CSymbolNode_assignment_expr_s*) pSymbol;
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 227 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13131 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 106:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 239 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13144 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 107:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 245 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13157 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 108:
			{
			CSymbolNode_postfix_expr_s* __pTarget = (CSymbolNode_postfix_expr_s*) pSymbol;
			CSymbolNode_unary_expr_s* __pSymbol = (CSymbolNode_unary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 253 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13170 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 109:
			{
			CSymbolNode_unary_operator_expr_s* __pTarget = (CSymbolNode_unary_operator_expr_s*) pSymbol;
			CSymbolNode_unary_expr_s* __pSymbol = (CSymbolNode_unary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 254 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13183 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 110:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 263 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13196 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 111:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 267 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13209 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 112:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 271 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13222 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 113:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 275 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13235 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 114:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 279 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13248 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 115:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 283 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13261 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 116:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 287 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13274 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 117:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 291 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13287 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 118:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 295 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13300 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 119:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 315 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13313 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 120:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 320 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13326 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 121:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 329 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13339 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 122:
			{
			CSymbolNode_primary_expr_s* __pTarget = (CSymbolNode_primary_expr_s*) pSymbol;
			CSymbolNode_postfix_expr_s* __pSymbol = (CSymbolNode_postfix_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 352 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13352 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 123:
			{
			CSymbolNode_postfix_operator_s* __pTarget = (CSymbolNode_postfix_operator_s*) pSymbol;
			CSymbolNode_postfix_expr_s* __pSymbol = (CSymbolNode_postfix_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 353 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13365 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 124:
			{
			CSymbolNode_member_operator_s* __pTarget = (CSymbolNode_member_operator_s*) pSymbol;
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 388 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13378 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 125:
			{
			CSymbolNode_member_operator_s* __pTarget = (CSymbolNode_member_operator_s*) pSymbol;
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 393 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13391 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 126:
			{
			CSymbolNode_literal_s* __pTarget = (CSymbolNode_literal_s*) pSymbol;
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 449 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13404 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 127:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 450 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13417 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 128:
			{
			CSymbolNode_switch_block_stmt* __pTarget = (CSymbolNode_switch_block_stmt*) pSymbol;
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 143 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pStmt = &__pSymbol->m_Local.Stmt;
// #line 13430 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 129:
			{
			CSymbolNode_declaration_specifier* __pTarget = (CSymbolNode_declaration_specifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
			
// #line 16 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
// #line 13443 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 130:
			{
			CSymbolNode_pitcher_modifier* __pTarget = (CSymbolNode_pitcher_modifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 37 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13456 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 131:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
			
// #line 55 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
// #line 13469 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 132:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
			
// #line 275 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pNamespace = __pSymbol->m_Local.pTemplate;
// #line 13482 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 133:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 4 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13495 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 134:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 4 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13508 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 135:
			{
			CSymbolNode_declarator* __pTarget = (CSymbolNode_declarator*) pSymbol;
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 12 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13521 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 136:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 39 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13534 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 137:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 40 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13547 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 138:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 41 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13560 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 139:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 42 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13573 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 140:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 48 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13586 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 141:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 52 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13599 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 142:
			{
			CSymbolNode_array_suffix* __pTarget = (CSymbolNode_array_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 302 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13612 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 143:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 303 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13625 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 144:
			{
			CSymbolNode_bitfield_suffix* __pTarget = (CSymbolNode_bitfield_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 305 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13638 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 145:
			{
			CSymbolNode_function_formal_argument_list* __pTarget = (CSymbolNode_function_formal_argument_list*) pSymbol;
			CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 342 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Local.pFunctionSuffix;
// #line 13651 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 146:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 347 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13664 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 147:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 348 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13677 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 148:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pTarget = (CSymbolNode_function_formal_argument_w_specifier*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 352 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13690 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 149:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pTarget = (CSymbolNode_function_formal_argument_no_specifier*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 353 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13703 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 150:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 373 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13716 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 151:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 374 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13729 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 152:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 375 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13742 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 153:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 396 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13755 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 154:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 397 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13768 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 155:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 398 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13781 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 156:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
			
// #line 56 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
// #line 13794 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 157:
			{
			CSymbolNode_enum_member_list* __pTarget = (CSymbolNode_enum_member_list*) pSymbol;
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 91 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13807 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 158:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 100 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13820 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 159:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 100 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13833 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 160:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
			
// #line 148 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13846 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 161:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
			
// #line 200 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13859 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 162:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
			
// #line 230 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13872 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
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
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_item_declaration_w_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 123 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
	;
// #line 13907 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_item_declaration_no_specifier:
			{
				CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 140 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
		__pSymbol->m_Local.Declarator.SetTypeSpecifier (NULL);
	;
// #line 13923 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_local_declaration:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 40 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		m_pModule->m_ControlFlowMgr.ResetJumpFlag (); // 'stack new' is currently illegal in conditional expression
		m_pModule->m_NamespaceMgr.SetSourcePos (m_CurrentToken.m_Pos);
	;
// #line 13938 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_statement:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 75 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.ResetJumpFlag (); // 'stack new' is currently illegal in conditional expression
		m_pModule->m_NamespaceMgr.SetSourcePos (m_CurrentToken.m_Pos);
	;
// #line 13952 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 9 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13970 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_function_suffix:
			{
				CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 334 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		__pSymbol->m_Local.pFunctionSuffix = __pSymbol->m_Arg.pDeclarator->AddFunctionSuffix ();
		__pSymbol->m_Local.PrevStorageKind = m_StorageKind;
	;
// #line 13988 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
// #line 48 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 14007 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_expression:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
// #line 97 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		m_pModule->m_NamespaceMgr.SetSourcePos (m_CurrentToken.m_Pos);
	;
// #line 14032 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_assignment_operator_expr:
			{
				CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 475 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsReactor ())  // reset on assignment
			m_ReactorBindSiteList.Clear ();
	;
// #line 14071 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_curly_initializer:
			{
				CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 833 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		ASSERT (m_CurlyInitializerTargetValue);

		__pSymbol->m_Local.Initializer.m_TargetValue = m_CurlyInitializerTargetValue;
		__pSymbol->m_Local.Initializer.m_Index = 0;
		__pSymbol->m_Local.Initializer.m_Count = 0;
	;
// #line 14089 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_new_operator_curly_initializer:
			{
				CSymbolNode_new_operator_curly_initializer* __pSymbol = (CSymbolNode_new_operator_curly_initializer*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 643 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, *__pSymbol->m_Arg.pValue, &m_CurlyInitializerTargetValue);
	;
// #line 14107 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_if_stmt:
			{
				CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 108 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.IfStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14160 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_switch_stmt:
			{
				CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 134 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.SwitchStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14173 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_while_stmt:
			{
				CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 167 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.WhileStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14186 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_do_stmt:
			{
				CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 186 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.DoStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14199 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_for_stmt:
			{
				CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 209 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.ForStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14212 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		case ESymbol_reactor_onchange_stmt:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 332 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_ReactorBindSiteList.Clear (); // clear bindsite list before processing expression list
	;
// #line 14233 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_reactor_stmt_0:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 81 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
				
		m_ReactorBindSiteCount = 0;
	;
// #line 14262 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		case ESymbol_named_type_block:
			{
				CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 239 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pNamespace);
	;
// #line 14283 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_full_declarator:
			{
				CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 9 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		return PreDeclare ();
	;
// #line 14296 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_declarator:
			{
				CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
				_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 36 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		(*__pAstNode).m_Declarator.SetTypeSpecifier (__pSymbol->m_Arg.pTypeSpecifier);
	;
// #line 14309 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_operator_name:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 111 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		(*__pAstNode).m_UnOpKind = EUnOp_Undefined;
		(*__pAstNode).m_BinOpKind = EBinOp_Undefined;
	;
// #line 14324 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_function_formal_argument_w_specifier:
			{
				CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 366 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = EStorage_Undefined;
	;
// #line 14341 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_function_formal_argument_no_specifier:
			{
				CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 392 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = EStorage_Undefined;
		__pSymbol->m_Local.Declarator.SetTypeSpecifier (NULL);
	;
// #line 14355 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_statement:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 80 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_OperatorMgr.NullifyTmpStackGcRootList ();
	;
// #line 14404 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 13 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 14422 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_declaration_terminator:
			{
				CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 173 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		if (m_pAttributeBlock)
		{
			err::SetFormatStringError ("unused attribute block in declaration");
			return false;
		}

		m_pLastDeclaredItem = NULL;
	;
// #line 14442 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		case ESymbol_function_suffix:
			{
				CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 339 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = __pSymbol->m_Local.PrevStorageKind;
	;
// #line 14457 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
// #line 52 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 14476 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_unary_operator_expr:
			{
				CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 552 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 14532 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		case ESymbol_primary_expr:
			{
				CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 725 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 14553 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_postfix_operator:
			{
				CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 667 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 14567 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_unary_operator_expr_s:
			{
				CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 259 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteCount++;
	;
// #line 14604 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_primary_expr_s:
			{
				CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 413 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteCount++;
	;
// #line 14619 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_postfix_operator_s:
			{
				CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 358 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteCount++;
	;
// #line 14633 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_named_type_block:
			{
				CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 243 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		m_pModule->m_NamespaceMgr.CloseNamespace ();
	;
// #line 14693 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_enum_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
// #line 38 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		if ((*__pAstNode).m_pType->GetName ().IsEmpty () || ((*__pAstNode).m_pType->GetFlags () & EEnumTypeFlag_Exposed))
			return m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->ExposeEnumConsts ((*__pAstNode).m_pType);
	;
// #line 14717 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_struct_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
// #line 130 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		if ((*__pAstNode).m_pType->GetName ().IsEmpty ())
			return DeclareUnnamedStructOrUnion ((*__pAstNode).m_pType);
	;
// #line 14731 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_union_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
// #line 189 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		if ((*__pAstNode).m_pType->GetName ().IsEmpty ())
			return DeclareUnnamedStructOrUnion ((*__pAstNode).m_pType);
	;
// #line 14745 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 1713;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 163;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 1715;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 3:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 163;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 4:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 167;
				pTransition->m_ResolverIndex = 166;
				pTransition->m_ResolverElseIndex = 165;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 5:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 167;
				pTransition->m_ResolverIndex = 166;
				pTransition->m_ResolverElseIndex = 165;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 6:
			
			switch (LookaheadToken)
			{
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 1719;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 7:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 163;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 8:
			
			switch (LookaheadToken)
			{
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 1721;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 9:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 163;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 10:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 167;
				pTransition->m_ResolverIndex = 166;
				pTransition->m_ResolverElseIndex = 165;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 11:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 167;
				pTransition->m_ResolverIndex = 166;
				pTransition->m_ResolverElseIndex = 165;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 12:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 167;
				pTransition->m_ResolverIndex = 994;
				pTransition->m_ResolverElseIndex = 1725;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 13:
			
			pTransition->m_ProductionIndex = 165;
			pTransition->m_ResolverIndex = 314;
			pTransition->m_ResolverElseIndex = 170;
				
			return ELaDfaResult_Resolver;
			
		
		case 14:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1727;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 15:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 16:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1729;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 17:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 18:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 167;
				pTransition->m_ResolverIndex = 994;
				pTransition->m_ResolverElseIndex = 1731;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 19:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 20:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1733;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 21:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 22:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1735;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 23:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 24:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1737;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 25:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 26:
			
			switch (LookaheadToken)
			{
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 1739;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 27:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 28:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1741;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 29:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 30:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1743;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 31:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 32:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1745;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 33:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 34:
			
			switch (LookaheadToken)
			{
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1747;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 35:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 36:
			
			switch (LookaheadToken)
			{
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1749;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 37:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 170;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 38:
			
			switch (LookaheadToken)
			{
			
			case '*':
					
				pTransition->m_ProductionIndex = 167;
				pTransition->m_ResolverIndex = 994;
				pTransition->m_ResolverElseIndex = 170;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 39:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1752;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 40:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 41:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1754;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 42:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 43:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1756;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 44:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 45:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1758;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 46:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 47:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1760;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 48:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 49:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1762;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 50:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 51:
			
			switch (LookaheadToken)
			{
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 1764;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 52:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 53:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1766;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 54:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 55:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1768;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 56:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 57:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1770;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 58:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 59:
			
			switch (LookaheadToken)
			{
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1772;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 60:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 61:
			
			switch (LookaheadToken)
			{
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1774;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 62:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 63:
			
			switch (LookaheadToken)
			{
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 1776;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 64:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 285;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_RetVal:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_CountOf:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_OffsetOf:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_BindingOf:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_HexLiteral:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_FmtLiteral:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 275;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 65:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 1778;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 66:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 67:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 522;
				pTransition->m_ResolverIndex = 314;
				pTransition->m_ResolverElseIndex = 523;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 68:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 609;
				pTransition->m_ResolverIndex = 232;
				pTransition->m_ResolverElseIndex = 608;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 69:
			
			switch (LookaheadToken)
			{
			
			case '=':
					
				pTransition->m_ProductionIndex = 1782;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 70:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_RetVal:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_CountOf:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_OffsetOf:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_BindingOf:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_HexLiteral:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_FmtLiteral:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 594;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 71:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 624;
				pTransition->m_ResolverIndex = 236;
				pTransition->m_ResolverElseIndex = 623;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 72:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 821;
				pTransition->m_ResolverIndex = 852;
				pTransition->m_ResolverElseIndex = 822;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 73:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 664;
				pTransition->m_ResolverIndex = 663;
				pTransition->m_ResolverElseIndex = 666;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 74:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 705;
				pTransition->m_ResolverIndex = 232;
				pTransition->m_ResolverElseIndex = 704;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 75:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 724;
				pTransition->m_ResolverIndex = 236;
				pTransition->m_ResolverElseIndex = 723;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 76:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 816;
				pTransition->m_ResolverIndex = 852;
				pTransition->m_ResolverElseIndex = 817;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 77:
			
			switch (LookaheadToken)
			{
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 1790;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 78:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 818;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 817;
				return ELaDfaResult_Production;
					
			}
			
		
		case 79:
			
			switch (LookaheadToken)
			{
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 1792;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 80:
			
			switch (LookaheadToken)
			{
			
			case '<':
					
				pTransition->m_ProductionIndex = 840;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 839;
				return ELaDfaResult_Production;
					
			}
			
		
		case 81:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1794;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 82:
			
			switch (LookaheadToken)
			{
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 83:
			
			switch (LookaheadToken)
			{
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 1796;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 84:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 899;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '|':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '^':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '<':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '>':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '/':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '%':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '@':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_LogOr:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_LogAnd:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Eq:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Ne:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Le:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Ge:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Shl:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Shr:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_RefAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_AddAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_SubAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_MulAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_DivAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_ModAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_ShlAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_ShrAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_AndAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_XorAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_OrAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			case EToken_AtAssign:
					
				pTransition->m_ProductionIndex = 901;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 85:
			
			switch (LookaheadToken)
			{
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 1798;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 86:
			
			switch (LookaheadToken)
			{
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 907;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 908;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 87:
			
			switch (LookaheadToken)
			{
			
			case '[':
					
				pTransition->m_ProductionIndex = 1800;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 88:
			
			switch (LookaheadToken)
			{
			
			case '\01':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '#':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '(':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case ';':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '}':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case ',':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '.':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case ']':
					
				pTransition->m_ProductionIndex = 949;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '?':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '|':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '^':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '<':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '>':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '/':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '%':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '@':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Pack:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Namespace:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Extend:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Using:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Friend:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Catch:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Finally:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_LogOr:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_LogAnd:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Eq:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Ne:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Le:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Ge:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Shl:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Shr:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_RefAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_AddAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_SubAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_MulAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_DivAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_ModAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_ShlAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_ShrAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_AndAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_XorAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_OrAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_RetVal:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 948;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_CountOf:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_OffsetOf:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_BindingOf:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_HexLiteral:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_FmtLiteral:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_FmtSpecifier:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_BaseType:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_If:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Else:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Switch:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Case:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Default:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_While:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Do:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_For:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Break:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Continue:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Return:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChanged:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_AtAssign:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_FEnum:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_CEnum:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Opaque:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 89:
			
			switch (LookaheadToken)
			{
			
			case ':':
					
				pTransition->m_ProductionIndex = 518;
				pTransition->m_ResolverIndex = 1354;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 90:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 756;
				pTransition->m_ResolverIndex = 273;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 91:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				pTransition->m_ResolverIndex = 314;
				pTransition->m_ResolverElseIndex = 774;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 92:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1805;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 93:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 94:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1807;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 95:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 96:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1809;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 97:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 98:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1811;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 99:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 100:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1813;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 101:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 102:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1815;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 103:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 104:
			
			switch (LookaheadToken)
			{
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 1817;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 105:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 106:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1819;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 107:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 108:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1821;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 109:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 110:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1823;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 111:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 112:
			
			switch (LookaheadToken)
			{
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1825;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 113:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 114:
			
			switch (LookaheadToken)
			{
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1827;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 115:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 774;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DConst:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Array:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_DEvent:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 116:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 813;
				pTransition->m_ResolverIndex = 314;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 117:
			
			switch (LookaheadToken)
			{
			
			case EToken_OnChanged:
					
				pTransition->m_ProductionIndex = 289;
				pTransition->m_ResolverIndex = 1226;
				pTransition->m_ResolverElseIndex = 290;
						
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


	} // namespace jnc {
	} // namespace axl {


#pragma warning (default: 4065)
