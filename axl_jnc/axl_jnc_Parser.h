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
	ESymbol_pragma_pack = 2, 
	ESymbol_struct_pack_factor_value = 3, 
	ESymbol_pragma_endian = 4, 
	ESymbol_pragma_endian_value = 5, 
	ESymbol_namespace_declaration = 6, 
	ESymbol_extend_declaration = 7, 
	ESymbol_using_declaration = 8, 
	ESymbol_friend_declaration = 9, 
	ESymbol_access_declaration = 10, 
	ESymbol_item_declaration = 11, 
	ESymbol_local_declaration = 12, 
	ESymbol_item_declaration_w_specifier = 13, 
	ESymbol_statement = 14, 
	ESymbol_qualified_name = 15, 
	ESymbol_named_type_block_declaration = 16, 
	ESymbol_access_specifier = 17, 
	ESymbol_qualified_name_impl = 18, 
	ESymbol_item_declaration_no_specifier = 19, 
	ESymbol_declaration_specifier_list = 20, 
	ESymbol_declarator_list = 21, 
	ESymbol_declaration_terminator = 22, 
	ESymbol_item_declaration_no_specifier_rslv = 23, 
	ESymbol_declarator_name = 24, 
	ESymbol_function_suffix = 25, 
	ESymbol_post_declarator_modifier = 26, 
	ESymbol_function_body_pass1 = 27, 
	ESymbol_type_name = 28, 
	ESymbol_type_specifier_modifier_list = 29, 
	ESymbol_pointer = 30, 
	ESymbol_declarator_suffix = 31, 
	ESymbol_type_name_list = 32, 
	ESymbol_generic_formal_argument_list = 33, 
	ESymbol_generic_formal_argument = 34, 
	ESymbol_generic_actual_argument_list = 35, 
	ESymbol_attribute_block = 36, 
	ESymbol_attribute_declarator_list = 37, 
	ESymbol_attribute_declarator = 38, 
	ESymbol_expression_pass1 = 39, 
	ESymbol_primary_expr_pass1 = 40, 
	ESymbol_expression = 41, 
	ESymbol_conditional_expr = 42, 
	ESymbol_expression_or_empty = 43, 
	ESymbol_expression_list = 44, 
	ESymbol_expression_or_empty_list = 45, 
	ESymbol_constant_expr = 46, 
	ESymbol_constant_integer_expr = 47, 
	ESymbol_logical_or_expr = 48, 
	ESymbol_logical_and_expr = 49, 
	ESymbol_inclusive_or_expr = 50, 
	ESymbol_exclusive_or_expr = 51, 
	ESymbol_and_expr = 52, 
	ESymbol_equality_expr = 53, 
	ESymbol_relational_expr = 54, 
	ESymbol_equality_operator = 55, 
	ESymbol_shift_expr = 56, 
	ESymbol_relational_operator = 57, 
	ESymbol_additive_expr = 58, 
	ESymbol_shift_operator = 59, 
	ESymbol_multiplicative_expr = 60, 
	ESymbol_additive_operator = 61, 
	ESymbol_at_expr = 62, 
	ESymbol_multiplicative_operator = 63, 
	ESymbol_assignment_expr = 64, 
	ESymbol_unary_expr = 65, 
	ESymbol_assignment_operator = 66, 
	ESymbol_postfix_expr = 67, 
	ESymbol_unary_operator_expr = 68, 
	ESymbol_type_name_or_expr = 69, 
	ESymbol_cast_operator_rslv = 70, 
	ESymbol_allocation_specifier = 71, 
	ESymbol_type_specifier_modifier = 72, 
	ESymbol_primary_expr = 73, 
	ESymbol_postfix_operator = 74, 
	ESymbol_member_operator = 75, 
	ESymbol_literal = 76, 
	ESymbol_compound_stmt_pass1 = 77, 
	ESymbol_statement_pass1 = 78, 
	ESymbol_compound_stmt = 79, 
	ESymbol_expression_stmt = 80, 
	ESymbol_if_stmt = 81, 
	ESymbol_switch_stmt = 82, 
	ESymbol_while_stmt = 83, 
	ESymbol_do_stmt = 84, 
	ESymbol_for_stmt = 85, 
	ESymbol_break_stmt = 86, 
	ESymbol_continue_stmt = 87, 
	ESymbol_return_stmt = 88, 
	ESymbol_switch_block_stmt = 89, 
	ESymbol_declaration_specifier = 90, 
	ESymbol_storage_specifier = 91, 
	ESymbol_type_specifier = 92, 
	ESymbol_type_modifier = 93, 
	ESymbol_enum_specifier = 94, 
	ESymbol_struct_specifier = 95, 
	ESymbol_union_specifier = 96, 
	ESymbol_class_specifier = 97, 
	ESymbol_property_template_specifier = 98, 
	ESymbol_property_specifier_rslv = 99, 
	ESymbol_property_specifier = 100, 
	ESymbol_autoev_specifier = 101, 
	ESymbol_qualified_type_name_rslv = 102, 
	ESymbol_qualified_type_name = 103, 
	ESymbol_full_declarator = 104, 
	ESymbol_declarator = 105, 
	ESymbol_initializer = 106, 
	ESymbol_initializer_list = 107, 
	ESymbol_initializer_list_entry = 108, 
	ESymbol_declarator_qualifier = 109, 
	ESymbol_static_this_rslv = 110, 
	ESymbol_operator_name = 111, 
	ESymbol_array_suffix = 112, 
	ESymbol_bitfield_suffix = 113, 
	ESymbol_function_formal_argument_list = 114, 
	ESymbol_function_formal_argument = 115, 
	ESymbol_enum_block = 116, 
	ESymbol_enum_member_list = 117, 
	ESymbol_enum_member = 118, 
	ESymbol_struct_pack_factor = 119, 
	ESymbol_named_type_block = 120, 
	ESymbol_autoev_body = 121, 
	ESymbol_autoev_stmt = 122, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 2,

		StartSymbol        = 0,
		StartPragmaSymbol  = 222,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 146,
		SymbolCount        = 223,
		SequenceCount      = 363,
		ActionCount        = 291,
		ArgumentCount      = 102,
		BeaconCount        = 118,
		LaDfaCount         = 66,

		TotalCount         = 1309,

		NamedSymbolCount   = 123,

		TokenFirst         = 0,
		TokenEnd           = 146,
		SymbolFirst        = 146,
		NamedSymbolEnd     = 269,
		SymbolEnd          = 369,
		SequenceFirst      = 369,
		SequenceEnd        = 732,
		ActionFirst        = 732,
		ActionEnd          = 1023,
		ArgumentFirst      = 1023,
		ArgumentEnd        = 1125,
		BeaconFirst        = 1125,
		BeaconEnd          = 1243,
		LaDfaFirst         = 1243,
		LaDfaEnd           = 1309,
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

	enum EAssign
	{
		EAssign_Normal,
		EAssign_BinOp,
	};

	CModule* m_pModule;
	EStage m_Stage;
	size_t m_StructPackFactor;
	size_t m_DefaultStructPackFactor;
	EEndianness m_Endianness;
	EStorage m_DefaultMethodStorageKind;
	EStorage m_StorageKind;
	EAccess m_AccessKind;
	CAttributeBlock* m_pAttributeBlock;
	CValue m_ThisValue;
	 
	CClassType* m_pAutoEvType;
	CBasicBlock* m_pAutoEvConstructorBlock;
	CBasicBlock* m_pAutoEvDestructorBlock;
	CValue m_AutoEvConstructorThisValue;
	CValue m_AutoEvDestructorThisValue;

	CModuleItem* m_pLastDeclaredItem;

	rtl::CArrayT <CTypeSpecifier*> m_TypeSpecifierStack;
	rtl::CBoxListT <CValue> m_BindablePropertyList;


	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class _cls1: public CAstNode
	{
	public:
#line 94 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CQualifiedName m_Name
;
#line 249 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls2: public CAstNode
	{
	public:
#line 138 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CDeclarator m_Declarator;
;
#line 259 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls3: public CAstNode
	{
	public:
#line 197 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
#line 269 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls4: public CAstNode
	{
	public:
#line 217 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	rtl::CBoxListT <CType*> m_TypeList;
;
#line 279 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls5: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
#line 289 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls6: public CAstNode
	{
	public:
#line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
#line 299 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls7: public CAstNode
	{
	public:
#line 34 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;
;
#line 309 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls8: public CAstNode
	{
	public:
#line 44 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
#line 319 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls9: public CAstNode
	{
	public:
#line 64 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CBoxListT <CValue> m_ValueList;
;
#line 329 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls10: public CAstNode
	{
	public:
#line 92 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_Value;	
;
#line 339 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls11: public CAstNode
	{
	public:
#line 108 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	intptr_t m_Value;
;
#line 349 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class CConditionalExpr: public CAstNode
	{
	public:
#line 132 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CValue m_TrueValue;
	CValue m_FalseValue;
	CBasicBlock* m_pThenBlock;
	CBasicBlock* m_pElseBlock;
	CBasicBlock* m_pPhiBlock;
;
#line 363 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls13: public CAstNode
	{
	public:
#line 246 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 373 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls14: public CAstNode
	{
	public:
#line 275 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 383 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls15: public CAstNode
	{
	public:
#line 312 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 393 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls16: public CAstNode
	{
	public:
#line 341 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 403 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls17: public CAstNode
	{
	public:
#line 370 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 413 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls18: public CAstNode
	{
	public:
#line 417 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EBinOp m_OpKind;
;
#line 423 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls19: public CAstNode
	{
	public:
#line 544 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	EAlloc m_AllocKind;
;
#line 433 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls20: public CAstNode
	{
	public:
#line 570 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	CType* m_pType;
;
#line 443 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls21: public CAstNode
	{
	public:
#line 700 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	rtl::CString m_String;
;
#line 453 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls22: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
		
	rtl::CBoxListT <CToken> m_TokenList;
;
#line 463 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class CCompoundStmt: public CAstNode
	{
	public:
#line 36 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
		
	CScope* m_pScope;
;
#line 473 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class CSwitchStmt: public CAstNode
	{
	public:
#line 101 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
		
	CValue m_Value;
	CBasicBlock* m_pSwitchBlock;
	CBasicBlock* m_pFollowBlock;
	CBasicBlock* m_pDefaultBlock;
	rtl::CHashTableMapT <intptr_t, CBasicBlock*, axl::rtl::CHashIdT <intptr_t> > m_CaseMap;
;
#line 487 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls25: public CAstNode
	{
	public:
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CTypeSpecifier m_TypeSpecifier;
;
#line 497 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls26: public CAstNode
	{
	public:
#line 42 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CTypeSpecifier m_TypeSpecifier;
;
#line 507 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls27: public CAstNode
	{
	public:
#line 71 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EStorage m_StorageKind;
;
#line 517 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls28: public CAstNode
	{
	public:
#line 104 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	EAccess m_AccessKind;
;
#line 527 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls29: public CAstNode
	{
	public:
#line 121 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
#line 537 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls30: public CAstNode
	{
	public:
#line 250 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	CType* m_pType;
;
#line 547 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls31: public CAstNode
	{
	public:
#line 270 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
		
	ETypeModifier m_Modifier;
;
#line 557 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls32: public CAstNode
	{
	public:
#line 36 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
		
	CDeclarator m_Declarator;
;
#line 567 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls33: public CAstNode
	{
	public:
#line 53 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
		
	CValue m_Value;
	rtl::CBoxListT <CToken> m_TokenList;
;
#line 578 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls34: public CAstNode
	{
	public:
#line 151 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
		
	EUnOp m_UnOpKind;
	EBinOp m_BinOpKind;
;
#line 589 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls35: public CAstNode
	{
	public:
#line 6 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CEnumType* m_pType;
;
#line 599 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls36: public CAstNode
	{
	public:
#line 69 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CStructType* m_pType;
;
#line 609 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls37: public CAstNode
	{
	public:
#line 93 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_PackFactor;
;
#line 619 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls38: public CAstNode
	{
	public:
#line 104 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	size_t m_Value;
;
#line 629 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls39: public CAstNode
	{
	public:
#line 123 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CUnionType* m_pType;
;
#line 639 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls40: public CAstNode
	{
	public:
#line 142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CClassType* m_pType;
;
#line 649 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls42: public CAstNode
	{
	public:
#line 184 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CPropertyType* m_pType;
;
#line 659 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls43: public CAstNode
	{
	public:
#line 213 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CProperty* m_pProperty;
;
#line 669 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls44: public CAstNode
	{
	public:
#line 244 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
		
	CClassType* m_pType;
	CDeclarator m_Declarator;
;
#line 680 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_qualified_name_impl: public CSymbolNode
	{
	public:
			
		struct
		{
#line 101 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			CQualifiedName* pName;
#line 713 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 3 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
#line 735 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 171 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			CTypeSpecifier* pTypeSpecifier;
#line 755 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_declaration_terminator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declaration_terminator;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_item_declaration_no_specifier_rslv: public CSymbolNode
	{
	public:
			
		struct
		{
#line 163 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
		CDeclarator Declarator;
	;
#line 779 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_item_declaration_no_specifier_rslv ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_item_declaration_no_specifier_rslv;
			
		}
	};
		
	
	class CSymbolNode_declarator_name: public CSymbolNode
	{
	public:
			
		struct
		{
#line 85 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
#line 799 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 370 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
#line 819 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 372 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
		CDeclFunctionSuffix* pFunctionSuffix;
	;
#line 828 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_function_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_suffix;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_post_declarator_modifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 418 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
#line 850 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
				
		CDeclarator Declarator;
	;
#line 873 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_type_name ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_type_name;
			
		}
	};
		
	
	
	class CSymbolNode_pointer: public CSymbolNode
	{
	public:
			
		struct
		{
#line 332 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
#line 894 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_pointer ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_pointer;
			
		}
	};
		
	
	class CSymbolNode_declarator_suffix: public CSymbolNode
	{
	public:
			
		struct
		{
#line 349 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
#line 914 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_declarator_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
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
#line 942 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 139 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 963 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 161 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 988 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 163 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 997 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 175 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1017 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 177 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1026 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 189 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1046 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 191 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1055 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 203 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1075 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 205 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1084 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 217 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1104 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 219 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1113 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 231 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1133 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 233 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 260 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1162 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 262 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1171 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 297 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1192 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 299 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1201 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 326 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1222 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 328 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1231 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 355 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1252 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 357 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1261 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 388 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 390 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1291 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 402 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1312 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 404 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue OpValue2;
	;
#line 1321 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
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
#line 471 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1341 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_unary_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_unary_expr;
			
		}
	};
		
	
	
	class CSymbolNode_postfix_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 592 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1362 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 476 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1382 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_unary_operator_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_unary_operator_expr;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
			
		}
	};
		
	
	class CSymbolNode_type_name_or_expr: public CSymbolNode
	{
	public:
			
		struct
		{
#line 575 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CValue Value;
	;
#line 1406 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_type_name_or_expr ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_type_name_or_expr;
			
		}
	};
		
	
	class CSymbolNode_cast_operator_rslv: public CSymbolNode
	{
	public:
			
		struct
		{
#line 563 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
		CTypeSpecifier TypeSpecifier;
	;
#line 1428 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_cast_operator_rslv ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_cast_operator_rslv;
			
		}
	};
		
	
	
	class CSymbolNode_type_specifier_modifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
#line 1449 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 658 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1469 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 597 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1491 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 631 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			CValue* pValue;
#line 1513 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_member_operator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_member_operator;
			
		}
	};
		
	
	
	class CSymbolNode_compound_stmt_pass1: public CSymbolNode
	{
	public:
			
		struct
		{
#line 12 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			rtl::CBoxListT <CToken>* pTokenList;
#line 1534 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 25 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			rtl::CBoxListT <CToken>* pTokenList;
#line 1554 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 71 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
	CBasicBlock* pThenBlock;
	CBasicBlock* pElseBlock;
	CBasicBlock* pFollowBlock;
;
#line 1580 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_if_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_if_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	class CSymbolNode_while_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
#line 152 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
	CBasicBlock* pConditionBlock;
	CBasicBlock* pBodyBlock;
	CBasicBlock* pFollowBlock;
;
#line 1607 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 180 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
	CBasicBlock* pConditionBlock;
	CBasicBlock* pBodyBlock;
	CBasicBlock* pFollowBlock;
;
#line 1633 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 212 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
	CScope* pScope;
	CBasicBlock* pConditionBlock;
	CBasicBlock* pBodyBlock;
	CBasicBlock* pLoopBlock;
	CBasicBlock* pFollowBlock;
;
#line 1661 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_for_stmt ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_for_stmt;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	
	
	class CSymbolNode_switch_block_stmt: public CSymbolNode
	{
	public:
			
		struct
		{
#line 138 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			CSwitchStmt* pSwitchStmt;
#line 1686 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			CTypeSpecifier* pTypeSpecifier;
#line 1706 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 189 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
		CPropertyTemplate* pTemplate;
	;
#line 1735 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_property_template_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_property_template_specifier;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	
	
	
	
	class CSymbolNode_full_declarator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 7 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
#line 1762 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 39 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CTypeSpecifier* pTypeSpecifier;
#line 1782 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_declarator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_declarator;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_initializer: public CSymbolNode
	{
	public:
			
		struct
		{
#line 57 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CType* pType;
#line 1804 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_initializer ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_initializer;
			
		}
	};
		
	
	class CSymbolNode_initializer_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 73 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CType* pType;
#line 1824 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_initializer_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_initializer_list;
			
		}
	};
		
	
	class CSymbolNode_initializer_list_entry: public CSymbolNode
	{
	public:
			
		struct
		{
#line 78 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CType* pType;
#line 1844 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_initializer_list_entry ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_initializer_list_entry;
			
		}
	};
		
	
	class CSymbolNode_declarator_qualifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 93 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
#line 1864 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 357 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
#line 1886 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 409 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclarator* pDeclarator;
#line 1906 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 383 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
#line 1926 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 388 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			CDeclFunctionSuffix* pFunctionSuffix;
#line 1946 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_function_formal_argument ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_function_formal_argument;
			
		}
	};
		
	
	class CSymbolNode_enum_block: public CSymbolNode
	{
	public:
			
		struct
		{
#line 31 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
#line 1966 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 44 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
#line 1986 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 49 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CEnumType* pType;
#line 2006 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 299 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CNamespace* pNamespace;
#line 2027 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_named_type_block ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_named_type_block;
			
		}
	};
		
	
	class CSymbolNode_autoev_body: public CSymbolNode
	{
	public:
			
		struct
		{
#line 272 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			CClassType* pType;
#line 2047 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_autoev_body ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_Named;
			m_Index = ESymbol_autoev_body;
			
		}
	};
		
	
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	
	CParser ();

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

	void
	PopTypeSpecifier (CTypeSpecifier* pTypeSpecifier)
	{
		m_TypeSpecifierStack.Pop ();
	}

	bool
	IsTypeSpecified ();

	bool
	IsEmptyDeclarationTerminatorAllowed (CTypeSpecifier* pTypeSpecifier);

	void
	AssignDeclarationAttributes (
		CModuleItem* pItem,
		const CToken::CPos& Pos
		);

	bool
	Declare (
		CDeclarator* pDeclarator,
		bool HasInitializer = false
		);
		
	bool
	DeclareTypedef (
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
		CDeclarator* pDeclarator
		);

	bool
	DeclareData (
		CType* pType,
		CDeclarator* pDeclarator,
		bool HasInitializer
		);

	CFunctionFormalArg*
	CreateFormalArg (
		CDeclFunctionSuffix* pArgSuffix,
		CTypeSpecifier* pTypeSpecifier,
		CDeclarator* pDeclarator,
		const CValue& DefaultValue
		);

	CEnumType*
	CreateEnumType (
		const rtl::CString& Name,
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
		size_t PackFactor,
		int Flags
		);

	CProperty*
	CreatePropertyImpl (
		const rtl::CString& Name,
		const CToken::CPos& Pos,
		size_t PackFactor
		);

	CProperty*
	CreateProperty (
		const rtl::CString& Name,
		size_t PackFactor
		)
	{
		return CreatePropertyImpl (Name, m_LastMatchedToken.m_Pos, PackFactor);
	}

	CClassType*
	CreateAutoEvType (CDeclarator* pDeclarator);

	bool
	PreAutoEvBlock (CClassType* pAutoEvTyoe);

	bool
	PostAutoEvBlock ();

	bool
	PreAutoEvExpression ();

	bool
	PostAutoEvExpression (const CValue& Value);

	bool
	SwitchCaseLabel (
		CSwitchStmt* pSwitchStmt,
		intptr_t Value,
		const CToken::CPos& ClosePos,
		const CToken::CPos& OpenPos
		);

	bool
	SwitchDefaultLabel (
		CSwitchStmt* pSwitchStmt,
		const CToken::CPos& ClosePos,
		const CToken::CPos& OpenPos
		);

	bool
	FinalizeSwitchStmt (CSwitchStmt* pSwitchStmt);

	bool
	FinalizeConditionalExpr (
		CConditionalExpr* pConditionalExpr,
		CValue* pResultValue
		);

	bool
	SetFunctionBody (rtl::CBoxListT <CToken>* pBody);

	bool
	LookupIdentifier (
		const rtl::CString& Name,
		CValue* pValue
		);

	bool
	SetThis (CValue* pValue);

	void
	SetDefaultMethodStorageKind ();


	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	static
	size_t*
	GetParseTable ()
	{
		static
		size_t _ParseTable [] = 
		{
			0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 369, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 369, -1, -1, -1, 369, -1, 369, 369, 369, 369, 369, 369, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 369, -1, 369, 369, 369, 369, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, -1, -1, -1, 369, 369, 369, 369, 369, 369, 369, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, -1, -1, -1, 157, -1, 157, 152, 153, 154, 155, 157, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, -1, 157, 157, 157, 157, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, 157, 157, 157, 157, 157, 1243, 1245, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, -1, -1, -1, 157, 157, 157, 157, 157, 157, 157, 
			-1, -1, 370, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 713, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 712, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 371, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 372, 372, 373, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 374, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 376, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 378, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 380, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 382, 382, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 159, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 165, -1, -1, -1, 159, -1, 159, -1, -1, -1, -1, 1247, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 159, -1, 165, 165, 165, 165, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 159, 1248, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 165, 165, -1, -1, -1, 159, 159, 159, 159, 159, 159, 159, 
			-1, -1, -1, 160, -1, 160, -1, -1, -1, -1, -1, -1, -1, 159, -1, -1, -1, -1, -1, 160, 160, 160, 160, -1, -1, -1, 160, 160, -1, -1, 159, -1, 159, -1, -1, 154, -1, 1249, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 160, 160, 160, 160, 160, 160, 160, 160, 160, -1, 159, -1, -1, -1, -1, 160, 160, 160, 160, 160, 160, 160, 160, -1, 160, -1, 160, 160, 160, 160, 160, 160, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, -1, -1, -1, -1, -1, 159, 159, 159, 159, 159, 159, 159, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 387, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 387, -1, 387, -1, -1, -1, -1, 387, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 387, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, 387, -1, -1, -1, -1, -1, 387, 387, 387, 387, 387, 387, 387, 
			-1, -1, -1, 226, -1, 225, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 226, 226, 226, 226, -1, -1, -1, 226, 226, -1, -1, -1, -1, -1, -1, -1, -1, -1, 226, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 226, 226, 226, 226, 226, 226, 226, 226, 226, -1, -1, -1, -1, -1, -1, 226, 226, 226, 226, 226, 226, 226, 227, -1, 228, -1, 229, 230, 231, 232, 233, 234, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 383, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, -1, -1, -1, 157, -1, 157, -1, -1, -1, 155, 157, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, -1, 157, 157, 157, 157, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 157, 157, 157, 157, 157, 157, 1250, 1252, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, 157, -1, -1, -1, 157, 157, 157, 157, 157, 157, 157, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 564, 565, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 384, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 278, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 278, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 278, 278, 278, 278, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 278, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 278, 278, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, -1, 548, -1, -1, -1, -1, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, 548, -1, -1, -1, -1, -1, 548, 548, 548, 548, 548, 548, 548, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 617, -1, -1, -1, 617, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 617, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 617, 617, 617, 617, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 617, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 617, 617, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 392, -1, -1, 391, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 390, 390, 390, 390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 390, 390, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 631, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 631, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 631, 631, 631, 631, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 631, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 631, 631, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1254, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 701, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 525, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 393, -1, 393, -1, -1, -1, -1, 393, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 393, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, 393, -1, -1, -1, -1, -1, 393, 393, 393, 393, 393, 393, 393, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, -1, 554, -1, -1, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, 554, -1, -1, -1, -1, -1, 554, 554, 554, 554, 554, 554, 554, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 685, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 688, -1, -1, -1, -1, -1, 689, -1, -1, -1, 687, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 396, -1, 396, -1, -1, -1, -1, 396, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 396, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, 396, -1, -1, -1, -1, -1, 396, 396, 396, 396, 396, 396, 396, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 398, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 37, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 400, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 402, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 404, -1, -1, -1, -1, -1, -1, 404, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 404, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 406, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 407, 
			-1, 415, 415, 414, 415, 410, 415, 415, 415, 415, 415, 415, 415, 412, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 
			-1, -1, -1, 416, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 416, 416, 416, 416, -1, -1, -1, 416, 416, -1, -1, -1, -1, -1, -1, -1, -1, -1, 416, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 416, 416, 416, 416, 416, 416, 416, 416, 416, -1, -1, -1, -1, -1, -1, 416, 416, 416, 416, 416, 416, 416, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 426, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 426, 426, 426, 426, -1, -1, -1, 426, 426, -1, -1, -1, -1, -1, -1, -1, -1, -1, 426, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 426, 426, 426, 426, 426, 426, 426, 426, 426, -1, -1, -1, -1, -1, -1, 426, 426, 426, 426, 426, 426, 426, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 417, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 417, 417, 417, 417, -1, -1, -1, 417, 417, -1, -1, -1, -1, -1, -1, -1, -1, -1, 417, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 417, 417, 417, 417, 417, 417, 417, 417, 417, -1, -1, -1, -1, -1, -1, 417, 417, 417, 417, 417, 417, 417, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 418, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 418, 418, 418, 418, -1, -1, -1, 418, 418, -1, -1, -1, -1, -1, -1, -1, -1, -1, 418, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 418, 418, 418, 418, 418, 418, 418, 418, 418, -1, -1, -1, -1, -1, -1, 418, 418, 418, 418, 418, 418, 418, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 420, 0, -1, -1, 423, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 420, 420, 420, 420, -1, -1, -1, 420, 420, -1, -1, -1, -1, -1, -1, -1, -1, -1, 420, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 420, 420, 420, 420, 420, 420, 420, 420, 420, -1, -1, -1, -1, -1, -1, 420, 420, 420, 420, 420, 420, 420, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 424, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 424, 424, 424, 424, -1, -1, -1, 424, 424, -1, -1, -1, -1, -1, -1, -1, -1, -1, 424, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 424, 424, 424, 424, 424, 424, 424, 424, 424, -1, -1, -1, -1, -1, -1, 424, 424, 424, 424, 424, 424, 424, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 425, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 425, 425, 425, 425, -1, -1, -1, 425, 425, -1, -1, -1, -1, -1, -1, -1, -1, -1, 425, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 425, 425, 425, 425, 425, 425, 425, 425, 425, -1, -1, -1, -1, -1, -1, 425, 425, 425, 425, 425, 425, 425, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 428, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 428, 428, 428, 428, -1, -1, -1, 428, 428, -1, -1, -1, -1, -1, -1, -1, -1, -1, 428, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 428, 428, 428, 428, 428, 428, 428, 428, 428, -1, -1, -1, -1, -1, -1, 428, 428, 428, 428, 428, 428, 428, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 430, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 430, 430, 430, 430, -1, -1, -1, 430, 430, -1, -1, -1, -1, -1, -1, -1, -1, -1, 430, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 430, 430, 430, 430, 430, 430, 430, 430, 430, -1, -1, -1, -1, -1, -1, 430, 430, 430, 430, 430, 430, 430, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 432, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 432, 432, 432, 432, -1, -1, -1, 432, 432, -1, -1, -1, -1, -1, -1, -1, -1, -1, 432, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 432, 432, 432, 432, 432, 432, 432, 432, 432, -1, -1, -1, -1, -1, -1, 432, 432, 432, 432, 432, 432, 432, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 434, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 434, 434, 434, 434, -1, -1, -1, 434, 434, -1, -1, -1, -1, -1, -1, -1, -1, -1, 434, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 434, 434, 434, 434, 434, 434, 434, 434, 434, -1, -1, -1, -1, -1, -1, 434, 434, 434, 434, 434, 434, 434, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 436, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 436, 436, 436, 436, -1, -1, -1, 436, 436, -1, -1, -1, -1, -1, -1, -1, -1, -1, 436, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 436, 436, 436, 436, 436, 436, 436, 436, 436, -1, -1, -1, -1, -1, -1, 436, 436, 436, 436, 436, 436, 436, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 438, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 438, 438, 438, 438, -1, -1, -1, 438, 438, -1, -1, -1, -1, -1, -1, -1, -1, -1, 438, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 438, 438, 438, 438, 438, 438, 438, 438, 438, -1, -1, -1, -1, -1, -1, 438, 438, 438, 438, 438, 438, 438, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 442, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 442, 442, 442, 442, -1, -1, -1, 442, 442, -1, -1, -1, -1, -1, -1, -1, -1, -1, 442, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 442, 442, 442, 442, 442, 442, 442, 442, 442, -1, -1, -1, -1, -1, -1, 442, 442, 442, 442, 442, 442, 442, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 440, 441, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 448, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 448, 448, 448, 448, -1, -1, -1, 448, 448, -1, -1, -1, -1, -1, -1, -1, -1, -1, 448, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 448, 448, 448, 448, 448, 448, 448, 448, 448, -1, -1, -1, -1, -1, -1, 448, 448, 448, 448, 448, 448, 448, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 444, 445, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 446, 447, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 452, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 452, 452, 452, 452, -1, -1, -1, 452, 452, -1, -1, -1, -1, -1, -1, -1, -1, -1, 452, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 452, 452, 452, 452, 452, 452, 452, 452, 452, -1, -1, -1, -1, -1, -1, 452, 452, 452, 452, 452, 452, 452, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 450, 451, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 456, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 456, 456, 456, 456, -1, -1, -1, 456, 456, -1, -1, -1, -1, -1, -1, -1, -1, -1, 456, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 456, 456, 456, 456, 456, 456, 456, 456, 456, -1, -1, -1, -1, -1, -1, 456, 456, 456, 456, 456, 456, 456, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 454, 455, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 461, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 461, 461, 461, 461, -1, -1, -1, 461, 461, -1, -1, -1, -1, -1, -1, -1, -1, -1, 461, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 461, 461, 461, 461, 461, 461, 461, 461, 461, -1, -1, -1, -1, -1, -1, 461, 461, 461, 461, 461, 461, 461, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 458, 459, 460, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 463, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 463, 463, 463, 463, -1, -1, -1, 463, 463, -1, -1, -1, -1, -1, -1, -1, -1, -1, 463, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 463, 463, 463, 463, 463, 463, 463, 463, 463, -1, -1, -1, -1, -1, -1, 463, 463, 463, 463, 463, 463, 463, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1256, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 478, 478, 478, 478, -1, -1, -1, 478, 478, -1, -1, -1, -1, -1, -1, -1, -1, -1, 477, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 478, 478, 478, 478, 478, 478, 478, 478, 478, -1, -1, -1, -1, -1, -1, 477, 477, 477, 477, 477, 477, 477, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 465, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 499, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 499, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 499, 499, 499, 499, 499, 499, 499, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 307, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 482, 479, 480, 483, -1, -1, -1, 481, 484, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 485, 486, 487, 488, 489, 490, 492, 492, 492, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1257, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 498, 498, 498, 498, -1, -1, -1, 498, 498, -1, -1, 496, -1, 496, -1, -1, -1, -1, 1259, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 498, 498, 498, 498, 498, 498, 498, 498, 498, -1, 496, -1, -1, -1, -1, 498, 498, 498, 498, 498, 498, 498, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, 496, -1, -1, -1, -1, -1, 496, 496, 496, 496, 496, 496, 496, 
			-1, -1, -1, 495, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 493, 494, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 557, -1, 557, -1, -1, -1, -1, 556, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 557, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 556, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 557, 1260, 557, 557, 557, 557, 557, 557, 557, -1, -1, -1, -1, -1, 556, 556, 556, 556, 556, 556, 556, 
			-1, -1, -1, 522, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 514, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 515, 516, 517, 519, 520, 521, 518, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 501, -1, -1, -1, -1, -1, -1, 506, -1, -1, 503, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 502, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 504, 505, -1, -1, -1, -1, -1, -1, -1, 507, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 508, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 509, 510, 511, 512, 513, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 523, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 526, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 529, 529, 529, 529, 528, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 529, 
			-1, -1, -1, -1, -1, 530, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 532, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, 532, 532, 532, -1, -1, -1, 532, 532, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 532, 532, 532, 532, 532, 532, 532, 532, 532, -1, -1, -1, -1, -1, -1, 532, 532, 532, 532, 532, 532, 532, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 533, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 535, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 539, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 540, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 541, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 545, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 546, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 547, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 160, -1, 160, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 160, 160, 160, 160, -1, -1, -1, 160, 160, -1, -1, -1, 538, -1, -1, -1, -1, -1, 160, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 160, 160, 160, 160, 160, 160, 160, 160, 160, -1, -1, -1, -1, -1, -1, 160, 160, 160, 160, 160, 160, 160, 160, -1, 160, 537, 160, 160, 160, 160, 160, 160, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 182, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 553, -1, 553, -1, -1, -1, -1, 552, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 553, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 550, 550, 550, 550, 550, 550, 551, 551, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 552, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 553, 1262, 553, 553, 553, 553, 553, 553, 553, -1, -1, -1, -1, -1, 552, 552, 552, 552, 552, 552, 552, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 558, 559, 560, 561, 562, 563, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 328, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1264, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 583, 583, 584, 585, 586, 586, 589, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 596, -1, 597, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 605, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 594, 595, 598, 599, 600, 601, 602, 603, 604, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 702, 702, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 708, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 715, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 716, 716, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 719, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 721, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 720, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 723, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 591, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 593, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, -1, -1, -1, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, 619, 619, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, 619, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 622, -1, -1, -1, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 622, 622, 622, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 622, 622, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 335, 335, 1265, 335, 1267, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 1284, 1285, 1286, 1287, 335, 335, 335, 1288, 1289, 335, 335, 335, 335, 335, 335, 335, 335, 335, 1290, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 335, 335, 335, 335, 335, 335, 1300, 1301, 1302, 1303, 1304, 1305, 1306, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 335, 
			-1, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 628, 
			-1, 630, 630, 630, 630, 630, 1307, 1308, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 630, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 639, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 633, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 634, 635, 643, 637, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 338, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 636, 1268, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 644, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 664, 666, 683, -1, 668, -1, 661, 660, 659, 652, 653, 654, 655, 656, 684, 645, 646, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 682, 681, 662, 663, 665, 667, 657, 658, 669, 670, 671, 672, 673, 674, 675, 676, 677, 678, 680, 647, 648, -1, -1, -1, -1, -1, -1, -1, 651, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1270, 679, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1272, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 700, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, -1, 694, -1, -1, -1, -1, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, 694, -1, -1, -1, -1, 694, 694, 694, 694, 694, 694, 694, 694, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, -1, 698, -1, -1, -1, -1, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 1274, 1276, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, 698, -1, -1, -1, -1, 699, 698, 698, 698, 698, 698, 698, 698, 
			-1, -1, -1, -1, -1, 703, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 704, 704, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 704, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 707, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 711, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 730, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 727, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 729, -1, -1, -1, -1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 729, 729, 729, 729, -1, -1, -1, 729, 729, -1, -1, -1, -1, -1, -1, -1, -1, -1, 729, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 729, 729, 729, 729, 729, 729, 729, 729, 729, -1, -1, -1, -1, -1, -1, 729, 729, 729, 729, 729, 729, 729, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 369, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 369, -1, -1, -1, 369, -1, 369, 369, 369, 369, 369, 369, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 369, -1, 369, 369, 369, 369, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, 369, -1, -1, -1, 369, 369, 369, 369, 369, 369, 369, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 30, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 375, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 375, -1, -1, -1, 375, -1, 375, 375, 375, 375, 375, 375, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 375, -1, 375, 375, 375, 375, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, 375, -1, -1, -1, 375, 375, 375, 375, 375, 375, 375, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 377, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 377, -1, -1, -1, 377, -1, 377, -1, -1, -1, 377, 377, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 377, -1, 377, 377, 377, 377, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, 377, -1, -1, -1, 377, 377, 377, 377, 377, 377, 377, 
			-1, -1, -1, -1, -1, -1, -1, 379, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 381, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 385, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 386, -1, -1, -1, 386, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 386, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 386, 386, 386, 386, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 386, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 386, 386, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 389, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 388, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 388, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 388, 388, 388, 388, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 388, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 388, 388, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 394, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 395, 0, 0, 0, 0, 0, 395, 0, 0, 0, 395, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, 397, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 399, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 401, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 403, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, 405, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 408, 408, 408, 0, 408, 0, 0, 0, 408, 408, 408, 408, 408, 0, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 408, 
			-1, 409, 409, 409, 409, 409, 0, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 409, 
			-1, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 0, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 411, 
			-1, 413, 413, 413, 0, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 413, 
			-1, -1, -1, -1, 0, -1, -1, 419, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, 421, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, 422, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 427, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 429, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 431, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 0, 433, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 435, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 0, 437, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 439, 439, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 443, 443, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 443, 443, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 449, 449, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 453, 453, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 457, 457, 457, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 462, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 464, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464, 464, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1152, 1152, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 491, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1154, 1154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 217, 217, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 500, 0, 0, 0, 0, 0, 0, 500, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 500, -1, -1, -1, -1, -1, -1, -1, 500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 524, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 527, 527, 527, 527, 527, 0, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 527, 
			-1, -1, -1, 531, -1, 531, 0, -1, -1, -1, -1, -1, -1, 531, -1, -1, -1, -1, -1, 531, 531, 531, 531, -1, -1, -1, 531, 531, -1, -1, 531, -1, 531, -1, -1, 531, -1, 531, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 531, 531, 531, 531, 531, 531, 531, 531, 531, -1, 531, -1, -1, -1, -1, 531, 531, 531, 531, 531, 531, 531, 531, -1, 531, -1, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, 531, -1, -1, -1, -1, -1, 531, 531, 531, 531, 531, 531, 531, 
			-1, -1, -1, 0, -1, 0, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, 0, 0, 0, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 534, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, 536, -1, 536, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 536, 536, 536, 536, -1, -1, -1, 536, 536, -1, -1, -1, 536, -1, -1, -1, -1, -1, 536, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 536, 536, 536, 536, 536, 536, 536, 536, 536, -1, -1, -1, -1, -1, -1, 536, 536, 536, 536, 536, 536, 536, 536, -1, 536, 536, 536, 536, 536, 536, 536, 536, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 542, -1, -1, -1, 542, 542, -1, -1, -1, -1, 159, -1, -1, -1, -1, -1, 542, 542, 542, 542, -1, -1, -1, 542, 542, -1, -1, 159, -1, 159, -1, -1, -1, -1, 1278, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 542, 542, 542, 542, 542, 542, 542, 542, 542, -1, 159, -1, -1, -1, -1, 542, 542, 542, 542, 542, 542, 542, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, -1, -1, -1, -1, -1, 159, 159, 159, 159, 159, 159, 159, 
			-1, -1, -1, 543, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 543, 543, 543, 543, -1, -1, -1, 543, 543, -1, -1, -1, -1, -1, -1, -1, -1, -1, 543, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 543, 543, 543, 543, 543, 543, 543, 543, 543, -1, -1, -1, -1, -1, -1, 543, 543, 543, 543, 543, 543, 543, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 544, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, 544, 544, 544, -1, -1, -1, 544, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 544, 544, 544, 544, 544, 544, 544, 544, 544, -1, -1, -1, -1, -1, -1, 544, 544, 544, 544, 544, 544, 544, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1191, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1191, 1191, 1191, 1191, -1, -1, -1, 1191, 1191, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1191, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1191, 1191, 1191, 1191, 1191, 1191, 1191, 1191, 1191, -1, -1, -1, -1, -1, -1, 1191, 1191, 1191, 1191, 1191, 1191, 1191, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1192, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1192, 1192, 1192, 1192, -1, -1, -1, 1192, 1192, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1192, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1192, 1192, 1192, 1192, 1192, 1192, 1192, 1192, 1192, -1, -1, -1, -1, -1, -1, 1192, 1192, 1192, 1192, 1192, 1192, 1192, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 1193, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1193, 1193, 1193, 1193, -1, -1, -1, 1193, 1193, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1193, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1193, 1193, 1193, 1193, 1193, 1193, 1193, 1193, 1193, -1, -1, -1, -1, -1, -1, 1193, 1193, 1193, 1193, 1193, 1193, 1193, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, 549, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 549, -1, 549, -1, -1, -1, -1, 1279, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 549, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 549, 0, 0, -1, -1, -1, 549, 549, 549, 549, 549, 549, 549, 
			-1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 555, -1, 555, -1, -1, -1, -1, 1280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 555, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 555, 0, 0, -1, -1, -1, 555, 555, 555, 555, 555, 555, 555, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, 179, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 588, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 181, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 590, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 0, 0, 592, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, -1, 618, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, 0, 0, -1, -1, -1, -1, -1, -1, 620, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 621, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 623, 0, 0, -1, 0, 0, 623, -1, -1, -1, 623, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, -1, 0, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 624, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 625, 
			-1, -1, -1, -1, -1, -1, 0, 629, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, -1, 0, 0, 0, 632, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 638, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 136, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, 686, -1, 686, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 686, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, 695, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 697, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, -1, -1, -1, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, 696, 696, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 696, 696, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 119, 1222, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 139, 1224, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1225, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, 705, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, 706, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1228, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, 179, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 709, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, 710, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 179, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 714, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 143, 1234, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1235, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, 179, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 717, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, 718, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1238, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 265, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 722, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 724, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 725, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 1281, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 728, -1, -1, 0, -1, 728, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 728, 728, 728, 728, -1, -1, -1, 728, 728, -1, -1, -1, -1, -1, -1, -1, -1, -1, 728, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 728, 728, 728, 728, 728, 728, 728, 728, 728, -1, -1, -1, -1, -1, -1, 728, 728, 728, 728, 728, 728, 728, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 731, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 731, -1, -1, -1, 731, -1, 731, -1, -1, -1, 731, 731, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 731, -1, 731, 731, 731, 731, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, 731, -1, -1, -1, 731, 731, 731, 731, 731, 731, 731, 
			-1, -1, 1282, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			
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
			269, 147,  -1, // 0
			732, 4, 1125, 3, 28, 2,  -1, // 1
			4, 151, 3, 29, 2,  -1, // 2
			733, 270,  -1, // 3
			734, 32,  -1, // 4
			736, 6, 271, 5, 735, 1127, 1126,  -1, // 5
			271, 147,  -1, // 6
			6, 272, 5, 737, 161, 34,  -1, // 7
			272, 162,  -1, // 8
			8, 273, 161, 35,  -1, // 9
			273, 161, 7,  -1, // 10
			8, 274, 161, 36,  -1, // 11
			274, 161, 7,  -1, // 12
			738, 9, 1128,  -1, // 13
			1023, 164,  -1, // 14
			275, 739, 1129,  -1, // 15
			275, 740, 1130, 10,  -1, // 16
			1024, 167,  -1, // 17
			1025, 168, 276, 1131,  -1, // 18
			1029, 168, 741, 277, 1027, 171, 1026, 170,  -1, // 19
			277, 1028, 172,  -1, // 20
			3, 1030, 170,  -1, // 21
			742, 8,  -1, // 22
			743, 1132,  -1, // 23
			745, 280, 279, 744, 1133,  -1, // 24
			279, 1031, 176,  -1, // 25
			280, 1032, 177,  -1, // 26
			281, 746, 1134,  -1, // 27
			281, 747, 1135, 7,  -1, // 28
			12, 282, 37, 11,  -1, // 29
			282, 180, 7,  -1, // 30
			12, 283, 174, 11,  -1, // 31
			283, 174, 7,  -1, // 32
			14, 183, 748, 13,  -1, // 33
			284, 184, 7,  -1, // 34
			284, 184,  -1, // 35
			185, 15,  -1, // 36
			749, 285, 1136,  -1, // 37
			286, 1033, 186,  -1, // 38
			286, 1033, 186,  -1, // 39
			287, 1034, 186,  -1, // 40
			6, 287, 1034, 186, 5,  -1, // 41
			288, 1035, 186,  -1, // 42
			14, 288, 1035, 186, 13,  -1, // 43
			289, 1036, 186,  -1, // 44
			4, 289, 3,  -1, // 45
			750, 1137,  -1, // 46
			1037, 188,  -1, // 47
			1038, 188,  -1, // 48
			290, 751, 1138,  -1, // 49
			290, 752, 1139, 7,  -1, // 50
			291, 753, 1140,  -1, // 51
			291, 754, 1141, 7,  -1, // 52
			292, 756, 1142, 7,  -1, // 53
			292, 756, 1142, 7, 755,  -1, // 54
			757, 1039, 188,  -1, // 55
			758, 1143,  -1, // 56
			293, 1040, 194,  -1, // 57
			761, 1042, 188, 9, 760, 1041, 188, 16, 759,  -1, // 58
			294, 1043, 195,  -1, // 59
			294, 762, 1044, 195, 38,  -1, // 60
			295, 1045, 196,  -1, // 61
			295, 763, 1046, 196, 39,  -1, // 62
			296, 1047, 197,  -1, // 63
			296, 764, 1048, 197, 17,  -1, // 64
			297, 1049, 198,  -1, // 65
			297, 765, 1050, 198, 18,  -1, // 66
			298, 1051, 199,  -1, // 67
			298, 766, 1052, 199, 19,  -1, // 68
			299, 1053, 200,  -1, // 69
			299, 767, 1054, 200, 1144,  -1, // 70
			768, 40,  -1, // 71
			769, 41,  -1, // 72
			300, 1055, 202,  -1, // 73
			300, 770, 1056, 202, 1145,  -1, // 74
			771, 11,  -1, // 75
			772, 12,  -1, // 76
			773, 42,  -1, // 77
			774, 43,  -1, // 78
			301, 1057, 204,  -1, // 79
			301, 775, 1058, 204, 1146,  -1, // 80
			776, 44,  -1, // 81
			777, 45,  -1, // 82
			302, 1059, 206,  -1, // 83
			302, 778, 1060, 206, 1147,  -1, // 84
			779, 20,  -1, // 85
			780, 21,  -1, // 86
			303, 1061, 208,  -1, // 87
			303, 781, 1062, 208, 1148,  -1, // 88
			782, 22,  -1, // 89
			783, 23,  -1, // 90
			784, 24,  -1, // 91
			304, 1063, 210,  -1, // 92
			304, 785, 1064, 210, 25,  -1, // 93
			305, 1065, 211,  -1, // 94
			786, 1066, 188, 1149,  -1, // 95
			787, 15,  -1, // 96
			788, 46,  -1, // 97
			789, 47,  -1, // 98
			790, 48,  -1, // 99
			791, 49,  -1, // 100
			792, 50,  -1, // 101
			793, 51,  -1, // 102
			794, 52,  -1, // 103
			795, 53,  -1, // 104
			796, 54,  -1, // 105
			797, 55,  -1, // 106
			798, 56,  -1, // 107
			1067, 213,  -1, // 108
			1068, 214,  -1, // 109
			799, 1069, 211, 20,  -1, // 110
			800, 1070, 211, 21,  -1, // 111
			801, 1071, 211, 26,  -1, // 112
			802, 1072, 211, 19,  -1, // 113
			803, 1073, 211, 22,  -1, // 114
			804, 1074, 211, 27,  -1, // 115
			805, 1075, 211, 57,  -1, // 116
			806, 1076, 211, 58,  -1, // 117
			807, 1150, 59,  -1, // 118
			808, 1151, 60,  -1, // 119
			809, 1077, 211, 61,  -1, // 120
			810, 1078, 211, 62,  -1, // 121
			811, 1079, 211, 4, 1153, 306, 3,  -1, // 122
			812, 1155, 63, 308,  -1, // 123
			813, 64,  -1, // 124
			814, 65,  -1, // 125
			1080, 218, 309, 3,  -1, // 126
			815, 1156,  -1, // 127
			816, 4, 1157, 3,  -1, // 128
			817, 1081, 211,  -1, // 129
			310, 1082, 219,  -1, // 130
			310, 1083, 220,  -1, // 131
			818, 4, 1158, 3,  -1, // 132
			819, 4, 1159, 3, 26,  -1, // 133
			820, 14, 1160, 13,  -1, // 134
			821, 57,  -1, // 135
			822, 58,  -1, // 136
			1084, 221, 10,  -1, // 137
			1085, 221, 823, 66,  -1, // 138
			824, 1161,  -1, // 139
			825, 1162, 67,  -1, // 140
			826, 68,  -1, // 141
			827, 69,  -1, // 142
			828, 70,  -1, // 143
			829, 71,  -1, // 144
			830, 1163,  -1, // 145
			831, 72,  -1, // 146
			832, 1164,  -1, // 147
			833, 1165,  -1, // 148
			834, 1166,  -1, // 149
			835, 75,  -1, // 150
			836, 76,  -1, // 151
			837, 77,  -1, // 152
			4, 1086, 188, 3,  -1, // 153
			311, 838, 1167,  -1, // 154
			311, 839, 1168,  -1, // 155
			1087, 223,  -1, // 156
			841, 1170, 312, 840, 1169,  -1, // 157
			312, 1088, 224,  -1, // 158
			1089, 223,  -1, // 159
			842, 1171,  -1, // 160
			844, 1173, 313, 843, 1172,  -1, // 161
			313, 158,  -1, // 162
			8, 187,  -1, // 163
			847, 314, 160, 845, 4, 1174, 3, 79,  -1, // 164
			160, 846, 80,  -1, // 165
			850, 1177, 315, 849, 1176, 848, 4, 1175, 3, 81,  -1, // 166
			315, 1090, 235,  -1, // 167
			851, 1180, 1179, 1178,  -1, // 168
			852, 1182, 1181,  -1, // 169
			854, 1185, 853, 1184, 1183, 3, 83,  -1, // 170
			8, 857, 4, 1187, 3, 83, 856, 160, 855, 1186,  -1, // 171
			864, 1190, 863, 4, 318, 8, 317, 316, 858, 3, 1188,  -1, // 172
			8, 191,  -1, // 173
			860, 1189, 859,  -1, // 174
			862, 190, 861,  -1, // 175
			865, 8, 319, 86,  -1, // 176
			866, 8, 320, 87,  -1, // 177
			867, 8, 321, 88,  -1, // 178
			322, 1091, 236,  -1, // 179
			322, 1091, 236,  -1, // 180
			868, 1194,  -1, // 181
			869, 1195,  -1, // 182
			870, 1196,  -1, // 183
			871, 1197,  -1, // 184
			323, 1092, 218,  -1, // 185
			323, 1092, 218,  -1, // 186
			872, 1198,  -1, // 187
			873, 1199,  -1, // 188
			874, 324, 89,  -1, // 189
			875, 90,  -1, // 190
			876, 325, 91,  -1, // 191
			877, 92,  -1, // 192
			878, 93,  -1, // 193
			879, 94,  -1, // 194
			880, 95,  -1, // 195
			881, 96,  -1, // 196
			882, 97,  -1, // 197
			883, 98,  -1, // 198
			884, 99,  -1, // 199
			885, 100,  -1, // 200
			886, 101,  -1, // 201
			887, 102,  -1, // 202
			888, 103,  -1, // 203
			889, 104,  -1, // 204
			890, 105,  -1, // 205
			891, 106,  -1, // 206
			892, 107,  -1, // 207
			893, 108,  -1, // 208
			894, 109,  -1, // 209
			895, 110,  -1, // 210
			896, 111,  -1, // 211
			897, 112,  -1, // 212
			898, 113,  -1, // 213
			899, 1200,  -1, // 214
			900, 1201,  -1, // 215
			901, 1202,  -1, // 216
			902, 1203,  -1, // 217
			903, 1204,  -1, // 218
			904, 1205,  -1, // 219
			905, 1206,  -1, // 220
			906, 327, 1207,  -1, // 221
			329, 908, 1208, 907,  -1, // 222
			909, 3,  -1, // 223
			910, 1209,  -1, // 224
			911, 114,  -1, // 225
			912, 115,  -1, // 226
			913, 30,  -1, // 227
			914, 32,  -1, // 228
			915, 116,  -1, // 229
			916, 117,  -1, // 230
			917, 118,  -1, // 231
			918, 119,  -1, // 232
			919, 120,  -1, // 233
			920, 121,  -1, // 234
			921, 122,  -1, // 235
			922, 67,  -1, // 236
			923, 123,  -1, // 237
			924, 124,  -1, // 238
			925, 125,  -1, // 239
			926, 126,  -1, // 240
			927, 127,  -1, // 241
			928, 128,  -1, // 242
			929, 129,  -1, // 243
			930, 130,  -1, // 244
			931, 131,  -1, // 245
			932, 132,  -1, // 246
			933, 133,  -1, // 247
			330, 1093, 250,  -1, // 248
			330, 1094, 250, 7,  -1, // 249
			936, 331, 1095, 1210,  -1, // 250
			935, 1096, 1211, 934, 15,  -1, // 251
			332, 1097, 176,  -1, // 252
			334, 333, 1098, 170, 332,  -1, // 253
			333, 1099, 177,  -1, // 254
			334, 1100, 172,  -1, // 255
			938, 1212,  -1, // 256
			939, 1213,  -1, // 257
			940, 6, 1101, 253, 5,  -1, // 258
			336, 1102, 254,  -1, // 259
			336, 1103, 254, 7,  -1, // 260
			1104, 252,  -1, // 261
			337, 941, 1105, 1214,  -1, // 262
			337, 1106, 255, 10,  -1, // 263
			942, 1215,  -1, // 264
			943, 69,  -1, // 265
			944, 70,  -1, // 266
			945, 134,  -1, // 267
			946, 72,  -1, // 268
			947, 72, 90,  -1, // 269
			948, 72, 26,  -1, // 270
			949, 135,  -1, // 271
			950, 1216, 135,  -1, // 272
			951, 1217, 135,  -1, // 273
			952, 71,  -1, // 274
			72, 90,  -1, // 275
			953, 26,  -1, // 276
			954, 27,  -1, // 277
			955, 57,  -1, // 278
			956, 58,  -1, // 279
			957, 57, 136,  -1, // 280
			958, 58, 136,  -1, // 281
			959, 66, 339,  -1, // 282
			960, 20,  -1, // 283
			961, 21,  -1, // 284
			962, 22,  -1, // 285
			963, 23,  -1, // 286
			964, 24,  -1, // 287
			965, 44,  -1, // 288
			966, 45,  -1, // 289
			967, 19,  -1, // 290
			968, 18,  -1, // 291
			969, 17,  -1, // 292
			970, 40,  -1, // 293
			971, 41,  -1, // 294
			972, 11,  -1, // 295
			973, 42,  -1, // 296
			974, 12,  -1, // 297
			975, 43,  -1, // 298
			976, 15,  -1, // 299
			977, 46,  -1, // 300
			978, 47,  -1, // 301
			979, 48,  -1, // 302
			980, 49,  -1, // 303
			981, 50,  -1, // 304
			982, 51,  -1, // 305
			983, 52,  -1, // 306
			984, 53,  -1, // 307
			985, 54,  -1, // 308
			986, 55,  -1, // 309
			987, 137,  -1, // 310
			988, 56,  -1, // 311
			989, 39,  -1, // 312
			990, 38,  -1, // 313
			991, 14, 13,  -1, // 314
			992, 25,  -1, // 315
			340, 993, 22,  -1, // 316
			340, 994, 1218,  -1, // 317
			1107, 258,  -1, // 318
			1108, 171,  -1, // 319
			1109, 259,  -1, // 320
			995, 14, 1219, 13,  -1, // 321
			996, 14, 13,  -1, // 322
			4, 1110, 260, 3,  -1, // 323
			4, 3,  -1, // 324
			341, 1111, 261,  -1, // 325
			341, 1112, 261, 7,  -1, // 326
			342, 1113, 1221,  -1, // 327
			185, 15,  -1, // 328
			997, 343, 1220,  -1, // 329
			998, 138, 344,  -1, // 330
			999, 1223, 9,  -1, // 331
			1000, 116,  -1, // 332
			1114, 262, 1001, 346, 345,  -1, // 333
			1003, 6, 1115, 263, 1002, 5,  -1, // 334
			347, 1116, 264,  -1, // 335
			347, 1117, 264, 7,  -1, // 336
			1227, 15,  -1, // 337
			1004, 348, 1226,  -1, // 338
			1118, 266, 1005, 352, 351, 349, 141,  -1, // 339
			350, 1229,  -1, // 340
			1230, 9,  -1, // 341
			1006, 4, 1231, 3, 28,  -1, // 342
			1007, 1232,  -1, // 343
			1008, 31,  -1, // 344
			353, 1233,  -1, // 345
			1119, 266, 1009, 354, 142,  -1, // 346
			1120, 266, 1010, 359, 358, 356, 355,  -1, // 347
			357, 1236,  -1, // 348
			1237, 9,  -1, // 349
			1011, 1121, 266, 126,  -1, // 350
			1122, 266, 1012, 1239, 360, 126,  -1, // 351
			5, 37, 361, 126,  -1, // 352
			1013, 1240,  -1, // 353
			365, 1014, 363, 362, 145,  -1, // 354
			1123, 171,  -1, // 355
			1016, 1241,  -1, // 356
			1124, 267,  -1, // 357
			1018, 6, 366, 5, 1017,  -1, // 358
			366, 268,  -1, // 359
			1020, 8, 1242, 1019,  -1, // 360
			1022, 6, 367, 1021, 5,  -1, // 361
			367, 162,  -1, // 362
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 10, 16, 19, 22, 30, 33, 40, 43, 48, 52, 57, 61, 65, 68, 72, 77, 80, 85, 94, 98, 102, 105, 108, 114, 118, 122, 126, 131, 136, 140, 145, 149, 154, 158, 161, 164, 168, 172, 176, 180, 186, 190, 196, 200, 204, 207, 210, 213, 217, 222, 226, 231, 236, 242, 246, 249, 253, 263, 267, 273, 277, 283, 287, 293, 297, 303, 307, 313, 317, 323, 326, 329, 333, 339, 342, 345, 348, 351, 355, 361, 364, 367, 371, 377, 380, 383, 387, 393, 396, 399, 402, 406, 412, 416, 421, 424, 427, 430, 433, 436, 439, 442, 445, 448, 451, 454, 457, 460, 463, 468, 473, 478, 483, 488, 493, 498, 503, 507, 511, 516, 521, 529, 534, 537, 540, 545, 548, 553, 557, 561, 565, 570, 576, 581, 584, 587, 591, 596, 599, 603, 606, 609, 612, 615, 618, 621, 624, 627, 630, 633, 636, 639, 644, 648, 652, 655, 661, 665, 668, 671, 677, 680, 683, 692, 696, 707, 711, 716, 720, 728, 739, 751, 754, 758, 762, 767, 772, 777, 781, 785, 788, 791, 794, 797, 801, 805, 808, 811, 815, 818, 822, 825, 828, 831, 834, 837, 840, 843, 846, 849, 852, 855, 858, 861, 864, 867, 870, 873, 876, 879, 882, 885, 888, 891, 894, 897, 900, 903, 906, 909, 913, 918, 921, 924, 927, 930, 933, 936, 939, 942, 945, 948, 951, 954, 957, 960, 963, 966, 969, 972, 975, 978, 981, 984, 987, 990, 993, 997, 1002, 1007, 1013, 1017, 1023, 1027, 1031, 1034, 1037, 1043, 1047, 1052, 1055, 1060, 1065, 1068, 1071, 1074, 1077, 1080, 1084, 1088, 1091, 1095, 1099, 1102, 1105, 1108, 1111, 1114, 1117, 1121, 1125, 1129, 1132, 1135, 1138, 1141, 1144, 1147, 1150, 1153, 1156, 1159, 1162, 1165, 1168, 1171, 1174, 1177, 1180, 1183, 1186, 1189, 1192, 1195, 1198, 1201, 1204, 1207, 1210, 1213, 1216, 1219, 1222, 1226, 1229, 1233, 1237, 1240, 1243, 1246, 1251, 1255, 1260, 1263, 1267, 1272, 1276, 1279, 1283, 1287, 1291, 1294, 1300, 1307, 1311, 1316, 1319, 1323, 1331, 1334, 1337, 1343, 1346, 1349, 1352, 1358, 1366, 1369, 1372, 1377, 1384, 1389, 1392, 1398, 1401, 1404, 1407, 1413, 1416, 1421, 1427, 
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
		
		case '{':
			return 5;
		
		case '}':
			return 6;
		
		case ',':
			return 7;
		
		case ';':
			return 8;
		
		case ':':
			return 9;
		
		case '.':
			return 10;
		
		case '<':
			return 11;
		
		case '>':
			return 12;
		
		case '[':
			return 13;
		
		case ']':
			return 14;
		
		case '=':
			return 15;
		
		case '?':
			return 16;
		
		case '|':
			return 17;
		
		case '^':
			return 18;
		
		case '&':
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
		
		case EToken_Endian:
			return 29;
		
		case EToken_LittleEndian:
			return 30;
		
		case EToken_Default:
			return 31;
		
		case EToken_BigEndian:
			return 32;
		
		case EToken_Namespace:
			return 33;
		
		case EToken_Extend:
			return 34;
		
		case EToken_Using:
			return 35;
		
		case EToken_Friend:
			return 36;
		
		case EToken_Identifier:
			return 37;
		
		case EToken_LogOr:
			return 38;
		
		case EToken_LogAnd:
			return 39;
		
		case EToken_Eq:
			return 40;
		
		case EToken_Ne:
			return 41;
		
		case EToken_Le:
			return 42;
		
		case EToken_Ge:
			return 43;
		
		case EToken_Shl:
			return 44;
		
		case EToken_Shr:
			return 45;
		
		case EToken_AddAssign:
			return 46;
		
		case EToken_SubAssign:
			return 47;
		
		case EToken_MulAssign:
			return 48;
		
		case EToken_DivAssign:
			return 49;
		
		case EToken_ModAssign:
			return 50;
		
		case EToken_ShlAssign:
			return 51;
		
		case EToken_ShrAssign:
			return 52;
		
		case EToken_AndAssign:
			return 53;
		
		case EToken_XorAssign:
			return 54;
		
		case EToken_OrAssign:
			return 55;
		
		case EToken_ClassAssign:
			return 56;
		
		case EToken_Inc:
			return 57;
		
		case EToken_Dec:
			return 58;
		
		case EToken_SizeOf:
			return 59;
		
		case EToken_TypeOf:
			return 60;
		
		case EToken_Snapshot:
			return 61;
		
		case EToken_Dynamic:
			return 62;
		
		case EToken_New:
			return 63;
		
		case EToken_Stack:
			return 64;
		
		case EToken_Heap:
			return 65;
		
		case EToken_Ptr:
			return 66;
		
		case EToken_Weak:
			return 67;
		
		case EToken_OnChange:
			return 68;
		
		case EToken_Get:
			return 69;
		
		case EToken_Set:
			return 70;
		
		case EToken_PropValue:
			return 71;
		
		case EToken_This:
			return 72;
		
		case EToken_Integer:
			return 73;
		
		case EToken_Fp:
			return 74;
		
		case EToken_True:
			return 75;
		
		case EToken_False:
			return 76;
		
		case EToken_Null:
			return 77;
		
		case EToken_Literal:
			return 78;
		
		case EToken_If:
			return 79;
		
		case EToken_Else:
			return 80;
		
		case EToken_Switch:
			return 81;
		
		case EToken_Case:
			return 82;
		
		case EToken_While:
			return 83;
		
		case EToken_Do:
			return 84;
		
		case EToken_For:
			return 85;
		
		case EToken_Break:
			return 86;
		
		case EToken_Continue:
			return 87;
		
		case EToken_Return:
			return 88;
		
		case EToken_Typedef:
			return 89;
		
		case EToken_Static:
			return 90;
		
		case EToken_Abstract:
			return 91;
		
		case EToken_Virtual:
			return 92;
		
		case EToken_NoVirtual:
			return 93;
		
		case EToken_Mutable:
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
		
		case EToken_Int:
			return 101;
		
		case EToken_IntPtr:
			return 102;
		
		case EToken_Char:
			return 103;
		
		case EToken_Short:
			return 104;
		
		case EToken_Long:
			return 105;
		
		case EToken_Float:
			return 106;
		
		case EToken_Double:
			return 107;
		
		case EToken_Int8:
			return 108;
		
		case EToken_Int16:
			return 109;
		
		case EToken_Int32:
			return 110;
		
		case EToken_Int64:
			return 111;
		
		case EToken_WChar:
			return 112;
		
		case EToken_Auto:
			return 113;
		
		case EToken_Signed:
			return 114;
		
		case EToken_Unsigned:
			return 115;
		
		case EToken_Const:
			return 116;
		
		case EToken_ReadOnly:
			return 117;
		
		case EToken_Volatile:
			return 118;
		
		case EToken_Safe:
			return 119;
		
		case EToken_Unsafe:
			return 120;
		
		case EToken_NoNull:
			return 121;
		
		case EToken_Strong:
			return 122;
		
		case EToken_Cdecl:
			return 123;
		
		case EToken_Stdcall:
			return 124;
		
		case EToken_Function:
			return 125;
		
		case EToken_Property:
			return 126;
		
		case EToken_Multicast:
			return 127;
		
		case EToken_Event:
			return 128;
		
		case EToken_Bindable:
			return 129;
		
		case EToken_AutoGet:
			return 130;
		
		case EToken_Indexed:
			return 131;
		
		case EToken_Closure:
			return 132;
		
		case EToken_Thin:
			return 133;
		
		case EToken_PreConstruct:
			return 134;
		
		case EToken_Operator:
			return 135;
		
		case EToken_Postfix:
			return 136;
		
		case EToken_AtAssign:
			return 137;
		
		case EToken_Ellipsis:
			return 138;
		
		case EToken_Enum:
			return 139;
		
		case EToken_EnumC:
			return 140;
		
		case EToken_Struct:
			return 141;
		
		case EToken_Union:
			return 142;
		
		case EToken_Class:
			return 143;
		
		case EToken_Interface:
			return 144;
		
		case EToken_AutoEv:
			return 145;
		
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
			'{', 
			'}', 
			',', 
			';', 
			':', 
			'.', 
			'<', 
			'>', 
			'[', 
			']', 
			'=', 
			'?', 
			'|', 
			'^', 
			'&', 
			'+', 
			'-', 
			'*', 
			'/', 
			'%', 
			'@', 
			'~', 
			'!', 
			EToken_Pack, 
			EToken_Endian, 
			EToken_LittleEndian, 
			EToken_Default, 
			EToken_BigEndian, 
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
			EToken_Snapshot, 
			EToken_Dynamic, 
			EToken_New, 
			EToken_Stack, 
			EToken_Heap, 
			EToken_Ptr, 
			EToken_Weak, 
			EToken_OnChange, 
			EToken_Get, 
			EToken_Set, 
			EToken_PropValue, 
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
			EToken_While, 
			EToken_Do, 
			EToken_For, 
			EToken_Break, 
			EToken_Continue, 
			EToken_Return, 
			EToken_Typedef, 
			EToken_Static, 
			EToken_Abstract, 
			EToken_Virtual, 
			EToken_NoVirtual, 
			EToken_Mutable, 
			EToken_Public, 
			EToken_Protected, 
			EToken_Void, 
			EToken_Variant, 
			EToken_Object, 
			EToken_Bool, 
			EToken_Int, 
			EToken_IntPtr, 
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
			EToken_Signed, 
			EToken_Unsigned, 
			EToken_Const, 
			EToken_ReadOnly, 
			EToken_Volatile, 
			EToken_Safe, 
			EToken_Unsafe, 
			EToken_NoNull, 
			EToken_Strong, 
			EToken_Cdecl, 
			EToken_Stdcall, 
			EToken_Function, 
			EToken_Property, 
			EToken_Multicast, 
			EToken_Event, 
			EToken_Bindable, 
			EToken_AutoGet, 
			EToken_Indexed, 
			EToken_Closure, 
			EToken_Thin, 
			EToken_PreConstruct, 
			EToken_Operator, 
			EToken_Postfix, 
			EToken_AtAssign, 
			EToken_Ellipsis, 
			EToken_Enum, 
			EToken_EnumC, 
			EToken_Struct, 
			EToken_Union, 
			EToken_Class, 
			EToken_Interface, 
			EToken_AutoEv, 
			
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
			_T("pragma_endian"),
			_T("pragma_endian_value"),
			_T("namespace_declaration"),
			_T("extend_declaration"),
			_T("using_declaration"),
			_T("friend_declaration"),
			_T("access_declaration"),
			_T("item_declaration"),
			_T("local_declaration"),
			_T("item_declaration_w_specifier"),
			_T("statement"),
			_T("qualified_name"),
			_T("named_type_block_declaration"),
			_T("access_specifier"),
			_T("qualified_name_impl"),
			_T("item_declaration_no_specifier"),
			_T("declaration_specifier_list"),
			_T("declarator_list"),
			_T("declaration_terminator"),
			_T("item_declaration_no_specifier_rslv"),
			_T("declarator_name"),
			_T("function_suffix"),
			_T("post_declarator_modifier"),
			_T("function_body_pass1"),
			_T("type_name"),
			_T("type_specifier_modifier_list"),
			_T("pointer"),
			_T("declarator_suffix"),
			_T("type_name_list"),
			_T("generic_formal_argument_list"),
			_T("generic_formal_argument"),
			_T("generic_actual_argument_list"),
			_T("attribute_block"),
			_T("attribute_declarator_list"),
			_T("attribute_declarator"),
			_T("expression_pass1"),
			_T("primary_expr_pass1"),
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
			_T("assignment_operator"),
			_T("postfix_expr"),
			_T("unary_operator_expr"),
			_T("type_name_or_expr"),
			_T("cast_operator_rslv"),
			_T("allocation_specifier"),
			_T("type_specifier_modifier"),
			_T("primary_expr"),
			_T("postfix_operator"),
			_T("member_operator"),
			_T("literal"),
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
			_T("declaration_specifier"),
			_T("storage_specifier"),
			_T("type_specifier"),
			_T("type_modifier"),
			_T("enum_specifier"),
			_T("struct_specifier"),
			_T("union_specifier"),
			_T("class_specifier"),
			_T("property_template_specifier"),
			_T("property_specifier_rslv"),
			_T("property_specifier"),
			_T("autoev_specifier"),
			_T("qualified_type_name_rslv"),
			_T("qualified_type_name"),
			_T("full_declarator"),
			_T("declarator"),
			_T("initializer"),
			_T("initializer_list"),
			_T("initializer_list_entry"),
			_T("declarator_qualifier"),
			_T("static_this_rslv"),
			_T("operator_name"),
			_T("array_suffix"),
			_T("bitfield_suffix"),
			_T("function_formal_argument_list"),
			_T("function_formal_argument"),
			_T("enum_block"),
			_T("enum_member_list"),
			_T("enum_member"),
			_T("struct_pack_factor"),
			_T("named_type_block"),
			_T("autoev_body"),
			_T("autoev_stmt"),
			
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls38);
				
			break;
			
		
		
		
		
		
		
		
		
		
		
		case ESymbol_item_declaration_w_specifier:
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls28);
				
			break;
			
		
		case ESymbol_qualified_name_impl:
			pNode = AXL_MEM_NEW (CSymbolNode_qualified_name_impl);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_item_declaration_no_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls2);
				
			break;
			
		
		case ESymbol_declaration_specifier_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls25);
				
			break;
			
		
		case ESymbol_declarator_list:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declaration_terminator:
			pNode = AXL_MEM_NEW (CSymbolNode_declaration_terminator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_item_declaration_no_specifier_rslv:
			pNode = AXL_MEM_NEW (CSymbolNode_item_declaration_no_specifier_rslv);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declarator_name:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_name);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_function_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_function_suffix);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_post_declarator_modifier:
			pNode = AXL_MEM_NEW (CSymbolNode_post_declarator_modifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_function_body_pass1:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls22);
				
			break;
			
		
		case ESymbol_type_name:
			pNode = AXL_MEM_NEW (CSymbolNode_type_name);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls3);
			
			break;
			
		
		case ESymbol_type_specifier_modifier_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls26);
				
			break;
			
		
		case ESymbol_pointer:
			pNode = AXL_MEM_NEW (CSymbolNode_pointer);
			
			break;
			
		
		case ESymbol_declarator_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_suffix);
			
			break;
			
		
		case ESymbol_type_name_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls4);
				
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
			
			pNode->m_pAstNode = AXL_MEM_NEW (CConditionalExpr);
			
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
			
		
		case ESymbol_assignment_operator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls18);
				
			break;
			
		
		case ESymbol_postfix_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_postfix_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_unary_operator_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_unary_operator_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_type_name_or_expr:
			pNode = AXL_MEM_NEW (CSymbolNode_type_name_or_expr);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls20);
			
			break;
			
		
		case ESymbol_cast_operator_rslv:
			pNode = AXL_MEM_NEW (CSymbolNode_cast_operator_rslv);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_allocation_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls19);
				
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
			
		
		case ESymbol_compound_stmt_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_compound_stmt_pass1);
			
			break;
			
		
		case ESymbol_statement_pass1:
			pNode = AXL_MEM_NEW (CSymbolNode_statement_pass1);
			
			break;
			
		
		case ESymbol_compound_stmt:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (CCompoundStmt);
				
			break;
			
		
		
		case ESymbol_if_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_if_stmt);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_switch_stmt:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (CSwitchStmt);
				
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
			
		
		
		
		
		case ESymbol_switch_block_stmt:
			pNode = AXL_MEM_NEW (CSymbolNode_switch_block_stmt);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declaration_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_declaration_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_storage_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls27);
				
			break;
			
		
		case ESymbol_type_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls29);
				
			break;
			
		
		case ESymbol_type_modifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls31);
				
			break;
			
		
		case ESymbol_enum_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls35);
				
			break;
			
		
		case ESymbol_struct_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls36);
				
			break;
			
		
		case ESymbol_union_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls39);
				
			break;
			
		
		case ESymbol_class_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls40);
				
			break;
			
		
		case ESymbol_property_template_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_property_template_specifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls42);
			
			break;
			
		
		
		case ESymbol_property_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls43);
				
			break;
			
		
		case ESymbol_autoev_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls44);
				
			break;
			
		
		
		case ESymbol_qualified_type_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls30);
				
			break;
			
		
		case ESymbol_full_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_full_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls32);
			
			break;
			
		
		case ESymbol_initializer:
			pNode = AXL_MEM_NEW (CSymbolNode_initializer);
			
			pNode->m_pAstNode = AXL_MEM_NEW (_cls33);
			
			break;
			
		
		case ESymbol_initializer_list:
			pNode = AXL_MEM_NEW (CSymbolNode_initializer_list);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_initializer_list_entry:
			pNode = AXL_MEM_NEW (CSymbolNode_initializer_list_entry);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_declarator_qualifier:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_qualifier);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		
		case ESymbol_operator_name:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_Named;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls34);
				
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
				
			pNode->m_pAstNode = AXL_MEM_NEW (_cls37);
				
			break;
			
		
		case ESymbol_named_type_block:
			pNode = AXL_MEM_NEW (CSymbolNode_named_type_block);
			
			pNode->m_pAstNode = AXL_MEM_NEW (CAstNode);
			
			break;
			
		
		case ESymbol_autoev_body:
			pNode = AXL_MEM_NEW (CSymbolNode_autoev_body);
			
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
			
			{ 0, 149 },
			
			{ 0, 33 },
			
			{ 1, 161 },
			
			{ 0, 163 },
			
			{ 0, 37 },
			
			{ 1, 37 },
			
			{ 0, 166 },
			
			{ 0, 173 },
			
			{ 0, 175 },
			
			{ 0, 174 },
			
			{ 1, 174 },
			
			{ 0, 37 },
			
			{ 0, 1 },
			
			{ 0, 187 },
			
			{ 1, 187 },
			
			{ 0, 187 },
			
			{ 1, 189 },
			
			{ 0, 189 },
			
			{ 0, 187 },
			
			{ 0, 201 },
			
			{ 0, 203 },
			
			{ 0, 205 },
			
			{ 0, 207 },
			
			{ 0, 209 },
			
			{ 0, 212 },
			
			{ 0, 215 },
			
			{ 0, 215 },
			
			{ 0, 217 },
			
			{ 1, 174 },
			
			{ 0, 217 },
			
			{ 1, 174 },
			
			{ 0, 174 },
			
			{ 0, 174 },
			
			{ 0, 191 },
			
			{ 0, 191 },
			
			{ 0, 187 },
			
			{ 0, 37 },
			
			{ 0, 37 },
			
			{ 0, 37 },
			
			{ 0, 73 },
			
			{ 0, 74 },
			
			{ 0, 222 },
			
			{ 0, 78 },
			
			{ 1, 78 },
			
			{ 0, 5 },
			
			{ 1, 6 },
			
			{ 0, 1 },
			
			{ 0, 5 },
			
			{ 1, 6 },
			
			{ 0, 187 },
			
			{ 0, 187 },
			
			{ 1, 5 },
			
			{ 2, 6 },
			
			{ 1, 82 },
			
			{ 0, 193 },
			
			{ 2, 9 },
			
			{ 0, 31 },
			
			{ 1, 9 },
			
			{ 1, 187 },
			
			{ 0, 4 },
			
			{ 2, 160 },
			
			{ 0, 84 },
			
			{ 1, 187 },
			
			{ 0, 85 },
			
			{ 1, 187 },
			
			{ 2, 160 },
			
			{ 0, 193 },
			
			{ 0, 193 },
			
			{ 0, 187 },
			
			{ 0, 237 },
			
			{ 0, 163 },
			
			{ 0, 238 },
			
			{ 0, 239 },
			
			{ 0, 238 },
			
			{ 0, 239 },
			
			{ 0, 240 },
			
			{ 0, 241 },
			
			{ 0, 242 },
			
			{ 0, 243 },
			
			{ 0, 244 },
			
			{ 0, 246 },
			
			{ 0, 247 },
			
			{ 0, 249 },
			
			{ 0, 161 },
			
			{ 0, 161 },
			
			{ 0, 251 },
			
			{ 1, 252 },
			
			{ 0, 185 },
			
			{ 0, 187 },
			
			{ 0, 255 },
			
			{ 0, 37 },
			
			{ 0, 174 },
			
			{ 0, 257 },
			
			{ 0, 239 },
			
			{ 0, 73 },
			
			{ 0, 175 },
			
			{ 1, 251 },
			
			{ 0, 120 },
			
			{ 0, 73 },
			
			{ 1, 140 },
			
			{ 0, 37 },
			
			{ 0, 37 },
			
			{ 1, 185 },
			
			{ 2, 265 },
			
			{ 0, 37 },
			
			{ 1, 178 },
			
			{ 0, 149 },
			
			{ 0, 73 },
			
			{ 0, 37 },
			
			{ 3, 144 },
			
			{ 2, 265 },
			
			{ 0, 37 },
			
			{ 1, 178 },
			
			{ 1, 265 },
			
			{ 0, 37 },
			
			{ 0, 37 },
			
			{ 1, 173 },
			
			{ 0, 187 },
			
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
#line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			m_StructPackFactor = (*(_cls38*) GetAstLocator (0)).m_Value;
		;
#line 4719 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 22 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			m_Endianness = EEndianness_LittleEndian;
		;
#line 4732 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			m_Endianness = EEndianness_BigEndian;
		;
#line 4745 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 56 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			CNamespace* pNamespace = m_pModule->m_NamespaceMgr.OpenNamespace ((*GetTokenLocator (0)).m_Pos, (*(_cls1*) GetAstLocator (1)).m_Name);
			if (!pNamespace)
				return false;
		;
#line 4760 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 62 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			m_pModule->m_NamespaceMgr.CloseNamespace ((*(_cls1*) GetAstLocator (1)).m_Name.m_List.GetCount () + 1);
		;
#line 4773 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 5:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 69 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			err::SetFormatStringError (_T("named type extensions are not supported yet"));
			return false;
		;
#line 4787 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 6:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 86 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->m_CurrentAccessKind = (*(_cls28*) GetAstLocator (0)).m_AccessKind;
		;
#line 4800 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 7:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 103 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			  
			__pSymbol->m_Arg.pName->m_First = (*GetTokenLocator (0)).m_Data.m_String; 
		;
#line 4813 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 8:
			{
			CSymbolNode_qualified_name_impl* __pSymbol = (CSymbolNode_qualified_name_impl*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 107 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			__pSymbol->m_Arg.pName->m_List.InsertTail ((*GetTokenLocator (1)).m_Data.m_String);
		;
#line 4826 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 9:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
#line 152 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			return Declare (&(*__pAstNode).m_Declarator);
		;
#line 4839 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 10:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 183 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			return IsEmptyDeclarationTerminatorAllowed (__pSymbol->m_Arg.pTypeSpecifier);
		;
#line 4852 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 11:
			{
			CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 187 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			return SetFunctionBody (&(*(_cls22*) GetAstLocator (0)).m_TokenList);
		;
#line 4865 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 12:
			{
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
#line 206 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			__pSymbol->m_Local.Declarator.SetTypeSpecifier (&(*(_cls26*) GetAstLocator (0)).m_TypeSpecifier);
		;
#line 4878 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 13:
			{
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
#line 211 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_pType = __pSymbol->m_Local.Declarator.CalcType ();
		;
#line 4891 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 14:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
#line 222 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_TypeList.InsertTail ((*(_cls3*) GetAstLocator (0)).m_pType);
		;
#line 4904 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 15:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAstNode = (_cls4*) __pSymbol->m_pAstNode;
#line 226 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			(*__pAstNode).m_TypeList.InsertTail ((*(_cls3*) GetAstLocator (1)).m_pType);
		;
#line 4917 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 16:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 259 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			m_pAttributeBlock = m_pModule->m_AttributeMgr.CreateAttributeBlock ();
		;
#line 4930 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 17:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			
			CAttribute* pAttribute = m_pAttributeBlock->CreateAttribute ((*GetTokenLocator (0)).m_Data.m_String, NULL);
			if (!pAttribute)
				return false;

			pAttribute->m_Pos = (*GetTokenLocator (0)).m_Pos;
		;
#line 4947 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 18:
			{
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
#line 4960 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 19:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 49 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_ValueList.InsertTail ((*(_cls6*) GetAstLocator (0)).m_Value);
		;
#line 4973 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 20:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAstNode = (_cls8*) __pSymbol->m_pAstNode;
#line 54 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls6*) GetAstLocator (1)).m_Value);
			;
#line 4986 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 21:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 70 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_ValueList.InsertTail ((*(_cls6*) GetAstLocator (0)).m_Value);
		;
#line 4999 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 22:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 75 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls7*) GetAstLocator (1)).m_Value);
			;
#line 5012 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 23:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 79 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_ValueList.InsertTail (CValue ());
		;
#line 5025 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 24:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAstNode = (_cls9*) __pSymbol->m_pAstNode;
#line 84 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_ValueList.InsertTail ((*(_cls7*) GetAstLocator (0)).m_Value);
			;
#line 5038 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 25:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
#line 98 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			if ((*__pAstNode).m_Value.GetValueKind () != EValue_Const)
			{
				err::SetFormatStringError (_T("not a constant expression"));
				return false;
			}
		;
#line 5055 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 26:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAstNode = (_cls11*) __pSymbol->m_pAstNode;
#line 114 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
									
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
#line 5078 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 27:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
#line 142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("cond_then"));
			(*__pAstNode).m_pElseBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("cond_else"));
			(*__pAstNode).m_pPhiBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("cond_phi"));			
			m_pModule->m_ControlFlowMgr.ConditionalJump (*__pSymbol->m_Arg.pValue, (*__pAstNode).m_pThenBlock, (*__pAstNode).m_pElseBlock);
		;
#line 5094 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 28:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
#line 150 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				(*__pAstNode).m_pThenBlock = m_pModule->m_ControlFlowMgr.SetCurrentBlock ((*__pAstNode).m_pElseBlock); // might have changed				
			;
#line 5107 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 29:
			{
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
#line 155 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				FinalizeConditionalExpr (&(*__pAstNode), __pSymbol->m_Arg.pValue);
			;
#line 5120 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 30:
			{
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 169 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5133 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 31:
			{
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 183 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_LogOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5146 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 32:
			{
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 197 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwOr, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5159 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 33:
			{
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 211 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwXor, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5172 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 34:
			{
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 225 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_BwAnd, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5185 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 35:
			{
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 239 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls13*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5198 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 36:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 251 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Eq;
		;
#line 5211 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 37:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAstNode = (_cls13*) __pSymbol->m_pAstNode;
#line 255 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ne;
		;
#line 5224 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 38:
			{
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 268 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls14*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5237 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 39:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
#line 280 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Lt;
		;
#line 5250 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 40:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
#line 284 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Gt;
		;
#line 5263 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 41:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
#line 288 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Le;
		;
#line 5276 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 42:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAstNode = (_cls14*) __pSymbol->m_pAstNode;
#line 292 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Ge;
		;
#line 5289 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 43:
			{
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 305 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls15*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 44:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
#line 317 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shl;
		;
#line 5315 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 45:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAstNode = (_cls15*) __pSymbol->m_pAstNode;
#line 321 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Shr;
		;
#line 5328 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 46:
			{
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 334 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls16*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5341 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 47:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
#line 346 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Add;
		;
#line 5354 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 48:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAstNode = (_cls16*) __pSymbol->m_pAstNode;
#line 350 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Sub;
		;
#line 5367 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 49:
			{
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 363 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls17*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5380 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 50:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
#line 375 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mul;
		;
#line 5393 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 51:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
#line 379 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Div;
		;
#line 5406 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 52:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls17* __pAstNode = (_cls17*) __pSymbol->m_pAstNode;
#line 383 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Mod;
		;
#line 5419 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 53:
			{
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 396 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_At, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5432 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 54:
			{
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 410 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
				return m_pModule->m_OperatorMgr.BinaryOperator ((*(_cls18*) GetAstLocator (0)).m_OpKind, __pSymbol->m_Arg.pValue, __pSymbol->m_Local.OpValue2);
			;
#line 5445 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 55:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 422 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_Assign;
		;
#line 5458 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 56:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 426 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_AddAssign;
		;
#line 5471 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 57:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 430 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_SubAssign;
		;
#line 5484 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 58:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 434 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_MulAssign;
		;
#line 5497 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 59:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 438 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_DivAssign;
		;
#line 5510 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 60:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 442 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_ModAssign;
		;
#line 5523 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 61:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 446 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_ShlAssign;
		;
#line 5536 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 62:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 450 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_ShrAssign;
		;
#line 5549 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 63:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 454 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_AndAssign;
		;
#line 5562 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 64:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 458 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_XorAssign;
		;
#line 5575 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 65:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 462 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_OrAssign;
		;
#line 5588 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 66:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAstNode = (_cls18*) __pSymbol->m_pAstNode;
#line 466 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_OpKind = EBinOp_ClassAssign;
		;
#line 5601 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 67:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 483 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Plus, __pSymbol->m_Arg.pValue);
		;
#line 5614 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 68:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 487 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Minus, __pSymbol->m_Arg.pValue);
		;
#line 5627 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 69:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 491 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_BwNot, __pSymbol->m_Arg.pValue);
		;
#line 5640 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 70:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 495 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, __pSymbol->m_Arg.pValue);
		;
#line 5653 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 71:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 499 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, __pSymbol->m_Arg.pValue);
		;
#line 5666 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 72:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 503 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_LogNot, __pSymbol->m_Arg.pValue);
		;
#line 5679 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 73:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 507 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreInc, __pSymbol->m_Arg.pValue);
		;
#line 5692 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 74:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 511 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PreDec, __pSymbol->m_Arg.pValue);
		;
#line 5705 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 75:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 515 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstSizeT ((*(_cls20*) GetAstLocator (0)).m_pType->GetSize ());
		;
#line 5718 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 76:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 519 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetType ((*(_cls20*) GetAstLocator (0)).m_pType);
		;
#line 5731 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 77:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 523 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			// not yet
		;
#line 5744 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 78:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 527 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			// not yet
		;
#line 5757 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 79:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 532 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			EAlloc AllocKind = IsValidLocator ((*(_cls19*) GetAstLocator (0))) ? (*(_cls19*) GetAstLocator (0)).m_AllocKind : EAlloc_Undefined;
			return m_pModule->m_OperatorMgr.CastOperator (AllocKind, __pSymbol->m_Arg.pValue, (*(_cls3*) GetAstLocator (1)).m_pType);
		;
#line 5771 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 80:
			{
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 537 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			EAlloc AllocKind = IsValidLocator ((*(_cls19*) GetAstLocator (0))) ? (*(_cls19*) GetAstLocator (0)).m_AllocKind : EAlloc_Undefined;
			return m_pModule->m_OperatorMgr.NewOperator (AllocKind, (*(_cls3*) GetAstLocator (1)).m_pType, __pSymbol->m_Arg.pValue);
		;
#line 5785 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 81:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
#line 549 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_AllocKind = EAlloc_Stack;
		;
#line 5798 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 82:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAstNode = (_cls19*) __pSymbol->m_pAstNode;
#line 553 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_AllocKind = EAlloc_Heap;
		;
#line 5811 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 83:
			{
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
#line 579 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_pType = (*(_cls3*) GetAstLocator (0)).m_pType;
		;
#line 5824 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 84:
			{
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
#line 583 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_pType = (*(_cls3*) GetAstLocator (0)).m_pType;
		;
#line 5837 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 85:
			{
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
#line 587 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_pType = __pSymbol->m_Local.Value.GetType ();
		;
#line 5850 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 86:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 604 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.CallOperator (__pSymbol->m_Arg.pValue, &(*(_cls9*) GetAstLocator (0)).m_ValueList);
		;
#line 5863 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 87:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 608 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.ClosureOperator (__pSymbol->m_Arg.pValue, &(*(_cls9*) GetAstLocator (0)).m_ValueList);
		;
#line 5876 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 88:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 612 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, __pSymbol->m_Arg.pValue, (*(_cls6*) GetAstLocator (0)).m_Value);
		;
#line 5889 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 89:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 616 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostInc, __pSymbol->m_Arg.pValue);
		;
#line 5902 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 90:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 620 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_PostDec, __pSymbol->m_Arg.pValue);
		;
#line 5915 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 91:
			{
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 625 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Ptr, __pSymbol->m_Arg.pValue);
		;
#line 5928 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 92:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 633 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.MemberOperator (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
		;
#line 5941 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 93:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 637 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.WeakMemberOperator (__pSymbol->m_Arg.pValue, (*GetTokenLocator (0)).m_Data.m_String);
		;
#line 5954 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 94:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 641 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.GetPropertyOnChangeEvent (__pSymbol->m_Arg.pValue);
		;
#line 5967 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 95:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 645 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.GetPropertyGetter (__pSymbol->m_Arg.pValue);
		;
#line 5980 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 96:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 649 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.GetPropertySetter (__pSymbol->m_Arg.pValue);
		;
#line 5993 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 97:
			{
			CSymbolNode_member_operator* __pSymbol = (CSymbolNode_member_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 653 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return m_pModule->m_OperatorMgr.GetPropertyPropValue (__pSymbol->m_Arg.pValue);
		;
#line 6006 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 98:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 665 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return LookupIdentifier ((*GetTokenLocator (0)).m_Data.m_String, __pSymbol->m_Arg.pValue);
		;
#line 6019 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 99:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 669 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			return SetThis (__pSymbol->m_Arg.pValue);
		;
#line 6032 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 100:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 673 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstUInt64 ((*GetTokenLocator (0)).m_Data.m_UInt64);
		;
#line 6045 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 101:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 677 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstDouble ((*GetTokenLocator (0)).m_Data.m_Double);
		;
#line 6058 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 102:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 681 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetLiteral ((*(_cls21*) GetAstLocator (0)).m_String);
		;
#line 6071 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 103:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 685 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstBool (true);
		;
#line 6084 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 104:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 689 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetConstBool (false);
		;
#line 6097 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 105:
			{
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 693 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			__pSymbol->m_Arg.pValue->SetNull ();
		;
#line 6110 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 106:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
#line 705 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_String = (*GetTokenLocator (0)).m_Data.m_String;
		;
#line 6123 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 107:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls21* __pAstNode = (_cls21*) __pSymbol->m_pAstNode;
#line 709 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAstNode).m_String.Append ((*GetTokenLocator (1)).m_Data.m_String);
		;
#line 6136 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 108:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 14 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
#line 6149 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 109:
			{
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 19 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (1)));
		;
#line 6162 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 110:
			{
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 28 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Arg.pTokenList->InsertTail ((*GetTokenLocator (0)));
		;
#line 6175 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 111:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CCompoundStmt* __pAstNode = (CCompoundStmt*) __pSymbol->m_pAstNode;
#line 42 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			(*__pAstNode).m_pScope = m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
		;
#line 6188 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 112:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CCompoundStmt* __pAstNode = (CCompoundStmt*) __pSymbol->m_pAstNode;
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (1)).m_Pos);
		;
#line 6201 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 113:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 83 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.ConditionalJump ((*(_cls6*) GetAstLocator (0)).m_Value, __pSymbol->m_Local.pThenBlock, __pSymbol->m_Local.pElseBlock);
		;
#line 6214 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 114:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 89 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				__pSymbol->m_Local.pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("if_follow"));
				m_pModule->m_ControlFlowMgr.Jump (__pSymbol->m_Local.pFollowBlock, __pSymbol->m_Local.pElseBlock);
			;
#line 6228 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 115:
			{
			CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 95 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
			m_pModule->m_ControlFlowMgr.Follow (__pSymbol->m_Local.pFollowBlock);
		;
#line 6241 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 116:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CSwitchStmt* __pAstNode = (CSwitchStmt*) __pSymbol->m_pAstNode;
#line 110 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			bool Result = m_pModule->m_OperatorMgr.CastOperator ((*(_cls6*) GetAstLocator (0)).m_Value, EType_Int, &(*__pAstNode).m_Value);
			if (!Result)
				return false;

			(*__pAstNode).m_pSwitchBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
			(*__pAstNode).m_pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("switch_follow"));
			(*__pAstNode).m_pDefaultBlock = NULL;
			
			CBasicBlock* pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("switch_body"));
			m_pModule->m_ControlFlowMgr.SetCurrentBlock (pBodyBlock);
			m_pModule->m_ControlFlowMgr.MarkUnreachable (pBodyBlock);
		;
#line 6264 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 117:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CSwitchStmt* __pAstNode = (CSwitchStmt*) __pSymbol->m_pAstNode;
#line 124 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (1)).m_Pos);
			pScope->m_pBreakBlock = (*__pAstNode).m_pFollowBlock;
		;
#line 6278 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 118:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CSwitchStmt* __pAstNode = (CSwitchStmt*) __pSymbol->m_pAstNode;
#line 130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.CloseScope ((*GetTokenLocator (2)).m_Pos);
			m_pModule->m_ControlFlowMgr.Follow ((*__pAstNode).m_pFollowBlock);

			return FinalizeSwitchStmt (&(*__pAstNode));
		;
#line 6294 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 119:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 140 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return SwitchCaseLabel (__pSymbol->m_Arg.pSwitchStmt, (*(_cls11*) GetAstLocator (0)).m_Value, (*GetTokenLocator (1)).m_Pos, (*GetTokenLocator (2)).m_Pos);
		;
#line 6307 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 120:
			{
			CSymbolNode_switch_block_stmt* __pSymbol = (CSymbolNode_switch_block_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 144 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return SwitchDefaultLabel (__pSymbol->m_Arg.pSwitchStmt, (*GetTokenLocator (0)).m_Pos, (*GetTokenLocator (1)).m_Pos);
		;
#line 6320 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 121:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 165 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
			pScope->m_pBreakBlock = __pSymbol->m_Local.pFollowBlock;
			pScope->m_pContinueBlock = __pSymbol->m_Local.pConditionBlock;
			return m_pModule->m_ControlFlowMgr.ConditionalJump ((*(_cls6*) GetAstLocator (1)).m_Value, __pSymbol->m_Local.pBodyBlock, __pSymbol->m_Local.pFollowBlock);
		;
#line 6336 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 122:
			{
			CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 172 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.CloseScope ((*GetAstLocator (2)).m_LastToken.m_Pos);
			m_pModule->m_ControlFlowMgr.Jump (__pSymbol->m_Local.pConditionBlock, __pSymbol->m_Local.pFollowBlock);
		;
#line 6350 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 123:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 193 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
			pScope->m_pBreakBlock = __pSymbol->m_Local.pFollowBlock;
			pScope->m_pContinueBlock = __pSymbol->m_Local.pConditionBlock;
		;
#line 6365 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 124:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 199 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_ControlFlowMgr.Follow (__pSymbol->m_Local.pConditionBlock);
		;
#line 6378 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 125:
			{
			CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 203 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.ConditionalJump ((*(_cls6*) GetAstLocator (1)).m_Value, __pSymbol->m_Local.pBodyBlock, __pSymbol->m_Local.pFollowBlock);
			m_pModule->m_ControlFlowMgr.SetCurrentBlock (__pSymbol->m_Local.pFollowBlock);
		;
#line 6392 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 126:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 227 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Local.pScope = m_pModule->m_NamespaceMgr.OpenScope ((*GetTokenLocator (0)).m_Pos);
		;
#line 6405 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 127:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 232 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				__pSymbol->m_Local.pConditionBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("for_condition"));
				__pSymbol->m_Local.pLoopBlock = __pSymbol->m_Local.pConditionBlock;
				m_pModule->m_ControlFlowMgr.Follow (__pSymbol->m_Local.pConditionBlock);
			;
#line 6420 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 128:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 238 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				return m_pModule->m_ControlFlowMgr.ConditionalJump ((*(_cls6*) GetAstLocator (1)).m_Value, __pSymbol->m_Local.pBodyBlock, __pSymbol->m_Local.pFollowBlock);
			;
#line 6433 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 129:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 243 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				__pSymbol->m_Local.pLoopBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("for_loop"));
				m_pModule->m_ControlFlowMgr.SetCurrentBlock (__pSymbol->m_Local.pLoopBlock);
			;
#line 6447 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 130:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 248 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
				m_pModule->m_ControlFlowMgr.Jump (__pSymbol->m_Local.pConditionBlock, __pSymbol->m_Local.pBodyBlock);
			;
#line 6460 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 131:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 252 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			__pSymbol->m_Local.pScope->m_pBreakBlock = __pSymbol->m_Local.pFollowBlock;
			__pSymbol->m_Local.pScope->m_pContinueBlock = __pSymbol->m_Local.pConditionBlock;
		;
#line 6474 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 132:
			{
			CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 257 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			m_pModule->m_NamespaceMgr.CloseScope ((*GetAstLocator (2)).m_LastToken.m_Pos);
			m_pModule->m_ControlFlowMgr.Jump (__pSymbol->m_Local.pLoopBlock, __pSymbol->m_Local.pFollowBlock);

			if (!(__pSymbol->m_Local.pFollowBlock->GetFlags () & EBasicBlockFlag_Jumped))
				m_pModule->m_ControlFlowMgr.MarkUnreachable (__pSymbol->m_Local.pFollowBlock);
		;
#line 6491 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 133:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 268 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.Break (IsValidLocator ((*(_cls11*) GetAstLocator (0))) ? (*(_cls11*) GetAstLocator (0)).m_Value : 1);
		;
#line 6504 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 134:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 275 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.Continue (IsValidLocator ((*(_cls11*) GetAstLocator (0))) ? (*(_cls11*) GetAstLocator (0)).m_Value : 1);
		;
#line 6517 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 135:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			
			return m_pModule->m_ControlFlowMgr.Return (IsValidLocator ((*(_cls6*) GetAstLocator (0))) ? (*(_cls6*) GetAstLocator (0)).m_Value : CValue ());
		;
#line 6530 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 136:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 22 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return SetStorageKind ((*(_cls27*) GetAstLocator (0)).m_StorageKind);
		;
#line 6543 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 137:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return SetAccessKind ((*(_cls28*) GetAstLocator (0)).m_AccessKind);
		;
#line 6556 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 138:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 30 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls29*) GetAstLocator (0)).m_pType);
		;
#line 6569 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 139:
			{
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 34 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls31*) GetAstLocator (0)).m_Modifier);
		;
#line 6582 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 140:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 59 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls29*) GetAstLocator (0)).m_pType);
		;
#line 6595 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 141:
			{
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 63 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return __pSymbol->m_Arg.pTypeSpecifier->SetTypeModifier ((*(_cls31*) GetAstLocator (0)).m_Modifier);
		;
#line 6608 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 142:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
#line 76 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Typedef;
		;
#line 6621 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 143:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
#line 80 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Static;
		;
#line 6634 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 144:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
#line 84 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Abstract;
		;
#line 6647 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 145:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
#line 88 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Virtual;
		;
#line 6660 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 146:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
#line 92 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_NoVirtual;
		;
#line 6673 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 147:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls27* __pAstNode = (_cls27*) __pSymbol->m_pAstNode;
#line 96 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_StorageKind = EStorage_Mutable;
		;
#line 6686 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 148:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
#line 109 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_AccessKind = EAccess_Public;
		;
#line 6699 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 149:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls28* __pAstNode = (_cls28*) __pSymbol->m_pAstNode;
#line 113 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_AccessKind = EAccess_Protected;
		;
#line 6712 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 150:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 126 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
		;
#line 6725 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 151:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Variant);
		;
#line 6738 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 152:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 134 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectClass);
		;
#line 6751 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 153:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 138 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Bool);
		;
#line 6764 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 154:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 142 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (m_Endianness == EEndianness_BigEndian ? EType_Int32_be : EType_Int);
		;
#line 6777 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 155:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 146 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (m_Endianness == EEndianness_BigEndian ? EType_Int_pbe : EType_Int_p);
		;
#line 6790 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 156:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 150 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Char);
		;
#line 6803 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 157:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 154 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (m_Endianness == EEndianness_BigEndian ? EType_Int16_be : EType_Short);
		;
#line 6816 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 158:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 158 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (m_Endianness == EEndianness_BigEndian ? EType_Int32_be : EType_Long);
		;
#line 6829 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 159:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 162 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Float);
		;
#line 6842 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 160:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 166 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Double);
		;
#line 6855 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 161:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 170 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int8);
		;
#line 6868 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 162:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 174 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (m_Endianness == EEndianness_BigEndian ? EType_Int16_be : EType_Int16);
		;
#line 6881 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 163:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 178 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (m_Endianness == EEndianness_BigEndian ? EType_Int32_be : EType_Int32);
		;
#line 6894 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 164:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (m_Endianness == EEndianness_BigEndian ? EType_Int64_be : EType_Int64);
		;
#line 6907 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 165:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 186 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_WChar);
		;
#line 6920 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 166:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 190 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			err::SetStringError (_T("'auto' type specifier is not yet supported"));
			return false; 
		;
#line 6934 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 167:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 195 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls35*) GetAstLocator (0)).m_pType;
		;
#line 6947 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 168:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 199 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls36*) GetAstLocator (0)).m_pType;
		;
#line 6960 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 169:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 203 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls39*) GetAstLocator (0)).m_pType;
		;
#line 6973 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 170:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 207 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls40*) GetAstLocator (0)).m_pType;
		;
#line 6986 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 171:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 211 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls42*) GetAstLocator (0)).m_pType;
		;
#line 6999 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 172:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 216 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls43*) GetAstLocator (0)).m_pProperty->GetType ();
		;
#line 7012 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 173:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 220 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls44*) GetAstLocator (0)).m_pType;
		;
#line 7025 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 174:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls29* __pAstNode = (_cls29*) __pSymbol->m_pAstNode;
#line 226 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_pType = (*(_cls30*) GetAstLocator (0)).m_pType;
		;
#line 7038 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 175:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			if (IsTypeSpecified ())
				return false;
		;
#line 7052 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 176:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 239 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return m_Stage == EStage_Pass1 || FindType ((*(_cls1*) GetAstLocator (0)).m_Name) != NULL;
		;
#line 7065 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 177:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 243 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			return false;
		;
#line 7078 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 178:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls30* __pAstNode = (_cls30*) __pSymbol->m_pAstNode;
#line 255 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			CType* pType = FindType ((*(_cls1*) GetAstLocator (0)).m_Name);
			if (!pType)
			{
				err::SetFormatStringError (_T("'%s' is not a type"), (*(_cls1*) GetAstLocator (0)).m_Name.GetFullName ());
				return false;
			}

			(*__pAstNode).m_pType = pType;
		;
#line 7098 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 179:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 275 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Signed;
		;
#line 7111 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 180:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 279 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Unsigned;
		;
#line 7124 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 181:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 283 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_LittleEndian;
		;
#line 7137 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 182:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 287 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_BigEndian;
		;
#line 7150 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 183:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 291 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Const;
		;
#line 7163 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 184:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 295 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_ReadOnly;
		;
#line 7176 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 185:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 299 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Volatile;
		;
#line 7189 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 186:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 303 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Safe;
		;
#line 7202 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 187:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 307 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Unsafe;
		;
#line 7215 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 188:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 311 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_NoNull;
		;
#line 7228 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 189:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 315 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Strong;
		;
#line 7241 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 190:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 319 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Weak;
		;
#line 7254 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 191:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 323 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Cdecl;
		;
#line 7267 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 192:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 327 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Stdcall;
		;
#line 7280 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 193:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 331 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Function;
		;
#line 7293 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 194:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 335 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Property;
		;
#line 7306 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 195:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 339 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Multicast;
		;
#line 7319 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 196:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 343 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Event;
		;
#line 7332 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 197:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 347 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Bindable;
		;
#line 7345 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 198:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 351 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_AutoGet;
		;
#line 7358 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 199:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 355 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Indexed;
		;
#line 7371 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 200:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 359 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Closure;
		;
#line 7384 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 201:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls31* __pAstNode = (_cls31*) __pSymbol->m_pAstNode;
#line 363 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			
			(*__pAstNode).m_Modifier = ETypeModifier_Thin;
		;
#line 7397 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 202:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 10 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			bool Result = Declare (&(*(_cls32*) GetAstLocator (0)).m_Declarator, true);
			if (!Result)
				return false;

			if (!m_pLastDeclaredItem || 
				m_pLastDeclaredItem->GetItemKind () != EModuleItem_Variable)
			{
				err::SetFormatStringError (_T("can only apply initializers to variables"));
				return false;
			}
		;
#line 7419 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 203:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 23 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Assign, (CVariable*) m_pLastDeclaredItem, (*(_cls33*) GetAstLocator (1)).m_Value);
		;
#line 7432 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 204:
			{
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 27 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			 
			if (!IsValidLocator ((*(_cls33*) GetAstLocator (1))))
				return Declare (&(*(_cls32*) GetAstLocator (0)).m_Declarator);
		;
#line 7446 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 205:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
#line 58 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			 return m_Stage == EStage_Pass1 ;
#line 7457 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 206:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
#line 60 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_TokenList.TakeOver (&(*(_cls5*) GetAstLocator (0)).m_TokenList);
		;
#line 7470 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 207:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
#line 64 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_Value = (*(_cls6*) GetAstLocator (0)).m_Value;
		;
#line 7483 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 208:
			{
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
#line 68 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
		;
#line 7495 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 209:
			{
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 87 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			__pSymbol->m_Arg.pDeclarator->m_Pos = (*GetAstLocator (0)).m_FirstToken.m_Pos;
		;
#line 7508 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 210:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 95 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddName ((*GetTokenLocator (0)).m_Data.m_String);
		;
#line 7521 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 211:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 99 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Getter);
		;
#line 7534 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 212:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 103 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Setter);
		;
#line 7547 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 213:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 107 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_PreConstructor);
		;
#line 7560 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 214:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 111 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Constructor);
		;
#line 7573 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 215:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 116 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_StaticConstructor);
		;
#line 7586 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 216:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 120 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_Destructor);
		;
#line 7599 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 217:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 124 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnnamedMethod (EFunction_CallOperator);
		;
#line 7612 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 218:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 128 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddCastOperator ((*(_cls3*) GetAstLocator (0)).m_pType);
		;
#line 7625 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 219:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 132 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddUnaryBinaryOperator ((*(_cls34*) GetAstLocator (0)).m_UnOpKind, (*(_cls34*) GetAstLocator (0)).m_BinOpKind);
		;
#line 7638 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 220:
			{
			CSymbolNode_declarator_qualifier* __pSymbol = (CSymbolNode_declarator_qualifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 136 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->SetPropValue ();
		;
#line 7651 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 221:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 162 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_BwNot;
		;
#line 7664 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 222:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 166 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_LogNot;
		;
#line 7677 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 223:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 170 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_PreInc;
		;
#line 7690 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 224:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 174 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_PreDec;
		;
#line 7703 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 225:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 178 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_PostInc;
		;
#line 7716 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 226:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_PostDec;
		;
#line 7729 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 227:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 186 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_Ptr;
		;
#line 7742 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 228:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 190 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_Plus;
			(*__pAstNode).m_BinOpKind = EBinOp_Add;
		;
#line 7756 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 229:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 195 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_Minus;
			(*__pAstNode).m_BinOpKind = EBinOp_Sub;
		;
#line 7770 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 230:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 200 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_Indir;
			(*__pAstNode).m_BinOpKind = EBinOp_Mul;
		;
#line 7784 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 231:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 205 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Div;
		;
#line 7797 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 232:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 209 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Mod;
		;
#line 7810 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 233:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 213 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Shl;
		;
#line 7823 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 234:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 217 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Shr;
		;
#line 7836 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 235:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 221 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_UnOpKind = EUnOp_Addr;
			(*__pAstNode).m_BinOpKind = EBinOp_BwAnd;
		;
#line 7850 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 236:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 226 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_BwXor;
		;
#line 7863 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 237:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 230 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_BwOr;
		;
#line 7876 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 238:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 234 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Eq;
		;
#line 7889 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 239:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 238 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Ne;
		;
#line 7902 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 240:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 242 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Lt;
		;
#line 7915 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 241:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 246 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Le;
		;
#line 7928 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 242:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 250 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Gt;
		;
#line 7941 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 243:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 254 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Ge;
		;
#line 7954 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 244:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 258 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Assign;
		;
#line 7967 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 245:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 262 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_AddAssign;
		;
#line 7980 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 246:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 266 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_SubAssign;
		;
#line 7993 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 247:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 270 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_MulAssign;
		;
#line 8006 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 248:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_DivAssign;
		;
#line 8019 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 249:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 278 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_ModAssign;
		;
#line 8032 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 250:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 282 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_ShlAssign;
		;
#line 8045 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 251:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 286 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_ShrAssign;
		;
#line 8058 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 252:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 290 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_AndAssign;
		;
#line 8071 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 253:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 294 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_XorAssign;
		;
#line 8084 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 254:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 298 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_OrAssign;
		;
#line 8097 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 255:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_AtAssign;
		;
#line 8110 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 256:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 306 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_ClassAssign;
		;
#line 8123 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 257:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 310 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_LogAnd;
		;
#line 8136 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 258:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 314 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_LogOr;
		;
#line 8149 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 259:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 318 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_Idx;
		;
#line 8162 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 260:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 322 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			(*__pAstNode).m_BinOpKind = EBinOp_At;
		;
#line 8175 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 261:
			{
			CSymbolNode_pointer* __pSymbol = (CSymbolNode_pointer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 334 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddPointer ();
		;
#line 8188 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 262:
			{
			CSymbolNode_pointer* __pSymbol = (CSymbolNode_pointer*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 338 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->SetTypeModifier ((*(_cls31*) GetAstLocator (0)).m_Modifier);
		;
#line 8201 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 263:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 359 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
		;
#line 8214 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 264:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 363 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix (0);
		;
#line 8227 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 265:
			{
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 391 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return CreateFormalArg (
				__pSymbol->m_Arg.pFunctionSuffix, 
				&(*(_cls26*) GetAstLocator (0)).m_TypeSpecifier, 
				IsValidLocator ((*(_cls32*) GetAstLocator (1))) ? &(*(_cls32*) GetAstLocator (1)).m_Declarator : NULL, 
				CValue ()
				) != NULL;
		;
#line 8245 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 266:
			{
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 400 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_VarArg;
			if (IsValidLocator ((*GetTokenLocator (0))))
				__pSymbol->m_Arg.pFunctionSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_UnsafeVarArg;
		;
#line 8260 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 267:
			{
			CSymbolNode_bitfield_suffix* __pSymbol = (CSymbolNode_bitfield_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 411 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->AddBitFieldSuffix ((*GetTokenLocator (0)).m_Data.m_Integer);
		;
#line 8273 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 268:
			{
			CSymbolNode_post_declarator_modifier* __pSymbol = (CSymbolNode_post_declarator_modifier*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 420 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			
			return __pSymbol->m_Arg.pDeclarator->SetPostDeclaratorModifier (EPostDeclaratorModifier_Const);
		;
#line 8286 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 269:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
#line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = CreateEnumType (
				IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
				IsValidLocator ((*GetTokenLocator (1))) ? EEnumTypeFlag_Exposed : 0
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 8305 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 270:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 33 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pType);	
		;
#line 8318 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 271:
			{
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 38 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();	
		;
#line 8331 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 272:
			{
			CSymbolNode_enum_member* __pSymbol = (CSymbolNode_enum_member*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 51 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			CEnumMember* pMember = __pSymbol->m_Arg.pType->CreateMember ((*GetTokenLocator (0)).m_Data.m_String);
			if (!pMember)
				return false;

			pMember->m_Pos = (*GetTokenLocator (0)).m_Pos;
			
			if (IsValidLocator ((*(_cls5*) GetAstLocator (1)))) 
				pMember->SetExpression (&(*(_cls5*) GetAstLocator (1)).m_TokenList);
		;
#line 8351 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 273:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
#line 77 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = CreateStructType (
				IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
				IsValidLocator ((*(_cls4*) GetAstLocator (1))) ? &(*(_cls4*) GetAstLocator (1)).m_TypeList : NULL,
				IsValidLocator ((*(_cls37*) GetAstLocator (2))) ? (*(_cls37*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 8371 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 274:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls37* __pAstNode = (_cls37*) __pSymbol->m_pAstNode;
#line 98 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_PackFactor = (*(_cls38*) GetAstLocator (0)).m_Value;
		;
#line 8384 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 275:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
#line 109 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_Value = (*GetTokenLocator (0)).m_Data.m_Integer;
		;
#line 8397 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 276:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls38* __pAstNode = (_cls38*) __pSymbol->m_pAstNode;
#line 113 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_Value = m_DefaultStructPackFactor;
		;
#line 8410 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 277:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
#line 129 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = CreateUnionType (IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString ());
			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 8425 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 278:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
#line 150 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = CreateClassType (
				IsValidLocator ((*GetTokenLocator (0))) ? (*GetTokenLocator (0)).m_Data.m_String : rtl::CString (),
				IsValidLocator ((*(_cls4*) GetAstLocator (1))) ? &(*(_cls4*) GetAstLocator (1)).m_TypeList : NULL,
				IsValidLocator ((*(_cls37*) GetAstLocator (2))) ? (*(_cls37*) GetAstLocator (2)).m_PackFactor : m_StructPackFactor,
				IsValidLocator ((*GetTokenLocator (3))) ? EClassTypeFlag_Interface : 0
				);

			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 8446 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 279:
			{
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
#line 198 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			if (!__pSymbol->m_Local.pTemplate->GetGetterType ()) 
			{
				err::SetFormatStringError (_T("incomplete property template (no getter)"));
				return false;
			}

			(*__pAstNode).m_pType = m_pModule->m_TypeMgr.GetPropertyType (
				__pSymbol->m_Local.pTemplate->GetGetterType (), 
				*__pSymbol->m_Local.pTemplate->GetSetterType ()
				);
		;
#line 8468 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 280:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls43* __pAstNode = (_cls43*) __pSymbol->m_pAstNode;
#line 220 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pProperty = CreateProperty (
				(*GetTokenLocator (0)).m_Data.m_String,
				IsValidLocator ((*(_cls37*) GetAstLocator (1))) ? (*(_cls37*) GetAstLocator (1)).m_PackFactor : m_StructPackFactor
				);

			if (!(*__pAstNode).m_pProperty)
				return false;
		;
#line 8487 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 281:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
#line 251 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_Declarator.AddName ((*GetTokenLocator (0)).m_Data.m_String);
		;
#line 8500 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 282:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
#line 256 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			(*__pAstNode).m_pType = CreateAutoEvType (&(*__pAstNode).m_Declarator);
			if (!(*__pAstNode).m_pType)
				return false;
		;
#line 8515 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 283:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
#line 262 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			 return m_Stage == EStage_Pass1 ;
#line 8526 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 284:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
#line 264 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
				(*__pAstNode).m_pType->SetAutoEvBody (&(*(_cls22*) GetAstLocator (1)).m_TokenList);
			;
#line 8539 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 285:
			{
			CSymbolNode_autoev_body* __pSymbol = (CSymbolNode_autoev_body*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 274 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			return PreAutoEvBlock (__pSymbol->m_Arg.pType);
		;
#line 8552 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 286:
			{
			CSymbolNode_autoev_body* __pSymbol = (CSymbolNode_autoev_body*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 278 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			return PostAutoEvBlock ();
		;
#line 8565 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 287:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 285 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			return PreAutoEvExpression ();
		;
#line 8578 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 288:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 289 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			return PostAutoEvExpression ((*(_cls6*) GetAstLocator (0)).m_Value);
		;
#line 8591 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 289:
			{
			CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 301 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.OpenNamespace (__pSymbol->m_Arg.pNamespace);
			SetDefaultMethodStorageKind ();
		;
#line 8605 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 290:
			{
			CSymbolNode_named_type_block* __pSymbol = (CSymbolNode_named_type_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 307 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			
			m_pModule->m_NamespaceMgr.CloseNamespace ();
			SetDefaultMethodStorageKind ();		
		;
#line 8619 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
			
#line 98 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pName = &(*__pAstNode).m_Name;
#line 8647 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_declarator_list* __pTarget = (CSymbolNode_declarator_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 131 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls25*) GetAstLocator (0)).m_TypeSpecifier;
#line 8660 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 132 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls25*) GetAstLocator (0)).m_TypeSpecifier;
#line 8673 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
#line 149 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 8686 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
#line 150 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 8699 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
#line 151 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 8712 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_declaration_terminator* __pTarget = (CSymbolNode_declaration_terminator*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
			
#line 155 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pTypeSpecifier = NULL;
#line 8725 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_item_declaration_no_specifier_rslv* __pSymbol = (CSymbolNode_item_declaration_no_specifier_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 166 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
#line 8738 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
			
#line 209 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
#line 8751 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls3* __pAstNode = (_cls3*) __pSymbol->m_pAstNode;
			
#line 210 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
#line 8764 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAstNode = (_cls5*) __pSymbol->m_pAstNode;
			
#line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
#line 8777 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 13 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 8790 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 14 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 8803 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_primary_expr_pass1* __pTarget = (CSymbolNode_primary_expr_pass1*) pSymbol;
			CSymbolNode_primary_expr_pass1* __pSymbol = (CSymbolNode_primary_expr_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 15 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 8816 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAstNode = (_cls6*) __pSymbol->m_pAstNode;
			
#line 30 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
#line 8829 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAstNode = (_cls7*) __pSymbol->m_pAstNode;
			
#line 39 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
#line 8842 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAstNode = (_cls10*) __pSymbol->m_pAstNode;
			
#line 97 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_Value;
#line 8855 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_logical_or_expr* __pTarget = (CSymbolNode_logical_or_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
			
#line 140 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 8868 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
			
#line 149 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_TrueValue;
#line 8881 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_conditional_expr* __pSymbol = (CSymbolNode_conditional_expr*) GetSymbolTop ();
			CConditionalExpr* __pAstNode = (CConditionalExpr*) __pSymbol->m_pAstNode;
			
#line 154 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &(*__pAstNode).m_FalseValue;
#line 8894 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 166 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 8907 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_logical_and_expr* __pTarget = (CSymbolNode_logical_and_expr*) pSymbol;
			CSymbolNode_logical_or_expr* __pSymbol = (CSymbolNode_logical_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 168 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 8920 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 180 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 8933 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_inclusive_or_expr* __pTarget = (CSymbolNode_inclusive_or_expr*) pSymbol;
			CSymbolNode_logical_and_expr* __pSymbol = (CSymbolNode_logical_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 182 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 8946 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 194 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 8959 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_exclusive_or_expr* __pTarget = (CSymbolNode_exclusive_or_expr*) pSymbol;
			CSymbolNode_inclusive_or_expr* __pSymbol = (CSymbolNode_inclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 196 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 8972 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 208 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 8985 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_and_expr* __pTarget = (CSymbolNode_and_expr*) pSymbol;
			CSymbolNode_exclusive_or_expr* __pSymbol = (CSymbolNode_exclusive_or_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 210 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 8998 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 222 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9011 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_equality_expr* __pTarget = (CSymbolNode_equality_expr*) pSymbol;
			CSymbolNode_and_expr* __pSymbol = (CSymbolNode_and_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 224 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 9024 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 236 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9037 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_relational_expr* __pTarget = (CSymbolNode_relational_expr*) pSymbol;
			CSymbolNode_equality_expr* __pSymbol = (CSymbolNode_equality_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 238 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 9050 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 265 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9063 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_shift_expr* __pTarget = (CSymbolNode_shift_expr*) pSymbol;
			CSymbolNode_relational_expr* __pSymbol = (CSymbolNode_relational_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 267 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 9076 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 302 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9089 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_additive_expr* __pTarget = (CSymbolNode_additive_expr*) pSymbol;
			CSymbolNode_shift_expr* __pSymbol = (CSymbolNode_shift_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 304 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 9102 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 331 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9115 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_multiplicative_expr* __pTarget = (CSymbolNode_multiplicative_expr*) pSymbol;
			CSymbolNode_additive_expr* __pSymbol = (CSymbolNode_additive_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 333 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 9128 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 360 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9141 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_at_expr* __pTarget = (CSymbolNode_at_expr*) pSymbol;
			CSymbolNode_multiplicative_expr* __pSymbol = (CSymbolNode_multiplicative_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 362 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 9154 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 393 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9167 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 41:
			{
			CSymbolNode_assignment_expr* __pTarget = (CSymbolNode_assignment_expr*) pSymbol;
			CSymbolNode_at_expr* __pSymbol = (CSymbolNode_at_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 395 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 9180 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 42:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 407 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9193 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 43:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_assignment_expr* __pSymbol = (CSymbolNode_assignment_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 409 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.OpValue2;
#line 9206 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 44:
			{
			CSymbolNode_postfix_expr* __pTarget = (CSymbolNode_postfix_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 472 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9219 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 45:
			{
			CSymbolNode_unary_operator_expr* __pTarget = (CSymbolNode_unary_operator_expr*) pSymbol;
			CSymbolNode_unary_expr* __pSymbol = (CSymbolNode_unary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 473 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9232 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 46:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 482 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9245 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 47:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 486 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9258 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 48:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 490 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9271 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 49:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 494 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9284 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 50:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 498 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9297 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 51:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 502 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9310 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 52:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 506 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9323 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 53:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 510 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9336 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 54:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 522 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9349 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 55:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 526 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9362 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 56:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 531 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9375 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 57:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_cast_operator_rslv* __pSymbol = (CSymbolNode_cast_operator_rslv*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 566 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pTypeSpecifier = &__pSymbol->m_Local.TypeSpecifier;
#line 9388 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 58:
			{
			CSymbolNode_unary_expr* __pTarget = (CSymbolNode_unary_expr*) pSymbol;
			CSymbolNode_type_name_or_expr* __pSymbol = (CSymbolNode_type_name_or_expr*) GetSymbolTop ();
			_cls20* __pAstNode = (_cls20*) __pSymbol->m_pAstNode;
			
#line 586 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = &__pSymbol->m_Local.Value;
#line 9401 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 59:
			{
			CSymbolNode_primary_expr* __pTarget = (CSymbolNode_primary_expr*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 593 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9414 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 60:
			{
			CSymbolNode_postfix_operator* __pTarget = (CSymbolNode_postfix_operator*) pSymbol;
			CSymbolNode_postfix_expr* __pSymbol = (CSymbolNode_postfix_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 594 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9427 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 61:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 623 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9440 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 62:
			{
			CSymbolNode_member_operator* __pTarget = (CSymbolNode_member_operator*) pSymbol;
			CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 628 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9453 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 63:
			{
			CSymbolNode_conditional_expr* __pTarget = (CSymbolNode_conditional_expr*) pSymbol;
			CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 696 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
			__pTarget->m_Arg.pValue = __pSymbol->m_Arg.pValue;
#line 9466 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 64:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls22* __pAstNode = (_cls22*) __pSymbol->m_pAstNode;
			
#line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = &(*__pAstNode).m_TokenList;
#line 9479 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 65:
			{
			CSymbolNode_statement_pass1* __pTarget = (CSymbolNode_statement_pass1*) pSymbol;
			CSymbolNode_compound_stmt_pass1* __pSymbol = (CSymbolNode_compound_stmt_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 17 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 9492 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 66:
			{
			CSymbolNode_compound_stmt_pass1* __pTarget = (CSymbolNode_compound_stmt_pass1*) pSymbol;
			CSymbolNode_statement_pass1* __pSymbol = (CSymbolNode_statement_pass1*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pTokenList = __pSymbol->m_Arg.pTokenList;
#line 9505 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 67:
			{
			CSymbolNode_switch_block_stmt* __pTarget = (CSymbolNode_switch_block_stmt*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CSwitchStmt* __pAstNode = (CSwitchStmt*) __pSymbol->m_pAstNode;
			
#line 128 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
			__pTarget->m_Arg.pSwitchStmt = &(*__pAstNode);
#line 9518 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 68:
			{
			CSymbolNode_declaration_specifier* __pTarget = (CSymbolNode_declaration_specifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
			
#line 16 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
#line 9531 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 69:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
			
#line 54 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAstNode).m_TypeSpecifier;
#line 9544 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 70:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
#line 9557 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 71:
			{
			CSymbolNode_full_declarator* __pTarget = (CSymbolNode_full_declarator*) pSymbol;
			CSymbolNode_declarator_list* __pSymbol = (CSymbolNode_declarator_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 4 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
#line 9570 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 72:
			{
			CSymbolNode_declarator* __pTarget = (CSymbolNode_declarator*) pSymbol;
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 8 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
#line 9583 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 73:
			{
			CSymbolNode_initializer* __pTarget = (CSymbolNode_initializer*) pSymbol;
			CSymbolNode_full_declarator* __pSymbol = (CSymbolNode_full_declarator*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 22 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pType = ((CVariable*) m_pLastDeclaredItem)->GetType ();
#line 9596 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 74:
			{
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
			
#line 44 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 9609 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 75:
			{
			CSymbolNode_declarator_name* __pTarget = (CSymbolNode_declarator_name*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
			
#line 45 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 9622 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 76:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
			
#line 46 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 9635 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 77:
			{
			CSymbolNode_post_declarator_modifier* __pTarget = (CSymbolNode_post_declarator_modifier*) pSymbol;
			CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
			_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
			
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 9648 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 78:
			{
			CSymbolNode_initializer_list* __pTarget = (CSymbolNode_initializer_list*) pSymbol;
			CSymbolNode_initializer* __pSymbol = (CSymbolNode_initializer*) GetSymbolTop ();
			_cls33* __pAstNode = (_cls33*) __pSymbol->m_pAstNode;
			
#line 67 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 9661 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 79:
			{
			CSymbolNode_initializer_list_entry* __pTarget = (CSymbolNode_initializer_list_entry*) pSymbol;
			CSymbolNode_initializer_list* __pSymbol = (CSymbolNode_initializer_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 74 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 9674 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 80:
			{
			CSymbolNode_initializer_list_entry* __pTarget = (CSymbolNode_initializer_list_entry*) pSymbol;
			CSymbolNode_initializer_list* __pSymbol = (CSymbolNode_initializer_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 74 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 9687 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 81:
			{
			CSymbolNode_initializer* __pTarget = (CSymbolNode_initializer*) pSymbol;
			CSymbolNode_initializer_list_entry* __pSymbol = (CSymbolNode_initializer_list_entry*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 79 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 9700 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 82:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 86 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 9713 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 83:
			{
			CSymbolNode_declarator_qualifier* __pTarget = (CSymbolNode_declarator_qualifier*) pSymbol;
			CSymbolNode_declarator_name* __pSymbol = (CSymbolNode_declarator_name*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 90 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 9726 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 84:
			{
			CSymbolNode_array_suffix* __pTarget = (CSymbolNode_array_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 350 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 9739 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 85:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 351 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 9752 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 86:
			{
			CSymbolNode_bitfield_suffix* __pTarget = (CSymbolNode_bitfield_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 352 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 9765 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 87:
			{
			CSymbolNode_function_formal_argument_list* __pTarget = (CSymbolNode_function_formal_argument_list*) pSymbol;
			CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 379 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Local.pFunctionSuffix;
#line 9778 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 88:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 384 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
#line 9791 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 89:
			{
			CSymbolNode_function_formal_argument* __pTarget = (CSymbolNode_function_formal_argument*) pSymbol;
			CSymbolNode_function_formal_argument_list* __pSymbol = (CSymbolNode_function_formal_argument_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 385 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pFunctionSuffix = __pSymbol->m_Arg.pFunctionSuffix;
#line 9804 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 90:
			{
			CSymbolNode_declarator* __pTarget = (CSymbolNode_declarator*) pSymbol;
			CSymbolNode_function_formal_argument* __pSymbol = (CSymbolNode_function_formal_argument*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 390 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls26*) GetAstLocator (0)).m_TypeSpecifier;
#line 9817 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 91:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
			
#line 26 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 9830 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 92:
			{
			CSymbolNode_enum_member_list* __pTarget = (CSymbolNode_enum_member_list*) pSymbol;
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 36 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 9843 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 93:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 45 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 9856 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 94:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAstNode* __pAstNode = __pSymbol->m_pAstNode;
			
#line 45 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 9869 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 95:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls36* __pAstNode = (_cls36*) __pSymbol->m_pAstNode;
			
#line 87 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
#line 9882 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 96:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls39* __pAstNode = (_cls39*) __pSymbol->m_pAstNode;
			
#line 134 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
#line 9895 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 97:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls40* __pAstNode = (_cls40*) __pSymbol->m_pAstNode;
			
#line 161 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pType;
#line 9908 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 98:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
			_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
			
#line 197 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = __pSymbol->m_Local.pTemplate;
#line 9921 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 99:
			{
			CSymbolNode_named_type_block* __pTarget = (CSymbolNode_named_type_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls43* __pAstNode = (_cls43*) __pSymbol->m_pAstNode;
			
#line 229 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pNamespace = (*__pAstNode).m_pProperty;
#line 9934 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 100:
			{
			CSymbolNode_function_suffix* __pTarget = (CSymbolNode_function_suffix*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
			
#line 255 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAstNode).m_Declarator;
#line 9947 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 101:
			{
			CSymbolNode_autoev_body* __pTarget = (CSymbolNode_autoev_body*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls44* __pAstNode = (_cls44*) __pSymbol->m_pAstNode;
			
#line 267 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAstNode).m_pType;
#line 9960 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			
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
#line 126 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
				
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
	;
#line 9998 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_item_declaration_no_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls2* __pAstNode = (_cls2*) __pSymbol->m_pAstNode;
#line 143 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
				
		m_StorageKind = EStorage_Undefined;
		m_AccessKind = EAccess_Undefined;
		(*__pAstNode).m_Declarator.SetTypeSpecifier (NULL);
	;
#line 10018 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_declaration_specifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
#line 9 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
#line 10031 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_function_suffix:
			{
				CSymbolNode_function_suffix* __pSymbol = (CSymbolNode_function_suffix*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 376 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
				
		__pSymbol->m_Local.pFunctionSuffix = __pSymbol->m_Arg.pDeclarator->AddFunctionSuffix ();
	;
#line 10048 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
#line 47 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PushTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
#line 10064 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_if_stmt:
			{
				CSymbolNode_if_stmt* __pSymbol = (CSymbolNode_if_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 77 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
	__pSymbol->m_Local.pThenBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("if_then"));
	__pSymbol->m_Local.pElseBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("if_else"));
	__pSymbol->m_Local.pFollowBlock = __pSymbol->m_Local.pElseBlock;
;
#line 10130 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		case ESymbol_while_stmt:
			{
				CSymbolNode_while_stmt* __pSymbol = (CSymbolNode_while_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 158 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
	__pSymbol->m_Local.pConditionBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("while_condition"));
	__pSymbol->m_Local.pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("while_body"));
	__pSymbol->m_Local.pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("while_follow"));
	m_pModule->m_ControlFlowMgr.Follow (__pSymbol->m_Local.pConditionBlock);
;
#line 10147 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_do_stmt:
			{
				CSymbolNode_do_stmt* __pSymbol = (CSymbolNode_do_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 186 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
	__pSymbol->m_Local.pConditionBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("do_condition"));
	__pSymbol->m_Local.pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("do_body"));
	__pSymbol->m_Local.pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("do_follow"));
	m_pModule->m_ControlFlowMgr.Follow (__pSymbol->m_Local.pBodyBlock);
;
#line 10163 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_for_stmt:
			{
				CSymbolNode_for_stmt* __pSymbol = (CSymbolNode_for_stmt*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 220 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Stmt.llk"
				
	__pSymbol->m_Local.pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("for_body"));
	__pSymbol->m_Local.pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("for_follow"));
	__pSymbol->m_Local.pConditionBlock = __pSymbol->m_Local.pBodyBlock;
	__pSymbol->m_Local.pLoopBlock = __pSymbol->m_Local.pBodyBlock;
;
#line 10179 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_property_template_specifier:
			{
				CSymbolNode_property_template_specifier* __pSymbol = (CSymbolNode_property_template_specifier*) GetSymbolTop ();
				_cls42* __pAstNode = (_cls42*) __pSymbol->m_pAstNode;
#line 193 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		__pSymbol->m_Local.pTemplate = m_pModule->m_FunctionMgr.CreatePropertyTemplate ();
	;
#line 10204 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		case ESymbol_declarator:
			{
				CSymbolNode_declarator* __pSymbol = (CSymbolNode_declarator*) GetSymbolTop ();
				_cls32* __pAstNode = (_cls32*) __pSymbol->m_pAstNode;
#line 41 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
				
		return (*__pAstNode).m_Declarator.SetTypeSpecifier (__pSymbol->m_Arg.pTypeSpecifier);
	;
#line 10223 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		case ESymbol_operator_name:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls34* __pAstNode = (_cls34*) __pSymbol->m_pAstNode;
#line 157 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Declarator.llk"
				
		(*__pAstNode).m_UnOpKind = EUnOp_Undefined;
		(*__pAstNode).m_BinOpKind = EBinOp_Undefined;
	;
#line 10242 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
				_cls25* __pAstNode = (_cls25*) __pSymbol->m_pAstNode;
#line 13 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
#line 10297 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		case ESymbol_declaration_terminator:
			{
				CSymbolNode_declaration_terminator* __pSymbol = (CSymbolNode_declaration_terminator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 173 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Decl.llk"
				
		if (m_pAttributeBlock)
		{
			err::SetFormatStringError (_T("unused attribute block in declaration"));
			return false;
		}

		m_pLastDeclaredItem = NULL;
	;
#line 10317 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		case ESymbol_type_specifier_modifier_list:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls26* __pAstNode = (_cls26*) __pSymbol->m_pAstNode;
#line 51 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_DeclarationSpecifier.llk"
				
		PopTypeSpecifier (&(*__pAstNode).m_TypeSpecifier);
	;
#line 10336 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_unary_operator_expr:
			{
				CSymbolNode_unary_operator_expr* __pSymbol = (CSymbolNode_unary_operator_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 478 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
		if (m_pAutoEvType && __pSymbol->m_Arg.pValue->GetType ()->IsBindablePropertyType ())
			m_BindablePropertyList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
#line 10388 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		case ESymbol_primary_expr:
			{
				CSymbolNode_primary_expr* __pSymbol = (CSymbolNode_primary_expr*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 660 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
		if (m_pAutoEvType && __pSymbol->m_Arg.pValue->GetType ()->IsBindablePropertyType ())
			m_BindablePropertyList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
#line 10406 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		case ESymbol_postfix_operator:
			{
				CSymbolNode_postfix_operator* __pSymbol = (CSymbolNode_postfix_operator*) GetSymbolTop ();
				CAstNode* __pAstNode = __pSymbol->m_pAstNode;
#line 599 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Expr.llk"
				
		if (m_pAutoEvType && __pSymbol->m_Arg.pValue->GetType ()->IsBindablePropertyType ())
			m_BindablePropertyList.InsertTail (*__pSymbol->m_Arg.pValue);
	;
#line 10420 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_enum_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls35* __pAstNode = (_cls35*) __pSymbol->m_pAstNode;
#line 11 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_NamedTypeSpecifier.llk"
				
		if ((*__pAstNode).m_pType->GetName ().IsEmpty () || ((*__pAstNode).m_pType->GetFlags () & EEnumTypeFlag_Exposed))
			return m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->ExposeEnumMembers ((*__pAstNode).m_pType);
	;
#line 10453 "D:/Prj/Ninja/axl3/axl_jnc/axl_jnc_Parser.h"
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
					
				pTransition->m_ProductionIndex = 1244;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case ';':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 156;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_NoVirtual:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Strong:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Closure:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 1246;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 3:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case ';':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 156;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_NoVirtual:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Strong:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Closure:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 4:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 165;
				pTransition->m_ResolverIndex = 169;
				pTransition->m_ResolverElseIndex = 159;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 5:
			
			switch (LookaheadToken)
			{
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 165;
				pTransition->m_ResolverIndex = 169;
				pTransition->m_ResolverElseIndex = 159;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 6:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 159;
				pTransition->m_ResolverIndex = 248;
				pTransition->m_ResolverElseIndex = 160;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 7:
			
			switch (LookaheadToken)
			{
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 1251;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 8:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case ';':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 156;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_NoVirtual:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Strong:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Closure:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 9:
			
			switch (LookaheadToken)
			{
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 1253;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 10:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case ';':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 156;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Abstract:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Virtual:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_NoVirtual:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Mutable:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Protected:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Strong:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Closure:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 157;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 11:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 1255;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 12:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 693;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Strong:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Closure:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 692;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 13:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 478;
				pTransition->m_ResolverIndex = 216;
				pTransition->m_ResolverElseIndex = 477;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 14:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 498;
				pTransition->m_ResolverIndex = 216;
				pTransition->m_ResolverElseIndex = 1258;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 15:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 497;
				pTransition->m_ResolverIndex = 248;
				pTransition->m_ResolverElseIndex = 498;
						
				return ELaDfaResult_Resolver;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Snapshot:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Dynamic:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 498;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Strong:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Closure:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 497;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 16:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 496;
				pTransition->m_ResolverIndex = 248;
				pTransition->m_ResolverElseIndex = 498;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 17:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 556;
				pTransition->m_ResolverIndex = 245;
				pTransition->m_ResolverElseIndex = 1261;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 18:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 556;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 557;
				return ELaDfaResult_Production;
					
			}
			
		
		case 19:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 552;
				pTransition->m_ResolverIndex = 245;
				pTransition->m_ResolverElseIndex = 1263;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 20:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 552;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 553;
				return ELaDfaResult_Production;
					
			}
			
		
		case 21:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 588;
				pTransition->m_ResolverIndex = 245;
				pTransition->m_ResolverElseIndex = 587;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 22:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 626;
				pTransition->m_ResolverIndex = 216;
				pTransition->m_ResolverElseIndex = 1266;
						
				pTransition->m_Flags = axl::llk::ELaDfaNodeFlag_HasChainedResolver;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 23:
			
			pTransition->m_ProductionIndex = 625;
			pTransition->m_ResolverIndex = 937;
			pTransition->m_ResolverElseIndex = 626;
				
			return ELaDfaResult_Resolver;
			
		
		case 24:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 627;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 25:
			
			switch (LookaheadToken)
			{
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 1269;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 26:
			
			switch (LookaheadToken)
			{
			
			case '<':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '>':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '|':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '^':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '/':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '%':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '@':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_LogOr:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_LogAnd:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Eq:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Ne:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Le:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Ge:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Shl:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Shr:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_AddAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_SubAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_MulAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_DivAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_ModAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_ShlAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_ShrAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_AndAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_XorAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_OrAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_ClassAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Strong:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Closure:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_AtAssign:
					
				pTransition->m_ProductionIndex = 642;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 641;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 640;
				return ELaDfaResult_Production;
					
			}
			
		
		case 27:
			
			switch (LookaheadToken)
			{
			
			case EToken_Postfix:
					
				pTransition->m_ProductionIndex = 1271;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 28:
			
			switch (LookaheadToken)
			{
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 649;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 650;
				return ELaDfaResult_Production;
					
			
			case EToken_Ptr:
					
				pTransition->m_ProductionIndex = 651;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 29:
			
			switch (LookaheadToken)
			{
			
			case '[':
					
				pTransition->m_ProductionIndex = 1273;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 30:
			
			switch (LookaheadToken)
			{
			
			case ']':
					
				pTransition->m_ProductionIndex = 691;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 690;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 31:
			
			switch (LookaheadToken)
			{
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 1275;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 32:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case ',':
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Strong:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Closure:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 699;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 33:
			
			switch (LookaheadToken)
			{
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 1277;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 34:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case ',':
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Weak:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_PropValue:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Object:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_IntPtr:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_ReadOnly:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Volatile:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Safe:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsafe:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_NoNull:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Strong:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Cdecl:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Stdcall:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Function:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Multicast:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Event:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Bindable:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoGet:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Indexed:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Closure:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Thin:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_PreConstruct:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Operator:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 699;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			case EToken_AutoEv:
					
				pTransition->m_ProductionIndex = 698;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 35:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 159;
				pTransition->m_ResolverIndex = 248;
				pTransition->m_ResolverElseIndex = 542;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 36:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 549;
				pTransition->m_ResolverIndex = 248;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 37:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 555;
				pTransition->m_ResolverIndex = 248;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 38:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 725;
				pTransition->m_ResolverIndex = 1015;
				pTransition->m_ResolverElseIndex = 726;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 39:
			
			switch (LookaheadToken)
			{
			
			case '#':
					
				pTransition->m_ProductionIndex = 1283;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 40:
			
			switch (LookaheadToken)
			{
			
			case EToken_Pack:
					
				pTransition->m_ProductionIndex = 148;
				return ELaDfaResult_Production;
					
			
			case EToken_Endian:
					
				pTransition->m_ProductionIndex = 150;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 41:
			
			switch (LookaheadToken)
			{
			
			case '&':
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 42:
			
			switch (LookaheadToken)
			{
			
			case '+':
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 43:
			
			switch (LookaheadToken)
			{
			
			case '-':
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 44:
			
			switch (LookaheadToken)
			{
			
			case '*':
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 45:
			
			switch (LookaheadToken)
			{
			
			case '~':
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 46:
			
			switch (LookaheadToken)
			{
			
			case '!':
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 47:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 48:
			
			switch (LookaheadToken)
			{
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 49:
			
			switch (LookaheadToken)
			{
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 50:
			
			switch (LookaheadToken)
			{
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 51:
			
			switch (LookaheadToken)
			{
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 52:
			
			switch (LookaheadToken)
			{
			
			case EToken_Snapshot:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 53:
			
			switch (LookaheadToken)
			{
			
			case EToken_Dynamic:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 54:
			
			switch (LookaheadToken)
			{
			
			case EToken_New:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 55:
			
			switch (LookaheadToken)
			{
			
			case EToken_Stack:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 56:
			
			switch (LookaheadToken)
			{
			
			case EToken_Heap:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 57:
			
			switch (LookaheadToken)
			{
			
			case EToken_This:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 58:
			
			switch (LookaheadToken)
			{
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 59:
			
			switch (LookaheadToken)
			{
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 60:
			
			switch (LookaheadToken)
			{
			
			case EToken_True:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 61:
			
			switch (LookaheadToken)
			{
			
			case EToken_False:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 62:
			
			switch (LookaheadToken)
			{
			
			case EToken_Null:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 63:
			
			switch (LookaheadToken)
			{
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 625;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 626;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 64:
			
			switch (LookaheadToken)
			{
			
			case '}':
					
				pTransition->m_ProductionIndex = 630;
				pTransition->m_ResolverIndex = 937;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 65:
			
			switch (LookaheadToken)
			{
			
			case ',':
					
				pTransition->m_ProductionIndex = 630;
				pTransition->m_ResolverIndex = 937;
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


	} // namespace jnc {
	} // namespace axl {


#pragma warning (default: 4065)
