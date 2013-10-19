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
	ESymbol_unwinder_modifier = 156, 
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

		TokenCount         = 154,
		SymbolCount        = 333,
		SequenceCount      = 504,
		ActionCount        = 389,
		ArgumentCount      = 163,
		BeaconCount        = 178,
		LaDfaCount         = 120,

		TotalCount         = 1841,

		NamedSymbolCount   = 181,

		TokenFirst         = 0,
		TokenEnd           = 154,
		SymbolFirst        = 154,
		NamedSymbolEnd     = 335,
		SymbolEnd          = 487,
		SequenceFirst      = 487,
		SequenceEnd        = 991,
		ActionFirst        = 991,
		ActionEnd          = 1380,
		ArgumentFirst      = 1380,
		ArgumentEnd        = 1543,
		BeaconFirst        = 1543,
		BeaconEnd          = 1721,
		LaDfaFirst         = 1721,
		LaDfaEnd           = 1841,
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

	// saved attributes

	CValue m_ExpressionValue;
	rtl::CBoxListT <CValue> m_ExpressionValueList;


	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class _cls1: public CAstNode
	{
	public:
// #line 98 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CQualifiedName m_Name
;
// #line 338 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls2: public CAstNode
	{
	public:
// #line 193 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 348 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls3: public CAstNode
	{
	public:
// #line 201 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CToken* m_pAnchorToken;
;
// #line 358 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls4: public CAstNode
	{
	public:
// #line 261 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
// #line 368 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls5: public CAstNode
	{
	public:
// #line 269 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	rtl::CBoxListT <CType*> m_TypeList;
;
// #line 378 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls6: public CAstNode
	{
	public:
// #line 285 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
// #line 388 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls7: public CAstNode
	{
	public:
// #line 4 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 398 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls8: public CAstNode
	{
	public:
// #line 51 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 408 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls9: public CAstNode
	{
	public:
// #line 91 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
// #line 418 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls10: public CAstNode
	{
	public:
// #line 104 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;
;
// #line 428 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls11: public CAstNode
	{
	public:
// #line 114 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 438 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls12: public CAstNode
	{
	public:
// #line 133 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 448 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls13: public CAstNode
	{
	public:
// #line 159 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
// #line 458 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls14: public CAstNode
	{
	public:
// #line 175 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	intptr_t m_Value;
;
// #line 468 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls15: public CAstNode
	{
	public:
// #line 200 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_TrueValue;
	CValue m_FalseValue;
	CBasicBlock* m_pThenBlock;
	CBasicBlock* m_pElseBlock;
	CBasicBlock* m_pPhiBlock;
;
// #line 482 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls16: public CAstNode
	{
	public:
// #line 314 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 492 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls17: public CAstNode
	{
	public:
// #line 342 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 502 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls18: public CAstNode
	{
	public:
// #line 378 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 512 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls19: public CAstNode
	{
	public:
// #line 406 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 522 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls20: public CAstNode
	{
	public:
// #line 434 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 532 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls21: public CAstNode
	{
	public:
// #line 491 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 542 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls22: public CAstNode
	{
	public:
// #line 625 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CType* m_pType;
	CValue m_ElementCountValue;
	rtl::CBoxListT <CValue> m_ArgList;
;
// #line 554 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls23: public CAstNode
	{
	public:
// #line 12 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	CValue m_Value;	
;
// #line 564 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls24: public CAstNode
	{
	public:
// #line 21 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	CValue m_Value;
;
// #line 574 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls25: public CAstNode
	{
	public:
// #line 31 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 584 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls26: public CAstNode
	{
	public:
// #line 50 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 594 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls27: public CAstNode
	{
	public:
// #line 336 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	CType* m_pType;
;
// #line 604 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls28: public CAstNode
	{
	public:
// #line 57 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
		
	CQualifiedName m_Name;
;
// #line 614 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls29: public CAstNode
	{
	public:
// #line 4 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CTypeSpecifier m_TypeSpecifier;
;
// #line 624 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls30: public CAstNode
	{
	public:
// #line 43 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CTypeSpecifier m_TypeSpecifier;
;
// #line 634 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls31: public CAstNode
	{
	public:
// #line 72 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EStorage m_StorageKind;
;
// #line 644 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls32: public CAstNode
	{
	public:
// #line 129 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EAccess m_AccessKind;
;
// #line 654 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls33: public CAstNode
	{
	public:
// #line 146 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
// #line 664 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls34: public CAstNode
	{
	public:
// #line 243 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
// #line 674 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls35: public CAstNode
	{
	public:
// #line 261 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CPropertyType* m_pType;
;
// #line 684 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls36: public CAstNode
	{
	public:
// #line 283 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	ETypeModifier m_TypeModifier;
;
// #line 694 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls37: public CAstNode
	{
	public:
// #line 31 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
		
	CDeclarator m_Declarator;
;
// #line 704 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls38: public CAstNode
	{
	public:
// #line 105 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
		
	EUnOp m_UnOpKind;
	EBinOp m_BinOpKind;
;
// #line 715 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls39: public CAstNode
	{
	public:
// #line 6 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CType* m_pType;
;
// #line 725 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls40: public CAstNode
	{
	public:
// #line 33 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CEnumType* m_pType;
;
// #line 735 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls41: public CAstNode
	{
	public:
// #line 62 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	EEnumType m_EnumTypeKind;
	uint_t m_Flags;
;
// #line 746 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls42: public CAstNode
	{
	public:
// #line 125 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CStructType* m_pType;
;
// #line 756 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls43: public CAstNode
	{
	public:
// #line 149 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_PackFactor;
;
// #line 766 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls44: public CAstNode
	{
	public:
// #line 160 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_Value;
;
// #line 776 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls45: public CAstNode
	{
	public:
// #line 179 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CUnionType* m_pType;
;
// #line 786 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls46: public CAstNode
	{
	public:
// #line 198 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CClassType* m_pType;
;
// #line 796 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_item_declaration_no_specifier_rslv: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 163 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
		CDeclarator Declarator;
	;
// #line 824 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 136 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
		CDeclarator Declarator;
	;
// #line 846 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 105 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			CQualifiedName* pName;
// #line 875 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 3 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 896 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 171 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 916 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 286 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 938 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 47 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 958 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 327 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 978 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 329 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclFunctionSuffix* pFunctionSuffix;
		EStorage PrevStorageKind;
	;
// #line 988 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 301 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 1012 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 426 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 1032 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 204 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 1053 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 218 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			rtl::CBoxListT <CToken>* pTokenList; CToken* pAnchorToken;
// #line 1073 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 240 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			CType** ppType; CValue* pCountValue;
// #line 1093 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 242 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		CDeclarator Declarator;
	;
// #line 1102 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 12 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 1131 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 59 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 1152 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 207 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1176 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 235 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1200 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 237 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1209 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 248 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1229 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 250 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1238 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 261 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1258 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 263 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1267 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 274 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1287 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 276 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1296 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 287 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1316 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 289 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1325 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 300 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1345 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 302 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1354 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 328 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1374 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 330 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1383 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 364 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1404 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 366 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1413 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 392 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1434 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 394 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1443 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 420 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1464 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 422 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1473 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 452 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1494 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 454 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1503 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 465 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1524 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 545 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1544 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 469 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1564 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 471 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1573 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 824 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		TCurlyInitializer Initializer;
	;
// #line 1598 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 663 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1620 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 550 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1640 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 657 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CTypeSpecifier TypeSpecifier;
	;
// #line 1664 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 644 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1686 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 58 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 1710 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 726 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1730 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 668 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1752 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 708 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1774 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 788 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1794 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 790 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		TLiteral Literal;
	;
// #line 1803 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 846 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			TCurlyInitializer* pInitializer;
// #line 1823 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 848 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue PrevCurlyInitializerTargetValue;
	;
// #line 1832 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 77 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1853 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 79 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue TrueValue;
		CValue FalseValue;
	;
// #line 1863 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 91 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1886 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 93 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1895 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 104 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1915 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 106 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1924 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 117 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1944 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 119 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1953 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 130 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1973 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 132 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1982 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 143 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2002 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 145 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2011 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 156 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2031 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 158 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2040 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 169 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2060 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 171 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2069 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 182 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2089 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 184 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2098 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 195 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2118 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 197 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2127 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 208 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2147 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 210 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2156 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 221 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2176 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 223 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2185 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 234 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2205 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 236 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2214 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 252 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2234 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 351 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2254 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 257 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2274 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 411 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2297 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 356 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2319 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 396 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2341 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 449 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2361 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 451 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		TLiteral Literal;
	;
// #line 2370 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 100 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TIfStmt Stmt;
	;
// #line 2398 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 126 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TSwitchStmt Stmt;
	;
// #line 2422 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 159 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TWhileStmt Stmt;
	;
// #line 2446 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 178 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TDoStmt Stmt;
	;
// #line 2470 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 201 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TForStmt Stmt;
	;
// #line 2494 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 263 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TOnceStmt Stmt;
	;
// #line 2521 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 145 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			TSwitchStmt* pStmt;
// #line 2542 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 19 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2584 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_declaration_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declaration_specifier;
			
		}
	};
		
	
	
	
	class CSymbolNode_unwinder_modifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 371 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2606 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_unwinder_modifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_unwinder_modifier;
			
		}
	};
		
	
	class CSymbolNode_property_template_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 266 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
		CPropertyTemplate* pTemplate;
	;
// #line 2628 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 223 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CNamespace* pNamespace;
// #line 2649 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 7 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2673 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 34 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2695 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 55 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2717 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 310 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2738 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 417 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2758 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 346 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2778 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 351 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2798 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 357 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2818 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 359 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclarator Declarator;
	;
// #line 2827 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 383 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2849 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 385 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclarator Declarator;
	;
// #line 2858 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 406 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2880 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 86 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2905 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 99 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2925 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 104 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2945 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
/*
	bool
	DeclareAutoGetValue (
		CType* pType,
		CDeclarator* pDeclarator,
		uint_t PtrTypeFlags
		);

	bool
	GetAutoGetValue (CValue* pValue);

	bool
	GetAutoGetValueType (CValue* pValue);

	bool
	GetOnChange (CValue* pValue);

	bool
	GetOnChangeType (CValue* pValue);
*/
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
			0, -1, -1, -1, -1, 487, -1, -1, -1, -1, -1, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, -1, -1, -1, -1, -1, -1, 487, 487, 487, 487, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, 487, -1, 487, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, -1, -1, -1, 487, 487, 487, 487, 487, 487, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, 164, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 336, -1, -1, -1, -1, -1, -1, 158, 159, 160, 161, 1725, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 336, 164, -1, 336, 336, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 164, 164, 1726, 164, 164, 164, 164, 164, 164, 164, 164, 164, 1721, 1723, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 336, 336, 336, -1, -1, -1, 163, 163, 163, 163, 163, 163, 
			-1, -1, 488, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 984, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 985, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 489, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 495, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 497, 497, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 967, 967, 967, 968, 969, 970, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 507, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 507, -1, -1, 507, 507, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 507, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 507, 507, 507, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 504, -1, -1, 504, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 504, 504, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, 164, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 337, -1, -1, -1, -1, -1, -1, -1, -1, -1, 161, 1731, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 337, 164, -1, 337, 337, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 164, 164, 1732, 164, 164, 164, 164, 164, 164, 164, 164, 164, 1727, 1729, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 337, 337, 337, -1, -1, -1, 163, 163, 163, 163, 163, 163, 
			-1, -1, -1, 169, -1, 169, 169, -1, -1, -1, -1, 164, -1, -1, -1, -1, -1, 169, -1, -1, 169, 169, 1759, -1, -1, -1, 169, 169, -1, -1, -1, 160, -1, 1733, 170, 171, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 169, 169, 169, 169, 169, 338, 164, -1, 338, 338, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, -1, -1, 169, -1, 169, -1, -1, 169, 169, 169, 169, 169, 169, 169, 169, -1, 1735, 1737, 1739, 1741, 1743, 1745, 1747, 1749, 1751, 1753, 1755, 1757, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 338, 338, 338, -1, -1, -1, 163, 163, 163, 163, 163, 163, 
			-1, -1, -1, 273, -1, 5, 268, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 273, -1, -1, 273, 273, 273, -1, -1, -1, 273, 273, -1, -1, -1, -1, -1, 273, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 273, 273, 1784, 273, 273, -1, -1, -1, -1, -1, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, -1, -1, 274, -1, 275, -1, -1, 276, 277, 278, 279, 280, 281, 281, 282, -1, 1760, 1762, 1764, 1766, 1768, 1770, 1772, 1774, 1776, 1778, 1780, 1782, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 780, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 781, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 498, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 832, 833, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 499, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 809, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 809, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 809, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, 809, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 877, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 877, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 877, -1, -1, 877, 877, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 877, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 877, 877, 877, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 508, 509, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 939, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, 886, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 886, 886, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1786, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 942, -1, -1, -1, -1, -1, 464, -1, 941, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 966, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 510, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 511, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 516, 516, 516, 516, 516, 513, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 514, 515, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 517, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 517, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 816, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 816, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, 816, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, -1, -1, 524, 524, 524, -1, -1, -1, 524, 524, -1, -1, -1, -1, -1, 1788, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, 524, 524, 524, 524, -1, 523, -1, -1, -1, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, -1, -1, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 668, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 668, -1, -1, 668, 668, 668, -1, -1, -1, 668, 668, -1, -1, -1, -1, -1, 668, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 668, 668, 668, 668, 668, -1, -1, -1, -1, -1, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 525, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 527, -1, -1, -1, 527, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 527, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 529, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 
			-1, 538, 538, 536, 538, 538, 532, 538, 538, 538, 538, 534, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 
			-1, -1, -1, 540, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 543, 543, 542, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 543, 
			-1, -1, -1, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, -1, -1, 544, 544, 544, -1, -1, -1, 544, 544, -1, -1, -1, -1, -1, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, 544, 544, 544, 544, -1, -1, -1, -1, -1, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, -1, -1, 546, 546, 546, -1, -1, -1, 546, 546, -1, -1, -1, -1, -1, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, 546, 546, 546, 546, -1, -1, -1, -1, -1, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			545, -1, -1, 545, -1, -1, -1, -1, 545, -1, -1, -1, -1, -1, -1, -1, -1, 545, -1, -1, 545, 545, 545, -1, -1, -1, 545, 545, -1, -1, -1, -1, -1, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 545, 545, 545, 545, 545, -1, -1, -1, -1, -1, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 550, 0, 0, -1, -1, 553, -1, -1, -1, -1, -1, -1, -1, -1, 550, -1, -1, 550, 550, 550, -1, -1, -1, 550, 550, -1, -1, -1, -1, -1, 550, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 550, 550, 550, 550, 550, -1, -1, -1, -1, -1, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 556, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 556, -1, -1, 556, 556, 556, -1, -1, -1, 556, 556, -1, -1, -1, -1, -1, 556, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 556, 556, 556, 556, 556, -1, -1, -1, -1, -1, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 547, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 547, -1, -1, 547, 547, 547, -1, -1, -1, 547, 547, -1, -1, -1, -1, -1, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, -1, -1, 548, 548, 548, -1, -1, -1, 548, 548, -1, -1, -1, -1, -1, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, 548, 548, 548, 548, -1, -1, -1, -1, -1, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, -1, -1, 554, 554, 554, -1, -1, -1, 554, 554, -1, -1, -1, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, -1, -1, 555, 555, 555, -1, -1, -1, 555, 555, -1, -1, -1, -1, -1, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 558, -1, -1, 558, 558, 558, -1, -1, -1, 558, 558, -1, -1, -1, -1, -1, 558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 558, 558, 558, 558, 558, -1, -1, -1, -1, -1, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 560, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 560, -1, -1, 560, 560, 560, -1, -1, -1, 560, 560, -1, -1, -1, -1, -1, 560, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 560, 560, 560, 560, 560, -1, -1, -1, -1, -1, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 562, -1, -1, 562, 562, 562, -1, -1, -1, 562, 562, -1, -1, -1, -1, -1, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 562, 562, 562, 562, 562, -1, -1, -1, -1, -1, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 564, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 564, -1, -1, 564, 564, 564, -1, -1, -1, 564, 564, -1, -1, -1, -1, -1, 564, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 564, 564, 564, 564, 564, -1, -1, -1, -1, -1, 564, 564, 564, 564, 564, 564, 564, 564, 564, 564, 564, 564, 564, 564, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 564, 564, 564, 564, 564, 564, 564, 564, 564, 564, 564, 564, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 566, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 566, -1, -1, 566, 566, 566, -1, -1, -1, 566, 566, -1, -1, -1, -1, -1, 566, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 566, 566, 566, 566, 566, -1, -1, -1, -1, -1, 566, 566, 566, 566, 566, 566, 566, 566, 566, 566, 566, 566, 566, 566, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 566, 566, 566, 566, 566, 566, 566, 566, 566, 566, 566, 566, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 568, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 568, -1, -1, 568, 568, 568, -1, -1, -1, 568, 568, -1, -1, -1, -1, -1, 568, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 568, 568, 568, 568, 568, -1, -1, -1, -1, -1, 568, 568, 568, 568, 568, 568, 568, 568, 568, 568, 568, 568, 568, 568, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 568, 568, 568, 568, 568, 568, 568, 568, 568, 568, 568, 568, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 572, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 572, -1, -1, 572, 572, 572, -1, -1, -1, 572, 572, -1, -1, -1, -1, -1, 572, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 572, 572, 572, 572, 572, -1, -1, -1, -1, -1, 572, 572, 572, 572, 572, 572, 572, 572, 572, 572, 572, 572, 572, 572, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 572, 572, 572, 572, 572, 572, 572, 572, 572, 572, 572, 572, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 570, 571, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 578, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 578, -1, -1, 578, 578, 578, -1, -1, -1, 578, 578, -1, -1, -1, -1, -1, 578, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 578, 578, 578, 578, 578, -1, -1, -1, -1, -1, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 574, 575, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 576, 577, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 582, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 582, -1, -1, 582, 582, 582, -1, -1, -1, 582, 582, -1, -1, -1, -1, -1, 582, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 582, 582, 582, 582, 582, -1, -1, -1, -1, -1, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, 582, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 580, 581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 586, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 586, -1, -1, 586, 586, 586, -1, -1, -1, 586, 586, -1, -1, -1, -1, -1, 586, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 586, 586, 586, 586, 586, -1, -1, -1, -1, -1, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 584, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 591, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 591, -1, -1, 591, 591, 591, -1, -1, -1, 591, 591, -1, -1, -1, -1, -1, 591, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 591, 591, 591, 591, 591, -1, -1, -1, -1, -1, 591, 591, 591, 591, 591, 591, 591, 591, 591, 591, 591, 591, 591, 591, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 591, 591, 591, 591, 591, 591, 591, 591, 591, 591, 591, 591, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 588, 589, 590, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 593, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 593, -1, -1, 593, 593, 593, -1, -1, -1, 593, 593, -1, -1, -1, -1, -1, 593, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 593, 593, 593, 593, 593, -1, -1, -1, -1, -1, 593, 593, 593, 593, 593, 593, 593, 593, 593, 593, 593, 593, 593, 593, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 593, 593, 593, 593, 593, 593, 593, 593, 593, 593, 593, 593, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1789, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 610, -1, -1, 610, 610, 610, -1, -1, -1, 610, 610, -1, -1, -1, -1, -1, 609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 610, 610, 610, 610, 610, -1, -1, -1, -1, -1, 609, 609, 609, 609, 609, 609, 609, 609, 609, 609, 609, 609, 609, 609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 610, 610, 610, 610, 610, 610, 610, 610, 610, 610, 610, 610, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1790, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 595, 595, 595, 595, 595, 595, 595, 595, 595, 595, 595, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 597, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 660, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 628, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 628, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 377, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 614, -1, -1, 611, 612, 615, -1, -1, -1, 613, 616, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 617, 618, 619, 621, 623, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 621, 621, 621, 621, 621, 621, 621, 621, 621, 621, 621, 621, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 627, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1792, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 624, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 228, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 626, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 854, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 818, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 818, 818, 818, 818, 818, 818, 818, 818, 818, 818, 818, 818, 818, 818, 1793, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, 819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 641, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 643, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654, 642, 642, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 630, -1, -1, -1, -1, -1, -1, 636, 632, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 631, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 633, 634, -1, -1, -1, -1, -1, 637, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 638, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 639, 640, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 658, 658, 658, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 664, -1, -1, 665, 666, 666, -1, -1, -1, -1, -1, -1, -1, -1, 664, -1, -1, 664, 664, 664, -1, -1, -1, 664, 664, -1, -1, -1, -1, -1, 1794, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 664, 664, 664, 664, 664, -1, -1, -1, -1, -1, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 667, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 667, -1, -1, 667, 667, 667, -1, -1, -1, 667, 667, -1, -1, -1, -1, -1, 667, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 667, 667, 667, 667, 667, -1, -1, -1, -1, -1, 667, 667, 667, 667, 667, 667, 667, 667, 667, 667, 667, 667, 667, 667, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 667, 667, 667, 667, 667, 667, 667, 667, 667, 667, 667, 667, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 676, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 676, -1, -1, 676, 676, 676, -1, -1, -1, 676, 676, -1, -1, -1, -1, -1, 676, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 676, 676, 676, 676, 676, -1, -1, -1, -1, -1, 676, 676, 676, 676, 676, 676, 676, 676, 676, 676, 676, 676, 676, 676, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 676, 676, 676, 676, 676, 676, 676, 676, 676, 676, 676, 676, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 669, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 669, -1, -1, 669, 669, 669, -1, -1, -1, 669, 669, -1, -1, -1, -1, -1, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 669, 669, 669, 669, 669, -1, -1, -1, -1, -1, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, -1, -1, 670, 670, 670, -1, -1, -1, 670, 670, -1, -1, -1, -1, -1, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 672, 0, -1, -1, -1, 675, -1, -1, -1, -1, -1, -1, -1, -1, 672, -1, -1, 672, 672, 672, -1, -1, -1, 672, 672, -1, -1, -1, -1, -1, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 678, -1, -1, 678, 678, 678, -1, -1, -1, 678, 678, -1, -1, -1, -1, -1, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 678, 678, 678, 678, 678, -1, -1, -1, -1, -1, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, 678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 680, -1, -1, 680, 680, 680, -1, -1, -1, 680, 680, -1, -1, -1, -1, -1, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 680, 680, 680, 680, 680, -1, -1, -1, -1, -1, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, -1, -1, 682, 682, 682, -1, -1, -1, 682, 682, -1, -1, -1, -1, -1, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, 682, 682, 682, 682, -1, -1, -1, -1, -1, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, 682, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 684, -1, -1, 684, 684, 684, -1, -1, -1, 684, 684, -1, -1, -1, -1, -1, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 684, 684, 684, 684, 684, -1, -1, -1, -1, -1, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, -1, -1, 686, 686, 686, -1, -1, -1, 686, 686, -1, -1, -1, -1, -1, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, 686, 686, 686, 686, -1, -1, -1, -1, -1, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 688, -1, -1, 688, 688, 688, -1, -1, -1, 688, 688, -1, -1, -1, -1, -1, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 688, 688, 688, 688, 688, -1, -1, -1, -1, -1, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 690, -1, -1, 690, 690, 690, -1, -1, -1, 690, 690, -1, -1, -1, -1, -1, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 690, 690, 690, 690, 690, -1, -1, -1, -1, -1, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 692, -1, -1, 692, 692, 692, -1, -1, -1, 692, 692, -1, -1, -1, -1, -1, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 692, 692, 692, 692, 692, -1, -1, -1, -1, -1, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, -1, -1, 694, 694, 694, -1, -1, -1, 694, 694, -1, -1, -1, -1, -1, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, 694, 694, 694, 694, -1, -1, -1, -1, -1, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, -1, -1, 696, 696, 696, -1, -1, -1, 696, 696, -1, -1, -1, -1, -1, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, 696, 696, 696, 696, -1, -1, -1, -1, -1, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, -1, -1, 698, 698, 698, -1, -1, -1, 698, 698, -1, -1, -1, -1, -1, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, 698, 698, 698, 698, -1, -1, -1, -1, -1, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 700, -1, -1, 700, 700, 700, -1, -1, -1, 700, 700, -1, -1, -1, -1, -1, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 700, 700, 700, 700, 700, -1, -1, -1, -1, -1, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1795, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 703, -1, -1, 703, 703, 703, -1, -1, -1, 703, 703, -1, -1, -1, -1, -1, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 703, 703, 703, 703, 703, -1, -1, -1, -1, -1, 702, 702, 702, 702, 702, 702, 703, 703, 703, 703, 703, 702, 702, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 703, 703, 703, 703, 703, 703, 703, 703, 703, 703, 703, 703, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 723, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 723, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 723, 723, 723, 723, 723, 723, -1, -1, -1, -1, -1, 723, 723, 723, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 409, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 707, -1, -1, 704, 705, 708, -1, -1, -1, 706, 709, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 710, 711, 712, 719, 720, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 713, 714, 715, 716, 717, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 719, 719, 719, 719, 719, 719, 719, 719, 719, 719, 719, 719, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1796, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 721, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, 721, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 744, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 736, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 737, 738, 739, 740, 741, 742, -1, -1, -1, -1, -1, 743, 743, 743, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 725, -1, -1, -1, -1, -1, -1, 731, 727, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 726, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 728, 729, -1, -1, -1, -1, -1, 732, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 733, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 734, 735, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 748, 748, 748, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 750, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 752, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 421, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 756, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 757, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 759, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 761, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 761, -1, -1, 761, 761, 761, -1, -1, -1, 761, 761, -1, -1, -1, -1, -1, 761, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 761, 761, 761, 761, 761, -1, -1, -1, -1, -1, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 762, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 764, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 768, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 769, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 770, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 774, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 776, 776, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 777, -1, 777, 777, 777, 777, 777, 777, 777, 777, 777, 777, 777, 777, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 778, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 168, -1, 168, 168, -1, -1, -1, -1, 168, -1, -1, -1, -1, -1, 168, -1, -1, 168, 168, 168, -1, -1, -1, 168, 168, -1, -1, -1, 168, -1, 168, 168, 168, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 168, 168, 168, 168, 168, 168, 168, -1, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, -1, -1, 168, -1, 168, 766, 767, 168, 168, 168, 168, 168, 168, 168, 168, -1, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, 168, -1, -1, -1, 168, 168, 168, 168, 168, 168, 
			-1, -1, -1, -1, -1, -1, 783, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 288, 288, 288, 288, 5, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 1840, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 784, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 785, 
			-1, 291, 291, 291, 291, 5, 290, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 292, 291, 293, 291, 291, 294, 295, 296, 297, 298, 299, 299, 300, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 291, 
			-1, -1, -1, -1, -1, -1, 787, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 788, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 795, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 797, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 798, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 799, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 800, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 801, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 802, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 803, 803, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 804, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 789, 
			-1, 1, 1, 794, 1, 1, 1, 1, 1, 1, 1, 792, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, -1, -1, -1, -1, -1, 806, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 306, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 306, -1, -1, 306, 306, 306, -1, -1, -1, 306, 306, -1, -1, -1, -1, -1, 306, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 306, 306, 306, 306, 306, -1, -1, -1, -1, -1, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 449, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, 306, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 807, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 808, -1, -1, 808, 808, 808, -1, -1, -1, 808, 808, -1, -1, -1, -1, -1, 808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 808, 808, 808, 808, 808, -1, -1, -1, -1, -1, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, 808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 192, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 813, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 814, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 811, 812, 812, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 813, 1797, 814, 814, 814, 814, 814, 814, 814, 1798, 814, 814, 814, 814, 814, 814, 814, 814, 814, 814, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 453, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 834, 835, 1800, 838, 839, 840, 841, 842, 843, 844, 845, 846, 847, 848, 452, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 861, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 868, 856, 857, 858, 859, 860, 862, 863, 864, 865, 866, 867, 869, 870, 871, 872, 873, 874, 875, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 876, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 855, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 852, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 334, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 879, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 879, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 879, -1, -1, 879, 879, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 879, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 879, 879, 879, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 883, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 883, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 883, -1, -1, 883, 883, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 883, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 883, 883, 883, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 888, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 892, -1, -1, 889, 890, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1802, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 891, 893, 1804, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 937, -1, 922, -1, 915, 914, 913, 918, 920, 906, 907, 908, 909, 910, 938, 899, 900, -1, -1, -1, -1, -1, -1, -1, -1, 936, 935, 916, 917, 919, 921, 911, 912, 923, 924, 925, 926, 927, 928, 929, 930, 931, 932, 933, 901, 902, -1, -1, -1, -1, -1, 905, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1806, 934, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1808, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 965, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 949, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 949, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 949, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 949, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, 949, -1, -1, -1, -1, -1, 949, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 951, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 951, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 951, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 1810, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, 951, -1, -1, -1, -1, -1, 953, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, 954, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 959, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 964, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 964, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 971, 971, 971, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 981, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 986, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 987, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 973, 974, 975, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 976, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 977, 977, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 977, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 980, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 983, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 990, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, -1, 487, -1, -1, -1, -1, -1, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, -1, -1, -1, -1, -1, -1, 487, 487, 487, 487, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, 487, -1, 487, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, 487, -1, -1, -1, 487, 487, 487, 487, 487, 487, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, 166, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, 166, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, 166, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, 166, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, 166, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 166, 166, 166, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 490, -1, 0, -1, -1, -1, 490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, -1, -1, -1, -1, -1, -1, 490, 490, 490, 490, 490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, 490, -1, 490, 490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, 490, -1, -1, -1, 490, 490, 490, 490, 490, 490, 
			-1, -1, -1, -1, -1, 492, -1, 0, -1, -1, -1, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, 492, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 492, 492, -1, 492, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, -1, -1, -1, 492, 492, 492, 492, 492, 492, 
			-1, -1, -1, -1, -1, 0, -1, -1, 494, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 496, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, -1, -1, 501, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, 501, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 505, -1, 0, 0, -1, -1, 505, -1, 505, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 506, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 512, 512, 512, 512, 512, 512, 0, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 518, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 519, 0, 0, 0, 0, 0, 1812, 0, 519, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 526, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, 528, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 531, 531, 531, 0, 0, 531, 0, 0, 531, 531, 531, 0, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 
			-1, 533, 533, 533, 533, 533, 533, 0, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 533, 
			-1, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 0, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 535, 
			-1, 537, 537, 537, 0, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 537, 
			-1, 539, 539, 539, 0, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 539, 
			-1, 541, 541, 541, 0, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 541, 
			-1, -1, -1, -1, 0, -1, -1, -1, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 551, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 552, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 557, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 559, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 561, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 563, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 565, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 567, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 569, 569, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 573, 573, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 573, 573, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 579, 579, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 583, 583, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 587, 587, 587, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 592, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 594, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, 594, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1584, 1584, 1584, 1584, 1584, 1584, 1584, 1584, 1584, 1584, 1584, 1584, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 620, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1586, 1586, 1586, 1586, 1586, 1586, 1586, 1586, 1586, 1586, 1586, 1586, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 622, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 625, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 629, 0, 0, -1, 0, 0, 0, 629, 629, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 629, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 629, 629, -1, -1, -1, -1, -1, 629, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 635, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 655, 656, 657, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 1604, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 659, 659, 659, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 661, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 663, -1, -1, 228, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 663, -1, -1, 663, 663, 663, -1, -1, -1, 663, 663, -1, -1, -1, -1, -1, 663, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 663, 663, 663, 663, 663, -1, -1, -1, -1, -1, 663, 663, 663, 663, 663, 663, 663, 663, 663, 663, 663, 663, 663, 663, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 663, 663, 663, 663, 663, 663, 663, 663, 663, 663, 663, 663, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 662, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 673, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 677, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 679, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 681, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 683, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 685, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 687, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 689, 689, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 691, 691, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 691, 691, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 693, 693, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 695, 695, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 697, 697, 697, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 699, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 701, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 701, 701, 701, 701, 701, 701, 701, 701, 701, 701, 701, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, 232, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 718, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1622, 1622, 1622, 1622, 1622, 1622, 1622, 1622, 1622, 1622, 1622, 1622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 722, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 724, 0, 0, -1, -1, 0, 0, 724, 724, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 724, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 724, 724, -1, -1, -1, -1, -1, 724, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 730, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 745, 746, 747, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 79, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 749, 749, 749, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 751, -1, 751, 751, 0, -1, -1, -1, 751, -1, -1, -1, -1, -1, 751, -1, -1, 751, 751, 751, -1, -1, -1, 751, 751, -1, -1, -1, 751, -1, 751, 751, 751, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 751, 751, 751, 751, 751, 751, 751, -1, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, -1, -1, 751, -1, 751, -1, -1, 751, 751, 751, 751, 751, 751, 751, 751, -1, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, -1, -1, -1, 751, 751, 751, 751, 751, 751, 
			-1, -1, -1, 0, -1, 753, 0, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, 0, -1, 1813, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 753, 0, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 754, -1, 754, 754, 0, -1, -1, -1, 754, -1, -1, -1, -1, -1, 754, -1, -1, 754, 754, 754, -1, -1, -1, 754, 754, -1, -1, -1, 754, -1, 754, 754, 754, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 754, 754, 754, 754, 754, 754, 754, -1, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, -1, -1, 754, -1, 754, -1, -1, 754, 754, 754, 754, 754, 754, 754, 754, -1, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, -1, -1, -1, 754, 754, 754, 754, 754, 754, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 755, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 758, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 760, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 763, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 765, -1, 765, 765, 0, -1, -1, -1, 765, -1, -1, -1, -1, -1, 765, -1, -1, 765, 765, 765, -1, -1, -1, 765, 765, -1, -1, -1, 765, -1, 765, 765, 765, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 765, 765, 765, 765, 765, 765, 765, -1, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, -1, -1, 765, -1, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, -1, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, 765, -1, -1, -1, 765, 765, 765, 765, 765, 765, 
			-1, -1, -1, 771, -1, 771, -1, -1, 771, -1, -1, 164, -1, -1, -1, -1, -1, 771, -1, -1, 771, 771, 771, -1, -1, -1, 771, 771, -1, -1, -1, -1, -1, 1814, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 771, 771, 771, 771, 771, -1, 164, -1, -1, -1, 771, 771, 771, 771, 771, 771, 771, 771, 771, 771, 771, 771, 771, 771, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1815, 1817, 1819, 1821, 1823, 1825, 1827, 1829, 1831, 1833, 1835, 1837, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, 164, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 772, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 772, -1, -1, 772, 772, 772, -1, -1, -1, 772, 772, -1, -1, -1, -1, -1, 772, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 772, 772, 772, 772, 772, -1, -1, -1, -1, -1, 772, 772, 772, 772, 772, 772, 772, 772, 772, 772, 772, 772, 772, 772, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 772, 772, 772, 772, 772, 772, 772, 772, 772, 772, 772, 772, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 773, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 773, -1, -1, 773, 773, 773, -1, -1, -1, 773, 773, -1, -1, -1, -1, -1, 773, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 773, 773, 773, 773, 773, -1, -1, -1, -1, -1, 773, 773, 773, 773, 773, 773, 773, 773, 773, 773, 773, 773, 773, 773, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 773, 773, 773, 773, 773, 773, 773, 773, 773, 773, 773, 773, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1668, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1669, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1669, -1, -1, 1669, 1669, 1669, -1, -1, -1, 1669, 1669, -1, -1, -1, -1, -1, 1669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1669, 1669, 1669, 1669, 1669, -1, -1, -1, -1, -1, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, 1669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 1670, 1670, 1670, 1670, 1670, 1670, 1670, 1670, 1670, 1670, 1670, 1670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 779, -1, 779, 779, 0, -1, -1, -1, 779, -1, -1, -1, -1, -1, 779, -1, -1, 779, 779, 779, -1, -1, -1, 779, 779, -1, -1, -1, 779, -1, 779, 779, 779, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 779, 779, 779, 779, 779, 779, 779, -1, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, -1, -1, 779, -1, 779, -1, -1, 779, 779, 779, 779, 779, 779, 779, 779, -1, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, -1, -1, -1, 779, 779, 779, 779, 779, 779, 
			-1, 782, 782, 782, 782, 782, 782, 0, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 287, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 1, 1, 1, 1, 1, 290, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, 786, 786, 786, 786, 786, 786, 0, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 786, 
			-1, 790, 790, 790, 0, 0, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 790, 
			-1, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 0, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 791, 
			-1, 793, 793, 793, 0, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 793, 
			-1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 796, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 301, 301, 301, 301, 0, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 
			-1, 301, 301, 301, 301, 0, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 
			-1, 301, 301, 301, 0, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 
			-1, 301, 301, 301, 301, 0, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 
			-1, 301, 301, 301, 301, 0, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 91, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 301, 301, 301, 301, 0, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 
			-1, -1, -1, 805, -1, 805, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 805, -1, -1, 805, 805, 805, -1, -1, -1, 805, 805, -1, -1, -1, -1, -1, 805, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 805, 805, 805, 805, 805, -1, -1, -1, -1, -1, 805, 805, 805, 805, 805, 805, 805, 805, 805, 805, 805, 805, 805, 805, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 805, 805, 805, 805, 805, 805, 805, 805, 805, 805, 805, 805, 805, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 305, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, -1, 0, 0, -1, 810, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1839, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 810, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 810, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 817, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 817, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, 817, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 850, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 851, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 853, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, 0, -1, 878, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 881, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 884, -1, 0, 0, -1, 0, 884, -1, 884, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 885, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, -1, 0, 0, 887, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 145, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 940, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 940, 940, 940, 940, 940, 940, 940, 940, 940, 940, 940, 940, 940, 940, 940, 940, 940, 940, 940, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 943, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 950, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 955, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 956, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 956, -1, -1, 956, 956, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 956, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 956, 956, 956, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 957, 0, -1, -1, -1, 0, 957, -1, 957, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 958, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 960, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 961, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 961, -1, -1, 961, 961, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 961, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 961, 961, 961, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 962, 0, -1, -1, -1, 0, 962, -1, 962, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 963, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1701, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1708, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 972, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 978, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, 979, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1712, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1713, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 982, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1717, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1718, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 988, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 989, -1, 0, -1, -1, -1, 989, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 989, -1, -1, -1, -1, -1, -1, -1, -1, -1, 989, 989, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 989, 989, -1, 989, 989, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, 989, -1, -1, -1, 989, 989, 989, 989, 989, 989, 
			-1, -1, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			
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
			335, 155,  -1, // 0
			991, 4, 1543, 3, 28, 2,  -1, // 1
			994, 7, 339, 6, 993, 1544, 29,  -1, // 2
			339, 155,  -1, // 3
			996, 7, 340, 6, 995, 1545, 30,  -1, // 4
			340, 167,  -1, // 5
			5, 341, 172, 31,  -1, // 6
			341, 172, 8,  -1, // 7
			5, 342, 172, 32,  -1, // 8
			342, 172, 8,  -1, // 9
			997, 9, 1546,  -1, // 10
			1380, 174,  -1, // 11
			343, 998, 1547,  -1, // 12
			343, 999, 1548, 10,  -1, // 13
			1381, 176,  -1, // 14
			1382, 177, 344, 1549,  -1, // 15
			345, 1383, 178,  -1, // 16
			1388, 177, 1000, 347, 346, 1385, 180, 1384, 179, 345,  -1, // 17
			346, 1386, 181,  -1, // 18
			347, 1387, 182,  -1, // 19
			3, 1389, 179,  -1, // 20
			1001, 5,  -1, // 21
			1002, 1550,  -1, // 22
			1390, 184,  -1, // 23
			1004, 1552, 348, 1003, 1551,  -1, // 24
			348, 1391, 185,  -1, // 25
			1392, 184,  -1, // 26
			1005, 1553,  -1, // 27
			1006, 1554,  -1, // 28
			1007, 1555,  -1, // 29
			1009, 350, 349, 1008, 1556,  -1, // 30
			349, 1393, 178,  -1, // 31
			350, 1394, 181,  -1, // 32
			1395, 186,  -1, // 33
			351, 1010, 1557,  -1, // 34
			351, 1011, 1558, 8,  -1, // 35
			1012, 1559,  -1, // 36
			1013, 1560,  -1, // 37
			12, 193, 1014, 11,  -1, // 38
			352, 194, 8,  -1, // 39
			352, 194,  -1, // 40
			195, 13,  -1, // 41
			1015, 353, 1561,  -1, // 42
			354, 1396, 196,  -1, // 43
			354, 1396, 196,  -1, // 44
			1017, 1563, 355, 1016, 1562,  -1, // 45
			355, 1397, 196,  -1, // 46
			1019, 1565, 356, 1018, 1564,  -1, // 47
			356, 1398, 196,  -1, // 48
			1021, 1567, 357, 1020, 1566,  -1, // 49
			357, 1399, 196,  -1, // 50
			1022, 1568,  -1, // 51
			358, 1400, 198,  -1, // 52
			4, 358, 3,  -1, // 53
			359, 1401, 198,  -1, // 54
			4, 359, 3,  -1, // 55
			1023, 1569,  -1, // 56
			1024, 1570,  -1, // 57
			1025, 1571,  -1, // 58
			1402, 203,  -1, // 59
			1403, 203,  -1, // 60
			360, 1026, 1572,  -1, // 61
			360, 1027, 1573, 8,  -1, // 62
			361, 1028, 1574,  -1, // 63
			361, 1029, 1575, 8,  -1, // 64
			362, 1031, 1576, 8,  -1, // 65
			362, 1031, 1576, 8, 1030,  -1, // 66
			1032, 1404, 203,  -1, // 67
			1033, 1577,  -1, // 68
			363, 1405, 208,  -1, // 69
			1036, 1407, 203, 9, 1035, 1406, 203, 14, 1034,  -1, // 70
			364, 1408, 209,  -1, // 71
			364, 1037, 1409, 209, 36,  -1, // 72
			365, 1410, 210,  -1, // 73
			365, 1038, 1411, 210, 37,  -1, // 74
			366, 1412, 211,  -1, // 75
			366, 1039, 1413, 211, 15,  -1, // 76
			367, 1414, 212,  -1, // 77
			367, 1040, 1415, 212, 16,  -1, // 78
			368, 1416, 213,  -1, // 79
			368, 1041, 1417, 213, 17,  -1, // 80
			369, 1418, 214,  -1, // 81
			369, 1042, 1419, 214, 1578,  -1, // 82
			1043, 38,  -1, // 83
			1044, 39,  -1, // 84
			370, 1420, 216,  -1, // 85
			370, 1045, 1421, 216, 1579,  -1, // 86
			1046, 18,  -1, // 87
			1047, 19,  -1, // 88
			1048, 40,  -1, // 89
			1049, 41,  -1, // 90
			371, 1422, 218,  -1, // 91
			371, 1050, 1423, 218, 1580,  -1, // 92
			1051, 42,  -1, // 93
			1052, 43,  -1, // 94
			372, 1424, 220,  -1, // 95
			372, 1053, 1425, 220, 1581,  -1, // 96
			1054, 20,  -1, // 97
			1055, 21,  -1, // 98
			373, 1426, 222,  -1, // 99
			373, 1056, 1427, 222, 1582,  -1, // 100
			1057, 22,  -1, // 101
			1058, 23,  -1, // 102
			1059, 24,  -1, // 103
			374, 1428, 224,  -1, // 104
			374, 1060, 1429, 224, 25,  -1, // 105
			375, 1430, 225,  -1, // 106
			1431, 226,  -1, // 107
			1061, 1432, 203, 1583,  -1, // 108
			228, 1062, 13,  -1, // 109
			1063, 13,  -1, // 110
			1064, 44,  -1, // 111
			1065, 45,  -1, // 112
			1066, 46,  -1, // 113
			1067, 47,  -1, // 114
			1068, 48,  -1, // 115
			1069, 49,  -1, // 116
			1070, 50,  -1, // 117
			1071, 51,  -1, // 118
			1072, 52,  -1, // 119
			1073, 53,  -1, // 120
			1074, 54,  -1, // 121
			1433, 229,  -1, // 122
			1434, 230,  -1, // 123
			1075, 1435, 225, 20,  -1, // 124
			1076, 1436, 225, 21,  -1, // 125
			1077, 1437, 225, 26,  -1, // 126
			1078, 1438, 225, 17,  -1, // 127
			1079, 1439, 225, 22,  -1, // 128
			1080, 1440, 225, 27,  -1, // 129
			1081, 1441, 225, 55,  -1, // 130
			1082, 1442, 225, 56,  -1, // 131
			1084, 1443, 225, 1083, 57,  -1, // 132
			1085, 1444, 225, 4, 1585, 376, 3,  -1, // 133
			379, 1086, 1587, 58, 378,  -1, // 134
			1445, 234,  -1, // 135
			1087, 1446, 225, 59,  -1, // 136
			1447, 186,  -1, // 137
			1088, 4, 1589, 3, 380, 1588,  -1, // 138
			3, 382, 172,  -1, // 139
			1448, 237, 383, 3,  -1, // 140
			384, 1449, 238,  -1, // 141
			384, 1450, 239,  -1, // 142
			1089, 4, 1590, 3,  -1, // 143
			1090, 4, 1591, 3, 26,  -1, // 144
			1091, 12, 1592, 11,  -1, // 145
			1092, 55,  -1, // 146
			1093, 56,  -1, // 147
			1094, 61,  -1, // 148
			1451, 240, 385, 10,  -1, // 149
			1452, 240, 1095, 62,  -1, // 150
			1096, 1593,  -1, // 151
			1097, 63,  -1, // 152
			1098, 64,  -1, // 153
			4, 1453, 203, 3,  -1, // 154
			1454, 241,  -1, // 155
			1099, 1594,  -1, // 156
			1100, 65,  -1, // 157
			1101, 1595,  -1, // 158
			1102, 1596,  -1, // 159
			1103, 68,  -1, // 160
			1104, 69,  -1, // 161
			1105, 70,  -1, // 162
			1106, 4, 1597, 3, 71,  -1, // 163
			1107, 4, 1598, 3, 72,  -1, // 164
			1108, 4, 1599, 3, 73,  -1, // 165
			1109, 4, 33, 8, 188, 3, 74,  -1, // 166
			1110, 4, 1455, 203, 3, 75,  -1, // 167
			1111, 1600,  -1, // 168
			1112, 1601,  -1, // 169
			1114, 387, 1603, 1113, 1602,  -1, // 170
			1115, 388, 386,  -1, // 171
			388, 386,  -1, // 172
			1116, 7, 389, 1456, 242, 6,  -1, // 173
			389, 1457, 242, 8,  -1, // 174
			390, 1118, 13, 1605,  -1, // 175
			1119, 1606,  -1, // 176
			1121, 1607, 1120,  -1, // 177
			228, 1122,  -1, // 178
			1123, 0,  -1, // 179
			1124, 1608,  -1, // 180
			1458, 244,  -1, // 181
			1459, 244,  -1, // 182
			392, 1125, 1609,  -1, // 183
			392, 1126, 1610, 8,  -1, // 184
			393, 1127, 1611,  -1, // 185
			393, 1128, 1612, 8,  -1, // 186
			394, 1130, 1613, 8,  -1, // 187
			394, 1130, 1613, 8, 1129,  -1, // 188
			395, 1460, 248,  -1, // 189
			1131, 1462, 244, 9, 1461, 244, 14,  -1, // 190
			396, 1463, 249,  -1, // 191
			396, 1132, 1464, 249, 36,  -1, // 192
			397, 1465, 250,  -1, // 193
			397, 1133, 1466, 250, 37,  -1, // 194
			398, 1467, 251,  -1, // 195
			398, 1134, 1468, 251, 15,  -1, // 196
			399, 1469, 252,  -1, // 197
			399, 1135, 1470, 252, 16,  -1, // 198
			400, 1471, 253,  -1, // 199
			400, 1136, 1472, 253, 17,  -1, // 200
			401, 1473, 254,  -1, // 201
			401, 1137, 1474, 254, 1614,  -1, // 202
			402, 1475, 255,  -1, // 203
			402, 1138, 1476, 255, 1615,  -1, // 204
			403, 1477, 256,  -1, // 205
			403, 1139, 1478, 256, 1616,  -1, // 206
			404, 1479, 257,  -1, // 207
			404, 1140, 1480, 257, 1617,  -1, // 208
			405, 1481, 258,  -1, // 209
			405, 1141, 1482, 258, 1618,  -1, // 210
			406, 1483, 259,  -1, // 211
			406, 1142, 1484, 259, 25,  -1, // 212
			407, 1485, 260,  -1, // 213
			1144, 1486, 244, 1619, 1143,  -1, // 214
			1487, 261,  -1, // 215
			1488, 262,  -1, // 216
			1145, 1489, 260, 20,  -1, // 217
			1146, 1490, 260, 21,  -1, // 218
			1147, 1491, 260, 26,  -1, // 219
			1148, 1492, 260, 17,  -1, // 220
			1149, 1493, 260, 22,  -1, // 221
			1150, 1494, 260, 27,  -1, // 222
			1151, 1495, 260, 55,  -1, // 223
			1152, 1496, 260, 56,  -1, // 224
			1153, 1497, 260, 57,  -1, // 225
			1154, 4, 190, 3, 71,  -1, // 226
			1155, 4, 190, 3, 72,  -1, // 227
			1156, 4, 1620, 3, 73,  -1, // 228
			1157, 4, 33, 8, 188, 3, 74,  -1, // 229
			1158, 4, 1498, 244, 3, 75,  -1, // 230
			1159, 1499, 260, 4, 1621, 408, 3,  -1, // 231
			1160, 1623, 58, 410,  -1, // 232
			1161, 1500, 260, 59,  -1, // 233
			1162, 1624,  -1, // 234
			1163, 4, 247, 3, 411, 1625,  -1, // 235
			413, 1501, 264,  -1, // 236
			413, 1502, 265,  -1, // 237
			1164, 4, 1626, 3,  -1, // 238
			1165, 4, 1627, 3, 26,  -1, // 239
			1166, 12, 1628, 11,  -1, // 240
			1167, 55,  -1, // 241
			1168, 56,  -1, // 242
			1169, 61,  -1, // 243
			1503, 266, 414, 10,  -1, // 244
			1504, 266, 1170, 62,  -1, // 245
			1171, 1629,  -1, // 246
			1172, 63,  -1, // 247
			1173, 64,  -1, // 248
			1174, 1630,  -1, // 249
			1175, 65,  -1, // 250
			1176, 1631,  -1, // 251
			1177, 67,  -1, // 252
			1178, 68,  -1, // 253
			1179, 69,  -1, // 254
			1180, 70,  -1, // 255
			1505, 267,  -1, // 256
			4, 1506, 244, 3,  -1, // 257
			1181, 1632,  -1, // 258
			1182, 1633,  -1, // 259
			1183, 416, 200, 78,  -1, // 260
			1184, 417, 415,  -1, // 261
			417, 415,  -1, // 262
			1186, 1635, 418, 1185, 1634,  -1, // 263
			418, 168,  -1, // 264
			1189, 1637, 420, 1188, 419, 1187, 1636,  -1, // 265
			419, 270,  -1, // 266
			420, 168,  -1, // 267
			1190, 4, 1639, 3, 1638,  -1, // 268
			1191, 4, 1641, 3, 60, 10, 1640,  -1, // 269
			60, 422, 10, 33,  -1, // 270
			422, 10, 33,  -1, // 271
			60, 423, 1192, 10, 1642,  -1, // 272
			423, 1193, 10, 1643,  -1, // 273
			5, 200,  -1, // 274
			1196, 424, 1646, 1194, 1645, 1644, 3, 81,  -1, // 275
			1648, 1195, 1647,  -1, // 276
			1198, 1651, 425, 1197, 1650, 4, 1649, 3, 83,  -1, // 277
			425, 1507, 284,  -1, // 278
			1199, 1654, 1653, 1652,  -1, // 279
			1200, 1656, 1655,  -1, // 280
			1202, 1659, 1201, 1658, 1657, 3, 86,  -1, // 281
			1205, 5, 4, 1662, 3, 86, 1204, 1661, 1203, 1660,  -1, // 282
			1212, 1665, 1211, 4, 428, 5, 427, 426, 1206, 3, 1663,  -1, // 283
			5, 202,  -1, // 284
			1208, 1664, 1207,  -1, // 285
			1210, 205, 1209,  -1, // 286
			1213, 5, 1666,  -1, // 287
			1214, 5, 1667,  -1, // 288
			1215, 5, 430, 92, 429,  -1, // 289
			1217, 1672, 1216, 1671, 431,  -1, // 290
			1219, 1674, 432, 1218, 1673, 57,  -1, // 291
			432, 168,  -1, // 292
			1220, 9, 34,  -1, // 293
			1221, 9, 35,  -1, // 294
			433, 286,  -1, // 295
			1222, 7, 433, 286, 6,  -1, // 296
			1225, 169, 1224, 4, 205, 3, 94,  -1, // 297
			1226, 5, 1675,  -1, // 298
			436, 435,  -1, // 299
			7, 436, 6,  -1, // 300
			5, 301,  -1, // 301
			437, 302,  -1, // 302
			437, 302,  -1, // 303
			438, 302,  -1, // 304
			12, 438, 302, 11,  -1, // 305
			439, 302,  -1, // 306
			4, 439, 3,  -1, // 307
			440, 289, 4, 301, 3, 81,  -1, // 308
			289, 82,  -1, // 309
			290, 4, 301, 3, 83,  -1, // 310
			289, 4, 301, 3, 86,  -1, // 311
			5, 4, 301, 3, 86, 289, 87,  -1, // 312
			289, 4, 443, 5, 442, 5, 441, 3, 88,  -1, // 313
			5, 444, 89,  -1, // 314
			5, 445, 90,  -1, // 315
			5, 447, 92, 446,  -1, // 316
			289, 93,  -1, // 317
			448, 304,  -1, // 318
			7, 448, 304, 6,  -1, // 319
			289, 1228, 4, 246, 3, 94,  -1, // 320
			1229, 5, 191,  -1, // 321
			450, 1508, 307,  -1, // 322
			450, 1508, 307,  -1, // 323
			1230, 1676,  -1, // 324
			1231, 1677,  -1, // 325
			1232, 1678,  -1, // 326
			1233, 1679,  -1, // 327
			1509, 310,  -1, // 328
			451, 1510, 237,  -1, // 329
			451, 1510, 237,  -1, // 330
			1234, 1680,  -1, // 331
			1235, 1681,  -1, // 332
			1236, 95,  -1, // 333
			1237, 96,  -1, // 334
			1238, 97,  -1, // 335
			1239, 98,  -1, // 336
			1240, 99,  -1, // 337
			1241, 100,  -1, // 338
			1242, 101,  -1, // 339
			1243, 102,  -1, // 340
			1244, 103,  -1, // 341
			1245, 104,  -1, // 342
			1246, 105,  -1, // 343
			1247, 106,  -1, // 344
			1248, 107,  -1, // 345
			1249, 108,  -1, // 346
			1250, 109,  -1, // 347
			1251, 110,  -1, // 348
			1252, 111,  -1, // 349
			1253, 19, 1682, 18, 111,  -1, // 350
			1254, 112,  -1, // 351
			1255, 113,  -1, // 352
			1256, 114,  -1, // 353
			1257, 115,  -1, // 354
			1258, 116,  -1, // 355
			1259, 117,  -1, // 356
			1260, 118,  -1, // 357
			1261, 119,  -1, // 358
			1262, 120,  -1, // 359
			1263, 121,  -1, // 360
			1264, 122,  -1, // 361
			6, 123,  -1, // 362
			1265, 1683,  -1, // 363
			1266, 1684,  -1, // 364
			454, 1268, 1685, 1267,  -1, // 365
			1269, 3,  -1, // 366
			1270, 1686,  -1, // 367
			1272, 1511, 313, 1271, 123,  -1, // 368
			1273, 124,  -1, // 369
			1274, 125,  -1, // 370
			1275, 126,  -1, // 371
			1276, 127,  -1, // 372
			1277, 128,  -1, // 373
			1278, 61,  -1, // 374
			1279, 129,  -1, // 375
			1280, 130,  -1, // 376
			1281, 131,  -1, // 377
			1282, 132,  -1, // 378
			1283, 133,  -1, // 379
			1284, 134,  -1, // 380
			1285, 123,  -1, // 381
			1286, 135,  -1, // 382
			1287, 136,  -1, // 383
			1288, 137,  -1, // 384
			1289, 138,  -1, // 385
			1290, 139,  -1, // 386
			1291, 140,  -1, // 387
			1292, 141,  -1, // 388
			1293, 4, 1687, 3, 131,  -1, // 389
			455, 1512, 314,  -1, // 390
			455, 1513, 314, 8,  -1, // 391
			1296, 457, 456, 1514, 1688,  -1, // 392
			1294, 1689, 60,  -1, // 393
			1295, 1690, 13,  -1, // 394
			458, 1515, 178,  -1, // 395
			460, 459, 1516, 179, 458,  -1, // 396
			459, 1517, 181,  -1, // 397
			460, 1518, 182,  -1, // 398
			461, 1297, 1519, 1691,  -1, // 399
			461, 1520, 316, 10,  -1, // 400
			1298, 1692,  -1, // 401
			1299, 63,  -1, // 402
			1300, 64,  -1, // 403
			1301, 142,  -1, // 404
			1302, 60,  -1, // 405
			1303, 143,  -1, // 406
			1304, 60, 97,  -1, // 407
			1305, 143, 97,  -1, // 408
			1306, 4, 3, 144,  -1, // 409
			1307, 1693, 144,  -1, // 410
			1308, 1694, 144,  -1, // 411
			1309, 26,  -1, // 412
			1310, 27,  -1, // 413
			1311, 55,  -1, // 414
			1312, 56,  -1, // 415
			1313, 55, 145,  -1, // 416
			1314, 56, 145,  -1, // 417
			1315, 62, 462,  -1, // 418
			1316, 20,  -1, // 419
			1317, 21,  -1, // 420
			1318, 22,  -1, // 421
			1319, 23,  -1, // 422
			1320, 24,  -1, // 423
			1321, 42,  -1, // 424
			1322, 43,  -1, // 425
			1323, 17,  -1, // 426
			1324, 16,  -1, // 427
			1325, 15,  -1, // 428
			1326, 38,  -1, // 429
			1327, 39,  -1, // 430
			1328, 18,  -1, // 431
			1329, 40,  -1, // 432
			1330, 19,  -1, // 433
			1331, 41,  -1, // 434
			1332, 13,  -1, // 435
			1333, 44,  -1, // 436
			1334, 45,  -1, // 437
			1335, 46,  -1, // 438
			1336, 47,  -1, // 439
			1337, 48,  -1, // 440
			1338, 49,  -1, // 441
			1339, 50,  -1, // 442
			1340, 51,  -1, // 443
			1341, 52,  -1, // 444
			1342, 53,  -1, // 445
			1343, 54,  -1, // 446
			1344, 146,  -1, // 447
			1345, 37,  -1, // 448
			1346, 36,  -1, // 449
			1347, 12, 11,  -1, // 450
			1348, 25,  -1, // 451
			463, 1349, 22,  -1, // 452
			463, 1350, 1695,  -1, // 453
			1521, 318,  -1, // 454
			1522, 180,  -1, // 455
			1523, 319,  -1, // 456
			1352, 12, 1696, 11,  -1, // 457
			1353, 12, 1697, 11,  -1, // 458
			1354, 12, 11,  -1, // 459
			4, 1524, 320, 3,  -1, // 460
			4, 3,  -1, // 461
			465, 1525, 321,  -1, // 462
			465, 1526, 321, 8,  -1, // 463
			1527, 322,  -1, // 464
			1528, 323,  -1, // 465
			1529, 324,  -1, // 466
			1357, 469, 468, 467, 466, 1355, 1698,  -1, // 467
			466, 1530, 178,  -1, // 468
			1531, 179,  -1, // 469
			468, 1532, 181,  -1, // 470
			1356, 1699, 13,  -1, // 471
			1359, 473, 472, 471, 470, 1533, 178,  -1, // 472
			470, 1533, 178,  -1, // 473
			1534, 179,  -1, // 474
			472, 1535, 181,  -1, // 475
			1358, 1700, 13,  -1, // 476
			1360, 147, 474,  -1, // 477
			1361, 1702, 9,  -1, // 478
			1362, 126,  -1, // 479
			1363, 1703,  -1, // 480
			1364, 1704,  -1, // 481
			1365, 1705,  -1, // 482
			1366, 1706,  -1, // 483
			1536, 330, 1367, 476, 475, 1707,  -1, // 484
			1709, 9,  -1, // 485
			1368, 148,  -1, // 486
			1369, 149,  -1, // 487
			1370, 150,  -1, // 488
			1372, 7, 1537, 331, 1371, 6,  -1, // 489
			477, 1538, 332,  -1, // 490
			477, 1539, 332, 8,  -1, // 491
			1711, 13,  -1, // 492
			1373, 478, 1710,  -1, // 493
			1540, 313, 1374, 481, 480, 479, 151,  -1, // 494
			1714, 9,  -1, // 495
			1375, 4, 1715, 3, 28,  -1, // 496
			1376, 1716,  -1, // 497
			1377, 85,  -1, // 498
			1541, 313, 1378, 482, 152,  -1, // 499
			1542, 313, 1379, 484, 1719, 483, 153,  -1, // 500
			1720, 9,  -1, // 501
			485, 167,  -1, // 502
			7, 485, 6,  -1, // 503
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 10, 18, 21, 29, 32, 37, 41, 46, 50, 54, 57, 61, 66, 69, 74, 78, 89, 93, 97, 101, 104, 107, 110, 116, 120, 123, 126, 129, 132, 138, 142, 146, 149, 153, 158, 161, 164, 169, 173, 176, 179, 183, 187, 191, 197, 201, 207, 211, 217, 221, 224, 228, 232, 236, 240, 243, 246, 249, 252, 255, 259, 264, 268, 273, 278, 284, 288, 291, 295, 305, 309, 315, 319, 325, 329, 335, 339, 345, 349, 355, 359, 365, 368, 371, 375, 381, 384, 387, 390, 393, 397, 403, 406, 409, 413, 419, 422, 425, 429, 435, 438, 441, 444, 448, 454, 458, 461, 466, 470, 473, 476, 479, 482, 485, 488, 491, 494, 497, 500, 503, 506, 509, 512, 517, 522, 527, 532, 537, 542, 547, 552, 558, 566, 572, 575, 580, 583, 590, 594, 599, 603, 607, 612, 618, 623, 626, 629, 632, 637, 642, 645, 648, 651, 656, 659, 662, 665, 668, 671, 674, 677, 680, 686, 692, 698, 706, 713, 716, 719, 725, 729, 732, 739, 744, 749, 752, 756, 759, 762, 765, 768, 771, 775, 780, 784, 789, 794, 800, 804, 812, 816, 822, 826, 832, 836, 842, 846, 852, 856, 862, 866, 872, 876, 882, 886, 892, 896, 902, 906, 912, 916, 922, 926, 932, 935, 938, 943, 948, 953, 958, 963, 968, 973, 978, 983, 989, 995, 1001, 1009, 1016, 1024, 1029, 1034, 1037, 1044, 1048, 1052, 1057, 1063, 1068, 1071, 1074, 1077, 1082, 1087, 1090, 1093, 1096, 1099, 1102, 1105, 1108, 1111, 1114, 1117, 1120, 1125, 1128, 1131, 1136, 1140, 1143, 1149, 1152, 1160, 1163, 1166, 1172, 1180, 1185, 1189, 1195, 1200, 1203, 1212, 1216, 1226, 1230, 1235, 1239, 1247, 1258, 1270, 1273, 1277, 1281, 1285, 1289, 1295, 1301, 1308, 1311, 1315, 1319, 1322, 1328, 1336, 1340, 1343, 1347, 1350, 1353, 1356, 1359, 1364, 1367, 1371, 1378, 1381, 1387, 1393, 1401, 1411, 1415, 1419, 1424, 1427, 1430, 1435, 1442, 1446, 1450, 1454, 1457, 1460, 1463, 1466, 1469, 1473, 1477, 1480, 1483, 1486, 1489, 1492, 1495, 1498, 1501, 1504, 1507, 1510, 1513, 1516, 1519, 1522, 1525, 1528, 1531, 1534, 1540, 1543, 1546, 1549, 1552, 1555, 1558, 1561, 1564, 1567, 1570, 1573, 1576, 1579, 1582, 1587, 1590, 1593, 1599, 1602, 1605, 1608, 1611, 1614, 1617, 1620, 1623, 1626, 1629, 1632, 1635, 1638, 1641, 1644, 1647, 1650, 1653, 1656, 1659, 1665, 1669, 1674, 1680, 1684, 1688, 1692, 1698, 1702, 1706, 1711, 1716, 1719, 1722, 1725, 1728, 1731, 1734, 1738, 1742, 1747, 1751, 1755, 1758, 1761, 1764, 1767, 1771, 1775, 1779, 1782, 1785, 1788, 1791, 1794, 1797, 1800, 1803, 1806, 1809, 1812, 1815, 1818, 1821, 1824, 1827, 1830, 1833, 1836, 1839, 1842, 1845, 1848, 1851, 1854, 1857, 1860, 1863, 1866, 1869, 1872, 1876, 1879, 1883, 1887, 1890, 1893, 1896, 1901, 1906, 1910, 1915, 1918, 1922, 1927, 1930, 1933, 1936, 1944, 1948, 1951, 1955, 1959, 1967, 1971, 1974, 1978, 1982, 1986, 1990, 1993, 1996, 1999, 2002, 2005, 2012, 2015, 2018, 2021, 2024, 2031, 2035, 2040, 2043, 2047, 2055, 2058, 2064, 2067, 2070, 2076, 2084, 2087, 2090, 
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
		
		case EToken_Integer:
			return 66;
		
		case EToken_Fp:
			return 67;
		
		case EToken_True:
			return 68;
		
		case EToken_False:
			return 69;
		
		case EToken_Null:
			return 70;
		
		case EToken_SizeOf:
			return 71;
		
		case EToken_CountOf:
			return 72;
		
		case EToken_TypeOf:
			return 73;
		
		case EToken_OffsetOf:
			return 74;
		
		case EToken_BindingOf:
			return 75;
		
		case EToken_Literal:
			return 76;
		
		case EToken_HexLiteral:
			return 77;
		
		case EToken_FmtLiteral:
			return 78;
		
		case EToken_FmtSpecifier:
			return 79;
		
		case EToken_BaseType:
			return 80;
		
		case EToken_If:
			return 81;
		
		case EToken_Else:
			return 82;
		
		case EToken_Switch:
			return 83;
		
		case EToken_Case:
			return 84;
		
		case EToken_Default:
			return 85;
		
		case EToken_While:
			return 86;
		
		case EToken_Do:
			return 87;
		
		case EToken_For:
			return 88;
		
		case EToken_Break:
			return 89;
		
		case EToken_Continue:
			return 90;
		
		case EToken_Silent:
			return 91;
		
		case EToken_Return:
			return 92;
		
		case EToken_Once:
			return 93;
		
		case EToken_OnChange:
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
		
		case EToken_HeapU:
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
		
		case EToken_ConstD:
			return 127;
		
		case EToken_Volatile:
			return 128;
		
		case EToken_Thin:
			return 129;
		
		case EToken_Unsafe:
			return 130;
		
		case EToken_Unwinder:
			return 131;
		
		case EToken_Cdecl:
			return 132;
		
		case EToken_Stdcall:
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
		
		case EToken_EventD:
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
		
		case EToken_EnumF:
			return 149;
		
		case EToken_EnumC:
			return 150;
		
		case EToken_Struct:
			return 151;
		
		case EToken_Union:
			return 152;
		
		case EToken_Class:
			return 153;
		
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
			EToken_Unwinder, 
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
			"unwinder_modifier",
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
			
		
		case ESymbol_unwinder_modifier:
			pNode = AXL_MEM_NEW (CSymbolNode_unwinder_modifier);
			
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
			
			{ 0, 157 },
			
			{ 0, 172 },
			
			{ 0, 172 },
			
			{ 0, 173 },
			
			{ 0, 33 },
			
			{ 1, 33 },
			
			{ 0, 175 },
			
			{ 0, 183 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 34 },
			
			{ 0, 35 },
			
			{ 0, 1 },
			
			{ 0, 187 },
			
			{ 0, 188 },
			
			{ 1, 188 },
			
			{ 0, 188 },
			
			{ 0, 191 },
			
			{ 0, 33 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 11 },
			
			{ 1, 12 },
			
			{ 0, 3 },
			
			{ 1, 4 },
			
			{ 0, 1 },
			
			{ 0, 1 },
			
			{ 0, 200 },
			
			{ 0, 202 },
			
			{ 0, 200 },
			
			{ 1, 200 },
			
			{ 0, 200 },
			
			{ 1, 204 },
			
			{ 0, 204 },
			
			{ 0, 200 },
			
			{ 0, 215 },
			
			{ 0, 217 },
			
			{ 0, 219 },
			
			{ 0, 221 },
			
			{ 0, 223 },
			
			{ 0, 227 },
			
			{ 0, 232 },
			
			{ 1, 188 },
			
			{ 0, 232 },
			
			{ 1, 233 },
			
			{ 0, 236 },
			
			{ 1, 202 },
			
			{ 0, 202 },
			
			{ 0, 202 },
			
			{ 0, 204 },
			
			{ 0, 33 },
			
			{ 0, 33 },
			
			{ 0, 66 },
			
			{ 0, 67 },
			
			{ 0, 190 },
			
			{ 0, 190 },
			
			{ 0, 190 },
			
			{ 0, 76 },
			
			{ 1, 77 },
			
			{ 2, 78 },
			
			{ 4, 200 },
			
			{ 3, 79 },
			
			{ 0, 33 },
			
			{ 1, 200 },
			
			{ 0, 200 },
			
			{ 0, 191 },
			
			{ 0, 191 },
			
			{ 1, 191 },
			
			{ 0, 191 },
			
			{ 1, 245 },
			
			{ 0, 245 },
			
			{ 0, 215 },
			
			{ 0, 217 },
			
			{ 0, 219 },
			
			{ 0, 221 },
			
			{ 0, 223 },
			
			{ 0, 227 },
			
			{ 0, 190 },
			
			{ 0, 188 },
			
			{ 0, 232 },
			
			{ 1, 263 },
			
			{ 0, 188 },
			
			{ 0, 236 },
			
			{ 0, 247 },
			
			{ 0, 247 },
			
			{ 0, 245 },
			
			{ 0, 33 },
			
			{ 0, 33 },
			
			{ 0, 66 },
			
			{ 0, 76 },
			
			{ 1, 77 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 272 },
			
			{ 1, 202 },
			
			{ 0, 80 },
			
			{ 1, 202 },
			
			{ 0, 33 },
			
			{ 1, 33 },
			
			{ 0, 200 },
			
			{ 1, 4 },
			
			{ 4, 169 },
			
			{ 2, 82 },
			
			{ 3, 169 },
			
			{ 0, 200 },
			
			{ 1, 6 },
			
			{ 2, 7 },
			
			{ 1, 84 },
			
			{ 0, 207 },
			
			{ 2, 9 },
			
			{ 0, 85 },
			
			{ 1, 9 },
			
			{ 0, 200 },
			
			{ 1, 4 },
			
			{ 2, 169 },
			
			{ 0, 87 },
			
			{ 1, 169 },
			
			{ 2, 200 },
			
			{ 0, 88 },
			
			{ 1, 200 },
			
			{ 2, 169 },
			
			{ 0, 89 },
			
			{ 0, 90 },
			
			{ 1, 91 },
			
			{ 0, 200 },
			
			{ 0, 232 },
			
			{ 1, 93 },
			
			{ 2, 169 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 195 },
			
			{ 0, 232 },
			
			{ 0, 173 },
			
			{ 0, 308 },
			
			{ 0, 309 },
			
			{ 0, 308 },
			
			{ 0, 309 },
			
			{ 0, 188 },
			
			{ 0, 311 },
			
			{ 0, 236 },
			
			{ 0, 172 },
			
			{ 0, 172 },
			
			{ 0, 195 },
			
			{ 0, 315 },
			
			{ 1, 197 },
			
			{ 2, 195 },
			
			{ 0, 316 },
			
			{ 0, 33 },
			
			{ 0, 188 },
			
			{ 0, 317 },
			
			{ 0, 309 },
			
			{ 0, 195 },
			
			{ 0, 66 },
			
			{ 0, 175 },
			
			{ 1, 195 },
			
			{ 0, 195 },
			
			{ 0, 130 },
			
			{ 0, 66 },
			
			{ 0, 325 },
			
			{ 0, 326 },
			
			{ 0, 327 },
			
			{ 0, 328 },
			
			{ 0, 329 },
			
			{ 1, 33 },
			
			{ 2, 188 },
			
			{ 0, 33 },
			
			{ 1, 195 },
			
			{ 2, 333 },
			
			{ 0, 33 },
			
			{ 1, 189 },
			
			{ 0, 157 },
			
			{ 0, 66 },
			
			{ 0, 33 },
			
			{ 2, 333 },
			
			{ 0, 33 },
			
			{ 1, 189 },
			
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
// #line 6 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
			m_StructPackFactor = (*(_cls44*) GetAstLocator (0)).m_Value;
		;
// #line 6558 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 48 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 return false; ;
// #line 6569 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 60 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 
				return OpenGlobalNamespace ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos) != NULL;
			;
// #line 6582 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 
				m_pModule->m_NamespaceMgr.CloseNamespace ();
			;
// #line 6595 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 71 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 
				return OpenTypeExtension ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos);
			;
// #line 6608 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 5:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 75 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 
				m_pModule->m_NamespaceMgr.CloseNamespace ();
			;
// #line 6621 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 6:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 90 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->m_CurrentAccessKind = (*(_cls32*) GetAstLocator (0)).m_AccessKind;
			;
// #line 6634 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 7:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 107 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			  
				__pSymbol->m_Arg.pName->AddName ((*GetTokenLocator (0)).m_Data.m_String); 
			;
// #line 6647 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 8:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 111 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 
				__pSymbol->m_Arg.pName->AddName ((*GetTokenLocator (1)).m_Data.m_String);
			;
// #line 6660 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 9:
			{
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 149 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return Declare (&__pSymbol->m_Local.Declarator);
			;
// #line 6673 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 10:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 183 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return IsEmptyDeclarationTerminatorAllowed (__pSymbol->m_Arg.pTypeSpecifier);
			;
// #line 6686 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 11:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 187 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return SetDeclarationBody (&(*(_cls2*) GetAstLocator (0)).m_TokenList);
			;
// #line 6699 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 12:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
// #line 206 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_pAnchorToken = &*__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
				(*__pAstNode).m_pAnchorToken->m_Data.m_Integer = 0; // tokens can be reused, ensure cleanup
			;
// #line 6713 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 13:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
// #line 212 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6726 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 14:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 221 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
				__pSymbol->m_Arg.pAnchorToken->m_Data.m_Integer |= ELandingPadFlag_Catch;
			;
// #line 6740 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 15:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 226 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
				__pSymbol->m_Arg.pAnchorToken->m_Data.m_Integer |= ELandingPadFlag_Finally;
			;
// #line 6754 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 16:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 231 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6767 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 17:
			{
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 246 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls30*) GetAstLocator (0)).m_TypeSpecifier);
			;
// #line 6780 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 18:
			{
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 251 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				CType* pType = __pSymbol->m_Local.Declarator.CalcType (__pSymbol->m_Arg.pCountValue);
				if (!pType)
					return false;

				*__pSymbol->m_Arg.ppType = pType;
			;
// #line 6797 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 19:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
// #line 274 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_TypeList.InsertTail ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 6810 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 20:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
// #line 278 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_TypeList.InsertTail ((*(_cls4*) GetAstLocator (1)).m_pType);
			;
// #line 6823 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 21:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
// #line 290 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_pType = (*(_cls4*) GetAstLocator (0)).m_pType;
			;
// #line 6836 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 22:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
// #line 294 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_pType = (*(_cls23*) GetAstLocator (0)).m_Value.GetType ();
			;
// #line 6849 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 23:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 305 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pAttributeBlock = m_pModule->m_AttributeMgr.CreateAttributeBlock ();
			;
// #line 6862 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 24:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 320 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				CAttribute* pAttribute = m_pAttributeBlock->CreateAttribute ((*GetTokenLocator (0)).m_Data.m_String, NULL);
				if (!pAttribute)
					return false;

				pAttribute->m_Pos = (*GetTokenLocator (0)).m_Pos;
			;
// #line 6879 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 25:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 14 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6892 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 26:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6905 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 27:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 23 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6918 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 28:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 28 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6931 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 29:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 32 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6944 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 30:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 37 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6957 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 31:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 41 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6970 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 32:
			{
			CSymbolNode_arg_pass1* __pSymbol = (CSymbolNode_arg_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6983 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 33:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 74 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_ExpressionValue = (*(_cls9*) GetAstLocator (0)).m_Value;
			;
// #line 6996 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 34:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 83 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_ExpressionValueList.TakeOver (&(*(_cls12*) GetAstLocator (0)).m_ValueList);
			;
// #line 7009 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 35:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
// #line 119 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls9*) GetAstLocator (0)).m_Value);
			;
// #line 7022 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 36:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
// #line 123 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls9*) GetAstLocator (1)).m_Value);
			;
// #line 7035 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 37:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 139 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls9*) GetAstLocator (0)).m_Value);
			;
// #line 7048 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 38:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 143 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls10*) GetAstLocator (1)).m_Value);
			;
// #line 7061 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 39:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 147 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail (CValue ());
			;
// #line 7074 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 40:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 151 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls10*) GetAstLocator (0)).m_Value);
			;
// #line 7087 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 41:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
// #line 165 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				if ((*__pAstNode).m_Value.GetValueKind () != EValue_Const)
				{
					err::SetFormatStringError ("not a constant expression");
					return false;
				}
			;
// #line 7104 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 42:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
// #line 181 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
									
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
// #line 7128 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 43:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 210 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("cond_then");
					(*__pAstNode).m_pElseBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("cond_else");
					(*__pAstNode).m_pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("cond_phi");			
					m_pModule->m_ControlFlowMgr.ConditionalJump (*__pSymbol->m_Arg.pValue, (*__pAstNode).m_pThenBlock, (*__pAstNode).m_pElseBlock);
				;
// #line 7144 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 44:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 218 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock ((*__pAstNode).m_pElseBlock); // might have changed				
				;
// #line 7157 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 45:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 223 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return m_pModule->m_OperatorMgr.ConditionalOperator (
						(*__pAstNode).m_TrueValue,
						(*__pAstNode).m_FalseValue,
						(*__pAstNode).m_pThenBlock,
						(*__pAstNode).m_pPhiBlock, 
						__pSymbol->m_Arg.pValue
						);
				;
// #line 7176 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 46:
			{
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 242 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7189 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 47:
			{
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 255 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7202 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 48:
			{
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 268 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7215 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 49:
			{
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 281 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7228 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 50:
			{
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 294 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7241 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 51:
			{
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 307 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7254 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 52:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
// #line 319 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Eq;
		;
// #line 7267 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 53:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
// #line 323 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ne;
		;
// #line 7280 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 54:
			{
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 335 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7293 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 55:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 347 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Lt;
			;
// #line 7306 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 56:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 351 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Gt;
			;
// #line 7319 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 57:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 355 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Le;
			;
// #line 7332 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 58:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 359 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Ge;
			;
// #line 7345 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 59:
			{
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 371 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7358 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 60:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 383 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Shl;
			;
// #line 7371 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 61:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 387 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Shr;
			;
// #line 7384 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 62:
			{
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 399 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls19*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7397 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 63:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 411 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Add;
			;
// #line 7410 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 64:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 415 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Sub;
			;
// #line 7423 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 65:
			{
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 427 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls20*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7436 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 66:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 439 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Mul;
			;
// #line 7449 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 67:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 443 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Div;
			;
// #line 7462 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 68:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 447 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Mod;
			;
// #line 7475 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 69:
			{
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 459 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7488 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 70:
			{
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 480 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls21*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7501 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 71:
			{
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 484 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_CurlyInitializerTargetValue = *__pSymbol->m_Arg.pValue;
			;
// #line 7514 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 72:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 496 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Assign;
			;
// #line 7527 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 73:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 500 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_RefAssign;
			;
// #line 7540 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 74:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 504 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_AddAssign;
			;
// #line 7553 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 75:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 508 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_SubAssign;
			;
// #line 7566 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 76:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 512 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_MulAssign;
			;
// #line 7579 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 77:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 516 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_DivAssign;
			;
// #line 7592 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 78:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 520 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ModAssign;
			;
// #line 7605 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 79:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 524 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ShlAssign;
			;
// #line 7618 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 80:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 528 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ShrAssign;
			;
// #line 7631 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 81:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 532 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_AndAssign;
			;
// #line 7644 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 82:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 536 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_XorAssign;
			;
// #line 7657 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 83:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 540 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_OrAssign;
			;
// #line 7670 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 84:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 557 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Plus, __pSymbol->m_Arg.pValue);
			;
// #line 7683 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 85:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 561 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Minus, __pSymbol->m_Arg.pValue);
			;
// #line 7696 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 86:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 565 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
			;
// #line 7709 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 87:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 569 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, __pSymbol->m_Arg.pValue);
			;
// #line 7722 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 88:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 573 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, __pSymbol->m_Arg.pValue);
			;
// #line 7735 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 89:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 577 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
			;
// #line 7748 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 90:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 581 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
			;
// #line 7761 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 91:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 585 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
			;
// #line 7774 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 92:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 589 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_pModule->m_ControlFlowMgr.LockUnwinding ();
			;
// #line 7787 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 93:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 593 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_pModule->m_ControlFlowMgr.UnlockUnwinding ();
			;
// #line 7800 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 94:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 598 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
				return m_pModule->m_OperatorMgr.CastOperator (StorageKind, __pSymbol->m_Arg.pValue, (*(_cls4*) GetAstLocator (1)).m_pType);
			;
// #line 7814 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 95:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 603 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
				return m_pModule->m_OperatorMgr.NewOperator (
					StorageKind, 
					(*(_cls22*) GetAstLocator (1)).m_pType, 
					(*(_cls22*) GetAstLocator (1)).m_ElementCountValue, 
					&(*(_cls22*) GetAstLocator (1)).m_ArgList, 
					__pSymbol->m_Arg.pValue
					);
			;
// #line 7834 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 96:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 615 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				bool Result = m_pModule->m_OperatorMgr.DeleteOperator (*__pSymbol->m_Arg.pValue);
				if (!Result)
					return false;

				__pSymbol->m_Arg.pValue->SetVoid ();
			;
// #line 7851 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 97:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
// #line 634 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
				(*__pAstNode).m_ArgList.TakeOver (&(*(_cls12*) GetAstLocator (1)).m_ValueList);
			;
// #line 7865 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 98:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 675 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.CallOperator (__pSymbol->m_Arg.pValue, &(*(_cls12*) GetAstLocator (0)).m_ValueList);
			;
// #line 7878 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 99:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 679 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.ClosureOperator (__pSymbol->m_Arg.pValue, &(*(_cls12*) GetAstLocator (0)).m_ValueList);
			;
// #line 7891 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 100:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 683 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls10*) GetAstLocator (0)).m_Value);
			;
// #line 7904 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 101:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 687 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
			;
// #line 7917 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 102:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 691 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
			;
// #line 7930 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 103:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 696 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.WeakenOperator (__pSymbol->m_Arg.pValue);
			;
// #line 7943 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 104:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 702 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
			;
// #line 7956 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 105:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 710 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.MemberOperator (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 7969 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 106:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 714 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyGetter (__pSymbol->m_Arg.pValue);
			;
// #line 7982 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 107:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 718 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertySetter (__pSymbol->m_Arg.pValue);
			;
// #line 7995 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 108:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 735 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return LookupIdentifier ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
			;
// #line 8008 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 109:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 739 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return GetThisValue (__pSymbol->m_Arg.pValue);
			;
// #line 8021 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 110:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 743 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstInt64_u ((*GetTokenLocator (0)).m_Data.m_Int64_u);
			;
// #line 8034 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 111:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 747 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstDouble ((*GetTokenLocator (0)).m_Data.m_Double);
			;
// #line 8047 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 112:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 751 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstBool (true);
			;
// #line 8060 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 113:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 755 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstBool (false);
			;
// #line 8073 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 114:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 759 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetNull ();
			;
// #line 8086 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 115:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 763 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				__pSymbol->m_Arg.pValue->SetConstSizeT (pType->GetSize ());
			;
// #line 8100 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 116:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 768 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				return GetCountOf (pType, __pSymbol->m_Arg.pValue);
			;
// #line 8114 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 117:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 773 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				__pSymbol->m_Arg.pValue->SetType ((*(_cls6*) GetAstLocator (0)).m_pType);
			;
// #line 8128 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 118:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 778 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			;
// #line 8140 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 119:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 781 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyOnChange (__pSymbol->m_Arg.pValue);
			;
// #line 8153 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 120:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 794 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
						
					__pSymbol->m_Local.Literal.m_BinData.Append ((const uchar_t*) (*GetTokenLocator (0)).m_Data.m_String.cc (), (*GetTokenLocator (0)).m_Data.m_String.GetLength ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_Literal;
				;
// #line 8167 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 121:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 799 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
						
					__pSymbol->m_Local.Literal.m_BinData.Append ((*GetTokenLocator (1)).m_Data.m_BinData, (*GetTokenLocator (1)).m_Data.m_BinData.GetCount ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_HexLiteral;
				;
// #line 8181 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 122:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 804 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return AppendFmtLiteral (&__pSymbol->m_Local.Literal, (*GetTokenLocator (2)).m_Data.m_String.cc (), (*GetTokenLocator (2)).m_Data.m_String.GetLength ());
				;
// #line 8194 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 123:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 808 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return IsValidLocator ((*GetTokenLocator (3))) ? 
						AppendFmtLiteralValue (&__pSymbol->m_Local.Literal, (*(_cls9*) GetAstLocator (4)).m_Value, (*GetTokenLocator (3)).m_Data.m_String) :
						AppendFmtLiteralValue (&__pSymbol->m_Local.Literal, (*(_cls9*) GetAstLocator (4)).m_Value, rtl::CString ());
				;
// #line 8209 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 124:
			{
			CSymbolNode_literal* __pSymbol = (CSymbolNode_literal*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 814 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return FinalizeLiteral (&__pSymbol->m_Local.Literal, __pSymbol->m_Arg.pValue);
			;
// #line 8222 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 125:
			{
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 836 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				if (!__pSymbol->m_Local.Initializer.m_Count)
				{
					err::SetFormatStringError ("empty curly initializer");
					return false;
				}
			;
// #line 8239 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 126:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 851 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			 return true; ;
// #line 8250 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 127:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 853 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerNamedItem (__pSymbol->m_Arg.pInitializer, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 8263 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 128:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 858 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, __pSymbol->m_Arg.pInitializer->m_MemberValue, (*(_cls9*) GetAstLocator (1)).m_Value);
				;
// #line 8276 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 129:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 862 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerIndexedItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 8289 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 130:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 866 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, __pSymbol->m_Arg.pInitializer->m_MemberValue, (*(_cls9*) GetAstLocator (0)).m_Value);
			;
// #line 8302 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 131:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 869 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerIndexedItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 8315 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 132:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 874 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return SkipCurlyInitializerItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 8328 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 133:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 6 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				m_ExpressionValue = (*(_cls23*) GetAstLocator (0)).m_Value;
			;
// #line 8341 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 134:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 36 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (0)).m_Value);
			;
// #line 8354 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 135:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 40 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (1)).m_Value);
			;
// #line 8367 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 136:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 56 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (0)).m_Value);
			;
// #line 8380 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 137:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 60 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls24*) GetAstLocator (1)).m_Value);
			;
// #line 8393 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 138:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail (CValue ());
			;
// #line 8406 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 139:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 68 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls24*) GetAstLocator (0)).m_Value);
			;
// #line 8419 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 140:
			{
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 85 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetConditionalOperatorResultType (__pSymbol->m_Local.TrueValue, __pSymbol->m_Local.FalseValue, __pSymbol->m_Arg.pValue);
			;
// #line 8432 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 141:
			{
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 98 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8445 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 142:
			{
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 111 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8458 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 143:
			{
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 124 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8471 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 144:
			{
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 137 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8484 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 145:
			{
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 150 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8497 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 146:
			{
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 163 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8510 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 147:
			{
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 176 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8523 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 148:
			{
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 189 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8536 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 149:
			{
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 202 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls19*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8549 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 150:
			{
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 215 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls20*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8562 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 151:
			{
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 228 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8575 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 152:
			{
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 241 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					if (IsReactor ())
						m_ReactorBindSiteCount = 0; // reset on assignment
				;
// #line 8589 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 153:
			{
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 246 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls21*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
				;
// #line 8602 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 154:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 264 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Plus, __pSymbol->m_Arg.pValue);
			;
// #line 8615 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 155:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 268 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Minus, __pSymbol->m_Arg.pValue);
			;
// #line 8628 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 156:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 272 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
			;
// #line 8641 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 157:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 276 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Addr, __pSymbol->m_Arg.pValue);
			;
// #line 8654 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 158:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 280 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Indir, __pSymbol->m_Arg.pValue);
			;
// #line 8667 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 159:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 284 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
			;
// #line 8680 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 160:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 288 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
			;
// #line 8693 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 161:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
			;
// #line 8706 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 162:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 296 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				// try operator does not change type of value				
			;
// #line 8719 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 163:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 300 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_SizeT);
			;
// #line 8732 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 164:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 304 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_SizeT);
			;
// #line 8745 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 165:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 308 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				CType* pType = (*(_cls6*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? ((CDataPtrType*) (*(_cls6*) GetAstLocator (0)).m_pType)->GetTargetType () : (*(_cls6*) GetAstLocator (0)).m_pType;
				__pSymbol->m_Arg.pValue->SetType ((*(_cls6*) GetAstLocator (0)).m_pType);
			;
// #line 8759 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 166:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 313 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
			;
// #line 8771 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 167:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 316 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyOnChangeType (__pSymbol->m_Arg.pValue);
			;
// #line 8784 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 168:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 321 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 8797 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 169:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 325 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
				return NewOperator_s (StorageKind, (*(_cls27*) GetAstLocator (1)).m_pType, __pSymbol->m_Arg.pValue);			
			;
// #line 8811 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 170:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 330 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetVoid ();
			;
// #line 8824 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 171:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
// #line 341 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_pType = (*(_cls4*) GetAstLocator (0)).m_pType;
			;
// #line 8837 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 172:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
// #line 346 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
			;
// #line 8850 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 173:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 363 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetCallOperatorResultType (__pSymbol->m_Arg.pValue, &(*(_cls26*) GetAstLocator (0)).m_ValueList);
			;
// #line 8863 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 174:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 367 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetClosureOperatorResultType (__pSymbol->m_Arg.pValue, &(*(_cls26*) GetAstLocator (0)).m_ValueList);
			;
// #line 8876 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 175:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 371 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls24*) GetAstLocator (0)).m_Value);
			;
// #line 8889 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 176:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 375 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
			;
// #line 8902 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 177:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 379 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
			;
// #line 8915 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 178:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 384 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetWeakenOperatorResultType (__pSymbol->m_Arg.pValue);
			;
// #line 8928 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 179:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 390 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
			;
// #line 8941 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 180:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 398 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetMemberOperatorResultType (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 8954 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 181:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 402 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyGetterType (__pSymbol->m_Arg.pValue);
			;
// #line 8967 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 182:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 406 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertySetterType (__pSymbol->m_Arg.pValue);
			;
// #line 8980 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 183:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 418 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return LookupIdentifierType ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
			;
// #line 8993 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 184:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 422 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return GetThisValueType (__pSymbol->m_Arg.pValue);
			;
// #line 9006 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 185:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 426 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (GetInt64TypeKind ((*GetTokenLocator (0)).m_Data.m_Int64));
			;
// #line 9019 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 186:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 430 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Double);
			;
// #line 9032 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 187:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 434 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Bool);
			;
// #line 9045 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 188:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 438 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Bool);
			;
// #line 9058 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 189:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 442 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetNull ();
			;
// #line 9071 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 190:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 455 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
						
					__pSymbol->m_Local.Literal.m_BinData.Append ((const uchar_t*) (*GetTokenLocator (0)).m_Data.m_String.cc (), (*GetTokenLocator (0)).m_Data.m_String.GetLength ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_Literal;
				;
// #line 9085 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 191:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 460 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
						
					__pSymbol->m_Local.Literal.m_BinData.Append ((*GetTokenLocator (1)).m_Data.m_BinData, (*GetTokenLocator (1)).m_Data.m_BinData.GetCount ());
					__pSymbol->m_Local.Literal.m_LastToken = EToken_HexLiteral;
				;
// #line 9099 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 192:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 465 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					// mark we have a formatting literal from now on

					if (!__pSymbol->m_Local.Literal.m_FmtLiteralValue)
						__pSymbol->m_Local.Literal.m_FmtLiteralValue.SetConstBool (true); 
				;
// #line 9115 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 193:
			{
			CSymbolNode_literal_s* __pSymbol = (CSymbolNode_literal_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 472 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return FinalizeLiteral_s (&__pSymbol->m_Local.Literal, __pSymbol->m_Arg.pValue);
			;
// #line 9128 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 194:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 6 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
				PreCreateLandingPads ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9142 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 195:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 12 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
			;
// #line 9155 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 196:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 20 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
				PreCreateLandingPads ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9169 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 197:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 25 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeBaseTypeMemberConstructBlock ();
			;
// #line 9182 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 198:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 30 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
			;
// #line 9195 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 199:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 42 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return CallBaseTypeMemberConstructor ((*(_cls28*) GetAstLocator (0)).m_Name, &(*(_cls12*) GetAstLocator (1)).m_ValueList);
			;
// #line 9208 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 200:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 46 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return CallBaseTypeConstructor ((*GetTokenLocator (0)).m_Data.m_Integer - 1, &(*(_cls12*) GetAstLocator (1)).m_ValueList);
			;
// #line 9221 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 201:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
// #line 62 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			  
				(*__pAstNode).m_Name.AddName ((*GetTokenLocator (0)).m_Data.m_String); 
			;
// #line 9234 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 202:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
// #line 66 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			 
				(*__pAstNode).m_Name.AddName ((*GetTokenLocator (1)).m_Data.m_String);
			;
// #line 9247 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 203:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 108 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.IfStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9260 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 204:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 114 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.IfStmt_Else (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (2)).m_Pos);
				;
// #line 9273 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 205:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 119 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
				m_pModule->m_ControlFlowMgr.IfStmt_Follow (&__pSymbol->m_Local.Stmt, IsValidLocator ((*GetAstLocator (3))) ? (*GetAstLocator (3)).m_LastToken.m_Pos : (*GetAstLocator (4)).m_LastToken.m_Pos);
			;
// #line 9286 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 206:
			{
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 134 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return 
					m_pModule->m_ControlFlowMgr.SwitchStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos) &&
					PreCreateLandingPads ((*GetTokenLocator (1)).m_Data.m_Integer);
			;
// #line 9301 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 207:
			{
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 140 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.SwitchStmt_Follow (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (2)).m_Pos);
			;
// #line 9314 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 208:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 147 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.SwitchStmt_Case (__pSymbol->m_Arg.pStmt, (*(_cls14*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos, (*GetTokenLocator (2)).m_Pos);
			;
// #line 9327 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 209:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 151 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.SwitchStmt_Default (__pSymbol->m_Arg.pStmt, (*GetTokenLocator (0)).m_Pos, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9340 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 210:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 167 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.WhileStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9353 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 211:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 171 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.WhileStmt_Follow (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
			;
// #line 9366 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 212:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 186 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.DoStmt_PreBody (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
			;
// #line 9379 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 213:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 190 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.DoStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (1)).m_LastToken.m_Pos);
			;
// #line 9392 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 214:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 194 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.DoStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (2)).m_Value);
			;
// #line 9405 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 215:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 209 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PreInit (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
			;
// #line 9418 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 216:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 214 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PreCondition (&__pSymbol->m_Local.Stmt);
				;
// #line 9431 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 217:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 218 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					return m_pModule->m_ControlFlowMgr.ForStmt_PostCondition (&__pSymbol->m_Local.Stmt, (*(_cls9*) GetAstLocator (1)).m_Value);
				;
// #line 9444 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 218:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 223 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PreLoop (&__pSymbol->m_Local.Stmt);
				;
// #line 9457 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 219:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 227 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PostLoop (&__pSymbol->m_Local.Stmt);
				;
// #line 9470 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 220:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 231 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PreBody (&__pSymbol->m_Local.Stmt);
			;
// #line 9483 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 221:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 235 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
			;
// #line 9496 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 222:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 242 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Break ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9509 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 223:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 249 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Continue ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 9522 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 224:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 256 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Return (IsValidLocator ((*(_cls9*) GetAstLocator (0))) ? (*(_cls9*) GetAstLocator (0)).m_Value : CValue (), IsValidLocator ((*GetTokenLocator (1))));
			;
// #line 9535 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 225:
			{
			CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 267 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Static;
				return 
					m_pModule->m_ControlFlowMgr.OnceStmt_Create (&__pSymbol->m_Local.Stmt, StorageKind) &&				
					m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9551 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 226:
			{
			CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 274 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.OnceStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
			;
// #line 9564 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 227:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 282 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
				PreCreateLandingPads ((*GetTokenLocator (0)).m_Data.m_Integer | ELandingPadFlag_Catch); // always catch
			;
// #line 9578 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 228:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 288 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.EndTry ();
				m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
			;
// #line 9592 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 229:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 297 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Catch ();
			;
// #line 9605 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 230:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 304 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Finally ();
			;
// #line 9618 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 231:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 314 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
			return FinalizeReactor ();
		;
// #line 9631 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 232:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 320 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			 return true; ;
// #line 9642 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 233:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 332 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeReactorOnChangeClause ();
			;
// #line 9655 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 234:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 336 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeReactorOnChangeStmt ();
			;
// #line 9668 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 235:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 343 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return ReactorExpressionStmt (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 9681 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 236:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 84 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			 return true; ;
// #line 9692 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 237:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 92 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			
				return CountReactorBindSites ();
			;
// #line 9705 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 238:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 100 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			
				return CountReactorBindSites ();
			;
// #line 9718 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 239:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 22 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return SetStorageKind ((*(_cls31*) GetAstLocator (0)).m_StorageKind);
			;
// #line 9731 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 240:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 26 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return SetAccessKind ((*(_cls32*) GetAstLocator (0)).m_AccessKind);
			;
// #line 9744 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 241:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 30 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls33*) GetAstLocator (0)).m_pType);
			;
// #line 9757 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 242:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 34 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls36*) GetAstLocator (0)).m_TypeModifier);
			;
// #line 9770 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 243:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 60 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls33*) GetAstLocator (0)).m_pType);
			;
// #line 9783 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 244:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls36*) GetAstLocator (0)).m_TypeModifier);
			;
// #line 9796 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 245:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 77 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Typedef;
			;
// #line 9809 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 246:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 81 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Alias;
			;
// #line 9822 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 247:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 85 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Static;
			;
// #line 9835 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 248:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 89 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
						
				(*__pAstNode).m_StorageKind = EStorage_Thread;
			;
// #line 9848 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 249:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 93 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Stack;
			;
// #line 9861 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 250:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 97 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Heap;
			;
// #line 9874 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 251:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 101 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_HeapU;
			;
// #line 9887 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 252:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 105 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Abstract;
			;
// #line 9900 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 253:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 109 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Virtual;
			;
// #line 9913 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 254:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 113 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Override;
			;
// #line 9926 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 255:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 117 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Mutable;
			;
// #line 9939 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 256:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 121 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Nullable;
			;
// #line 9952 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 257:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
// #line 134 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_AccessKind = EAccess_Public;
			;
// #line 9965 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 258:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
// #line 138 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_AccessKind = EAccess_Protected;
			;
// #line 9978 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 259:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 151 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
			;
// #line 9991 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 260:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 155 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				err::SetStringError ("'variant' type specifier is not yet supported");
				return false; 
			;
// #line 10005 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 261:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 160 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectClass);
			;
// #line 10018 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 262:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 164 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetBoxClassType ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 10031 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 263:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 168 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Bool);
			;
// #line 10044 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 264:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 172 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int8);
			;
// #line 10057 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 265:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 176 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int16);
			;
// #line 10070 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 266:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 180 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32);
			;
// #line 10083 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 267:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 184 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64);
			;
// #line 10096 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 268:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 188 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int);
			;
// #line 10109 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 269:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 192 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
			;
// #line 10122 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 270:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 196 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char);
			;
// #line 10135 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 271:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 200 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Float);
			;
// #line 10148 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 272:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 204 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Double);
			;
// #line 10161 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 273:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 208 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				err::SetStringError ("'auto' type specifier is not yet supported");
				return false; 
			;
// #line 10175 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 274:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 214 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls35*) GetAstLocator (0)).m_pType;
			;
// #line 10188 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 275:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 219 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
			;
// #line 10201 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 276:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 227 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				if (IsTypeSpecified ())
					return false;
			;
// #line 10215 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 277:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 232 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return m_Stage == EStage_Pass1 || FindType ((*(_cls1*) GetAstLocator (0)).m_Name) != NULL;
			;
// #line 10228 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 278:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 236 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return false;
			;
// #line 10241 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 279:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
// #line 248 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				CType* pType = FindType ((*(_cls1*) GetAstLocator (0)).m_Name);
				if (!pType)
				{
					err::SetFormatStringError ("'%s' is not a type", (*(_cls1*) GetAstLocator (0)).m_Name.GetFullName ().cc ());
					return false;
				}

				(*__pAstNode).m_pType = pType;
			;
// #line 10261 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 280:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 270 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				__pSymbol->m_Local.pTemplate = CreatePropertyTemplate ();
			;
// #line 10274 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 281:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 274 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = __pSymbol->m_Local.pTemplate->CalcType ();
				return (*__pAstNode).m_pType != NULL;
			;
// #line 10288 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 282:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 288 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Unsigned;
			;
// #line 10301 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 283:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 292 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_BigEndian;
			;
// #line 10314 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 284:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 296 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Const;
			;
// #line 10327 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 285:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 300 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_ConstD;
			;
// #line 10340 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 286:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 304 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Volatile;
			;
// #line 10353 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 287:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 308 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Weak;
			;
// #line 10366 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 288:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 312 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Thin;
			;
// #line 10379 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 289:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 316 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Unsafe;
			;
// #line 10392 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 290:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 320 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Unwinder;
			;
// #line 10405 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 291:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 324 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Cdecl;
			;
// #line 10418 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 292:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 328 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Stdcall;
			;
// #line 10431 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 293:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 332 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Function;
			;
// #line 10444 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 294:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 336 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Property;
			;
// #line 10457 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 295:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 340 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Bindable;
			;
// #line 10470 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 296:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 344 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_AutoGet;
			;
// #line 10483 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 297:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 348 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Indexed;
			;
// #line 10496 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 298:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 352 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Multicast;
			;
// #line 10509 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 299:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 356 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Event;
			;
// #line 10522 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 300:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 360 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_EventD;
			;
// #line 10535 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 301:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 364 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_TypeModifier = ETypeModifier_Reactor;
			;
// #line 10548 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 302:
			{
			CSymbolNode_unwinder_modifier* __pSymbol = (CSymbolNode_unwinder_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 373 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetUnwinderExpression (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 10561 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 303:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 14 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*(_cls37*) GetAstLocator (0)).m_Declarator.m_Constructor.TakeOver (&(*(_cls8*) GetAstLocator (1)).m_TokenList);
			;
// #line 10574 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 304:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*(_cls37*) GetAstLocator (0)).m_Declarator.m_Initializer.TakeOver (&(*(_cls7*) GetAstLocator (2)).m_TokenList);
			;
// #line 10587 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 305:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 23 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			 
				return Declare (&(*(_cls37*) GetAstLocator (0)).m_Declarator);
			;
// #line 10600 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 306:
			{
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 49 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->m_Pos = (*GetAstLocator (0)).m_FirstToken.m_Pos;
			;
// #line 10613 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 307:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 57 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddName ((*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 10626 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 308:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 61 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Getter);
			;
// #line 10639 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 309:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 65 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Setter);
			;
// #line 10652 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 310:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 69 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_PreConstructor);
			;
// #line 10665 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 311:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 73 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Constructor);
			;
// #line 10678 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 312:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 77 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Destructor);
			;
// #line 10691 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 313:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 81 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticConstructor);
			;
// #line 10704 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 314:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 85 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticDestructor);
			;
// #line 10717 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 315:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 89 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_CallOperator);
			;
// #line 10730 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 316:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 93 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddCastOperator ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 10743 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 317:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 97 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnaryBinaryOperator ((*(_cls38*) GetAstLocator (0)).m_UnOpKind, (*(_cls38*) GetAstLocator (0)).m_BinOpKind);
			;
// #line 10756 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 318:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 116 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_BwNot;
			;
// #line 10769 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 319:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 120 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_LogNot;
			;
// #line 10782 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 320:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 124 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PreInc;
			;
// #line 10795 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 321:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 128 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PreDec;
			;
// #line 10808 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 322:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 132 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PostInc;
			;
// #line 10821 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 323:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 136 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PostDec;
			;
// #line 10834 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 324:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 140 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Ptr;
			;
// #line 10847 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 325:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 144 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Plus;
				(*__pAstNode).m_BinOpKind = EBinOp_Add;
			;
// #line 10861 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 326:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 149 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Minus;
				(*__pAstNode).m_BinOpKind = EBinOp_Sub;
			;
// #line 10875 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 327:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 154 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Indir;
				(*__pAstNode).m_BinOpKind = EBinOp_Mul;
			;
// #line 10889 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 328:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 159 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Div;
			;
// #line 10902 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 329:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 163 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Mod;
			;
// #line 10915 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 330:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 167 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Shl;
			;
// #line 10928 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 331:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 171 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Shr;
			;
// #line 10941 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 332:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 175 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Addr;
				(*__pAstNode).m_BinOpKind = EBinOp_BwAnd;
			;
// #line 10955 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 333:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 180 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_BwXor;
			;
// #line 10968 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 334:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 184 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_BwOr;
			;
// #line 10981 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 335:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 188 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Eq;
			;
// #line 10994 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 336:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 192 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Ne;
			;
// #line 11007 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 337:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 196 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Lt;
			;
// #line 11020 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 338:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 200 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Le;
			;
// #line 11033 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 339:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 204 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Gt;
			;
// #line 11046 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 340:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 208 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Ge;
			;
// #line 11059 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 341:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 212 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Assign;
			;
// #line 11072 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 342:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 216 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_RefAssign;
			;
// #line 11085 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 343:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 220 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AddAssign;
			;
// #line 11098 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 344:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 224 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_SubAssign;
			;
// #line 11111 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 345:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 228 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_MulAssign;
			;
// #line 11124 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 346:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 232 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_DivAssign;
			;
// #line 11137 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 347:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 236 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ModAssign;
			;
// #line 11150 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 348:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 240 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ShlAssign;
			;
// #line 11163 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 349:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 244 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ShrAssign;
			;
// #line 11176 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 350:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 248 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AndAssign;
			;
// #line 11189 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 351:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 252 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_XorAssign;
			;
// #line 11202 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 352:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 256 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_OrAssign;
			;
// #line 11215 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 353:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 260 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AtAssign;
			;
// #line 11228 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 354:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 264 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_LogAnd;
			;
// #line 11241 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 355:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 268 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_LogOr;
			;
// #line 11254 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 356:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 272 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Idx;
			;
// #line 11267 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 357:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 276 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_At;
			;
// #line 11280 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 358:
			{
			CSymbolNode_declarator_prefix* __pSymbol = (CSymbolNode_declarator_prefix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 288 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddPointerPrefix ();
			;
// #line 11293 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 359:
			{
			CSymbolNode_declarator_prefix* __pSymbol = (CSymbolNode_declarator_prefix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->SetTypeModifier ((*(_cls36*) GetAstLocator (0)).m_TypeModifier);
			;
// #line 11306 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 360:
			{
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 304 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			 return false; ;
// #line 11317 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 361:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 312 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 11330 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 362:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 316 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 11343 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 363:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 320 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix (-1);
			;
// #line 11356 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 364:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 367 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier);
			;
// #line 11369 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 365:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 374 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.m_Initializer.TakeOver (&(*(_cls7*) GetAstLocator (1)).m_TokenList);
			;
// #line 11382 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 366:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 378 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return CreateFormalArg (__pSymbol->m_Arg.pFunctionSuffix, &__pSymbol->m_Local.Declarator) != NULL;
			;
// #line 11395 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 367:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 397 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.m_Initializer.TakeOver (&(*(_cls7*) GetAstLocator (0)).m_TokenList);
			;
// #line 11408 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 368:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 401 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return CreateFormalArg (__pSymbol->m_Arg.pFunctionSuffix, &__pSymbol->m_Local.Declarator) != NULL;
			;
// #line 11421 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 369:
			{
			CSymbolNode_function_formal_argument_ellipsis* __pSymbol = (CSymbolNode_function_formal_argument_ellipsis*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 408 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_VarArg;
				if (IsValidLocator ((*GetTokenLocator (0))))
					__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_UnsafeVarArg;
			;
// #line 11436 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 370:
			{
			CSymbolNode_bitfield_suffix* __pSymbol = (CSymbolNode_bitfield_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 419 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddBitFieldSuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 11449 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 371:
			{
			CSymbolNode_post_declarator_modifier* __pSymbol = (CSymbolNode_post_declarator_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 428 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->SetPostDeclaratorModifier (EPostDeclaratorModifier_Const);
			;
// #line 11462 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 372:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 11 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls40*) GetAstLocator (0)).m_pType;
			;
// #line 11475 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 373:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 15 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls42*) GetAstLocator (0)).m_pType;
			;
// #line 11488 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 374:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 19 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls45*) GetAstLocator (0)).m_pType;
			;
// #line 11501 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 375:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 23 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls46*) GetAstLocator (0)).m_pType;
			;
// #line 11514 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 376:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
// #line 45 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateEnumType (
					(*(_cls41*) GetAstLocator (0)).m_EnumTypeKind,
					IsValidLocator ((*GetTokenLocator (1))) ? (*GetTokenLocator (1)).m_Data.m_String : rtl::CString (),
					IsValidLocator ((*(_cls4*) GetAstLocator (2))) ? (*(_cls4*) GetAstLocator (2)).m_pType : NULL,
					(*(_cls41*) GetAstLocator (0)).m_Flags
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11535 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 377:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 68 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_EnumTypeKind = EEnumType_Normal;
				(*__pAstNode).m_Flags = 0;
			;
// #line 11549 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 378:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 73 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_EnumTypeKind = EEnumType_Flag;
				(*__pAstNode).m_Flags = 0;
			;
// #line 11563 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 379:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 78 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_EnumTypeKind = EEnumType_Normal;
				(*__pAstNode).m_Flags = EEnumTypeFlag_Exposed;
			;
// #line 11577 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 380:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 88 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
			;
// #line 11590 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 381:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 93 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pModule->m_NamespaceMgr.CloseNamespace ();	
			;
// #line 11603 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 382:
			{
			CSymbolNode_enum_member* __pSymbol = (CSymbolNode_enum_member*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 106 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				CEnumConst* pMember = __pSymbol->m_Arg.pType->CreateConst (
					(*GetTokenLocator (0)).m_Data.m_String,
					IsValidLocator ((*(_cls7*) GetAstLocator (1))) ? &(*(_cls7*) GetAstLocator (1)).m_TokenList : NULL
					);

				if (!pMember)
					return false;

				pMember->m_Pos = (*GetTokenLocator (0)).m_Pos;
			;
// #line 11624 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 383:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
// #line 133 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateStructType (
					IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
					IsValidLocator ((*(_cls5*) GetAstLocator (1))) ? &(*(_cls5*) GetAstLocator (1)).m_TypeList : NULL,
					IsValidLocator ((*(_cls43*) GetAstLocator (2))) ? (*(_cls43*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11644 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 384:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls43* __pAstNode = (_cls43*) __pSymbol->m_pAstNode;
// #line 154 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_PackFactor = (*(_cls44*) GetAstLocator (0)).m_Value;
			;
// #line 11657 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 385:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
// #line 165 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_Value = (*GetTokenLocator (0)).m_Data.m_Integer;
			;
// #line 11670 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 386:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
// #line 169 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_Value = m_DefaultStructPackFactor;
			;
// #line 11683 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 387:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
// #line 185 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateUnionType (IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString ());
				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11698 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 388:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
// #line 206 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateClassType (
					(*GetTokenLocator (0)).m_Data.m_String,
					IsValidLocator ((*(_cls5*) GetAstLocator (1))) ? &(*(_cls5*) GetAstLocator (1)).m_TypeList : NULL,
					IsValidLocator ((*(_cls43*) GetAstLocator (2))) ? (*(_cls43*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11718 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
			
// #line 102 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pName = &(*__pAstNode).m_Name;
// #line 11746 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_declarator_list* __pTarget = (CSymbolNode_declarator_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 128 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier;
// #line 11759 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 129 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier;
// #line 11772 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 145 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11785 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 146 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11798 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 147 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11811 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 147 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11824 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 148 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11837 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 152 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = NULL;
// #line 11850 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier_rslv* __pSymbol = (CSymbolNode_item_declaration_no_specifier_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 166 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11863 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
// #line 197 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 11876 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_statement_pass1* __pTarget = (CSymbolNode_statement_pass1*) pSymbol;
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
			
// #line 210 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11889 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 210 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pAnchorToken =  (*__pAstNode).m_pAnchorToken;
// #line 11893 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 219 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11906 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 249 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11919 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 250 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11932 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_type_name_impl* __pTarget = (CSymbolNode_type_name_impl*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
			
// #line 265 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.ppType = &(*__pAstNode).m_pType;
// #line 11945 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 265 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pCountValue =  NULL;
// #line 11949 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
			
// #line 8 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 11962 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 17 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11975 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 26 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11988 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 35 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12001 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_arg_pass1* __pTarget = (CSymbolNode_arg_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
			
// #line 55 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 12014 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_arg_pass1* __pTarget = (CSymbolNode_arg_pass1*) pSymbol;
			CSymbolNode_arg_pass1* __pSymbol = (CSymbolNode_arg_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 61 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12027 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
			
// #line 100 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12040 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
			
// #line 109 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12053 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
			
// #line 164 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12066 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_logical_or_expr* __pTarget = (CSymbolNode_logical_or_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
// #line 208 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12079 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
// #line 217 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_TrueValue;
// #line 12092 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
			
// #line 222 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_FalseValue;
// #line 12105 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 240 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12118 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 241 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12131 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 253 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12144 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 254 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12157 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 266 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12170 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 267 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12183 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 279 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12196 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 280 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12209 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 292 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12222 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 293 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12235 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 305 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12248 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 306 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12261 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 333 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12274 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 41:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 334 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12287 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 42:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 369 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12300 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 43:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 370 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12313 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 44:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 397 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12326 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 45:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 398 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12339 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 46:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 425 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12352 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 47:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 426 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12365 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 48:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 457 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12378 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 49:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 458 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12391 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 50:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 466 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12404 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 51:
			{
			CSymbolNode_assignment_operator_expr* __pTarget = (CSymbolNode_assignment_operator_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 466 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12417 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 52:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 479 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12430 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 53:
			{
			CSymbolNode_postfix_expr* __pTarget = (CSymbolNode_postfix_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 546 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12443 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 54:
			{
			CSymbolNode_unary_operator_expr* __pTarget = (CSymbolNode_unary_operator_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 547 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12456 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 55:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 556 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12469 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 56:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 560 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12482 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 57:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 564 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12495 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 58:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 568 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12508 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 59:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 572 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12521 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 60:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 576 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12534 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 61:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 580 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12547 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 62:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 584 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12560 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 63:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 592 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12573 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 64:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 597 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12586 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 65:
			{
			CSymbolNode_new_operator_curly_initializer* __pTarget = (CSymbolNode_new_operator_curly_initializer*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 613 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12599 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 66:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 614 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12612 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 67:
			{
			CSymbolNode_type_name_impl* __pTarget = (CSymbolNode_type_name_impl*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
			
// #line 631 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.ppType = &(*__pAstNode).m_pType;
// #line 12625 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 631 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pCountValue =  &(*__pAstNode).m_ElementCountValue;
// #line 12629 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 68:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_cast_operator_rslv* __pSymbol = (CSymbolNode_cast_operator_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 660 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTypeSpecifier = &__pSymbol->m_Local.TypeSpecifier;
// #line 12642 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 69:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 664 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12655 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 70:
			{
			CSymbolNode_postfix_operator* __pTarget = (CSymbolNode_postfix_operator*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 665 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12668 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 71:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 700 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12681 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 72:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 705 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12694 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 73:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 732 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12707 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 74:
			{
			CSymbolNode_literal* __pTarget = (CSymbolNode_literal*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 733 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12720 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 75:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 780 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12733 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 76:
			{
			CSymbolNode_curly_initializer_item* __pTarget = (CSymbolNode_curly_initializer_item*) pSymbol;
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 835 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pInitializer = &__pSymbol->m_Local.Initializer;
// #line 12746 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 77:
			{
			CSymbolNode_curly_initializer_item* __pTarget = (CSymbolNode_curly_initializer_item*) pSymbol;
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 835 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pInitializer = &__pSymbol->m_Local.Initializer;
// #line 12759 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 78:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls23* __pAstNode = (_cls23*) __pSymbol->m_pAstNode;
			
// #line 17 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12772 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 79:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
			
// #line 26 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12785 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 80:
			{
			CSymbolNode_logical_or_expr_s* __pTarget = (CSymbolNode_logical_or_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 83 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12798 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 81:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 84 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.TrueValue;
// #line 12811 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 82:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 84 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.FalseValue;
// #line 12824 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 83:
			{
			CSymbolNode_logical_and_expr_s* __pTarget = (CSymbolNode_logical_and_expr_s*) pSymbol;
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 96 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12837 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 84:
			{
			CSymbolNode_logical_and_expr_s* __pTarget = (CSymbolNode_logical_and_expr_s*) pSymbol;
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 97 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12850 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 85:
			{
			CSymbolNode_inclusive_or_expr_s* __pTarget = (CSymbolNode_inclusive_or_expr_s*) pSymbol;
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 109 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12863 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 86:
			{
			CSymbolNode_inclusive_or_expr_s* __pTarget = (CSymbolNode_inclusive_or_expr_s*) pSymbol;
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 110 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12876 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 87:
			{
			CSymbolNode_exclusive_or_expr_s* __pTarget = (CSymbolNode_exclusive_or_expr_s*) pSymbol;
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 122 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12889 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 88:
			{
			CSymbolNode_exclusive_or_expr_s* __pTarget = (CSymbolNode_exclusive_or_expr_s*) pSymbol;
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 123 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12902 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 89:
			{
			CSymbolNode_and_expr_s* __pTarget = (CSymbolNode_and_expr_s*) pSymbol;
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 135 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12915 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 90:
			{
			CSymbolNode_and_expr_s* __pTarget = (CSymbolNode_and_expr_s*) pSymbol;
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 136 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12928 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 91:
			{
			CSymbolNode_equality_expr_s* __pTarget = (CSymbolNode_equality_expr_s*) pSymbol;
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 148 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12941 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 92:
			{
			CSymbolNode_equality_expr_s* __pTarget = (CSymbolNode_equality_expr_s*) pSymbol;
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 149 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12954 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 93:
			{
			CSymbolNode_relational_expr_s* __pTarget = (CSymbolNode_relational_expr_s*) pSymbol;
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 161 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12967 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 94:
			{
			CSymbolNode_relational_expr_s* __pTarget = (CSymbolNode_relational_expr_s*) pSymbol;
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 162 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12980 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 95:
			{
			CSymbolNode_shift_expr_s* __pTarget = (CSymbolNode_shift_expr_s*) pSymbol;
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 174 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12993 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 96:
			{
			CSymbolNode_shift_expr_s* __pTarget = (CSymbolNode_shift_expr_s*) pSymbol;
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 175 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13006 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 97:
			{
			CSymbolNode_additive_expr_s* __pTarget = (CSymbolNode_additive_expr_s*) pSymbol;
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 187 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13019 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 98:
			{
			CSymbolNode_additive_expr_s* __pTarget = (CSymbolNode_additive_expr_s*) pSymbol;
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 188 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13032 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 99:
			{
			CSymbolNode_multiplicative_expr_s* __pTarget = (CSymbolNode_multiplicative_expr_s*) pSymbol;
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 200 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13045 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 100:
			{
			CSymbolNode_multiplicative_expr_s* __pTarget = (CSymbolNode_multiplicative_expr_s*) pSymbol;
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 201 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13058 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 101:
			{
			CSymbolNode_at_expr_s* __pTarget = (CSymbolNode_at_expr_s*) pSymbol;
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 213 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13071 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 102:
			{
			CSymbolNode_at_expr_s* __pTarget = (CSymbolNode_at_expr_s*) pSymbol;
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 214 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13084 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 103:
			{
			CSymbolNode_assignment_expr_s* __pTarget = (CSymbolNode_assignment_expr_s*) pSymbol;
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 226 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13097 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 104:
			{
			CSymbolNode_assignment_expr_s* __pTarget = (CSymbolNode_assignment_expr_s*) pSymbol;
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 227 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13110 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 105:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 239 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13123 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 106:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 245 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 13136 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 107:
			{
			CSymbolNode_postfix_expr_s* __pTarget = (CSymbolNode_postfix_expr_s*) pSymbol;
			CSymbolNode_unary_expr_s* __pSymbol = (CSymbolNode_unary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 253 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13149 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 108:
			{
			CSymbolNode_unary_operator_expr_s* __pTarget = (CSymbolNode_unary_operator_expr_s*) pSymbol;
			CSymbolNode_unary_expr_s* __pSymbol = (CSymbolNode_unary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 254 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13162 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 109:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 263 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13175 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 110:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 267 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13188 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 111:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 271 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13201 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 112:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 275 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13214 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 113:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 279 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13227 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 114:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 283 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13240 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 115:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 287 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13253 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 116:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 291 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13266 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 117:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 295 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13279 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 118:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 315 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13292 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 119:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 320 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13305 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 120:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 329 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13318 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 121:
			{
			CSymbolNode_primary_expr_s* __pTarget = (CSymbolNode_primary_expr_s*) pSymbol;
			CSymbolNode_postfix_expr_s* __pSymbol = (CSymbolNode_postfix_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 352 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13331 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 122:
			{
			CSymbolNode_postfix_operator_s* __pTarget = (CSymbolNode_postfix_operator_s*) pSymbol;
			CSymbolNode_postfix_expr_s* __pSymbol = (CSymbolNode_postfix_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 353 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13344 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 123:
			{
			CSymbolNode_member_operator_s* __pTarget = (CSymbolNode_member_operator_s*) pSymbol;
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 388 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13357 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 124:
			{
			CSymbolNode_member_operator_s* __pTarget = (CSymbolNode_member_operator_s*) pSymbol;
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 393 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13370 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 125:
			{
			CSymbolNode_literal_s* __pTarget = (CSymbolNode_literal_s*) pSymbol;
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 445 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13383 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 126:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 446 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 13396 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 127:
			{
			CSymbolNode_switch_block_stmt* __pTarget = (CSymbolNode_switch_block_stmt*) pSymbol;
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 139 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pStmt = &__pSymbol->m_Local.Stmt;
// #line 13409 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 128:
			{
			CSymbolNode_declaration_specifier* __pTarget = (CSymbolNode_declaration_specifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
			
// #line 16 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
// #line 13422 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 129:
			{
			CSymbolNode_unwinder_modifier* __pTarget = (CSymbolNode_unwinder_modifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 37 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13435 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 130:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
			
// #line 55 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
// #line 13448 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 131:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
			
// #line 273 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pNamespace = __pSymbol->m_Local.pTemplate;
// #line 13461 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 132:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 4 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13474 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 133:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 4 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13487 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 134:
			{
			CSymbolNode_declarator* __pTarget = (CSymbolNode_declarator*) pSymbol;
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 12 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 13500 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 135:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 39 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13513 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 136:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 40 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13526 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 137:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 41 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13539 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 138:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
			
// #line 42 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 13552 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 139:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 48 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13565 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 140:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 52 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13578 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 141:
			{
			CSymbolNode_array_suffix* __pTarget = (CSymbolNode_array_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 302 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13591 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 142:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 303 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13604 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 143:
			{
			CSymbolNode_bitfield_suffix* __pTarget = (CSymbolNode_bitfield_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 305 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13617 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 144:
			{
			CSymbolNode_function_formal_argument_list* __pTarget = (CSymbolNode_function_formal_argument_list*) pSymbol;
			CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 342 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Local.pFunctionSuffix;
// #line 13630 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 145:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 347 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13643 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 146:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 348 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13656 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 147:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pTarget = (CSymbolNode_function_formal_argument_w_specifier*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 352 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13669 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 148:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pTarget = (CSymbolNode_function_formal_argument_no_specifier*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 353 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13682 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 149:
			{
			CSymbolNode_function_formal_argument_ellipsis* __pTarget = (CSymbolNode_function_formal_argument_ellipsis*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 354 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13695 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 150:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 370 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13708 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 151:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 371 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13721 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 152:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 372 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13734 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 153:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 393 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13747 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 154:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 394 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13760 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 155:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 395 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13773 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 156:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
			
// #line 56 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
// #line 13786 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 157:
			{
			CSymbolNode_enum_member_list* __pTarget = (CSymbolNode_enum_member_list*) pSymbol;
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 91 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13799 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 158:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 100 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13812 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 159:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 100 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13825 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 160:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
			
// #line 143 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13838 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 161:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
			
// #line 190 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13851 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 162:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
			
// #line 216 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13864 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
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
// #line 123 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
	;
// #line 13899 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_item_declaration_no_specifier:
			{
				CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 140 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
		__pSymbol->m_Local.Declarator.SetTypeSpecifier (NULL);
	;
// #line 13915 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_local_declaration:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 40 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		m_pModule->m_ControlFlowMgr.ResetJumpFlag (); // 'stack new' is currently illegal in conditional expression
		m_pModule->m_LlvmIrBuilder.SetSourcePos (m_CurrentToken.m_Pos);
	;
// #line 13930 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_statement:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 75 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.ResetJumpFlag (); // 'stack new' is currently illegal in conditional expression
		m_pModule->m_LlvmIrBuilder.SetSourcePos (m_CurrentToken.m_Pos);
	;
// #line 13944 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 9 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13962 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_function_suffix:
			{
				CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 334 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		__pSymbol->m_Local.pFunctionSuffix = __pSymbol->m_Arg.pDeclarator->AddFunctionSuffix ();
		__pSymbol->m_Local.PrevStorageKind = m_StorageKind;
	;
// #line 13980 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
// #line 48 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13999 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_expression:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
// #line 97 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		m_pModule->m_LlvmIrBuilder.SetSourcePos (m_CurrentToken.m_Pos);
	;
// #line 14024 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_assignment_operator_expr:
			{
				CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 475 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsReactor ())  // reset on assignment
			m_ReactorBindSiteList.Clear (); 
	;
// #line 14063 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_curly_initializer:
			{
				CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 828 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		ASSERT (m_CurlyInitializerTargetValue);

		__pSymbol->m_Local.Initializer.m_TargetValue = m_CurlyInitializerTargetValue;
		__pSymbol->m_Local.Initializer.m_Index = 0;
		__pSymbol->m_Local.Initializer.m_Count = 0;
	;
// #line 14081 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_new_operator_curly_initializer:
			{
				CSymbolNode_new_operator_curly_initializer* __pSymbol = (CSymbolNode_new_operator_curly_initializer*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 646 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		m_CurlyInitializerTargetValue = *__pSymbol->m_Arg.pValue;
	;
// #line 14099 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_if_stmt:
			{
				CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 104 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.IfStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14151 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_switch_stmt:
			{
				CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 130 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.SwitchStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14164 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_while_stmt:
			{
				CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 163 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.WhileStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14177 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_do_stmt:
			{
				CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 182 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.DoStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14190 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_for_stmt:
			{
				CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 205 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.ForStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 14203 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		case ESymbol_reactor_onchange_stmt:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 328 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_ReactorBindSiteList.Clear (); // clear bindsite list before processing expression list
	;
// #line 14224 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_reactor_stmt_0:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 81 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
				
		m_ReactorBindSiteCount = 0;
	;
// #line 14253 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		case ESymbol_named_type_block:
			{
				CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 225 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pNamespace);
	;
// #line 14274 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_full_declarator:
			{
				CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 9 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		return PreDeclare ();
	;
// #line 14287 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_declarator:
			{
				CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
				_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 36 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		(*__pAstNode).m_Declarator.SetTypeSpecifier (__pSymbol->m_Arg.pTypeSpecifier);
	;
// #line 14300 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_operator_name:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 111 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		(*__pAstNode).m_UnOpKind = EUnOp_Undefined;
		(*__pAstNode).m_BinOpKind = EBinOp_Undefined;
	;
// #line 14315 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_function_formal_argument_w_specifier:
			{
				CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 363 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = EStorage_Undefined;
	;
// #line 14332 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_function_formal_argument_no_specifier:
			{
				CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 389 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = EStorage_Undefined;
		__pSymbol->m_Local.Declarator.SetTypeSpecifier (NULL);
	;
// #line 14346 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 13 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 14402 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_declaration_terminator:
			{
				CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 173 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		if (m_pAttributeBlock)
		{
			err::SetFormatStringError ("unused attribute block in declaration");
			return false;
		}

		m_pLastDeclaredItem = NULL;
	;
// #line 14422 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		case ESymbol_function_suffix:
			{
				CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 339 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = __pSymbol->m_Local.PrevStorageKind;
	;
// #line 14437 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
// #line 52 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 14456 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_unary_operator_expr:
			{
				CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 552 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 14512 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		case ESymbol_primary_expr:
			{
				CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 728 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 14533 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_postfix_operator:
			{
				CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 670 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 14547 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_unary_operator_expr_s:
			{
				CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 259 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteCount++;
	;
// #line 14583 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_primary_expr_s:
			{
				CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 413 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteCount++;
	;
// #line 14598 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_postfix_operator_s:
			{
				CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 358 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsReactor () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_ReactorBindSiteCount++;
	;
// #line 14612 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_named_type_block:
			{
				CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 229 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		m_pModule->m_NamespaceMgr.CloseNamespace ();
	;
// #line 14672 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_enum_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
// #line 38 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		if ((*__pAstNode).m_pType->GetName ().IsEmpty () || ((*__pAstNode).m_pType->GetFlags () & EEnumTypeFlag_Exposed))
			return m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->ExposeEnumConsts ((*__pAstNode).m_pType);
	;
// #line 14697 "/home/vladimir/Projects/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
					
				pTransition->m_ProductionIndex = 1722;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 162;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 1724;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 3:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 162;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 4:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 166;
				pTransition->m_ResolverIndex = 165;
				pTransition->m_ResolverElseIndex = 164;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 5:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 166;
				pTransition->m_ResolverIndex = 165;
				pTransition->m_ResolverElseIndex = 164;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 6:
			
			switch (LookaheadToken)
			{
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 1728;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 7:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 162;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 8:
			
			switch (LookaheadToken)
			{
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 1730;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 9:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 162;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 10:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 166;
				pTransition->m_ResolverIndex = 165;
				pTransition->m_ResolverElseIndex = 164;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 11:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 166;
				pTransition->m_ResolverIndex = 165;
				pTransition->m_ResolverElseIndex = 164;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 12:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 166;
				pTransition->m_ResolverIndex = 992;
				pTransition->m_ResolverElseIndex = 1734;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 13:
			
			pTransition->m_ProductionIndex = 164;
			pTransition->m_ResolverIndex = 312;
			pTransition->m_ResolverElseIndex = 169;
				
			return ELaDfaResult_Resolver;
			
		
		case 14:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1736;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 15:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 16:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1738;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 17:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 18:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 166;
				pTransition->m_ResolverIndex = 992;
				pTransition->m_ResolverElseIndex = 1740;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 19:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 20:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1742;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 21:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 22:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1744;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 23:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 24:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1746;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 25:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 26:
			
			switch (LookaheadToken)
			{
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 1748;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 27:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 28:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1750;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 29:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 30:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1752;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 31:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 32:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1754;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 33:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 34:
			
			switch (LookaheadToken)
			{
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1756;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 35:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 36:
			
			switch (LookaheadToken)
			{
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1758;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 37:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 169;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 38:
			
			switch (LookaheadToken)
			{
			
			case '*':
					
				pTransition->m_ProductionIndex = 166;
				pTransition->m_ResolverIndex = 992;
				pTransition->m_ResolverElseIndex = 169;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 39:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1761;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 40:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 41:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1763;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 42:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 43:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1765;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 44:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 45:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1767;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 46:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 47:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1769;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 48:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 49:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1771;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 50:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 51:
			
			switch (LookaheadToken)
			{
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 1773;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 52:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 53:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1775;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 54:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 55:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1777;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 56:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 57:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1779;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 58:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 59:
			
			switch (LookaheadToken)
			{
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1781;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 60:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 61:
			
			switch (LookaheadToken)
			{
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1783;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 62:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 282;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 63:
			
			switch (LookaheadToken)
			{
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 1785;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 64:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 283;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_CountOf:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_OffsetOf:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_BindingOf:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_HexLiteral:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_FmtLiteral:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 273;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 65:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 1787;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 66:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 948;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
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
					
			
			case EToken_Unwinder:
					
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
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 947;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 67:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 523;
				pTransition->m_ResolverIndex = 312;
				pTransition->m_ResolverElseIndex = 524;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 68:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 610;
				pTransition->m_ResolverIndex = 231;
				pTransition->m_ResolverElseIndex = 609;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 69:
			
			switch (LookaheadToken)
			{
			
			case '=':
					
				pTransition->m_ProductionIndex = 1791;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 70:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 596;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_CountOf:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_OffsetOf:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_BindingOf:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_HexLiteral:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_FmtLiteral:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 595;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 71:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 235;
				pTransition->m_ResolverElseIndex = 624;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 72:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 818;
				pTransition->m_ResolverIndex = 849;
				pTransition->m_ResolverElseIndex = 819;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 73:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 662;
				pTransition->m_ResolverIndex = 1117;
				pTransition->m_ResolverElseIndex = 664;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 74:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 703;
				pTransition->m_ResolverIndex = 231;
				pTransition->m_ResolverElseIndex = 702;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 75:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 722;
				pTransition->m_ResolverIndex = 235;
				pTransition->m_ResolverElseIndex = 721;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 76:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 813;
				pTransition->m_ResolverIndex = 849;
				pTransition->m_ResolverElseIndex = 814;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 77:
			
			switch (LookaheadToken)
			{
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 1799;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 78:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 815;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 814;
				return ELaDfaResult_Production;
					
			}
			
		
		case 79:
			
			switch (LookaheadToken)
			{
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 1801;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 80:
			
			switch (LookaheadToken)
			{
			
			case '<':
					
				pTransition->m_ProductionIndex = 837;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			}
			
		
		case 81:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1803;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 82:
			
			switch (LookaheadToken)
			{
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 894;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 895;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 83:
			
			switch (LookaheadToken)
			{
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 1805;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 84:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 896;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '|':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '^':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '<':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '>':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '/':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '%':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '@':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_LogOr:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_LogAnd:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Eq:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Ne:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Le:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Ge:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Shl:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Shr:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_RefAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_AddAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_SubAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_MulAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_DivAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_ModAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_ShlAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_ShrAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_AndAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_XorAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_OrAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 897;
				return ELaDfaResult_Production;
					
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			case EToken_AtAssign:
					
				pTransition->m_ProductionIndex = 898;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 85:
			
			switch (LookaheadToken)
			{
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 1807;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 86:
			
			switch (LookaheadToken)
			{
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 903;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 904;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 905;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 87:
			
			switch (LookaheadToken)
			{
			
			case '[':
					
				pTransition->m_ProductionIndex = 1809;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 88:
			
			switch (LookaheadToken)
			{
			
			case '\01':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '#':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '(':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case ';':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '}':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case ',':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '.':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case ']':
					
				pTransition->m_ProductionIndex = 946;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '?':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '|':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '^':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '<':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '>':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '/':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '%':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '@':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Pack:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Namespace:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Extend:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Using:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Friend:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Catch:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Finally:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_LogOr:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_LogAnd:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Eq:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Ne:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Le:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Ge:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Shl:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Shr:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_RefAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_AddAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_SubAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_MulAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_DivAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_ModAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_ShlAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_ShrAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_AndAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_XorAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_OrAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Try:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 945;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_CountOf:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_OffsetOf:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_BindingOf:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_HexLiteral:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_FmtLiteral:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_FmtSpecifier:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_BaseType:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_If:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Else:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Switch:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Case:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Default:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_While:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Do:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_For:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Break:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Continue:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Silent:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Return:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_AtAssign:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumF:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 944;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 89:
			
			switch (LookaheadToken)
			{
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 1811;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 90:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case ',':
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 951;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 953;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 91:
			
			switch (LookaheadToken)
			{
			
			case ':':
					
				pTransition->m_ProductionIndex = 519;
				pTransition->m_ResolverIndex = 1351;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 92:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 753;
				pTransition->m_ResolverIndex = 271;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 93:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				pTransition->m_ResolverIndex = 312;
				pTransition->m_ResolverElseIndex = 771;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 94:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1816;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 95:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 96:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1818;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 97:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 98:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1820;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 99:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 100:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1822;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 101:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 102:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1824;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 103:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 104:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1826;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 105:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 106:
			
			switch (LookaheadToken)
			{
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 1828;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 107:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 108:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1830;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 109:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 110:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1832;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 111:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 112:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1834;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 113:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 114:
			
			switch (LookaheadToken)
			{
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1836;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 115:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 116:
			
			switch (LookaheadToken)
			{
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1838;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 117:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 771;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_HeapU:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_ConstD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Unwinder:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_EventD:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Reactor:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 164;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 118:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 810;
				pTransition->m_ResolverIndex = 312;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 119:
			
			switch (LookaheadToken)
			{
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 287;
				pTransition->m_ResolverIndex = 1223;
				pTransition->m_ResolverElseIndex = 288;
						
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
