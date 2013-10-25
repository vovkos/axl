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
	ESymbol_curly_initializer_item = 88, 
	ESymbol_expression_save_value_s = 89, 
	ESymbol_conditional_expr_s = 90, 
	ESymbol_expression_or_empty_s = 91, 
	ESymbol_expression_list_s = 92, 
	ESymbol_expression_or_empty_list_s = 93, 
	ESymbol_logical_or_expr_s = 94, 
	ESymbol_logical_and_expr_s = 95, 
	ESymbol_inclusive_or_expr_s = 96, 
	ESymbol_exclusive_or_expr_s = 97, 
	ESymbol_and_expr_s = 98, 
	ESymbol_equality_expr_s = 99, 
	ESymbol_relational_expr_s = 100, 
	ESymbol_shift_expr_s = 101, 
	ESymbol_additive_expr_s = 102, 
	ESymbol_multiplicative_expr_s = 103, 
	ESymbol_at_expr_s = 104, 
	ESymbol_assignment_expr_s = 105, 
	ESymbol_unary_expr_s = 106, 
	ESymbol_postfix_expr_s = 107, 
	ESymbol_unary_operator_expr_s = 108, 
	ESymbol_new_operator_sype_s = 109, 
	ESymbol_primary_expr_s = 110, 
	ESymbol_postfix_operator_s = 111, 
	ESymbol_member_operator_s = 112, 
	ESymbol_literal_s = 113, 
	ESymbol_compound_stmt = 114, 
	ESymbol_constructor_compound_stmt = 115, 
	ESymbol_btm_construct_stmt = 116, 
	ESymbol_btm_construct_stmt_rslv = 117, 
	ESymbol_btm_construct_name = 118, 
	ESymbol_expression_stmt = 119, 
	ESymbol_if_stmt = 120, 
	ESymbol_switch_stmt = 121, 
	ESymbol_while_stmt = 122, 
	ESymbol_do_stmt = 123, 
	ESymbol_for_stmt = 124, 
	ESymbol_break_stmt = 125, 
	ESymbol_continue_stmt = 126, 
	ESymbol_return_stmt = 127, 
	ESymbol_once_stmt = 128, 
	ESymbol_try_stmt = 129, 
	ESymbol_switch_block_stmt = 130, 
	ESymbol_reactor_body = 131, 
	ESymbol_reactor_stmt = 132, 
	ESymbol_reactor_onchange_stmt = 133, 
	ESymbol_reactor_expression_stmt = 134, 
	ESymbol_statement_0 = 135, 
	ESymbol_compound_stmt_0 = 136, 
	ESymbol_expression_stmt_0 = 137, 
	ESymbol_if_stmt_0 = 138, 
	ESymbol_switch_stmt_0 = 139, 
	ESymbol_while_stmt_0 = 140, 
	ESymbol_do_stmt_0 = 141, 
	ESymbol_for_stmt_0 = 142, 
	ESymbol_break_stmt_0 = 143, 
	ESymbol_continue_stmt_0 = 144, 
	ESymbol_return_stmt_0 = 145, 
	ESymbol_once_stmt_0 = 146, 
	ESymbol_expression_0 = 147, 
	ESymbol_primary_expr_0 = 148, 
	ESymbol_reactor_body_0 = 149, 
	ESymbol_reactor_stmt_0 = 150, 
	ESymbol_reactor_onchange_stmt_0 = 151, 
	ESymbol_reactor_expression_stmt_0 = 152, 
	ESymbol_declaration_specifier = 153, 
	ESymbol_type_specifier = 154, 
	ESymbol_type_modifier = 155, 
	ESymbol_pitcher_modifier = 156, 
	ESymbol_property_template_specifier = 157, 
	ESymbol_qualified_type_name_rslv = 158, 
	ESymbol_named_type_block = 159, 
	ESymbol_full_declarator = 160, 
	ESymbol_declarator = 161, 
	ESymbol_declarator_qualifier = 162, 
	ESymbol_operator_name = 163, 
	ESymbol_array_suffix = 164, 
	ESymbol_bitfield_suffix = 165, 
	ESymbol_function_formal_argument_list = 166, 
	ESymbol_function_formal_argument = 167, 
	ESymbol_function_formal_argument_w_specifier = 168, 
	ESymbol_function_formal_argument_no_specifier = 169, 
	ESymbol_function_formal_argument_ellipsis = 170, 
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
		StartPragmaSymbol  = 332,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 155,
		SymbolCount        = 333,
		SequenceCount      = 506,
		ActionCount        = 391,
		ArgumentCount      = 163,
		BeaconCount        = 178,
		LaDfaCount         = 120,

		TotalCount         = 1846,

		NamedSymbolCount   = 181,

		TokenFirst         = 0,
		TokenEnd           = 155,
		SymbolFirst        = 155,
		NamedSymbolEnd     = 336,
		SymbolEnd          = 488,
		SequenceFirst      = 488,
		SequenceEnd        = 994,
		ActionFirst        = 994,
		ActionEnd          = 1385,
		ArgumentFirst      = 1385,
		ArgumentEnd        = 1548,
		BeaconFirst        = 1548,
		BeaconEnd          = 1726,
		LaDfaFirst         = 1726,
		LaDfaEnd           = 1846,
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
// #line 625 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
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
// #line 129 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EAccess m_AccessKind;
;
// #line 655 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls33: public CAstNode
	{
	public:
// #line 146 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
// #line 665 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls34: public CAstNode
	{
	public:
// #line 243 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
// #line 675 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls35: public CAstNode
	{
	public:
// #line 261 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CPropertyType* m_pType;
;
// #line 685 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls36: public CAstNode
	{
	public:
// #line 283 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
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
// #line 149 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_PackFactor;
;
// #line 767 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls44: public CAstNode
	{
	public:
// #line 160 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_Value;
;
// #line 777 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls45: public CAstNode
	{
	public:
// #line 179 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CUnionType* m_pType;
;
// #line 787 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls46: public CAstNode
	{
	public:
// #line 198 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
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
// #line 426 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
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
// #line 828 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
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
// #line 663 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
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
// #line 657 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
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
// #line 644 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
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
// #line 726 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
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
// #line 668 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
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
// #line 708 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
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
// #line 792 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1795 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 794 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
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
		
	
	class CSymbolNode_curly_initializer_item: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 850 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			TCurlyInitializer* pInitializer;
// #line 1824 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 852 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue PrevCurlyInitializerTargetValue;
	;
// #line 1833 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 1854 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 79 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue TrueValue;
		CValue FalseValue;
	;
// #line 1864 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 1887 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 93 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1896 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 1916 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 106 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1925 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 1945 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1954 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 1974 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 132 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1983 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2003 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 145 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2012 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2032 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 158 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2041 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2061 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 171 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2070 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2090 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 184 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2099 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 197 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2128 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2148 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2157 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2177 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 223 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2186 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2206 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 236 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2215 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2235 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2255 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2275 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2298 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2320 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2342 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2362 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 455 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		TLiteral Literal;
	;
// #line 2371 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 100 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TIfStmt Stmt;
	;
// #line 2399 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 126 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TSwitchStmt Stmt;
	;
// #line 2423 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 159 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TWhileStmt Stmt;
	;
// #line 2447 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 178 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TDoStmt Stmt;
	;
// #line 2471 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 201 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TForStmt Stmt;
	;
// #line 2495 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 263 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TOnceStmt Stmt;
	;
// #line 2522 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 145 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			TSwitchStmt* pStmt;
// #line 2543 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2585 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 371 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2607 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 266 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
		CPropertyTemplate* pTemplate;
	;
// #line 2629 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 223 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CNamespace* pNamespace;
// #line 2650 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2674 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2696 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2718 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2739 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 417 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2759 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2779 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 2799 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 357 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2819 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 359 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclarator Declarator;
	;
// #line 2828 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 383 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2850 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 385 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclarator Declarator;
	;
// #line 2859 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_function_formal_argument_no_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_formal_argument_no_specifier;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_function_formal_argument_ellipsis: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 406 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2881 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_function_formal_argument_ellipsis ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_formal_argument_ellipsis;
			
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

	CType*
	FindType (const CQualifiedName& Name);

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
		CNamespace* pNamespace,
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
	DeclareReactor (
		CDeclarator* pDeclarator,
		CReactorClassType* pType,
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
		size_t PackFactor
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
	FinalizeReactorOnChangeClause ();

	bool
	FinalizeReactorOnChangeStmt ()
	{
		return m_pModule->m_FunctionMgr.Epilogue (m_LastMatchedToken.m_Pos);
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
		CValue* pValue
		);

	bool
	LookupIdentifierType (
		const rtl::CString& Name,
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
			0, -1, -1, -1, -1, 488, -1, -1, -1, -1, -1, 488, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 488, -1, -1, -1, -1, -1, -1, 488, 488, 488, 488, 488, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 488, 488, -1, 488, 488, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, -1, -1, -1, 488, 488, 488, 488, 488, 488, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, 165, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 337, -1, -1, -1, -1, -1, -1, 159, 160, 161, 162, 1730, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 337, 165, -1, 337, 337, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 165, 165, 1731, 165, 165, 165, 165, 165, 165, 165, 165, 165, 1726, 1728, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 337, 337, 337, -1, -1, -1, 164, 164, 164, 164, 164, 164, 
			-1, -1, 489, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 987, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 988, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 494, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 496, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 498, 498, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 970, 970, 970, 971, 972, 973, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 508, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 508, -1, -1, 508, 508, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 508, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 508, 508, 508, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 505, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 505, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 505, -1, -1, 505, 505, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 505, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 505, 505, 505, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, 165, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 338, -1, -1, -1, -1, -1, -1, -1, -1, -1, 162, 1736, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 338, 165, -1, 338, 338, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 165, 165, 1737, 165, 165, 165, 165, 165, 165, 165, 165, 165, 1732, 1734, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 338, 338, 338, -1, -1, -1, 164, 164, 164, 164, 164, 164, 
			-1, -1, -1, 170, -1, 170, 170, -1, -1, -1, -1, 165, -1, -1, -1, -1, -1, 170, -1, -1, 170, 170, 1764, -1, -1, -1, 170, 170, -1, -1, -1, 161, -1, 1738, 171, 172, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 170, 170, 170, 170, 170, 339, 165, -1, 339, 339, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, -1, -1, 170, -1, 170, -1, -1, 170, 170, 170, 170, 170, 170, 170, 170, -1, 1740, 1742, 1744, 1746, 1748, 1750, 1752, 1754, 1756, 1758, 1760, 1762, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 339, 339, 339, -1, -1, -1, 164, 164, 164, 164, 164, 164, 
			-1, -1, -1, 274, -1, 5, 269, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 274, -1, -1, 274, 274, 274, -1, -1, -1, 274, 274, -1, -1, -1, -1, -1, 274, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 274, 274, 1789, 274, 274, -1, -1, -1, -1, -1, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274, 274, -1, -1, 275, -1, 276, -1, -1, 277, 278, 279, 280, 281, 282, 282, 283, -1, 1765, 1767, 1769, 1771, 1773, 1775, 1777, 1779, 1781, 1783, 1785, 1787, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 783, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 784, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 499, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 835, 836, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 812, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 812, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 812, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, 812, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, -1, -1, 880, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, 880, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 509, 510, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 942, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 889, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 889, -1, -1, 889, 889, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 889, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 889, 889, 889, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1791, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 945, -1, -1, -1, -1, -1, 465, -1, 944, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 969, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 511, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 512, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 517, 517, 517, 517, 517, 514, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 515, 516, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 518, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 518, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 525, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 525, -1, -1, 525, 525, 525, -1, -1, -1, 525, 525, -1, -1, -1, -1, -1, 1793, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 525, 525, 525, 525, 525, -1, 524, -1, -1, -1, 525, 525, 525, 525, 525, 525, 525, 525, 525, 525, 525, 525, 525, 525, 525, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 525, 525, 525, 525, 525, 525, 525, 525, 525, 525, 525, 525, -1, -1, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, -1, -1, 670, 670, 670, -1, -1, -1, 670, 670, -1, -1, -1, -1, -1, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 526, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 528, -1, -1, -1, 528, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 528, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 530, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 
			-1, 539, 539, 537, 539, 539, 533, 539, 539, 539, 539, 535, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 
			-1, -1, -1, 541, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 544, 544, 543, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 
			-1, -1, -1, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 545, -1, -1, 545, 545, 545, -1, -1, -1, 545, 545, -1, -1, -1, -1, -1, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 545, 545, 545, 545, 545, -1, -1, -1, -1, -1, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, -1, -1, 547, 547, 547, -1, -1, -1, 547, 547, -1, -1, -1, -1, -1, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			546, -1, -1, 546, -1, -1, -1, -1, 546, -1, -1, -1, -1, -1, -1, -1, -1, 546, -1, -1, 546, 546, 546, -1, -1, -1, 546, 546, -1, -1, -1, -1, -1, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, 546, 546, 546, 546, -1, -1, -1, -1, -1, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 551, 0, 0, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, 551, -1, -1, 551, 551, 551, -1, -1, -1, 551, 551, -1, -1, -1, -1, -1, 551, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 551, 551, 551, 551, 551, -1, -1, -1, -1, -1, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 557, -1, -1, 557, 557, 557, -1, -1, -1, 557, 557, -1, -1, -1, -1, -1, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 557, 557, 557, 557, 557, -1, -1, -1, -1, -1, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 548, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 548, -1, -1, 548, 548, 548, -1, -1, -1, 548, 548, -1, -1, -1, -1, -1, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, 548, 548, 548, 548, -1, -1, -1, -1, -1, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 549, -1, -1, 549, 549, 549, -1, -1, -1, 549, 549, -1, -1, -1, -1, -1, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 549, 549, 549, 549, 549, -1, -1, -1, -1, -1, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, -1, -1, 555, 555, 555, -1, -1, -1, 555, 555, -1, -1, -1, -1, -1, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 556, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 556, -1, -1, 556, 556, 556, -1, -1, -1, 556, 556, -1, -1, -1, -1, -1, 556, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 556, 556, 556, 556, 556, -1, -1, -1, -1, -1, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 559, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 559, -1, -1, 559, 559, 559, -1, -1, -1, 559, 559, -1, -1, -1, -1, -1, 559, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 559, 559, 559, 559, 559, -1, -1, -1, -1, -1, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, 559, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 561, -1, -1, 561, 561, 561, -1, -1, -1, 561, 561, -1, -1, -1, -1, -1, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 561, 561, 561, 561, 561, -1, -1, -1, -1, -1, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 563, -1, -1, 563, 563, 563, -1, -1, -1, 563, 563, -1, -1, -1, -1, -1, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 563, 563, 563, 563, 563, -1, -1, -1, -1, -1, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 565, -1, -1, 565, 565, 565, -1, -1, -1, 565, 565, -1, -1, -1, -1, -1, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 565, 565, 565, 565, 565, -1, -1, -1, -1, -1, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 567, -1, -1, 567, 567, 567, -1, -1, -1, 567, 567, -1, -1, -1, -1, -1, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 567, 567, 567, 567, 567, -1, -1, -1, -1, -1, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 569, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 569, -1, -1, 569, 569, 569, -1, -1, -1, 569, 569, -1, -1, -1, -1, -1, 569, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 569, 569, 569, 569, 569, -1, -1, -1, -1, -1, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 573, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 573, -1, -1, 573, 573, 573, -1, -1, -1, 573, 573, -1, -1, -1, -1, -1, 573, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 573, 573, 573, 573, 573, -1, -1, -1, -1, -1, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, 573, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 571, 572, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 579, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 579, -1, -1, 579, 579, 579, -1, -1, -1, 579, 579, -1, -1, -1, -1, -1, 579, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 579, 579, 579, 579, 579, -1, -1, -1, -1, -1, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579, 579, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 575, 576, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 577, 578, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 583, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 583, -1, -1, 583, 583, 583, -1, -1, -1, 583, 583, -1, -1, -1, -1, -1, 583, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 583, 583, 583, 583, 583, -1, -1, -1, -1, -1, 583, 583, 583, 583, 583, 583, 583, 583, 583, 583, 583, 583, 583, 583, 583, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 583, 583, 583, 583, 583, 583, 583, 583, 583, 583, 583, 583, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 581, 582, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 587, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 587, -1, -1, 587, 587, 587, -1, -1, -1, 587, 587, -1, -1, -1, -1, -1, 587, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 587, 587, 587, 587, 587, -1, -1, -1, -1, -1, 587, 587, 587, 587, 587, 587, 587, 587, 587, 587, 587, 587, 587, 587, 587, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 587, 587, 587, 587, 587, 587, 587, 587, 587, 587, 587, 587, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 585, 586, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 592, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 592, -1, -1, 592, 592, 592, -1, -1, -1, 592, 592, -1, -1, -1, -1, -1, 592, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 592, 592, 592, 592, 592, -1, -1, -1, -1, -1, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, 592, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 589, 590, 591, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 594, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 594, -1, -1, 594, 594, 594, -1, -1, -1, 594, 594, -1, -1, -1, -1, -1, 594, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 594, 594, 594, 594, 594, -1, -1, -1, -1, -1, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1794, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 611, -1, -1, 611, 611, 611, -1, -1, -1, 611, 611, -1, -1, -1, -1, -1, 610, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 611, 611, 611, 611, 611, -1, -1, -1, -1, -1, 610, 610, 610, 610, 610, 610, 610, 610, 610, 610, 610, 610, 610, 610, 610, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 611, 611, 611, 611, 611, 611, 611, 611, 611, 611, 611, 611, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1795, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, 596, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 598, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 662, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 629, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 629, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 629, 629, 629, 629, 629, 629, 629, 629, 629, 629, 629, 629, 629, 629, 629, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 378, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 615, -1, -1, 612, 613, 616, -1, -1, -1, 614, 617, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 618, 619, 620, 622, 624, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 622, 622, 622, 622, 622, 622, 622, 622, 622, 622, 622, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 628, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 834, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1797, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 625, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 229, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 627, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 857, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 821, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 822, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 821, 821, 821, 821, 821, 821, 821, 821, 821, 821, 821, 821, 821, 821, 1798, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, 822, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 644, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654, 655, 656, 643, 643, 643, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 631, -1, -1, -1, -1, -1, -1, 637, 633, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 632, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 634, 635, -1, -1, -1, -1, -1, 638, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 639, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 640, 641, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 660, 660, 660, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 666, -1, -1, 667, 668, 668, -1, -1, -1, -1, -1, -1, -1, -1, 666, -1, -1, 666, 666, 666, -1, -1, -1, 666, 666, -1, -1, -1, -1, -1, 1799, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 666, 666, 666, 666, 666, -1, -1, -1, -1, -1, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 669, -1, -1, 669, 669, 669, -1, -1, -1, 669, 669, -1, -1, -1, -1, -1, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 669, 669, 669, 669, 669, -1, -1, -1, -1, -1, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 678, -1, -1, 678, 678, 678, -1, -1, -1, 678, 678, -1, -1, -1, -1, -1, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 678, 678, 678, 678, 678, -1, -1, -1, -1, -1, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 671, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 671, -1, -1, 671, 671, 671, -1, -1, -1, 671, 671, -1, -1, -1, -1, -1, 671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 671, 671, 671, 671, 671, -1, -1, -1, -1, -1, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, 671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, -1, -1, 672, 672, 672, -1, -1, -1, 672, 672, -1, -1, -1, -1, -1, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 674, 0, -1, -1, -1, 677, -1, -1, -1, -1, -1, -1, -1, -1, 674, -1, -1, 674, 674, 674, -1, -1, -1, 674, 674, -1, -1, -1, -1, -1, 674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 674, 674, 674, 674, 674, -1, -1, -1, -1, -1, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, 674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 680, -1, -1, 680, 680, 680, -1, -1, -1, 680, 680, -1, -1, -1, -1, -1, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 680, 680, 680, 680, 680, -1, -1, -1, -1, -1, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, -1, -1, 682, 682, 682, -1, -1, -1, 682, 682, -1, -1, -1, -1, -1, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, 682, 682, 682, 682, -1, -1, -1, -1, -1, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 684, -1, -1, 684, 684, 684, -1, -1, -1, 684, 684, -1, -1, -1, -1, -1, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 684, 684, 684, 684, 684, -1, -1, -1, -1, -1, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, -1, -1, 686, 686, 686, -1, -1, -1, 686, 686, -1, -1, -1, -1, -1, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, 686, 686, 686, 686, -1, -1, -1, -1, -1, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 688, -1, -1, 688, 688, 688, -1, -1, -1, 688, 688, -1, -1, -1, -1, -1, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 688, 688, 688, 688, 688, -1, -1, -1, -1, -1, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 690, -1, -1, 690, 690, 690, -1, -1, -1, 690, 690, -1, -1, -1, -1, -1, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 690, 690, 690, 690, 690, -1, -1, -1, -1, -1, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 692, -1, -1, 692, 692, 692, -1, -1, -1, 692, 692, -1, -1, -1, -1, -1, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 692, 692, 692, 692, 692, -1, -1, -1, -1, -1, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, -1, -1, 694, 694, 694, -1, -1, -1, 694, 694, -1, -1, -1, -1, -1, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, 694, 694, 694, 694, -1, -1, -1, -1, -1, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, -1, -1, 696, 696, 696, -1, -1, -1, 696, 696, -1, -1, -1, -1, -1, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, 696, 696, 696, 696, -1, -1, -1, -1, -1, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, -1, -1, 698, 698, 698, -1, -1, -1, 698, 698, -1, -1, -1, -1, -1, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, 698, 698, 698, 698, -1, -1, -1, -1, -1, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 700, -1, -1, 700, 700, 700, -1, -1, -1, 700, 700, -1, -1, -1, -1, -1, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 700, 700, 700, 700, 700, -1, -1, -1, -1, -1, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 702, -1, -1, 702, 702, 702, -1, -1, -1, 702, 702, -1, -1, -1, -1, -1, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 702, 702, 702, 702, 702, -1, -1, -1, -1, -1, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1800, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 705, -1, -1, 705, 705, 705, -1, -1, -1, 705, 705, -1, -1, -1, -1, -1, 704, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 705, 705, 705, 705, 705, -1, -1, -1, -1, -1, 704, 704, 704, 704, 704, 704, 704, 705, 705, 705, 705, 705, 704, 704, 704, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 725, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 725, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 725, 725, 725, 725, 725, 725, 725, -1, -1, -1, -1, -1, 725, 725, 725, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 410, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 709, -1, -1, 706, 707, 710, -1, -1, -1, 708, 711, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 712, 713, 714, 721, 722, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 715, 716, 717, 718, 719, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1801, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 723, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, 723, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 747, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 738, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 739, 740, 741, 742, 743, 744, 745, -1, -1, -1, -1, -1, 746, 746, 746, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 727, -1, -1, -1, -1, -1, -1, 733, 729, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 728, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 730, 731, -1, -1, -1, -1, -1, 734, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 735, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 736, 737, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 751, 751, 751, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 753, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 755, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 422, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 759, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 760, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 762, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 764, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 764, -1, -1, 764, 764, 764, -1, -1, -1, 764, 764, -1, -1, -1, -1, -1, 764, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 764, 764, 764, 764, 764, -1, -1, -1, -1, -1, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, 764, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 765, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 767, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 771, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 772, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 773, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 777, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 778, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 779, 779, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 780, -1, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 781, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 169, -1, 169, 169, -1, -1, -1, -1, 169, -1, -1, -1, -1, -1, 169, -1, -1, 169, 169, 169, -1, -1, -1, 169, 169, -1, -1, -1, 169, -1, 169, 169, 169, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 169, 169, 169, 169, 169, 169, 169, -1, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, -1, -1, 169, -1, 169, 769, 770, 169, 169, 169, 169, 169, 169, 169, 169, -1, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, -1, -1, -1, 169, 169, 169, 169, 169, 169, 
			-1, -1, -1, -1, -1, -1, 786, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 289, 289, 289, 289, 5, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 1845, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 289, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 787, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 
			-1, 292, 292, 292, 292, 5, 291, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 293, 292, 294, 292, 292, 295, 296, 297, 298, 299, 300, 300, 301, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 
			-1, -1, -1, -1, -1, -1, 790, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 798, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 800, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 801, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 802, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 803, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 804, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 805, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 806, 806, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 807, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 792, 
			-1, 1, 1, 797, 1, 1, 1, 1, 1, 1, 1, 795, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, -1, -1, -1, -1, -1, 809, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 307, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 307, -1, -1, 307, 307, 307, -1, -1, -1, 307, 307, -1, -1, -1, -1, -1, 307, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 307, 307, 307, 307, 307, -1, -1, -1, -1, -1, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 450, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, 307, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 810, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 811, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 811, -1, -1, 811, 811, 811, -1, -1, -1, 811, 811, -1, -1, -1, -1, -1, 811, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 811, 811, 811, 811, 811, -1, -1, -1, -1, -1, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 193, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 816, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 817, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 814, 814, 814, 814, 814, 814, 814, 814, 814, 814, 814, 814, 815, 815, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 1802, 817, 817, 817, 817, 817, 817, 817, 1803, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 454, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 837, 838, 1805, 841, 842, 843, 844, 845, 846, 847, 848, 849, 850, 851, 453, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 864, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 871, 859, 860, 861, 862, 863, 865, 866, 867, 868, 869, 870, 872, 873, 874, 875, 876, 877, 878, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 879, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 858, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 855, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 335, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, -1, -1, 882, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, 882, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, 886, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, 886, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 891, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 895, -1, -1, 892, 893, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1807, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 894, 896, 1809, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 940, -1, 925, -1, 918, 917, 916, 921, 923, 909, 910, 911, 912, 913, 941, 902, 903, -1, -1, -1, -1, -1, -1, -1, -1, 939, 938, 919, 920, 922, 924, 914, 915, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 904, 905, -1, -1, -1, -1, -1, 908, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1811, 937, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1813, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 968, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, 952, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 955, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 1815, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, -1, -1, -1, -1, -1, 956, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 957, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 957, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 957, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, 957, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 962, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 967, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 967, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 974, 974, 974, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 984, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 989, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 990, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 976, 977, 978, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 979, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 980, 980, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 980, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 983, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 986, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 993, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, -1, 488, -1, -1, -1, -1, -1, 488, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 488, -1, -1, -1, -1, -1, -1, 488, 488, 488, 488, 488, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 488, 488, -1, 488, 488, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, 488, -1, -1, -1, 488, 488, 488, 488, 488, 488, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 167, 167, 167, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 491, -1, 0, -1, -1, -1, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, -1, -1, -1, -1, -1, -1, 491, 491, 491, 491, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, 491, -1, 491, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, 491, -1, -1, -1, 491, 491, 491, 491, 491, 491, 
			-1, -1, -1, -1, -1, 493, -1, 0, -1, -1, -1, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, 493, -1, 493, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, -1, -1, -1, 493, 493, 493, 493, 493, 493, 
			-1, -1, -1, -1, -1, 0, -1, -1, 495, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 497, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 501, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, 502, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, 502, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 506, -1, 0, 0, -1, -1, 506, -1, 506, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 507, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 513, 513, 513, 513, 513, 513, 0, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 519, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 520, 0, 0, 0, 0, 0, 1817, 0, 520, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, 523, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 527, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, 529, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 532, 532, 532, 0, 0, 532, 0, 0, 532, 532, 532, 0, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 
			-1, 534, 534, 534, 534, 534, 534, 0, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 
			-1, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 0, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 
			-1, 538, 538, 538, 0, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 
			-1, 540, 540, 540, 0, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 
			-1, 542, 542, 542, 0, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 
			-1, -1, -1, -1, 0, -1, -1, -1, 550, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 552, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 558, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 560, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 562, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 564, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 566, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 568, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 570, 570, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 574, 574, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 574, 574, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 580, 580, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 584, 584, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 588, 588, 588, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 593, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 595, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 595, 595, 595, 595, 595, 595, 595, 595, 595, 595, 595, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1589, 1589, 1589, 1589, 1589, 1589, 1589, 1589, 1589, 1589, 1589, 1589, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 621, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1591, 1591, 1591, 1591, 1591, 1591, 1591, 1591, 1591, 1591, 1591, 1591, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 623, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 626, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 630, 0, 0, -1, 0, 0, 0, 630, 630, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 630, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 630, 630, -1, -1, -1, -1, -1, 630, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 636, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 657, 658, 659, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 1609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 661, 661, 661, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 663, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 665, -1, -1, 229, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 665, -1, -1, 665, 665, 665, -1, -1, -1, 665, 665, -1, -1, -1, -1, -1, 665, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 665, 665, 665, 665, 665, -1, -1, -1, -1, -1, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, 665, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
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
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, 233, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 720, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1627, 1627, 1627, 1627, 1627, 1627, 1627, 1627, 1627, 1627, 1627, 1627, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 724, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 726, 0, 0, -1, -1, 0, 0, 726, 726, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 726, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 726, 726, -1, -1, -1, -1, -1, 726, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 732, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 748, 749, 750, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 80, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 752, 752, 752, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 754, -1, 754, 754, 0, -1, -1, -1, 754, -1, -1, -1, -1, -1, 754, -1, -1, 754, 754, 754, -1, -1, -1, 754, 754, -1, -1, -1, 754, -1, 754, 754, 754, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 754, 754, 754, 754, 754, 754, 754, -1, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, -1, -1, 754, -1, 754, -1, -1, 754, 754, 754, 754, 754, 754, 754, 754, -1, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, -1, -1, -1, 754, 754, 754, 754, 754, 754, 
			-1, -1, -1, 0, -1, 756, 0, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, 0, -1, 1818, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 756, 0, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 757, -1, 757, 757, 0, -1, -1, -1, 757, -1, -1, -1, -1, -1, 757, -1, -1, 757, 757, 757, -1, -1, -1, 757, 757, -1, -1, -1, 757, -1, 757, 757, 757, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 757, 757, 757, 757, 757, 757, 757, -1, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, -1, -1, 757, -1, 757, -1, -1, 757, 757, 757, 757, 757, 757, 757, 757, -1, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, -1, -1, -1, 757, 757, 757, 757, 757, 757, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 758, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 761, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 763, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 766, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 768, -1, 768, 768, 0, -1, -1, -1, 768, -1, -1, -1, -1, -1, 768, -1, -1, 768, 768, 768, -1, -1, -1, 768, 768, -1, -1, -1, 768, -1, 768, 768, 768, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 768, 768, 768, 768, 768, 768, 768, -1, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, -1, -1, 768, -1, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, -1, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, -1, -1, -1, 768, 768, 768, 768, 768, 768, 
			-1, -1, -1, 774, -1, 774, -1, -1, 774, -1, -1, 165, -1, -1, -1, -1, -1, 774, -1, -1, 774, 774, 774, -1, -1, -1, 774, 774, -1, -1, -1, -1, -1, 1819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 774, 774, 774, 774, 774, -1, 165, -1, -1, -1, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1820, 1822, 1824, 1826, 1828, 1830, 1832, 1834, 1836, 1838, 1840, 1842, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, 165, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 775, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, -1, -1, 775, 775, 775, -1, -1, -1, 775, 775, -1, -1, -1, -1, -1, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, 775, 775, 775, 775, -1, -1, -1, -1, -1, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 776, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 776, -1, -1, 776, 776, 776, -1, -1, -1, 776, 776, -1, -1, -1, -1, -1, 776, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 776, 776, 776, 776, 776, -1, -1, -1, -1, -1, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1673, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1674, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1674, -1, -1, 1674, 1674, 1674, -1, -1, -1, 1674, 1674, -1, -1, -1, -1, -1, 1674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1674, 1674, 1674, 1674, 1674, -1, -1, -1, -1, -1, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, 1674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 1675, 1675, 1675, 1675, 1675, 1675, 1675, 1675, 1675, 1675, 1675, 1675, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 782, -1, 782, 782, 0, -1, -1, -1, 782, -1, -1, -1, -1, -1, 782, -1, -1, 782, 782, 782, -1, -1, -1, 782, 782, -1, -1, -1, 782, -1, 782, 782, 782, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 782, 782, 782, 782, 782, 782, 782, -1, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, -1, -1, 782, -1, 782, -1, -1, 782, 782, 782, 782, 782, 782, 782, 782, -1, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, -1, -1, -1, 782, 782, 782, 782, 782, 782, 
			-1, 785, 785, 785, 785, 785, 785, 0, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 288, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 1, 1, 1, 1, 1, 291, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, 789, 789, 789, 789, 789, 789, 0, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 
			-1, 793, 793, 793, 0, 0, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 
			-1, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 0, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 794, 
			-1, 796, 796, 796, 0, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 796, 
			-1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 799, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 302, 302, 302, 302, 0, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 
			-1, 302, 302, 302, 302, 0, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 
			-1, 302, 302, 302, 0, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 
			-1, 302, 302, 302, 302, 0, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 
			-1, 302, 302, 302, 302, 0, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 92, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 302, 302, 302, 302, 0, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 
			-1, -1, -1, 808, -1, 808, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 808, -1, -1, 808, 808, 808, -1, -1, -1, 808, 808, -1, -1, -1, -1, -1, 808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 808, 808, 808, 808, 808, -1, -1, -1, -1, -1, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 306, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, -1, 0, 0, -1, 813, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1844, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 813, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 820, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 820, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, 820, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 853, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 854, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 856, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, 0, -1, 881, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 883, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 884, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 885, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 887, -1, 0, 0, -1, 0, 887, -1, 887, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 888, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, -1, 0, 0, 890, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 146, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 943, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 943, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 946, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 953, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 958, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 959, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 959, -1, -1, 959, 959, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 959, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 959, 959, 959, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 960, 0, -1, -1, -1, 0, 960, -1, 960, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 961, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 963, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 964, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 964, -1, -1, 964, 964, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 964, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 964, 964, 964, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 965, 0, -1, -1, -1, 0, 965, -1, 965, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 966, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1706, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1713, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 975, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 981, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, 982, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1717, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1718, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 985, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1722, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1723, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 991, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 992, -1, 0, -1, -1, -1, 992, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 992, -1, -1, -1, -1, -1, -1, -1, -1, -1, 992, 992, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 992, 992, -1, 992, 992, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, 992, -1, -1, -1, 992, 992, 992, 992, 992, 992, 
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
			994, 4, 1548, 3, 28, 2,  -1, // 1
			997, 7, 340, 6, 996, 1549, 29,  -1, // 2
			340, 156,  -1, // 3
			999, 7, 341, 6, 998, 1550, 30,  -1, // 4
			341, 168,  -1, // 5
			5, 342, 173, 31,  -1, // 6
			342, 173, 8,  -1, // 7
			5, 343, 173, 32,  -1, // 8
			343, 173, 8,  -1, // 9
			1000, 9, 1551,  -1, // 10
			1385, 175,  -1, // 11
			344, 1001, 1552,  -1, // 12
			344, 1002, 1553, 10,  -1, // 13
			1386, 177,  -1, // 14
			1387, 178, 345, 1554,  -1, // 15
			346, 1388, 179,  -1, // 16
			1393, 178, 1003, 348, 347, 1390, 181, 1389, 180, 346,  -1, // 17
			347, 1391, 182,  -1, // 18
			348, 1392, 183,  -1, // 19
			3, 1394, 180,  -1, // 20
			1004, 5,  -1, // 21
			1005, 1555,  -1, // 22
			1395, 185,  -1, // 23
			1007, 1557, 349, 1006, 1556,  -1, // 24
			349, 1396, 186,  -1, // 25
			1397, 185,  -1, // 26
			1008, 1558,  -1, // 27
			1009, 1559,  -1, // 28
			1010, 1560,  -1, // 29
			1012, 351, 350, 1011, 1561,  -1, // 30
			350, 1398, 179,  -1, // 31
			351, 1399, 182,  -1, // 32
			1400, 187,  -1, // 33
			352, 1013, 1562,  -1, // 34
			352, 1014, 1563, 8,  -1, // 35
			1015, 1564,  -1, // 36
			1016, 1565,  -1, // 37
			12, 194, 1017, 11,  -1, // 38
			353, 195, 8,  -1, // 39
			353, 195,  -1, // 40
			196, 13,  -1, // 41
			1018, 354, 1566,  -1, // 42
			355, 1401, 197,  -1, // 43
			355, 1401, 197,  -1, // 44
			1020, 1568, 356, 1019, 1567,  -1, // 45
			356, 1402, 197,  -1, // 46
			1022, 1570, 357, 1021, 1569,  -1, // 47
			357, 1403, 197,  -1, // 48
			1024, 1572, 358, 1023, 1571,  -1, // 49
			358, 1404, 197,  -1, // 50
			1025, 1573,  -1, // 51
			359, 1405, 199,  -1, // 52
			4, 359, 3,  -1, // 53
			360, 1406, 199,  -1, // 54
			4, 360, 3,  -1, // 55
			1026, 1574,  -1, // 56
			1027, 1575,  -1, // 57
			1028, 1576,  -1, // 58
			1407, 204,  -1, // 59
			1408, 204,  -1, // 60
			361, 1029, 1577,  -1, // 61
			361, 1030, 1578, 8,  -1, // 62
			362, 1031, 1579,  -1, // 63
			362, 1032, 1580, 8,  -1, // 64
			363, 1034, 1581, 8,  -1, // 65
			363, 1034, 1581, 8, 1033,  -1, // 66
			1035, 1409, 204,  -1, // 67
			1036, 1582,  -1, // 68
			364, 1410, 209,  -1, // 69
			1039, 1412, 204, 9, 1038, 1411, 204, 14, 1037,  -1, // 70
			365, 1413, 210,  -1, // 71
			365, 1040, 1414, 210, 36,  -1, // 72
			366, 1415, 211,  -1, // 73
			366, 1041, 1416, 211, 37,  -1, // 74
			367, 1417, 212,  -1, // 75
			367, 1042, 1418, 212, 15,  -1, // 76
			368, 1419, 213,  -1, // 77
			368, 1043, 1420, 213, 16,  -1, // 78
			369, 1421, 214,  -1, // 79
			369, 1044, 1422, 214, 17,  -1, // 80
			370, 1423, 215,  -1, // 81
			370, 1045, 1424, 215, 1583,  -1, // 82
			1046, 38,  -1, // 83
			1047, 39,  -1, // 84
			371, 1425, 217,  -1, // 85
			371, 1048, 1426, 217, 1584,  -1, // 86
			1049, 18,  -1, // 87
			1050, 19,  -1, // 88
			1051, 40,  -1, // 89
			1052, 41,  -1, // 90
			372, 1427, 219,  -1, // 91
			372, 1053, 1428, 219, 1585,  -1, // 92
			1054, 42,  -1, // 93
			1055, 43,  -1, // 94
			373, 1429, 221,  -1, // 95
			373, 1056, 1430, 221, 1586,  -1, // 96
			1057, 20,  -1, // 97
			1058, 21,  -1, // 98
			374, 1431, 223,  -1, // 99
			374, 1059, 1432, 223, 1587,  -1, // 100
			1060, 22,  -1, // 101
			1061, 23,  -1, // 102
			1062, 24,  -1, // 103
			375, 1433, 225,  -1, // 104
			375, 1063, 1434, 225, 25,  -1, // 105
			376, 1435, 226,  -1, // 106
			1436, 227,  -1, // 107
			1064, 1437, 204, 1588,  -1, // 108
			229, 1065, 13,  -1, // 109
			1066, 13,  -1, // 110
			1067, 44,  -1, // 111
			1068, 45,  -1, // 112
			1069, 46,  -1, // 113
			1070, 47,  -1, // 114
			1071, 48,  -1, // 115
			1072, 49,  -1, // 116
			1073, 50,  -1, // 117
			1074, 51,  -1, // 118
			1075, 52,  -1, // 119
			1076, 53,  -1, // 120
			1077, 54,  -1, // 121
			1438, 230,  -1, // 122
			1439, 231,  -1, // 123
			1078, 1440, 226, 20,  -1, // 124
			1079, 1441, 226, 21,  -1, // 125
			1080, 1442, 226, 26,  -1, // 126
			1081, 1443, 226, 17,  -1, // 127
			1082, 1444, 226, 22,  -1, // 128
			1083, 1445, 226, 27,  -1, // 129
			1084, 1446, 226, 55,  -1, // 130
			1085, 1447, 226, 56,  -1, // 131
			1087, 1448, 226, 1086, 57,  -1, // 132
			1088, 1449, 226, 4, 1590, 377, 3,  -1, // 133
			380, 1089, 1592, 58, 379,  -1, // 134
			1450, 235,  -1, // 135
			1090, 1451, 226, 59,  -1, // 136
			1452, 187,  -1, // 137
			1091, 4, 1594, 3, 381, 1593,  -1, // 138
			3, 383, 173,  -1, // 139
			1453, 238, 384, 3,  -1, // 140
			385, 1454, 239,  -1, // 141
			385, 1455, 240,  -1, // 142
			1092, 4, 1595, 3,  -1, // 143
			1093, 4, 1596, 3, 26,  -1, // 144
			1094, 12, 1597, 11,  -1, // 145
			1095, 55,  -1, // 146
			1096, 56,  -1, // 147
			1097, 61,  -1, // 148
			1456, 241, 386, 10,  -1, // 149
			1457, 241, 1098, 62,  -1, // 150
			1099, 1598,  -1, // 151
			1100, 63,  -1, // 152
			1101, 64,  -1, // 153
			4, 1458, 204, 3,  -1, // 154
			1459, 242,  -1, // 155
			1102, 1599,  -1, // 156
			1103, 65,  -1, // 157
			1104, 66,  -1, // 158
			1105, 1600,  -1, // 159
			1106, 1601,  -1, // 160
			1107, 69,  -1, // 161
			1108, 70,  -1, // 162
			1109, 71,  -1, // 163
			1110, 4, 1602, 3, 72,  -1, // 164
			1111, 4, 1603, 3, 73,  -1, // 165
			1112, 4, 1604, 3, 74,  -1, // 166
			1113, 4, 33, 8, 189, 3, 75,  -1, // 167
			1114, 4, 1460, 204, 3, 76,  -1, // 168
			1115, 1605,  -1, // 169
			1116, 1606,  -1, // 170
			1118, 388, 1608, 1117, 1607,  -1, // 171
			1119, 389, 387,  -1, // 172
			389, 387,  -1, // 173
			1120, 7, 390, 1461, 243, 6,  -1, // 174
			390, 1462, 243, 8,  -1, // 175
			391, 1122, 13, 1610,  -1, // 176
			1123, 1611,  -1, // 177
			1125, 1612, 1124,  -1, // 178
			229, 1126,  -1, // 179
			1127, 0,  -1, // 180
			1128, 1613,  -1, // 181
			1463, 245,  -1, // 182
			1464, 245,  -1, // 183
			393, 1129, 1614,  -1, // 184
			393, 1130, 1615, 8,  -1, // 185
			394, 1131, 1616,  -1, // 186
			394, 1132, 1617, 8,  -1, // 187
			395, 1134, 1618, 8,  -1, // 188
			395, 1134, 1618, 8, 1133,  -1, // 189
			396, 1465, 249,  -1, // 190
			1135, 1467, 245, 9, 1466, 245, 14,  -1, // 191
			397, 1468, 250,  -1, // 192
			397, 1136, 1469, 250, 36,  -1, // 193
			398, 1470, 251,  -1, // 194
			398, 1137, 1471, 251, 37,  -1, // 195
			399, 1472, 252,  -1, // 196
			399, 1138, 1473, 252, 15,  -1, // 197
			400, 1474, 253,  -1, // 198
			400, 1139, 1475, 253, 16,  -1, // 199
			401, 1476, 254,  -1, // 200
			401, 1140, 1477, 254, 17,  -1, // 201
			402, 1478, 255,  -1, // 202
			402, 1141, 1479, 255, 1619,  -1, // 203
			403, 1480, 256,  -1, // 204
			403, 1142, 1481, 256, 1620,  -1, // 205
			404, 1482, 257,  -1, // 206
			404, 1143, 1483, 257, 1621,  -1, // 207
			405, 1484, 258,  -1, // 208
			405, 1144, 1485, 258, 1622,  -1, // 209
			406, 1486, 259,  -1, // 210
			406, 1145, 1487, 259, 1623,  -1, // 211
			407, 1488, 260,  -1, // 212
			407, 1146, 1489, 260, 25,  -1, // 213
			408, 1490, 261,  -1, // 214
			1148, 1491, 245, 1624, 1147,  -1, // 215
			1492, 262,  -1, // 216
			1493, 263,  -1, // 217
			1149, 1494, 261, 20,  -1, // 218
			1150, 1495, 261, 21,  -1, // 219
			1151, 1496, 261, 26,  -1, // 220
			1152, 1497, 261, 17,  -1, // 221
			1153, 1498, 261, 22,  -1, // 222
			1154, 1499, 261, 27,  -1, // 223
			1155, 1500, 261, 55,  -1, // 224
			1156, 1501, 261, 56,  -1, // 225
			1157, 1502, 261, 57,  -1, // 226
			1158, 4, 191, 3, 72,  -1, // 227
			1159, 4, 191, 3, 73,  -1, // 228
			1160, 4, 1625, 3, 74,  -1, // 229
			1161, 4, 33, 8, 189, 3, 75,  -1, // 230
			1162, 4, 1503, 245, 3, 76,  -1, // 231
			1163, 1504, 261, 4, 1626, 409, 3,  -1, // 232
			1164, 1628, 58, 411,  -1, // 233
			1165, 1505, 261, 59,  -1, // 234
			1166, 1629,  -1, // 235
			1167, 4, 248, 3, 412, 1630,  -1, // 236
			414, 1506, 265,  -1, // 237
			414, 1507, 266,  -1, // 238
			1168, 4, 1631, 3,  -1, // 239
			1169, 4, 1632, 3, 26,  -1, // 240
			1170, 12, 1633, 11,  -1, // 241
			1171, 55,  -1, // 242
			1172, 56,  -1, // 243
			1173, 61,  -1, // 244
			1508, 267, 415, 10,  -1, // 245
			1509, 267, 1174, 62,  -1, // 246
			1175, 1634,  -1, // 247
			1176, 63,  -1, // 248
			1177, 64,  -1, // 249
			1178, 1635,  -1, // 250
			1179, 65,  -1, // 251
			1180, 66,  -1, // 252
			1181, 1636,  -1, // 253
			1182, 68,  -1, // 254
			1183, 69,  -1, // 255
			1184, 70,  -1, // 256
			1185, 71,  -1, // 257
			1510, 268,  -1, // 258
			4, 1511, 245, 3,  -1, // 259
			1186, 1637,  -1, // 260
			1187, 1638,  -1, // 261
			1188, 417, 201, 79,  -1, // 262
			1189, 418, 416,  -1, // 263
			418, 416,  -1, // 264
			1191, 1640, 419, 1190, 1639,  -1, // 265
			419, 169,  -1, // 266
			1194, 1642, 421, 1193, 420, 1192, 1641,  -1, // 267
			420, 271,  -1, // 268
			421, 169,  -1, // 269
			1195, 4, 1644, 3, 1643,  -1, // 270
			1196, 4, 1646, 3, 60, 10, 1645,  -1, // 271
			60, 423, 10, 33,  -1, // 272
			423, 10, 33,  -1, // 273
			60, 424, 1197, 10, 1647,  -1, // 274
			424, 1198, 10, 1648,  -1, // 275
			5, 201,  -1, // 276
			1201, 425, 1651, 1199, 1650, 1649, 3, 82,  -1, // 277
			1653, 1200, 1652,  -1, // 278
			1203, 1656, 426, 1202, 1655, 4, 1654, 3, 84,  -1, // 279
			426, 1512, 285,  -1, // 280
			1204, 1659, 1658, 1657,  -1, // 281
			1205, 1661, 1660,  -1, // 282
			1207, 1664, 1206, 1663, 1662, 3, 87,  -1, // 283
			1210, 5, 4, 1667, 3, 87, 1209, 1666, 1208, 1665,  -1, // 284
			1217, 1670, 1216, 4, 429, 5, 428, 427, 1211, 3, 1668,  -1, // 285
			5, 203,  -1, // 286
			1213, 1669, 1212,  -1, // 287
			1215, 206, 1214,  -1, // 288
			1218, 5, 1671,  -1, // 289
			1219, 5, 1672,  -1, // 290
			1220, 5, 431, 93, 430,  -1, // 291
			1222, 1677, 1221, 1676, 432,  -1, // 292
			1224, 1679, 433, 1223, 1678, 57,  -1, // 293
			433, 169,  -1, // 294
			1225, 9, 34,  -1, // 295
			1226, 9, 35,  -1, // 296
			434, 287,  -1, // 297
			1227, 7, 434, 287, 6,  -1, // 298
			1230, 170, 1229, 4, 206, 3, 95,  -1, // 299
			1231, 5, 1680,  -1, // 300
			437, 436,  -1, // 301
			7, 437, 6,  -1, // 302
			5, 302,  -1, // 303
			438, 303,  -1, // 304
			438, 303,  -1, // 305
			439, 303,  -1, // 306
			12, 439, 303, 11,  -1, // 307
			440, 303,  -1, // 308
			4, 440, 3,  -1, // 309
			441, 290, 4, 302, 3, 82,  -1, // 310
			290, 83,  -1, // 311
			291, 4, 302, 3, 84,  -1, // 312
			290, 4, 302, 3, 87,  -1, // 313
			5, 4, 302, 3, 87, 290, 88,  -1, // 314
			290, 4, 444, 5, 443, 5, 442, 3, 89,  -1, // 315
			5, 445, 90,  -1, // 316
			5, 446, 91,  -1, // 317
			5, 448, 93, 447,  -1, // 318
			290, 94,  -1, // 319
			449, 305,  -1, // 320
			7, 449, 305, 6,  -1, // 321
			290, 1233, 4, 247, 3, 95,  -1, // 322
			1234, 5, 192,  -1, // 323
			451, 1513, 308,  -1, // 324
			451, 1513, 308,  -1, // 325
			1235, 1681,  -1, // 326
			1236, 1682,  -1, // 327
			1237, 1683,  -1, // 328
			1238, 1684,  -1, // 329
			1514, 311,  -1, // 330
			452, 1515, 238,  -1, // 331
			452, 1515, 238,  -1, // 332
			1239, 1685,  -1, // 333
			1240, 1686,  -1, // 334
			1241, 96,  -1, // 335
			1242, 97,  -1, // 336
			1243, 98,  -1, // 337
			1244, 99,  -1, // 338
			1245, 100,  -1, // 339
			1246, 101,  -1, // 340
			1247, 102,  -1, // 341
			1248, 103,  -1, // 342
			1249, 104,  -1, // 343
			1250, 105,  -1, // 344
			1251, 106,  -1, // 345
			1252, 107,  -1, // 346
			1253, 108,  -1, // 347
			1254, 109,  -1, // 348
			1255, 110,  -1, // 349
			1256, 111,  -1, // 350
			1257, 112,  -1, // 351
			1258, 19, 1687, 18, 112,  -1, // 352
			1259, 113,  -1, // 353
			1260, 114,  -1, // 354
			1261, 115,  -1, // 355
			1262, 116,  -1, // 356
			1263, 117,  -1, // 357
			1264, 118,  -1, // 358
			1265, 119,  -1, // 359
			1266, 120,  -1, // 360
			1267, 121,  -1, // 361
			1268, 122,  -1, // 362
			1269, 123,  -1, // 363
			6, 124,  -1, // 364
			1270, 1688,  -1, // 365
			1271, 1689,  -1, // 366
			455, 1273, 1690, 1272,  -1, // 367
			1274, 3,  -1, // 368
			1275, 1691,  -1, // 369
			1277, 1516, 314, 1276, 124,  -1, // 370
			1278, 125,  -1, // 371
			1279, 126,  -1, // 372
			1280, 127,  -1, // 373
			1281, 128,  -1, // 374
			1282, 129,  -1, // 375
			1283, 61,  -1, // 376
			1284, 130,  -1, // 377
			1285, 131,  -1, // 378
			1286, 132,  -1, // 379
			1287, 133,  -1, // 380
			1288, 134,  -1, // 381
			1289, 135,  -1, // 382
			1290, 124,  -1, // 383
			1291, 136,  -1, // 384
			1292, 137,  -1, // 385
			1293, 138,  -1, // 386
			1294, 139,  -1, // 387
			1295, 140,  -1, // 388
			1296, 141,  -1, // 389
			1297, 142,  -1, // 390
			1298, 4, 1692, 3, 132,  -1, // 391
			456, 1517, 315,  -1, // 392
			456, 1518, 315, 8,  -1, // 393
			1301, 458, 457, 1519, 1693,  -1, // 394
			1299, 1694, 60,  -1, // 395
			1300, 1695, 13,  -1, // 396
			459, 1520, 179,  -1, // 397
			461, 460, 1521, 180, 459,  -1, // 398
			460, 1522, 182,  -1, // 399
			461, 1523, 183,  -1, // 400
			462, 1302, 1524, 1696,  -1, // 401
			462, 1525, 317, 10,  -1, // 402
			1303, 1697,  -1, // 403
			1304, 63,  -1, // 404
			1305, 64,  -1, // 405
			1306, 143,  -1, // 406
			1307, 60,  -1, // 407
			1308, 144,  -1, // 408
			1309, 60, 98,  -1, // 409
			1310, 144, 98,  -1, // 410
			1311, 4, 3, 145,  -1, // 411
			1312, 1698, 145,  -1, // 412
			1313, 1699, 145,  -1, // 413
			1314, 26,  -1, // 414
			1315, 27,  -1, // 415
			1316, 55,  -1, // 416
			1317, 56,  -1, // 417
			1318, 55, 146,  -1, // 418
			1319, 56, 146,  -1, // 419
			1320, 62, 463,  -1, // 420
			1321, 20,  -1, // 421
			1322, 21,  -1, // 422
			1323, 22,  -1, // 423
			1324, 23,  -1, // 424
			1325, 24,  -1, // 425
			1326, 42,  -1, // 426
			1327, 43,  -1, // 427
			1328, 17,  -1, // 428
			1329, 16,  -1, // 429
			1330, 15,  -1, // 430
			1331, 38,  -1, // 431
			1332, 39,  -1, // 432
			1333, 18,  -1, // 433
			1334, 40,  -1, // 434
			1335, 19,  -1, // 435
			1336, 41,  -1, // 436
			1337, 13,  -1, // 437
			1338, 44,  -1, // 438
			1339, 45,  -1, // 439
			1340, 46,  -1, // 440
			1341, 47,  -1, // 441
			1342, 48,  -1, // 442
			1343, 49,  -1, // 443
			1344, 50,  -1, // 444
			1345, 51,  -1, // 445
			1346, 52,  -1, // 446
			1347, 53,  -1, // 447
			1348, 54,  -1, // 448
			1349, 147,  -1, // 449
			1350, 37,  -1, // 450
			1351, 36,  -1, // 451
			1352, 12, 11,  -1, // 452
			1353, 25,  -1, // 453
			464, 1354, 22,  -1, // 454
			464, 1355, 1700,  -1, // 455
			1526, 319,  -1, // 456
			1527, 181,  -1, // 457
			1528, 320,  -1, // 458
			1357, 12, 1701, 11,  -1, // 459
			1358, 12, 1702, 11,  -1, // 460
			1359, 12, 11,  -1, // 461
			4, 1529, 321, 3,  -1, // 462
			4, 3,  -1, // 463
			466, 1530, 322,  -1, // 464
			466, 1531, 322, 8,  -1, // 465
			1532, 323,  -1, // 466
			1533, 324,  -1, // 467
			1534, 325,  -1, // 468
			1362, 470, 469, 468, 467, 1360, 1703,  -1, // 469
			467, 1535, 179,  -1, // 470
			1536, 180,  -1, // 471
			469, 1537, 182,  -1, // 472
			1361, 1704, 13,  -1, // 473
			1364, 474, 473, 472, 471, 1538, 179,  -1, // 474
			471, 1538, 179,  -1, // 475
			1539, 180,  -1, // 476
			473, 1540, 182,  -1, // 477
			1363, 1705, 13,  -1, // 478
			1365, 148, 475,  -1, // 479
			1366, 1707, 9,  -1, // 480
			1367, 127,  -1, // 481
			1368, 1708,  -1, // 482
			1369, 1709,  -1, // 483
			1370, 1710,  -1, // 484
			1371, 1711,  -1, // 485
			1541, 331, 1372, 477, 476, 1712,  -1, // 486
			1714, 9,  -1, // 487
			1373, 149,  -1, // 488
			1374, 150,  -1, // 489
			1375, 151,  -1, // 490
			1377, 7, 1542, 332, 1376, 6,  -1, // 491
			478, 1543, 333,  -1, // 492
			478, 1544, 333, 8,  -1, // 493
			1716, 13,  -1, // 494
			1378, 479, 1715,  -1, // 495
			1545, 314, 1379, 482, 481, 480, 152,  -1, // 496
			1719, 9,  -1, // 497
			1380, 4, 1720, 3, 28,  -1, // 498
			1381, 1721,  -1, // 499
			1382, 86,  -1, // 500
			1546, 314, 1383, 483, 153,  -1, // 501
			1547, 314, 1384, 485, 1724, 484, 154,  -1, // 502
			1725, 9,  -1, // 503
			486, 168,  -1, // 504
			7, 486, 6,  -1, // 505
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 10, 18, 21, 29, 32, 37, 41, 46, 50, 54, 57, 61, 66, 69, 74, 78, 89, 93, 97, 101, 104, 107, 110, 116, 120, 123, 126, 129, 132, 138, 142, 146, 149, 153, 158, 161, 164, 169, 173, 176, 179, 183, 187, 191, 197, 201, 207, 211, 217, 221, 224, 228, 232, 236, 240, 243, 246, 249, 252, 255, 259, 264, 268, 273, 278, 284, 288, 291, 295, 305, 309, 315, 319, 325, 329, 335, 339, 345, 349, 355, 359, 365, 368, 371, 375, 381, 384, 387, 390, 393, 397, 403, 406, 409, 413, 419, 422, 425, 429, 435, 438, 441, 444, 448, 454, 458, 461, 466, 470, 473, 476, 479, 482, 485, 488, 491, 494, 497, 500, 503, 506, 509, 512, 517, 522, 527, 532, 537, 542, 547, 552, 558, 566, 572, 575, 580, 583, 590, 594, 599, 603, 607, 612, 618, 623, 626, 629, 632, 637, 642, 645, 648, 651, 656, 659, 662, 665, 668, 671, 674, 677, 680, 683, 689, 695, 701, 709, 716, 719, 722, 728, 732, 735, 742, 747, 752, 755, 759, 762, 765, 768, 771, 774, 778, 783, 787, 792, 797, 803, 807, 815, 819, 825, 829, 835, 839, 845, 849, 855, 859, 865, 869, 875, 879, 885, 889, 895, 899, 905, 909, 915, 919, 925, 929, 935, 938, 941, 946, 951, 956, 961, 966, 971, 976, 981, 986, 992, 998, 1004, 1012, 1019, 1027, 1032, 1037, 1040, 1047, 1051, 1055, 1060, 1066, 1071, 1074, 1077, 1080, 1085, 1090, 1093, 1096, 1099, 1102, 1105, 1108, 1111, 1114, 1117, 1120, 1123, 1126, 1131, 1134, 1137, 1142, 1146, 1149, 1155, 1158, 1166, 1169, 1172, 1178, 1186, 1191, 1195, 1201, 1206, 1209, 1218, 1222, 1232, 1236, 1241, 1245, 1253, 1264, 1276, 1279, 1283, 1287, 1291, 1295, 1301, 1307, 1314, 1317, 1321, 1325, 1328, 1334, 1342, 1346, 1349, 1353, 1356, 1359, 1362, 1365, 1370, 1373, 1377, 1384, 1387, 1393, 1399, 1407, 1417, 1421, 1425, 1430, 1433, 1436, 1441, 1448, 1452, 1456, 1460, 1463, 1466, 1469, 1472, 1475, 1479, 1483, 1486, 1489, 1492, 1495, 1498, 1501, 1504, 1507, 1510, 1513, 1516, 1519, 1522, 1525, 1528, 1531, 1534, 1537, 1540, 1546, 1549, 1552, 1555, 1558, 1561, 1564, 1567, 1570, 1573, 1576, 1579, 1582, 1585, 1588, 1593, 1596, 1599, 1605, 1608, 1611, 1614, 1617, 1620, 1623, 1626, 1629, 1632, 1635, 1638, 1641, 1644, 1647, 1650, 1653, 1656, 1659, 1662, 1665, 1671, 1675, 1680, 1686, 1690, 1694, 1698, 1704, 1708, 1712, 1717, 1722, 1725, 1728, 1731, 1734, 1737, 1740, 1744, 1748, 1753, 1757, 1761, 1764, 1767, 1770, 1773, 1777, 1781, 1785, 1788, 1791, 1794, 1797, 1800, 1803, 1806, 1809, 1812, 1815, 1818, 1821, 1824, 1827, 1830, 1833, 1836, 1839, 1842, 1845, 1848, 1851, 1854, 1857, 1860, 1863, 1866, 1869, 1872, 1875, 1878, 1882, 1885, 1889, 1893, 1896, 1899, 1902, 1907, 1912, 1916, 1921, 1924, 1928, 1933, 1936, 1939, 1942, 1950, 1954, 1957, 1961, 1965, 1973, 1977, 1980, 1984, 1988, 1992, 1996, 1999, 2002, 2005, 2008, 2011, 2018, 2021, 2024, 2027, 2030, 2037, 2041, 2046, 2049, 2053, 2061, 2064, 2070, 2073, 2076, 2082, 2090, 2093, 2096, 
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
		
		case EToken_Silent:
			return 92;
		
		case EToken_Return:
			return 93;
		
		case EToken_Once:
			return 94;
		
		case EToken_OnChange:
			return 95;
		
		case EToken_Typedef:
			return 96;
		
		case EToken_Alias:
			return 97;
		
		case EToken_Static:
			return 98;
		
		case EToken_Thread:
			return 99;
		
		case EToken_Stack:
			return 100;
		
		case EToken_Heap:
			return 101;
		
		case EToken_HeapU:
			return 102;
		
		case EToken_Abstract:
			return 103;
		
		case EToken_Virtual:
			return 104;
		
		case EToken_Override:
			return 105;
		
		case EToken_Mutable:
			return 106;
		
		case EToken_Nullable:
			return 107;
		
		case EToken_Public:
			return 108;
		
		case EToken_Protected:
			return 109;
		
		case EToken_Void:
			return 110;
		
		case EToken_Variant:
			return 111;
		
		case EToken_Object:
			return 112;
		
		case EToken_Bool:
			return 113;
		
		case EToken_Int8:
			return 114;
		
		case EToken_Int16:
			return 115;
		
		case EToken_Int32:
			return 116;
		
		case EToken_Int64:
			return 117;
		
		case EToken_Int:
			return 118;
		
		case EToken_IntPtr:
			return 119;
		
		case EToken_Char:
			return 120;
		
		case EToken_Float:
			return 121;
		
		case EToken_Double:
			return 122;
		
		case EToken_Auto:
			return 123;
		
		case EToken_Property:
			return 124;
		
		case EToken_Unsigned:
			return 125;
		
		case EToken_BigEndian:
			return 126;
		
		case EToken_Const:
			return 127;
		
		case EToken_ConstD:
			return 128;
		
		case EToken_Volatile:
			return 129;
		
		case EToken_Thin:
			return 130;
		
		case EToken_Unsafe:
			return 131;
		
		case EToken_Pitcher:
			return 132;
		
		case EToken_Cdecl:
			return 133;
		
		case EToken_Stdcall:
			return 134;
		
		case EToken_Function:
			return 135;
		
		case EToken_Bindable:
			return 136;
		
		case EToken_AutoGet:
			return 137;
		
		case EToken_Indexed:
			return 138;
		
		case EToken_Multicast:
			return 139;
		
		case EToken_Event:
			return 140;
		
		case EToken_EventD:
			return 141;
		
		case EToken_Reactor:
			return 142;
		
		case EToken_PreConstruct:
			return 143;
		
		case EToken_Destruct:
			return 144;
		
		case EToken_Operator:
			return 145;
		
		case EToken_Postfix:
			return 146;
		
		case EToken_AtAssign:
			return 147;
		
		case EToken_Ellipsis:
			return 148;
		
		case EToken_Enum:
			return 149;
		
		case EToken_EnumF:
			return 150;
		
		case EToken_EnumC:
			return 151;
		
		case EToken_Struct:
			return 152;
		
		case EToken_Union:
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
			EToken_Silent, 
			EToken_Return, 
			EToken_Once, 
			EToken_OnChange, 
			EToken_Typedef, 
			EToken_Alias, 
			EToken_Static, 
			EToken_Thread, 
			EToken_Stack, 
			EToken_Heap, 
			EToken_HeapU, 
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
			EToken_ConstD, 
			EToken_Volatile, 
			EToken_Thin, 
			EToken_Unsafe, 
			EToken_Pitcher, 
			EToken_Cdecl, 
			EToken_Stdcall, 
			EToken_Function, 
			EToken_Bindable, 
			EToken_AutoGet, 
			EToken_Indexed, 
			EToken_Multicast, 
			EToken_Event, 
			EToken_EventD, 
			EToken_Reactor, 
			EToken_PreConstruct, 
			EToken_Destruct, 
			EToken_Operator, 
			EToken_Postfix, 
			EToken_AtAssign, 
			EToken_Ellipsis, 
			EToken_Enum, 
			EToken_EnumF, 
			EToken_EnumC, 
			EToken_Struct, 
			EToken_Union, 
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
			"function_formal_argument_ellipsis",
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
			
		
		case ESymbol_function_formal_argument_ellipsis:
			pNode = AXL_MEM_NEW (CSymbolNode_function_formal_argument_ellipsis);
			
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls42);
				
			break;
			
		
		case ESymbol_union_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
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
			
			{ 4, 201 },
			
			{ 3, 80 },
			
			{ 0, 33 },
			
			{ 1, 201 },
			
			{ 0, 201 },
			
			{ 0, 192 },
			
			{ 0, 192 },
			
			{ 1, 192 },
			
			{ 0, 192 },
			
			{ 1, 246 },
			
			{ 0, 246 },
			
			{ 0, 216 },
			
			{ 0, 218 },
			
			{ 0, 220 },
			
			{ 0, 222 },
			
			{ 0, 224 },
			
			{ 0, 228 },
			
			{ 0, 191 },
			
			{ 0, 189 },
			
			{ 0, 233 },
			
			{ 1, 264 },
			
			{ 0, 189 },
			
			{ 0, 237 },
			
			{ 0, 248 },
			
			{ 0, 248 },
			
			{ 0, 246 },
			
			{ 0, 33 },
			
			{ 0, 33 },
			
			{ 0, 67 },
			
			{ 0, 77 },
			
			{ 1, 78 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 273 },
			
			{ 1, 203 },
			
			{ 0, 81 },
			
			{ 1, 203 },
			
			{ 0, 33 },
			
			{ 1, 33 },
			
			{ 0, 201 },
			
			{ 1, 4 },
			
			{ 4, 170 },
			
			{ 2, 83 },
			
			{ 3, 170 },
			
			{ 0, 201 },
			
			{ 1, 6 },
			
			{ 2, 7 },
			
			{ 1, 85 },
			
			{ 0, 208 },
			
			{ 2, 9 },
			
			{ 0, 86 },
			
			{ 1, 9 },
			
			{ 0, 201 },
			
			{ 1, 4 },
			
			{ 2, 170 },
			
			{ 0, 88 },
			
			{ 1, 170 },
			
			{ 2, 201 },
			
			{ 0, 89 },
			
			{ 1, 201 },
			
			{ 2, 170 },
			
			{ 0, 90 },
			
			{ 0, 91 },
			
			{ 1, 92 },
			
			{ 0, 201 },
			
			{ 0, 233 },
			
			{ 1, 94 },
			
			{ 2, 170 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 196 },
			
			{ 0, 233 },
			
			{ 0, 174 },
			
			{ 0, 309 },
			
			{ 0, 310 },
			
			{ 0, 309 },
			
			{ 0, 310 },
			
			{ 0, 189 },
			
			{ 0, 312 },
			
			{ 0, 237 },
			
			{ 0, 173 },
			
			{ 0, 173 },
			
			{ 0, 196 },
			
			{ 0, 316 },
			
			{ 1, 198 },
			
			{ 2, 196 },
			
			{ 0, 317 },
			
			{ 0, 33 },
			
			{ 0, 189 },
			
			{ 0, 318 },
			
			{ 0, 310 },
			
			{ 0, 196 },
			
			{ 0, 67 },
			
			{ 0, 176 },
			
			{ 1, 196 },
			
			{ 0, 196 },
			
			{ 0, 131 },
			
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
// #line 6552 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 48 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 return false; ;
// #line 6563 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 60 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return OpenGlobalNamespace ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos) != NULL;
			;
// #line 6576 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pModule->m_NamespaceMgr.CloseNamespace ();
			;
// #line 6589 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 71 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return OpenTypeExtension ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos);
			;
// #line 6602 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 5:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 75 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pModule->m_NamespaceMgr.CloseNamespace ();
			;
// #line 6615 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 6:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 90 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->m_CurrentAccessKind = (*(_cls32*) GetAstLocator (0)).m_AccessKind;
			;
// #line 6628 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 7:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 107 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pName->AddName ((*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 6641 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 8:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 111 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pName->AddName ((*GetTokenLocator (1)).m_Data.m_String);
			;
// #line 6654 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 9:
			{
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 149 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return Declare (&__pSymbol->m_Local.Declarator);
			;
// #line 6667 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 10:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 183 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return IsEmptyDeclarationTerminatorAllowed (__pSymbol->m_Arg.pTypeSpecifier);
			;
// #line 6680 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 11:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 187 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return SetDeclarationBody (&(*(_cls2*) GetAstLocator (0)).m_TokenList);
			;
// #line 6693 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 6707 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 13:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
// #line 212 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6720 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 6734 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 6748 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 16:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 231 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6761 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 17:
			{
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 246 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls30*) GetAstLocator (0)).m_TypeSpecifier);
			;
// #line 6774 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 6791 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 19:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
// #line 274 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_TypeList.InsertTail ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 6804 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 20:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
// #line 278 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_TypeList.InsertTail ((*(_cls4*) GetAstLocator (1)).m_pType);
			;
// #line 6817 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 21:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
// #line 290 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_pType = (*(_cls4*) GetAstLocator (0)).m_pType;
			;
// #line 6830 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 22:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
// #line 294 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_pType = (*(_cls23*) GetAstLocator (0)).m_Value.GetType ();
			;
// #line 6843 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 23:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 305 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pAttributeBlock = m_pModule->m_AttributeMgr.CreateAttributeBlock ();
			;
// #line 6856 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 6873 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 25:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 14 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6886 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 26:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6899 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 27:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 23 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6912 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 28:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 28 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6925 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 29:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 32 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6938 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 30:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 37 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6951 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 31:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 41 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6964 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 32:
			{
			CSymbolNode_arg_pass1* __pSymbol = (CSymbolNode_arg_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6977 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 33:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 74 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_ExpressionValue = (*(_cls9*) GetAstLocator (0)).m_Value;
			;
// #line 6990 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 34:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 83 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_ExpressionValueList.TakeOver (&(*(_cls12*) GetAstLocator (0)).m_ValueList);
			;
// #line 7003 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 35:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
// #line 119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls9*) GetAstLocator (0)).m_Value);
			;
// #line 7016 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 36:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
// #line 123 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls9*) GetAstLocator (1)).m_Value);
			;
// #line 7029 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 37:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 139 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls9*) GetAstLocator (0)).m_Value);
			;
// #line 7042 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 38:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 143 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls10*) GetAstLocator (1)).m_Value);
			;
// #line 7055 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 39:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 147 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail (CValue ());
			;
// #line 7068 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 40:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 151 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls10*) GetAstLocator (0)).m_Value);
			;
// #line 7081 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 7098 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 7122 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 7138 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 44:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 218 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock ((*__pAstNode).m_pElseBlock); // might have changed
				;
// #line 7151 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 7170 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 46:
			{
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 242 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7183 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 47:
			{
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 255 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7196 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 48:
			{
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 268 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7209 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 49:
			{
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 281 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7222 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 50:
			{
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 294 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7235 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 51:
			{
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 307 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7248 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 52:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
// #line 319 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Eq;
		;
// #line 7261 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 53:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
// #line 323 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ne;
		;
// #line 7274 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 54:
			{
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 335 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7287 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 55:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 347 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Lt;
			;
// #line 7300 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 56:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 351 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Gt;
			;
// #line 7313 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 57:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 355 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Le;
			;
// #line 7326 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 58:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 359 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Ge;
			;
// #line 7339 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 59:
			{
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 371 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7352 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 60:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 383 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Shl;
			;
// #line 7365 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 61:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 387 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Shr;
			;
// #line 7378 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 62:
			{
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 399 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls19*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7391 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 63:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 411 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Add;
			;
// #line 7404 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 64:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 415 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Sub;
			;
// #line 7417 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 65:
			{
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 427 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls20*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7430 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 66:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 439 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Mul;
			;
// #line 7443 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 67:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 443 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Div;
			;
// #line 7456 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 68:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 447 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Mod;
			;
// #line 7469 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 69:
			{
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 459 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7482 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 70:
			{
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 480 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls21*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7495 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 71:
			{
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 484 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_CurlyInitializerTargetValue = *__pSymbol->m_Arg.pValue;
			;
// #line 7508 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 72:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 496 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Assign;
			;
// #line 7521 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 73:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 500 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_RefAssign;
			;
// #line 7534 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 74:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 504 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_AddAssign;
			;
// #line 7547 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 75:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 508 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_SubAssign;
			;
// #line 7560 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 76:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 512 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_MulAssign;
			;
// #line 7573 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 77:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 516 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_DivAssign;
			;
// #line 7586 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 78:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 520 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ModAssign;
			;
// #line 7599 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 79:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 524 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ShlAssign;
			;
// #line 7612 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 80:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 528 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ShrAssign;
			;
// #line 7625 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 81:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 532 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_AndAssign;
			;
// #line 7638 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 82:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 536 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_XorAssign;
			;
// #line 7651 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 83:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 540 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_OrAssign;
			;
// #line 7664 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 84:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 557 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Plus, __pSymbol->m_Arg.pValue);
			;
// #line 7677 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 85:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 561 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Minus, __pSymbol->m_Arg.pValue);
			;
// #line 7690 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 86:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 565 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
			;
// #line 7703 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 87:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 569 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, __pSymbol->m_Arg.pValue);
			;
// #line 7716 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 88:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 573 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, __pSymbol->m_Arg.pValue);
			;
// #line 7729 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 89:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 577 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
			;
// #line 7742 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 90:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 581 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
			;
// #line 7755 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 91:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 585 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
			;
// #line 7768 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 92:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 589 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_pModule->m_ControlFlowMgr.LockThrow ();
			;
// #line 7781 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 93:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 593 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_pModule->m_ControlFlowMgr.UnlockThrow ();
			;
// #line 7794 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 7808 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 7828 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 96:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 615 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				bool Result = m_pModule->m_OperatorMgr.DeleteOperator (*__pSymbol->m_Arg.pValue);
				if (!Result)
					return false;

				__pSymbol->m_Arg.pValue->SetVoid ();
			;
// #line 7845 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 97:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
// #line 634 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
				(*__pAstNode).m_ArgList.TakeOver (&(*(_cls12*) GetAstLocator (1)).m_ValueList);
			;
// #line 7859 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 98:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 675 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.CallOperator (__pSymbol->m_Arg.pValue, &(*(_cls12*) GetAstLocator (0)).m_ValueList);
			;
// #line 7872 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 99:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 679 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.ClosureOperator (__pSymbol->m_Arg.pValue, &(*(_cls12*) GetAstLocator (0)).m_ValueList);
			;
// #line 7885 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 100:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 683 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls10*) GetAstLocator (0)).m_Value);
			;
// #line 7898 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 101:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 687 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
			;
// #line 7911 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 102:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 691 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
			;
// #line 7924 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 103:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 696 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.WeakenOperator (__pSymbol->m_Arg.pValue);
			;
// #line 7937 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 104:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 702 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
			;
// #line 7950 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 105:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 710 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.MemberOperator (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 7963 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 106:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 714 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyGetter (__pSymbol->m_Arg.pValue);
			;
// #line 7976 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 107:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 718 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertySetter (__pSymbol->m_Arg.pValue);
			;
// #line 7989 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 108:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 735 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return LookupIdentifier ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
			;
// #line 8002 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 109:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 739 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return GetThisValue (__pSymbol->m_Arg.pValue);
			;
// #line 8015 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 110:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 743 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return GetPitcherReturnValue (__pSymbol->m_Arg.pValue);
			;
// #line 8028 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 111:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 747 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstInt64_u ((*GetTokenLocator (0)).m_Data.m_Int64_u);
			;
// #line 8041 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 112:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 751 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstDouble ((*GetTokenLocator (0)).m_Data.m_Double);
			;
// #line 8054 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 113:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 755 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstBool (true);
			;
// #line 8067 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 114:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 759 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstBool (false);
			;
// #line 8080 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 115:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 763 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetNull ();
			;
// #line 8093 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 116:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 767 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				__pSymbol->m_Arg.pValue->SetConstSizeT (pType->GetSize ());
			;
// #line 8107 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 117:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 772 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				return GetCountOf (pType, __pSymbol->m_Arg.pValue);
			;
// #line 8121 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 118:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 777 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				__pSymbol->m_Arg.pValue->SetType ((*(_cls6*) GetAstLocator (0)).m_pType);
			;
// #line 8135 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 119:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 782 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			;
// #line 8147 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 120:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 785 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyOnChange (__pSymbol->m_Arg.pValue);
			;
// #line 8160 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 121:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 798 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					__pSymbol->m_Local.Literal.m_BinData.Append ((const uchar_t*) (*GetTokenLocator (0)).m_Data.m_String.cc (), (*GetTokenLocator (0)).m_Data.m_String.GetLength ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_Literal;
				;
// #line 8174 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 122:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 803 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					__pSymbol->m_Local.Literal.m_BinData.Append ((*GetTokenLocator (1)).m_Data.m_BinData, (*GetTokenLocator (1)).m_Data.m_BinData.GetCount ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_HexLiteral;
				;
// #line 8188 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 123:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 808 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return AppendFmtLiteral (&__pSymbol->m_Local.Literal, (*GetTokenLocator (2)).m_Data.m_String.cc (), (*GetTokenLocator (2)).m_Data.m_String.GetLength ());
				;
// #line 8201 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 124:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 812 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return IsValidLocator ((*GetTokenLocator (3))) ?
						AppendFmtLiteralValue (&__pSymbol->m_Local.Literal, (*(_cls9*) GetAstLocator (4)).m_Value, (*GetTokenLocator (3)).m_Data.m_String) :
						AppendFmtLiteralValue (&__pSymbol->m_Local.Literal, (*(_cls9*) GetAstLocator (4)).m_Value, rtl::CString ());
				;
// #line 8216 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 125:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 818 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return FinalizeLiteral (&__pSymbol->m_Local.Literal, __pSymbol->m_Arg.pValue);
			;
// #line 8229 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 126:
			{
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 840 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				if (!__pSymbol->m_Local.Initializer.m_Count)
				{
					err::SetFormatStringError ("empty curly initializer");
					return false;
				}
			;
// #line 8246 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 127:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 855 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			 return true; ;
// #line 8257 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 128:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 857 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerNamedItem (__pSymbol->m_Arg.pInitializer, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 8270 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 129:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 862 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, __pSymbol->m_Arg.pInitializer->m_MemberValue, (*(_cls9*) GetAstLocator (1)).m_Value);
				;
// #line 8283 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 130:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 866 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerIndexedItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 8296 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 131:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 870 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, __pSymbol->m_Arg.pInitializer->m_MemberValue, (*(_cls9*) GetAstLocator (0)).m_Value);
			;
// #line 8309 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 132:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 873 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerIndexedItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 8322 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 133:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 878 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return SkipCurlyInitializerItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 8335 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 134:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 6 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				m_ExpressionValue = (*(_cls23*) GetAstLocator (0)).m_Value;
			;
// #line 8348 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 135:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 36 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (0)).m_Value);
			;
// #line 8361 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 136:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 40 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (1)).m_Value);
			;
// #line 8374 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 137:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 56 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (0)).m_Value);
			;
// #line 8387 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 138:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 60 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls24*) GetAstLocator (1)).m_Value);
			;
// #line 8400 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 139:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail (CValue ());
			;
// #line 8413 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 140:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 68 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls24*) GetAstLocator (0)).m_Value);
			;
// #line 8426 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 141:
			{
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 85 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetConditionalOperatorResultType (__pSymbol->m_Local.TrueValue, __pSymbol->m_Local.FalseValue, __pSymbol->m_Arg.pValue);
			;
// #line 8439 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 142:
			{
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 98 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8452 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 143:
			{
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 111 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8465 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 144:
			{
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 124 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8478 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 145:
			{
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 137 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8491 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 146:
			{
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 150 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8504 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 147:
			{
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 163 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8517 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 148:
			{
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 176 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8530 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 149:
			{
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 189 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8543 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 150:
			{
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 202 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls19*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8556 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 151:
			{
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 215 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls20*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8569 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 152:
			{
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 228 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8582 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 153:
			{
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 241 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					if (IsReactor ())
						m_ReactorBindSiteCount = 0; // reset on assignment
				;
// #line 8596 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 154:
			{
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 246 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls21*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
				;
// #line 8609 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 155:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 264 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Plus, __pSymbol->m_Arg.pValue);
			;
// #line 8622 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 156:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 268 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Minus, __pSymbol->m_Arg.pValue);
			;
// #line 8635 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 157:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 272 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
			;
// #line 8648 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 158:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 276 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Addr, __pSymbol->m_Arg.pValue);
			;
// #line 8661 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 159:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 280 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Indir, __pSymbol->m_Arg.pValue);
			;
// #line 8674 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 160:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 284 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
			;
// #line 8687 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 161:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 288 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
			;
// #line 8700 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 162:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
			;
// #line 8713 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 163:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 296 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				// try operator does not change type of value				
			;
// #line 8726 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 164:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 300 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_SizeT);
			;
// #line 8739 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 165:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 304 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_SizeT);
			;
// #line 8752 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 166:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 308 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				__pSymbol->m_Arg.pValue->SetType ((*(_cls6*) GetAstLocator (0)).m_pType);
			;
// #line 8766 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 167:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 313 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
			;
// #line 8778 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 168:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 316 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyOnChangeType (__pSymbol->m_Arg.pValue);
			;
// #line 8791 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 169:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 321 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 8804 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 170:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 325 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
				return NewOperator_s (StorageKind, (*(_cls27*) GetAstLocator (1)).m_pType, __pSymbol->m_Arg.pValue);			
			;
// #line 8818 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 171:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 330 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetVoid ();
			;
// #line 8831 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 172:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
// #line 341 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_pType = (*(_cls4*) GetAstLocator (0)).m_pType;
			;
// #line 8844 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 173:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
// #line 346 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
			;
// #line 8857 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 174:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 363 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetCallOperatorResultType (__pSymbol->m_Arg.pValue, &(*(_cls26*) GetAstLocator (0)).m_ValueList);
			;
// #line 8870 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 175:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 367 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetClosureOperatorResultType (__pSymbol->m_Arg.pValue, &(*(_cls26*) GetAstLocator (0)).m_ValueList);
			;
// #line 8883 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 176:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 371 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls24*) GetAstLocator (0)).m_Value);
			;
// #line 8896 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 177:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 375 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
			;
// #line 8909 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 178:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 379 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
			;
// #line 8922 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 179:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 384 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetWeakenOperatorResultType (__pSymbol->m_Arg.pValue);
			;
// #line 8935 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 180:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 390 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
			;
// #line 8948 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 181:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 398 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetMemberOperatorResultType (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 8961 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 182:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 402 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyGetterType (__pSymbol->m_Arg.pValue);
			;
// #line 8974 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 183:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 406 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertySetterType (__pSymbol->m_Arg.pValue);
			;
// #line 8987 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 184:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 418 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return LookupIdentifierType ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
			;
// #line 9000 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 185:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 422 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return GetThisValueType (__pSymbol->m_Arg.pValue);
			;
// #line 9013 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 186:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 426 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return GetPitcherReturnValueType (__pSymbol->m_Arg.pValue);
			;
// #line 9026 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 187:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 430 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (GetInt64TypeKind ((*GetTokenLocator (0)).m_Data.m_Int64));
			;
// #line 9039 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 188:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 434 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Double);
			;
// #line 9052 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 189:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 438 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Bool);
			;
// #line 9065 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 190:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 442 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Bool);
			;
// #line 9078 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 191:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 446 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetNull ();
			;
// #line 9091 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 192:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 459 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
						
					__pSymbol->m_Local.Literal.m_BinData.Append ((const uchar_t*) (*GetTokenLocator (0)).m_Data.m_String.cc (), (*GetTokenLocator (0)).m_Data.m_String.GetLength ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_Literal;
				;
// #line 9105 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 193:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 464 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
						
					__pSymbol->m_Local.Literal.m_BinData.Append ((*GetTokenLocator (1)).m_Data.m_BinData, (*GetTokenLocator (1)).m_Data.m_BinData.GetCount ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_HexLiteral;
				;
// #line 9119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 194:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 469 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					// mark we have a formatting literal from now on

					if (!__pSymbol->m_Local.Literal.m_FmtLiteralValue)
						__pSymbol->m_Local.Literal.m_FmtLiteralValue.SetConstBool (true); 
				;
// #line 9135 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 195:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 476 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return FinalizeLiteral_s (&__pSymbol->m_Local.Literal, __pSymbol->m_Arg.pValue);
			;
// #line 9148 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 196:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 6 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
				PreCreateLandingPads ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9162 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 197:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 12 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
			;
// #line 9175 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 198:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 20 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
				PreCreateLandingPads ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9189 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 199:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 25 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeBaseTypeMemberConstructBlock ();
			;
// #line 9202 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 200:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 30 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
			;
// #line 9215 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 201:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 42 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return CallBaseTypeMemberConstructor ((*(_cls28*) GetAstLocator (0)).m_Name, &(*(_cls12*) GetAstLocator (1)).m_ValueList);
			;
// #line 9228 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 202:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 46 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return CallBaseTypeConstructor ((*GetTokenLocator (0)).m_Data.m_Integer - 1, &(*(_cls12*) GetAstLocator (1)).m_ValueList);
			;
// #line 9241 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 203:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
// #line 62 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				(*__pAstNode).m_Name.AddName ((*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 9254 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 204:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
// #line 66 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				(*__pAstNode).m_Name.AddName ((*GetTokenLocator (1)).m_Data.m_String);
			;
// #line 9267 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 205:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 108 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.IfStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9280 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 206:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 114 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.IfStmt_Else (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (2)).m_Pos);
				;
// #line 9293 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 207:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.IfStmt_Follow (&__pSymbol->m_Local.Stmt, IsValidLocator ((*GetAstLocator (3))) ? (*GetAstLocator (3)).m_LastToken.m_Pos : (*GetAstLocator (4)).m_LastToken.m_Pos);
			;
// #line 9306 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 208:
			{
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 134 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return
					m_pModule->m_ControlFlowMgr.SwitchStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos) &&
					PreCreateLandingPads ((*GetTokenLocator (1)).m_Data.m_Integer);
			;
// #line 9321 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 209:
			{
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 140 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.SwitchStmt_Follow (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (2)).m_Pos);
			;
// #line 9334 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 210:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 147 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.SwitchStmt_Case (__pSymbol->m_Arg.pStmt, (*(_cls14*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos, (*GetTokenLocator (2)).m_Pos);
			;
// #line 9347 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 211:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 151 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.SwitchStmt_Default (__pSymbol->m_Arg.pStmt, (*GetTokenLocator (0)).m_Pos, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9360 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 212:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 167 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.WhileStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9373 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 213:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 171 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.WhileStmt_Follow (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
			;
// #line 9386 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 214:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 186 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.DoStmt_PreBody (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
			;
// #line 9399 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 215:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 190 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.DoStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (1)).m_LastToken.m_Pos);
			;
// #line 9412 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 216:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 194 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.DoStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (2)).m_Value);
			;
// #line 9425 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 217:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 209 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PreInit (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
			;
// #line 9438 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 218:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 214 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PreCondition (&__pSymbol->m_Local.Stmt);
				;
// #line 9451 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 219:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 218 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					return m_pModule->m_ControlFlowMgr.ForStmt_PostCondition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (1)).m_Value);
				;
// #line 9464 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 220:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 223 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PreLoop (&__pSymbol->m_Local.Stmt);
				;
// #line 9477 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 221:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 227 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PostLoop (&__pSymbol->m_Local.Stmt);
				;
// #line 9490 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 222:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 231 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PreBody (&__pSymbol->m_Local.Stmt);
			;
// #line 9503 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 223:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 235 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
			;
// #line 9516 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 224:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 242 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Break ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9529 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 225:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 249 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Continue ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9542 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 226:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 256 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Return (IsValidLocator ((*(_cls9*) GetAstLocator (0))) ? (*(_cls9*) GetAstLocator (0)).m_Value : CValue (), IsValidLocator ((*GetTokenLocator (1))));
			;
// #line 9555 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 227:
			{
			CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 267 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Static;
				return
					m_pModule->m_ControlFlowMgr.OnceStmt_Create (&__pSymbol->m_Local.Stmt, StorageKind) &&
					m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9571 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 228:
			{
			CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 274 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.OnceStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
			;
// #line 9584 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 229:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 282 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
				PreCreateLandingPads ((*GetTokenLocator (0)).m_Data.m_Integer | ELandingPadFlag_Catch); // always catch
			;
// #line 9598 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 230:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 288 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.EndTry ();
				m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
			;
// #line 9612 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 231:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 297 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Catch ();
			;
// #line 9625 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 232:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 304 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Finally ();
			;
// #line 9638 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 233:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 314 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
			return FinalizeReactor ();
		;
// #line 9651 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 234:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 320 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			 return true; ;
// #line 9662 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 235:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 332 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeReactorOnChangeClause ();
			;
// #line 9675 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 236:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 336 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeReactorOnChangeStmt ();
			;
// #line 9688 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 237:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 343 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return ReactorExpressionStmt (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 9701 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 238:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 84 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			 return true; ;
// #line 9712 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 239:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 92 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			
				return CountReactorBindSites ();
			;
// #line 9725 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 240:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 100 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			
				return CountReactorBindSites ();
			;
// #line 9738 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 241:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 22 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return SetStorageKind ((*(_cls31*) GetAstLocator (0)).m_StorageKind);
			;
// #line 9751 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 242:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 26 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return SetAccessKind ((*(_cls32*) GetAstLocator (0)).m_AccessKind);
			;
// #line 9764 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 243:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 30 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls33*) GetAstLocator (0)).m_pType);
			;
// #line 9777 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 244:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 34 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls36*) GetAstLocator (0)).m_TypeModifier);
			;
// #line 9790 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 245:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 60 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls33*) GetAstLocator (0)).m_pType);
			;
// #line 9803 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 246:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls36*) GetAstLocator (0)).m_TypeModifier);
			;
// #line 9816 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 247:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 77 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Typedef;
			;
// #line 9829 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 248:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 81 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Alias;
			;
// #line 9842 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 249:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 85 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Static;
			;
// #line 9855 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 250:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 89 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
						
				(*__pAstNode).m_StorageKind = EStorage_Thread;
			;
// #line 9868 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 251:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 93 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Stack;
			;
// #line 9881 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 252:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 97 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Heap;
			;
// #line 9894 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 253:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 101 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_HeapU;
			;
// #line 9907 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 254:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 105 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Abstract;
			;
// #line 9920 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 255:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 109 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Virtual;
			;
// #line 9933 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 256:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 113 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Override;
			;
// #line 9946 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 257:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 117 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Mutable;
			;
// #line 9959 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 258:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 121 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Nullable;
			;
// #line 9972 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 259:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
// #line 134 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_AccessKind = EAccess_Public;
			;
// #line 9985 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 260:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
// #line 138 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_AccessKind = EAccess_Protected;
			;
// #line 9998 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 261:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 151 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
			;
// #line 10011 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 262:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 155 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				err::SetStringError ("'variant' type specifier is not yet supported");
				return false; 
			;
// #line 10025 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 263:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 160 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectClass);
			;
// #line 10038 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 264:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 164 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetBoxClassType ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 10051 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 265:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 168 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Bool);
			;
// #line 10064 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 266:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 172 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int8);
			;
// #line 10077 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 267:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 176 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int16);
			;
// #line 10090 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 268:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 180 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32);
			;
// #line 10103 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 269:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 184 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64);
			;
// #line 10116 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 270:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 188 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int);
			;
// #line 10129 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 271:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 192 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
			;
// #line 10142 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 272:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 196 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char);
			;
// #line 10155 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 273:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 200 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Float);
			;
// #line 10168 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 274:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 204 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Double);
			;
// #line 10181 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 275:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 208 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				err::SetStringError ("'auto' type specifier is not yet supported");
				return false; 
			;
// #line 10195 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 276:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 214 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls35*) GetAstLocator (0)).m_pType;
			;
// #line 10208 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 277:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 219 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
			;
// #line 10221 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 278:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 227 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				if (IsTypeSpecified ())
					return false;
			;
// #line 10235 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 279:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 232 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return m_Stage == EStage_Pass1 || FindType ((*(_cls1*) GetAstLocator (0)).m_Name) != NULL;
			;
// #line 10248 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 280:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 236 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return false;
			;
// #line 10261 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 281:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
// #line 248 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				CType* pType = FindType ((*(_cls1*) GetAstLocator (0)).m_Name);
				if (!pType)
				{
					err::SetFormatStringError ("'%s' is not a type", (*(_cls1*) GetAstLocator (0)).m_Name.GetFullName ().cc ());
					return false;
				}

				(*__pAstNode).m_pType = pType;
			;
// #line 10281 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 282:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 270 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				__pSymbol->m_Local.pTemplate = CreatePropertyTemplate ();
			;
// #line 10294 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 283:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 274 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = __pSymbol->m_Local.pTemplate->CalcType ();
				return (*__pAstNode).m_pType != NULL;
			;
// #line 10308 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 284:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 288 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Unsigned;
			;
// #line 10321 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 285:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 292 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_BigEndian;
			;
// #line 10334 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 286:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 296 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Const;
			;
// #line 10347 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 287:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 300 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_ConstD;
			;
// #line 10360 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 288:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 304 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Volatile;
			;
// #line 10373 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 289:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 308 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Weak;
			;
// #line 10386 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 290:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 312 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Thin;
			;
// #line 10399 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 291:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 316 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Unsafe;
			;
// #line 10412 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 292:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 320 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Pitcher;
			;
// #line 10425 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 293:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 324 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Cdecl;
			;
// #line 10438 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 294:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 328 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Stdcall;
			;
// #line 10451 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 295:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 332 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Function;
			;
// #line 10464 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 296:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 336 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Property;
			;
// #line 10477 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 297:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 340 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Bindable;
			;
// #line 10490 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 298:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 344 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_AutoGet;
			;
// #line 10503 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 299:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 348 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Indexed;
			;
// #line 10516 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 300:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 352 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Multicast;
			;
// #line 10529 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 301:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 356 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Event;
			;
// #line 10542 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 302:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 360 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_EventD;
			;
// #line 10555 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 303:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 364 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Reactor;
			;
// #line 10568 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 304:
			{
			CSymbolNode_pitcher_modifier* __pSymbol = (CSymbolNode_pitcher_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 373 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetPitcherCondition (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 10581 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 305:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 14 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*(_cls37*) GetAstLocator (0)).m_Declarator.m_Constructor.TakeOver (&(*(_cls8*) GetAstLocator (1)).m_TokenList);
			;
// #line 10594 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 306:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*(_cls37*) GetAstLocator (0)).m_Declarator.m_Initializer.TakeOver (&(*(_cls7*) GetAstLocator (2)).m_TokenList);
			;
// #line 10607 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 307:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 23 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			 
				return Declare (&(*(_cls37*) GetAstLocator (0)).m_Declarator);
			;
// #line 10620 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 308:
			{
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 49 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->m_Pos = (*GetAstLocator (0)).m_FirstToken.m_Pos;
			;
// #line 10633 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 309:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 57 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddName ((*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 10646 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 310:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 61 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Getter);
			;
// #line 10659 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 311:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 65 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Setter);
			;
// #line 10672 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 312:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 69 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_PreConstructor);
			;
// #line 10685 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 313:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 73 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Constructor);
			;
// #line 10698 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 314:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 77 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Destructor);
			;
// #line 10711 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 315:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 81 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticConstructor);
			;
// #line 10724 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 316:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 85 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticDestructor);
			;
// #line 10737 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 317:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 89 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_CallOperator);
			;
// #line 10750 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 318:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 93 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddCastOperator ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 10763 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 319:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 97 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnaryBinaryOperator ((*(_cls38*) GetAstLocator (0)).m_UnOpKind, (*(_cls38*) GetAstLocator (0)).m_BinOpKind);
			;
// #line 10776 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 320:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 116 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_BwNot;
			;
// #line 10789 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 321:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 120 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_LogNot;
			;
// #line 10802 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 322:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 124 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PreInc;
			;
// #line 10815 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 323:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 128 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PreDec;
			;
// #line 10828 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 324:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 132 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PostInc;
			;
// #line 10841 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 325:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 136 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PostDec;
			;
// #line 10854 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 326:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 140 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Ptr;
			;
// #line 10867 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 327:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 144 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Plus;
				(*__pAstNode).m_BinOpKind = EBinOp_Add;
			;
// #line 10881 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 328:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 149 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Minus;
				(*__pAstNode).m_BinOpKind = EBinOp_Sub;
			;
// #line 10895 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 329:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 154 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Indir;
				(*__pAstNode).m_BinOpKind = EBinOp_Mul;
			;
// #line 10909 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 330:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 159 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Div;
			;
// #line 10922 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 331:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 163 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Mod;
			;
// #line 10935 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 332:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 167 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Shl;
			;
// #line 10948 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 333:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 171 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Shr;
			;
// #line 10961 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 334:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 175 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Addr;
				(*__pAstNode).m_BinOpKind = EBinOp_BwAnd;
			;
// #line 10975 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 335:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 180 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_BwXor;
			;
// #line 10988 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 336:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 184 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_BwOr;
			;
// #line 11001 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 337:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 188 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Eq;
			;
// #line 11014 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 338:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 192 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Ne;
			;
// #line 11027 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 339:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 196 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Lt;
			;
// #line 11040 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 340:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 200 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Le;
			;
// #line 11053 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 341:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 204 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Gt;
			;
// #line 11066 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 342:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 208 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Ge;
			;
// #line 11079 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 343:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 212 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Assign;
			;
// #line 11092 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 344:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 216 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_RefAssign;
			;
// #line 11105 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 345:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 220 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AddAssign;
			;
// #line 11118 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 346:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 224 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_SubAssign;
			;
// #line 11131 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 347:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 228 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_MulAssign;
			;
// #line 11144 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 348:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 232 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_DivAssign;
			;
// #line 11157 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 349:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 236 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ModAssign;
			;
// #line 11170 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 350:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 240 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ShlAssign;
			;
// #line 11183 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 351:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 244 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ShrAssign;
			;
// #line 11196 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 352:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 248 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AndAssign;
			;
// #line 11209 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 353:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 252 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_XorAssign;
			;
// #line 11222 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 354:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 256 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_OrAssign;
			;
// #line 11235 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 355:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 260 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AtAssign;
			;
// #line 11248 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 356:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 264 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_LogAnd;
			;
// #line 11261 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 357:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 268 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_LogOr;
			;
// #line 11274 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 358:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 272 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Idx;
			;
// #line 11287 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 359:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 276 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_At;
			;
// #line 11300 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 360:
			{
			CSymbolNode_declarator_prefix* __pSymbol = (CSymbolNode_declarator_prefix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 288 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddPointerPrefix ();
			;
// #line 11313 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 361:
			{
			CSymbolNode_declarator_prefix* __pSymbol = (CSymbolNode_declarator_prefix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->SetTypeModifier ((*(_cls36*) GetAstLocator (0)).m_TypeModifier);
			;
// #line 11326 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 362:
			{
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 304 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			 return false; ;
// #line 11337 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 363:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 312 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 11350 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 364:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 316 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 11363 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 365:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 320 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix (-1);
			;
// #line 11376 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 366:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 367 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier);
			;
// #line 11389 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 367:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 374 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.m_Initializer.TakeOver (&(*(_cls7*) GetAstLocator (1)).m_TokenList);
			;
// #line 11402 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 368:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 378 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return CreateFormalArg (__pSymbol->m_Arg.pFunctionSuffix, &__pSymbol->m_Local.Declarator) != NULL;
			;
// #line 11415 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 369:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 397 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.m_Initializer.TakeOver (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 11428 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 370:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 401 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return CreateFormalArg (__pSymbol->m_Arg.pFunctionSuffix, &__pSymbol->m_Local.Declarator) != NULL;
			;
// #line 11441 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 371:
			{
			CSymbolNode_function_formal_argument_ellipsis* __pSymbol = (CSymbolNode_function_formal_argument_ellipsis*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 408 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_VarArg;
				if (IsValidLocator ((*GetTokenLocator (0))))
					__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_UnsafeVarArg;
			;
// #line 11456 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 372:
			{
			CSymbolNode_bitfield_suffix* __pSymbol = (CSymbolNode_bitfield_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 419 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddBitFieldSuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 11469 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 373:
			{
			CSymbolNode_post_declarator_modifier* __pSymbol = (CSymbolNode_post_declarator_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 428 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->SetPostDeclaratorModifier (EPostDeclaratorModifier_Const);
			;
// #line 11482 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 374:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 11 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls40*) GetAstLocator (0)).m_pType;
			;
// #line 11495 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 375:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 15 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls42*) GetAstLocator (0)).m_pType;
			;
// #line 11508 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 376:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 19 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls45*) GetAstLocator (0)).m_pType;
			;
// #line 11521 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 377:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 23 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls46*) GetAstLocator (0)).m_pType;
			;
// #line 11534 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 378:
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
// #line 11555 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 379:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 68 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_EnumTypeKind = EEnumType_Normal;
				(*__pAstNode).m_Flags = 0;
			;
// #line 11569 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 380:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 73 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_EnumTypeKind = EEnumType_Flag;
				(*__pAstNode).m_Flags = 0;
			;
// #line 11583 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 381:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 78 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_EnumTypeKind = EEnumType_Normal;
				(*__pAstNode).m_Flags = EEnumTypeFlag_Exposed;
			;
// #line 11597 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 382:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 88 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
			;
// #line 11610 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 383:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 93 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pModule->m_NamespaceMgr.CloseNamespace ();	
			;
// #line 11623 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 384:
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
// #line 11644 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 385:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
// #line 133 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateStructType (
					IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
					IsValidLocator ((*(_cls5*) GetAstLocator (1))) ? &(*(_cls5*) GetAstLocator (1)).m_TypeList : NULL,
					IsValidLocator ((*(_cls43*) GetAstLocator (2))) ? (*(_cls43*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11664 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 386:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls43* __pAstNode = (_cls43*) __pSymbol->m_pAstNode;
// #line 154 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_PackFactor = (*(_cls44*) GetAstLocator (0)).m_Value;
			;
// #line 11677 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 387:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
// #line 165 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_Value = (*GetTokenLocator (0)).m_Data.m_Integer;
			;
// #line 11690 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 388:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
// #line 169 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_Value = m_DefaultStructPackFactor;
			;
// #line 11703 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 389:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
// #line 185 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateUnionType (IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString ());
				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11718 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 390:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
// #line 206 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateClassType (
					(*GetTokenLocator (0)).m_Data.m_String,
					IsValidLocator ((*(_cls5*) GetAstLocator (1))) ? &(*(_cls5*) GetAstLocator (1)).m_TypeList : NULL,
					IsValidLocator ((*(_cls43*) GetAstLocator (2))) ? (*(_cls43*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11738 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 11766 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_declarator_list* __pTarget = (CSymbolNode_declarator_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 128 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier;
// #line 11779 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 129 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier;
// #line 11792 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 145 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11805 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 146 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11818 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 147 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11831 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 147 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11844 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 148 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11857 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 152 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = NULL;
// #line 11870 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier_rslv* __pSymbol = (CSymbolNode_item_declaration_no_specifier_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 166 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11883 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
// #line 197 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 11896 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_statement_pass1* __pTarget = (CSymbolNode_statement_pass1*) pSymbol;
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
			
// #line 210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11909 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pAnchorToken =  (*__pAstNode).m_pAnchorToken;
// #line 11913 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 219 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11926 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 249 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11939 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 250 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11952 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_type_name_impl* __pTarget = (CSymbolNode_type_name_impl*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
			
// #line 265 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.ppType = &(*__pAstNode).m_pType;
// #line 11965 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 265 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pCountValue =  NULL;
// #line 11969 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
			
// #line 8 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 11982 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 17 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11995 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 26 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12008 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 35 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12021 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_arg_pass1* __pTarget = (CSymbolNode_arg_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
			
// #line 55 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 12034 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_arg_pass1* __pTarget = (CSymbolNode_arg_pass1*) pSymbol;
			CSymbolNode_arg_pass1* __pSymbol = (CSymbolNode_arg_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 61 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12047 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
			
// #line 100 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12060 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
			
// #line 109 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12073 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
			
// #line 164 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12086 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_logical_or_expr* __pTarget = (CSymbolNode_logical_or_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
// #line 208 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12099 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
// #line 217 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_TrueValue;
// #line 12112 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
// #line 222 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_FalseValue;
// #line 12125 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 240 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12138 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 241 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12151 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 253 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12164 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 254 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12177 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 266 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12190 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 267 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12203 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 279 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12216 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 280 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12229 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 292 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12242 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 293 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12255 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 305 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12268 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 306 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12281 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 333 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12294 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 41:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 334 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12307 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 42:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 369 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12320 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 43:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 370 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12333 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 44:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 397 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12346 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 45:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 398 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12359 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 46:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 425 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12372 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 47:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 426 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12385 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 48:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 457 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12398 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 49:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 458 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12411 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 50:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 466 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12424 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 51:
			{
			CSymbolNode_assignment_operator_expr* __pTarget = (CSymbolNode_assignment_operator_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 466 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12437 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 52:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 479 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12450 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 53:
			{
			CSymbolNode_postfix_expr* __pTarget = (CSymbolNode_postfix_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 546 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12463 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 54:
			{
			CSymbolNode_unary_operator_expr* __pTarget = (CSymbolNode_unary_operator_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 547 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12476 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 55:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 556 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12489 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 56:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 560 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12502 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 57:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 564 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12515 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 58:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 568 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12528 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 59:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 572 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12541 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 60:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 576 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12554 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 61:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 580 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12567 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 62:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 584 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12580 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 63:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 592 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12593 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 64:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 597 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12606 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 65:
			{
			CSymbolNode_new_operator_curly_initializer* __pTarget = (CSymbolNode_new_operator_curly_initializer*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 613 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12619 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 66:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 614 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12632 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 67:
			{
			CSymbolNode_type_name_impl* __pTarget = (CSymbolNode_type_name_impl*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
			
// #line 631 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.ppType = &(*__pAstNode).m_pType;
// #line 12645 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 631 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pCountValue =  &(*__pAstNode).m_ElementCountValue;
// #line 12649 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 68:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_cast_operator_rslv* __pSymbol = (CSymbolNode_cast_operator_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 660 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTypeSpecifier = &__pSymbol->m_Local.TypeSpecifier;
// #line 12662 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 69:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 664 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12675 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 70:
			{
			CSymbolNode_postfix_operator* __pTarget = (CSymbolNode_postfix_operator*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 665 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12688 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 71:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 700 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12701 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 72:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 705 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12714 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 73:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 732 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12727 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 74:
			{
			CSymbolNode_literal* __pTarget = (CSymbolNode_literal*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 733 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12740 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 75:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 784 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12753 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 76:
			{
			CSymbolNode_curly_initializer_item* __pTarget = (CSymbolNode_curly_initializer_item*) pSymbol;
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 839 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pInitializer = &__pSymbol->m_Local.Initializer;
// #line 12766 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 77:
			{
			CSymbolNode_curly_initializer_item* __pTarget = (CSymbolNode_curly_initializer_item*) pSymbol;
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 839 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pInitializer = &__pSymbol->m_Local.Initializer;
// #line 12779 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 78:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls23* __pAstNode = (_cls23*) __pSymbol->m_pAstNode;
			
// #line 17 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12792 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 79:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
			
// #line 26 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12805 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 80:
			{
			CSymbolNode_logical_or_expr_s* __pTarget = (CSymbolNode_logical_or_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 83 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12818 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 81:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 84 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.TrueValue;
// #line 12831 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 82:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 84 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.FalseValue;
// #line 12844 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 83:
			{
			CSymbolNode_logical_and_expr_s* __pTarget = (CSymbolNode_logical_and_expr_s*) pSymbol;
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 96 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12857 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 84:
			{
			CSymbolNode_logical_and_expr_s* __pTarget = (CSymbolNode_logical_and_expr_s*) pSymbol;
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 97 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12870 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 85:
			{
			CSymbolNode_inclusive_or_expr_s* __pTarget = (CSymbolNode_inclusive_or_expr_s*) pSymbol;
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 109 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12883 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 86:
			{
			CSymbolNode_inclusive_or_expr_s* __pTarget = (CSymbolNode_inclusive_or_expr_s*) pSymbol;
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 110 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12896 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 87:
			{
			CSymbolNode_exclusive_or_expr_s* __pTarget = (CSymbolNode_exclusive_or_expr_s*) pSymbol;
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 122 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12909 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 88:
			{
			CSymbolNode_exclusive_or_expr_s* __pTarget = (CSymbolNode_exclusive_or_expr_s*) pSymbol;
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 123 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12922 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 89:
			{
			CSymbolNode_and_expr_s* __pTarget = (CSymbolNode_and_expr_s*) pSymbol;
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 135 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12935 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 90:
			{
			CSymbolNode_and_expr_s* __pTarget = (CSymbolNode_and_expr_s*) pSymbol;
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 136 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12948 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 91:
			{
			CSymbolNode_equality_expr_s* __pTarget = (CSymbolNode_equality_expr_s*) pSymbol;
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 148 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12961 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 92:
			{
			CSymbolNode_equality_expr_s* __pTarget = (CSymbolNode_equality_expr_s*) pSymbol;
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 149 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12974 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 93:
			{
			CSymbolNode_relational_expr_s* __pTarget = (CSymbolNode_relational_expr_s*) pSymbol;
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 161 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12987 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 94:
			{
			CSymbolNode_relational_expr_s* __pTarget = (CSymbolNode_relational_expr_s*) pSymbol;
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 162 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13000 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 95:
			{
			CSymbolNode_shift_expr_s* __pTarget = (CSymbolNode_shift_expr_s*) pSymbol;
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 174 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13013 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 96:
			{
			CSymbolNode_shift_expr_s* __pTarget = (CSymbolNode_shift_expr_s*) pSymbol;
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 175 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13026 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 97:
			{
			CSymbolNode_additive_expr_s* __pTarget = (CSymbolNode_additive_expr_s*) pSymbol;
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 187 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13039 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 98:
			{
			CSymbolNode_additive_expr_s* __pTarget = (CSymbolNode_additive_expr_s*) pSymbol;
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 188 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13052 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 99:
			{
			CSymbolNode_multiplicative_expr_s* __pTarget = (CSymbolNode_multiplicative_expr_s*) pSymbol;
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 200 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13065 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 100:
			{
			CSymbolNode_multiplicative_expr_s* __pTarget = (CSymbolNode_multiplicative_expr_s*) pSymbol;
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 201 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13078 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 101:
			{
			CSymbolNode_at_expr_s* __pTarget = (CSymbolNode_at_expr_s*) pSymbol;
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 213 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13091 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 102:
			{
			CSymbolNode_at_expr_s* __pTarget = (CSymbolNode_at_expr_s*) pSymbol;
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 214 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13104 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 103:
			{
			CSymbolNode_assignment_expr_s* __pTarget = (CSymbolNode_assignment_expr_s*) pSymbol;
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 226 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13117 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 104:
			{
			CSymbolNode_assignment_expr_s* __pTarget = (CSymbolNode_assignment_expr_s*) pSymbol;
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 227 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13130 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 105:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 239 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13143 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 106:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 245 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13156 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 107:
			{
			CSymbolNode_postfix_expr_s* __pTarget = (CSymbolNode_postfix_expr_s*) pSymbol;
			CSymbolNode_unary_expr_s* __pSymbol = (CSymbolNode_unary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 253 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13169 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 108:
			{
			CSymbolNode_unary_operator_expr_s* __pTarget = (CSymbolNode_unary_operator_expr_s*) pSymbol;
			CSymbolNode_unary_expr_s* __pSymbol = (CSymbolNode_unary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 254 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13182 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 109:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 263 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13195 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 110:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 267 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13208 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 111:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 271 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13221 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 112:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 275 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13234 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 113:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 279 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13247 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 114:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 283 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13260 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 115:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 287 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13273 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 116:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 291 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13286 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 117:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 295 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13299 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 118:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 315 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13312 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 119:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 320 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13325 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 120:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 329 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13338 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 121:
			{
			CSymbolNode_primary_expr_s* __pTarget = (CSymbolNode_primary_expr_s*) pSymbol;
			CSymbolNode_postfix_expr_s* __pSymbol = (CSymbolNode_postfix_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 352 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13351 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 122:
			{
			CSymbolNode_postfix_operator_s* __pTarget = (CSymbolNode_postfix_operator_s*) pSymbol;
			CSymbolNode_postfix_expr_s* __pSymbol = (CSymbolNode_postfix_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 353 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13364 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 123:
			{
			CSymbolNode_member_operator_s* __pTarget = (CSymbolNode_member_operator_s*) pSymbol;
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 388 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13377 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 124:
			{
			CSymbolNode_member_operator_s* __pTarget = (CSymbolNode_member_operator_s*) pSymbol;
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 393 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13390 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 125:
			{
			CSymbolNode_literal_s* __pTarget = (CSymbolNode_literal_s*) pSymbol;
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 449 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13403 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 126:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 450 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13416 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 127:
			{
			CSymbolNode_switch_block_stmt* __pTarget = (CSymbolNode_switch_block_stmt*) pSymbol;
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 139 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pStmt = &__pSymbol->m_Local.Stmt;
// #line 13429 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 128:
			{
			CSymbolNode_declaration_specifier* __pTarget = (CSymbolNode_declaration_specifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
			
// #line 16 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
// #line 13442 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 129:
			{
			CSymbolNode_pitcher_modifier* __pTarget = (CSymbolNode_pitcher_modifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 37 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13455 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 130:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
			
// #line 55 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
// #line 13468 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 131:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
			
// #line 273 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pNamespace = __pSymbol->m_Local.pTemplate;
// #line 13481 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 132:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 4 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13494 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 133:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 4 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13507 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 134:
			{
			CSymbolNode_declarator* __pTarget = (CSymbolNode_declarator*) pSymbol;
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 12 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13520 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 135:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 39 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13533 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 136:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 40 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13546 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 137:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 41 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13559 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 138:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 42 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13572 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 139:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 48 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13585 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 140:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 52 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13598 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 141:
			{
			CSymbolNode_array_suffix* __pTarget = (CSymbolNode_array_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 302 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13611 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 142:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 303 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13624 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 143:
			{
			CSymbolNode_bitfield_suffix* __pTarget = (CSymbolNode_bitfield_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 305 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13637 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 144:
			{
			CSymbolNode_function_formal_argument_list* __pTarget = (CSymbolNode_function_formal_argument_list*) pSymbol;
			CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 342 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Local.pFunctionSuffix;
// #line 13650 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 145:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 347 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13663 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 146:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 348 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13676 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 147:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pTarget = (CSymbolNode_function_formal_argument_w_specifier*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 352 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13689 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 148:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pTarget = (CSymbolNode_function_formal_argument_no_specifier*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 353 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13702 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 149:
			{
			CSymbolNode_function_formal_argument_ellipsis* __pTarget = (CSymbolNode_function_formal_argument_ellipsis*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 354 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13715 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 150:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 370 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13728 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 151:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 371 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13741 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 152:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 372 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13754 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 153:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 393 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13767 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 154:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 394 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13780 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 155:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 395 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13793 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 156:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
			
// #line 56 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
// #line 13806 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 157:
			{
			CSymbolNode_enum_member_list* __pTarget = (CSymbolNode_enum_member_list*) pSymbol;
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 91 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13819 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 158:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 100 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13832 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 159:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 100 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13845 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 160:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
			
// #line 143 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13858 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 161:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
			
// #line 190 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13871 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 162:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
			
// #line 216 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13884 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
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
// #line 13919 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 13935 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 13950 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 13964 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 9 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13982 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 14000 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
// #line 48 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 14019 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_expression:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
// #line 97 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		m_pModule->m_NamespaceMgr.SetSourcePos (m_CurrentToken.m_Pos);
	;
// #line 14044 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 14083 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_curly_initializer:
			{
				CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 832 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		ASSERT (m_CurlyInitializerTargetValue);

		__pSymbol->m_Local.Initializer.m_TargetValue = m_CurlyInitializerTargetValue;
		__pSymbol->m_Local.Initializer.m_Index = 0;
		__pSymbol->m_Local.Initializer.m_Count = 0;
	;
// #line 14101 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_new_operator_curly_initializer:
			{
				CSymbolNode_new_operator_curly_initializer* __pSymbol = (CSymbolNode_new_operator_curly_initializer*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 646 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		m_CurlyInitializerTargetValue = *__pSymbol->m_Arg.pValue;
	;
// #line 14119 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_if_stmt:
			{
				CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 104 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.IfStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14171 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_switch_stmt:
			{
				CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 130 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.SwitchStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14184 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_while_stmt:
			{
				CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 163 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.WhileStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14197 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_do_stmt:
			{
				CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 182 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.DoStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14210 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_for_stmt:
			{
				CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 205 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.ForStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14223 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		case ESymbol_reactor_onchange_stmt:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 328 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_ReactorBindSiteList.Clear (); // clear bindsite list before processing expression list
	;
// #line 14244 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_reactor_stmt_0:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 81 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
				
		m_ReactorBindSiteCount = 0;
	;
// #line 14273 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		case ESymbol_named_type_block:
			{
				CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 225 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pNamespace);
	;
// #line 14294 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_full_declarator:
			{
				CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 9 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		return PreDeclare ();
	;
// #line 14307 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_declarator:
			{
				CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
				_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 36 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		(*__pAstNode).m_Declarator.SetTypeSpecifier (__pSymbol->m_Arg.pTypeSpecifier);
	;
// #line 14320 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 14335 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_function_formal_argument_w_specifier:
			{
				CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 363 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = EStorage_Undefined;
	;
// #line 14352 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_function_formal_argument_no_specifier:
			{
				CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 389 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = EStorage_Undefined;
		__pSymbol->m_Local.Declarator.SetTypeSpecifier (NULL);
	;
// #line 14366 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 728 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
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
// #line 670 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
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
// #line 14603 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 14618 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 14632 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_named_type_block:
			{
				CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 229 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		m_pModule->m_NamespaceMgr.CloseNamespace ();
	;
// #line 14692 "/home/vladimir/Projects/ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
					
				pTransition->m_ProductionIndex = 1727;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1729;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1733;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1735;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
				pTransition->m_ResolverIndex = 995;
				pTransition->m_ResolverElseIndex = 1739;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 13:
			
			pTransition->m_ProductionIndex = 165;
			pTransition->m_ResolverIndex = 313;
			pTransition->m_ResolverElseIndex = 170;
				
			return ELaDfaResult_Resolver;
			
		
		case 14:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1741;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1743;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
				pTransition->m_ResolverIndex = 995;
				pTransition->m_ResolverElseIndex = 1745;
						
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1747;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1749;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1751;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 1753;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1755;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1757;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1759;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1761;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1763;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
				pTransition->m_ResolverIndex = 995;
				pTransition->m_ResolverElseIndex = 170;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 39:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1766;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 40:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 41:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1768;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 42:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 43:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1770;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 44:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 45:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1772;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 46:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 47:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1774;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 48:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 49:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1776;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 50:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 51:
			
			switch (LookaheadToken)
			{
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 1778;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 52:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 53:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1780;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 54:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 55:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1782;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 56:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 57:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1784;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 58:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 59:
			
			switch (LookaheadToken)
			{
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1786;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 60:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 61:
			
			switch (LookaheadToken)
			{
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1788;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 62:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 63:
			
			switch (LookaheadToken)
			{
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 1790;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 64:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 284;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_RetVal:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_CountOf:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_OffsetOf:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_BindingOf:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_HexLiteral:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_FmtLiteral:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 274;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 65:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 1792;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 950;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 524;
				pTransition->m_ResolverIndex = 313;
				pTransition->m_ResolverElseIndex = 525;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 68:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 611;
				pTransition->m_ResolverIndex = 232;
				pTransition->m_ResolverElseIndex = 610;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 69:
			
			switch (LookaheadToken)
			{
			
			case '=':
					
				pTransition->m_ProductionIndex = 1796;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 70:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 597;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_RetVal:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_CountOf:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_OffsetOf:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_BindingOf:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_HexLiteral:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_FmtLiteral:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 71:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 626;
				pTransition->m_ResolverIndex = 236;
				pTransition->m_ResolverElseIndex = 625;
						
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
				pTransition->m_ResolverIndex = 1121;
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
					
				pTransition->m_ProductionIndex = 1804;
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
					
				pTransition->m_ProductionIndex = 1806;
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
					
				pTransition->m_ProductionIndex = 1808;
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
					
				pTransition->m_ProductionIndex = 1810;
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 900;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
				pTransition->m_ProductionIndex = 1812;
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
					
				pTransition->m_ProductionIndex = 1814;
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
					
			
			case EToken_Silent:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Return:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
					
			
			case EToken_EnumF:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
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
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 1816;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 90:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case ',':
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Pitcher:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 954;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 956;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 91:
			
			switch (LookaheadToken)
			{
			
			case ':':
					
				pTransition->m_ProductionIndex = 520;
				pTransition->m_ResolverIndex = 1356;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 92:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 756;
				pTransition->m_ResolverIndex = 272;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 93:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				pTransition->m_ResolverIndex = 313;
				pTransition->m_ResolverElseIndex = 774;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 94:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1821;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1823;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1825;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1827;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1829;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1831;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 1833;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1835;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1837;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1839;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1841;
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1843;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 117:
			
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
					
			
			case EToken_HeapU:
					
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
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 165;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
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
					
			
			case EToken_EventD:
					
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
			
		
		case 118:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 813;
				pTransition->m_ResolverIndex = 313;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 119:
			
			switch (LookaheadToken)
			{
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 288;
				pTransition->m_ResolverIndex = 1228;
				pTransition->m_ResolverElseIndex = 289;
						
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
