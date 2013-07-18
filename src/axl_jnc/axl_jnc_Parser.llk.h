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
	ESymbol_qualified_name = 16, 
	ESymbol_access_specifier = 17, 
	ESymbol_qualified_name_impl = 18, 
	ESymbol_declaration_specifier_list = 19, 
	ESymbol_declarator_list = 20, 
	ESymbol_declaration_terminator = 21, 
	ESymbol_declarator_prefix = 22, 
	ESymbol_declarator_name = 23, 
	ESymbol_function_suffix = 24, 
	ESymbol_declarator_suffix = 25, 
	ESymbol_post_declarator_modifier = 26, 
	ESymbol_function_body_pass1 = 27, 
	ESymbol_type_name_impl = 28, 
	ESymbol_type_specifier_modifier_list = 29, 
	ESymbol_type_name = 30, 
	ESymbol_type_name_list = 31, 
	ESymbol_type_name_or_expr = 32, 
	ESymbol_expression_s = 33, 
	ESymbol_attribute_block = 34, 
	ESymbol_attribute_declarator_list = 35, 
	ESymbol_attribute_declarator = 36, 
	ESymbol_expression_pass1 = 37, 
	ESymbol_primary_expr_pass1 = 38, 
	ESymbol_arg_list_pass1 = 39, 
	ESymbol_arg_pass1 = 40, 
	ESymbol_expression_save_value = 41, 
	ESymbol_expression = 42, 
	ESymbol_expression_or_empty_list_save_list = 43, 
	ESymbol_expression_or_empty_list = 44, 
	ESymbol_conditional_expr = 45, 
	ESymbol_expression_or_empty = 46, 
	ESymbol_expression_list = 47, 
	ESymbol_constant_expr = 48, 
	ESymbol_constant_integer_expr = 49, 
	ESymbol_logical_or_expr = 50, 
	ESymbol_logical_and_expr = 51, 
	ESymbol_inclusive_or_expr = 52, 
	ESymbol_exclusive_or_expr = 53, 
	ESymbol_and_expr = 54, 
	ESymbol_equality_expr = 55, 
	ESymbol_relational_expr = 56, 
	ESymbol_equality_operator = 57, 
	ESymbol_shift_expr = 58, 
	ESymbol_relational_operator = 59, 
	ESymbol_additive_expr = 60, 
	ESymbol_shift_operator = 61, 
	ESymbol_multiplicative_expr = 62, 
	ESymbol_additive_operator = 63, 
	ESymbol_at_expr = 64, 
	ESymbol_multiplicative_operator = 65, 
	ESymbol_assignment_expr = 66, 
	ESymbol_unary_expr = 67, 
	ESymbol_assignment_operator_expr = 68, 
	ESymbol_assignment_operator = 69, 
	ESymbol_curly_initializer = 70, 
	ESymbol_postfix_expr = 71, 
	ESymbol_unary_operator_expr = 72, 
	ESymbol_cast_operator_rslv = 73, 
	ESymbol_storage_specifier = 74, 
	ESymbol_new_operator_type = 75, 
	ESymbol_new_operator_curly_initializer = 76, 
	ESymbol_type_name_w_constructor_rslv = 77, 
	ESymbol_qualified_type_name = 78, 
	ESymbol_type_specifier_modifier = 79, 
	ESymbol_primary_expr = 80, 
	ESymbol_postfix_operator = 81, 
	ESymbol_member_operator = 82, 
	ESymbol_literal = 83, 
	ESymbol_curly_initializer_item = 84, 
	ESymbol_expression_save_value_s = 85, 
	ESymbol_conditional_expr_s = 86, 
	ESymbol_expression_or_empty_s = 87, 
	ESymbol_expression_list_s = 88, 
	ESymbol_expression_or_empty_list_s = 89, 
	ESymbol_logical_or_expr_s = 90, 
	ESymbol_logical_and_expr_s = 91, 
	ESymbol_inclusive_or_expr_s = 92, 
	ESymbol_exclusive_or_expr_s = 93, 
	ESymbol_and_expr_s = 94, 
	ESymbol_equality_expr_s = 95, 
	ESymbol_relational_expr_s = 96, 
	ESymbol_shift_expr_s = 97, 
	ESymbol_additive_expr_s = 98, 
	ESymbol_multiplicative_expr_s = 99, 
	ESymbol_at_expr_s = 100, 
	ESymbol_assignment_expr_s = 101, 
	ESymbol_unary_expr_s = 102, 
	ESymbol_postfix_expr_s = 103, 
	ESymbol_unary_operator_expr_s = 104, 
	ESymbol_new_operator_sype_s = 105, 
	ESymbol_primary_expr_s = 106, 
	ESymbol_postfix_operator_s = 107, 
	ESymbol_member_operator_s = 108, 
	ESymbol_compound_stmt_pass1 = 109, 
	ESymbol_statement_pass1 = 110, 
	ESymbol_compound_stmt = 111, 
	ESymbol_constructor_compound_stmt = 112, 
	ESymbol_btm_construct_stmt = 113, 
	ESymbol_btm_construct_stmt_rslv = 114, 
	ESymbol_btm_construct_name = 115, 
	ESymbol_expression_stmt = 116, 
	ESymbol_if_stmt = 117, 
	ESymbol_switch_stmt = 118, 
	ESymbol_while_stmt = 119, 
	ESymbol_do_stmt = 120, 
	ESymbol_for_stmt = 121, 
	ESymbol_break_stmt = 122, 
	ESymbol_continue_stmt = 123, 
	ESymbol_return_stmt = 124, 
	ESymbol_once_stmt = 125, 
	ESymbol_switch_block_stmt = 126, 
	ESymbol_autoev_body = 127, 
	ESymbol_autoev_stmt = 128, 
	ESymbol_autoev_onchange_stmt = 129, 
	ESymbol_autoev_expression_stmt = 130, 
	ESymbol_statement_0 = 131, 
	ESymbol_compound_stmt_0 = 132, 
	ESymbol_expression_stmt_0 = 133, 
	ESymbol_if_stmt_0 = 134, 
	ESymbol_switch_stmt_0 = 135, 
	ESymbol_while_stmt_0 = 136, 
	ESymbol_do_stmt_0 = 137, 
	ESymbol_for_stmt_0 = 138, 
	ESymbol_break_stmt_0 = 139, 
	ESymbol_continue_stmt_0 = 140, 
	ESymbol_return_stmt_0 = 141, 
	ESymbol_once_stmt_0 = 142, 
	ESymbol_expression_0 = 143, 
	ESymbol_primary_expr_0 = 144, 
	ESymbol_autoev_body_0 = 145, 
	ESymbol_autoev_stmt_0 = 146, 
	ESymbol_autoev_onchange_stmt_0 = 147, 
	ESymbol_autoev_expression_stmt_0 = 148, 
	ESymbol_declaration_specifier = 149, 
	ESymbol_type_specifier = 150, 
	ESymbol_type_modifier = 151, 
	ESymbol_property_template_specifier = 152, 
	ESymbol_qualified_type_name_rslv = 153, 
	ESymbol_full_declarator = 154, 
	ESymbol_declarator = 155, 
	ESymbol_declarator_qualifier = 156, 
	ESymbol_operator_name = 157, 
	ESymbol_array_suffix = 158, 
	ESymbol_bitfield_suffix = 159, 
	ESymbol_function_formal_argument_list = 160, 
	ESymbol_function_formal_argument = 161, 
	ESymbol_function_formal_argument_w_specifier = 162, 
	ESymbol_function_formal_argument_no_specifier = 163, 
	ESymbol_function_formal_argument_ellipsis = 164, 
	ESymbol_enum_specifier = 165, 
	ESymbol_struct_specifier = 166, 
	ESymbol_union_specifier = 167, 
	ESymbol_class_specifier = 168, 
	ESymbol_property_specifier_rslv = 169, 
	ESymbol_property_specifier = 170, 
	ESymbol_enum_block = 171, 
	ESymbol_enum_member_list = 172, 
	ESymbol_enum_member = 173, 
	ESymbol_struct_pack_factor = 174, 
	ESymbol_named_type_block = 175, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 2,

		StartSymbol        = 0,
		StartPragmaSymbol  = 327,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 142,
		SymbolCount        = 328,
		SequenceCount      = 484,
		ActionCount        = 376,
		ArgumentCount      = 157,
		BeaconCount        = 173,
		LaDfaCount         = 127,

		TotalCount         = 1787,

		NamedSymbolCount   = 176,

		TokenFirst         = 0,
		TokenEnd           = 142,
		SymbolFirst        = 142,
		NamedSymbolEnd     = 318,
		SymbolEnd          = 470,
		SequenceFirst      = 470,
		SequenceEnd        = 954,
		ActionFirst        = 954,
		ActionEnd          = 1330,
		ArgumentFirst      = 1330,
		ArgumentEnd        = 1487,
		BeaconFirst        = 1487,
		BeaconEnd          = 1660,
		LaDfaFirst         = 1660,
		LaDfaEnd           = 1787,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	
	enum EStage
	{
		EStage_Pass1, // global declarations (before linkage)
		EStage_Pass2, // statements and expressions (after linkage & calclayout)		
		EStage_AutoEvScan, // scan autoev body to build bind site map (after linkage but before calclayout)
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

	struct TCurlyInitializer
	{
		CValue m_TargetValue;
		CValue m_MemberValue;
		size_t m_Index;
		size_t m_Count;
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
	
	CAutoEvClassType* m_pAutoEvType;
	rtl::CStdListT <TAutoEvHandler> m_AutoEvHandlerList;
	rtl::CBoxListT <CValue> m_AutoEvBindSiteList;
	size_t m_AutoEvBindSiteCount;
	size_t m_AutoEvBindSiteTotalCount;
	
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
// #line 95 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CQualifiedName m_Name
;
// #line 317 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls2: public CAstNode
	{
	public:
// #line 214 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
// #line 327 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls3: public CAstNode
	{
	public:
// #line 222 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	rtl::CBoxListT <CType*> m_TypeList;
;
// #line 337 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls4: public CAstNode
	{
	public:
// #line 238 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
// #line 347 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls5: public CAstNode
	{
	public:
// #line 4 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 357 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls6: public CAstNode
	{
	public:
// #line 51 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 367 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls7: public CAstNode
	{
	public:
// #line 91 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
// #line 377 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls8: public CAstNode
	{
	public:
// #line 100 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;
;
// #line 387 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls9: public CAstNode
	{
	public:
// #line 110 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 397 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls10: public CAstNode
	{
	public:
// #line 129 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 407 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls11: public CAstNode
	{
	public:
// #line 155 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
// #line 417 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls12: public CAstNode
	{
	public:
// #line 171 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	intptr_t m_Value;
;
// #line 427 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls13: public CAstNode
	{
	public:
// #line 196 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_TrueValue;
	CValue m_FalseValue;
	CBasicBlock* m_pThenBlock;
	CBasicBlock* m_pElseBlock;
	CBasicBlock* m_pPhiBlock;
;
// #line 441 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls14: public CAstNode
	{
	public:
// #line 310 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 451 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls15: public CAstNode
	{
	public:
// #line 338 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 461 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls16: public CAstNode
	{
	public:
// #line 374 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 471 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls17: public CAstNode
	{
	public:
// #line 402 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 481 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls18: public CAstNode
	{
	public:
// #line 430 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 491 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls19: public CAstNode
	{
	public:
// #line 487 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 501 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls20: public CAstNode
	{
	public:
// #line 624 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	CType* m_pType;
	CValue m_ElementCountValue;
	rtl::CBoxListT <CValue> m_ArgList;
;
// #line 513 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls21: public CAstNode
	{
	public:
// #line 777 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CString m_String;
;
// #line 523 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls22: public CAstNode
	{
	public:
// #line 12 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	CValue m_Value;	
;
// #line 533 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls23: public CAstNode
	{
	public:
// #line 21 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	CValue m_Value;
;
// #line 543 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls24: public CAstNode
	{
	public:
// #line 31 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 553 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls25: public CAstNode
	{
	public:
// #line 50 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 563 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls26: public CAstNode
	{
	public:
// #line 320 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
		
	CType* m_pType;
;
// #line 573 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls27: public CAstNode
	{
	public:
// #line 4 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 583 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls28: public CAstNode
	{
	public:
// #line 83 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
		
	CQualifiedName m_Name;
;
// #line 593 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls29: public CAstNode
	{
	public:
// #line 4 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CTypeSpecifier m_TypeSpecifier;
;
// #line 603 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls30: public CAstNode
	{
	public:
// #line 42 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CTypeSpecifier m_TypeSpecifier;
;
// #line 613 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls31: public CAstNode
	{
	public:
// #line 71 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EStorage m_StorageKind;
;
// #line 623 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls32: public CAstNode
	{
	public:
// #line 128 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EAccess m_AccessKind;
;
// #line 633 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls33: public CAstNode
	{
	public:
// #line 145 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
// #line 643 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls34: public CAstNode
	{
	public:
// #line 242 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
// #line 653 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls35: public CAstNode
	{
	public:
// #line 262 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	ETypeModifier m_Modifier;
;
// #line 663 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls36: public CAstNode
	{
	public:
// #line 27 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
		
	CDeclarator m_Declarator;
;
// #line 673 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls37: public CAstNode
	{
	public:
// #line 105 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
		
	EUnOp m_UnOpKind;
	EBinOp m_BinOpKind;
;
// #line 684 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls38: public CAstNode
	{
	public:
// #line 6 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CType* m_pType;
;
// #line 694 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls39: public CAstNode
	{
	public:
// #line 38 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CEnumType* m_pType;
;
// #line 704 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls40: public CAstNode
	{
	public:
// #line 104 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CStructType* m_pType;
;
// #line 714 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls41: public CAstNode
	{
	public:
// #line 128 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_PackFactor;
;
// #line 724 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls42: public CAstNode
	{
	public:
// #line 139 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_Value;
;
// #line 734 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls43: public CAstNode
	{
	public:
// #line 158 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CUnionType* m_pType;
;
// #line 744 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls44: public CAstNode
	{
	public:
// #line 177 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CClassType* m_pType;
;
// #line 754 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls45: public CAstNode
	{
	public:
// #line 209 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CProperty* m_pProperty;
;
// #line 764 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	
	class _cls46: public CAstNode
	{
	public:
// #line 239 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CPropertyType* m_pType;
;
// #line 774 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
	};
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_item_declaration_no_specifier_rslv: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 160 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
		CDeclarator Declarator;
	;
// #line 802 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 133 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
		CDeclarator Declarator;
	;
// #line 824 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 102 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			CQualifiedName* pName;
// #line 851 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 3 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 872 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 168 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 892 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 286 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 914 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 43 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 934 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 327 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 954 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 329 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclFunctionSuffix* pFunctionSuffix;
		EStorage PrevStorageKind;
	;
// #line 964 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 301 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 988 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 426 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 1008 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_post_declarator_modifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_post_declarator_modifier;
			
		}
	};
		
	
	
	class CSymbolNode_type_name_impl: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 193 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			CType** ppType; CValue* pCountValue;
// #line 1029 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 195 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		CDeclarator Declarator;
	;
// #line 1038 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 12 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 1067 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 59 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 1088 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 203 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1112 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 231 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1136 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 233 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1145 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 244 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1165 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 246 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1174 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 257 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1194 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 259 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1203 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 270 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1223 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 272 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1232 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 283 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1252 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 285 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1261 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 296 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1281 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 298 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1290 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 324 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1310 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 326 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1319 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 360 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1340 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 362 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1349 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 388 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1370 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 390 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1379 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 416 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1400 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 418 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1409 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 448 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1430 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 450 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1439 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 461 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1460 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 541 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1480 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 465 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1500 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 467 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1509 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 797 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		TCurlyInitializer Initializer;
	;
// #line 1534 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 662 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1556 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 546 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1576 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 656 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CTypeSpecifier TypeSpecifier;
	;
// #line 1600 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 643 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1622 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 57 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 1646 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 727 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1666 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 667 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1688 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 704 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1710 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_member_operator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_member_operator;
			
		}
	};
		
	
	
	class CSymbolNode_curly_initializer_item: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 819 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			TCurlyInitializer* pInitializer;
// #line 1731 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 821 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
		CValue PrevCurlyInitializerTargetValue;
	;
// #line 1740 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 77 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1761 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 79 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue TrueValue;
		CValue FalseValue;
	;
// #line 1771 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 91 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1794 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 93 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1803 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 104 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1823 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 106 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1832 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 117 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1852 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 119 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1861 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 130 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1881 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 132 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1890 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 143 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1910 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 145 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1919 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 156 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1939 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 158 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1948 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 169 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1968 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 171 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1977 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 182 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1997 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 184 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2006 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 195 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2026 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 197 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2035 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 208 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2055 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 210 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2064 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 221 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2084 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 223 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2093 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 234 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2113 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 236 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2122 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 252 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2142 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 335 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2162 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 257 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2182 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 400 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2205 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 340 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2227 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 377 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2249 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_member_operator_s ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_member_operator_s;
			
		}
	};
		
	
	class CSymbolNode_compound_stmt_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 12 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 2269 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 25 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 2289 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_statement_pass1 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_statement_pass1;
			
		}
	};
		
	
	
	
	
	
	
	
	class CSymbolNode_if_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 120 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TIfStmt Stmt;
	;
// #line 2317 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 146 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TSwitchStmt Stmt;
	;
// #line 2341 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 177 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TWhileStmt Stmt;
	;
// #line 2365 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 196 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TDoStmt Stmt;
	;
// #line 2389 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 219 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TForStmt Stmt;
	;
// #line 2413 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 281 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
		TOnceStmt Stmt;
	;
// #line 2440 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 163 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			TSwitchStmt* pStmt;
// #line 2460 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 19 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2502 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_declaration_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declaration_specifier;
			
		}
	};
		
	
	
	
	class CSymbolNode_property_template_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 244 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
		CPropertyTemplate* pTemplate;
	;
// #line 2526 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Local;	
			
		CSymbolNode_property_template_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_property_template_specifier;
			
		}
	};
		
	
	
	class CSymbolNode_full_declarator: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 7 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2547 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_full_declarator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_full_declarator;
			
		}
	};
		
	
	class CSymbolNode_declarator: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 30 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2567 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 51 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2589 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 310 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2610 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 417 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2630 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 346 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2650 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 351 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2670 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 357 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2690 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 359 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclarator Declarator;
	;
// #line 2699 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 383 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2721 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		struct
		{
// #line 385 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclarator Declarator;
	;
// #line 2730 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 406 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2752 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 65 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2778 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 78 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2798 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 83 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2818 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_enum_member ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_enum_member;
			
		}
	};
		
	
	
	class CSymbolNode_named_type_block: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 262 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CNamespace* pNamespace;
// #line 2839 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
		} m_Arg;	
			
		CSymbolNode_named_type_block ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_named_type_block;
			
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
	IsAutoEv ()
	{
		return m_pAutoEvType != NULL;
	}

	bool
	IsTypeSpecified ();

	bool
	IsEmptyDeclarationTerminatorAllowed (CTypeSpecifier* pTypeSpecifier);

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
	Declare (CDeclarator* pDeclarator);
		
	bool
	DeclareTypedef (
		CType* pType,
		CDeclarator* pDeclarator
		);

	bool
	DeclareAlias (
		CType* pType,
		CDeclarator* pDeclarator
		);

	bool
	DeclareFunction (
		CFunctionType* pType,
		CDeclarator* pDeclarator
		);

	bool
	DeclareProperty (
		CPropertyType* pType,
		CDeclarator* pDeclarator,
		uint_t PropertyFlags
		);

	bool
	DeclareData (
		CType* pType,
		CDeclarator* pDeclarator,
		uint_t PtrTypeFlags
		);

	bool
	DeclareAutoEv (
		CAutoEvClassType* pType,
		CDeclarator* pDeclarator,
		uint_t PtrTypeFlags
		);

	bool
	DeclarePropValue (
		CType* pType,
		CDeclarator* pDeclarator,
		uint_t PtrTypeFlags
		);

	CFunctionArg*
	CreateFormalArg (
		CDeclFunctionSuffix* pArgSuffix,
		CDeclarator* pDeclarator
		);

	CEnumType*
	CreateEnumType (
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
	NewOperator_s (
		EStorage StorageKind, 
		CType* pType, 
		CValue* pResultValue
		);

	bool
	FinalizeAutoEv ();

	bool
	FinalizeAutoEvOnChangeClause ();

	bool
	FinalizeAutoEvOnChangeStmt ()
	{
		return m_pModule->m_FunctionMgr.Epilogue (m_LastMatchedToken.m_Pos);
	}

	bool
	AutoEvExpressionStmt (rtl::CBoxListT <CToken>* pTokenList);

	bool
	CountAutoEvBindSites ();

	bool
	CallBaseTypeMemberConstructor (
		const CQualifiedName& Name,
		rtl::CBoxListT <CValue>* pArgList
		);

	bool
	CallBaseTypeConstructor (
		CType* pType,
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
	SetFunctionBody (rtl::CBoxListT <CToken>* pBody);

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
	GetThisValue (CValue* pValue);

	bool
	GetThisValueType (CValue* pValue);

	bool
	GetPropValue (CValue* pValue);

	bool
	GetPropValueType (CValue* pValue);

	bool
	GetOnChange (CValue* pValue);

	bool
	GetOnChangeType (CValue* pValue);

	bool
	PrepareCurlyInitializerNamedItem (
		TCurlyInitializer* pInitializer, 
		const char* pName
		);

	bool
	PrepareCurlyInitializerIndexedItem (TCurlyInitializer* pInitializer);

	bool
	SkipCurlyInitializerItem (TCurlyInitializer* pInitializer);


	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	static
	size_t*
	GetParseTable ()
	{
		static
		size_t _ParseTable [] = 
		{
			0, -1, -1, -1, -1, 470, -1, -1, -1, -1, -1, 470, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 470, -1, -1, -1, -1, -1, -1, 470, 470, 470, 470, 470, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 470, 470, -1, -1, 470, 470, 470, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, -1, -1, -1, 470, 470, 470, 470, 470, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, 152, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 319, -1, -1, -1, -1, -1, -1, 146, 147, 148, 149, 1667, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 319, 152, -1, -1, 319, 319, 319, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 152, 152, 1668, 152, 152, 152, 152, 152, 152, 152, 152, 152, 1660, 1662, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 1664, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 1666, 152, 152, 152, 152, 152, 319, 319, 319, -1, -1, -1, 151, 151, 151, 151, 151, 
			-1, -1, 471, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 944, -1, -1, -1, -1, -1, -1, -1, -1, -1, 945, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 472, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 474, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 476, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 478, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 480, 480, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 453, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 453, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 929, 929, 930, 931, 932, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 485, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 485, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 485, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, -1, -1, -1, 490, 490, 490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 490, 490, 490, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, -1, -1, -1, 487, 487, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, 487, 487, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, 152, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 320, -1, -1, -1, -1, -1, -1, -1, -1, -1, 149, 1676, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 320, 152, -1, -1, 320, 320, 320, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 152, 152, 1677, 152, 152, 152, 152, 152, 152, 152, 152, 152, 1669, 1671, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 1673, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 1675, 152, 152, 152, 152, 152, 320, 320, 320, -1, -1, -1, 151, 151, 151, 151, 151, 
			-1, -1, -1, 157, -1, 157, 157, -1, -1, -1, -1, 152, -1, -1, -1, -1, -1, 157, -1, -1, 157, 157, 1707, -1, -1, -1, 157, 157, -1, -1, -1, 148, -1, 1678, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, 157, 157, 157, 157, 157, 321, 152, -1, 157, 1708, 321, 321, 157, 157, 157, 157, 157, 157, 157, 157, -1, 157, -1, -1, 157, 157, 157, 157, 157, 157, 157, 157, 1680, 1682, 1684, 1686, 1688, 1690, 1692, 1694, 1696, 1698, 1700, 1702, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 1704, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 1706, 152, 152, 152, 152, 152, 321, 321, 321, -1, -1, -1, 151, 151, 151, 151, 151, 
			-1, -1, -1, 258, -1, 5, 253, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 258, -1, -1, 258, 258, 258, -1, -1, -1, 258, 258, -1, -1, -1, -1, -1, 258, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 258, 258, 258, 258, 258, 258, -1, -1, -1, 258, 258, -1, -1, 258, 258, 258, 258, 258, 258, 258, 259, -1, 260, -1, -1, 261, 262, 263, 264, 265, 266, 266, 267, 1709, 1711, 1713, 1715, 1717, 1719, 1721, 1723, 1725, 1727, 1729, 1731, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 481, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 797, 798, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 482, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, 775, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 838, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 838, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 838, -1, -1, -1, 838, 838, 838, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 838, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 838, 838, 838, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 491, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 901, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 847, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 847, -1, -1, -1, 847, 847, 847, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 847, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 847, 847, 847, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1733, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 904, -1, -1, -1, -1, -1, 442, -1, 903, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 928, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 716, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 781, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 781, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, 781, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 496, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 496, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 497, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 497, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, -1, -1, 500, 500, 500, -1, -1, -1, 500, 500, -1, -1, -1, -1, -1, 1735, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, 500, 500, 500, 500, 500, -1, 499, -1, 500, 500, -1, -1, 500, 500, 500, 500, 500, 500, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, -1, -1, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 640, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 640, -1, -1, 640, 640, 640, -1, -1, -1, 640, 640, -1, -1, -1, -1, -1, 640, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 640, 640, 640, 640, 640, 640, -1, -1, -1, 640, 640, -1, -1, 640, 640, 640, 640, 640, 640, 640, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 640, 640, 640, 640, 640, 640, 640, 640, 640, 640, 640, 640, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 505, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 
			-1, 514, 514, 512, 514, 514, 508, 514, 514, 514, 514, 510, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 
			-1, -1, -1, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 519, 519, 518, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 519, 
			-1, -1, -1, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, -1, -1, 520, 520, 520, -1, -1, -1, 520, 520, -1, -1, -1, -1, -1, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, 520, 520, 520, 520, 520, -1, -1, -1, 520, 520, -1, -1, 520, 520, 520, 520, 520, 520, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, -1, -1, 522, 522, 522, -1, -1, -1, 522, 522, -1, -1, -1, -1, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, 522, 522, 522, 522, 522, -1, -1, -1, 522, 522, -1, -1, 522, 522, 522, 522, 522, 522, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			521, -1, -1, 521, -1, -1, -1, -1, 521, -1, -1, -1, -1, -1, -1, -1, -1, 521, -1, -1, 521, 521, 521, -1, -1, -1, 521, 521, -1, -1, -1, -1, -1, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 521, 521, 521, 521, 521, 521, -1, -1, -1, 521, 521, -1, -1, 521, 521, 521, 521, 521, 521, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, 521, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 526, 0, 0, -1, -1, 529, -1, -1, -1, -1, -1, -1, -1, -1, 526, -1, -1, 526, 526, 526, -1, -1, -1, 526, 526, -1, -1, -1, -1, -1, 526, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 526, 526, 526, 526, 526, 526, -1, -1, -1, 526, 526, -1, -1, 526, 526, 526, 526, 526, 526, 526, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 526, 526, 526, 526, 526, 526, 526, 526, 526, 526, 526, 526, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 532, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, -1, -1, 532, 532, 532, -1, -1, -1, 532, 532, -1, -1, -1, -1, -1, 532, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, 532, 532, 532, 532, 532, -1, -1, -1, 532, 532, -1, -1, 532, 532, 532, 532, 532, 532, 532, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 523, 0, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 523, -1, -1, 523, 523, 523, -1, -1, -1, 523, 523, -1, -1, -1, -1, -1, 523, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 523, 523, 523, 523, 523, 523, -1, -1, -1, 523, 523, -1, -1, 523, 523, 523, 523, 523, 523, 523, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, 523, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, -1, -1, 524, 524, 524, -1, -1, -1, 524, 524, -1, -1, -1, -1, -1, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, 524, 524, 524, 524, 524, -1, -1, -1, 524, 524, -1, -1, 524, 524, 524, 524, 524, 524, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 530, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 530, -1, -1, 530, 530, 530, -1, -1, -1, 530, 530, -1, -1, -1, -1, -1, 530, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 530, 530, 530, 530, 530, 530, -1, -1, -1, 530, 530, -1, -1, 530, 530, 530, 530, 530, 530, 530, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, 530, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 531, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 531, -1, -1, 531, 531, 531, -1, -1, -1, 531, 531, -1, -1, -1, -1, -1, 531, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 531, 531, 531, 531, 531, 531, -1, -1, -1, 531, 531, -1, -1, 531, 531, 531, 531, 531, 531, 531, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 534, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 534, -1, -1, 534, 534, 534, -1, -1, -1, 534, 534, -1, -1, -1, -1, -1, 534, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 534, 534, 534, 534, 534, 534, -1, -1, -1, 534, 534, -1, -1, 534, 534, 534, 534, 534, 534, 534, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 536, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 536, -1, -1, 536, 536, 536, -1, -1, -1, 536, 536, -1, -1, -1, -1, -1, 536, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 536, 536, 536, 536, 536, 536, -1, -1, -1, 536, 536, -1, -1, 536, 536, 536, 536, 536, 536, 536, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, 536, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 538, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 538, -1, -1, 538, 538, 538, -1, -1, -1, 538, 538, -1, -1, -1, -1, -1, 538, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 538, 538, 538, 538, 538, 538, -1, -1, -1, 538, 538, -1, -1, 538, 538, 538, 538, 538, 538, 538, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 540, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 540, -1, -1, 540, 540, 540, -1, -1, -1, 540, 540, -1, -1, -1, -1, -1, 540, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 540, 540, 540, 540, 540, 540, -1, -1, -1, 540, 540, -1, -1, 540, 540, 540, 540, 540, 540, 540, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, 540, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 542, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 542, -1, -1, 542, 542, 542, -1, -1, -1, 542, 542, -1, -1, -1, -1, -1, 542, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 542, 542, 542, 542, 542, 542, -1, -1, -1, 542, 542, -1, -1, 542, 542, 542, 542, 542, 542, 542, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, -1, -1, 544, 544, 544, -1, -1, -1, 544, 544, -1, -1, -1, -1, -1, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, 544, 544, 544, 544, 544, -1, -1, -1, 544, 544, -1, -1, 544, 544, 544, 544, 544, 544, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, -1, -1, 548, 548, 548, -1, -1, -1, 548, 548, -1, -1, -1, -1, -1, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, 548, 548, 548, 548, 548, -1, -1, -1, 548, 548, -1, -1, 548, 548, 548, 548, 548, 548, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, -1, -1, 554, 554, 554, -1, -1, -1, 554, 554, -1, -1, -1, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, 554, -1, -1, -1, 554, 554, -1, -1, 554, 554, 554, 554, 554, 554, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 550, 551, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 552, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 558, -1, -1, 558, 558, 558, -1, -1, -1, 558, 558, -1, -1, -1, -1, -1, 558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 558, 558, 558, 558, 558, 558, -1, -1, -1, 558, 558, -1, -1, 558, 558, 558, 558, 558, 558, 558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, 558, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 556, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 562, -1, -1, 562, 562, 562, -1, -1, -1, 562, 562, -1, -1, -1, -1, -1, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 562, 562, 562, 562, 562, 562, -1, -1, -1, 562, 562, -1, -1, 562, 562, 562, 562, 562, 562, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, 562, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 560, 561, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 567, -1, -1, 567, 567, 567, -1, -1, -1, 567, 567, -1, -1, -1, -1, -1, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 567, 567, 567, 567, 567, 567, -1, -1, -1, 567, 567, -1, -1, 567, 567, 567, 567, 567, 567, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, 567, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 564, 565, 566, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 569, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 569, -1, -1, 569, 569, 569, -1, -1, -1, 569, 569, -1, -1, -1, -1, -1, 569, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 569, 569, 569, 569, 569, 569, -1, -1, -1, 569, 569, -1, -1, 569, 569, 569, 569, 569, 569, 569, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, 569, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1736, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 586, -1, -1, 586, 586, 586, -1, -1, -1, 586, 586, -1, -1, -1, -1, -1, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 586, 586, 586, 586, 586, 586, -1, -1, -1, 585, 585, -1, -1, 585, 585, 585, 585, 585, 585, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, 586, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1737, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, 571, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 573, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 632, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 605, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 605, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 605, 605, -1, -1, 605, 605, 605, 605, 605, 605, 605, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 359, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 590, -1, -1, 587, 588, 591, -1, -1, -1, 589, 592, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 593, 594, 595, 596, 598, 600, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 598, 598, 598, 598, 598, 598, 598, 598, 598, 598, 598, 598, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 604, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 785, 786, 787, 788, 789, 790, 791, 792, 793, 794, 795, 796, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1739, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 601, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, 601, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 212, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 603, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 783, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 784, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 783, 783, 783, 783, 783, 783, 783, 783, 783, 783, 783, 783, 783, 783, 1740, 784, 784, 784, 784, 784, 784, 784, 784, 784, 784, 784, 784, 784, 784, 784, 784, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 629, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 621, 622, -1, -1, 620, 623, 624, 626, 627, 628, 625, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 607, -1, -1, -1, -1, -1, -1, 612, 609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 608, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 610, 611, -1, -1, -1, -1, -1, -1, 613, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 614, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 615, 616, 617, 618, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 630, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 636, -1, -1, 637, 638, 638, -1, -1, -1, -1, -1, -1, -1, -1, 636, -1, -1, 636, 636, 636, -1, -1, -1, 636, 636, -1, -1, -1, -1, -1, 1741, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 636, 636, 636, 636, 636, 636, -1, -1, -1, 636, 636, -1, -1, 636, 636, 636, 636, 636, 636, 636, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 636, 636, 636, 636, 636, 636, 636, 636, 636, 636, 636, 636, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 639, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 639, -1, -1, 639, 639, 639, -1, -1, -1, 639, 639, -1, -1, -1, -1, -1, 639, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 639, 639, 639, 639, 639, 639, -1, -1, -1, 639, 639, -1, -1, 639, 639, 639, 639, 639, 639, 639, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 639, 639, 639, 639, 639, 639, 639, 639, 639, 639, 639, 639, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 648, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 648, -1, -1, 648, 648, 648, -1, -1, -1, 648, 648, -1, -1, -1, -1, -1, 648, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 648, 648, 648, 648, 648, 648, -1, -1, -1, 648, 648, -1, -1, 648, 648, 648, 648, 648, 648, 648, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 648, 648, 648, 648, 648, 648, 648, 648, 648, 648, 648, 648, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 641, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 641, -1, -1, 641, 641, 641, -1, -1, -1, 641, 641, -1, -1, -1, -1, -1, 641, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 641, 641, 641, 641, 641, 641, -1, -1, -1, 641, 641, -1, -1, 641, 641, 641, 641, 641, 641, 641, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 642, -1, -1, 642, 642, 642, -1, -1, -1, 642, 642, -1, -1, -1, -1, -1, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 642, 642, 642, 642, 642, 642, -1, -1, -1, 642, 642, -1, -1, 642, 642, 642, 642, 642, 642, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 644, 0, -1, -1, -1, 647, -1, -1, -1, -1, -1, -1, -1, -1, 644, -1, -1, 644, 644, 644, -1, -1, -1, 644, 644, -1, -1, -1, -1, -1, 644, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 644, 644, 644, 644, 644, 644, -1, -1, -1, 644, 644, -1, -1, 644, 644, 644, 644, 644, 644, 644, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 644, 644, 644, 644, 644, 644, 644, 644, 644, 644, 644, 644, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 650, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 650, -1, -1, 650, 650, 650, -1, -1, -1, 650, 650, -1, -1, -1, -1, -1, 650, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 650, 650, 650, 650, 650, 650, -1, -1, -1, 650, 650, -1, -1, 650, 650, 650, 650, 650, 650, 650, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 650, 650, 650, 650, 650, 650, 650, 650, 650, 650, 650, 650, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 652, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 652, -1, -1, 652, 652, 652, -1, -1, -1, 652, 652, -1, -1, -1, -1, -1, 652, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 652, 652, 652, 652, 652, 652, -1, -1, -1, 652, 652, -1, -1, 652, 652, 652, 652, 652, 652, 652, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 652, 652, 652, 652, 652, 652, 652, 652, 652, 652, 652, 652, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 654, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 654, -1, -1, 654, 654, 654, -1, -1, -1, 654, 654, -1, -1, -1, -1, -1, 654, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 654, 654, 654, 654, 654, 654, -1, -1, -1, 654, 654, -1, -1, 654, 654, 654, 654, 654, 654, 654, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 654, 654, 654, 654, 654, 654, 654, 654, 654, 654, 654, 654, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 656, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 656, -1, -1, 656, 656, 656, -1, -1, -1, 656, 656, -1, -1, -1, -1, -1, 656, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 656, 656, 656, 656, 656, 656, -1, -1, -1, 656, 656, -1, -1, 656, 656, 656, 656, 656, 656, 656, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 656, 656, 656, 656, 656, 656, 656, 656, 656, 656, 656, 656, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 658, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 658, -1, -1, 658, 658, 658, -1, -1, -1, 658, 658, -1, -1, -1, -1, -1, 658, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 658, 658, 658, 658, 658, 658, -1, -1, -1, 658, 658, -1, -1, 658, 658, 658, 658, 658, 658, 658, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 658, 658, 658, 658, 658, 658, 658, 658, 658, 658, 658, 658, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 660, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 660, -1, -1, 660, 660, 660, -1, -1, -1, 660, 660, -1, -1, -1, -1, -1, 660, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 660, 660, 660, 660, 660, 660, -1, -1, -1, 660, 660, -1, -1, 660, 660, 660, 660, 660, 660, 660, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 662, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 662, -1, -1, 662, 662, 662, -1, -1, -1, 662, 662, -1, -1, -1, -1, -1, 662, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 662, 662, 662, 662, 662, 662, -1, -1, -1, 662, 662, -1, -1, 662, 662, 662, 662, 662, 662, 662, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 662, 662, 662, 662, 662, 662, 662, 662, 662, 662, 662, 662, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 664, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 664, -1, -1, 664, 664, 664, -1, -1, -1, 664, 664, -1, -1, -1, -1, -1, 664, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 664, 664, 664, 664, 664, 664, -1, -1, -1, 664, 664, -1, -1, 664, 664, 664, 664, 664, 664, 664, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, 664, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 666, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 666, -1, -1, 666, 666, 666, -1, -1, -1, 666, 666, -1, -1, -1, -1, -1, 666, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 666, 666, 666, 666, 666, 666, -1, -1, -1, 666, 666, -1, -1, 666, 666, 666, 666, 666, 666, 666, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 668, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 668, -1, -1, 668, 668, 668, -1, -1, -1, 668, 668, -1, -1, -1, -1, -1, 668, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 668, 668, 668, 668, 668, 668, -1, -1, -1, 668, 668, -1, -1, 668, 668, 668, 668, 668, 668, 668, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, 668, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, -1, -1, 670, 670, 670, -1, -1, -1, 670, 670, -1, -1, -1, -1, -1, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, 670, -1, -1, -1, 670, 670, -1, -1, 670, 670, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, -1, -1, 672, 672, 672, -1, -1, -1, 672, 672, -1, -1, -1, -1, -1, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, 672, -1, -1, -1, 672, 672, -1, -1, 672, 672, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1742, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 675, -1, -1, 675, 675, 675, -1, -1, -1, 675, 675, -1, -1, -1, -1, -1, 674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 675, 675, 675, 675, 675, 675, -1, -1, -1, 674, 674, -1, -1, 674, 674, 674, 674, 674, 674, 674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 675, 675, 675, 675, 675, 675, 675, 675, 675, 675, 675, 675, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 691, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 691, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 691, 691, -1, -1, 691, 691, 691, 691, 691, 691, 691, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 388, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 679, -1, -1, 676, 677, 680, -1, -1, -1, 678, 681, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, 683, 684, 685, 687, 688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 687, 687, 687, 687, 687, 687, 687, 687, 687, 687, 687, 687, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1743, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 689, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, 689, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 715, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 705, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 707, 708, -1, -1, 706, 709, 710, 712, 713, 714, 711, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 693, -1, -1, -1, -1, -1, -1, 698, 695, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, 697, -1, -1, -1, -1, -1, -1, 699, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 701, 702, 703, 704, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 717, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 720, 720, 720, 720, 720, 719, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 
			-1, -1, -1, -1, -1, -1, 721, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 723, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 398, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 727, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 729, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 731, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 731, -1, -1, 731, 731, 731, -1, -1, -1, 731, 731, -1, -1, -1, -1, -1, 731, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 731, 731, 731, 731, 731, 731, -1, -1, -1, 731, 731, -1, -1, 731, 731, 731, 731, 731, 731, 731, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 732, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 734, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 738, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 739, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 740, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 744, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 745, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 746, 746, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 747, 747, 747, 747, 747, 747, 747, 747, 747, 747, 747, 747, 747, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 157, -1, 157, 157, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, -1, -1, 157, 157, 157, -1, -1, -1, 157, 157, -1, -1, -1, -1, -1, 157, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, 157, 157, 157, 157, 157, -1, -1, -1, 157, 157, -1, -1, 157, 157, 157, 157, 157, 157, 157, 157, -1, 157, 736, 737, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 749, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 272, 272, 272, 272, 5, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 1786, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 750, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 
			-1, 275, 275, 275, 275, 5, 274, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 276, 275, 277, 275, 275, 278, 279, 280, 281, 282, 283, 283, 284, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 275, 
			-1, -1, -1, -1, -1, -1, 753, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 761, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 763, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 764, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 765, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 766, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 767, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 768, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 769, 769, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 770, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 755, 
			-1, 1, 1, 760, 1, 1, 1, 1, 1, 1, 1, 758, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, -1, -1, -1, -1, -1, 772, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 290, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 290, -1, -1, 290, 290, 290, -1, -1, -1, 290, 290, -1, -1, -1, -1, -1, 290, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 290, 290, 290, 290, 290, 290, -1, -1, -1, 1744, 290, -1, -1, 290, 290, 290, 290, 290, 290, 290, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 773, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 774, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 774, -1, -1, 774, 774, 774, -1, -1, -1, 774, 774, -1, -1, -1, -1, -1, 774, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 774, 774, 774, 774, 774, 774, -1, -1, -1, 774, 774, -1, -1, 774, 774, 774, 774, 774, 774, 774, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, 774, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 176, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 779, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 780, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 777, 777, 777, 777, 777, 777, 777, 777, 777, 777, 777, 777, 778, 778, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 779, 1745, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, 780, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 431, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 799, 800, 1746, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 430, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 825, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 831, 820, 821, 822, 823, 824, 826, 827, 828, 829, 830, 832, 833, 834, 835, 836, 837, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 951, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 817, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 840, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 840, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 840, -1, -1, -1, 840, 840, 840, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 840, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 840, 840, 840, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 844, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 844, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 844, -1, -1, -1, 844, 844, 844, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 844, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 844, 844, 844, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 849, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 853, -1, -1, -1, 860, 850, 851, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1748, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 852, 854, 1750, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 899, -1, 884, -1, 877, 876, 875, 880, 882, 868, 869, 870, 871, 872, 900, 861, 862, -1, -1, -1, -1, -1, -1, 898, 897, 878, 879, 881, 883, 873, 874, 885, 886, 887, 888, 889, 890, 891, 892, 893, 894, 895, 863, 864, -1, -1, -1, -1, -1, -1, 867, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1752, 896, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1754, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 927, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 911, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 911, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 911, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 911, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, 911, -1, -1, -1, -1, -1, 911, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 913, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 914, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 913, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 913, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 913, 1756, 913, 913, 913, 913, 913, 913, 913, 913, 913, -1, -1, -1, -1, -1, 915, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 916, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 916, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 916, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, 916, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 921, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 926, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 926, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 934, 934, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 941, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 946, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 947, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 949, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 949, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 950, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 950, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 936, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 937, 937, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 937, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 940, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 943, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 952, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, -1, 470, -1, -1, -1, -1, -1, 470, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 470, -1, -1, -1, -1, -1, -1, 470, 470, 470, 470, 470, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 470, 470, -1, -1, 470, 470, 470, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, 470, -1, -1, -1, 470, 470, 470, 470, 470, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, 154, 154, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, 154, 154, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, 154, 154, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, 154, 154, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, 154, 154, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, 154, 154, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 473, -1, 0, -1, -1, -1, 473, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 473, -1, -1, -1, -1, -1, -1, 473, 473, 473, 473, 473, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 473, 473, -1, -1, 473, 473, 473, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, 473, -1, -1, -1, 473, 473, 473, 473, 473, 
			-1, -1, -1, -1, -1, 475, -1, 0, -1, -1, -1, 475, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 475, -1, -1, -1, -1, -1, -1, -1, -1, -1, 475, 475, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 475, 475, -1, -1, 475, 475, 475, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, 475, -1, -1, -1, 475, 475, 475, 475, 475, 
			-1, -1, -1, -1, -1, 0, -1, -1, 477, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 479, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 483, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 484, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 484, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 484, -1, -1, -1, 484, 484, 484, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 484, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 484, 484, 484, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 486, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 488, -1, 0, 0, -1, -1, 488, -1, 488, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 489, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 494, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 495, 0, 0, 0, 0, 0, 1758, 0, 495, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, 498, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 507, 507, 507, 0, 0, 507, 0, 0, 507, 507, 507, 0, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 
			-1, 509, 509, 509, 509, 509, 509, 0, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 
			-1, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 0, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 
			-1, 513, 513, 513, 0, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 513, 
			-1, 515, 515, 515, 0, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 515, 
			-1, 517, 517, 517, 0, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 517, 
			-1, -1, -1, -1, 0, -1, -1, -1, 525, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 527, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 528, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 533, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 535, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 537, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 539, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 541, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 543, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 545, 545, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 549, 549, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 549, 549, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 555, 555, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 559, 559, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 563, 563, 563, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 568, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, 570, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 570, 570, 570, 570, 570, 570, 570, 570, 570, 570, 570, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1525, 1525, 1525, 1525, 1525, 1525, 1525, 1525, 1525, 1525, 1525, 1525, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 597, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1527, 1527, 1527, 1527, 1527, 1527, 1527, 1527, 1527, 1527, 1527, 1527, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, 599, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 59, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 602, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 606, 0, 0, -1, 0, 0, 0, 606, 606, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 606, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 606, 606, -1, -1, -1, -1, -1, -1, 606, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1534, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 631, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 633, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 635, -1, -1, 212, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 635, -1, -1, 635, 635, 635, -1, -1, -1, 635, 635, -1, -1, -1, -1, -1, 635, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 635, 635, 635, 635, 635, 635, -1, -1, -1, 635, 635, -1, -1, 635, 635, 635, 635, 635, 635, 635, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 635, 635, 635, 635, 635, 635, 635, 635, 635, 635, 635, 635, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 634, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 643, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 645, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 646, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 649, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 651, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 653, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 655, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 657, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 659, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 661, 661, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 663, 663, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 663, 663, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 665, 665, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 667, 667, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 669, 669, 669, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 671, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 673, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 673, 673, 673, 673, 673, 673, 673, 673, 673, 673, 673, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1560, 1560, 1560, 1560, 1560, 1560, 1560, 1560, 1560, 1560, 1560, 1560, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 59, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 692, 0, 0, -1, -1, 0, 0, 692, 692, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 692, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 692, 692, -1, -1, -1, -1, -1, -1, 692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1567, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 718, 718, 718, 718, 718, 718, 0, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 718, 
			-1, -1, -1, 722, -1, 722, 722, 0, -1, -1, -1, 722, -1, -1, -1, -1, -1, 722, -1, -1, 722, 722, 722, -1, -1, -1, 722, 722, -1, -1, -1, 722, -1, 722, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 722, 722, 722, 722, 722, 722, 722, 722, -1, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, -1, 722, -1, -1, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, 722, -1, -1, -1, 722, 722, 722, 722, 722, 
			-1, -1, -1, 0, -1, 724, 0, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, 0, -1, 1759, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 
			-1, -1, -1, 725, -1, 725, 725, 0, -1, -1, -1, 725, -1, -1, -1, -1, -1, 725, -1, -1, 725, 725, 725, -1, -1, -1, 725, 725, -1, -1, -1, 725, -1, 725, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 725, 725, 725, 725, 725, 725, 725, 725, -1, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, -1, 725, -1, -1, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, -1, -1, -1, 725, 725, 725, 725, 725, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 726, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 728, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 730, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 733, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 735, -1, 735, 735, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 735, -1, -1, 735, 735, 735, -1, -1, -1, 735, 735, -1, -1, -1, -1, -1, 735, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 735, 735, 735, 735, 735, 735, -1, -1, -1, 735, 735, -1, -1, 735, 735, 735, 735, 735, 735, 735, 735, -1, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, 735, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 741, -1, 741, -1, -1, 741, -1, -1, 152, -1, -1, -1, -1, -1, 741, -1, -1, 741, 741, 741, -1, -1, -1, 741, 741, -1, -1, -1, -1, -1, 1760, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 741, 741, 741, 741, 741, 741, -1, 152, -1, 741, 741, -1, -1, 741, 741, 741, 741, 741, 741, 741, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1761, 1763, 1765, 1767, 1769, 1771, 1773, 1775, 1777, 1779, 1781, 1783, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, 152, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 742, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 742, -1, -1, 742, 742, 742, -1, -1, -1, 742, 742, -1, -1, -1, -1, -1, 742, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 742, 742, 742, 742, 742, 742, -1, -1, -1, 742, 742, -1, -1, 742, 742, 742, 742, 742, 742, 742, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 742, 742, 742, 742, 742, 742, 742, 742, 742, 742, 742, 742, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 743, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 743, -1, -1, 743, 743, 743, -1, -1, -1, 743, 743, -1, -1, -1, -1, -1, 743, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 743, 743, 743, 743, 743, 743, -1, -1, -1, 743, 743, -1, -1, 743, 743, 743, 743, 743, 743, 743, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 743, 743, 743, 743, 743, 743, 743, 743, 743, 743, 743, 743, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1605, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1605, -1, -1, 1605, 1605, 1605, -1, -1, -1, 1605, 1605, -1, -1, -1, -1, -1, 1605, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1605, 1605, 1605, 1605, 1605, 1605, -1, -1, -1, 1605, 1605, -1, -1, 1605, 1605, 1605, 1605, 1605, 1605, 1605, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1605, 1605, 1605, 1605, 1605, 1605, 1605, 1605, 1605, 1605, 1605, 1605, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1606, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1606, -1, -1, 1606, 1606, 1606, -1, -1, -1, 1606, 1606, -1, -1, -1, -1, -1, 1606, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1606, 1606, 1606, 1606, 1606, 1606, -1, -1, -1, 1606, 1606, -1, -1, 1606, 1606, 1606, 1606, 1606, 1606, 1606, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1606, 1606, 1606, 1606, 1606, 1606, 1606, 1606, 1606, 1606, 1606, 1606, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1607, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1608, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1608, -1, -1, 1608, 1608, 1608, -1, -1, -1, 1608, 1608, -1, -1, -1, -1, -1, 1608, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1608, 1608, 1608, 1608, 1608, 1608, -1, -1, -1, 1608, 1608, -1, -1, 1608, 1608, 1608, 1608, 1608, 1608, 1608, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1608, 1608, 1608, 1608, 1608, 1608, 1608, 1608, 1608, 1608, 1608, 1608, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1609, 1609, 1609, 1609, 1609, 1609, 1609, 1609, 1609, 1609, 1609, 1609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 748, 748, 748, 748, 748, 748, 0, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 271, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 1, 1, 1, 1, 1, 274, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, 752, 752, 752, 752, 752, 752, 0, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 
			-1, 756, 756, 756, 0, 0, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 
			-1, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 0, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 
			-1, 759, 759, 759, 0, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 
			-1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, -1, 762, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 285, 285, 285, 285, 0, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 
			-1, 285, 285, 285, 285, 0, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 
			-1, 285, 285, 285, 0, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 
			-1, 285, 285, 285, 285, 0, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 
			-1, 285, 285, 285, 285, 0, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 83, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 285, 285, 285, 285, 0, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 
			-1, -1, -1, 771, -1, 771, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 771, -1, -1, 771, 771, 771, -1, -1, -1, 771, 771, -1, -1, -1, -1, -1, 771, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 771, 771, 771, 771, 771, 771, -1, -1, -1, 771, 771, -1, -1, 771, 771, 771, 771, 771, 771, 771, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 771, 771, 771, 771, 771, 771, 771, 771, 771, 771, 771, 771, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 289, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, -1, 0, 0, -1, 776, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1785, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 776, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 776, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 782, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, 782, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, 782, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 815, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 816, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 818, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, 0, -1, 839, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 841, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 842, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 843, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 845, -1, 0, 0, -1, 0, 845, -1, 845, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 846, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, -1, 0, 0, 848, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 134, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, 902, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 902, 902, 902, 902, 902, 902, 902, 902, 902, 902, 902, 902, 902, 902, 902, 902, 902, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 905, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 912, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 917, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 918, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 918, -1, -1, -1, 918, 918, 918, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 918, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 918, 918, 918, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 919, 0, -1, -1, -1, 0, 919, -1, 919, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 920, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 922, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 923, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 923, -1, -1, -1, 923, 923, 923, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 923, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 923, 923, 923, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 924, 0, -1, -1, -1, 0, 924, -1, 924, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 925, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1637, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 933, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 933, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 137, 1644, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1645, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 935, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 938, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, 939, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1649, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1650, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 942, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1654, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1655, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 948, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 125, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 28, -1, -1, -1, -1, 33, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1658, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 953, -1, 0, -1, -1, -1, 953, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 953, -1, -1, -1, -1, -1, -1, -1, -1, -1, 953, 953, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 953, 953, -1, -1, 953, 953, 953, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, 953, -1, -1, -1, 953, 953, 953, 953, 953, 
			-1, -1, 144, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			
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
			318, 143,  -1, // 0
			954, 4, 1487, 3, 28, 2,  -1, // 1
			957, 7, 322, 6, 956, 1488, 29,  -1, // 2
			322, 143,  -1, // 3
			959, 7, 323, 6, 958, 1489, 30,  -1, // 4
			323, 155,  -1, // 5
			5, 324, 158, 31,  -1, // 6
			324, 158, 8,  -1, // 7
			5, 325, 158, 32,  -1, // 8
			325, 158, 8,  -1, // 9
			960, 9, 1490,  -1, // 10
			1330, 160,  -1, // 11
			326, 961, 1491,  -1, // 12
			326, 962, 1492, 10,  -1, // 13
			1331, 162,  -1, // 14
			1332, 163, 327, 1493,  -1, // 15
			328, 1333, 164,  -1, // 16
			1338, 163, 963, 330, 329, 1335, 166, 1334, 165, 328,  -1, // 17
			329, 1336, 167,  -1, // 18
			330, 1337, 168,  -1, // 19
			3, 1339, 165,  -1, // 20
			964, 5,  -1, // 21
			965, 1494,  -1, // 22
			967, 332, 331, 966, 1495,  -1, // 23
			331, 1340, 164,  -1, // 24
			332, 1341, 167,  -1, // 25
			1342, 170,  -1, // 26
			333, 968, 1496,  -1, // 27
			333, 969, 1497, 8,  -1, // 28
			970, 1498,  -1, // 29
			971, 1499,  -1, // 30
			12, 177, 972, 11,  -1, // 31
			334, 178, 8,  -1, // 32
			334, 178,  -1, // 33
			179, 13,  -1, // 34
			973, 335, 1500,  -1, // 35
			336, 1343, 180,  -1, // 36
			336, 1343, 180,  -1, // 37
			975, 1502, 337, 1344, 180, 974, 1501,  -1, // 38
			337, 1344, 180,  -1, // 39
			977, 1504, 338, 1345, 180, 976, 1503,  -1, // 40
			338, 1345, 180,  -1, // 41
			979, 1506, 339, 978, 1505,  -1, // 42
			339, 1346, 180,  -1, // 43
			980, 1507,  -1, // 44
			340, 1347, 182,  -1, // 45
			4, 340, 3,  -1, // 46
			341, 1348, 182,  -1, // 47
			4, 341, 3,  -1, // 48
			981, 1508,  -1, // 49
			982, 1509,  -1, // 50
			983, 1510,  -1, // 51
			1349, 187,  -1, // 52
			1350, 187,  -1, // 53
			342, 984, 1511,  -1, // 54
			342, 985, 1512, 8,  -1, // 55
			343, 986, 1513,  -1, // 56
			343, 987, 1514, 8,  -1, // 57
			344, 989, 1515, 8,  -1, // 58
			344, 989, 1515, 8, 988,  -1, // 59
			990, 1351, 187,  -1, // 60
			991, 1516,  -1, // 61
			345, 1352, 192,  -1, // 62
			994, 1354, 187, 9, 993, 1353, 187, 14, 992,  -1, // 63
			346, 1355, 193,  -1, // 64
			346, 995, 1356, 193, 34,  -1, // 65
			347, 1357, 194,  -1, // 66
			347, 996, 1358, 194, 35,  -1, // 67
			348, 1359, 195,  -1, // 68
			348, 997, 1360, 195, 15,  -1, // 69
			349, 1361, 196,  -1, // 70
			349, 998, 1362, 196, 16,  -1, // 71
			350, 1363, 197,  -1, // 72
			350, 999, 1364, 197, 17,  -1, // 73
			351, 1365, 198,  -1, // 74
			351, 1000, 1366, 198, 1517,  -1, // 75
			1001, 36,  -1, // 76
			1002, 37,  -1, // 77
			352, 1367, 200,  -1, // 78
			352, 1003, 1368, 200, 1518,  -1, // 79
			1004, 18,  -1, // 80
			1005, 19,  -1, // 81
			1006, 38,  -1, // 82
			1007, 39,  -1, // 83
			353, 1369, 202,  -1, // 84
			353, 1008, 1370, 202, 1519,  -1, // 85
			1009, 40,  -1, // 86
			1010, 41,  -1, // 87
			354, 1371, 204,  -1, // 88
			354, 1011, 1372, 204, 1520,  -1, // 89
			1012, 20,  -1, // 90
			1013, 21,  -1, // 91
			355, 1373, 206,  -1, // 92
			355, 1014, 1374, 206, 1521,  -1, // 93
			1015, 22,  -1, // 94
			1016, 23,  -1, // 95
			1017, 24,  -1, // 96
			356, 1375, 208,  -1, // 97
			356, 1018, 1376, 208, 25,  -1, // 98
			357, 1377, 209,  -1, // 99
			1378, 210,  -1, // 100
			1019, 1379, 187, 1522,  -1, // 101
			212, 1020, 13,  -1, // 102
			1021, 13,  -1, // 103
			1022, 42,  -1, // 104
			1023, 43,  -1, // 105
			1024, 44,  -1, // 106
			1025, 45,  -1, // 107
			1026, 46,  -1, // 108
			1027, 47,  -1, // 109
			1028, 48,  -1, // 110
			1029, 49,  -1, // 111
			1030, 50,  -1, // 112
			1031, 51,  -1, // 113
			1032, 52,  -1, // 114
			1380, 213,  -1, // 115
			1381, 214,  -1, // 116
			1033, 1382, 209, 20,  -1, // 117
			1034, 1383, 209, 21,  -1, // 118
			1035, 1384, 209, 26,  -1, // 119
			1036, 1385, 209, 17,  -1, // 120
			1037, 1386, 209, 22,  -1, // 121
			1038, 1387, 209, 27,  -1, // 122
			1039, 1388, 209, 53,  -1, // 123
			1040, 1389, 209, 54,  -1, // 124
			1041, 4, 1523, 3, 55,  -1, // 125
			1042, 4, 1524, 3, 56,  -1, // 126
			1043, 1390, 209, 4, 1526, 358, 3,  -1, // 127
			361, 1044, 1528, 57, 360,  -1, // 128
			1391, 218,  -1, // 129
			1045, 1392, 209, 58,  -1, // 130
			1393, 170,  -1, // 131
			1046, 4, 1530, 3, 362, 1529,  -1, // 132
			3, 158,  -1, // 133
			1394, 221, 364, 3,  -1, // 134
			365, 1395, 222,  -1, // 135
			365, 1396, 223,  -1, // 136
			1047, 4, 1531, 3,  -1, // 137
			1048, 4, 1532, 3, 26,  -1, // 138
			1049, 12, 1533, 11,  -1, // 139
			1050, 53,  -1, // 140
			1051, 54,  -1, // 141
			1052, 1397, 224, 366, 10,  -1, // 142
			1398, 224, 1053, 61,  -1, // 143
			1054, 1535,  -1, // 144
			1055, 62,  -1, // 145
			1056, 63,  -1, // 146
			1057, 64,  -1, // 147
			1058, 65,  -1, // 148
			1059, 1536,  -1, // 149
			1060, 66,  -1, // 150
			1061, 62,  -1, // 151
			1062, 63,  -1, // 152
			1063, 1537,  -1, // 153
			1064, 1538,  -1, // 154
			1065, 1539,  -1, // 155
			1066, 69,  -1, // 156
			1067, 70,  -1, // 157
			1068, 71,  -1, // 158
			4, 1399, 187, 3,  -1, // 159
			367, 1069, 1540,  -1, // 160
			367, 1070, 1541,  -1, // 161
			1071, 7, 368, 1400, 226, 6,  -1, // 162
			368, 1401, 226, 8,  -1, // 163
			369, 1073, 13, 1542,  -1, // 164
			1074, 1543,  -1, // 165
			1076, 1544, 1075,  -1, // 166
			212, 1077,  -1, // 167
			1078, 0,  -1, // 168
			1079, 1545,  -1, // 169
			1402, 228,  -1, // 170
			1403, 228,  -1, // 171
			371, 1080, 1546,  -1, // 172
			371, 1081, 1547, 8,  -1, // 173
			372, 1082, 1548,  -1, // 174
			372, 1083, 1549, 8,  -1, // 175
			373, 1085, 1550, 8,  -1, // 176
			373, 1085, 1550, 8, 1084,  -1, // 177
			374, 1404, 232,  -1, // 178
			1086, 1406, 228, 9, 1405, 228, 14,  -1, // 179
			375, 1407, 233,  -1, // 180
			375, 1087, 1408, 233, 34,  -1, // 181
			376, 1409, 234,  -1, // 182
			376, 1088, 1410, 234, 35,  -1, // 183
			377, 1411, 235,  -1, // 184
			377, 1089, 1412, 235, 15,  -1, // 185
			378, 1413, 236,  -1, // 186
			378, 1090, 1414, 236, 16,  -1, // 187
			379, 1415, 237,  -1, // 188
			379, 1091, 1416, 237, 17,  -1, // 189
			380, 1417, 238,  -1, // 190
			380, 1092, 1418, 238, 1551,  -1, // 191
			381, 1419, 239,  -1, // 192
			381, 1093, 1420, 239, 1552,  -1, // 193
			382, 1421, 240,  -1, // 194
			382, 1094, 1422, 240, 1553,  -1, // 195
			383, 1423, 241,  -1, // 196
			383, 1095, 1424, 241, 1554,  -1, // 197
			384, 1425, 242,  -1, // 198
			384, 1096, 1426, 242, 1555,  -1, // 199
			385, 1427, 243,  -1, // 200
			385, 1097, 1428, 243, 25,  -1, // 201
			386, 1429, 244,  -1, // 202
			1099, 1430, 228, 1556, 1098,  -1, // 203
			1431, 245,  -1, // 204
			1432, 246,  -1, // 205
			1100, 1433, 244, 20,  -1, // 206
			1101, 1434, 244, 21,  -1, // 207
			1102, 1435, 244, 26,  -1, // 208
			1103, 1436, 244, 17,  -1, // 209
			1104, 1437, 244, 22,  -1, // 210
			1105, 1438, 244, 27,  -1, // 211
			1106, 1439, 244, 53,  -1, // 212
			1107, 1440, 244, 54,  -1, // 213
			1108, 4, 1557, 3, 55,  -1, // 214
			1109, 4, 1558, 3, 56,  -1, // 215
			1110, 1441, 244, 4, 1559, 387, 3,  -1, // 216
			1111, 1561, 57, 389,  -1, // 217
			1112, 1442, 244, 58,  -1, // 218
			1113, 1562,  -1, // 219
			1114, 4, 231, 3, 390, 1563,  -1, // 220
			392, 1443, 248,  -1, // 221
			392, 1444, 249,  -1, // 222
			1115, 4, 1564, 3,  -1, // 223
			1116, 4, 1565, 3, 26,  -1, // 224
			1117, 12, 1566, 11,  -1, // 225
			1118, 53,  -1, // 226
			1119, 54,  -1, // 227
			1120, 1445, 250, 393, 10,  -1, // 228
			1446, 250, 1121, 61,  -1, // 229
			1122, 1568,  -1, // 230
			1123, 62,  -1, // 231
			1124, 63,  -1, // 232
			1125, 64,  -1, // 233
			1126, 65,  -1, // 234
			1127, 1569,  -1, // 235
			1128, 66,  -1, // 236
			1129, 62,  -1, // 237
			1130, 63,  -1, // 238
			1131, 1570,  -1, // 239
			1132, 68,  -1, // 240
			1133, 1571,  -1, // 241
			1134, 69,  -1, // 242
			1135, 70,  -1, // 243
			1136, 71,  -1, // 244
			4, 1447, 228, 3,  -1, // 245
			1448, 251,  -1, // 246
			1138, 1573, 394, 1137, 1572,  -1, // 247
			394, 1449, 252,  -1, // 248
			1450, 251,  -1, // 249
			1139, 1574,  -1, // 250
			1141, 1576, 395, 1140, 1575,  -1, // 251
			395, 156,  -1, // 252
			1144, 1578, 397, 1143, 396, 1142, 1577,  -1, // 253
			396, 255,  -1, // 254
			397, 156,  -1, // 255
			1145, 4, 1580, 3, 1579,  -1, // 256
			59, 399, 10, 33,  -1, // 257
			399, 10, 33,  -1, // 258
			59, 400, 1146, 10, 1581,  -1, // 259
			400, 1147, 10, 1582,  -1, // 260
			5, 184,  -1, // 261
			1150, 401, 1585, 1148, 1584, 1583, 3, 73,  -1, // 262
			1587, 1149, 1586,  -1, // 263
			1152, 1590, 402, 1151, 1589, 4, 1588, 3, 75,  -1, // 264
			402, 1451, 268,  -1, // 265
			1153, 1593, 1592, 1591,  -1, // 266
			1154, 1595, 1594,  -1, // 267
			1156, 1598, 1155, 1597, 1596, 3, 78,  -1, // 268
			1159, 5, 4, 1601, 3, 78, 1158, 1600, 1157, 1599,  -1, // 269
			1166, 1604, 1165, 4, 405, 5, 404, 403, 1160, 3, 1602,  -1, // 270
			5, 186,  -1, // 271
			1162, 1603, 1161,  -1, // 272
			1164, 189, 1163,  -1, // 273
			1167, 5, 406, 81,  -1, // 274
			1168, 5, 407, 82,  -1, // 275
			1169, 5, 409, 84, 408,  -1, // 276
			1171, 1611, 1170, 1610, 410,  -1, // 277
			411, 270,  -1, // 278
			1172, 7, 411, 270, 6,  -1, // 279
			1175, 157, 1174, 4, 189, 3, 62,  -1, // 280
			1176, 5, 1612,  -1, // 281
			414, 413,  -1, // 282
			7, 414, 6,  -1, // 283
			5, 285,  -1, // 284
			415, 286,  -1, // 285
			415, 286,  -1, // 286
			416, 286,  -1, // 287
			12, 416, 286, 11,  -1, // 288
			417, 286,  -1, // 289
			4, 417, 3,  -1, // 290
			418, 273, 4, 285, 3, 73,  -1, // 291
			273, 74,  -1, // 292
			274, 4, 285, 3, 75,  -1, // 293
			273, 4, 285, 3, 78,  -1, // 294
			5, 4, 285, 3, 78, 273, 79,  -1, // 295
			273, 4, 421, 5, 420, 5, 419, 3, 80,  -1, // 296
			5, 422, 81,  -1, // 297
			5, 423, 82,  -1, // 298
			5, 425, 84, 424,  -1, // 299
			273, 85,  -1, // 300
			426, 288,  -1, // 301
			7, 426, 288, 6,  -1, // 302
			273, 1178, 4, 230, 3, 62,  -1, // 303
			1179, 5, 175,  -1, // 304
			428, 1452, 291,  -1, // 305
			428, 1452, 291,  -1, // 306
			1180, 1613,  -1, // 307
			1181, 1614,  -1, // 308
			1182, 1615,  -1, // 309
			1183, 1616,  -1, // 310
			429, 1453, 221,  -1, // 311
			429, 1453, 221,  -1, // 312
			1184, 1617,  -1, // 313
			1185, 1618,  -1, // 314
			1186, 86,  -1, // 315
			1187, 87,  -1, // 316
			1188, 88,  -1, // 317
			1189, 89,  -1, // 318
			1190, 90,  -1, // 319
			1191, 91,  -1, // 320
			1192, 92,  -1, // 321
			1193, 93,  -1, // 322
			1194, 94,  -1, // 323
			1195, 95,  -1, // 324
			1196, 96,  -1, // 325
			1197, 97,  -1, // 326
			1198, 98,  -1, // 327
			1199, 99,  -1, // 328
			1200, 100,  -1, // 329
			1201, 101,  -1, // 330
			1202, 102,  -1, // 331
			1203, 19, 1619, 18, 102,  -1, // 332
			1204, 103,  -1, // 333
			1205, 104,  -1, // 334
			1206, 105,  -1, // 335
			1207, 106,  -1, // 336
			1208, 107,  -1, // 337
			1209, 108,  -1, // 338
			1210, 109,  -1, // 339
			1211, 110,  -1, // 340
			1212, 111,  -1, // 341
			1213, 112,  -1, // 342
			1214, 113,  -1, // 343
			6, 114,  -1, // 344
			1215, 1620,  -1, // 345
			1216, 1621,  -1, // 346
			432, 1218, 1622, 1217,  -1, // 347
			1219, 3,  -1, // 348
			1220, 1623,  -1, // 349
			1221, 115,  -1, // 350
			1222, 116,  -1, // 351
			1223, 117,  -1, // 352
			1224, 118,  -1, // 353
			1225, 119,  -1, // 354
			1226, 60,  -1, // 355
			1227, 120,  -1, // 356
			1228, 121,  -1, // 357
			1229, 122,  -1, // 358
			1230, 123,  -1, // 359
			1231, 124,  -1, // 360
			1232, 114,  -1, // 361
			1233, 125,  -1, // 362
			1234, 126,  -1, // 363
			1235, 127,  -1, // 364
			1236, 128,  -1, // 365
			1237, 129,  -1, // 366
			1238, 130,  -1, // 367
			433, 1454, 296,  -1, // 368
			433, 1455, 296, 8,  -1, // 369
			1241, 435, 434, 1456, 1624,  -1, // 370
			1239, 1625, 59,  -1, // 371
			1240, 1626, 13,  -1, // 372
			436, 1457, 164,  -1, // 373
			438, 437, 1458, 165, 436,  -1, // 374
			437, 1459, 167,  -1, // 375
			438, 1460, 168,  -1, // 376
			439, 1242, 1461, 1627,  -1, // 377
			439, 1462, 298, 10,  -1, // 378
			1243, 1628,  -1, // 379
			1244, 64,  -1, // 380
			1245, 65,  -1, // 381
			1246, 131,  -1, // 382
			1247, 59,  -1, // 383
			1248, 132,  -1, // 384
			1249, 59, 88,  -1, // 385
			1250, 132, 88,  -1, // 386
			1251, 4, 3, 133,  -1, // 387
			1252, 1629, 133,  -1, // 388
			1253, 1630, 133,  -1, // 389
			1254, 63,  -1, // 390
			1255, 26,  -1, // 391
			1256, 27,  -1, // 392
			1257, 53,  -1, // 393
			1258, 54,  -1, // 394
			1259, 53, 134,  -1, // 395
			1260, 54, 134,  -1, // 396
			1261, 61, 440,  -1, // 397
			1262, 20,  -1, // 398
			1263, 21,  -1, // 399
			1264, 22,  -1, // 400
			1265, 23,  -1, // 401
			1266, 24,  -1, // 402
			1267, 40,  -1, // 403
			1268, 41,  -1, // 404
			1269, 17,  -1, // 405
			1270, 16,  -1, // 406
			1271, 15,  -1, // 407
			1272, 36,  -1, // 408
			1273, 37,  -1, // 409
			1274, 18,  -1, // 410
			1275, 38,  -1, // 411
			1276, 19,  -1, // 412
			1277, 39,  -1, // 413
			1278, 13,  -1, // 414
			1279, 42,  -1, // 415
			1280, 43,  -1, // 416
			1281, 44,  -1, // 417
			1282, 45,  -1, // 418
			1283, 46,  -1, // 419
			1284, 47,  -1, // 420
			1285, 48,  -1, // 421
			1286, 49,  -1, // 422
			1287, 50,  -1, // 423
			1288, 51,  -1, // 424
			1289, 52,  -1, // 425
			1290, 135,  -1, // 426
			1291, 35,  -1, // 427
			1292, 34,  -1, // 428
			1293, 12, 11,  -1, // 429
			1294, 25,  -1, // 430
			441, 1295, 22,  -1, // 431
			441, 1296, 1631,  -1, // 432
			1463, 300,  -1, // 433
			1464, 166,  -1, // 434
			1465, 301,  -1, // 435
			1298, 12, 1632, 11,  -1, // 436
			1299, 12, 1633, 11,  -1, // 437
			1300, 12, 11,  -1, // 438
			4, 1466, 302, 3,  -1, // 439
			4, 3,  -1, // 440
			443, 1467, 303,  -1, // 441
			443, 1468, 303, 8,  -1, // 442
			1469, 304,  -1, // 443
			1470, 305,  -1, // 444
			1471, 306,  -1, // 445
			1303, 447, 446, 445, 444, 1301, 1634,  -1, // 446
			444, 1472, 164,  -1, // 447
			1473, 165,  -1, // 448
			446, 1474, 167,  -1, // 449
			1302, 1635, 13,  -1, // 450
			1305, 451, 450, 449, 448, 1475, 164,  -1, // 451
			448, 1475, 164,  -1, // 452
			1476, 165,  -1, // 453
			450, 1477, 167,  -1, // 454
			1304, 1636, 13,  -1, // 455
			1306, 136, 452,  -1, // 456
			1307, 1638, 9,  -1, // 457
			1308, 117,  -1, // 458
			1309, 1639,  -1, // 459
			1310, 1640,  -1, // 460
			1311, 1641,  -1, // 461
			1312, 1642,  -1, // 462
			1313, 1643,  -1, // 463
			1478, 313, 1314, 456, 455, 454,  -1, // 464
			1646, 9,  -1, // 465
			1316, 7, 1479, 314, 1315, 6,  -1, // 466
			457, 1480, 315,  -1, // 467
			457, 1481, 315, 8,  -1, // 468
			1648, 13,  -1, // 469
			1317, 458, 1647,  -1, // 470
			1482, 317, 1318, 461, 460, 459, 139,  -1, // 471
			1651, 9,  -1, // 472
			1319, 4, 1652, 3, 28,  -1, // 473
			1320, 1653,  -1, // 474
			1321, 77,  -1, // 475
			1483, 317, 1322, 462, 140,  -1, // 476
			1484, 317, 1323, 464, 1656, 463, 141,  -1, // 477
			1657, 9,  -1, // 478
			466, 114, 465,  -1, // 479
			1325, 1485, 317, 1324, 1659, 114, 467,  -1, // 480
			1327, 1486, 317, 1326, 114,  -1, // 481
			1329, 7, 468, 1328, 6,  -1, // 482
			468, 155,  -1, // 483
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 10, 18, 21, 29, 32, 37, 41, 46, 50, 54, 57, 61, 66, 69, 74, 78, 89, 93, 97, 101, 104, 107, 113, 117, 121, 124, 128, 133, 136, 139, 144, 148, 151, 154, 158, 162, 166, 174, 178, 186, 190, 196, 200, 203, 207, 211, 215, 219, 222, 225, 228, 231, 234, 238, 243, 247, 252, 257, 263, 267, 270, 274, 284, 288, 294, 298, 304, 308, 314, 318, 324, 328, 334, 338, 344, 347, 350, 354, 360, 363, 366, 369, 372, 376, 382, 385, 388, 392, 398, 401, 404, 408, 414, 417, 420, 423, 427, 433, 437, 440, 445, 449, 452, 455, 458, 461, 464, 467, 470, 473, 476, 479, 482, 485, 488, 491, 496, 501, 506, 511, 516, 521, 526, 531, 537, 543, 551, 557, 560, 565, 568, 575, 578, 583, 587, 591, 596, 602, 607, 610, 613, 619, 624, 627, 630, 633, 636, 639, 642, 645, 648, 651, 654, 657, 660, 663, 666, 669, 674, 678, 682, 689, 694, 699, 702, 706, 709, 712, 715, 718, 721, 725, 730, 734, 739, 744, 750, 754, 762, 766, 772, 776, 782, 786, 792, 796, 802, 806, 812, 816, 822, 826, 832, 836, 842, 846, 852, 856, 862, 866, 872, 876, 882, 885, 888, 893, 898, 903, 908, 913, 918, 923, 928, 934, 940, 948, 953, 958, 961, 968, 972, 976, 981, 987, 992, 995, 998, 1004, 1009, 1012, 1015, 1018, 1021, 1024, 1027, 1030, 1033, 1036, 1039, 1042, 1045, 1048, 1051, 1054, 1059, 1062, 1068, 1072, 1075, 1078, 1084, 1087, 1095, 1098, 1101, 1107, 1112, 1116, 1122, 1127, 1130, 1139, 1143, 1153, 1157, 1162, 1166, 1174, 1185, 1197, 1200, 1204, 1208, 1213, 1218, 1224, 1230, 1233, 1239, 1247, 1251, 1254, 1258, 1261, 1264, 1267, 1270, 1275, 1278, 1282, 1289, 1292, 1298, 1304, 1312, 1322, 1326, 1330, 1335, 1338, 1341, 1346, 1353, 1357, 1361, 1365, 1368, 1371, 1374, 1377, 1381, 1385, 1388, 1391, 1394, 1397, 1400, 1403, 1406, 1409, 1412, 1415, 1418, 1421, 1424, 1427, 1430, 1433, 1436, 1439, 1442, 1448, 1451, 1454, 1457, 1460, 1463, 1466, 1469, 1472, 1475, 1478, 1481, 1484, 1487, 1490, 1495, 1498, 1501, 1504, 1507, 1510, 1513, 1516, 1519, 1522, 1525, 1528, 1531, 1534, 1537, 1540, 1543, 1546, 1549, 1552, 1555, 1559, 1564, 1570, 1574, 1578, 1582, 1588, 1592, 1596, 1601, 1606, 1609, 1612, 1615, 1618, 1621, 1624, 1628, 1632, 1637, 1641, 1645, 1648, 1651, 1654, 1657, 1660, 1664, 1668, 1672, 1675, 1678, 1681, 1684, 1687, 1690, 1693, 1696, 1699, 1702, 1705, 1708, 1711, 1714, 1717, 1720, 1723, 1726, 1729, 1732, 1735, 1738, 1741, 1744, 1747, 1750, 1753, 1756, 1759, 1762, 1765, 1769, 1772, 1776, 1780, 1783, 1786, 1789, 1794, 1799, 1803, 1808, 1811, 1815, 1820, 1823, 1826, 1829, 1837, 1841, 1844, 1848, 1852, 1860, 1864, 1867, 1871, 1875, 1879, 1883, 1886, 1889, 1892, 1895, 1898, 1901, 1908, 1911, 1918, 1922, 1927, 1930, 1934, 1942, 1945, 1951, 1954, 1957, 1963, 1971, 1974, 1978, 1986, 1992, 1998, 
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
		
		case EToken_LogOr:
			return 34;
		
		case EToken_LogAnd:
			return 35;
		
		case EToken_Eq:
			return 36;
		
		case EToken_Ne:
			return 37;
		
		case EToken_Le:
			return 38;
		
		case EToken_Ge:
			return 39;
		
		case EToken_Shl:
			return 40;
		
		case EToken_Shr:
			return 41;
		
		case EToken_RefAssign:
			return 42;
		
		case EToken_AddAssign:
			return 43;
		
		case EToken_SubAssign:
			return 44;
		
		case EToken_MulAssign:
			return 45;
		
		case EToken_DivAssign:
			return 46;
		
		case EToken_ModAssign:
			return 47;
		
		case EToken_ShlAssign:
			return 48;
		
		case EToken_ShrAssign:
			return 49;
		
		case EToken_AndAssign:
			return 50;
		
		case EToken_XorAssign:
			return 51;
		
		case EToken_OrAssign:
			return 52;
		
		case EToken_Inc:
			return 53;
		
		case EToken_Dec:
			return 54;
		
		case EToken_SizeOf:
			return 55;
		
		case EToken_TypeOf:
			return 56;
		
		case EToken_New:
			return 57;
		
		case EToken_Delete:
			return 58;
		
		case EToken_Construct:
			return 59;
		
		case EToken_Weak:
			return 60;
		
		case EToken_Ptr:
			return 61;
		
		case EToken_OnChange:
			return 62;
		
		case EToken_PropValue:
			return 63;
		
		case EToken_Get:
			return 64;
		
		case EToken_Set:
			return 65;
		
		case EToken_This:
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
		
		case EToken_Literal:
			return 72;
		
		case EToken_If:
			return 73;
		
		case EToken_Else:
			return 74;
		
		case EToken_Switch:
			return 75;
		
		case EToken_Case:
			return 76;
		
		case EToken_Default:
			return 77;
		
		case EToken_While:
			return 78;
		
		case EToken_Do:
			return 79;
		
		case EToken_For:
			return 80;
		
		case EToken_Break:
			return 81;
		
		case EToken_Continue:
			return 82;
		
		case EToken_Silent:
			return 83;
		
		case EToken_Return:
			return 84;
		
		case EToken_Once:
			return 85;
		
		case EToken_Typedef:
			return 86;
		
		case EToken_Alias:
			return 87;
		
		case EToken_Static:
			return 88;
		
		case EToken_Thread:
			return 89;
		
		case EToken_Stack:
			return 90;
		
		case EToken_Heap:
			return 91;
		
		case EToken_UHeap:
			return 92;
		
		case EToken_Abstract:
			return 93;
		
		case EToken_Virtual:
			return 94;
		
		case EToken_Override:
			return 95;
		
		case EToken_Mutable:
			return 96;
		
		case EToken_Nullable:
			return 97;
		
		case EToken_Public:
			return 98;
		
		case EToken_Protected:
			return 99;
		
		case EToken_Void:
			return 100;
		
		case EToken_Variant:
			return 101;
		
		case EToken_Object:
			return 102;
		
		case EToken_Bool:
			return 103;
		
		case EToken_Int8:
			return 104;
		
		case EToken_Int16:
			return 105;
		
		case EToken_Int32:
			return 106;
		
		case EToken_Int64:
			return 107;
		
		case EToken_Int:
			return 108;
		
		case EToken_IntPtr:
			return 109;
		
		case EToken_Char:
			return 110;
		
		case EToken_Float:
			return 111;
		
		case EToken_Double:
			return 112;
		
		case EToken_Auto:
			return 113;
		
		case EToken_Property:
			return 114;
		
		case EToken_Unsigned:
			return 115;
		
		case EToken_BigEndian:
			return 116;
		
		case EToken_Const:
			return 117;
		
		case EToken_ReadOnly:
			return 118;
		
		case EToken_Volatile:
			return 119;
		
		case EToken_Thin:
			return 120;
		
		case EToken_Unsafe:
			return 121;
		
		case EToken_Cdecl:
			return 122;
		
		case EToken_Stdcall:
			return 123;
		
		case EToken_Function:
			return 124;
		
		case EToken_Bindable:
			return 125;
		
		case EToken_AutoGet:
			return 126;
		
		case EToken_Indexed:
			return 127;
		
		case EToken_Multicast:
			return 128;
		
		case EToken_Event:
			return 129;
		
		case EToken_AutoEv:
			return 130;
		
		case EToken_PreConstruct:
			return 131;
		
		case EToken_Destruct:
			return 132;
		
		case EToken_Operator:
			return 133;
		
		case EToken_Postfix:
			return 134;
		
		case EToken_AtAssign:
			return 135;
		
		case EToken_Ellipsis:
			return 136;
		
		case EToken_Enum:
			return 137;
		
		case EToken_EnumC:
			return 138;
		
		case EToken_Struct:
			return 139;
		
		case EToken_Union:
			return 140;
		
		case EToken_Class:
			return 141;
		
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
			EToken_SizeOf, 
			EToken_TypeOf, 
			EToken_New, 
			EToken_Delete, 
			EToken_Construct, 
			EToken_Weak, 
			EToken_Ptr, 
			EToken_OnChange, 
			EToken_PropValue, 
			EToken_Get, 
			EToken_Set, 
			EToken_This, 
			EToken_Integer, 
			EToken_Fp, 
			EToken_True, 
			EToken_False, 
			EToken_Null, 
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
			EToken_Silent, 
			EToken_Return, 
			EToken_Once, 
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
			EToken_ReadOnly, 
			EToken_Volatile, 
			EToken_Thin, 
			EToken_Unsafe, 
			EToken_Cdecl, 
			EToken_Stdcall, 
			EToken_Function, 
			EToken_Bindable, 
			EToken_AutoGet, 
			EToken_Indexed, 
			EToken_Multicast, 
			EToken_Event, 
			EToken_AutoEv, 
			EToken_PreConstruct, 
			EToken_Destruct, 
			EToken_Operator, 
			EToken_Postfix, 
			EToken_AtAssign, 
			EToken_Ellipsis, 
			EToken_Enum, 
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
			"function_body_pass1",
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
			"compound_stmt_pass1",
			"statement_pass1",
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
			"switch_block_stmt",
			"autoev_body",
			"autoev_stmt",
			"autoev_onchange_stmt",
			"autoev_expression_stmt",
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
			"autoev_body_0",
			"autoev_stmt_0",
			"autoev_onchange_stmt_0",
			"autoev_expression_stmt_0",
			"declaration_specifier",
			"type_specifier",
			"type_modifier",
			"property_template_specifier",
			"qualified_type_name_rslv",
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
			"property_specifier_rslv",
			"property_specifier",
			"enum_block",
			"enum_member_list",
			"enum_member",
			"struct_pack_factor",
			"named_type_block",
			
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls42);
				
			break;
			
		
		
		
		
		
		
		case ESymbol_named_type_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls38);
				
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
			
		
		case ESymbol_function_body_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls27);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls2);
				
			break;
			
		
		case ESymbol_type_name_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls3);
				
			break;
			
		
		case ESymbol_type_name_or_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls4);
				
			break;
			
		
		case ESymbol_expression_s:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls22);
				
			break;
			
		
		
		
		
		case ESymbol_expression_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls5);
				
			break;
			
		
		case ESymbol_primary_expr_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_primary_expr_pass1);
			
			break;
			
		
		case ESymbol_arg_list_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls6);
				
			break;
			
		
		case ESymbol_arg_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_arg_pass1);
			
			break;
			
		
		
		case ESymbol_expression:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls7);
				
			break;
			
		
		
		case ESymbol_expression_or_empty_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls10);
				
			break;
			
		
		case ESymbol_conditional_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_conditional_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls13);
			
			break;
			
		
		case ESymbol_expression_or_empty:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls8);
				
			break;
			
		
		case ESymbol_expression_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls9);
				
			break;
			
		
		case ESymbol_constant_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls11);
				
			break;
			
		
		case ESymbol_constant_integer_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls12);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls14);
				
			break;
			
		
		case ESymbol_shift_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_shift_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_relational_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls15);
				
			break;
			
		
		case ESymbol_additive_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_additive_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_shift_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls16);
				
			break;
			
		
		case ESymbol_multiplicative_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_multiplicative_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_additive_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls17);
				
			break;
			
		
		case ESymbol_at_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_at_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_multiplicative_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls18);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls19);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls20);
				
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
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls21);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls23);
				
			break;
			
		
		case ESymbol_expression_list_s:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls24);
				
			break;
			
		
		case ESymbol_expression_or_empty_list_s:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls25);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls26);
				
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
			
		
		case ESymbol_compound_stmt_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_compound_stmt_pass1);
			
			break;
			
		
		case ESymbol_statement_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_statement_pass1);
			
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
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_autoev_stmt_0:
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls35);
				
			break;
			
		
		case ESymbol_property_template_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_property_template_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls46);
			
			break;
			
		
		
		case ESymbol_full_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_full_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls36);
			
			break;
			
		
		case ESymbol_declarator_qualifier:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_qualifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_operator_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls37);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls39);
				
			break;
			
		
		case ESymbol_struct_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls40);
				
			break;
			
		
		case ESymbol_union_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls43);
				
			break;
			
		
		case ESymbol_class_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls44);
				
			break;
			
		
		
		case ESymbol_property_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls45);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls41);
				
			break;
			
		
		case ESymbol_named_type_block:
			pNode = AXL_MEM_NEW (CSymbolNode_named_type_block);
			
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
			
			{ 0, 145 },
			
			{ 0, 158 },
			
			{ 0, 158 },
			
			{ 0, 159 },
			
			{ 0, 33 },
			
			{ 1, 33 },
			
			{ 0, 161 },
			
			{ 0, 169 },
			
			{ 0, 171 },
			
			{ 0, 172 },
			
			{ 1, 172 },
			
			{ 0, 172 },
			
			{ 0, 175 },
			
			{ 0, 33 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 11 },
			
			{ 1, 12 },
			
			{ 0, 3 },
			
			{ 1, 4 },
			
			{ 0, 1 },
			
			{ 0, 1 },
			
			{ 0, 184 },
			
			{ 0, 186 },
			
			{ 0, 184 },
			
			{ 1, 184 },
			
			{ 0, 184 },
			
			{ 1, 188 },
			
			{ 0, 188 },
			
			{ 0, 184 },
			
			{ 0, 199 },
			
			{ 0, 201 },
			
			{ 0, 203 },
			
			{ 0, 205 },
			
			{ 0, 207 },
			
			{ 0, 211 },
			
			{ 0, 174 },
			
			{ 0, 174 },
			
			{ 0, 216 },
			
			{ 1, 172 },
			
			{ 0, 216 },
			
			{ 1, 217 },
			
			{ 0, 220 },
			
			{ 1, 186 },
			
			{ 0, 186 },
			
			{ 0, 186 },
			
			{ 0, 184 },
			
			{ 0, 60 },
			
			{ 0, 33 },
			
			{ 0, 33 },
			
			{ 0, 67 },
			
			{ 0, 68 },
			
			{ 0, 225 },
			
			{ 0, 72 },
			
			{ 1, 72 },
			
			{ 0, 33 },
			
			{ 1, 184 },
			
			{ 0, 184 },
			
			{ 0, 175 },
			
			{ 0, 175 },
			
			{ 1, 175 },
			
			{ 0, 175 },
			
			{ 1, 229 },
			
			{ 0, 229 },
			
			{ 0, 199 },
			
			{ 0, 201 },
			
			{ 0, 203 },
			
			{ 0, 205 },
			
			{ 0, 207 },
			
			{ 0, 211 },
			
			{ 0, 174 },
			
			{ 0, 174 },
			
			{ 0, 172 },
			
			{ 0, 216 },
			
			{ 1, 247 },
			
			{ 0, 172 },
			
			{ 0, 220 },
			
			{ 0, 231 },
			
			{ 0, 231 },
			
			{ 0, 175 },
			
			{ 0, 60 },
			
			{ 0, 33 },
			
			{ 0, 33 },
			
			{ 0, 67 },
			
			{ 0, 225 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 1 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 257 },
			
			{ 1, 186 },
			
			{ 0, 33 },
			
			{ 1, 33 },
			
			{ 0, 184 },
			
			{ 1, 4 },
			
			{ 4, 157 },
			
			{ 2, 74 },
			
			{ 3, 157 },
			
			{ 0, 184 },
			
			{ 1, 6 },
			
			{ 2, 7 },
			
			{ 1, 76 },
			
			{ 0, 191 },
			
			{ 2, 9 },
			
			{ 0, 77 },
			
			{ 1, 9 },
			
			{ 0, 184 },
			
			{ 1, 4 },
			
			{ 2, 157 },
			
			{ 0, 79 },
			
			{ 1, 157 },
			
			{ 2, 184 },
			
			{ 0, 80 },
			
			{ 1, 184 },
			
			{ 2, 157 },
			
			{ 0, 191 },
			
			{ 0, 191 },
			
			{ 1, 83 },
			
			{ 0, 184 },
			
			{ 0, 216 },
			
			{ 1, 85 },
			
			{ 2, 157 },
			
			{ 0, 179 },
			
			{ 0, 216 },
			
			{ 0, 159 },
			
			{ 0, 292 },
			
			{ 0, 293 },
			
			{ 0, 292 },
			
			{ 0, 293 },
			
			{ 0, 172 },
			
			{ 0, 294 },
			
			{ 0, 220 },
			
			{ 0, 158 },
			
			{ 0, 158 },
			
			{ 0, 297 },
			
			{ 1, 181 },
			
			{ 2, 179 },
			
			{ 0, 298 },
			
			{ 0, 33 },
			
			{ 0, 172 },
			
			{ 0, 299 },
			
			{ 0, 293 },
			
			{ 0, 179 },
			
			{ 0, 67 },
			
			{ 0, 161 },
			
			{ 1, 179 },
			
			{ 0, 179 },
			
			{ 0, 121 },
			
			{ 0, 67 },
			
			{ 0, 307 },
			
			{ 0, 308 },
			
			{ 0, 309 },
			
			{ 0, 310 },
			
			{ 0, 312 },
			
			{ 2, 138 },
			
			{ 0, 33 },
			
			{ 1, 172 },
			
			{ 0, 33 },
			
			{ 1, 179 },
			
			{ 2, 316 },
			
			{ 0, 33 },
			
			{ 1, 173 },
			
			{ 0, 145 },
			
			{ 0, 67 },
			
			{ 0, 33 },
			
			{ 2, 316 },
			
			{ 0, 33 },
			
			{ 1, 173 },
			
			{ 1, 125 },
			
			{ 0, 33 },
			
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
// #line 6 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
			m_StructPackFactor = (*(_cls42*) GetAstLocator (0)).m_Value;
		;
// #line 6263 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 47 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 return false; ;
// #line 6274 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 57 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 
				return OpenGlobalNamespace ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos) != NULL;
			;
// #line 6287 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 61 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 
				m_pModule->m_NamespaceMgr.CloseNamespace ();
			;
// #line 6300 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 68 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 
				return OpenTypeExtension ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos);
			;
// #line 6313 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 5:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 72 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 
				m_pModule->m_NamespaceMgr.CloseNamespace ();
			;
// #line 6326 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 6:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 87 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->m_CurrentAccessKind = (*(_cls32*) GetAstLocator (0)).m_AccessKind;
			;
// #line 6339 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 7:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 104 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			  
				__pSymbol->m_Arg.pName->AddName ((*GetTokenLocator (0)).m_Data.m_String); 
			;
// #line 6352 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 8:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 108 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			 
				__pSymbol->m_Arg.pName->AddName ((*GetTokenLocator (1)).m_Data.m_String);
			;
// #line 6365 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 9:
			{
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 146 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return Declare (&__pSymbol->m_Local.Declarator);
			;
// #line 6378 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 10:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 180 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return IsEmptyDeclarationTerminatorAllowed (__pSymbol->m_Arg.pTypeSpecifier);
			;
// #line 6391 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 11:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 184 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				return SetFunctionBody (&(*(_cls27*) GetAstLocator (0)).m_TokenList);
			;
// #line 6404 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 12:
			{
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 199 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls30*) GetAstLocator (0)).m_TypeSpecifier);
			;
// #line 6417 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 13:
			{
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 204 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				CType* pType = __pSymbol->m_Local.Declarator.CalcType (__pSymbol->m_Arg.pCountValue);
				if (!pType)
					return false;

				*__pSymbol->m_Arg.ppType = pType;
			;
// #line 6434 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 14:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
// #line 227 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_TypeList.InsertTail ((*(_cls2*) GetAstLocator (0)).m_pType);
			;
// #line 6447 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 15:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
// #line 231 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_TypeList.InsertTail ((*(_cls2*) GetAstLocator (1)).m_pType);
			;
// #line 6460 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 16:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
// #line 243 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_pType = (*(_cls2*) GetAstLocator (0)).m_pType;
			;
// #line 6473 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 17:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
// #line 247 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				(*__pAstNode).m_pType = (*(_cls22*) GetAstLocator (0)).m_Value.GetType ();
			;
// #line 6486 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 18:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 258 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				m_pAttributeBlock = m_pModule->m_AttributeMgr.CreateAttributeBlock ();
			;
// #line 6499 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 19:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 273 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			
				CAttribute* pAttribute = m_pAttributeBlock->CreateAttribute ((*GetTokenLocator (0)).m_Data.m_String, NULL);
				if (!pAttribute)
					return false;

				pAttribute->m_Pos = (*GetTokenLocator (0)).m_Pos;
			;
// #line 6516 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 20:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 14 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6529 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 21:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6542 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 22:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 23 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6555 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 23:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 28 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6568 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 24:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 32 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6581 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 25:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 37 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 6594 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 26:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 41 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6607 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 27:
			{
			CSymbolNode_arg_pass1* __pSymbol = (CSymbolNode_arg_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 6620 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 28:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 74 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_ExpressionValue = (*(_cls7*) GetAstLocator (0)).m_Value;
			;
// #line 6633 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 29:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 83 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_ExpressionValueList.TakeOver (&(*(_cls10*) GetAstLocator (0)).m_ValueList);
			;
// #line 6646 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 30:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
// #line 115 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls7*) GetAstLocator (0)).m_Value);
			;
// #line 6659 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 31:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
// #line 119 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls7*) GetAstLocator (1)).m_Value);
			;
// #line 6672 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 32:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
// #line 135 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls7*) GetAstLocator (0)).m_Value);
			;
// #line 6685 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 33:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
// #line 139 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls8*) GetAstLocator (1)).m_Value);
			;
// #line 6698 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 34:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
// #line 143 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail (CValue ());
			;
// #line 6711 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 35:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
// #line 147 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls8*) GetAstLocator (0)).m_Value);
			;
// #line 6724 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 36:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
// #line 161 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				if ((*__pAstNode).m_Value.GetValueKind () != EValue_Const)
				{
					err::SetFormatStringError ("not a constant expression");
					return false;
				}
			;
// #line 6741 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 37:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 177 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
									
				if ((*(_cls7*) GetAstLocator (0)).m_Value.GetValueKind () != EValue_Const || 
					!((*(_cls7*) GetAstLocator (0)).m_Value.GetType ()->GetTypeKindFlags () & ETypeKindFlag_Integer))
				{
					err::SetFormatStringError ("not a constant integer expression");
					return false;
				}

				bool Result = m_pModule->m_OperatorMgr.CastOperator (&(*(_cls7*) GetAstLocator (0)).m_Value, EType_Int32);
				if (!Result)
					return false;

				(*__pAstNode).m_Value = (*(_cls7*) GetAstLocator (0)).m_Value.GetInt32 ();
			;
// #line 6765 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 38:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
// #line 206 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("cond_then");
					(*__pAstNode).m_pElseBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("cond_else");
					(*__pAstNode).m_pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("cond_phi");			
					m_pModule->m_ControlFlowMgr.ConditionalJump (*__pSymbol->m_Arg.pValue, (*__pAstNode).m_pThenBlock, (*__pAstNode).m_pElseBlock);
				;
// #line 6781 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 39:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
// #line 214 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock ((*__pAstNode).m_pElseBlock); // might have changed				
				;
// #line 6794 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 40:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
// #line 219 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return m_pModule->m_OperatorMgr.ConditionalOperator (
						(*__pAstNode).m_TrueValue,
						(*__pAstNode).m_FalseValue,
						(*__pAstNode).m_pThenBlock,
						(*__pAstNode).m_pPhiBlock, 
						__pSymbol->m_Arg.pValue
						);
				;
// #line 6813 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 41:
			{
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 238 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6826 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 42:
			{
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 251 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6839 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 43:
			{
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 264 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6852 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 44:
			{
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 277 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6865 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 45:
			{
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 290 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6878 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 46:
			{
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 303 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls14*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6891 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 47:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
// #line 315 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Eq;
		;
// #line 6904 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 48:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
// #line 319 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ne;
		;
// #line 6917 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 49:
			{
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 331 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls15*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6930 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 50:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 343 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Lt;
			;
// #line 6943 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 51:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 347 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Gt;
			;
// #line 6956 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 52:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 351 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Le;
			;
// #line 6969 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 53:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 355 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Ge;
			;
// #line 6982 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 54:
			{
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 367 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6995 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 55:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
// #line 379 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Shl;
			;
// #line 7008 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 56:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
// #line 383 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Shr;
			;
// #line 7021 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 57:
			{
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 395 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7034 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 58:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 407 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Add;
			;
// #line 7047 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 59:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 411 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Sub;
			;
// #line 7060 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 60:
			{
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 423 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7073 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 61:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 435 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Mul;
			;
// #line 7086 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 62:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 439 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Div;
			;
// #line 7099 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 63:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 443 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Mod;
			;
// #line 7112 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 64:
			{
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 455 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7125 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 65:
			{
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 476 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls19*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7138 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 66:
			{
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 480 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				m_CurlyInitializerTargetValue = *__pSymbol->m_Arg.pValue;
			;
// #line 7151 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 67:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 492 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_Assign;
			;
// #line 7164 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 68:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 496 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_RefAssign;
			;
// #line 7177 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 69:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 500 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_AddAssign;
			;
// #line 7190 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 70:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 504 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_SubAssign;
			;
// #line 7203 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 71:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 508 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_MulAssign;
			;
// #line 7216 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 72:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 512 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_DivAssign;
			;
// #line 7229 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 73:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 516 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ModAssign;
			;
// #line 7242 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 74:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 520 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ShlAssign;
			;
// #line 7255 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 75:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 524 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_ShrAssign;
			;
// #line 7268 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 76:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 528 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_AndAssign;
			;
// #line 7281 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 77:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 532 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_XorAssign;
			;
// #line 7294 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 78:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 536 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_OpKind = EBinOp_OrAssign;
			;
// #line 7307 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 79:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 553 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Plus, __pSymbol->m_Arg.pValue);
			;
// #line 7320 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 80:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 557 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Minus, __pSymbol->m_Arg.pValue);
			;
// #line 7333 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 81:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 561 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
			;
// #line 7346 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 82:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 565 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, __pSymbol->m_Arg.pValue);
			;
// #line 7359 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 83:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 569 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, __pSymbol->m_Arg.pValue);
			;
// #line 7372 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 84:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 573 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
			;
// #line 7385 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 85:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 577 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
			;
// #line 7398 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 86:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 581 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
			;
// #line 7411 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 87:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 585 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstSizeT ((*(_cls4*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? 
					((CDataPtrType*) (*(_cls4*) GetAstLocator (0)).m_pType)->GetTargetType ()->GetSize () : 
					(*(_cls4*) GetAstLocator (0)).m_pType->GetSize ()
					);
			;
// #line 7427 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 88:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 592 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetType ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 7440 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 89:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 597 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
				return m_pModule->m_OperatorMgr.CastOperator (StorageKind, __pSymbol->m_Arg.pValue, (*(_cls2*) GetAstLocator (1)).m_pType);
			;
// #line 7454 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 90:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 602 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
				return m_pModule->m_OperatorMgr.NewOperator (
					StorageKind, 
					(*(_cls20*) GetAstLocator (1)).m_pType, 
					(*(_cls20*) GetAstLocator (1)).m_ElementCountValue, 
					&(*(_cls20*) GetAstLocator (1)).m_ArgList, 
					__pSymbol->m_Arg.pValue
					);
			;
// #line 7474 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 91:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 614 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				bool Result = m_pModule->m_OperatorMgr.DeleteOperator (*__pSymbol->m_Arg.pValue);
				if (!Result)
					return false;

				__pSymbol->m_Arg.pValue->SetVoid ();
			;
// #line 7491 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 92:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 633 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
				(*__pAstNode).m_ArgList.TakeOver (&(*(_cls10*) GetAstLocator (1)).m_ValueList);
			;
// #line 7505 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 93:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 674 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.CallOperator (__pSymbol->m_Arg.pValue, &(*(_cls10*) GetAstLocator (0)).m_ValueList);
			;
// #line 7518 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 94:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 678 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.ClosureOperator (__pSymbol->m_Arg.pValue, &(*(_cls10*) GetAstLocator (0)).m_ValueList);
			;
// #line 7531 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 95:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 682 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls7*) GetAstLocator (0)).m_Value);
			;
// #line 7544 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 96:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 686 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
			;
// #line 7557 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 97:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 690 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
			;
// #line 7570 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 98:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 694 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return IsValidLocator ((*GetTokenLocator (0))) ? m_pModule->m_OperatorMgr.Weaken (__pSymbol->m_Arg.pValue) : true;
			;
// #line 7583 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 99:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 698 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
			;
// #line 7596 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 100:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 706 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.MemberOperator (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 7609 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 101:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 710 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyOnChange (__pSymbol->m_Arg.pValue);
			;
// #line 7622 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 102:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 714 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyPropValue (__pSymbol->m_Arg.pValue);
			;
// #line 7635 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 103:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 718 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyGetter (__pSymbol->m_Arg.pValue);
			;
// #line 7648 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 104:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 722 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertySetter (__pSymbol->m_Arg.pValue);
			;
// #line 7661 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 105:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 734 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return LookupIdentifier ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
			;
// #line 7674 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 106:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 738 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return GetThisValue (__pSymbol->m_Arg.pValue);
			;
// #line 7687 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 107:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 742 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return GetOnChange (__pSymbol->m_Arg.pValue);
			;
// #line 7700 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 108:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 746 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return GetPropValue (__pSymbol->m_Arg.pValue);
			;
// #line 7713 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 109:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 750 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstInt64_u ((*GetTokenLocator (0)).m_Data.m_Int64_u);
			;
// #line 7726 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 110:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 754 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstDouble ((*GetTokenLocator (0)).m_Data.m_Double);
			;
// #line 7739 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 111:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 758 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetLiteral ((*(_cls21*) GetAstLocator (0)).m_String);
			;
// #line 7752 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 112:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 762 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstBool (true);
			;
// #line 7765 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 113:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 766 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetConstBool (false);
			;
// #line 7778 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 114:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 770 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				__pSymbol->m_Arg.pValue->SetNull ();
			;
// #line 7791 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 115:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 782 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_String = (*GetTokenLocator (0)).m_Data.m_String;
			;
// #line 7804 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 116:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
// #line 786 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_String.Append ((*GetTokenLocator (1)).m_Data.m_String);
			;
// #line 7817 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 117:
			{
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 809 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				if (!__pSymbol->m_Local.Initializer.m_Count)
				{
					err::SetFormatStringError ("empty curly initializer");
					return false;
				}
			;
// #line 7834 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 118:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 824 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			 return true; ;
// #line 7845 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 119:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 826 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerNamedItem (__pSymbol->m_Arg.pInitializer, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 7858 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 120:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 831 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
					return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, __pSymbol->m_Arg.pInitializer->m_MemberValue, (*(_cls7*) GetAstLocator (1)).m_Value);
				;
// #line 7871 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 121:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 835 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerIndexedItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 7884 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 122:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 839 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, __pSymbol->m_Arg.pInitializer->m_MemberValue, (*(_cls7*) GetAstLocator (0)).m_Value);
			;
// #line 7897 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 123:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 842 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return PrepareCurlyInitializerIndexedItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 7910 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 124:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 847 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			
				return SkipCurlyInitializerItem (__pSymbol->m_Arg.pInitializer);
			;
// #line 7923 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 125:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 6 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				m_ExpressionValue = (*(_cls22*) GetAstLocator (0)).m_Value;
			;
// #line 7936 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 126:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
// #line 36 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls22*) GetAstLocator (0)).m_Value);
			;
// #line 7949 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 127:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
// #line 40 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls22*) GetAstLocator (1)).m_Value);
			;
// #line 7962 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 128:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 56 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls22*) GetAstLocator (0)).m_Value);
			;
// #line 7975 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 129:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 60 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (1)).m_Value);
			;
// #line 7988 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 130:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 64 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail (CValue ());
			;
// #line 8001 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 131:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 68 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls23*) GetAstLocator (0)).m_Value);
			;
// #line 8014 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 132:
			{
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 85 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetConditionalOperatorResultType (__pSymbol->m_Local.TrueValue, __pSymbol->m_Local.FalseValue, __pSymbol->m_Arg.pValue);
			;
// #line 8027 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 133:
			{
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 98 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8040 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 134:
			{
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 111 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8053 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 135:
			{
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 124 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8066 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 136:
			{
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 137 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8079 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 137:
			{
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 150 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8092 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 138:
			{
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 163 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls14*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8105 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 139:
			{
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 176 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls15*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8118 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 140:
			{
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 189 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8131 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 141:
			{
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 202 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8144 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 142:
			{
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 215 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8157 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 143:
			{
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 228 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8170 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 144:
			{
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 241 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					if (IsAutoEv ())
						m_AutoEvBindSiteCount = 0; // reset on assignment
				;
// #line 8184 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 145:
			{
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 246 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
					return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls19*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
				;
// #line 8197 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 146:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 264 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Plus, __pSymbol->m_Arg.pValue);
			;
// #line 8210 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 147:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 268 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Minus, __pSymbol->m_Arg.pValue);
			;
// #line 8223 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 148:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 272 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
			;
// #line 8236 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 149:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 276 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Addr, __pSymbol->m_Arg.pValue);
			;
// #line 8249 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 150:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 280 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Indir, __pSymbol->m_Arg.pValue);
			;
// #line 8262 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 151:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 284 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
			;
// #line 8275 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 152:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 288 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
			;
// #line 8288 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 153:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
			;
// #line 8301 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 154:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 296 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetConstSizeT ((*(_cls4*) GetAstLocator (0)).m_pType->GetSize ());
			;
// #line 8314 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 155:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 300 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType ((*(_cls4*) GetAstLocator (0)).m_pType);
			;
// #line 8327 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 156:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 305 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType ((*(_cls2*) GetAstLocator (0)).m_pType);
			;
// #line 8340 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 157:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 309 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
				return NewOperator_s (StorageKind, (*(_cls26*) GetAstLocator (1)).m_pType, __pSymbol->m_Arg.pValue);			
			;
// #line 8354 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 158:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 314 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetVoid ();
			;
// #line 8367 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 159:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 325 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_pType = (*(_cls2*) GetAstLocator (0)).m_pType;
			;
// #line 8380 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 160:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
// #line 330 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
			;
// #line 8393 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 161:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 347 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetCallOperatorResultType (__pSymbol->m_Arg.pValue, &(*(_cls25*) GetAstLocator (0)).m_ValueList);
			;
// #line 8406 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 162:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 351 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetClosureOperatorResultType (__pSymbol->m_Arg.pValue, &(*(_cls25*) GetAstLocator (0)).m_ValueList);
			;
// #line 8419 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 163:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 355 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls22*) GetAstLocator (0)).m_Value);
			;
// #line 8432 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 164:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 359 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
			;
// #line 8445 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 165:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 363 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
			;
// #line 8458 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 166:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 367 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return IsValidLocator ((*GetTokenLocator (0))) ? m_pModule->m_OperatorMgr.GetWeakenedType (__pSymbol->m_Arg.pValue) : true;
			;
// #line 8471 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 167:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 371 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
			;
// #line 8484 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 168:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 379 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetMemberOperatorResultType (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 8497 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 169:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 383 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyOnChangeType (__pSymbol->m_Arg.pValue);
			;
// #line 8510 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 170:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 387 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyPropValueType (__pSymbol->m_Arg.pValue);
			;
// #line 8523 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 171:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 391 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertyGetterType (__pSymbol->m_Arg.pValue);
			;
// #line 8536 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 172:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 395 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetPropertySetterType (__pSymbol->m_Arg.pValue);
			;
// #line 8549 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 173:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 407 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return LookupIdentifierType ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
			;
// #line 8562 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 174:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 411 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return GetThisValueType (__pSymbol->m_Arg.pValue);
			;
// #line 8575 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 175:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 415 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return GetOnChangeType (__pSymbol->m_Arg.pValue);
			;
// #line 8588 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 176:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 419 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				return GetPropValueType (__pSymbol->m_Arg.pValue);
			;
// #line 8601 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 177:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 423 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (GetInt64TypeKind ((*GetTokenLocator (0)).m_Data.m_Int64));
			;
// #line 8614 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 178:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 427 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Double);
			;
// #line 8627 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 179:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 431 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (m_pModule->m_TypeMgr.GetLiteralType ((*(_cls21*) GetAstLocator (0)).m_String.GetLength ()));
			;
// #line 8640 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 180:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 435 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Bool);
			;
// #line 8653 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 181:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 439 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetType (EType_Bool);
			;
// #line 8666 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 182:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 443 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			
				__pSymbol->m_Arg.pValue->SetNull ();
			;
// #line 8679 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 183:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 14 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 8692 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 184:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
			;
// #line 8705 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 185:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 28 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
			;
// #line 8718 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 186:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 38 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
			;
// #line 8731 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 187:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 43 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
			;
// #line 8744 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 188:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 51 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
			;
// #line 8757 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 189:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 55 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeBaseTypeMemberConstructBlock ();
			;
// #line 8770 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 190:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 60 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
			;
// #line 8783 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 191:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 72 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return CallBaseTypeMemberConstructor ((*(_cls28*) GetAstLocator (0)).m_Name, &(*(_cls10*) GetAstLocator (1)).m_ValueList);
			;
// #line 8796 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 192:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
// #line 88 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			  
				(*__pAstNode).m_Name.AddName ((*GetTokenLocator (0)).m_Data.m_String); 
			;
// #line 8809 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 193:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
// #line 92 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			 
				(*__pAstNode).m_Name.AddName ((*GetTokenLocator (1)).m_Data.m_String);
			;
// #line 8822 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 194:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 128 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.IfStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls7*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
			;
// #line 8835 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 195:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 134 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.IfStmt_Else (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (2)).m_Pos);
				;
// #line 8848 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 196:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 139 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
				m_pModule->m_ControlFlowMgr.IfStmt_Follow (&__pSymbol->m_Local.Stmt, IsValidLocator ((*GetAstLocator (3))) ? (*GetAstLocator (3)).m_LastToken.m_Pos : (*GetAstLocator (4)).m_LastToken.m_Pos);
			;
// #line 8861 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 197:
			{
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 154 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.SwitchStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls7*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
			;
// #line 8874 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 198:
			{
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 158 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.SwitchStmt_Follow (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (2)).m_Pos);
			;
// #line 8887 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 199:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 165 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.SwitchStmt_Case (__pSymbol->m_Arg.pStmt, (*(_cls12*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos, (*GetTokenLocator (2)).m_Pos);
			;
// #line 8900 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 200:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 169 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.SwitchStmt_Default (__pSymbol->m_Arg.pStmt, (*GetTokenLocator (0)).m_Pos, (*GetTokenLocator (1)).m_Pos);
			;
// #line 8913 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 201:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 185 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.WhileStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls7*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
			;
// #line 8926 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 202:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 189 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.WhileStmt_Follow (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
			;
// #line 8939 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 203:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 204 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.DoStmt_PreBody (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
			;
// #line 8952 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 204:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 208 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.DoStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (1)).m_LastToken.m_Pos);
			;
// #line 8965 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 205:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 212 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.DoStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls7*) GetAstLocator (2)).m_Value);
			;
// #line 8978 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 206:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 227 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PreInit (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
			;
// #line 8991 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 207:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 232 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PreCondition (&__pSymbol->m_Local.Stmt);
				;
// #line 9004 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 208:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 236 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					return m_pModule->m_ControlFlowMgr.ForStmt_PostCondition (&__pSymbol->m_Local.Stmt, (*(_cls7*) GetAstLocator (1)).m_Value);
				;
// #line 9017 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 209:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 241 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PreLoop (&__pSymbol->m_Local.Stmt);
				;
// #line 9030 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 210:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 245 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
					m_pModule->m_ControlFlowMgr.ForStmt_PostLoop (&__pSymbol->m_Local.Stmt);
				;
// #line 9043 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 211:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 249 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PreBody (&__pSymbol->m_Local.Stmt);
			;
// #line 9056 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 212:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 253 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
			;
// #line 9069 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 213:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 260 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Break (IsValidLocator ((*(_cls12*) GetAstLocator (0))) ? (*(_cls12*) GetAstLocator (0)).m_Value : 1);
			;
// #line 9082 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 214:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 267 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Continue (IsValidLocator ((*(_cls12*) GetAstLocator (0))) ? (*(_cls12*) GetAstLocator (0)).m_Value : 1);
			;
// #line 9095 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 215:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 274 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.Return (IsValidLocator ((*(_cls7*) GetAstLocator (0))) ? (*(_cls7*) GetAstLocator (0)).m_Value : CValue (), IsValidLocator ((*GetTokenLocator (1))));
			;
// #line 9108 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 216:
			{
			CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 285 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				EStorage StorageKind = IsValidLocator ((*(_cls31*) GetAstLocator (0))) ? (*(_cls31*) GetAstLocator (0)).m_StorageKind : EStorage_Static;
				return 
					m_pModule->m_ControlFlowMgr.OnceStmt_Create (&__pSymbol->m_Local.Stmt, StorageKind) &&				
					m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (1)).m_Pos);
			;
// #line 9124 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 217:
			{
			CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.OnceStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
			;
// #line 9137 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 218:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 302 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
			return FinalizeAutoEv ();
		;
// #line 9150 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 219:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 308 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			 return true; ;
// #line 9161 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 220:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 316 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeAutoEvOnChangeClause ();
			;
// #line 9174 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 221:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 320 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return FinalizeAutoEvOnChangeStmt ();
			;
// #line 9187 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 222:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 327 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			
				return AutoEvExpressionStmt (&(*(_cls5*) GetAstLocator (0)).m_TokenList);
			;
// #line 9200 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 223:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 84 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			 return true; ;
// #line 9211 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 224:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 92 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			
				return CountAutoEvBindSites ();
			;
// #line 9224 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 225:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 100 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
			
				return CountAutoEvBindSites ();
			;
// #line 9237 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 226:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 22 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return SetStorageKind ((*(_cls31*) GetAstLocator (0)).m_StorageKind);
			;
// #line 9250 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 227:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 26 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return SetAccessKind ((*(_cls32*) GetAstLocator (0)).m_AccessKind);
			;
// #line 9263 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 228:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 30 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls33*) GetAstLocator (0)).m_pType);
			;
// #line 9276 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 229:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 34 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls35*) GetAstLocator (0)).m_Modifier);
			;
// #line 9289 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 230:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 59 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls33*) GetAstLocator (0)).m_pType);
			;
// #line 9302 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 231:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 63 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls35*) GetAstLocator (0)).m_Modifier);
			;
// #line 9315 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 232:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 76 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Typedef;
			;
// #line 9328 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 233:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 80 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Alias;
			;
// #line 9341 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 234:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 84 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Static;
			;
// #line 9354 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 235:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 88 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
						
				(*__pAstNode).m_StorageKind = EStorage_Thread;
			;
// #line 9367 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 236:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 92 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Stack;
			;
// #line 9380 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 237:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 96 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Heap;
			;
// #line 9393 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 238:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 100 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_UHeap;
			;
// #line 9406 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 239:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 104 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Abstract;
			;
// #line 9419 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 240:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 108 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Virtual;
			;
// #line 9432 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 241:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 112 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Override;
			;
// #line 9445 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 242:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 116 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Mutable;
			;
// #line 9458 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 243:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 120 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_StorageKind = EStorage_Nullable;
			;
// #line 9471 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 244:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
// #line 133 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_AccessKind = EAccess_Public;
			;
// #line 9484 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 245:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
// #line 137 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_AccessKind = EAccess_Protected;
			;
// #line 9497 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 246:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 150 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
			;
// #line 9510 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 247:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 154 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				err::SetStringError ("'variant' type specifier is not yet supported");
				return false; 
			;
// #line 9524 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 248:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 159 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectClass);
			;
// #line 9537 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 249:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 163 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetBoxClassType ((*(_cls2*) GetAstLocator (0)).m_pType);
			;
// #line 9550 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 250:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 167 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Bool);
			;
// #line 9563 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 251:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 171 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int8);
			;
// #line 9576 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 252:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 175 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int16);
			;
// #line 9589 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 253:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 179 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32);
			;
// #line 9602 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 254:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 183 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64);
			;
// #line 9615 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 255:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 187 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int);
			;
// #line 9628 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 256:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 191 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
			;
// #line 9641 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 257:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 195 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char);
			;
// #line 9654 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 258:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 199 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Float);
			;
// #line 9667 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 259:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 203 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Double);
			;
// #line 9680 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 260:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 207 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				err::SetStringError ("'auto' type specifier is not yet supported");
				return false; 
			;
// #line 9694 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 261:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 213 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls46*) GetAstLocator (0)).m_pType;
			;
// #line 9707 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 262:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 218 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls34*) GetAstLocator (0)).m_pType;
			;
// #line 9720 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 263:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 226 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				if (IsTypeSpecified ())
					return false;
			;
// #line 9734 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 264:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 231 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return m_Stage == EStage_Pass1 || FindType ((*(_cls1*) GetAstLocator (0)).m_Name) != NULL;
			;
// #line 9747 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 265:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 235 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				return false;
			;
// #line 9760 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 266:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
// #line 247 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				CType* pType = FindType ((*(_cls1*) GetAstLocator (0)).m_Name);
				if (!pType)
				{
					err::SetFormatStringError ("'%s' is not a type", (*(_cls1*) GetAstLocator (0)).m_Name.GetFullName ().cc ());
					return false;
				}

				(*__pAstNode).m_pType = pType;
			;
// #line 9780 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 267:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 267 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Unsigned;
			;
// #line 9793 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 268:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 271 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_BigEndian;
			;
// #line 9806 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 269:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 275 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Const;
			;
// #line 9819 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 270:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 279 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_ReadOnly;
			;
// #line 9832 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 271:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 283 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Volatile;
			;
// #line 9845 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 272:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 287 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Weak;
			;
// #line 9858 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 273:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 291 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Thin;
			;
// #line 9871 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 274:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 295 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Unsafe;
			;
// #line 9884 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 275:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 299 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Cdecl;
			;
// #line 9897 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 276:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 303 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Stdcall;
			;
// #line 9910 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 277:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 307 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Function;
			;
// #line 9923 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 278:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 311 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Property;
			;
// #line 9936 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 279:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 315 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Bindable;
			;
// #line 9949 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 280:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 319 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_AutoGet;
			;
// #line 9962 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 281:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 323 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Indexed;
			;
// #line 9975 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 282:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 327 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Multicast;
			;
// #line 9988 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 283:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 331 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_Event;
			;
// #line 10001 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 284:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 335 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
				(*__pAstNode).m_Modifier = ETypeModifier_AutoEv;
			;
// #line 10014 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 285:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 10 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*(_cls36*) GetAstLocator (0)).m_Declarator.m_Constructor.TakeOver (&(*(_cls6*) GetAstLocator (1)).m_TokenList);
			;
// #line 10027 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 286:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 15 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*(_cls36*) GetAstLocator (0)).m_Declarator.m_Initializer.TakeOver (&(*(_cls5*) GetAstLocator (2)).m_TokenList);
			;
// #line 10040 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 287:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			 
				return Declare (&(*(_cls36*) GetAstLocator (0)).m_Declarator);
			;
// #line 10053 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 288:
			{
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 45 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->m_Pos = (*GetAstLocator (0)).m_FirstToken.m_Pos;
			;
// #line 10066 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 289:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 53 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddName ((*GetTokenLocator (0)).m_Data.m_String);
			;
// #line 10079 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 290:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 57 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Getter);
			;
// #line 10092 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 291:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 61 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Setter);
			;
// #line 10105 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 292:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 65 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_PreConstructor);
			;
// #line 10118 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 293:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 69 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Constructor);
			;
// #line 10131 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 294:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 73 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Destructor);
			;
// #line 10144 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 295:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 77 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticConstructor);
			;
// #line 10157 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 296:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 81 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticDestructor);
			;
// #line 10170 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 297:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 85 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_CallOperator);
			;
// #line 10183 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 298:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 89 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddCastOperator ((*(_cls2*) GetAstLocator (0)).m_pType);
			;
// #line 10196 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 299:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 93 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddUnaryBinaryOperator ((*(_cls37*) GetAstLocator (0)).m_UnOpKind, (*(_cls37*) GetAstLocator (0)).m_BinOpKind);
			;
// #line 10209 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 300:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 97 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->SetPropValue ();
			;
// #line 10222 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 301:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 116 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_BwNot;
			;
// #line 10235 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 302:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 120 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_LogNot;
			;
// #line 10248 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 303:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 124 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PreInc;
			;
// #line 10261 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 304:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 128 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PreDec;
			;
// #line 10274 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 305:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 132 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PostInc;
			;
// #line 10287 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 306:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 136 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_PostDec;
			;
// #line 10300 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 307:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 140 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Ptr;
			;
// #line 10313 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 308:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 144 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Plus;
				(*__pAstNode).m_BinOpKind = EBinOp_Add;
			;
// #line 10327 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 309:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 149 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Minus;
				(*__pAstNode).m_BinOpKind = EBinOp_Sub;
			;
// #line 10341 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 310:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 154 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Indir;
				(*__pAstNode).m_BinOpKind = EBinOp_Mul;
			;
// #line 10355 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 311:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 159 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Div;
			;
// #line 10368 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 312:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 163 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Mod;
			;
// #line 10381 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 313:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 167 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Shl;
			;
// #line 10394 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 314:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 171 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Shr;
			;
// #line 10407 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 315:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 175 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_UnOpKind = EUnOp_Addr;
				(*__pAstNode).m_BinOpKind = EBinOp_BwAnd;
			;
// #line 10421 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 316:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 180 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_BwXor;
			;
// #line 10434 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 317:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 184 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_BwOr;
			;
// #line 10447 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 318:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 188 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Eq;
			;
// #line 10460 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 319:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 192 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Ne;
			;
// #line 10473 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 320:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 196 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Lt;
			;
// #line 10486 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 321:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 200 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Le;
			;
// #line 10499 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 322:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 204 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Gt;
			;
// #line 10512 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 323:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 208 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Ge;
			;
// #line 10525 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 324:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 212 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Assign;
			;
// #line 10538 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 325:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 216 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_RefAssign;
			;
// #line 10551 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 326:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 220 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AddAssign;
			;
// #line 10564 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 327:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 224 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_SubAssign;
			;
// #line 10577 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 328:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 228 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_MulAssign;
			;
// #line 10590 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 329:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 232 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_DivAssign;
			;
// #line 10603 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 330:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 236 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ModAssign;
			;
// #line 10616 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 331:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 240 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ShlAssign;
			;
// #line 10629 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 332:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 244 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_ShrAssign;
			;
// #line 10642 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 333:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 248 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AndAssign;
			;
// #line 10655 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 334:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 252 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_XorAssign;
			;
// #line 10668 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 335:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 256 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_OrAssign;
			;
// #line 10681 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 336:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 260 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_AtAssign;
			;
// #line 10694 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 337:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 264 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_LogAnd;
			;
// #line 10707 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 338:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 268 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_LogOr;
			;
// #line 10720 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 339:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 272 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_Idx;
			;
// #line 10733 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 340:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 276 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				(*__pAstNode).m_BinOpKind = EBinOp_At;
			;
// #line 10746 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 341:
			{
			CSymbolNode_declarator_prefix* __pSymbol = (CSymbolNode_declarator_prefix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 288 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddPointerPrefix ();
			;
// #line 10759 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 342:
			{
			CSymbolNode_declarator_prefix* __pSymbol = (CSymbolNode_declarator_prefix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->SetTypeModifier ((*(_cls35*) GetAstLocator (0)).m_Modifier);
			;
// #line 10772 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 343:
			{
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 304 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			 return false; ;
// #line 10783 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 344:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 312 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix (&(*(_cls5*) GetAstLocator (0)).m_TokenList);
			;
// #line 10796 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 345:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 316 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 10809 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 346:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 320 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pDeclarator->AddArraySuffix (-1);
			;
// #line 10822 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 347:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 367 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier);
			;
// #line 10835 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 348:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 374 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.m_Initializer.TakeOver (&(*(_cls5*) GetAstLocator (1)).m_TokenList);
			;
// #line 10848 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 349:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 378 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return CreateFormalArg (__pSymbol->m_Arg.pFunctionSuffix, &__pSymbol->m_Local.Declarator) != NULL;
			;
// #line 10861 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 350:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 397 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Local.Declarator.m_Initializer.TakeOver (&(*(_cls5*) GetAstLocator (0)).m_TokenList);
			;
// #line 10874 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 351:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 401 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return CreateFormalArg (__pSymbol->m_Arg.pFunctionSuffix, &__pSymbol->m_Local.Declarator) != NULL;
			;
// #line 10887 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 352:
			{
			CSymbolNode_function_formal_argument_ellipsis* __pSymbol = (CSymbolNode_function_formal_argument_ellipsis*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 408 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_VarArg;
				if (IsValidLocator ((*GetTokenLocator (0))))
					__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_UnsafeVarArg;
			;
// #line 10902 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 353:
			{
			CSymbolNode_bitfield_suffix* __pSymbol = (CSymbolNode_bitfield_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 419 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->AddBitFieldSuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
			;
// #line 10915 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 354:
			{
			CSymbolNode_post_declarator_modifier* __pSymbol = (CSymbolNode_post_declarator_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 428 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			
				return __pSymbol->m_Arg.pDeclarator->SetPostDeclaratorModifier (EPostDeclaratorModifier_Const);
			;
// #line 10928 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 355:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 11 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls39*) GetAstLocator (0)).m_pType;
			;
// #line 10941 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 356:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 15 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls40*) GetAstLocator (0)).m_pType;
			;
// #line 10954 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 357:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 19 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls43*) GetAstLocator (0)).m_pType;
			;
// #line 10967 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 358:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 23 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls44*) GetAstLocator (0)).m_pType;
			;
// #line 10980 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 359:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 28 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = (*(_cls45*) GetAstLocator (0)).m_pProperty->GetType ();
			;
// #line 10993 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 360:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 50 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateEnumType (
					IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
					IsValidLocator ((*(_cls2*) GetAstLocator (1))) ? (*(_cls2*) GetAstLocator (1)).m_pType : NULL,
					IsValidLocator ((*GetTokenLocator (2))) ? EEnumTypeFlag_Exposed : 0
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11013 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 361:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 67 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
			;
// #line 11026 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 362:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 72 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pModule->m_NamespaceMgr.CloseNamespace ();	
			;
// #line 11039 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 363:
			{
			CSymbolNode_enum_member* __pSymbol = (CSymbolNode_enum_member*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 85 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				CEnumConst* pMember = __pSymbol->m_Arg.pType->CreateConst (
					(*GetTokenLocator (0)).m_Data.m_String,
					IsValidLocator ((*(_cls5*) GetAstLocator (1))) ? &(*(_cls5*) GetAstLocator (1)).m_TokenList : NULL
					);

				if (!pMember)
					return false;

				pMember->m_Pos = (*GetTokenLocator (0)).m_Pos;
			;
// #line 11060 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 364:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
// #line 112 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateStructType (
					IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
					IsValidLocator ((*(_cls3*) GetAstLocator (1))) ? &(*(_cls3*) GetAstLocator (1)).m_TypeList : NULL,
					IsValidLocator ((*(_cls41*) GetAstLocator (2))) ? (*(_cls41*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11080 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 365:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 133 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_PackFactor = (*(_cls42*) GetAstLocator (0)).m_Value;
			;
// #line 11093 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 366:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
// #line 144 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_Value = (*GetTokenLocator (0)).m_Data.m_Integer;
			;
// #line 11106 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 367:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
// #line 148 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_Value = m_DefaultStructPackFactor;
			;
// #line 11119 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 368:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls43* __pAstNode = (_cls43*) __pSymbol->m_pAstNode;
// #line 164 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateUnionType (IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString ());
				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11134 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 369:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
// #line 185 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = CreateClassType (
					(*GetTokenLocator (0)).m_Data.m_String,
					IsValidLocator ((*(_cls3*) GetAstLocator (1))) ? &(*(_cls3*) GetAstLocator (1)).m_TypeList : NULL,
					IsValidLocator ((*(_cls41*) GetAstLocator (2))) ? (*(_cls41*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
					);

				if (!(*__pAstNode).m_pType)
					return false;
			;
// #line 11154 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 370:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
// #line 216 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pProperty = CreateProperty (
					(*GetTokenLocator (0)).m_Data.m_String,
					(*GetTokenLocator (0)).m_Pos
					);

				if (!(*__pAstNode).m_pProperty)
					return false;

				return IsValidLocator ((*GetTokenLocator (1))) ? (*__pAstNode).m_pProperty->CreateOnChange () : true;
			;
// #line 11175 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 371:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
// #line 228 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pLastDeclaredItem = (*__pAstNode).m_pProperty;
				return (*__pAstNode).m_pProperty->CalcType () != NULL; 
			;
// #line 11189 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 372:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
// #line 248 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				__pSymbol->m_Local.pTemplate = CreatePropertyTemplate ();
			;
// #line 11202 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 373:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
// #line 252 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType = __pSymbol->m_Local.pTemplate->CalcType ();
				return (*__pAstNode).m_pType != NULL;
			;
// #line 11216 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 374:
			{
			CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 264 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pNamespace);
			;
// #line 11229 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}

			return true;
		
		case 375:
			{
			CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 269 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				m_pModule->m_NamespaceMgr.CloseNamespace ();
			;
// #line 11242 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
			
// #line 99 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pName = &(*__pAstNode).m_Name;
// #line 11270 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_declarator_list* __pTarget = (CSymbolNode_declarator_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 125 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier;
// #line 11283 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 126 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls29*) GetAstLocator (0)).m_TypeSpecifier;
// #line 11296 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 142 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11309 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 143 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11322 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 144 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11335 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 144 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11348 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 145 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11361 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 149 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = NULL;
// #line 11374 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier_rslv* __pSymbol = (CSymbolNode_item_declaration_no_specifier_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 163 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11387 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 202 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11400 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_type_name_impl* __pSymbol = (CSymbolNode_type_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 203 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11413 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_type_name_impl* __pTarget = (CSymbolNode_type_name_impl*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
// #line 218 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.ppType = &(*__pAstNode).m_pType;
// #line 11426 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 218 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pCountValue =  NULL;
// #line 11430 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
			
// #line 8 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 11443 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 17 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11456 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 26 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11469 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 35 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11482 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_arg_pass1* __pTarget = (CSymbolNode_arg_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
			
// #line 55 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 11495 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_arg_pass1* __pTarget = (CSymbolNode_arg_pass1*) pSymbol;
			CSymbolNode_arg_pass1* __pSymbol = (CSymbolNode_arg_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 61 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11508 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
			
// #line 96 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 11521 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
			
// #line 105 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 11534 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
			
// #line 160 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 11547 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_logical_or_expr* __pTarget = (CSymbolNode_logical_or_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
			
// #line 204 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11560 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
			
// #line 213 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_TrueValue;
// #line 11573 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
			
// #line 218 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_FalseValue;
// #line 11586 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 236 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11599 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 237 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11612 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 249 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11625 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 250 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11638 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 262 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11651 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 263 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11664 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 275 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11677 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 276 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11690 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 288 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11703 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 289 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11716 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 301 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11729 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 302 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11742 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 329 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11755 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 330 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11768 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 365 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11781 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 366 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11794 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 41:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 393 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11807 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 42:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 394 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11820 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 43:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 421 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11833 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 44:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 422 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11846 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 45:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 453 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11859 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 46:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 454 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11872 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 47:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 462 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11885 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 48:
			{
			CSymbolNode_assignment_operator_expr* __pTarget = (CSymbolNode_assignment_operator_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 462 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11898 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 49:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 475 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11911 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 50:
			{
			CSymbolNode_postfix_expr* __pTarget = (CSymbolNode_postfix_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 542 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11924 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 51:
			{
			CSymbolNode_unary_operator_expr* __pTarget = (CSymbolNode_unary_operator_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 543 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11937 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 52:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 552 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11950 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 53:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 556 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11963 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 54:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 560 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11976 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 55:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 564 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11989 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 56:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 568 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12002 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 57:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 572 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12015 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 58:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 576 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12028 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 59:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 580 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12041 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 60:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 596 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12054 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 61:
			{
			CSymbolNode_new_operator_curly_initializer* __pTarget = (CSymbolNode_new_operator_curly_initializer*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 612 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12067 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 62:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 613 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12080 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 63:
			{
			CSymbolNode_type_name_impl* __pTarget = (CSymbolNode_type_name_impl*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
			
// #line 630 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.ppType = &(*__pAstNode).m_pType;
// #line 12093 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
// #line 630 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pCountValue =  &(*__pAstNode).m_ElementCountValue;
// #line 12097 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 64:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_cast_operator_rslv* __pSymbol = (CSymbolNode_cast_operator_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 659 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTypeSpecifier = &__pSymbol->m_Local.TypeSpecifier;
// #line 12110 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 65:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 663 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12123 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 66:
			{
			CSymbolNode_postfix_operator* __pTarget = (CSymbolNode_postfix_operator*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 664 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12136 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 67:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 693 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12149 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 68:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 701 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12162 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 69:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 773 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12175 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 70:
			{
			CSymbolNode_curly_initializer_item* __pTarget = (CSymbolNode_curly_initializer_item*) pSymbol;
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 808 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pInitializer = &__pSymbol->m_Local.Initializer;
// #line 12188 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 71:
			{
			CSymbolNode_curly_initializer_item* __pTarget = (CSymbolNode_curly_initializer_item*) pSymbol;
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 808 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pInitializer = &__pSymbol->m_Local.Initializer;
// #line 12201 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 72:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
			
// #line 17 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12214 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 73:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls23* __pAstNode = (_cls23*) __pSymbol->m_pAstNode;
			
// #line 26 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12227 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 74:
			{
			CSymbolNode_logical_or_expr_s* __pTarget = (CSymbolNode_logical_or_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 83 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12240 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 75:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 84 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.TrueValue;
// #line 12253 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 76:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 84 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.FalseValue;
// #line 12266 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 77:
			{
			CSymbolNode_logical_and_expr_s* __pTarget = (CSymbolNode_logical_and_expr_s*) pSymbol;
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 96 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12279 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 78:
			{
			CSymbolNode_logical_and_expr_s* __pTarget = (CSymbolNode_logical_and_expr_s*) pSymbol;
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 97 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12292 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 79:
			{
			CSymbolNode_inclusive_or_expr_s* __pTarget = (CSymbolNode_inclusive_or_expr_s*) pSymbol;
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 109 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12305 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 80:
			{
			CSymbolNode_inclusive_or_expr_s* __pTarget = (CSymbolNode_inclusive_or_expr_s*) pSymbol;
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 110 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12318 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 81:
			{
			CSymbolNode_exclusive_or_expr_s* __pTarget = (CSymbolNode_exclusive_or_expr_s*) pSymbol;
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 122 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12331 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 82:
			{
			CSymbolNode_exclusive_or_expr_s* __pTarget = (CSymbolNode_exclusive_or_expr_s*) pSymbol;
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 123 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12344 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 83:
			{
			CSymbolNode_and_expr_s* __pTarget = (CSymbolNode_and_expr_s*) pSymbol;
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 135 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12357 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 84:
			{
			CSymbolNode_and_expr_s* __pTarget = (CSymbolNode_and_expr_s*) pSymbol;
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 136 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12370 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 85:
			{
			CSymbolNode_equality_expr_s* __pTarget = (CSymbolNode_equality_expr_s*) pSymbol;
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 148 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12383 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 86:
			{
			CSymbolNode_equality_expr_s* __pTarget = (CSymbolNode_equality_expr_s*) pSymbol;
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 149 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12396 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 87:
			{
			CSymbolNode_relational_expr_s* __pTarget = (CSymbolNode_relational_expr_s*) pSymbol;
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 161 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12409 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 88:
			{
			CSymbolNode_relational_expr_s* __pTarget = (CSymbolNode_relational_expr_s*) pSymbol;
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 162 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12422 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 89:
			{
			CSymbolNode_shift_expr_s* __pTarget = (CSymbolNode_shift_expr_s*) pSymbol;
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 174 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12435 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 90:
			{
			CSymbolNode_shift_expr_s* __pTarget = (CSymbolNode_shift_expr_s*) pSymbol;
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 175 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12448 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 91:
			{
			CSymbolNode_additive_expr_s* __pTarget = (CSymbolNode_additive_expr_s*) pSymbol;
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 187 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12461 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 92:
			{
			CSymbolNode_additive_expr_s* __pTarget = (CSymbolNode_additive_expr_s*) pSymbol;
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 188 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12474 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 93:
			{
			CSymbolNode_multiplicative_expr_s* __pTarget = (CSymbolNode_multiplicative_expr_s*) pSymbol;
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 200 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12487 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 94:
			{
			CSymbolNode_multiplicative_expr_s* __pTarget = (CSymbolNode_multiplicative_expr_s*) pSymbol;
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 201 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12500 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 95:
			{
			CSymbolNode_at_expr_s* __pTarget = (CSymbolNode_at_expr_s*) pSymbol;
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 213 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12513 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 96:
			{
			CSymbolNode_at_expr_s* __pTarget = (CSymbolNode_at_expr_s*) pSymbol;
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 214 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12526 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 97:
			{
			CSymbolNode_assignment_expr_s* __pTarget = (CSymbolNode_assignment_expr_s*) pSymbol;
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 226 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12539 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 98:
			{
			CSymbolNode_assignment_expr_s* __pTarget = (CSymbolNode_assignment_expr_s*) pSymbol;
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 227 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12552 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 99:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 239 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12565 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 100:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 245 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12578 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 101:
			{
			CSymbolNode_postfix_expr_s* __pTarget = (CSymbolNode_postfix_expr_s*) pSymbol;
			CSymbolNode_unary_expr_s* __pSymbol = (CSymbolNode_unary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 253 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12591 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 102:
			{
			CSymbolNode_unary_operator_expr_s* __pTarget = (CSymbolNode_unary_operator_expr_s*) pSymbol;
			CSymbolNode_unary_expr_s* __pSymbol = (CSymbolNode_unary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 254 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12604 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 103:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 263 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12617 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 104:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 267 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12630 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 105:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 271 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12643 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 106:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 275 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12656 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 107:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 279 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12669 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 108:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 283 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12682 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 109:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 287 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12695 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 110:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 291 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12708 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 111:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 304 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12721 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 112:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 313 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12734 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 113:
			{
			CSymbolNode_primary_expr_s* __pTarget = (CSymbolNode_primary_expr_s*) pSymbol;
			CSymbolNode_postfix_expr_s* __pSymbol = (CSymbolNode_postfix_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 336 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12747 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 114:
			{
			CSymbolNode_postfix_operator_s* __pTarget = (CSymbolNode_postfix_operator_s*) pSymbol;
			CSymbolNode_postfix_expr_s* __pSymbol = (CSymbolNode_postfix_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 337 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12760 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 115:
			{
			CSymbolNode_member_operator_s* __pTarget = (CSymbolNode_member_operator_s*) pSymbol;
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 366 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12773 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 116:
			{
			CSymbolNode_member_operator_s* __pTarget = (CSymbolNode_member_operator_s*) pSymbol;
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 374 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12786 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 117:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 446 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12799 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 118:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
			
// #line 8 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 12812 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 119:
			{
			CSymbolNode_statement_pass1* __pTarget = (CSymbolNode_statement_pass1*) pSymbol;
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 17 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12825 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 120:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 26 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12838 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 121:
			{
			CSymbolNode_switch_block_stmt* __pTarget = (CSymbolNode_switch_block_stmt*) pSymbol;
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 157 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pStmt = &__pSymbol->m_Local.Stmt;
// #line 12851 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 122:
			{
			CSymbolNode_declaration_specifier* __pTarget = (CSymbolNode_declaration_specifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
			
// #line 16 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
// #line 12864 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 123:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
			
// #line 54 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
// #line 12877 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 124:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 4 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 12890 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 125:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 4 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 12903 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 126:
			{
			CSymbolNode_declarator* __pTarget = (CSymbolNode_declarator*) pSymbol;
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 8 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 12916 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 127:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
			
// #line 35 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 12929 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 128:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
			
// #line 36 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 12942 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 129:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
			
// #line 37 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 12955 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 130:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
			
// #line 38 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 12968 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 131:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 44 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 12981 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 132:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 48 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 12994 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 133:
			{
			CSymbolNode_array_suffix* __pTarget = (CSymbolNode_array_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 302 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13007 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 134:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 303 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13020 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 135:
			{
			CSymbolNode_bitfield_suffix* __pTarget = (CSymbolNode_bitfield_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 305 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 13033 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 136:
			{
			CSymbolNode_function_formal_argument_list* __pTarget = (CSymbolNode_function_formal_argument_list*) pSymbol;
			CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 342 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Local.pFunctionSuffix;
// #line 13046 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 137:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 347 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13059 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 138:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 348 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13072 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 139:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pTarget = (CSymbolNode_function_formal_argument_w_specifier*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 352 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13085 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 140:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pTarget = (CSymbolNode_function_formal_argument_no_specifier*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 353 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13098 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 141:
			{
			CSymbolNode_function_formal_argument_ellipsis* __pTarget = (CSymbolNode_function_formal_argument_ellipsis*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 354 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 13111 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 142:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 370 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13124 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 143:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 371 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13137 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 144:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 372 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13150 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 145:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 393 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13163 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 146:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 394 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13176 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 147:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 395 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13189 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 148:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
			
// #line 60 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
// #line 13202 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 149:
			{
			CSymbolNode_enum_member_list* __pTarget = (CSymbolNode_enum_member_list*) pSymbol;
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 70 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13215 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 150:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 79 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13228 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 151:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 79 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13241 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 152:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
			
// #line 122 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13254 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 153:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls43* __pAstNode = (_cls43*) __pSymbol->m_pAstNode;
			
// #line 169 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13267 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 154:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
			
// #line 195 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13280 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 155:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
			
// #line 227 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pProperty;
// #line 13293 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
			}
			break;
		
		case 156:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
			
// #line 251 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = __pSymbol->m_Local.pTemplate;
// #line 13306 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			
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
// #line 120 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
	;
// #line 13341 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_item_declaration_no_specifier:
			{
				CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 137 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
		__pSymbol->m_Local.Declarator.SetTypeSpecifier (NULL);
	;
// #line 13357 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_local_declaration:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 40 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		m_pModule->m_ControlFlowMgr.ResetJumpFlag (); // 'stack new' is currently illegal in conditional expression
	;
// #line 13371 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 9 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13388 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_function_suffix:
			{
				CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 334 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		__pSymbol->m_Local.pFunctionSuffix = __pSymbol->m_Arg.pDeclarator->AddFunctionSuffix ();
		__pSymbol->m_Local.PrevStorageKind = m_StorageKind;
	;
// #line 13406 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
// #line 47 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13423 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_assignment_operator_expr:
			{
				CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 471 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsAutoEv ())  // reset on assignment
			m_AutoEvBindSiteList.Clear (); 
	;
// #line 13475 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_curly_initializer:
			{
				CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 801 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		ASSERT (m_CurlyInitializerTargetValue);

		__pSymbol->m_Local.Initializer.m_TargetValue = m_CurlyInitializerTargetValue;
		__pSymbol->m_Local.Initializer.m_Index = 0;
		__pSymbol->m_Local.Initializer.m_Count = 0;
	;
// #line 13493 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_new_operator_curly_initializer:
			{
				CSymbolNode_new_operator_curly_initializer* __pSymbol = (CSymbolNode_new_operator_curly_initializer*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 645 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		m_CurlyInitializerTargetValue = *__pSymbol->m_Arg.pValue;
	;
// #line 13511 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_if_stmt:
			{
				CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 124 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.IfStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13564 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_switch_stmt:
			{
				CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 150 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.SwitchStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13577 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_while_stmt:
			{
				CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 181 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.WhileStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13590 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_do_stmt:
			{
				CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 200 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.DoStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13603 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_for_stmt:
			{
				CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 223 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.ForStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13616 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_autoev_stmt_0:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 81 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Stmt_0.llk"
				
		m_AutoEvBindSiteCount = 0;
	;
// #line 13653 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		case ESymbol_declarator:
			{
				CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
				_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 32 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		(*__pAstNode).m_Declarator.SetTypeSpecifier (__pSymbol->m_Arg.pTypeSpecifier);
	;
// #line 13674 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_operator_name:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 111 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		(*__pAstNode).m_UnOpKind = EUnOp_Undefined;
		(*__pAstNode).m_BinOpKind = EBinOp_Undefined;
	;
// #line 13689 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_function_formal_argument_w_specifier:
			{
				CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 363 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = EStorage_Undefined;
	;
// #line 13706 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_function_formal_argument_no_specifier:
			{
				CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 389 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = EStorage_Undefined;
		__pSymbol->m_Local.Declarator.SetTypeSpecifier (NULL);
	;
// #line 13720 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
// #line 13 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13775 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_declaration_terminator:
			{
				CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 170 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Decl.llk"
				
		if (m_pAttributeBlock)
		{
			err::SetFormatStringError ("unused attribute block in declaration");
			return false;
		}

		m_pLastDeclaredItem = NULL;
	;
// #line 13795 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		case ESymbol_function_suffix:
			{
				CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 339 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Declarator.llk"
				
		m_StorageKind = __pSymbol->m_Local.PrevStorageKind;
	;
// #line 13810 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
// #line 51 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13827 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_unary_operator_expr:
			{
				CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 548 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsAutoEv () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_AutoEvBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 13883 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		case ESymbol_primary_expr:
			{
				CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 729 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsAutoEv () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_AutoEvBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 13904 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_postfix_operator:
			{
				CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 669 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsAutoEv () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_AutoEvBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 13918 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_unary_operator_expr_s:
			{
				CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 259 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsAutoEv () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_AutoEvBindSiteCount++;
	;
// #line 13954 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		case ESymbol_primary_expr_s:
			{
				CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 402 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsAutoEv () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_AutoEvBindSiteCount++;
	;
// #line 13969 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		case ESymbol_postfix_operator_s:
			{
				CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 342 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsAutoEv () && IsBindableType (__pSymbol->m_Arg.pValue->GetType ()))
			m_AutoEvBindSiteCount++;
	;
// #line 13983 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_enum_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 43 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		if ((*__pAstNode).m_pType->GetName ().IsEmpty () || ((*__pAstNode).m_pType->GetFlags () & EEnumTypeFlag_Exposed))
			return m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->ExposeEnumConsts ((*__pAstNode).m_pType);
	;
// #line 14054 "D:/Prj/Ninja/axl/src/axl_jnc/axl_jnc_Parser.llk.h"
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
					
				pTransition->m_ProductionIndex = 1661;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 150;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 1663;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 3:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 150;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 4:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				pTransition->m_ResolverIndex = 814;
				pTransition->m_ResolverElseIndex = 1665;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 5:
			
			pTransition->m_ProductionIndex = 151;
			pTransition->m_ResolverIndex = 311;
			pTransition->m_ResolverElseIndex = 152;
				
			return ELaDfaResult_Resolver;
			
		
		case 6:
			
			switch (LookaheadToken)
			{
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 151;
				pTransition->m_ResolverIndex = 311;
				pTransition->m_ResolverElseIndex = 152;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 7:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 153;
				pTransition->m_ResolverElseIndex = 152;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 8:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 153;
				pTransition->m_ResolverElseIndex = 152;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 9:
			
			switch (LookaheadToken)
			{
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 1670;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 10:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 150;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 11:
			
			switch (LookaheadToken)
			{
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 1672;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 12:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 150;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 13:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				pTransition->m_ResolverIndex = 814;
				pTransition->m_ResolverElseIndex = 1674;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 14:
			
			pTransition->m_ProductionIndex = 151;
			pTransition->m_ResolverIndex = 311;
			pTransition->m_ResolverElseIndex = 152;
				
			return ELaDfaResult_Resolver;
			
		
		case 15:
			
			switch (LookaheadToken)
			{
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 151;
				pTransition->m_ResolverIndex = 311;
				pTransition->m_ResolverElseIndex = 152;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 16:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 153;
				pTransition->m_ResolverElseIndex = 152;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 17:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 153;
				pTransition->m_ResolverElseIndex = 152;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 18:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 955;
				pTransition->m_ResolverElseIndex = 1679;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 19:
			
			pTransition->m_ProductionIndex = 152;
			pTransition->m_ResolverIndex = 295;
			pTransition->m_ResolverElseIndex = 157;
				
			return ELaDfaResult_Resolver;
			
		
		case 20:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1681;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 21:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 22:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1683;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 23:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 24:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 955;
				pTransition->m_ResolverElseIndex = 1685;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 25:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 26:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1687;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 27:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 28:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1689;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 29:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 30:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1691;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 31:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 32:
			
			switch (LookaheadToken)
			{
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 1693;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 33:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 34:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1695;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 35:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 36:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1697;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 37:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 38:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1699;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 39:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 40:
			
			switch (LookaheadToken)
			{
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1701;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 41:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 42:
			
			switch (LookaheadToken)
			{
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1703;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 43:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 44:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				pTransition->m_ResolverIndex = 814;
				pTransition->m_ResolverElseIndex = 1705;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 45:
			
			pTransition->m_ProductionIndex = 151;
			pTransition->m_ResolverIndex = 311;
			pTransition->m_ResolverElseIndex = 152;
				
			return ELaDfaResult_Resolver;
			
		
		case 46:
			
			switch (LookaheadToken)
			{
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 151;
				pTransition->m_ResolverIndex = 311;
				pTransition->m_ResolverElseIndex = 152;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 47:
			
			switch (LookaheadToken)
			{
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 955;
				pTransition->m_ResolverElseIndex = 157;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 48:
			
			switch (LookaheadToken)
			{
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 955;
				pTransition->m_ResolverElseIndex = 157;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 49:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1710;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 50:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 51:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1712;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 52:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 53:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1714;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 54:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 55:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1716;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 56:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 57:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1718;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 58:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 59:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1720;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 60:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 61:
			
			switch (LookaheadToken)
			{
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 1722;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 62:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 63:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1724;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 64:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 65:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1726;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 66:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 67:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1728;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 68:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 69:
			
			switch (LookaheadToken)
			{
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1730;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 70:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 71:
			
			switch (LookaheadToken)
			{
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1732;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 72:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 258;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 267;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 73:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 1734;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 74:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 910;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 909;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 75:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 499;
				pTransition->m_ResolverIndex = 295;
				pTransition->m_ResolverElseIndex = 500;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 76:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 586;
				pTransition->m_ResolverIndex = 215;
				pTransition->m_ResolverElseIndex = 585;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 77:
			
			switch (LookaheadToken)
			{
			
			case '=':
					
				pTransition->m_ProductionIndex = 1738;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 78:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 572;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 571;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 79:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 602;
				pTransition->m_ResolverIndex = 219;
				pTransition->m_ResolverElseIndex = 601;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 80:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 783;
				pTransition->m_ResolverIndex = 814;
				pTransition->m_ResolverElseIndex = 784;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 81:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 634;
				pTransition->m_ResolverIndex = 1072;
				pTransition->m_ResolverElseIndex = 636;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 82:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 675;
				pTransition->m_ResolverIndex = 215;
				pTransition->m_ResolverElseIndex = 674;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 83:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 690;
				pTransition->m_ResolverIndex = 219;
				pTransition->m_ResolverElseIndex = 689;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 84:
			
			switch (LookaheadToken)
			{
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 289;
				pTransition->m_ResolverIndex = 1177;
				pTransition->m_ResolverElseIndex = 290;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 85:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 779;
				pTransition->m_ResolverIndex = 814;
				pTransition->m_ResolverElseIndex = 780;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 86:
			
			switch (LookaheadToken)
			{
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 1747;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 87:
			
			switch (LookaheadToken)
			{
			
			case '<':
					
				pTransition->m_ProductionIndex = 802;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 801;
				return ELaDfaResult_Production;
					
			}
			
		
		case 88:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1749;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 89:
			
			switch (LookaheadToken)
			{
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 855;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 856;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 90:
			
			switch (LookaheadToken)
			{
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 1751;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 91:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 857;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '|':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '^':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '<':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '>':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '/':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '%':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '@':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_LogOr:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_LogAnd:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Eq:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Ne:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Le:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Ge:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Shl:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Shr:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_RefAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_AddAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_SubAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_MulAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_DivAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_ModAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_ShlAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_ShrAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_AndAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_XorAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_OrAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 858;
				return ELaDfaResult_Production;
					
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			case EToken_AtAssign:
					
				pTransition->m_ProductionIndex = 859;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 92:
			
			switch (LookaheadToken)
			{
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 1753;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 93:
			
			switch (LookaheadToken)
			{
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 865;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 866;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 867;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 94:
			
			switch (LookaheadToken)
			{
			
			case '[':
					
				pTransition->m_ProductionIndex = 1755;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 95:
			
			switch (LookaheadToken)
			{
			
			case '\01':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '#':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '(':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case ';':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '}':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case ',':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '.':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case ']':
					
				pTransition->m_ProductionIndex = 908;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '?':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '|':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '^':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '<':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '>':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '/':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '%':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '@':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Pack:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Namespace:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Extend:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Using:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Friend:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_LogOr:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_LogAnd:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Eq:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Ne:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Le:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Ge:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Shl:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Shr:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_RefAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_AddAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_SubAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_MulAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_DivAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_ModAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_ShlAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_ShrAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_AndAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_XorAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_OrAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 907;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_If:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Else:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Switch:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Case:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Default:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_While:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Do:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_For:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Break:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Continue:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Silent:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Return:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Once:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_AtAssign:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 906;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 96:
			
			switch (LookaheadToken)
			{
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 1757;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 97:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case ',':
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 913;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 915;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 98:
			
			switch (LookaheadToken)
			{
			
			case ':':
					
				pTransition->m_ProductionIndex = 495;
				pTransition->m_ResolverIndex = 1297;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 99:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 724;
				pTransition->m_ResolverIndex = 256;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 100:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				pTransition->m_ResolverIndex = 295;
				pTransition->m_ResolverElseIndex = 741;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 101:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1762;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 102:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 103:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1764;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 104:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 105:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1766;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 106:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 107:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1768;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 108:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 109:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1770;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 110:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 111:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1772;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 112:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 113:
			
			switch (LookaheadToken)
			{
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 1774;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 114:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 115:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1776;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 116:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 117:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1778;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 118:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 119:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1780;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 120:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 121:
			
			switch (LookaheadToken)
			{
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 1782;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 122:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 123:
			
			switch (LookaheadToken)
			{
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 1784;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 124:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 741;
				return ELaDfaResult_Production;
					
			
			case EToken_Construct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Destruct:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 125:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 776;
				pTransition->m_ResolverIndex = 295;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 126:
			
			switch (LookaheadToken)
			{
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 271;
				pTransition->m_ResolverIndex = 1173;
				pTransition->m_ResolverElseIndex = 272;
						
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
