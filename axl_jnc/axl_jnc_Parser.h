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
	ESymbol_type_name = 28, 
	ESymbol_type_specifier_modifier_list = 29, 
	ESymbol_type_name_list = 30, 
	ESymbol_type_name_or_expr = 31, 
	ESymbol_expression_s = 32, 
	ESymbol_attribute_block = 33, 
	ESymbol_attribute_declarator_list = 34, 
	ESymbol_attribute_declarator = 35, 
	ESymbol_expression_pass1 = 36, 
	ESymbol_primary_expr_pass1 = 37, 
	ESymbol_expression_save_value = 38, 
	ESymbol_expression = 39, 
	ESymbol_conditional_expr = 40, 
	ESymbol_expression_or_empty = 41, 
	ESymbol_expression_list = 42, 
	ESymbol_expression_or_empty_list = 43, 
	ESymbol_constant_expr = 44, 
	ESymbol_constant_integer_expr = 45, 
	ESymbol_logical_or_expr = 46, 
	ESymbol_logical_and_expr = 47, 
	ESymbol_inclusive_or_expr = 48, 
	ESymbol_exclusive_or_expr = 49, 
	ESymbol_and_expr = 50, 
	ESymbol_equality_expr = 51, 
	ESymbol_relational_expr = 52, 
	ESymbol_equality_operator = 53, 
	ESymbol_shift_expr = 54, 
	ESymbol_relational_operator = 55, 
	ESymbol_additive_expr = 56, 
	ESymbol_shift_operator = 57, 
	ESymbol_multiplicative_expr = 58, 
	ESymbol_additive_operator = 59, 
	ESymbol_at_expr = 60, 
	ESymbol_multiplicative_operator = 61, 
	ESymbol_assignment_expr = 62, 
	ESymbol_unary_expr = 63, 
	ESymbol_assignment_operator_expr = 64, 
	ESymbol_assignment_operator = 65, 
	ESymbol_curly_initializer = 66, 
	ESymbol_postfix_expr = 67, 
	ESymbol_unary_operator_expr = 68, 
	ESymbol_cast_operator_rslv = 69, 
	ESymbol_storage_specifier = 70, 
	ESymbol_new_operator_type = 71, 
	ESymbol_new_operator_curly_initializer = 72, 
	ESymbol_type_name_w_constructor_rslv = 73, 
	ESymbol_qualified_type_name = 74, 
	ESymbol_type_specifier_modifier = 75, 
	ESymbol_primary_expr = 76, 
	ESymbol_postfix_operator = 77, 
	ESymbol_member_operator = 78, 
	ESymbol_literal = 79, 
	ESymbol_curly_initializer_item = 80, 
	ESymbol_expression_save_value_s = 81, 
	ESymbol_conditional_expr_s = 82, 
	ESymbol_expression_or_empty_s = 83, 
	ESymbol_expression_list_s = 84, 
	ESymbol_expression_or_empty_list_s = 85, 
	ESymbol_logical_or_expr_s = 86, 
	ESymbol_logical_and_expr_s = 87, 
	ESymbol_inclusive_or_expr_s = 88, 
	ESymbol_exclusive_or_expr_s = 89, 
	ESymbol_and_expr_s = 90, 
	ESymbol_equality_expr_s = 91, 
	ESymbol_relational_expr_s = 92, 
	ESymbol_shift_expr_s = 93, 
	ESymbol_additive_expr_s = 94, 
	ESymbol_multiplicative_expr_s = 95, 
	ESymbol_at_expr_s = 96, 
	ESymbol_assignment_expr_s = 97, 
	ESymbol_unary_expr_s = 98, 
	ESymbol_postfix_expr_s = 99, 
	ESymbol_unary_operator_expr_s = 100, 
	ESymbol_new_operator_sype_s = 101, 
	ESymbol_primary_expr_s = 102, 
	ESymbol_postfix_operator_s = 103, 
	ESymbol_member_operator_s = 104, 
	ESymbol_compound_stmt_pass1 = 105, 
	ESymbol_statement_pass1 = 106, 
	ESymbol_compound_stmt = 107, 
	ESymbol_constructor_compound_stmt = 108, 
	ESymbol_base_type_constructor = 109, 
	ESymbol_base_type_constructor_rslv = 110, 
	ESymbol_expression_stmt = 111, 
	ESymbol_if_stmt = 112, 
	ESymbol_switch_stmt = 113, 
	ESymbol_while_stmt = 114, 
	ESymbol_do_stmt = 115, 
	ESymbol_for_stmt = 116, 
	ESymbol_break_stmt = 117, 
	ESymbol_continue_stmt = 118, 
	ESymbol_return_stmt = 119, 
	ESymbol_once_stmt = 120, 
	ESymbol_switch_block_stmt = 121, 
	ESymbol_autoev_body = 122, 
	ESymbol_autoev_stmt = 123, 
	ESymbol_autoev_onchange_stmt = 124, 
	ESymbol_autoev_expression_stmt = 125, 
	ESymbol_statement_0 = 126, 
	ESymbol_compound_stmt_0 = 127, 
	ESymbol_expression_stmt_0 = 128, 
	ESymbol_if_stmt_0 = 129, 
	ESymbol_switch_stmt_0 = 130, 
	ESymbol_while_stmt_0 = 131, 
	ESymbol_do_stmt_0 = 132, 
	ESymbol_for_stmt_0 = 133, 
	ESymbol_break_stmt_0 = 134, 
	ESymbol_continue_stmt_0 = 135, 
	ESymbol_return_stmt_0 = 136, 
	ESymbol_once_stmt_0 = 137, 
	ESymbol_expression_0 = 138, 
	ESymbol_primary_expr_0 = 139, 
	ESymbol_autoev_body_0 = 140, 
	ESymbol_autoev_stmt_0 = 141, 
	ESymbol_autoev_onchange_stmt_0 = 142, 
	ESymbol_autoev_expression_stmt_0 = 143, 
	ESymbol_declaration_specifier = 144, 
	ESymbol_type_specifier = 145, 
	ESymbol_type_modifier = 146, 
	ESymbol_property_template_specifier = 147, 
	ESymbol_qualified_type_name_rslv = 148, 
	ESymbol_full_declarator = 149, 
	ESymbol_declarator = 150, 
	ESymbol_declarator_qualifier = 151, 
	ESymbol_operator_name = 152, 
	ESymbol_declarator_prefix_kind = 153, 
	ESymbol_array_suffix = 154, 
	ESymbol_bitfield_suffix = 155, 
	ESymbol_function_formal_argument_list = 156, 
	ESymbol_function_formal_argument = 157, 
	ESymbol_function_formal_argument_w_specifier = 158, 
	ESymbol_function_formal_argument_no_specifier = 159, 
	ESymbol_function_formal_argument_ellipsis = 160, 
	ESymbol_enum_specifier = 161, 
	ESymbol_struct_specifier = 162, 
	ESymbol_union_specifier = 163, 
	ESymbol_class_specifier = 164, 
	ESymbol_property_specifier = 165, 
	ESymbol_autoev_class_specifier = 166, 
	ESymbol_enum_block = 167, 
	ESymbol_enum_member_list = 168, 
	ESymbol_enum_member = 169, 
	ESymbol_struct_pack_factor = 170, 
	ESymbol_named_type_block = 171, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 2,

		StartSymbol        = 0,
		StartPragmaSymbol  = 312,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 144,
		SymbolCount        = 313,
		SequenceCount      = 479,
		ActionCount        = 375,
		ArgumentCount      = 154,
		BeaconCount        = 168,
		LaDfaCount         = 105,

		TotalCount         = 1738,

		NamedSymbolCount   = 172,

		TokenFirst         = 0,
		TokenEnd           = 144,
		SymbolFirst        = 144,
		NamedSymbolEnd     = 316,
		SymbolEnd          = 457,
		SequenceFirst      = 457,
		SequenceEnd        = 936,
		ActionFirst        = 936,
		ActionEnd          = 1311,
		ArgumentFirst      = 1311,
		ArgumentEnd        = 1465,
		BeaconFirst        = 1465,
		BeaconEnd          = 1633,
		LaDfaFirst         = 1633,
		LaDfaEnd           = 1738,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	
	enum EStage
	{
		EStage_Pass1, // global declarations (before linkage)
		EStage_Pass2, // statements and expressions (after linkage & calclayout)		
		EStage_AutoEvScan, // scan autoev body to build bind site map (after linkage but before calclayout)
	};

	enum EAssign
	{
		EAssign_Normal,
		EAssign_BinOp,
	};
	
	struct TAutoEvHandler: rtl::TListLink
	{
		rtl::CBoxListT <CValue> m_BindSiteList;
		CFunction* m_pFunction;
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
	size_t m_StructPackFactor;
	size_t m_DefaultStructPackFactor;	
	EStorage m_StorageKind;
	EAccess m_AccessKind;
	CAttributeBlock* m_pAttributeBlock;
	CModuleItem* m_pLastDeclaredItem;
	rtl::CArrayT <CTypeSpecifier*> m_TypeSpecifierStack;
	rtl::CStdListT <TAutoEvHandler> m_AutoEvHandlerList;
	rtl::CBoxListT <CValue> m_AutoEvBindSiteList;
	size_t m_AutoEvBindSiteCount;
	size_t m_AutoEvBindSiteTotalCount;
	CValue m_ExpressionValue;
	CValue m_CurlyInitializerTargetValue;
	CClassType* m_pMemberNewTargetType;


	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class _cls1: public CAstNode
	{
	public:
// #line 91 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CQualifiedName m_Name
;
// #line 304 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls2: public CAstNode
	{
	public:
// #line 190 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
// #line 314 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls3: public CAstNode
	{
	public:
// #line 210 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	rtl::CBoxListT <CType*> m_TypeList;
;
// #line 324 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls4: public CAstNode
	{
	public:
// #line 227 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
// #line 334 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls5: public CAstNode
	{
	public:
// #line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 344 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls6: public CAstNode
	{
	public:
// #line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
// #line 354 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls7: public CAstNode
	{
	public:
// #line 66 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;
;
// #line 364 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls8: public CAstNode
	{
	public:
// #line 76 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 374 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls9: public CAstNode
	{
	public:
// #line 96 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 384 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls10: public CAstNode
	{
	public:
// #line 124 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
// #line 394 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls11: public CAstNode
	{
	public:
// #line 140 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	intptr_t m_Value;
;
// #line 404 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls12: public CAstNode
	{
	public:
// #line 164 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_TrueValue;
	CValue m_FalseValue;
	CBasicBlock* m_pThenBlock;
	CBasicBlock* m_pElseBlock;
	CBasicBlock* m_pPhiBlock;
;
// #line 418 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls13: public CAstNode
	{
	public:
// #line 284 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 428 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls14: public CAstNode
	{
	public:
// #line 313 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 438 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls15: public CAstNode
	{
	public:
// #line 350 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 448 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls16: public CAstNode
	{
	public:
// #line 379 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 458 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls17: public CAstNode
	{
	public:
// #line 408 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 468 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls18: public CAstNode
	{
	public:
// #line 466 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
// #line 478 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls19: public CAstNode
	{
	public:
// #line 597 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CType* m_pType;
	rtl::CBoxListT <CValue> m_ArgList;
;
// #line 489 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls20: public CAstNode
	{
	public:
// #line 749 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CString m_String;
;
// #line 499 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls21: public CAstNode
	{
	public:
// #line 12 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
		
	CValue m_Value;	
;
// #line 509 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls22: public CAstNode
	{
	public:
// #line 21 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
		
	CValue m_Value;
;
// #line 519 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls23: public CAstNode
	{
	public:
// #line 31 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 529 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls24: public CAstNode
	{
	public:
// #line 51 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
// #line 539 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls25: public CAstNode
	{
	public:
// #line 335 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
		
	CType* m_pType;
;
// #line 549 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls26: public CAstNode
	{
	public:
// #line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
// #line 559 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls27: public CAstNode
	{
	public:
// #line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CTypeSpecifier m_TypeSpecifier;
;
// #line 569 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls28: public CAstNode
	{
	public:
// #line 43 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CTypeSpecifier m_TypeSpecifier;
;
// #line 579 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls29: public CAstNode
	{
	public:
// #line 72 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EStorage m_StorageKind;
;
// #line 589 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls30: public CAstNode
	{
	public:
// #line 125 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EAccess m_AccessKind;
;
// #line 599 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls31: public CAstNode
	{
	public:
// #line 142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
// #line 609 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls32: public CAstNode
	{
	public:
// #line 241 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
// #line 619 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls33: public CAstNode
	{
	public:
// #line 261 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	ETypeModifier m_Modifier;
;
// #line 629 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls34: public CAstNode
	{
	public:
// #line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
		
	CDeclarator m_Declarator;
;
// #line 639 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls35: public CAstNode
	{
	public:
// #line 99 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
		
	EUnOp m_UnOpKind;
	EBinOp m_BinOpKind;
;
// #line 650 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls36: public CAstNode
	{
	public:
// #line 293 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
		
	EDeclPrefix m_PrefixKind;
;
// #line 660 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls37: public CAstNode
	{
	public:
// #line 6 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CType* m_pType;
;
// #line 670 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls38: public CAstNode
	{
	public:
// #line 41 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CEnumType* m_pType;
;
// #line 680 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls39: public CAstNode
	{
	public:
// #line 106 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CStructType* m_pType;
;
// #line 690 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls40: public CAstNode
	{
	public:
// #line 130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_PackFactor;
;
// #line 700 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls41: public CAstNode
	{
	public:
// #line 141 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_Value;
;
// #line 710 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls42: public CAstNode
	{
	public:
// #line 160 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CUnionType* m_pType;
;
// #line 720 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls43: public CAstNode
	{
	public:
// #line 179 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CClassType* m_pType;
;
// #line 730 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls44: public CAstNode
	{
	public:
// #line 205 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CClassType* m_pType;
;
// #line 740 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls45: public CAstNode
	{
	public:
// #line 235 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CProperty* m_pProperty;
;
// #line 750 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls46: public CAstNode
	{
	public:
// #line 264 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CPropertyType* m_pType;
;
// #line 760 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_item_declaration_no_specifier_rslv: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 156 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
		CDeclarator Declarator;
	;
// #line 788 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 129 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
		CDeclarator Declarator;
	;
// #line 810 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 98 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			CQualifiedName* pName;
// #line 837 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 3 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 858 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 164 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 878 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 280 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 900 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 33 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 920 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 338 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 940 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 340 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclFunctionSuffix* pFunctionSuffix;
	;
// #line 949 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_function_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_suffix;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_declarator_suffix: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 316 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 971 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 426 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 991 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_post_declarator_modifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_post_declarator_modifier;
			
		}
	};
		
	
	
	class CSymbolNode_type_name: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 195 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
				
		CDeclarator Declarator;
	;
// #line 1014 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_type_name ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_type_name;
			
		}
	};
		
	
	
	
	
	
	
	
	
	
	class CSymbolNode_primary_expr_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 12 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 1042 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_primary_expr_pass1 ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_primary_expr_pass1;
			
		}
	};
		
	
	
	
	class CSymbolNode_conditional_expr: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 171 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1064 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 199 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1089 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 201 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1098 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 213 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1118 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 215 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1127 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 227 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1147 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 229 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1156 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 241 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1176 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 243 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1185 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 255 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1205 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 257 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1214 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 269 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 271 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1243 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 298 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1263 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 300 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1272 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 335 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1293 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 337 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 364 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1323 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 366 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1332 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 393 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1353 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 395 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1362 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 426 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1383 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 428 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1392 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 440 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1413 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 520 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1433 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 444 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1453 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 446 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
// #line 1462 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 769 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		TCurlyInitializer Initializer;
	;
// #line 1487 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 637 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1509 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 525 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1529 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 631 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CTypeSpecifier TypeSpecifier;
	;
// #line 1553 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 618 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1575 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 58 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 1599 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 699 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1619 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 642 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1641 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 676 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
// #line 1663 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 791 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			TCurlyInitializer* pInitializer;
// #line 1684 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 793 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue PrevCurlyInitializerTargetValue;
	;
// #line 1693 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 80 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1714 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 82 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue TrueValue;
		CValue FalseValue;
	;
// #line 1724 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 95 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1747 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 97 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1756 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 109 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1776 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 111 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1785 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 123 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1805 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 125 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1814 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 137 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1834 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 139 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1843 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 151 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1863 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 153 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1872 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 165 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1892 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 167 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1901 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 179 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1921 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 181 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1930 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 193 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1950 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 195 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1959 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 207 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 1979 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 209 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 1988 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2008 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 223 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2017 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 235 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2037 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 237 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2046 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 249 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2066 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 251 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
		CValue OpValue2;
	;
// #line 2075 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 267 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2095 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 350 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2115 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 272 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2135 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 412 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2158 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 355 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2180 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 389 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			CValue* pValue;
// #line 2202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 12 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 2222 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			rtl::CBoxListT <CToken>* pTokenList;
// #line 2242 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 103 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
		TIfStmt Stmt;
	;
// #line 2269 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 129 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
		TSwitchStmt Stmt;
	;
// #line 2293 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 160 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
		TWhileStmt Stmt;
	;
// #line 2317 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 179 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
		TDoStmt Stmt;
	;
// #line 2341 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
		TForStmt Stmt;
	;
// #line 2365 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 264 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
		TOnceStmt Stmt;
	;
// #line 2392 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_once_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_once_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_switch_block_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 146 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			TSwitchStmt* pStmt;
// #line 2414 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2456 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 269 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
		CPropertyTemplate* pTemplate;
	;
// #line 2480 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 7 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2501 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 20 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
// #line 2521 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 41 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2543 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 325 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2565 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 417 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
// #line 2585 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 351 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2605 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 356 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2625 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 362 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2645 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 364 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclarator Declarator;
	;
// #line 2654 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_function_formal_argument_w_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_formal_argument_w_specifier;
			
		}
	};
		
	
	class CSymbolNode_function_formal_argument_no_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 384 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2674 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
// #line 386 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclarator Declarator;
	;
// #line 2683 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 406 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
// #line 2705 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_function_formal_argument_ellipsis ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_formal_argument_ellipsis;
			
		}
	};
		
	
	
	
	
	
	
	class CSymbolNode_autoev_class_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 210 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
		CDeclarator Declarator;
	;
// #line 2732 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_autoev_class_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_autoev_class_specifier;
			
		}
	};
		
	
	class CSymbolNode_enum_block: public CSymbolNode
	{
	public:
			
		struct
		{
// #line 68 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2752 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 81 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2772 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 86 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
// #line 2792 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
// #line 287 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CNamespace* pNamespace;
// #line 2813 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
	IsAutoEvStarter ()
	{
		CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
		return pFunction && pFunction->GetFunctionKind () == EFunction_AutoEvStarter;
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
	Declare (
		CDeclarator* pDeclarator,
		rtl::CBoxListT <CToken>* pInitializer = NULL
		);
		
	bool
	DeclareTypedef (
		CType* pType,
		CDeclarator* pDeclarator
		);

	bool
	DeclareAlias (
		CType* pType,
		CDeclarator* pDeclarator,
		rtl::CBoxListT <CToken>* pInitializer
		);

	bool
	DeclareFunction (
		CFunctionType* pType,
		CDeclarator* pDeclarator
		);

	bool
	DeclareProperty (
		CPropertyType* pType,
		CDeclarator* pDeclarator
		);

	bool
	DeclareAutoEv (
		CAutoEvType* pType,
		CDeclarator* pDeclarator
		);

	bool
	DeclareData (
		CType* pType,
		CDeclarator* pDeclarator,
		int PtrTypeFlags,
		rtl::CBoxListT <CToken>* pInitializer
		);

	bool
	DeclarePropValue (
		CType* pType,
		int PtrTypeFlags,
		rtl::CBoxListT <CToken>* pInitializer
		);

	bool
	DeclareOnChange (
		CType* pType,
		int PtrTypeFlags
		);

	CFunctionArg*
	CreateFormalArg (
		CDeclFunctionSuffix* pArgSuffix,
		CDeclarator* pDeclarator,
		rtl::CBoxListT <CToken>* pInitializer
		);

	CEnumType*
	CreateEnumType (
		const rtl::CString& Name,
		CType* pBaseType,
		int Flags
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
		const CToken::CPos& Pos,
		size_t PackFactor
		);

	CPropertyTemplate*
	CreatePropertyTemplate ();

	CClassType*
	CreateAutoEvClassType (
		const rtl::CString& Name,
		CDeclFunctionSuffix* pFunctionSuffix,
		rtl::CBoxListT <CToken>* pTokenList
		);

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

	void
	FinalizeAutoEvOnChangeStmt ();

	bool
	AutoEvExpressionStmt (rtl::CBoxListT <CToken>* pTokenList);

	bool
	CountAutoEvBindSites ();

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
	GetAuPropertyField (
		CValue* pValue,
		EAuPropertyField Field
		);

	bool
	GetAuPropertyFieldType (
		CValue* pValue,
		EAuPropertyField Field
		);

	void
	PreStatement ();

	bool
	PrepareCurlyInitializerNamedItem (
		TCurlyInitializer* pInitializer, 
		const tchar_t* pName
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
			0, -1, -1, -1, -1, 457, -1, -1, -1, -1, -1, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, -1, -1, -1, 457, -1, -1, 457, 457, 457, 457, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, 457, 457, 457, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, -1, -1, 457, 457, -1, 457, 457, 457, 457, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 317, -1, -1, -1, 317, -1, -1, 148, 149, 150, 151, 1641, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 317, 317, 317, 317, 317, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, 154, 1642, 154, 154, 154, 154, 154, 154, 154, 154, 1633, 1635, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 1637, 154, 1638, 1640, 154, 154, 154, 317, 317, -1, -1, 317, 317, -1, 153, 153, 153, 153, 
			-1, -1, 458, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 925, -1, -1, -1, -1, -1, -1, -1, -1, -1, 926, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 459, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 461, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 463, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 465, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 467, 467, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 912, -1, 913, 914, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 909, 909, 910, 911, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 472, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 472, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, 472, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 477, -1, -1, -1, -1, -1, -1, 477, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 477, 477, 477, 477, 477, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 477, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 477, 477, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 474, -1, -1, -1, 474, -1, -1, -1, -1, -1, -1, 474, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 474, 474, 474, 474, 474, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 474, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 474, 474, -1, -1, 474, 474, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 318, -1, -1, -1, 318, -1, -1, -1, -1, -1, 151, 1651, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 318, 318, 318, 318, 318, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 154, 154, 1652, 154, 154, 154, 154, 154, 154, 154, 154, 1643, 1645, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 1647, 154, 1648, 1650, 154, 154, 154, 318, 318, -1, -1, 318, 318, -1, 153, 153, 153, 153, 
			-1, -1, -1, 159, -1, 159, 159, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, 159, -1, -1, 159, 159, 1681, -1, -1, -1, 1682, 159, -1, -1, -1, 150, -1, 1653, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 159, 159, 159, 159, 159, 159, -1, 1683, 1684, 319, 319, 1685, 159, 159, 159, 159, 159, 159, 159, -1, 159, -1, -1, 159, 159, 159, 159, 159, 159, 159, 159, 1655, 1657, 1659, 1661, 1663, 1665, 1667, 1669, 1671, 1673, 1675, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 1677, 154, 1678, 1680, 154, 154, 154, 319, 319, -1, -1, 319, 319, -1, 153, 153, 153, 153, 
			-1, -1, -1, 255, -1, 5, 251, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 255, -1, -1, 255, 255, 255, -1, -1, -1, 255, 255, -1, -1, -1, -1, -1, 255, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 255, 255, 255, 255, 255, 255, -1, 255, 255, -1, -1, 255, 255, 255, 255, 255, 255, 255, 256, -1, 257, -1, -1, 258, 259, 260, 261, 262, 263, 263, 264, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 468, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 773, 774, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 469, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 752, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 752, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, 752, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 816, -1, -1, -1, 816, -1, -1, -1, -1, -1, -1, 816, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 816, 816, 816, 816, 816, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 816, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 816, 816, -1, -1, 816, 816, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 478, 479, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 879, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 879, 879, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 824, -1, -1, -1, -1, -1, -1, 824, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 824, 824, 824, 824, 824, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 824, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 824, 824, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1686, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 885, -1, -1, -1, -1, -1, 430, -1, 884, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 908, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 480, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, 480, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 758, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, 758, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 483, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, 483, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 486, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 486, -1, -1, 486, 486, 486, -1, -1, -1, 486, 486, -1, -1, -1, -1, -1, 1688, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 486, 486, 486, 486, 486, 486, -1, 486, 486, -1, -1, 486, 486, 486, 486, 486, 486, 486, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, 486, -1, -1, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, 485, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 620, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 620, -1, -1, 620, 620, 620, -1, -1, -1, 620, 620, -1, -1, -1, -1, -1, 620, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 620, 620, 620, 620, 620, 620, -1, 620, 620, -1, -1, 620, 620, 620, 620, 620, 620, 620, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, 620, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 487, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 489, -1, -1, -1, 489, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 489, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 492, 
			-1, 500, 500, 498, 500, 500, 494, 500, 500, 500, 500, 496, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 
			-1, -1, -1, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, -1, -1, 501, 501, 501, -1, -1, -1, 501, 501, -1, -1, -1, -1, -1, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, 501, 501, 501, 501, 501, -1, 501, 501, -1, -1, 501, 501, 501, 501, 501, 501, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, 502, 502, 502, -1, -1, -1, 502, 502, -1, -1, -1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, 502, 502, 502, 502, 502, -1, 502, 502, -1, -1, 502, 502, 502, 502, 502, 502, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 512, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 512, -1, -1, 512, 512, 512, -1, -1, -1, 512, 512, -1, -1, -1, -1, -1, 512, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 512, 512, 512, 512, 512, 512, -1, 512, 512, -1, -1, 512, 512, 512, 512, 512, 512, 512, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 503, 0, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 503, -1, -1, 503, 503, 503, -1, -1, -1, 503, 503, -1, -1, -1, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, 503, 503, 503, 503, 503, -1, 503, 503, -1, -1, 503, 503, 503, 503, 503, 503, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 504, -1, -1, 504, 504, 504, -1, -1, -1, 504, 504, -1, -1, -1, -1, -1, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 504, 504, 504, 504, 504, 504, -1, 504, 504, -1, -1, 504, 504, 504, 504, 504, 504, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 506, 0, 0, -1, -1, 509, -1, -1, -1, -1, -1, -1, -1, -1, 506, -1, -1, 506, 506, 506, -1, -1, -1, 506, 506, -1, -1, -1, -1, -1, 506, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 506, 506, 506, 506, 506, 506, -1, 506, 506, -1, -1, 506, 506, 506, 506, 506, 506, 506, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 510, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 510, -1, -1, 510, 510, 510, -1, -1, -1, 510, 510, -1, -1, -1, -1, -1, 510, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 510, 510, 510, 510, 510, 510, -1, 510, 510, -1, -1, 510, 510, 510, 510, 510, 510, 510, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, 510, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 511, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 511, -1, -1, 511, 511, 511, -1, -1, -1, 511, 511, -1, -1, -1, -1, -1, 511, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 511, 511, 511, 511, 511, 511, -1, 511, 511, -1, -1, 511, 511, 511, 511, 511, 511, 511, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 514, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 514, -1, -1, 514, 514, 514, -1, -1, -1, 514, 514, -1, -1, -1, -1, -1, 514, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 514, 514, 514, 514, 514, 514, -1, 514, 514, -1, -1, 514, 514, 514, 514, 514, 514, 514, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, 514, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 516, -1, -1, 516, 516, 516, -1, -1, -1, 516, 516, -1, -1, -1, -1, -1, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 516, 516, 516, 516, 516, 516, -1, 516, 516, -1, -1, 516, 516, 516, 516, 516, 516, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, 516, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 518, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 518, -1, -1, 518, 518, 518, -1, -1, -1, 518, 518, -1, -1, -1, -1, -1, 518, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 518, 518, 518, 518, 518, 518, -1, 518, 518, -1, -1, 518, 518, 518, 518, 518, 518, 518, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, 518, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, -1, -1, 520, 520, 520, -1, -1, -1, 520, 520, -1, -1, -1, -1, -1, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, 520, 520, 520, 520, 520, -1, 520, 520, -1, -1, 520, 520, 520, 520, 520, 520, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, 520, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, -1, -1, 522, 522, 522, -1, -1, -1, 522, 522, -1, -1, -1, -1, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, 522, 522, 522, 522, 522, -1, 522, 522, -1, -1, 522, 522, 522, 522, 522, 522, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, -1, -1, 524, 524, 524, -1, -1, -1, 524, 524, -1, -1, -1, -1, -1, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, 524, 524, 524, 524, 524, -1, 524, 524, -1, -1, 524, 524, 524, 524, 524, 524, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 528, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 528, -1, -1, 528, 528, 528, -1, -1, -1, 528, 528, -1, -1, -1, -1, -1, 528, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 528, 528, 528, 528, 528, 528, -1, 528, 528, -1, -1, 528, 528, 528, 528, 528, 528, 528, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 528, 528, 528, 528, 528, 528, 528, 528, 528, 528, 528, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 526, 527, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 534, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 534, -1, -1, 534, 534, 534, -1, -1, -1, 534, 534, -1, -1, -1, -1, -1, 534, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 534, 534, 534, 534, 534, 534, -1, 534, 534, -1, -1, 534, 534, 534, 534, 534, 534, 534, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, 534, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 530, 531, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, 533, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 538, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 538, -1, -1, 538, 538, 538, -1, -1, -1, 538, 538, -1, -1, -1, -1, -1, 538, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 538, 538, 538, 538, 538, 538, -1, 538, 538, -1, -1, 538, 538, 538, 538, 538, 538, 538, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 536, 537, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 542, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 542, -1, -1, 542, 542, 542, -1, -1, -1, 542, 542, -1, -1, -1, -1, -1, 542, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 542, 542, 542, 542, 542, 542, -1, 542, 542, -1, -1, 542, 542, 542, 542, 542, 542, 542, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, 542, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 540, 541, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, -1, -1, 547, 547, 547, -1, -1, -1, 547, 547, -1, -1, -1, -1, -1, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, 547, -1, 547, 547, -1, -1, 547, 547, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, 545, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 549, -1, -1, 549, 549, 549, -1, -1, -1, 549, 549, -1, -1, -1, -1, -1, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 549, 549, 549, 549, 549, 549, -1, 549, 549, -1, -1, 549, 549, 549, 549, 549, 549, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1689, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 566, -1, -1, 566, 566, 566, -1, -1, -1, 566, 566, -1, -1, -1, -1, -1, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 566, 566, 566, 566, 566, 566, -1, 565, 565, -1, -1, 565, 565, 565, 565, 565, 565, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 566, 566, 566, 566, 566, 566, 566, 566, 566, 566, 566, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1690, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 612, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 585, 585, -1, -1, 585, 585, 585, 585, 585, 585, 585, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 355, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 570, -1, -1, 567, 568, 571, -1, -1, -1, 569, 572, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 573, 574, 575, 576, 578, 580, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 584, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 762, 763, 764, 765, 766, 767, 768, 769, 770, 771, 772, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1692, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, 581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 210, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 583, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 795, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 760, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 760, 760, 760, 760, 760, 760, 760, 760, 760, 760, 760, 760, 760, 760, 760, 760, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 761, 1693, 761, 761, 761, 761, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 609, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 599, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 601, 602, -1, -1, 600, 603, 604, 606, 607, 608, 605, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 587, -1, -1, -1, -1, -1, -1, 592, 589, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 588, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 590, 591, -1, -1, -1, -1, 593, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 594, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 595, 596, 597, 598, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 610, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 616, -1, -1, 617, 618, 618, -1, -1, -1, -1, -1, -1, -1, -1, 616, -1, -1, 616, 616, 616, -1, -1, -1, 616, 616, -1, -1, -1, -1, -1, 1695, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 616, 616, 616, 616, 616, 616, -1, 616, 616, -1, -1, 616, 616, 616, 616, 616, 616, 616, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 616, 616, 616, 616, 616, 616, 616, 616, 616, 616, 616, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, -1, -1, 619, 619, 619, -1, -1, -1, 619, 619, -1, -1, -1, -1, -1, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, 619, 619, 619, 619, 619, -1, 619, 619, -1, -1, 619, 619, 619, 619, 619, 619, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, 619, 619, 619, 619, 619, 619, 619, 619, 619, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 628, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 628, -1, -1, 628, 628, 628, -1, -1, -1, 628, 628, -1, -1, -1, -1, -1, 628, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 628, 628, 628, 628, 628, 628, -1, 628, 628, -1, -1, 628, 628, 628, 628, 628, 628, 628, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 621, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 621, -1, -1, 621, 621, 621, -1, -1, -1, 621, 621, -1, -1, -1, -1, -1, 621, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 621, 621, 621, 621, 621, 621, -1, 621, 621, -1, -1, 621, 621, 621, 621, 621, 621, 621, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 621, 621, 621, 621, 621, 621, 621, 621, 621, 621, 621, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 622, -1, -1, 622, 622, 622, -1, -1, -1, 622, 622, -1, -1, -1, -1, -1, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 622, 622, 622, 622, 622, 622, -1, 622, 622, -1, -1, 622, 622, 622, 622, 622, 622, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 622, 622, 622, 622, 622, 622, 622, 622, 622, 622, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 624, 0, -1, -1, -1, 627, -1, -1, -1, -1, -1, -1, -1, -1, 624, -1, -1, 624, 624, 624, -1, -1, -1, 624, 624, -1, -1, -1, -1, -1, 624, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 624, 624, 624, 624, 624, 624, -1, 624, 624, -1, -1, 624, 624, 624, 624, 624, 624, 624, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, 624, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 630, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 630, -1, -1, 630, 630, 630, -1, -1, -1, 630, 630, -1, -1, -1, -1, -1, 630, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 630, 630, 630, 630, 630, 630, -1, 630, 630, -1, -1, 630, 630, 630, 630, 630, 630, 630, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 632, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 632, -1, -1, 632, 632, 632, -1, -1, -1, 632, 632, -1, -1, -1, -1, -1, 632, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 632, 632, 632, 632, 632, 632, -1, 632, 632, -1, -1, 632, 632, 632, 632, 632, 632, 632, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 632, 632, 632, 632, 632, 632, 632, 632, 632, 632, 632, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 634, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 634, -1, -1, 634, 634, 634, -1, -1, -1, 634, 634, -1, -1, -1, -1, -1, 634, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 634, 634, 634, 634, 634, 634, -1, 634, 634, -1, -1, 634, 634, 634, 634, 634, 634, 634, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 634, 634, 634, 634, 634, 634, 634, 634, 634, 634, 634, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 636, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 636, -1, -1, 636, 636, 636, -1, -1, -1, 636, 636, -1, -1, -1, -1, -1, 636, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 636, 636, 636, 636, 636, 636, -1, 636, 636, -1, -1, 636, 636, 636, 636, 636, 636, 636, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 636, 636, 636, 636, 636, 636, 636, 636, 636, 636, 636, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 638, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 638, -1, -1, 638, 638, 638, -1, -1, -1, 638, 638, -1, -1, -1, -1, -1, 638, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 638, 638, 638, 638, 638, 638, -1, 638, 638, -1, -1, 638, 638, 638, 638, 638, 638, 638, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 638, 638, 638, 638, 638, 638, 638, 638, 638, 638, 638, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 640, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 640, -1, -1, 640, 640, 640, -1, -1, -1, 640, 640, -1, -1, -1, -1, -1, 640, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 640, 640, 640, 640, 640, 640, -1, 640, 640, -1, -1, 640, 640, 640, 640, 640, 640, 640, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 640, 640, 640, 640, 640, 640, 640, 640, 640, 640, 640, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 642, -1, -1, 642, 642, 642, -1, -1, -1, 642, 642, -1, -1, -1, -1, -1, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 642, 642, 642, 642, 642, 642, -1, 642, 642, -1, -1, 642, 642, 642, 642, 642, 642, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 644, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 644, -1, -1, 644, 644, 644, -1, -1, -1, 644, 644, -1, -1, -1, -1, -1, 644, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 644, 644, 644, 644, 644, 644, -1, 644, 644, -1, -1, 644, 644, 644, 644, 644, 644, 644, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 644, 644, 644, 644, 644, 644, 644, 644, 644, 644, 644, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 646, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 646, -1, -1, 646, 646, 646, -1, -1, -1, 646, 646, -1, -1, -1, -1, -1, 646, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 646, 646, 646, 646, 646, 646, -1, 646, 646, -1, -1, 646, 646, 646, 646, 646, 646, 646, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 646, 646, 646, 646, 646, 646, 646, 646, 646, 646, 646, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 648, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 648, -1, -1, 648, 648, 648, -1, -1, -1, 648, 648, -1, -1, -1, -1, -1, 648, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 648, 648, 648, 648, 648, 648, -1, 648, 648, -1, -1, 648, 648, 648, 648, 648, 648, 648, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 648, 648, 648, 648, 648, 648, 648, 648, 648, 648, 648, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 650, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 650, -1, -1, 650, 650, 650, -1, -1, -1, 650, 650, -1, -1, -1, -1, -1, 650, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 650, 650, 650, 650, 650, 650, -1, 650, 650, -1, -1, 650, 650, 650, 650, 650, 650, 650, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 650, 650, 650, 650, 650, 650, 650, 650, 650, 650, 650, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 652, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 652, -1, -1, 652, 652, 652, -1, -1, -1, 652, 652, -1, -1, -1, -1, -1, 652, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 652, 652, 652, 652, 652, 652, -1, 652, 652, -1, -1, 652, 652, 652, 652, 652, 652, 652, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 652, 652, 652, 652, 652, 652, 652, 652, 652, 652, 652, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 655, -1, -1, 655, 655, 655, -1, -1, -1, 655, 655, -1, -1, -1, -1, -1, 654, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 655, 655, 655, 655, 655, 655, -1, 654, 654, -1, -1, 654, 654, 654, 654, 654, 654, 654, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 655, 655, 655, 655, 655, 655, 655, 655, 655, 655, 655, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 671, 671, -1, -1, 671, 671, 671, 671, 671, 671, 671, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 382, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 659, -1, -1, 656, 657, 660, -1, -1, -1, 658, 661, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 662, 663, 664, 665, 667, 668, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 667, 667, 667, 667, 667, 667, 667, 667, 667, 667, 667, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1697, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, 669, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 695, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 685, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 687, 688, -1, -1, 686, 689, 690, 692, 693, 694, 691, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 673, -1, -1, -1, -1, -1, -1, 678, 675, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 674, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 676, 677, -1, -1, -1, -1, 679, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 680, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 681, 682, 683, 684, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 697, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 700, 700, 700, 700, 700, 699, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 700, 
			-1, -1, -1, -1, -1, -1, 701, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 703, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 707, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 708, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 708, -1, -1, 708, 708, 708, -1, -1, -1, 708, 708, -1, -1, -1, -1, -1, 708, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 708, 708, 708, 708, 708, 708, -1, 708, 708, -1, -1, 708, 708, 708, 708, 708, 708, 708, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 708, 708, 708, 708, 708, 708, 708, 708, 708, 708, 708, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 709, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 711, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 715, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 716, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 717, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 721, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 722, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 723, 723, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 724, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 159, -1, 159, 159, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 159, -1, -1, 159, 159, 159, -1, -1, -1, 159, 159, -1, -1, -1, -1, -1, 159, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 159, 159, 159, 159, 159, 159, -1, 159, 159, -1, -1, 159, 159, 159, 159, 159, 159, 159, 159, -1, 159, 713, 714, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 726, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 269, 269, 269, 269, 5, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 1737, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 727, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 728, 
			-1, 272, 272, 272, 272, 5, 271, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 273, 272, 274, 272, 272, 275, 276, 277, 278, 279, 280, 280, 281, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 
			-1, -1, -1, -1, -1, -1, 730, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 738, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 740, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 741, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 742, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 743, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 744, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 745, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 746, 746, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 747, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 732, 
			-1, 1, 1, 737, 1, 1, 1, 1, 1, 1, 1, 735, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, -1, -1, -1, -1, -1, 749, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 287, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 287, -1, -1, 287, 287, 287, -1, -1, -1, 287, 287, -1, -1, -1, -1, -1, 287, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 287, 287, 287, 287, 287, 287, -1, 1698, 287, -1, -1, 287, 287, 287, 287, 287, 287, 287, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, 287, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 750, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 751, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 751, -1, -1, 751, 751, 751, -1, -1, -1, 751, 751, -1, -1, -1, -1, -1, 751, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 751, 751, 751, 751, 751, 751, -1, 751, 751, -1, -1, 751, 751, 751, 751, 751, 751, 751, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, 751, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 177, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 756, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 754, 755, 755, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 756, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 1699, 418, 418, 418, 418, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 420, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 775, 776, 777, 778, 779, 780, 781, 782, 783, 784, 785, 786, 787, 788, 789, 790, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 791, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 796, 797, 798, 799, 800, 801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 933, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 793, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 818, -1, -1, -1, 818, -1, -1, -1, -1, -1, -1, 818, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 818, 818, 818, 818, 818, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 818, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 818, 818, -1, -1, 818, 818, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 821, -1, -1, -1, 821, -1, -1, -1, -1, -1, -1, 821, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 821, 821, 821, 821, 821, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 821, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 821, 821, -1, -1, 821, 821, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 831, -1, -1, -1, -1, -1, -1, 826, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 838, 837, 827, 828, 830, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 829, 1702, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 877, -1, 862, -1, 855, 854, 853, 858, 860, 846, 847, 848, 849, 850, 878, 839, 840, -1, -1, -1, -1, -1, -1, 876, 875, 856, 857, 859, 861, 851, 852, 863, 864, 865, 866, 867, 868, 869, 870, 871, 872, 874, 841, 842, -1, -1, -1, -1, 845, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1704, 873, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 881, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 882, 883, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1706, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 907, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 891, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 891, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, 891, -1, -1, -1, -1, 891, 891, 891, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 894, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 893, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 893, 1708, 893, 893, 893, 893, 893, 893, 893, 893, 893, -1, -1, -1, -1, 894, 894, 895, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 896, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, 896, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 901, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 901, 901, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 906, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 906, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 915, 915, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 922, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 927, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 928, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 932, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 930, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 917, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 918, 918, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 918, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 921, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 924, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 934, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, -1, 457, -1, -1, -1, -1, -1, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, -1, -1, -1, 457, -1, -1, 457, 457, 457, 457, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, 457, 457, 457, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, 457, -1, -1, 457, 457, -1, 457, 457, 457, 457, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, -1, -1, -1, 156, -1, -1, -1, -1, -1, -1, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, 156, 156, 156, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, 156, -1, -1, 156, 156, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, -1, -1, -1, 156, -1, -1, -1, -1, -1, -1, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, 156, 156, 156, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, 156, -1, -1, 156, 156, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, -1, -1, -1, 156, -1, -1, -1, -1, -1, -1, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, 156, 156, 156, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 156, 156, -1, -1, 156, 156, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 460, -1, 0, -1, -1, -1, 460, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 460, -1, -1, -1, 460, -1, -1, 460, 460, 460, 460, 460, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 460, 460, 460, 460, 460, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, 460, -1, -1, 460, 460, -1, 460, 460, 460, 460, 
			-1, -1, -1, -1, -1, 462, -1, 0, -1, -1, -1, 462, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 462, -1, -1, -1, 462, -1, -1, -1, -1, -1, 462, 462, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 462, 462, 462, 462, 462, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, 462, -1, -1, 462, 462, -1, 462, 462, 462, 462, 
			-1, -1, -1, -1, -1, 0, -1, -1, 464, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 466, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 470, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 471, -1, -1, -1, 471, -1, -1, -1, -1, -1, -1, 471, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 471, 471, 471, 471, 471, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 471, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 471, 471, -1, -1, 471, 471, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 473, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, 473, 473, -1, -1, -1, -1, -1, 
			-1, -1, -1, 475, -1, 0, 0, -1, -1, 475, -1, 475, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 476, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 481, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 481, 481, -1, -1, -1, -1, -1, 
			0, -1, -1, 482, 0, 0, 0, 0, 0, 1710, 0, 482, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, 484, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 488, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, 490, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 493, 493, 493, 0, 0, 493, 0, 0, 493, 493, 493, 0, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 493, 
			-1, 495, 495, 495, 495, 495, 495, 0, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 495, 
			-1, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 0, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 497, 
			-1, 499, 499, 499, 0, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 499, 
			-1, -1, -1, -1, 0, -1, -1, -1, 505, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, -1, -1, 507, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, -1, -1, 508, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 513, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 515, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 517, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 519, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 521, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 523, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 525, 525, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 529, 529, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 529, 529, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 535, 535, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 539, 539, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 543, 543, 543, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 548, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, 0, 0, 0, -1, -1, 0, 550, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1501, 1501, 1501, 1501, 1501, 1501, 1501, 1501, 1501, 1501, 1501, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 577, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1503, 1503, 1503, 1503, 1503, 1503, 1503, 1503, 1503, 1503, 1503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, 579, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 582, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 586, 0, 0, -1, 0, 0, 0, 586, 586, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 586, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 586, 586, -1, -1, -1, -1, 586, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 611, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 613, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 615, -1, -1, 210, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 615, -1, -1, 615, 615, 615, -1, -1, -1, 615, 615, -1, -1, -1, -1, -1, 615, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 615, 615, 615, 615, 615, 615, -1, 615, 615, -1, -1, 615, 615, 615, 615, 615, 615, 615, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 615, 615, 615, 615, 615, 615, 615, 615, 615, 615, 615, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 614, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 623, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 625, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 626, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 629, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 631, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 633, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 635, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 637, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 639, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 641, 641, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 643, 643, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 643, 643, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 645, 645, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 647, 647, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 649, 649, 649, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 651, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 653, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 653, 653, 653, 653, 653, 653, 653, 653, 653, 653, 653, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 666, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1536, 1536, 1536, 1536, 1536, 1536, 1536, 1536, 1536, 1536, 1536, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 670, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 672, 0, 0, -1, -1, 0, 0, 672, 672, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 672, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 672, 672, -1, -1, -1, -1, 672, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 698, 698, 698, 698, 698, 698, 0, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 
			-1, -1, -1, 702, -1, 702, 702, 0, -1, -1, -1, 702, -1, -1, -1, -1, -1, 702, -1, -1, 702, 702, 702, -1, -1, -1, 702, 702, -1, -1, -1, 702, -1, 702, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 702, 702, 702, 702, 702, 702, -1, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, -1, 702, -1, -1, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, 702, -1, -1, 702, 702, -1, 702, 702, 702, 702, 
			-1, -1, -1, 0, -1, 704, 0, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, 0, -1, 1711, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, 0, 0, 0, 0, 
			-1, -1, -1, 705, -1, 705, 705, 0, -1, -1, -1, 705, -1, -1, -1, -1, -1, 705, -1, -1, 705, 705, 705, -1, -1, -1, 705, 705, -1, -1, -1, 705, -1, 705, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 705, 705, 705, 705, 705, 705, -1, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, -1, 705, -1, -1, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, 705, -1, -1, 705, 705, -1, 705, 705, 705, 705, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 706, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 710, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 712, -1, 712, 712, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 712, -1, -1, 712, 712, 712, -1, -1, -1, 712, 712, -1, -1, -1, -1, -1, 712, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 712, 712, 712, 712, 712, 712, -1, 712, 712, -1, -1, 712, 712, 712, 712, 712, 712, 712, 712, -1, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, 712, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 718, -1, 718, -1, -1, 718, -1, -1, 154, -1, -1, -1, -1, -1, 718, -1, -1, 718, 718, 718, -1, -1, -1, 718, 718, -1, -1, -1, -1, -1, 1712, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 718, 718, 718, 718, 718, 718, -1, 718, 718, -1, -1, 718, 718, 718, 718, 718, 718, 718, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1713, 1715, 1717, 1719, 1721, 1723, 1725, 1727, 1729, 1731, 1733, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 719, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 719, -1, -1, 719, 719, 719, -1, -1, -1, 719, 719, -1, -1, -1, -1, -1, 719, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 719, 719, 719, 719, 719, 719, -1, 719, 719, -1, -1, 719, 719, 719, 719, 719, 719, 719, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 719, 719, 719, 719, 719, 719, 719, 719, 719, 719, 719, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 720, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 720, -1, -1, 720, 720, 720, -1, -1, -1, 720, 720, -1, -1, -1, -1, -1, 720, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 720, 720, 720, 720, 720, 720, -1, 720, 720, -1, -1, 720, 720, 720, 720, 720, 720, 720, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, 720, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1578, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1578, -1, -1, 1578, 1578, 1578, -1, -1, -1, 1578, 1578, -1, -1, -1, -1, -1, 1578, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1578, 1578, 1578, 1578, 1578, 1578, -1, 1578, 1578, -1, -1, 1578, 1578, 1578, 1578, 1578, 1578, 1578, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1578, 1578, 1578, 1578, 1578, 1578, 1578, 1578, 1578, 1578, 1578, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1579, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1579, -1, -1, 1579, 1579, 1579, -1, -1, -1, 1579, 1579, -1, -1, -1, -1, -1, 1579, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1579, 1579, 1579, 1579, 1579, 1579, -1, 1579, 1579, -1, -1, 1579, 1579, 1579, 1579, 1579, 1579, 1579, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1579, 1579, 1579, 1579, 1579, 1579, 1579, 1579, 1579, 1579, 1579, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1580, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1581, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1581, -1, -1, 1581, 1581, 1581, -1, -1, -1, 1581, 1581, -1, -1, -1, -1, -1, 1581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1581, 1581, 1581, 1581, 1581, 1581, -1, 1581, 1581, -1, -1, 1581, 1581, 1581, 1581, 1581, 1581, 1581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1581, 1581, 1581, 1581, 1581, 1581, 1581, 1581, 1581, 1581, 1581, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 725, 725, 725, 725, 725, 725, 0, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 725, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 268, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 1, 1, 1, 1, 1, 271, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, 729, 729, 729, 729, 729, 729, 0, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 729, 
			-1, 733, 733, 733, 0, 0, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 
			-1, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 0, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 734, 
			-1, 736, 736, 736, 0, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 736, 
			-1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, -1, 739, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 282, 282, 282, 282, 0, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 
			-1, 282, 282, 282, 282, 0, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 
			-1, 282, 282, 282, 0, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 
			-1, 282, 282, 282, 282, 0, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 
			-1, 282, 282, 282, 282, 0, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 81, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 282, 282, 282, 282, 0, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 
			-1, -1, -1, 748, -1, 748, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 748, -1, -1, 748, 748, 748, -1, -1, -1, 748, 748, -1, -1, -1, -1, -1, 748, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 748, 748, 748, 748, 748, 748, -1, 748, 748, -1, -1, 748, 748, 748, 748, 748, 748, 748, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, 748, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 286, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, 753, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 1735, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 753, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, 757, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 1736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 759, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 792, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 794, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, 0, -1, 817, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 819, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 820, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, 820, 820, -1, -1, -1, -1, -1, 
			-1, -1, -1, 822, -1, 0, 0, -1, 0, 822, -1, 822, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 823, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, -1, 0, 0, 825, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 135, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 880, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 886, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 892, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 897, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, 897, 897, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 898, -1, -1, -1, -1, -1, -1, 898, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 898, 898, 898, 898, 898, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 898, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 898, 898, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 899, 0, -1, -1, -1, 0, 899, -1, 899, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 900, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 902, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, 902, 902, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, -1, -1, -1, 0, 0, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 903, -1, -1, -1, -1, -1, -1, 903, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 903, 903, 903, 903, 903, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 903, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 903, 903, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 904, 0, -1, -1, -1, 0, 904, -1, 904, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, 905, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1607, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 140, 1615, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1616, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 916, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 919, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, 920, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1620, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1621, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 923, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1625, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1626, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, 929, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, 931, -1, 0, 0, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, 0, 0, 0, 0, 
			0, -1, -1, 0, -1, 0, 1630, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1631, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 935, -1, 0, -1, -1, -1, 935, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 935, -1, -1, -1, 935, -1, -1, -1, -1, -1, 935, 935, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 935, 935, 935, 935, 935, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, 935, -1, -1, 935, 935, -1, 935, 935, 935, 935, 
			-1, -1, 146, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			
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
			316, 145,  -1, // 0
			936, 4, 1465, 3, 28, 2,  -1, // 1
			939, 7, 320, 6, 938, 1466, 29,  -1, // 2
			320, 145,  -1, // 3
			941, 7, 321, 6, 940, 1467, 30,  -1, // 4
			321, 157,  -1, // 5
			5, 322, 160, 31,  -1, // 6
			322, 160, 8,  -1, // 7
			5, 323, 160, 32,  -1, // 8
			323, 160, 8,  -1, // 9
			942, 9, 1468,  -1, // 10
			1311, 162,  -1, // 11
			324, 943, 1469,  -1, // 12
			324, 944, 1470, 10,  -1, // 13
			1312, 164,  -1, // 14
			1313, 165, 325, 1471,  -1, // 15
			326, 1314, 166,  -1, // 16
			1319, 165, 945, 328, 327, 1316, 168, 1315, 167, 326,  -1, // 17
			327, 1317, 169,  -1, // 18
			328, 1318, 170,  -1, // 19
			3, 1320, 167,  -1, // 20
			946, 5,  -1, // 21
			947, 1472,  -1, // 22
			949, 330, 329, 948, 1473,  -1, // 23
			329, 1321, 166,  -1, // 24
			330, 1322, 169,  -1, // 25
			331, 950, 1474,  -1, // 26
			331, 951, 1475, 8,  -1, // 27
			952, 1476,  -1, // 28
			953, 1477,  -1, // 29
			12, 178, 954, 11,  -1, // 30
			332, 179, 8,  -1, // 31
			332, 179,  -1, // 32
			180, 13,  -1, // 33
			955, 333, 1478,  -1, // 34
			334, 1323, 181,  -1, // 35
			334, 1323, 181,  -1, // 36
			957, 1480, 335, 1324, 181, 956, 1479,  -1, // 37
			335, 1324, 181,  -1, // 38
			959, 1482, 336, 1325, 181, 958, 1481,  -1, // 39
			336, 1325, 181,  -1, // 40
			961, 1484, 337, 960, 1483,  -1, // 41
			337, 1326, 181,  -1, // 42
			962, 1485,  -1, // 43
			963, 1486,  -1, // 44
			1327, 184,  -1, // 45
			1328, 184,  -1, // 46
			338, 964, 1487,  -1, // 47
			338, 965, 1488, 8,  -1, // 48
			339, 966, 1489,  -1, // 49
			339, 967, 1490, 8,  -1, // 50
			340, 969, 1491, 8,  -1, // 51
			340, 969, 1491, 8, 968,  -1, // 52
			970, 1329, 184,  -1, // 53
			971, 1492,  -1, // 54
			341, 1330, 190,  -1, // 55
			974, 1332, 184, 9, 973, 1331, 184, 14, 972,  -1, // 56
			342, 1333, 191,  -1, // 57
			342, 975, 1334, 191, 34,  -1, // 58
			343, 1335, 192,  -1, // 59
			343, 976, 1336, 192, 35,  -1, // 60
			344, 1337, 193,  -1, // 61
			344, 977, 1338, 193, 15,  -1, // 62
			345, 1339, 194,  -1, // 63
			345, 978, 1340, 194, 16,  -1, // 64
			346, 1341, 195,  -1, // 65
			346, 979, 1342, 195, 17,  -1, // 66
			347, 1343, 196,  -1, // 67
			347, 980, 1344, 196, 1493,  -1, // 68
			981, 36,  -1, // 69
			982, 37,  -1, // 70
			348, 1345, 198,  -1, // 71
			348, 983, 1346, 198, 1494,  -1, // 72
			984, 18,  -1, // 73
			985, 19,  -1, // 74
			986, 38,  -1, // 75
			987, 39,  -1, // 76
			349, 1347, 200,  -1, // 77
			349, 988, 1348, 200, 1495,  -1, // 78
			989, 40,  -1, // 79
			990, 41,  -1, // 80
			350, 1349, 202,  -1, // 81
			350, 991, 1350, 202, 1496,  -1, // 82
			992, 20,  -1, // 83
			993, 21,  -1, // 84
			351, 1351, 204,  -1, // 85
			351, 994, 1352, 204, 1497,  -1, // 86
			995, 22,  -1, // 87
			996, 23,  -1, // 88
			997, 24,  -1, // 89
			352, 1353, 206,  -1, // 90
			352, 998, 1354, 206, 25,  -1, // 91
			353, 1355, 207,  -1, // 92
			1356, 208,  -1, // 93
			999, 1357, 184, 1498,  -1, // 94
			210, 1000, 13,  -1, // 95
			1001, 13,  -1, // 96
			1002, 42,  -1, // 97
			1003, 43,  -1, // 98
			1004, 44,  -1, // 99
			1005, 45,  -1, // 100
			1006, 46,  -1, // 101
			1007, 47,  -1, // 102
			1008, 48,  -1, // 103
			1009, 49,  -1, // 104
			1010, 50,  -1, // 105
			1011, 51,  -1, // 106
			1012, 52,  -1, // 107
			1358, 211,  -1, // 108
			1359, 212,  -1, // 109
			1013, 1360, 207, 20,  -1, // 110
			1014, 1361, 207, 21,  -1, // 111
			1015, 1362, 207, 26,  -1, // 112
			1016, 1363, 207, 17,  -1, // 113
			1017, 1364, 207, 22,  -1, // 114
			1018, 1365, 207, 27,  -1, // 115
			1019, 1366, 207, 53,  -1, // 116
			1020, 1367, 207, 54,  -1, // 117
			1021, 4, 1499, 3, 55,  -1, // 118
			1022, 4, 1500, 3, 56,  -1, // 119
			1023, 1368, 207, 4, 1502, 354, 3,  -1, // 120
			357, 1024, 1504, 57, 356,  -1, // 121
			1369, 216,  -1, // 122
			1025, 1370, 207, 58,  -1, // 123
			1026, 1505,  -1, // 124
			1027, 4, 1507, 3, 1506,  -1, // 125
			3, 160,  -1, // 126
			1371, 219, 359, 3,  -1, // 127
			360, 1372, 220,  -1, // 128
			360, 1373, 221,  -1, // 129
			1028, 4, 1508, 3,  -1, // 130
			1029, 4, 1509, 3, 26,  -1, // 131
			1030, 12, 1510, 11,  -1, // 132
			1031, 53,  -1, // 133
			1032, 54,  -1, // 134
			1374, 222, 10,  -1, // 135
			1375, 222, 1033, 59,  -1, // 136
			1034, 1511,  -1, // 137
			1035, 60,  -1, // 138
			1036, 61,  -1, // 139
			1037, 62,  -1, // 140
			1038, 63,  -1, // 141
			1039, 1512,  -1, // 142
			1040, 64,  -1, // 143
			1041, 60,  -1, // 144
			1042, 61,  -1, // 145
			1043, 1513,  -1, // 146
			1044, 1514,  -1, // 147
			1045, 1515,  -1, // 148
			1046, 67,  -1, // 149
			1047, 68,  -1, // 150
			1048, 69,  -1, // 151
			4, 1376, 184, 3,  -1, // 152
			361, 1049, 1516,  -1, // 153
			361, 1050, 1517,  -1, // 154
			1051, 7, 362, 1377, 224, 6,  -1, // 155
			362, 1378, 224, 8,  -1, // 156
			363, 1053, 13, 1518,  -1, // 157
			1054, 1519,  -1, // 158
			1056, 1520, 1055,  -1, // 159
			210, 1057,  -1, // 160
			1058, 0,  -1, // 161
			1059, 1521,  -1, // 162
			1379, 226,  -1, // 163
			1380, 226,  -1, // 164
			365, 1060, 1522,  -1, // 165
			365, 1061, 1523, 8,  -1, // 166
			366, 1062, 1524,  -1, // 167
			366, 1063, 1525, 8,  -1, // 168
			367, 1065, 1526, 8,  -1, // 169
			367, 1065, 1526, 8, 1064,  -1, // 170
			368, 1381, 230,  -1, // 171
			1066, 1383, 226, 9, 1382, 226, 14,  -1, // 172
			369, 1384, 231,  -1, // 173
			369, 1067, 1385, 231, 34,  -1, // 174
			370, 1386, 232,  -1, // 175
			370, 1068, 1387, 232, 35,  -1, // 176
			371, 1388, 233,  -1, // 177
			371, 1069, 1389, 233, 15,  -1, // 178
			372, 1390, 234,  -1, // 179
			372, 1070, 1391, 234, 16,  -1, // 180
			373, 1392, 235,  -1, // 181
			373, 1071, 1393, 235, 17,  -1, // 182
			374, 1394, 236,  -1, // 183
			374, 1072, 1395, 236, 1527,  -1, // 184
			375, 1396, 237,  -1, // 185
			375, 1073, 1397, 237, 1528,  -1, // 186
			376, 1398, 238,  -1, // 187
			376, 1074, 1399, 238, 1529,  -1, // 188
			377, 1400, 239,  -1, // 189
			377, 1075, 1401, 239, 1530,  -1, // 190
			378, 1402, 240,  -1, // 191
			378, 1076, 1403, 240, 1531,  -1, // 192
			379, 1404, 241,  -1, // 193
			379, 1077, 1405, 241, 25,  -1, // 194
			380, 1406, 242,  -1, // 195
			1079, 1407, 226, 1532, 1078,  -1, // 196
			1408, 243,  -1, // 197
			1409, 244,  -1, // 198
			1080, 1410, 242, 20,  -1, // 199
			1081, 1411, 242, 21,  -1, // 200
			1082, 1412, 242, 26,  -1, // 201
			1083, 1413, 242, 17,  -1, // 202
			1084, 1414, 242, 22,  -1, // 203
			1085, 1415, 242, 27,  -1, // 204
			1086, 1416, 242, 53,  -1, // 205
			1087, 1417, 242, 54,  -1, // 206
			1088, 4, 1533, 3, 55,  -1, // 207
			1089, 4, 1534, 3, 56,  -1, // 208
			1090, 1418, 242, 4, 1535, 381, 3,  -1, // 209
			1091, 1537, 57, 383,  -1, // 210
			1092, 1419, 242, 58,  -1, // 211
			1093, 1538,  -1, // 212
			1094, 4, 229, 3, 1539,  -1, // 213
			385, 1420, 246,  -1, // 214
			385, 1421, 247,  -1, // 215
			1095, 4, 1540, 3,  -1, // 216
			1096, 4, 1541, 3, 26,  -1, // 217
			1097, 12, 1542, 11,  -1, // 218
			1098, 53,  -1, // 219
			1099, 54,  -1, // 220
			1422, 248, 10,  -1, // 221
			1423, 248, 1100, 59,  -1, // 222
			1101, 1543,  -1, // 223
			1102, 60,  -1, // 224
			1103, 61,  -1, // 225
			1104, 62,  -1, // 226
			1105, 63,  -1, // 227
			1106, 1544,  -1, // 228
			1107, 64,  -1, // 229
			1108, 60,  -1, // 230
			1109, 61,  -1, // 231
			1110, 1545,  -1, // 232
			1111, 66,  -1, // 233
			1112, 1546,  -1, // 234
			1113, 67,  -1, // 235
			1114, 68,  -1, // 236
			1115, 69,  -1, // 237
			4, 1424, 226, 3,  -1, // 238
			1425, 249,  -1, // 239
			1117, 1548, 386, 1116, 1547,  -1, // 240
			386, 1426, 250,  -1, // 241
			1427, 249,  -1, // 242
			1118, 1549,  -1, // 243
			1120, 1551, 387, 1119, 1550,  -1, // 244
			387, 158,  -1, // 245
			1123, 1553, 389, 1122, 388, 1121, 1552,  -1, // 246
			388, 253,  -1, // 247
			389, 158,  -1, // 248
			1124, 4, 1555, 3, 1554,  -1, // 249
			3, 218,  -1, // 250
			5, 183,  -1, // 251
			1127, 391, 1558, 1125, 1557, 1556, 3, 71,  -1, // 252
			1560, 1126, 1559,  -1, // 253
			1129, 1563, 392, 1128, 1562, 4, 1561, 3, 73,  -1, // 254
			392, 1428, 265,  -1, // 255
			1130, 1566, 1565, 1564,  -1, // 256
			1131, 1568, 1567,  -1, // 257
			1133, 1571, 1132, 1570, 1569, 3, 76,  -1, // 258
			1136, 5, 4, 1574, 3, 76, 1135, 1573, 1134, 1572,  -1, // 259
			1143, 1577, 1142, 4, 395, 5, 394, 393, 1137, 3, 1575,  -1, // 260
			5, 187,  -1, // 261
			1139, 1576, 1138,  -1, // 262
			1141, 186, 1140,  -1, // 263
			1144, 5, 396, 79,  -1, // 264
			1145, 5, 397, 80,  -1, // 265
			1146, 5, 399, 82, 398,  -1, // 266
			1148, 1583, 1147, 1582,  -1, // 267
			400, 267,  -1, // 268
			1149, 7, 400, 267, 6,  -1, // 269
			1152, 159, 1151, 4, 186, 3, 60,  -1, // 270
			1153, 5, 1584,  -1, // 271
			403, 402,  -1, // 272
			7, 403, 6,  -1, // 273
			5, 282,  -1, // 274
			404, 283,  -1, // 275
			404, 283,  -1, // 276
			405, 283,  -1, // 277
			12, 405, 283, 11,  -1, // 278
			406, 283,  -1, // 279
			4, 406, 3,  -1, // 280
			407, 270, 4, 282, 3, 71,  -1, // 281
			270, 72,  -1, // 282
			271, 4, 282, 3, 73,  -1, // 283
			270, 4, 282, 3, 76,  -1, // 284
			5, 4, 282, 3, 76, 270, 77,  -1, // 285
			270, 4, 410, 5, 409, 5, 408, 3, 78,  -1, // 286
			5, 411, 79,  -1, // 287
			5, 412, 80,  -1, // 288
			5, 414, 82, 413,  -1, // 289
			270, 83,  -1, // 290
			415, 285,  -1, // 291
			7, 415, 285, 6,  -1, // 292
			270, 1155, 4, 228, 3, 60,  -1, // 293
			1156, 5, 176,  -1, // 294
			417, 1429, 288,  -1, // 295
			417, 1429, 288,  -1, // 296
			1157, 1585,  -1, // 297
			1158, 1586,  -1, // 298
			1159, 1587,  -1, // 299
			1161, 1588,  -1, // 300
			419, 1430, 219,  -1, // 301
			419, 1430, 219,  -1, // 302
			1162, 1589,  -1, // 303
			1163, 1590,  -1, // 304
			1164, 84,  -1, // 305
			1165, 85,  -1, // 306
			1166, 86,  -1, // 307
			1167, 87,  -1, // 308
			1168, 88,  -1, // 309
			1169, 89,  -1, // 310
			1170, 90,  -1, // 311
			1171, 91,  -1, // 312
			1172, 92,  -1, // 313
			1173, 93,  -1, // 314
			1174, 94,  -1, // 315
			1175, 95,  -1, // 316
			1176, 96,  -1, // 317
			1177, 97,  -1, // 318
			1178, 98,  -1, // 319
			1179, 99,  -1, // 320
			1180, 100,  -1, // 321
			1181, 101,  -1, // 322
			1182, 102,  -1, // 323
			1183, 103,  -1, // 324
			1184, 104,  -1, // 325
			1185, 105,  -1, // 326
			1186, 106,  -1, // 327
			1187, 107,  -1, // 328
			1188, 108,  -1, // 329
			1189, 109,  -1, // 330
			1190, 110,  -1, // 331
			1191, 111,  -1, // 332
			1192, 112,  -1, // 333
			1193, 1591,  -1, // 334
			1194, 1592,  -1, // 335
			421, 1196, 1593, 1195,  -1, // 336
			1197, 3,  -1, // 337
			1198, 1594,  -1, // 338
			1199, 113,  -1, // 339
			1200, 114,  -1, // 340
			1201, 115,  -1, // 341
			1202, 116,  -1, // 342
			1203, 117,  -1, // 343
			1204, 118,  -1, // 344
			1205, 119,  -1, // 345
			1206, 120,  -1, // 346
			1207, 121,  -1, // 347
			1208, 122,  -1, // 348
			1209, 123,  -1, // 349
			1210, 124,  -1, // 350
			1211, 125,  -1, // 351
			1212, 126,  -1, // 352
			1213, 127,  -1, // 353
			1214, 128,  -1, // 354
			1215, 129,  -1, // 355
			1216, 130,  -1, // 356
			1217, 131,  -1, // 357
			1218, 132,  -1, // 358
			422, 1431, 293,  -1, // 359
			422, 1432, 293, 8,  -1, // 360
			1219, 423, 1433, 1595,  -1, // 361
			1596, 13,  -1, // 362
			424, 1434, 166,  -1, // 363
			426, 425, 1435, 167, 424,  -1, // 364
			425, 1436, 169,  -1, // 365
			426, 1437, 170,  -1, // 366
			427, 1220, 1438, 1597,  -1, // 367
			427, 1439, 295, 10,  -1, // 368
			1221, 1598,  -1, // 369
			1222, 62,  -1, // 370
			1223, 63,  -1, // 371
			1224, 133,  -1, // 372
			1225, 64,  -1, // 373
			1226, 64, 26,  -1, // 374
			1227, 64, 86,  -1, // 375
			1228, 64, 26, 86,  -1, // 376
			1229, 134,  -1, // 377
			1230, 1599, 134,  -1, // 378
			1231, 1600, 134,  -1, // 379
			1232, 61,  -1, // 380
			1233, 60,  -1, // 381
			1234, 26,  -1, // 382
			1235, 27,  -1, // 383
			1236, 53,  -1, // 384
			1237, 54,  -1, // 385
			1238, 53, 135,  -1, // 386
			1239, 54, 135,  -1, // 387
			1240, 59, 428,  -1, // 388
			1241, 20,  -1, // 389
			1242, 21,  -1, // 390
			1243, 22,  -1, // 391
			1244, 23,  -1, // 392
			1245, 24,  -1, // 393
			1246, 40,  -1, // 394
			1247, 41,  -1, // 395
			1248, 17,  -1, // 396
			1249, 16,  -1, // 397
			1250, 15,  -1, // 398
			1251, 36,  -1, // 399
			1252, 37,  -1, // 400
			1253, 18,  -1, // 401
			1254, 38,  -1, // 402
			1255, 19,  -1, // 403
			1256, 39,  -1, // 404
			1257, 13,  -1, // 405
			1258, 42,  -1, // 406
			1259, 43,  -1, // 407
			1260, 44,  -1, // 408
			1261, 45,  -1, // 409
			1262, 46,  -1, // 410
			1263, 47,  -1, // 411
			1264, 48,  -1, // 412
			1265, 49,  -1, // 413
			1266, 50,  -1, // 414
			1267, 51,  -1, // 415
			1268, 136,  -1, // 416
			1269, 52,  -1, // 417
			1270, 35,  -1, // 418
			1271, 34,  -1, // 419
			1272, 12, 11,  -1, // 420
			1273, 25,  -1, // 421
			429, 1274, 1601,  -1, // 422
			429, 1275, 1602,  -1, // 423
			1276, 22,  -1, // 424
			1277, 137,  -1, // 425
			1278, 138,  -1, // 426
			1440, 298,  -1, // 427
			1441, 168,  -1, // 428
			1442, 299,  -1, // 429
			1280, 12, 1603, 11,  -1, // 430
			1281, 12, 11,  -1, // 431
			4, 1443, 300, 3,  -1, // 432
			4, 3,  -1, // 433
			431, 1444, 301,  -1, // 434
			431, 1445, 301, 8,  -1, // 435
			1446, 302,  -1, // 436
			1447, 303,  -1, // 437
			1448, 304,  -1, // 438
			1283, 435, 434, 433, 432, 1282, 1604,  -1, // 439
			432, 1449, 166,  -1, // 440
			1450, 167,  -1, // 441
			434, 1451, 169,  -1, // 442
			1605, 13,  -1, // 443
			1284, 439, 438, 437, 436, 1452, 166,  -1, // 444
			436, 1452, 166,  -1, // 445
			1453, 167,  -1, // 446
			438, 1454, 169,  -1, // 447
			1606, 13,  -1, // 448
			1285, 139, 440,  -1, // 449
			1286, 1608, 9,  -1, // 450
			1287, 117,  -1, // 451
			1288, 1609,  -1, // 452
			1289, 1610,  -1, // 453
			1290, 1611,  -1, // 454
			1291, 1612,  -1, // 455
			1292, 1613,  -1, // 456
			1293, 1614,  -1, // 457
			1455, 311, 1294, 443, 442, 441,  -1, // 458
			1617, 9,  -1, // 459
			1296, 7, 1456, 312, 1295, 6,  -1, // 460
			444, 1457, 313,  -1, // 461
			444, 1458, 313, 8,  -1, // 462
			1619, 13,  -1, // 463
			1297, 445, 1618,  -1, // 464
			1459, 315, 1298, 448, 447, 446, 142,  -1, // 465
			1622, 9,  -1, // 466
			1299, 4, 1623, 3, 28,  -1, // 467
			1300, 1624,  -1, // 468
			1301, 75,  -1, // 469
			1460, 315, 1302, 449, 143,  -1, // 470
			1461, 315, 1303, 451, 1627, 450, 126,  -1, // 471
			1628, 9,  -1, // 472
			1304, 453, 452, 1629, 126, 129,  -1, // 473
			1462, 168,  -1, // 474
			1306, 1463, 315, 1305, 1632, 454, 128,  -1, // 475
			1308, 1464, 315, 1307, 128,  -1, // 476
			1310, 7, 455, 1309, 6,  -1, // 477
			455, 157,  -1, // 478
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 10, 18, 21, 29, 32, 37, 41, 46, 50, 54, 57, 61, 66, 69, 74, 78, 89, 93, 97, 101, 104, 107, 113, 117, 121, 125, 130, 133, 136, 141, 145, 148, 151, 155, 159, 163, 171, 175, 183, 187, 193, 197, 200, 203, 206, 209, 213, 218, 222, 227, 232, 238, 242, 245, 249, 259, 263, 269, 273, 279, 283, 289, 293, 299, 303, 309, 313, 319, 322, 325, 329, 335, 338, 341, 344, 347, 351, 357, 360, 363, 367, 373, 376, 379, 383, 389, 392, 395, 398, 402, 408, 412, 415, 420, 424, 427, 430, 433, 436, 439, 442, 445, 448, 451, 454, 457, 460, 463, 466, 471, 476, 481, 486, 491, 496, 501, 506, 512, 518, 526, 532, 535, 540, 543, 549, 552, 557, 561, 565, 570, 576, 581, 584, 587, 591, 596, 599, 602, 605, 608, 611, 614, 617, 620, 623, 626, 629, 632, 635, 638, 641, 646, 650, 654, 661, 666, 671, 674, 678, 681, 684, 687, 690, 693, 697, 702, 706, 711, 716, 722, 726, 734, 738, 744, 748, 754, 758, 764, 768, 774, 778, 784, 788, 794, 798, 804, 808, 814, 818, 824, 828, 834, 838, 844, 848, 854, 857, 860, 865, 870, 875, 880, 885, 890, 895, 900, 906, 912, 920, 925, 930, 933, 939, 943, 947, 952, 958, 963, 966, 969, 973, 978, 981, 984, 987, 990, 993, 996, 999, 1002, 1005, 1008, 1011, 1014, 1017, 1020, 1023, 1028, 1031, 1037, 1041, 1044, 1047, 1053, 1056, 1064, 1067, 1070, 1076, 1079, 1082, 1091, 1095, 1105, 1109, 1114, 1118, 1126, 1137, 1149, 1152, 1156, 1160, 1165, 1170, 1176, 1181, 1184, 1190, 1198, 1202, 1205, 1209, 1212, 1215, 1218, 1221, 1226, 1229, 1233, 1240, 1243, 1249, 1255, 1263, 1273, 1277, 1281, 1286, 1289, 1292, 1297, 1304, 1308, 1312, 1316, 1319, 1322, 1325, 1328, 1332, 1336, 1339, 1342, 1345, 1348, 1351, 1354, 1357, 1360, 1363, 1366, 1369, 1372, 1375, 1378, 1381, 1384, 1387, 1390, 1393, 1396, 1399, 1402, 1405, 1408, 1411, 1414, 1417, 1420, 1423, 1426, 1429, 1432, 1435, 1440, 1443, 1446, 1449, 1452, 1455, 1458, 1461, 1464, 1467, 1470, 1473, 1476, 1479, 1482, 1485, 1488, 1491, 1494, 1497, 1500, 1503, 1506, 1510, 1515, 1520, 1523, 1527, 1533, 1537, 1541, 1546, 1551, 1554, 1557, 1560, 1563, 1566, 1570, 1574, 1579, 1582, 1586, 1590, 1593, 1596, 1599, 1602, 1605, 1608, 1612, 1616, 1620, 1623, 1626, 1629, 1632, 1635, 1638, 1641, 1644, 1647, 1650, 1653, 1656, 1659, 1662, 1665, 1668, 1671, 1674, 1677, 1680, 1683, 1686, 1689, 1692, 1695, 1698, 1701, 1704, 1707, 1710, 1713, 1717, 1720, 1724, 1728, 1731, 1734, 1737, 1740, 1743, 1746, 1751, 1755, 1760, 1763, 1767, 1772, 1775, 1778, 1781, 1789, 1793, 1796, 1800, 1803, 1811, 1815, 1818, 1822, 1825, 1829, 1833, 1836, 1839, 1842, 1845, 1848, 1851, 1854, 1861, 1864, 1871, 1875, 1880, 1883, 1887, 1895, 1898, 1904, 1907, 1910, 1916, 1924, 1927, 1934, 1937, 1945, 1951, 1957, 
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
		
		case EToken_AddAssign:
			return 42;
		
		case EToken_SubAssign:
			return 43;
		
		case EToken_MulAssign:
			return 44;
		
		case EToken_DivAssign:
			return 45;
		
		case EToken_ModAssign:
			return 46;
		
		case EToken_ShlAssign:
			return 47;
		
		case EToken_ShrAssign:
			return 48;
		
		case EToken_AndAssign:
			return 49;
		
		case EToken_XorAssign:
			return 50;
		
		case EToken_OrAssign:
			return 51;
		
		case EToken_ClassAssign:
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
		
		case EToken_Ptr:
			return 59;
		
		case EToken_OnChange:
			return 60;
		
		case EToken_PropValue:
			return 61;
		
		case EToken_Get:
			return 62;
		
		case EToken_Set:
			return 63;
		
		case EToken_This:
			return 64;
		
		case EToken_Integer:
			return 65;
		
		case EToken_Fp:
			return 66;
		
		case EToken_True:
			return 67;
		
		case EToken_False:
			return 68;
		
		case EToken_Null:
			return 69;
		
		case EToken_Literal:
			return 70;
		
		case EToken_If:
			return 71;
		
		case EToken_Else:
			return 72;
		
		case EToken_Switch:
			return 73;
		
		case EToken_Case:
			return 74;
		
		case EToken_Default:
			return 75;
		
		case EToken_While:
			return 76;
		
		case EToken_Do:
			return 77;
		
		case EToken_For:
			return 78;
		
		case EToken_Break:
			return 79;
		
		case EToken_Continue:
			return 80;
		
		case EToken_Silent:
			return 81;
		
		case EToken_Return:
			return 82;
		
		case EToken_Once:
			return 83;
		
		case EToken_Typedef:
			return 84;
		
		case EToken_Alias:
			return 85;
		
		case EToken_Static:
			return 86;
		
		case EToken_Thread:
			return 87;
		
		case EToken_Stack:
			return 88;
		
		case EToken_Heap:
			return 89;
		
		case EToken_UHeap:
			return 90;
		
		case EToken_Member:
			return 91;
		
		case EToken_Abstract:
			return 92;
		
		case EToken_Virtual:
			return 93;
		
		case EToken_Override:
			return 94;
		
		case EToken_Public:
			return 95;
		
		case EToken_Protected:
			return 96;
		
		case EToken_Void:
			return 97;
		
		case EToken_Variant:
			return 98;
		
		case EToken_Object:
			return 99;
		
		case EToken_Bool:
			return 100;
		
		case EToken_Int8:
			return 101;
		
		case EToken_Int16:
			return 102;
		
		case EToken_Int32:
			return 103;
		
		case EToken_Int64:
			return 104;
		
		case EToken_Int:
			return 105;
		
		case EToken_IntPtr:
			return 106;
		
		case EToken_Char:
			return 107;
		
		case EToken_Short:
			return 108;
		
		case EToken_Long:
			return 109;
		
		case EToken_Float:
			return 110;
		
		case EToken_Double:
			return 111;
		
		case EToken_Auto:
			return 112;
		
		case EToken_Signed:
			return 113;
		
		case EToken_Unsigned:
			return 114;
		
		case EToken_BigEndian:
			return 115;
		
		case EToken_Nullable:
			return 116;
		
		case EToken_Const:
			return 117;
		
		case EToken_ReadOnly:
			return 118;
		
		case EToken_Mutable:
			return 119;
		
		case EToken_Volatile:
			return 120;
		
		case EToken_Weak:
			return 121;
		
		case EToken_Thin:
			return 122;
		
		case EToken_Unsafe:
			return 123;
		
		case EToken_Cdecl:
			return 124;
		
		case EToken_Stdcall:
			return 125;
		
		case EToken_Class:
			return 126;
		
		case EToken_Function:
			return 127;
		
		case EToken_Property:
			return 128;
		
		case EToken_AutoEv:
			return 129;
		
		case EToken_Bindable:
			return 130;
		
		case EToken_AutoGet:
			return 131;
		
		case EToken_Indexed:
			return 132;
		
		case EToken_PreConstruct:
			return 133;
		
		case EToken_Operator:
			return 134;
		
		case EToken_Postfix:
			return 135;
		
		case EToken_AtAssign:
			return 136;
		
		case EToken_Multicast:
			return 137;
		
		case EToken_Event:
			return 138;
		
		case EToken_Ellipsis:
			return 139;
		
		case EToken_Enum:
			return 140;
		
		case EToken_EnumC:
			return 141;
		
		case EToken_Struct:
			return 142;
		
		case EToken_Union:
			return 143;
		
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
			EToken_ClassAssign, 
			EToken_Inc, 
			EToken_Dec, 
			EToken_SizeOf, 
			EToken_TypeOf, 
			EToken_New, 
			EToken_Delete, 
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
			EToken_Member, 
			EToken_Abstract, 
			EToken_Virtual, 
			EToken_Override, 
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
			EToken_Short, 
			EToken_Long, 
			EToken_Float, 
			EToken_Double, 
			EToken_Auto, 
			EToken_Signed, 
			EToken_Unsigned, 
			EToken_BigEndian, 
			EToken_Nullable, 
			EToken_Const, 
			EToken_ReadOnly, 
			EToken_Mutable, 
			EToken_Volatile, 
			EToken_Weak, 
			EToken_Thin, 
			EToken_Unsafe, 
			EToken_Cdecl, 
			EToken_Stdcall, 
			EToken_Class, 
			EToken_Function, 
			EToken_Property, 
			EToken_AutoEv, 
			EToken_Bindable, 
			EToken_AutoGet, 
			EToken_Indexed, 
			EToken_PreConstruct, 
			EToken_Operator, 
			EToken_Postfix, 
			EToken_AtAssign, 
			EToken_Multicast, 
			EToken_Event, 
			EToken_Ellipsis, 
			EToken_Enum, 
			EToken_EnumC, 
			EToken_Struct, 
			EToken_Union, 
			
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
			_T("pragma_pack"),
			_T("struct_pack_factor_value"),
			_T("namespace_declaration"),
			_T("extend_declaration"),
			_T("using_declaration"),
			_T("friend_declaration"),
			_T("access_declaration"),
			_T("named_type_specifier"),
			_T("item_declaration_w_specifier"),
			_T("item_declaration_no_specifier_rslv"),
			_T("item_declaration_no_specifier"),
			_T("named_type_block_declaration"),
			_T("local_declaration"),
			_T("statement"),
			_T("qualified_name"),
			_T("access_specifier"),
			_T("qualified_name_impl"),
			_T("declaration_specifier_list"),
			_T("declarator_list"),
			_T("declaration_terminator"),
			_T("declarator_prefix"),
			_T("declarator_name"),
			_T("function_suffix"),
			_T("declarator_suffix"),
			_T("post_declarator_modifier"),
			_T("function_body_pass1"),
			_T("type_name"),
			_T("type_specifier_modifier_list"),
			_T("type_name_list"),
			_T("type_name_or_expr"),
			_T("expression_s"),
			_T("attribute_block"),
			_T("attribute_declarator_list"),
			_T("attribute_declarator"),
			_T("expression_pass1"),
			_T("primary_expr_pass1"),
			_T("expression_save_value"),
			_T("expression"),
			_T("conditional_expr"),
			_T("expression_or_empty"),
			_T("expression_list"),
			_T("expression_or_empty_list"),
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
			_T("additive_operator"),
			_T("at_expr"),
			_T("multiplicative_operator"),
			_T("assignment_expr"),
			_T("unary_expr"),
			_T("assignment_operator_expr"),
			_T("assignment_operator"),
			_T("curly_initializer"),
			_T("postfix_expr"),
			_T("unary_operator_expr"),
			_T("cast_operator_rslv"),
			_T("storage_specifier"),
			_T("new_operator_type"),
			_T("new_operator_curly_initializer"),
			_T("type_name_w_constructor_rslv"),
			_T("qualified_type_name"),
			_T("type_specifier_modifier"),
			_T("primary_expr"),
			_T("postfix_operator"),
			_T("member_operator"),
			_T("literal"),
			_T("curly_initializer_item"),
			_T("expression_save_value_s"),
			_T("conditional_expr_s"),
			_T("expression_or_empty_s"),
			_T("expression_list_s"),
			_T("expression_or_empty_list_s"),
			_T("logical_or_expr_s"),
			_T("logical_and_expr_s"),
			_T("inclusive_or_expr_s"),
			_T("exclusive_or_expr_s"),
			_T("and_expr_s"),
			_T("equality_expr_s"),
			_T("relational_expr_s"),
			_T("shift_expr_s"),
			_T("additive_expr_s"),
			_T("multiplicative_expr_s"),
			_T("at_expr_s"),
			_T("assignment_expr_s"),
			_T("unary_expr_s"),
			_T("postfix_expr_s"),
			_T("unary_operator_expr_s"),
			_T("new_operator_sype_s"),
			_T("primary_expr_s"),
			_T("postfix_operator_s"),
			_T("member_operator_s"),
			_T("compound_stmt_pass1"),
			_T("statement_pass1"),
			_T("compound_stmt"),
			_T("constructor_compound_stmt"),
			_T("base_type_constructor"),
			_T("base_type_constructor_rslv"),
			_T("expression_stmt"),
			_T("if_stmt"),
			_T("switch_stmt"),
			_T("while_stmt"),
			_T("do_stmt"),
			_T("for_stmt"),
			_T("break_stmt"),
			_T("continue_stmt"),
			_T("return_stmt"),
			_T("once_stmt"),
			_T("switch_block_stmt"),
			_T("autoev_body"),
			_T("autoev_stmt"),
			_T("autoev_onchange_stmt"),
			_T("autoev_expression_stmt"),
			_T("statement_0"),
			_T("compound_stmt_0"),
			_T("expression_stmt_0"),
			_T("if_stmt_0"),
			_T("switch_stmt_0"),
			_T("while_stmt_0"),
			_T("do_stmt_0"),
			_T("for_stmt_0"),
			_T("break_stmt_0"),
			_T("continue_stmt_0"),
			_T("return_stmt_0"),
			_T("once_stmt_0"),
			_T("expression_0"),
			_T("primary_expr_0"),
			_T("autoev_body_0"),
			_T("autoev_stmt_0"),
			_T("autoev_onchange_stmt_0"),
			_T("autoev_expression_stmt_0"),
			_T("declaration_specifier"),
			_T("type_specifier"),
			_T("type_modifier"),
			_T("property_template_specifier"),
			_T("qualified_type_name_rslv"),
			_T("full_declarator"),
			_T("declarator"),
			_T("declarator_qualifier"),
			_T("operator_name"),
			_T("declarator_prefix_kind"),
			_T("array_suffix"),
			_T("bitfield_suffix"),
			_T("function_formal_argument_list"),
			_T("function_formal_argument"),
			_T("function_formal_argument_w_specifier"),
			_T("function_formal_argument_no_specifier"),
			_T("function_formal_argument_ellipsis"),
			_T("enum_specifier"),
			_T("struct_specifier"),
			_T("union_specifier"),
			_T("class_specifier"),
			_T("property_specifier"),
			_T("autoev_class_specifier"),
			_T("enum_block"),
			_T("enum_member_list"),
			_T("enum_member"),
			_T("struct_pack_factor"),
			_T("named_type_block"),
			
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls41);
				
			break;
			
		
		
		
		
		
		
		case ESymbol_named_type_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls37);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls30);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls27);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls26);
				
			break;
			
		
		case ESymbol_type_name:
			pNode = AXL_MEM_NEW (CSymbolNode_type_name);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls2);
			
			break;
			
		
		case ESymbol_type_specifier_modifier_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls28);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls21);
				
			break;
			
		
		
		
		
		case ESymbol_expression_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls5);
				
			break;
			
		
		case ESymbol_primary_expr_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_primary_expr_pass1);
			
			break;
			
		
		
		case ESymbol_expression:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls6);
				
			break;
			
		
		case ESymbol_conditional_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_conditional_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls12);
			
			break;
			
		
		case ESymbol_expression_or_empty:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls7);
				
			break;
			
		
		case ESymbol_expression_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls8);
				
			break;
			
		
		case ESymbol_expression_or_empty_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls9);
				
			break;
			
		
		case ESymbol_constant_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls10);
				
			break;
			
		
		case ESymbol_constant_integer_expr:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls11);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls13);
				
			break;
			
		
		case ESymbol_shift_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_shift_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_relational_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls14);
				
			break;
			
		
		case ESymbol_additive_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_additive_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_shift_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls15);
				
			break;
			
		
		case ESymbol_multiplicative_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_multiplicative_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_additive_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls16);
				
			break;
			
		
		case ESymbol_at_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_at_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_multiplicative_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls17);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls18);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls29);
				
			break;
			
		
		case ESymbol_new_operator_type:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls19);
				
			break;
			
		
		case ESymbol_new_operator_curly_initializer:
			pNode = AXL_MEM_NEW (CSymbolNode_new_operator_curly_initializer);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		case ESymbol_qualified_type_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls32);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls20);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls22);
				
			break;
			
		
		case ESymbol_expression_list_s:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls23);
				
			break;
			
		
		case ESymbol_expression_or_empty_list_s:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls24);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls25);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls31);
				
			break;
			
		
		case ESymbol_type_modifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls33);
				
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
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls34);
			
			break;
			
		
		case ESymbol_declarator_qualifier:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_qualifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_operator_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls35);
				
			break;
			
		
		case ESymbol_declarator_prefix_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls36);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls38);
				
			break;
			
		
		case ESymbol_struct_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls39);
				
			break;
			
		
		case ESymbol_union_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls42);
				
			break;
			
		
		case ESymbol_class_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls43);
				
			break;
			
		
		case ESymbol_property_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls45);
				
			break;
			
		
		case ESymbol_autoev_class_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_autoev_class_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls44);
			
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls40);
				
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
			
			{ 0, 147 },
			
			{ 0, 160 },
			
			{ 0, 160 },
			
			{ 0, 161 },
			
			{ 0, 33 },
			
			{ 1, 33 },
			
			{ 0, 163 },
			
			{ 0, 171 },
			
			{ 0, 173 },
			
			{ 0, 172 },
			
			{ 1, 172 },
			
			{ 0, 172 },
			
			{ 0, 176 },
			
			{ 0, 33 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 11 },
			
			{ 1, 12 },
			
			{ 0, 3 },
			
			{ 1, 4 },
			
			{ 0, 1 },
			
			{ 0, 183 },
			
			{ 0, 183 },
			
			{ 1, 183 },
			
			{ 0, 183 },
			
			{ 1, 185 },
			
			{ 0, 185 },
			
			{ 0, 183 },
			
			{ 0, 197 },
			
			{ 0, 199 },
			
			{ 0, 201 },
			
			{ 0, 203 },
			
			{ 0, 205 },
			
			{ 0, 209 },
			
			{ 0, 175 },
			
			{ 0, 175 },
			
			{ 0, 214 },
			
			{ 1, 172 },
			
			{ 0, 214 },
			
			{ 1, 215 },
			
			{ 0, 172 },
			
			{ 0, 218 },
			
			{ 1, 187 },
			
			{ 0, 187 },
			
			{ 0, 187 },
			
			{ 0, 183 },
			
			{ 0, 33 },
			
			{ 0, 33 },
			
			{ 0, 65 },
			
			{ 0, 66 },
			
			{ 0, 223 },
			
			{ 0, 70 },
			
			{ 1, 70 },
			
			{ 0, 33 },
			
			{ 1, 183 },
			
			{ 0, 183 },
			
			{ 0, 176 },
			
			{ 0, 176 },
			
			{ 1, 176 },
			
			{ 0, 176 },
			
			{ 1, 227 },
			
			{ 0, 227 },
			
			{ 0, 197 },
			
			{ 0, 199 },
			
			{ 0, 201 },
			
			{ 0, 203 },
			
			{ 0, 205 },
			
			{ 0, 209 },
			
			{ 0, 175 },
			
			{ 0, 175 },
			
			{ 0, 172 },
			
			{ 0, 214 },
			
			{ 1, 245 },
			
			{ 0, 172 },
			
			{ 0, 218 },
			
			{ 0, 229 },
			
			{ 0, 229 },
			
			{ 0, 176 },
			
			{ 0, 33 },
			
			{ 0, 33 },
			
			{ 0, 65 },
			
			{ 0, 223 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 1 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 6 },
			
			{ 1, 7 },
			
			{ 0, 218 },
			
			{ 1, 187 },
			
			{ 0, 183 },
			
			{ 1, 4 },
			
			{ 4, 159 },
			
			{ 2, 72 },
			
			{ 3, 159 },
			
			{ 0, 183 },
			
			{ 1, 6 },
			
			{ 2, 7 },
			
			{ 1, 74 },
			
			{ 0, 189 },
			
			{ 2, 9 },
			
			{ 0, 75 },
			
			{ 1, 9 },
			
			{ 0, 183 },
			
			{ 1, 4 },
			
			{ 2, 159 },
			
			{ 0, 77 },
			
			{ 1, 159 },
			
			{ 2, 183 },
			
			{ 0, 78 },
			
			{ 1, 183 },
			
			{ 2, 159 },
			
			{ 0, 189 },
			
			{ 0, 189 },
			
			{ 1, 81 },
			
			{ 0, 183 },
			
			{ 0, 83 },
			
			{ 1, 159 },
			
			{ 0, 180 },
			
			{ 0, 214 },
			
			{ 0, 161 },
			
			{ 0, 289 },
			
			{ 0, 290 },
			
			{ 0, 289 },
			
			{ 0, 290 },
			
			{ 0, 291 },
			
			{ 0, 218 },
			
			{ 0, 160 },
			
			{ 0, 160 },
			
			{ 0, 294 },
			
			{ 1, 180 },
			
			{ 0, 295 },
			
			{ 0, 33 },
			
			{ 0, 172 },
			
			{ 0, 296 },
			
			{ 0, 297 },
			
			{ 1, 290 },
			
			{ 0, 65 },
			
			{ 0, 173 },
			
			{ 1, 180 },
			
			{ 0, 180 },
			
			{ 0, 123 },
			
			{ 0, 65 },
			
			{ 0, 305 },
			
			{ 0, 306 },
			
			{ 0, 307 },
			
			{ 0, 308 },
			
			{ 0, 309 },
			
			{ 0, 310 },
			
			{ 2, 141 },
			
			{ 0, 33 },
			
			{ 1, 172 },
			
			{ 0, 33 },
			
			{ 1, 180 },
			
			{ 2, 314 },
			
			{ 0, 33 },
			
			{ 1, 174 },
			
			{ 0, 147 },
			
			{ 0, 65 },
			
			{ 0, 33 },
			
			{ 2, 314 },
			
			{ 0, 33 },
			
			{ 1, 174 },
			
			{ 0, 33 },
			
			{ 1, 171 },
			
			{ 1, 314 },
			
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
// #line 6 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			m_StructPackFactor = (*(_cls41*) GetAstLocator (0)).m_Value;
		;
// #line 6191 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 43 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 return false; ;
// #line 6202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 53 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			return OpenGlobalNamespace ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos) != NULL;
		;
// #line 6215 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			m_pModule->m_NamespaceMgr.CloseNamespace ();
		;
// #line 6228 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			return OpenTypeExtension ((*(_cls1*) GetAstLocator (0)).m_Name, (*(_cls1*) GetAstLocator (0)).m_FirstToken.m_Pos);
		;
// #line 6241 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 5:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 68 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			m_pModule->m_NamespaceMgr.CloseNamespace ();
		;
// #line 6254 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 6:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 83 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->m_CurrentAccessKind = (*(_cls30*) GetAstLocator (0)).m_AccessKind;
		;
// #line 6267 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 7:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 100 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			  
			__pSymbol->m_Arg.pName->AddName ((*GetTokenLocator (0)).m_Data.m_String); 
		;
// #line 6280 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 8:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 104 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			__pSymbol->m_Arg.pName->AddName ((*GetTokenLocator (1)).m_Data.m_String);
		;
// #line 6293 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 9:
			{
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			return Declare (&__pSymbol->m_Local.Declarator);
		;
// #line 6306 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 10:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 176 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			return IsEmptyDeclarationTerminatorAllowed (__pSymbol->m_Arg.pTypeSpecifier);
		;
// #line 6319 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 11:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 180 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			return SetFunctionBody (&(*(_cls26*) GetAstLocator (0)).m_TokenList);
		;
// #line 6332 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 12:
			{
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
// #line 199 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls28*) GetAstLocator (0)).m_TypeSpecifier);
		;
// #line 6345 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 13:
			{
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
// #line 204 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_pType = __pSymbol->m_Local.Declarator.CalcType ();
		;
// #line 6358 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 14:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
// #line 215 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_TypeList.InsertTail ((*(_cls2*) GetAstLocator (0)).m_pType);
		;
// #line 6371 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 15:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
// #line 219 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_TypeList.InsertTail ((*(_cls2*) GetAstLocator (1)).m_pType);
		;
// #line 6384 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 16:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
// #line 232 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_pType = (*(_cls2*) GetAstLocator (0)).m_pType;
		;
// #line 6397 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 17:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
// #line 236 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_pType = (*(_cls21*) GetAstLocator (0)).m_Value.GetType ();
		;
// #line 6410 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 18:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 247 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			m_pAttributeBlock = m_pModule->m_AttributeMgr.CreateAttributeBlock ();
		;
// #line 6423 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 19:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 262 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			CAttribute* pAttribute = m_pAttributeBlock->CreateAttribute ((*GetTokenLocator (0)).m_Data.m_String, NULL);
			if (!pAttribute)
				return false;

			pAttribute->m_Pos = (*GetTokenLocator (0)).m_Pos;
		;
// #line 6440 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 20:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 14 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
// #line 6453 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 21:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
		;
// #line 6466 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 22:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 23 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
// #line 6479 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 23:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 28 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
		;
// #line 6492 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 24:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 32 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
// #line 6505 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 25:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 37 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
		;
// #line 6518 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 26:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 41 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
// #line 6531 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 27:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 51 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			m_ExpressionValue = (*(_cls6*) GetAstLocator (0)).m_Value;
		;
// #line 6544 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 28:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
// #line 81 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_ValueList.InsertTail ((*(_cls6*) GetAstLocator (0)).m_Value);
		;
// #line 6557 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 29:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
// #line 86 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls6*) GetAstLocator (1)).m_Value);
			;
// #line 6570 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 30:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
// #line 102 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_ValueList.InsertTail ((*(_cls6*) GetAstLocator (0)).m_Value);
		;
// #line 6583 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 31:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
// #line 107 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls7*) GetAstLocator (1)).m_Value);
			;
// #line 6596 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 32:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
// #line 111 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_ValueList.InsertTail (CValue ());
		;
// #line 6609 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 33:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
// #line 116 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls7*) GetAstLocator (0)).m_Value);
			;
// #line 6622 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 34:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
// #line 130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			if ((*__pAstNode).m_Value.GetValueKind () != EValue_Const)
			{
				err::SetFormatStringError (_T("not a constant expression"));
				return false;
			}
		;
// #line 6639 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 35:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
// #line 146 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
									
			if ((*(_cls6*) GetAstLocator (0)).m_Value.GetValueKind () != EValue_Const || !(*(_cls6*) GetAstLocator (0)).m_Value.GetType ()->IsIntegerType ())
			{
				err::SetFormatStringError (_T("not a constant integer expression"));
				return false;
			}

			bool Result = m_pModule->m_OperatorMgr.CastOperator (&(*(_cls6*) GetAstLocator (0)).m_Value, EType_Int32);
			if (!Result)
				return false;

			(*__pAstNode).m_Value = (*(_cls6*) GetAstLocator (0)).m_Value.GetInt32 ();
		;
// #line 6662 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 36:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 174 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("cond_then"));
			(*__pAstNode).m_pElseBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("cond_else"));
			(*__pAstNode).m_pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("cond_phi"));			
			m_pModule->m_ControlFlowMgr.ConditionalJump (*__pSymbol->m_Arg.pValue, (*__pAstNode).m_pThenBlock, (*__pAstNode).m_pElseBlock);
		;
// #line 6678 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 37:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock ((*__pAstNode).m_pElseBlock); // might have changed				
			;
// #line 6691 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 38:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
// #line 187 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.ConditionalOperator (
					(*__pAstNode).m_TrueValue,
					(*__pAstNode).m_FalseValue,
					(*__pAstNode).m_pThenBlock,
					(*__pAstNode).m_pPhiBlock, 
					__pSymbol->m_Arg.pValue
					);
			;
// #line 6710 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 39:
			{
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 207 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6723 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 40:
			{
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6736 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 41:
			{
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 235 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6749 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 42:
			{
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 249 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6762 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 43:
			{
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 263 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6775 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 44:
			{
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 277 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls13*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6788 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 45:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
// #line 289 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Eq;
		;
// #line 6801 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 46:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
// #line 293 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ne;
		;
// #line 6814 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 47:
			{
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 306 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls14*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6827 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 48:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
// #line 318 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Lt;
		;
// #line 6840 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 49:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
// #line 322 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Gt;
		;
// #line 6853 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 50:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
// #line 326 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Le;
		;
// #line 6866 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 51:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
// #line 330 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ge;
		;
// #line 6879 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 52:
			{
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 343 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls15*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6892 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 53:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 355 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shl;
		;
// #line 6905 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 54:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
// #line 359 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shr;
		;
// #line 6918 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 55:
			{
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 372 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6931 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 56:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
// #line 384 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Add;
		;
// #line 6944 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 57:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
// #line 388 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Sub;
		;
// #line 6957 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 58:
			{
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 401 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 6970 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 59:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 413 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mul;
		;
// #line 6983 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 60:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 417 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Div;
		;
// #line 6996 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 61:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
// #line 421 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mod;
		;
// #line 7009 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 62:
			{
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 434 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7022 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 63:
			{
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 455 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
		;
// #line 7035 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 64:
			{
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 459 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			m_CurlyInitializerTargetValue = *__pSymbol->m_Arg.pValue;
		;
// #line 7048 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 65:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 471 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Assign;
		;
// #line 7061 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 66:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 475 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_AddAssign;
		;
// #line 7074 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 67:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 479 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_SubAssign;
		;
// #line 7087 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 68:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 483 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_MulAssign;
		;
// #line 7100 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 69:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 487 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_DivAssign;
		;
// #line 7113 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 70:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 491 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_ModAssign;
		;
// #line 7126 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 71:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 495 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_ShlAssign;
		;
// #line 7139 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 72:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 499 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_ShrAssign;
		;
// #line 7152 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 73:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 503 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_AndAssign;
		;
// #line 7165 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 74:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 507 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_XorAssign;
		;
// #line 7178 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 75:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 511 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_OrAssign;
		;
// #line 7191 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 76:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
// #line 515 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_ClassAssign;
		;
// #line 7204 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 77:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 532 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Plus, __pSymbol->m_Arg.pValue);
		;
// #line 7217 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 78:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 536 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Minus, __pSymbol->m_Arg.pValue);
		;
// #line 7230 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 79:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 540 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
		;
// #line 7243 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 80:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 544 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, __pSymbol->m_Arg.pValue);
		;
// #line 7256 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 81:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 548 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, __pSymbol->m_Arg.pValue);
		;
// #line 7269 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 82:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 552 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
		;
// #line 7282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 83:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 556 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
		;
// #line 7295 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 84:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 560 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
		;
// #line 7308 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 85:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 564 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstSizeT ((*(_cls4*) GetAstLocator (0)).m_pType->GetTypeKind () == EType_DataRef ? 
				((CDataPtrType*) (*(_cls4*) GetAstLocator (0)).m_pType)->GetTargetType ()->GetSize () : 
				(*(_cls4*) GetAstLocator (0)).m_pType->GetSize ()
				);
		;
// #line 7324 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 86:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 571 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetType ((*(_cls4*) GetAstLocator (0)).m_pType);
		;
// #line 7337 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 87:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 576 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			EStorage StorageKind = IsValidLocator ((*(_cls29*) GetAstLocator (0))) ? (*(_cls29*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
			return m_pModule->m_OperatorMgr.CastOperator (StorageKind, __pSymbol->m_Arg.pValue, (*(_cls2*) GetAstLocator (1)).m_pType);
		;
// #line 7351 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 88:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 581 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			EStorage StorageKind = IsValidLocator ((*(_cls29*) GetAstLocator (0))) ? (*(_cls29*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
			return m_pModule->m_OperatorMgr.NewOperator (StorageKind, (*(_cls19*) GetAstLocator (1)).m_pType, &(*(_cls19*) GetAstLocator (1)).m_ArgList, __pSymbol->m_Arg.pValue);
		;
// #line 7365 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 89:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 587 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			bool Result = m_pModule->m_OperatorMgr.DeleteOperator (*__pSymbol->m_Arg.pValue);
			if (!Result)
				return false;

			__pSymbol->m_Arg.pValue->SetVoid ();
		;
// #line 7382 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 90:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 603 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_pType = (*(_cls2*) GetAstLocator (0)).m_pType;
		;
// #line 7395 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 91:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
// #line 608 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_pType = (*(_cls32*) GetAstLocator (0)).m_pType;
			(*__pAstNode).m_ArgList.TakeOver (&(*(_cls9*) GetAstLocator (1)).m_ValueList);
		;
// #line 7409 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 92:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 649 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.CallOperator (__pSymbol->m_Arg.pValue, &(*(_cls9*) GetAstLocator (0)).m_ValueList);
		;
// #line 7422 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 93:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 653 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.ClosureOperator (__pSymbol->m_Arg.pValue, &(*(_cls9*) GetAstLocator (0)).m_ValueList);
		;
// #line 7435 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 94:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 657 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls6*) GetAstLocator (0)).m_Value);
		;
// #line 7448 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 95:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 661 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
		;
// #line 7461 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 96:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 665 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
		;
// #line 7474 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 97:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 670 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
		;
// #line 7487 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 98:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 678 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.MemberOperator (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
		;
// #line 7500 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 99:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 682 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.GetAuPropertyField (__pSymbol->m_Arg.pValue, EAuPropertyField_OnChange);
		;
// #line 7513 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 100:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 686 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.GetAuPropertyField (__pSymbol->m_Arg.pValue, EAuPropertyField_PropValue);
		;
// #line 7526 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 101:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 690 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.GetPropertyGetter (__pSymbol->m_Arg.pValue);
		;
// #line 7539 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 102:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 694 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.GetPropertySetter (__pSymbol->m_Arg.pValue);
		;
// #line 7552 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 103:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 706 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return LookupIdentifier ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
		;
// #line 7565 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 104:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 710 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return GetThisValue (__pSymbol->m_Arg.pValue);
		;
// #line 7578 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 105:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 714 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return GetAuPropertyField (__pSymbol->m_Arg.pValue, EAuPropertyField_OnChange);
		;
// #line 7591 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 106:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 718 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return GetAuPropertyField (__pSymbol->m_Arg.pValue, EAuPropertyField_PropValue);
		;
// #line 7604 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 107:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 722 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstUInt64 ((*GetTokenLocator (0)).m_Data.m_UInt64);
		;
// #line 7617 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 108:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 726 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstDouble ((*GetTokenLocator (0)).m_Data.m_Double);
		;
// #line 7630 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 109:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 730 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetLiteral ((*(_cls20*) GetAstLocator (0)).m_String);
		;
// #line 7643 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 110:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 734 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstBool (true);
		;
// #line 7656 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 111:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 738 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstBool (false);
		;
// #line 7669 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 112:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 742 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetNull ();
		;
// #line 7682 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 113:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 754 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_String = (*GetTokenLocator (0)).m_Data.m_String;
		;
// #line 7695 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 114:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
// #line 758 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_String.Append ((*GetTokenLocator (1)).m_Data.m_String);
		;
// #line 7708 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 115:
			{
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 781 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			if (!__pSymbol->m_Local.Initializer.m_Count)
			{
				err::SetFormatStringError (_T("empty curly initializer"));
				return false;
			}
		;
// #line 7725 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 116:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 796 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			 return true; ;
// #line 7736 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 117:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 798 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return PrepareCurlyInitializerNamedItem (__pSymbol->m_Arg.pInitializer, (*GetTokenLocator (0)).m_Data.m_String);
		;
// #line 7749 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 118:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 803 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, __pSymbol->m_Arg.pInitializer->m_MemberValue, (*(_cls6*) GetAstLocator (1)).m_Value);
			;
// #line 7762 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 119:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 807 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return PrepareCurlyInitializerIndexedItem (__pSymbol->m_Arg.pInitializer);
		;
// #line 7775 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 120:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 811 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, __pSymbol->m_Arg.pInitializer->m_MemberValue, (*(_cls6*) GetAstLocator (0)).m_Value);
		;
// #line 7788 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 121:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 814 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return PrepareCurlyInitializerIndexedItem (__pSymbol->m_Arg.pInitializer);
		;
// #line 7801 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 122:
			{
			CSymbolNode_curly_initializer_item* __pSymbol = (CSymbolNode_curly_initializer_item*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 819 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return SkipCurlyInitializerItem (__pSymbol->m_Arg.pInitializer);
		;
// #line 7814 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 123:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 6 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			m_ExpressionValue = (*(_cls21*) GetAstLocator (0)).m_Value;
		;
// #line 7827 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 124:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls23* __pAstNode = (_cls23*) __pSymbol->m_pAstNode;
// #line 36 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			(*__pAstNode).m_ValueList.InsertTail ((*(_cls21*) GetAstLocator (0)).m_Value);
		;
// #line 7840 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 125:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls23* __pAstNode = (_cls23*) __pSymbol->m_pAstNode;
// #line 41 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls21*) GetAstLocator (1)).m_Value);
			;
// #line 7853 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 126:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
// #line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			(*__pAstNode).m_ValueList.InsertTail ((*(_cls21*) GetAstLocator (0)).m_Value);
		;
// #line 7866 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 127:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
// #line 62 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls22*) GetAstLocator (1)).m_Value);
			;
// #line 7879 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 128:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
// #line 66 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			(*__pAstNode).m_ValueList.InsertTail (CValue ());
		;
// #line 7892 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 129:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls24* __pAstNode = (_cls24*) __pSymbol->m_pAstNode;
// #line 71 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls22*) GetAstLocator (0)).m_Value);
			;
// #line 7905 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 130:
			{
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 89 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetConditionalOperatorResultType (__pSymbol->m_Local.TrueValue, __pSymbol->m_Local.FalseValue, __pSymbol->m_Arg.pValue);
			;
// #line 7918 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 131:
			{
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 103 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7931 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 132:
			{
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 117 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7944 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 133:
			{
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 131 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7957 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 134:
			{
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 145 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7970 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 135:
			{
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 159 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7983 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 136:
			{
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 173 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls13*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 7996 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 137:
			{
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 187 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls14*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8009 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 138:
			{
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 201 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls15*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8022 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 139:
			{
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 215 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8035 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 140:
			{
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 229 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8048 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 141:
			{
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 243 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8061 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 142:
			{
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 256 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				if (IsAutoEvStarter ())
					m_AutoEvBindSiteCount = 0; // reset on assignment
			;
// #line 8075 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 143:
			{
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 261 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
				return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
// #line 8088 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 144:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 279 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Plus, __pSymbol->m_Arg.pValue);
		;
// #line 8101 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 145:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 283 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Minus, __pSymbol->m_Arg.pValue);
		;
// #line 8114 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 146:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 287 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
		;
// #line 8127 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 147:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 291 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Addr, __pSymbol->m_Arg.pValue);
		;
// #line 8140 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 148:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 295 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Indir, __pSymbol->m_Arg.pValue);
		;
// #line 8153 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 149:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 299 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
		;
// #line 8166 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 150:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 303 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
		;
// #line 8179 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 151:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 307 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
		;
// #line 8192 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 152:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 311 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			__pSymbol->m_Arg.pValue->SetConstSizeT ((*(_cls4*) GetAstLocator (0)).m_pType->GetSize ());
		;
// #line 8205 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 153:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 315 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			__pSymbol->m_Arg.pValue->SetType ((*(_cls4*) GetAstLocator (0)).m_pType);
		;
// #line 8218 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 154:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 320 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			__pSymbol->m_Arg.pValue->SetType ((*(_cls2*) GetAstLocator (0)).m_pType);
		;
// #line 8231 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 155:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 324 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			EStorage StorageKind = IsValidLocator ((*(_cls29*) GetAstLocator (0))) ? (*(_cls29*) GetAstLocator (0)).m_StorageKind : EStorage_Heap;
			return NewOperator_s (StorageKind, (*(_cls25*) GetAstLocator (1)).m_pType, __pSymbol->m_Arg.pValue);			
		;
// #line 8245 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 156:
			{
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 329 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			__pSymbol->m_Arg.pValue->SetVoid ();
		;
// #line 8258 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 157:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 340 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			(*__pAstNode).m_pType = (*(_cls2*) GetAstLocator (0)).m_pType;
		;
// #line 8271 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 158:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
// #line 345 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			(*__pAstNode).m_pType = (*(_cls32*) GetAstLocator (0)).m_pType;
		;
// #line 8284 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 159:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 362 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetCallOperatorResultType (__pSymbol->m_Arg.pValue, &(*(_cls24*) GetAstLocator (0)).m_ValueList);
		;
// #line 8297 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 160:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 366 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetClosureOperatorResultType (__pSymbol->m_Arg.pValue, &(*(_cls24*) GetAstLocator (0)).m_ValueList);
		;
// #line 8310 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 161:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 370 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetBinaryOperatorResultType (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls21*) GetAstLocator (0)).m_Value);
		;
// #line 8323 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 162:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 374 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
		;
// #line 8336 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 163:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 378 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
		;
// #line 8349 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 164:
			{
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 383 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetUnaryOperatorResultType (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
		;
// #line 8362 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 165:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 391 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetMemberOperatorResultType (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
		;
// #line 8375 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 166:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 395 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetAuPropertyFieldType (__pSymbol->m_Arg.pValue, EAuPropertyField_OnChange);
		;
// #line 8388 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 167:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 399 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetAuPropertyFieldType (__pSymbol->m_Arg.pValue, EAuPropertyField_PropValue);
		;
// #line 8401 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 168:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 403 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetPropertyGetterType (__pSymbol->m_Arg.pValue);
		;
// #line 8414 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 169:
			{
			CSymbolNode_member_operator_s* __pSymbol = (CSymbolNode_member_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 407 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return m_pModule->m_OperatorMgr.GetPropertySetterType (__pSymbol->m_Arg.pValue);
		;
// #line 8427 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 170:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 419 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return LookupIdentifierType ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
		;
// #line 8440 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 171:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 423 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return GetThisValueType (__pSymbol->m_Arg.pValue);
		;
// #line 8453 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 172:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 427 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return GetAuPropertyFieldType (__pSymbol->m_Arg.pValue, EAuPropertyField_OnChange);
		;
// #line 8466 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 173:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 431 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			return GetAuPropertyFieldType (__pSymbol->m_Arg.pValue, EAuPropertyField_PropValue);
		;
// #line 8479 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 174:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 435 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			__pSymbol->m_Arg.pValue->SetType (GetUInt64TypeKind ((*GetTokenLocator (0)).m_Data.m_UInt64, false));
		;
// #line 8492 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 175:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 439 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			__pSymbol->m_Arg.pValue->SetType (EType_Double);
		;
// #line 8505 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 176:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 443 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			__pSymbol->m_Arg.pValue->SetType (m_pModule->m_TypeMgr.GetLiteralType ((*(_cls20*) GetAstLocator (0)).m_String.GetLength ()));
		;
// #line 8518 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 177:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 447 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			__pSymbol->m_Arg.pValue->SetType (EType_Bool);
		;
// #line 8531 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 178:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 451 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			__pSymbol->m_Arg.pValue->SetType (EType_Bool);
		;
// #line 8544 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 179:
			{
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 455 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			
			__pSymbol->m_Arg.pValue->SetNull ();
		;
// #line 8557 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 180:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 14 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
// #line 8570 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 181:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
		;
// #line 8583 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 182:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 28 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
// #line 8596 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 183:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 38 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
		;
// #line 8609 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 184:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 43 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
		;
// #line 8622 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 185:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 51 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
		;
// #line 8635 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 186:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 55 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_OperatorMgr.PostBaseTypeConstructorList ();
		;
// #line 8648 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 187:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 60 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
		;
// #line 8661 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 188:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 68 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_OperatorMgr.CallBaseTypeConstructor ((*(_cls32*) GetAstLocator (0)).m_pType, &(*(_cls9*) GetAstLocator (1)).m_ValueList);
		;
// #line 8674 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 189:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 111 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.IfStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls6*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
		;
// #line 8687 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 190:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 117 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.IfStmt_Else (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (2)).m_Pos);
			;
// #line 8700 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 191:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 122 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
			m_pModule->m_ControlFlowMgr.IfStmt_Follow (&__pSymbol->m_Local.Stmt, IsValidLocator ((*GetAstLocator (3))) ? (*GetAstLocator (3)).m_LastToken.m_Pos : (*GetAstLocator (4)).m_LastToken.m_Pos);
		;
// #line 8713 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 192:
			{
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 137 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.SwitchStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls6*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
		;
// #line 8726 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 193:
			{
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 141 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.SwitchStmt_Follow (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (2)).m_Pos);
		;
// #line 8739 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 194:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 148 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.SwitchStmt_Case (__pSymbol->m_Arg.pStmt, (*(_cls11*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos, (*GetTokenLocator (2)).m_Pos);
		;
// #line 8752 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 195:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 152 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.SwitchStmt_Default (__pSymbol->m_Arg.pStmt, (*GetTokenLocator (0)).m_Pos, (*GetTokenLocator (1)).m_Pos);
		;
// #line 8765 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 196:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 168 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.WhileStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls6*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos);
		;
// #line 8778 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 197:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 172 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.WhileStmt_Follow (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
		;
// #line 8791 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 198:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 187 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.DoStmt_PreBody (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
		;
// #line 8804 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 199:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 191 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.DoStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (1)).m_LastToken.m_Pos);
		;
// #line 8817 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 200:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 195 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.DoStmt_Condition (&__pSymbol->m_Local.Stmt, (*(_cls6*) GetAstLocator (2)).m_Value);
		;
// #line 8830 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 201:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 210 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.ForStmt_PreInit (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
		;
// #line 8843 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 202:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 215 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PreCondition (&__pSymbol->m_Local.Stmt);
			;
// #line 8856 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 203:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 219 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.ForStmt_PostCondition (&__pSymbol->m_Local.Stmt, (*(_cls6*) GetAstLocator (1)).m_Value);
			;
// #line 8869 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 204:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 224 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PreLoop (&__pSymbol->m_Local.Stmt);
			;
// #line 8882 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 205:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 228 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.ForStmt_PostLoop (&__pSymbol->m_Local.Stmt);
			;
// #line 8895 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 206:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 232 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.ForStmt_PreBody (&__pSymbol->m_Local.Stmt);
		;
// #line 8908 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 207:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 236 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.ForStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (2)).m_LastToken.m_Pos);
		;
// #line 8921 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 208:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 243 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.Break (IsValidLocator ((*(_cls11*) GetAstLocator (0))) ? (*(_cls11*) GetAstLocator (0)).m_Value : 1);
		;
// #line 8934 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 209:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 250 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.Continue (IsValidLocator ((*(_cls11*) GetAstLocator (0))) ? (*(_cls11*) GetAstLocator (0)).m_Value : 1);
		;
// #line 8947 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 210:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 257 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.Return (IsValidLocator ((*(_cls6*) GetAstLocator (0))) ? (*(_cls6*) GetAstLocator (0)).m_Value : CValue (), IsValidLocator ((*GetTokenLocator (1))));
		;
// #line 8960 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 211:
			{
			CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 272 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&__pSymbol->m_Local.Stmt, (*GetTokenLocator (0)).m_Pos);
		;
// #line 8973 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 212:
			{
			CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 276 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.OnceStmt_PostBody (&__pSymbol->m_Local.Stmt, (*GetAstLocator (1)).m_LastToken.m_Pos);
		;
// #line 8986 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 213:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 286 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return FinalizeAutoEv ();
		;
// #line 8999 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 214:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 292 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			 return true; ;
// #line 9010 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 215:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 300 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return FinalizeAutoEvOnChangeClause ();
		;
// #line 9023 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 216:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 304 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			FinalizeAutoEvOnChangeStmt ();
		;
// #line 9036 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 217:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 311 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return AutoEvExpressionStmt (&(*(_cls5*) GetAstLocator (0)).m_TokenList);
		;
// #line 9049 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 218:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 84 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt_0.llk"
			 return true; ;
// #line 9060 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 219:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 92 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt_0.llk"
			
			return CountAutoEvBindSites ();
		;
// #line 9073 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 220:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 100 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt_0.llk"
			
			return CountAutoEvBindSites ();
		;
// #line 9086 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 221:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 22 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return SetStorageKind ((*(_cls29*) GetAstLocator (0)).m_StorageKind);
		;
// #line 9099 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 222:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return SetAccessKind ((*(_cls30*) GetAstLocator (0)).m_AccessKind);
		;
// #line 9112 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 223:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 30 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls31*) GetAstLocator (0)).m_pType);
		;
// #line 9125 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 224:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 33 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			 return false; ;
// #line 9136 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 225:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 35 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls33*) GetAstLocator (0)).m_Modifier);
		;
// #line 9149 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 226:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 60 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls31*) GetAstLocator (0)).m_pType);
		;
// #line 9162 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 227:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 64 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls33*) GetAstLocator (0)).m_Modifier);
		;
// #line 9175 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 228:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 77 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Typedef;
		;
// #line 9188 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 229:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 81 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Alias;
		;
// #line 9201 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 230:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 85 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Static;
		;
// #line 9214 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 231:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 89 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
						
			(*__pAstNode).m_StorageKind = EStorage_Thread;
		;
// #line 9227 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 232:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 93 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Stack;
		;
// #line 9240 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 233:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 97 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Heap;
		;
// #line 9253 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 234:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 101 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_UHeap;
		;
// #line 9266 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 235:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 105 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Member;
		;
// #line 9279 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 236:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 109 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Abstract;
		;
// #line 9292 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 237:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 113 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Virtual;
		;
// #line 9305 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 238:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
// #line 117 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Override;
		;
// #line 9318 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 239:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
// #line 130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_AccessKind = EAccess_Public;
		;
// #line 9331 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 240:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
// #line 134 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_AccessKind = EAccess_Protected;
		;
// #line 9344 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 241:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 147 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
		;
// #line 9357 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 242:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 151 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Variant);
		;
// #line 9370 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 243:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 155 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectClass);
		;
// #line 9383 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 244:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 159 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Bool);
		;
// #line 9396 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 245:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 163 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int8);
		;
// #line 9409 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 246:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 167 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int16);
		;
// #line 9422 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 247:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 171 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32);
		;
// #line 9435 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 248:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 175 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64);
		;
// #line 9448 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 249:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 179 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int);
		;
// #line 9461 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 250:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 183 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int_p);
		;
// #line 9474 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 251:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 187 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char);
		;
// #line 9487 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 252:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 191 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Short);
		;
// #line 9500 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 253:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 195 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Long);
		;
// #line 9513 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 254:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 199 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Float);
		;
// #line 9526 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 255:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 203 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Double);
		;
// #line 9539 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 256:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 207 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			err::SetStringError (_T("'auto' type specifier is not yet supported"));
			return false; 
		;
// #line 9553 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 257:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 212 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls46*) GetAstLocator (0)).m_pType;
		;
// #line 9566 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 258:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
// #line 217 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls32*) GetAstLocator (0)).m_pType;
		;
// #line 9579 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 259:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 225 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			if (IsTypeSpecified ())
				return false;
		;
// #line 9593 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 260:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 230 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return m_Stage == EStage_Pass1 || FindType ((*(_cls1*) GetAstLocator (0)).m_Name) != NULL;
		;
// #line 9606 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 261:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return false;
		;
// #line 9619 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 262:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
// #line 246 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			CType* pType = FindType ((*(_cls1*) GetAstLocator (0)).m_Name);
			if (!pType)
			{
				err::SetFormatStringError (_T("'%s' is not a type"), (*(_cls1*) GetAstLocator (0)).m_Name.GetFullName ());
				return false;
			}

			(*__pAstNode).m_pType = pType;
		;
// #line 9639 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 263:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 266 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Signed;
		;
// #line 9652 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 264:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 270 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Unsigned;
		;
// #line 9665 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 265:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_BigEndian;
		;
// #line 9678 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 266:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 278 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Nullable;
		;
// #line 9691 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 267:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Const;
		;
// #line 9704 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 268:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 286 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_ReadOnly;
		;
// #line 9717 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 269:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 290 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Mutable;
		;
// #line 9730 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 270:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 294 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Volatile;
		;
// #line 9743 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 271:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 298 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Weak;
		;
// #line 9756 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 272:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Thin;
		;
// #line 9769 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 273:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 306 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Unsafe;
		;
// #line 9782 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 274:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 310 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Cdecl;
		;
// #line 9795 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 275:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 314 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Stdcall;
		;
// #line 9808 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 276:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 318 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Class;
		;
// #line 9821 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 277:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 322 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Function;
		;
// #line 9834 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 278:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 326 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Property;
		;
// #line 9847 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 279:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 330 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_AutoEv;
		;
// #line 9860 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 280:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 334 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Bindable;
		;
// #line 9873 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 281:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 338 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_AutoGet;
		;
// #line 9886 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 282:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
// #line 342 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Indexed;
		;
// #line 9899 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 283:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 9 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			 
			return Declare (&(*(_cls34*) GetAstLocator (0)).m_Declarator, IsValidLocator ((*(_cls5*) GetAstLocator (1))) ? &(*(_cls5*) GetAstLocator (1)).m_TokenList : NULL);
		;
// #line 9912 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 284:
			{
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 35 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			__pSymbol->m_Arg.pDeclarator->m_Pos = (*GetAstLocator (0)).m_FirstToken.m_Pos;
		;
// #line 9925 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 285:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 43 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddName ((*GetTokenLocator (0)).m_Data.m_String);
		;
// #line 9938 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 286:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Getter);
		;
// #line 9951 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 287:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 51 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Setter);
		;
// #line 9964 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 288:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 55 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_PreConstructor);
		;
// #line 9977 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 289:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 59 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Constructor);
		;
// #line 9990 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 290:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 63 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Destructor);
		;
// #line 10003 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 291:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 67 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticConstructor);
		;
// #line 10016 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 292:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 71 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticDestructor);
		;
// #line 10029 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 293:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 75 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_CallOperator);
		;
// #line 10042 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 294:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 79 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddCastOperator ((*(_cls2*) GetAstLocator (0)).m_pType);
		;
// #line 10055 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 295:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 83 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnaryBinaryOperator ((*(_cls35*) GetAstLocator (0)).m_UnOpKind, (*(_cls35*) GetAstLocator (0)).m_BinOpKind);
		;
// #line 10068 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 296:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 87 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->SetPropValue ();
		;
// #line 10081 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 297:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 91 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->SetOnChange ();
		;
// #line 10094 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 298:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 110 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_BwNot;
		;
// #line 10107 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 299:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 114 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_LogNot;
		;
// #line 10120 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 300:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 118 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_PreInc;
		;
// #line 10133 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 301:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 122 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_PreDec;
		;
// #line 10146 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 302:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 126 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_PostInc;
		;
// #line 10159 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 303:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_PostDec;
		;
// #line 10172 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 304:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 134 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_Ptr;
		;
// #line 10185 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 305:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 138 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_Plus;
			(*__pAstNode).m_BinOpKind = EBinOp_Add;
		;
// #line 10199 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 306:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 143 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_Minus;
			(*__pAstNode).m_BinOpKind = EBinOp_Sub;
		;
// #line 10213 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 307:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 148 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_Indir;
			(*__pAstNode).m_BinOpKind = EBinOp_Mul;
		;
// #line 10227 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 308:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 153 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Div;
		;
// #line 10240 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 309:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 157 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Mod;
		;
// #line 10253 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 310:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 161 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Shl;
		;
// #line 10266 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 311:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 165 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Shr;
		;
// #line 10279 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 312:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 169 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_Addr;
			(*__pAstNode).m_BinOpKind = EBinOp_BwAnd;
		;
// #line 10293 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 313:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 174 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_BwXor;
		;
// #line 10306 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 314:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 178 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_BwOr;
		;
// #line 10319 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 315:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Eq;
		;
// #line 10332 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 316:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 186 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Ne;
		;
// #line 10345 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 317:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 190 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Lt;
		;
// #line 10358 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 318:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 194 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Le;
		;
// #line 10371 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 319:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 198 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Gt;
		;
// #line 10384 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 320:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Ge;
		;
// #line 10397 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 321:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 206 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Assign;
		;
// #line 10410 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 322:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 210 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_AddAssign;
		;
// #line 10423 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 323:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 214 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_SubAssign;
		;
// #line 10436 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 324:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 218 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_MulAssign;
		;
// #line 10449 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 325:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 222 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_DivAssign;
		;
// #line 10462 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 326:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 226 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_ModAssign;
		;
// #line 10475 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 327:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 230 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_ShlAssign;
		;
// #line 10488 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 328:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_ShrAssign;
		;
// #line 10501 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 329:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 238 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_AndAssign;
		;
// #line 10514 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 330:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 242 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_XorAssign;
		;
// #line 10527 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 331:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 246 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_OrAssign;
		;
// #line 10540 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 332:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 250 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_AtAssign;
		;
// #line 10553 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 333:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 254 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_ClassAssign;
		;
// #line 10566 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 334:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 258 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_LogAnd;
		;
// #line 10579 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 335:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 262 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_LogOr;
		;
// #line 10592 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 336:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 266 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Idx;
		;
// #line 10605 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 337:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 270 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_At;
		;
// #line 10618 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 338:
			{
			CSymbolNode_declarator_prefix* __pSymbol = (CSymbolNode_declarator_prefix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddPrefix ((*(_cls36*) GetAstLocator (0)).m_PrefixKind);
		;
// #line 10631 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 339:
			{
			CSymbolNode_declarator_prefix* __pSymbol = (CSymbolNode_declarator_prefix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 286 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->SetTypeModifier ((*(_cls33*) GetAstLocator (1)).m_Modifier);
		;
// #line 10644 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 340:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 298 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_PrefixKind = EDeclPrefix_Pointer;			
		;
// #line 10657 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 341:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_PrefixKind = EDeclPrefix_Multicast;
		;
// #line 10670 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 342:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
// #line 306 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_PrefixKind = EDeclPrefix_Event;
		;
// #line 10683 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 343:
			{
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 319 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			 return false; ;
// #line 10694 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 344:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 327 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
		;
// #line 10707 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 345:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 331 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix (0);
		;
// #line 10720 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 346:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 368 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls28*) GetAstLocator (0)).m_TypeSpecifier);
		;
// #line 10733 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 347:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 375 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return CreateFormalArg (
				__pSymbol->m_Arg.pFunctionSuffix, 
				&__pSymbol->m_Local.Declarator, 
				IsValidLocator ((*(_cls5*) GetAstLocator (1))) ? &(*(_cls5*) GetAstLocator (1)).m_TokenList : NULL
				) != NULL;
		;
// #line 10750 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 348:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 397 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return CreateFormalArg (
				__pSymbol->m_Arg.pFunctionSuffix,
				&__pSymbol->m_Local.Declarator,
				IsValidLocator ((*(_cls5*) GetAstLocator (0))) ? &(*(_cls5*) GetAstLocator (0)).m_TokenList : NULL
				) != NULL;
		;
// #line 10767 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 349:
			{
			CSymbolNode_function_formal_argument_ellipsis* __pSymbol = (CSymbolNode_function_formal_argument_ellipsis*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 408 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_VarArg;
			if (IsValidLocator ((*GetTokenLocator (0))))
				__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_UnsafeVarArg;
		;
// #line 10782 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 350:
			{
			CSymbolNode_bitfield_suffix* __pSymbol = (CSymbolNode_bitfield_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 419 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddBitFieldSuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
		;
// #line 10795 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 351:
			{
			CSymbolNode_post_declarator_modifier* __pSymbol = (CSymbolNode_post_declarator_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 428 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->SetPostDeclaratorModifier (EPostDeclaratorModifier_Const);
		;
// #line 10808 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 352:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 11 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls38*) GetAstLocator (0)).m_pType;
		;
// #line 10821 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 353:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 15 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls39*) GetAstLocator (0)).m_pType;
		;
// #line 10834 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 354:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls42*) GetAstLocator (0)).m_pType;
		;
// #line 10847 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 355:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 23 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls43*) GetAstLocator (0)).m_pType;
		;
// #line 10860 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 356:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 27 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls45*) GetAstLocator (0)).m_pProperty->GetType ();
		;
// #line 10873 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 357:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
// #line 31 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls44*) GetAstLocator (0)).m_pType;
		;
// #line 10886 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 358:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 53 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = CreateEnumType (
				IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
				IsValidLocator ((*(_cls2*) GetAstLocator (1))) ? (*(_cls2*) GetAstLocator (1)).m_pType : NULL,
				IsValidLocator ((*GetTokenLocator (2))) ? EEnumTypeFlag_Exposed : 0
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
// #line 10906 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 359:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 70 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
		;
// #line 10919 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 360:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 75 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();	
		;
// #line 10932 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 361:
			{
			CSymbolNode_enum_member* __pSymbol = (CSymbolNode_enum_member*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 88 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			CEnumConst* pMember = __pSymbol->m_Arg.pType->CreateConst ((*GetTokenLocator (0)).m_Data.m_String);
			if (!pMember)
				return false;

			pMember->m_Pos = (*GetTokenLocator (0)).m_Pos;
			
			if (IsValidLocator ((*(_cls5*) GetAstLocator (1)))) 
				pMember->SetExpression (&(*(_cls5*) GetAstLocator (1)).m_TokenList);
		;
// #line 10952 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 362:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
// #line 114 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = CreateStructType (
				IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
				IsValidLocator ((*(_cls3*) GetAstLocator (1))) ? &(*(_cls3*) GetAstLocator (1)).m_TypeList : NULL,
				IsValidLocator ((*(_cls40*) GetAstLocator (2))) ? (*(_cls40*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
// #line 10972 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 363:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
// #line 135 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_PackFactor = (*(_cls41*) GetAstLocator (0)).m_Value;
		;
// #line 10985 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 364:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 146 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_Value = (*GetTokenLocator (0)).m_Data.m_Integer;
		;
// #line 10998 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 365:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls41* __pAstNode = (_cls41*) __pSymbol->m_pAstNode;
// #line 150 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_Value = m_DefaultStructPackFactor;
		;
// #line 11011 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 366:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
// #line 166 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = CreateUnionType (IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString ());
			if (!(*__pAstNode).m_pType)
				return false;
		;
// #line 11026 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 367:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls43* __pAstNode = (_cls43*) __pSymbol->m_pAstNode;
// #line 187 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = CreateClassType (
				IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
				IsValidLocator ((*(_cls3*) GetAstLocator (1))) ? &(*(_cls3*) GetAstLocator (1)).m_TypeList : NULL,
				IsValidLocator ((*(_cls40*) GetAstLocator (2))) ? (*(_cls40*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
// #line 11046 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 368:
			{
			CSymbolNode_autoev_class_specifier* __pSymbol = (CSymbolNode_autoev_class_specifier*) GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
// #line 218 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = CreateAutoEvClassType (
				(*GetTokenLocator (0)).m_Data.m_String, 
				__pSymbol->m_Local.Declarator.GetFunctionSuffix (), 
				IsValidLocator ((*(_cls26*) GetAstLocator (1))) ? &(*(_cls26*) GetAstLocator (1)).m_TokenList : NULL
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
// #line 11066 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 369:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
// #line 242 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pProperty = CreateProperty (
				(*GetTokenLocator (0)).m_Data.m_String,
				(*GetTokenLocator (0)).m_Pos,
				IsValidLocator ((*(_cls40*) GetAstLocator (1))) ? (*(_cls40*) GetAstLocator (1)).m_PackFactor : m_StructPackFactor
				);

			if (!(*__pAstNode).m_pProperty)
				return false;
		;
// #line 11086 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 370:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
// #line 253 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			m_pLastDeclaredItem = (*__pAstNode).m_pProperty;
			return (*__pAstNode).m_pProperty->CalcType () != NULL; 
		;
// #line 11100 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 371:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
// #line 273 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			__pSymbol->m_Local.pTemplate = CreatePropertyTemplate ();
		;
// #line 11113 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 372:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
// #line 277 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = __pSymbol->m_Local.pTemplate->CalcType ();
			return (*__pAstNode).m_pType != NULL;
		;
// #line 11127 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 373:
			{
			CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 289 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pNamespace);
		;
// #line 11140 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 374:
			{
			CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 294 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();
		;
// #line 11153 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
			
// #line 95 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pName = &(*__pAstNode).m_Name;
// #line 11181 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_declarator_list* __pTarget = (CSymbolNode_declarator_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 121 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls27*) GetAstLocator (0)).m_TypeSpecifier;
// #line 11194 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 122 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls27*) GetAstLocator (0)).m_TypeSpecifier;
// #line 11207 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 138 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11220 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 139 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11233 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 140 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11246 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 140 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11259 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 141 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11272 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 145 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = NULL;
// #line 11285 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier_rslv* __pSymbol = (CSymbolNode_item_declaration_no_specifier_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 159 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11298 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
// #line 202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11311 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
// #line 203 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 11324 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
			
// #line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 11337 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11350 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11363 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 35 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 11376 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
			
// #line 62 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 11389 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
			
// #line 71 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 11402 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
			
// #line 129 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 11415 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_logical_or_expr* __pTarget = (CSymbolNode_logical_or_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
			
// #line 172 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11428 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
			
// #line 181 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_TrueValue;
// #line 11441 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			_cls12* __pAstNode = (_cls12*) __pSymbol->m_pAstNode;
			
// #line 186 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_FalseValue;
// #line 11454 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 204 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11467 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 206 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11480 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 218 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11493 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 220 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11506 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 232 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11519 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11532 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 246 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11545 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 248 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11558 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 260 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11571 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 262 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11584 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11597 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 276 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11610 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 303 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11623 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 305 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11636 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 340 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11649 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 342 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11662 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 369 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11675 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 371 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11688 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 398 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11701 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 41:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 400 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11714 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 42:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 431 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11727 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 43:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 433 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11740 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 44:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 441 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11753 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 45:
			{
			CSymbolNode_assignment_operator_expr* __pTarget = (CSymbolNode_assignment_operator_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 441 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11766 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 46:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 454 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 11779 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 47:
			{
			CSymbolNode_postfix_expr* __pTarget = (CSymbolNode_postfix_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 521 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11792 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 48:
			{
			CSymbolNode_unary_operator_expr* __pTarget = (CSymbolNode_unary_operator_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 522 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11805 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 49:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 531 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11818 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 50:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 535 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11831 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 51:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 539 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11844 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 52:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 543 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11857 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 53:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 547 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11870 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 54:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 551 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11883 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 55:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 555 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11896 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 56:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 559 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11909 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 57:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 575 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11922 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 58:
			{
			CSymbolNode_new_operator_curly_initializer* __pTarget = (CSymbolNode_new_operator_curly_initializer*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 585 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11935 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 59:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 586 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11948 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 60:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_cast_operator_rslv* __pSymbol = (CSymbolNode_cast_operator_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 634 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTypeSpecifier = &__pSymbol->m_Local.TypeSpecifier;
// #line 11961 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 61:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 638 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11974 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 62:
			{
			CSymbolNode_postfix_operator* __pTarget = (CSymbolNode_postfix_operator*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 639 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 11987 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 63:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 668 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12000 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 64:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 673 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12013 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 65:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 745 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12026 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 66:
			{
			CSymbolNode_curly_initializer_item* __pTarget = (CSymbolNode_curly_initializer_item*) pSymbol;
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 780 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pInitializer = &__pSymbol->m_Local.Initializer;
// #line 12039 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 67:
			{
			CSymbolNode_curly_initializer_item* __pTarget = (CSymbolNode_curly_initializer_item*) pSymbol;
			CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 780 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pInitializer = &__pSymbol->m_Local.Initializer;
// #line 12052 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 68:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
			
// #line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12065 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 69:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
			
// #line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
// #line 12078 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 70:
			{
			CSymbolNode_logical_or_expr_s* __pTarget = (CSymbolNode_logical_or_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 86 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12091 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 71:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 88 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.TrueValue;
// #line 12104 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 72:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_conditional_expr_s* __pSymbol = (CSymbolNode_conditional_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 88 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.FalseValue;
// #line 12117 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 73:
			{
			CSymbolNode_logical_and_expr_s* __pTarget = (CSymbolNode_logical_and_expr_s*) pSymbol;
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 100 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 74:
			{
			CSymbolNode_logical_and_expr_s* __pTarget = (CSymbolNode_logical_and_expr_s*) pSymbol;
			CSymbolNode_logical_or_expr_s* __pSymbol = (CSymbolNode_logical_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 102 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12143 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 75:
			{
			CSymbolNode_inclusive_or_expr_s* __pTarget = (CSymbolNode_inclusive_or_expr_s*) pSymbol;
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 114 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12156 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 76:
			{
			CSymbolNode_inclusive_or_expr_s* __pTarget = (CSymbolNode_inclusive_or_expr_s*) pSymbol;
			CSymbolNode_logical_and_expr_s* __pSymbol = (CSymbolNode_logical_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 116 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12169 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 77:
			{
			CSymbolNode_exclusive_or_expr_s* __pTarget = (CSymbolNode_exclusive_or_expr_s*) pSymbol;
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 128 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 78:
			{
			CSymbolNode_exclusive_or_expr_s* __pTarget = (CSymbolNode_exclusive_or_expr_s*) pSymbol;
			CSymbolNode_inclusive_or_expr_s* __pSymbol = (CSymbolNode_inclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12195 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 79:
			{
			CSymbolNode_and_expr_s* __pTarget = (CSymbolNode_and_expr_s*) pSymbol;
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12208 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 80:
			{
			CSymbolNode_and_expr_s* __pTarget = (CSymbolNode_and_expr_s*) pSymbol;
			CSymbolNode_exclusive_or_expr_s* __pSymbol = (CSymbolNode_exclusive_or_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 144 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 81:
			{
			CSymbolNode_equality_expr_s* __pTarget = (CSymbolNode_equality_expr_s*) pSymbol;
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 156 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 82:
			{
			CSymbolNode_equality_expr_s* __pTarget = (CSymbolNode_equality_expr_s*) pSymbol;
			CSymbolNode_and_expr_s* __pSymbol = (CSymbolNode_and_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 158 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12247 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 83:
			{
			CSymbolNode_relational_expr_s* __pTarget = (CSymbolNode_relational_expr_s*) pSymbol;
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 170 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12260 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 84:
			{
			CSymbolNode_relational_expr_s* __pTarget = (CSymbolNode_relational_expr_s*) pSymbol;
			CSymbolNode_equality_expr_s* __pSymbol = (CSymbolNode_equality_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 172 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12273 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 85:
			{
			CSymbolNode_shift_expr_s* __pTarget = (CSymbolNode_shift_expr_s*) pSymbol;
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 184 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12286 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 86:
			{
			CSymbolNode_shift_expr_s* __pTarget = (CSymbolNode_shift_expr_s*) pSymbol;
			CSymbolNode_relational_expr_s* __pSymbol = (CSymbolNode_relational_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 186 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12299 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 87:
			{
			CSymbolNode_additive_expr_s* __pTarget = (CSymbolNode_additive_expr_s*) pSymbol;
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 198 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12312 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 88:
			{
			CSymbolNode_additive_expr_s* __pTarget = (CSymbolNode_additive_expr_s*) pSymbol;
			CSymbolNode_shift_expr_s* __pSymbol = (CSymbolNode_shift_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 200 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12325 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 89:
			{
			CSymbolNode_multiplicative_expr_s* __pTarget = (CSymbolNode_multiplicative_expr_s*) pSymbol;
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 212 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12338 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 90:
			{
			CSymbolNode_multiplicative_expr_s* __pTarget = (CSymbolNode_multiplicative_expr_s*) pSymbol;
			CSymbolNode_additive_expr_s* __pSymbol = (CSymbolNode_additive_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 214 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12351 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 91:
			{
			CSymbolNode_at_expr_s* __pTarget = (CSymbolNode_at_expr_s*) pSymbol;
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 226 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12364 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 92:
			{
			CSymbolNode_at_expr_s* __pTarget = (CSymbolNode_at_expr_s*) pSymbol;
			CSymbolNode_multiplicative_expr_s* __pSymbol = (CSymbolNode_multiplicative_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 228 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12377 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 93:
			{
			CSymbolNode_assignment_expr_s* __pTarget = (CSymbolNode_assignment_expr_s*) pSymbol;
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 240 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12390 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 94:
			{
			CSymbolNode_assignment_expr_s* __pTarget = (CSymbolNode_assignment_expr_s*) pSymbol;
			CSymbolNode_at_expr_s* __pSymbol = (CSymbolNode_at_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 242 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12403 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 95:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 254 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12416 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 96:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_assignment_expr_s* __pSymbol = (CSymbolNode_assignment_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 260 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
// #line 12429 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 97:
			{
			CSymbolNode_postfix_expr_s* __pTarget = (CSymbolNode_postfix_expr_s*) pSymbol;
			CSymbolNode_unary_expr_s* __pSymbol = (CSymbolNode_unary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 268 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12442 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 98:
			{
			CSymbolNode_unary_operator_expr_s* __pTarget = (CSymbolNode_unary_operator_expr_s*) pSymbol;
			CSymbolNode_unary_expr_s* __pSymbol = (CSymbolNode_unary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 269 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12455 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 99:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 278 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12468 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 100:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12481 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 101:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 286 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12494 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 102:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 290 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12507 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 103:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 294 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12520 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 104:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 298 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12533 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 105:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12546 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 106:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 306 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12559 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 107:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 319 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12572 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 108:
			{
			CSymbolNode_unary_expr_s* __pTarget = (CSymbolNode_unary_expr_s*) pSymbol;
			CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 328 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12585 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 109:
			{
			CSymbolNode_primary_expr_s* __pTarget = (CSymbolNode_primary_expr_s*) pSymbol;
			CSymbolNode_postfix_expr_s* __pSymbol = (CSymbolNode_postfix_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 351 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12598 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 110:
			{
			CSymbolNode_postfix_operator_s* __pTarget = (CSymbolNode_postfix_operator_s*) pSymbol;
			CSymbolNode_postfix_expr_s* __pSymbol = (CSymbolNode_postfix_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 352 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12611 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 111:
			{
			CSymbolNode_member_operator_s* __pTarget = (CSymbolNode_member_operator_s*) pSymbol;
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 381 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12624 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 112:
			{
			CSymbolNode_member_operator_s* __pTarget = (CSymbolNode_member_operator_s*) pSymbol;
			CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 386 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12637 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 113:
			{
			CSymbolNode_conditional_expr_s* __pTarget = (CSymbolNode_conditional_expr_s*) pSymbol;
			CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 458 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
// #line 12650 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 114:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
			
// #line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
// #line 12663 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 115:
			{
			CSymbolNode_statement_pass1* __pTarget = (CSymbolNode_statement_pass1*) pSymbol;
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12676 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 116:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
// #line 12689 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 117:
			{
			CSymbolNode_switch_block_stmt* __pTarget = (CSymbolNode_switch_block_stmt*) pSymbol;
			CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 140 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pStmt = &__pSymbol->m_Local.Stmt;
// #line 12702 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 118:
			{
			CSymbolNode_declaration_specifier* __pTarget = (CSymbolNode_declaration_specifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
			
// #line 16 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
// #line 12715 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 119:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
			
// #line 55 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
// #line 12728 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 120:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 12741 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 121:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 12754 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 122:
			{
			CSymbolNode_declarator* __pTarget = (CSymbolNode_declarator*) pSymbol;
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
// #line 12767 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 123:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
			
// #line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 12780 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 124:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
			
// #line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 12793 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 125:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
			
// #line 27 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 12806 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 126:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
			
// #line 28 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
// #line 12819 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 127:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 34 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 12832 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 128:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 38 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 12845 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 129:
			{
			CSymbolNode_array_suffix* __pTarget = (CSymbolNode_array_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 317 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 12858 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 130:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 318 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 12871 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 131:
			{
			CSymbolNode_bitfield_suffix* __pTarget = (CSymbolNode_bitfield_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 320 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
// #line 12884 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 132:
			{
			CSymbolNode_function_formal_argument_list* __pTarget = (CSymbolNode_function_formal_argument_list*) pSymbol;
			CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 347 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Local.pFunctionSuffix;
// #line 12897 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 133:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 352 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 12910 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 134:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 353 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 12923 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 135:
			{
			CSymbolNode_function_formal_argument_w_specifier* __pTarget = (CSymbolNode_function_formal_argument_w_specifier*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 357 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 12936 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 136:
			{
			CSymbolNode_function_formal_argument_no_specifier* __pTarget = (CSymbolNode_function_formal_argument_no_specifier*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 358 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 12949 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 137:
			{
			CSymbolNode_function_formal_argument_ellipsis* __pTarget = (CSymbolNode_function_formal_argument_ellipsis*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 359 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
// #line 12962 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 138:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 371 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 12975 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 139:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 372 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 12988 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 140:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_function_formal_argument_w_specifier* __pSymbol = (CSymbolNode_function_formal_argument_w_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 373 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13001 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 141:
			{
			CSymbolNode_declarator_prefix* __pTarget = (CSymbolNode_declarator_prefix*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 393 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13014 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 142:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 394 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13027 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 143:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 395 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13040 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 144:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
			
// #line 63 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
// #line 13053 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 145:
			{
			CSymbolNode_enum_member_list* __pTarget = (CSymbolNode_enum_member_list*) pSymbol;
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 73 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13066 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 146:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 82 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13079 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 147:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
// #line 82 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
// #line 13092 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 148:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
			
// #line 124 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13105 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 149:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
			
// #line 171 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13118 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 150:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls43* __pAstNode = (_cls43*) __pSymbol->m_pAstNode;
			
// #line 197 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
// #line 13131 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 151:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_autoev_class_specifier* __pSymbol = (CSymbolNode_autoev_class_specifier*) GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
			
// #line 216 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
// #line 13144 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 152:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls45* __pAstNode = (_cls45*) __pSymbol->m_pAstNode;
			
// #line 252 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pProperty;
// #line 13157 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 153:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls46* __pAstNode = (_cls46*) __pSymbol->m_pAstNode;
			
// #line 276 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = __pSymbol->m_Local.pTemplate;
// #line 13170 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
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
// #line 116 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
				
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
	;
// #line 13205 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		case ESymbol_item_declaration_no_specifier:
			{
				CSymbolNode_item_declaration_no_specifier* __pSymbol = (CSymbolNode_item_declaration_no_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 133 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
				
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
		__pSymbol->m_Local.Declarator.SetTypeSpecifier (NULL);
	;
// #line 13221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		case ESymbol_statement:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 80 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
		PreStatement ();
	;
// #line 13236 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
// #line 9 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13252 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_function_suffix:
			{
				CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 344 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
				
		__pSymbol->m_Local.pFunctionSuffix = __pSymbol->m_Arg.pDeclarator->AddFunctionSuffix ();
	;
// #line 13269 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
// #line 48 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13286 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_assignment_operator_expr:
			{
				CSymbolNode_assignment_operator_expr* __pSymbol = (CSymbolNode_assignment_operator_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 450 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsAutoEvStarter ())  // reset on assignment
			m_AutoEvBindSiteList.Clear (); 
	;
// #line 13334 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		case ESymbol_curly_initializer:
			{
				CSymbolNode_curly_initializer* __pSymbol = (CSymbolNode_curly_initializer*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 773 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
		ASSERT (m_CurlyInitializerTargetValue);

		__pSymbol->m_Local.Initializer.m_TargetValue = m_CurlyInitializerTargetValue;
		__pSymbol->m_Local.Initializer.m_Index = 0;
		__pSymbol->m_Local.Initializer.m_Count = 0;
	;
// #line 13352 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_new_operator_curly_initializer:
			{
				CSymbolNode_new_operator_curly_initializer* __pSymbol = (CSymbolNode_new_operator_curly_initializer*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 620 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
		m_CurlyInitializerTargetValue = *__pSymbol->m_Arg.pValue;
	;
// #line 13370 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_if_stmt:
			{
				CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 107 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.IfStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13422 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_switch_stmt:
			{
				CSymbolNode_switch_stmt* __pSymbol = (CSymbolNode_switch_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 133 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.SwitchStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13435 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_while_stmt:
			{
				CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 164 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.WhileStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13448 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_do_stmt:
			{
				CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 183 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.DoStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13461 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_for_stmt:
			{
				CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 206 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.ForStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13474 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		case ESymbol_once_stmt:
			{
				CSymbolNode_once_stmt* __pSymbol = (CSymbolNode_once_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 268 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
		m_pModule->m_ControlFlowMgr.OnceStmt_Create (&__pSymbol->m_Local.Stmt);
	;
// #line 13490 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_autoev_stmt_0:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 81 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt_0.llk"
				
		m_AutoEvBindSiteCount = 0;
	;
// #line 13523 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		case ESymbol_declarator:
			{
				CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
				_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
// #line 22 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
				
		(*__pAstNode).m_Declarator.SetTypeSpecifier (__pSymbol->m_Arg.pTypeSpecifier);
	;
// #line 13544 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		case ESymbol_operator_name:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
// #line 105 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
				
		(*__pAstNode).m_UnOpKind = EUnOp_Undefined;
		(*__pAstNode).m_BinOpKind = EBinOp_Undefined;
	;
// #line 13559 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		case ESymbol_function_formal_argument_no_specifier:
			{
				CSymbolNode_function_formal_argument_no_specifier* __pSymbol = (CSymbolNode_function_formal_argument_no_specifier*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 390 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
				
		__pSymbol->m_Local.Declarator.SetTypeSpecifier (NULL);
	;
// #line 13578 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
				_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
// #line 13 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13633 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		case ESymbol_declaration_terminator:
			{
				CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 166 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
				
		if (m_pAttributeBlock)
		{
			err::SetFormatStringError (_T("unused attribute block in declaration"));
			return false;
		}

		m_pLastDeclaredItem = NULL;
	;
// #line 13653 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
// #line 52 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
// #line 13673 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_unary_operator_expr:
			{
				CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 527 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsAutoEvStarter () && __pSymbol->m_Arg.pValue->GetType ()->IsBindablePropertyType ())
			m_AutoEvBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 13725 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		case ESymbol_primary_expr:
			{
				CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 701 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsAutoEvStarter () && __pSymbol->m_Arg.pValue->GetType ()->IsBindablePropertyType ())
			m_AutoEvBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 13746 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_postfix_operator:
			{
				CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 644 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
		if (IsAutoEvStarter () && __pSymbol->m_Arg.pValue->GetType ()->IsBindablePropertyType ())
			m_AutoEvBindSiteList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
// #line 13760 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_unary_operator_expr_s:
			{
				CSymbolNode_unary_operator_expr_s* __pSymbol = (CSymbolNode_unary_operator_expr_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsAutoEvStarter () && __pSymbol->m_Arg.pValue->GetType ()->IsBindablePropertyType ())
			m_AutoEvBindSiteCount++;
	;
// #line 13796 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		case ESymbol_primary_expr_s:
			{
				CSymbolNode_primary_expr_s* __pSymbol = (CSymbolNode_primary_expr_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 414 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsAutoEvStarter () && __pSymbol->m_Arg.pValue->GetType ()->IsBindablePropertyType ())
			m_AutoEvBindSiteCount++;
	;
// #line 13811 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_postfix_operator_s:
			{
				CSymbolNode_postfix_operator_s* __pSymbol = (CSymbolNode_postfix_operator_s*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
// #line 357 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr_s.llk"
				
		if (IsAutoEvStarter () && __pSymbol->m_Arg.pValue->GetType ()->IsBindablePropertyType ())
			m_AutoEvBindSiteCount++;
	;
// #line 13825 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_enum_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
// #line 46 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		if ((*__pAstNode).m_pType->GetName ().IsEmpty () || ((*__pAstNode).m_pType->GetFlags () & EEnumTypeFlag_Exposed))
			return m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->ExposeEnumConsts ((*__pAstNode).m_pType);
	;
// #line 13896 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
					
				pTransition->m_ProductionIndex = 1634;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 1636;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 3:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 4:
			
			switch (LookaheadToken)
			{
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 1160;
				pTransition->m_ResolverElseIndex = 153;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 5:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 1160;
				pTransition->m_ResolverElseIndex = 1639;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 6:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Pack:
					
				pTransition->m_ProductionIndex = 153;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 153;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 7:
			
			switch (LookaheadToken)
			{
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 1160;
				pTransition->m_ResolverElseIndex = 153;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 8:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 155;
				pTransition->m_ResolverElseIndex = 154;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 9:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 155;
				pTransition->m_ResolverElseIndex = 154;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 10:
			
			switch (LookaheadToken)
			{
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 1644;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 11:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 12:
			
			switch (LookaheadToken)
			{
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 1646;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 13:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 152;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 14:
			
			switch (LookaheadToken)
			{
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 1160;
				pTransition->m_ResolverElseIndex = 153;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 15:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 1160;
				pTransition->m_ResolverElseIndex = 1649;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 16:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Pack:
					
				pTransition->m_ProductionIndex = 153;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 153;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 17:
			
			switch (LookaheadToken)
			{
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 1160;
				pTransition->m_ResolverElseIndex = 153;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 18:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 155;
				pTransition->m_ResolverElseIndex = 154;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 19:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 155;
				pTransition->m_ResolverElseIndex = 154;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 20:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 1654;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 21:
			
			pTransition->m_ProductionIndex = 154;
			pTransition->m_ResolverIndex = 292;
			pTransition->m_ResolverElseIndex = 159;
				
			return ELaDfaResult_Resolver;
			
		
		case 22:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1656;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 23:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 24:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1658;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 25:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 26:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 1660;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 27:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 28:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1662;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 29:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 30:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1664;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 31:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 32:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1666;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 33:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 34:
			
			switch (LookaheadToken)
			{
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 1668;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 35:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 36:
			
			switch (LookaheadToken)
			{
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 1670;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 37:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 38:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1672;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 39:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 40:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1674;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 41:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 42:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1676;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 43:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 159;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 44:
			
			switch (LookaheadToken)
			{
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 1160;
				pTransition->m_ResolverElseIndex = 153;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 45:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 1160;
				pTransition->m_ResolverElseIndex = 1679;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 46:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Pack:
					
				pTransition->m_ProductionIndex = 153;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 153;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 47:
			
			switch (LookaheadToken)
			{
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 1160;
				pTransition->m_ResolverElseIndex = 153;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 48:
			
			switch (LookaheadToken)
			{
			
			case '*':
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 159;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 49:
			
			switch (LookaheadToken)
			{
			
			case '~':
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 159;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 50:
			
			switch (LookaheadToken)
			{
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 159;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 51:
			
			switch (LookaheadToken)
			{
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 159;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 52:
			
			switch (LookaheadToken)
			{
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 156;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 159;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 53:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 1687;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 54:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 890;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 889;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 55:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 485;
				pTransition->m_ResolverIndex = 292;
				pTransition->m_ResolverElseIndex = 486;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 56:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 566;
				pTransition->m_ResolverIndex = 213;
				pTransition->m_ResolverElseIndex = 565;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 57:
			
			switch (LookaheadToken)
			{
			
			case '=':
					
				pTransition->m_ProductionIndex = 1691;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 58:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 552;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Delete:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 551;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 59:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 582;
				pTransition->m_ResolverIndex = 217;
				pTransition->m_ResolverElseIndex = 581;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 60:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 1694;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 61:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 760;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 761;
				return ELaDfaResult_Production;
					
			}
			
		
		case 62:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 614;
				pTransition->m_ResolverIndex = 1052;
				pTransition->m_ResolverElseIndex = 616;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 63:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 655;
				pTransition->m_ResolverIndex = 213;
				pTransition->m_ResolverElseIndex = 654;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 64:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 670;
				pTransition->m_ResolverIndex = 217;
				pTransition->m_ResolverElseIndex = 669;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 65:
			
			switch (LookaheadToken)
			{
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 286;
				pTransition->m_ResolverIndex = 1154;
				pTransition->m_ResolverElseIndex = 287;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 66:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 757;
				pTransition->m_ResolverIndex = 1160;
				pTransition->m_ResolverElseIndex = 756;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 67:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1701;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 68:
			
			switch (LookaheadToken)
			{
			
			case '~':
					
				pTransition->m_ProductionIndex = 833;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 832;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 69:
			
			switch (LookaheadToken)
			{
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 1703;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 70:
			
			switch (LookaheadToken)
			{
			
			case '[':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '|':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '^':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '<':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '>':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '/':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '%':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '@':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_LogOr:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_LogAnd:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Eq:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Ne:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Le:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Ge:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Shl:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Shr:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_AddAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_SubAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_MulAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_DivAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_ModAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_ShlAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_ShrAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_AndAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_XorAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_OrAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_ClassAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 835;
				return ELaDfaResult_Production;
					
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			case EToken_AtAssign:
					
				pTransition->m_ProductionIndex = 836;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 834;
				return ELaDfaResult_Production;
					
			}
			
		
		case 71:
			
			switch (LookaheadToken)
			{
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 1705;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 72:
			
			switch (LookaheadToken)
			{
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 843;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 844;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 845;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 73:
			
			switch (LookaheadToken)
			{
			
			case '[':
					
				pTransition->m_ProductionIndex = 1707;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 74:
			
			switch (LookaheadToken)
			{
			
			case ']':
					
				pTransition->m_ProductionIndex = 888;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 887;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 75:
			
			switch (LookaheadToken)
			{
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 1709;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 76:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case ',':
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 893;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 895;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 77:
			
			switch (LookaheadToken)
			{
			
			case ':':
					
				pTransition->m_ProductionIndex = 482;
				pTransition->m_ResolverIndex = 1279;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 78:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 704;
				pTransition->m_ResolverIndex = 254;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 79:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				pTransition->m_ResolverIndex = 292;
				pTransition->m_ResolverElseIndex = 718;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 80:
			
			switch (LookaheadToken)
			{
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 1714;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 81:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 82:
			
			switch (LookaheadToken)
			{
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 1716;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 83:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 84:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 1718;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 85:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 86:
			
			switch (LookaheadToken)
			{
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 1720;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 87:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 88:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 1722;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 89:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 90:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 1724;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 91:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 92:
			
			switch (LookaheadToken)
			{
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 1726;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 93:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 94:
			
			switch (LookaheadToken)
			{
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 1728;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 95:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 96:
			
			switch (LookaheadToken)
			{
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 1730;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 97:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 98:
			
			switch (LookaheadToken)
			{
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 1732;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 99:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 100:
			
			switch (LookaheadToken)
			{
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 1734;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 101:
			
			switch (LookaheadToken)
			{
			
			case ';':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 718;
				return ELaDfaResult_Production;
					
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Alias:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thread:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_UHeap:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Member:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Override:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Nullable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 154;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 102:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 753;
				pTransition->m_ResolverIndex = 292;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 103:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 759;
				pTransition->m_ResolverIndex = 292;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 104:
			
			switch (LookaheadToken)
			{
			
			case EToken_OnChange:
					
				pTransition->m_ProductionIndex = 268;
				pTransition->m_ResolverIndex = 1150;
				pTransition->m_ResolverElseIndex = 269;
						
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
