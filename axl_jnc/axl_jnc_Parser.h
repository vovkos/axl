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
	ESymbol_compound_statement_1st_pass = 5, 
	ESymbol_local_declaration = 6, 
	ESymbol_statement = 7, 
	ESymbol_pragma_pack = 8, 
	ESymbol_pack_value = 9, 
	ESymbol_qualified_identifier = 10, 
	ESymbol_type_name = 11, 
	ESymbol_type_specifier_modifier_list = 12, 
	ESymbol_pointer = 13, 
	ESymbol_declarator_suffix = 14, 
	ESymbol_expression_1st_pass = 15, 
	ESymbol_primary_expression_1st_pass = 16, 
	ESymbol_expression = 17, 
	ESymbol_logical_or_expression = 18, 
	ESymbol_constant_integer_expression = 19, 
	ESymbol_logical_and_expression = 20, 
	ESymbol_inclusive_or_expression = 21, 
	ESymbol_exclusive_or_expression = 22, 
	ESymbol_and_expression = 23, 
	ESymbol_equality_expression = 24, 
	ESymbol_relational_expression = 25, 
	ESymbol_equality_operator = 26, 
	ESymbol_shift_expression = 27, 
	ESymbol_relational_operator = 28, 
	ESymbol_additive_expression = 29, 
	ESymbol_shift_operator = 30, 
	ESymbol_multiplicative_expression = 31, 
	ESymbol_additivie_operator = 32, 
	ESymbol_unary_expression = 33, 
	ESymbol_multiplicative_operator = 34, 
	ESymbol_unary_operator = 35, 
	ESymbol_postfix_expression = 36, 
	ESymbol_assignment_operator = 37, 
	ESymbol_primary_expression = 38, 
	ESymbol_postfix_operator = 39, 
	ESymbol_member_operator = 40, 
	ESymbol_index_operator = 41, 
	ESymbol_call_operator = 42, 
	ESymbol_compound_statement = 43, 
	ESymbol_expression_statement = 44, 
	ESymbol_if_statement = 45, 
	ESymbol_switch_statement = 46, 
	ESymbol_while_statement = 47, 
	ESymbol_do_statement = 48, 
	ESymbol_for_statement = 49, 
	ESymbol_break_statement = 50, 
	ESymbol_continue_statement = 51, 
	ESymbol_return_statement = 52, 
	ESymbol_switch_block_statement = 53, 
	ESymbol_attribute_block = 54, 
	ESymbol_attribute_declarator_list = 55, 
	ESymbol_attribute_declarator = 56, 
	ESymbol_declaration_specifier_list = 57, 
	ESymbol_init_declarator_list = 58, 
	ESymbol_declaration_terminator = 59, 
	ESymbol_init_declarator = 60, 
	ESymbol_declarator = 61, 
	ESymbol_initializer = 62, 
	ESymbol_initializer_list = 63, 
	ESymbol_initializer_list_entry = 64, 
	ESymbol_statement_1st_pass = 65, 
	ESymbol_declaration_specifier = 66, 
	ESymbol_type_specifier_modifier = 67, 
	ESymbol_access_specifier = 68, 
	ESymbol_storage_class_specifier = 69, 
	ESymbol_type_specifier = 70, 
	ESymbol_type_modifier = 71, 
	ESymbol_enum_specifier = 72, 
	ESymbol_struct_specifier = 73, 
	ESymbol_class_specifier = 74, 
	ESymbol_property_specifier = 75, 
	ESymbol_qualified_type_identifier_rslv = 76, 
	ESymbol_qualified_type_identifier = 77, 
	ESymbol_property_accessor_kind = 78, 
	ESymbol_pointer_kind = 79, 
	ESymbol_array_suffix = 80, 
	ESymbol_formal_argument_suffix = 81, 
	ESymbol_formal_argument_list = 82, 
	ESymbol_formal_argument_vararg = 83, 
	ESymbol_formal_argument = 84, 
	ESymbol_enum_kind = 85, 
	ESymbol_enum_block = 86, 
	ESymbol_enum_member_list = 87, 
	ESymbol_enum_member = 88, 
	ESymbol_struct_kind = 89, 
	ESymbol_inheritance_list = 90, 
	ESymbol_struct_block = 91, 
	ESymbol_struct_member_declaration = 92, 
	ESymbol_struct_member_declarator_list = 93, 
	ESymbol_struct_member_declarator = 94, 
	ESymbol_struct_member_bit_field = 95, 
	ESymbol_class_kind = 96, 
	ESymbol_class_block = 97, 
	ESymbol_class_member_declaration = 98, 
	ESymbol_class_member_declarator_list = 99, 
	ESymbol_class_member_declarator = 100, 
	ESymbol_property_block = 101, 
	ESymbol_property_accessor_declaration = 102, 
	ESymbol_property_declarator = 103, 
	
};

//.............................................................................

class CParser: public axl::llk::CParserT <CParser, CToken>
{
public:
	enum
	{
		Lookahead          = 2,
		PragmaNestingLimit = 1,

		StartSymbol        = 0,
		StartPragmaSymbol  = 176,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 100,
		SymbolCount        = 177,
		SequenceCount      = 193,
		ActionCount        = 68,
		ArgumentCount      = 42,
		BeaconCount        = 37,
		LaDfaCount         = 23,

		TotalCount         = 640,

		NamedSymbolCount   = 104,

		TokenFirst         = 0,
		TokenEnd           = 100,
		SymbolFirst        = 100,
		NamedSymbolEnd     = 204,
		SymbolEnd          = 277,
		SequenceFirst      = 277,
		SequenceEnd        = 470,
		ActionFirst        = 470,
		ActionEnd          = 538,
		ArgumentFirst      = 538,
		ArgumentEnd        = 580,
		BeaconFirst        = 580,
		BeaconEnd          = 617,
		LaDfaFirst         = 617,
		LaDfaEnd           = 640,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class _cls1: public CAst
	{
	public:
#line 32 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CQualifiedIdentifier m_Identifier;
;
#line 187 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls2: public CAst
	{
	public:
#line 50 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Decl.llk"
		
	CType* m_pType;
;
#line 197 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls4: public CAst
	{
	public:
#line 33 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Expr.llk"
		
	intptr_t m_Value;
;
#line 207 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls5: public CAst
	{
	public:
#line 62 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CDeclSpecifiers m_DeclSpecifiers;
;
#line 217 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls6: public CAst
	{
	public:
#line 70 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CTypeSpecifierModifiers m_TypeSpecifier;
;
#line 227 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls7: public CAst
	{
	public:
#line 283 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CDeclarator m_Declarator;
;
#line 237 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls8: public CAst
	{
	public:
#line 317 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	EType m_Type;
;
#line 247 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls9: public CAst
	{
	public:
#line 397 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
		
	CType* m_pType;
;
#line 257 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls10: public CAst
	{
	public:
#line 4 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
		
	CEnumType* m_pType;
;
#line 267 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls11: public CAst
	{
	public:
#line 38 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
		
	EType m_Type;
;
#line 277 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls12: public CAst
	{
	public:
#line 4 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
		
	CStructType* m_pType;
;
#line 287 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls13: public CAst
	{
	public:
#line 29 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
		
	EType m_Type;
;
#line 297 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls14: public CAst
	{
	public:
#line 4 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
		
	CClassType* m_pType;
;
#line 307 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls15: public CAst
	{
	public:
#line 29 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
		
	EType m_Type;
;
#line 317 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls16: public CAst
	{
	public:
#line 4 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	CProperty* m_pProperty;
;
#line 327 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class CPropertyBlock: public CAst
	{
	public:
#line 15 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	CFunction* m_pGetter;
	CFunctionOverload m_Setter;

	CPropertyBlock ()
	{
		m_pGetter = NULL;
	}
;
#line 343 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls18: public CAst
	{
	public:
#line 41 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	CDeclarator m_Declarator;
;
#line 353 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	
	class _cls19: public CAst
	{
	public:
#line 55 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
		
	EPropertyAccessor m_PropertyAccessorKind;
;
#line 363 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
	};
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	
	class CSymbolNode_using_declaration: public CSymbolNode
	{
	public:
			
		struct
		{
#line 18 "D:/Projects.v/axl3/axl_jnc/axl_jnc_NamespaceDeclaration.llk"
			
		CQualifiedIdentifier Identifier;
	;
#line 383 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_using_declaration ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_using_declaration;
			
		}
	};
		
	
	
	
	
	
	
	
	
	class CSymbolNode_type_name: public CSymbolNode
	{
	public:
			
		struct
		{
#line 55 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Decl.llk"
				
		CDeclarator Declarator;
	;
#line 412 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 304 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 433 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 306 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		CDeclPointer* pPointer;
	;
#line 442 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 334 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 462 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_declarator_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_declarator_suffix;
			
		}
	};
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	class CSymbolNode_init_declarator_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 11 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclSpecifiers* pDeclSpecifiers;
#line 525 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_init_declarator_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_init_declarator_list;
			
		}
	};
		
	
	
	class CSymbolNode_init_declarator: public CSymbolNode
	{
	public:
			
		struct
		{
#line 15 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclSpecifiers* pDeclSpecifiers;
#line 546 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_init_declarator ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_init_declarator;
			
		}
	};
		
	
	
	
	
	
	
	class CSymbolNode_declaration_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 77 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclSpecifiers* pDeclSpecifiers;
#line 571 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_declaration_specifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_declaration_specifier;
			
		}
	};
		
	
	class CSymbolNode_type_specifier_modifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 84 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CTypeSpecifierModifiers* pTypeSpecifier;
#line 591 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_type_specifier_modifier ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_type_specifier_modifier;
			
		}
	};
		
	
	class CSymbolNode_access_specifier: public CSymbolNode
	{
	public:
			
		struct
		{
#line 89 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CAccessSpecifier* pAccessSpecifier;
#line 611 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 104 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclSpecifiers* pDeclSpecifiers;
#line 631 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 157 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CTypeSpecifier* pTypeSpecifier;
#line 651 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 118 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CTypeModifiers* pTypeModifiers;
#line 671 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 339 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 699 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_array_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_array_suffix;
			
		}
	};
		
	
	class CSymbolNode_formal_argument_suffix: public CSymbolNode
	{
	public:
			
		struct
		{
#line 350 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclarator* pDeclarator;
#line 719 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		struct
		{
#line 352 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
		CDeclFormalArgSuffix* pArgSuffix;
	;
#line 728 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Local;	
			
		CSymbolNode_formal_argument_suffix ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_formal_argument_suffix;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	class CSymbolNode_formal_argument_list: public CSymbolNode
	{
	public:
			
		struct
		{
#line 364 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclFormalArgSuffix* pArgSuffix;
#line 750 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_formal_argument_list ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_formal_argument_list;
			
		}
	};
		
	
	class CSymbolNode_formal_argument_vararg: public CSymbolNode
	{
	public:
			
		struct
		{
#line 380 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclFormalArgSuffix* pArgSuffix;
#line 770 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_formal_argument_vararg ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_formal_argument_vararg;
			
		}
	};
		
	
	class CSymbolNode_formal_argument: public CSymbolNode
	{
	public:
			
		struct
		{
#line 370 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			CDeclFormalArgSuffix* pArgSuffix;
#line 790 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_formal_argument ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_formal_argument;
			
		}
	};
		
	
	
	class CSymbolNode_enum_block: public CSymbolNode
	{
	public:
			
		struct
		{
#line 52 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			CEnumType* pType;
#line 811 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 56 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			CEnumType* pType;
#line 831 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 60 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			CEnumType* pType;
#line 851 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 43 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CStructType* pType;
#line 873 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 47 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CStructType* pType;
#line 893 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 56 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CTypeSpecifierModifiers* pTypeSpecifier; CStructType* pType;
#line 913 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 60 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			CTypeSpecifierModifiers* pTypeSpecifier; CStructType* pType;
#line 933 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
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
#line 47 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CClassType* pType;
#line 955 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 51 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CClassType* pType;
#line 975 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 60 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CDeclSpecifiers* pDeclSpecifiers; CClassType* pType;
#line 995 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
#line 64 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			CDeclSpecifiers* pDeclSpecifiers; CClassType* pType;
#line 1015 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
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
#line 28 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			CPropertyBlock* pBlock;
#line 1036 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
		} m_Arg;	
			
		CSymbolNode_property_accessor_declaration ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_property_accessor_declaration;
			
		}
	};
		
	
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	
	
	CModule* m_pModule;
	size_t m_StructPackFactor;

	CParser ()
	{
		m_pModule = NULL;
		m_StructPackFactor = 8;
	}

	bool 
	IsType (const CQualifiedIdentifier& Identifier);

	CType*
	FindType (const CQualifiedIdentifier& Identifier);

	bool
	Declare (
		CDeclSpecifiers* pDeclSpecifiers,
		CDeclarator* pDeclarator
		);

	bool
	Declare (
		CNamedType* pType,
		const CToken::CPos& Pos
		);

	bool
	DeclareStructMember (
		CStructType* pStructType,
		CTypeSpecifierModifiers* pTypeSpecifier,
		CDeclarator* pDeclarator
		);

	bool
	DeclareClassMember (
		CClassType* pClassType,
		CDeclSpecifiers* pDeclSpecifiers,
		CDeclarator* pDeclarator
		);

	bool
	DeclareFormalArg (
		CDeclFormalArgSuffix* pArgSuffix,
		CTypeSpecifierModifiers* pTypeSpecifier,
		CDeclarator* pDeclarator,
		CValue* pDefaultValue
		);

	bool
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


	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	static
	size_t*
	GetParseTable ()
	{
		static
		size_t _ParseTable [] = 
		{
			0, -1, 277, -1, -1, 277, -1, 277, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 277, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, -1, 277, 277, 277, 277, 277, 277, -1, -1, 
			-1, -1, 2, -1, -1, 105, -1, 104, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 104, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 102, 103, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, -1, 104, 104, 104, 104, 104, 104, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 347, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 349, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 350, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 350, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, -1, 350, 350, 350, 350, 350, 350, -1, -1, 
			-1, -1, -1, -1, -1, 360, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 107, -1, -1, 107, -1, 104, -1, 107, -1, -1, -1, -1, -1, 107, -1, -1, 107, 107, 107, -1, -1, 107, 107, -1, -1, -1, 107, -1, 617, -1, -1, -1, -1, -1, -1, -1, -1, 107, 107, 107, 107, -1, -1, -1, -1, -1, -1, -1, -1, -1, 107, 107, 107, -1, 107, -1, 107, 107, 107, 107, 107, 107, -1, 103, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, -1, 104, 104, 104, 104, 104, 104, -1, -1, 
			-1, -1, 2, -1, -1, 143, -1, -1, -1, 144, -1, -1, -1, -1, -1, 144, -1, -1, 144, 144, 144, -1, -1, 144, 144, -1, -1, -1, 144, -1, 144, -1, -1, -1, -1, -1, -1, -1, -1, 144, 144, 144, 144, -1, -1, -1, -1, -1, -1, -1, -1, -1, 144, 144, 145, -1, 146, -1, 147, 148, 149, 150, 151, 152, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 278, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 28, 29, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 279, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 282, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, -1, 282, 282, 282, 282, 282, 282, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 363, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, 363, -1, 363, 363, 363, 363, 363, 363, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 404, -1, -1, -1, -1, 404, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 408, -1, 409, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 284, 
			-1, 1, 1, 1, 1, 287, 1, 289, 1, 291, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 293, -1, -1, -1, -1, -1, 293, -1, -1, 293, 293, 293, -1, -1, 293, 293, -1, -1, -1, 293, -1, 293, -1, -1, -1, -1, -1, -1, -1, -1, 293, 293, 293, 293, -1, -1, -1, -1, -1, -1, -1, -1, -1, 293, 293, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 296, -1, -1, -1, -1, -1, 296, -1, -1, 296, 296, 296, -1, -1, 296, 296, -1, -1, -1, 296, -1, 296, -1, -1, -1, -1, -1, -1, -1, -1, 296, 296, 296, 296, -1, -1, -1, -1, -1, -1, -1, -1, -1, 296, 296, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 294, -1, -1, -1, -1, -1, 294, -1, -1, 294, 294, 294, -1, -1, 294, 294, -1, -1, -1, 294, -1, 294, -1, -1, -1, -1, -1, -1, -1, -1, 294, 294, 294, 294, -1, -1, -1, -1, -1, -1, -1, -1, -1, 294, 294, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 298, -1, -1, -1, -1, -1, 298, -1, -1, 298, 298, 298, -1, -1, 298, 298, -1, -1, -1, 298, -1, 298, -1, -1, -1, -1, -1, -1, -1, -1, 298, 298, 298, 298, -1, -1, -1, -1, -1, -1, -1, -1, -1, 298, 298, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 300, -1, -1, -1, -1, -1, 300, -1, -1, 300, 300, 300, -1, -1, 300, 300, -1, -1, -1, 300, -1, 300, -1, -1, -1, -1, -1, -1, -1, -1, 300, 300, 300, 300, -1, -1, -1, -1, -1, -1, -1, -1, -1, 300, 300, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 302, -1, -1, -1, -1, -1, 302, -1, -1, 302, 302, 302, -1, -1, 302, 302, -1, -1, -1, 302, -1, 302, -1, -1, -1, -1, -1, -1, -1, -1, 302, 302, 302, 302, -1, -1, -1, -1, -1, -1, -1, -1, -1, 302, 302, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 304, -1, -1, -1, -1, -1, 304, -1, -1, 304, 304, 304, -1, -1, 304, 304, -1, -1, -1, 304, -1, 304, -1, -1, -1, -1, -1, -1, -1, -1, 304, 304, 304, 304, -1, -1, -1, -1, -1, -1, -1, -1, -1, 304, 304, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 306, -1, -1, -1, -1, -1, 306, -1, -1, 306, 306, 306, -1, -1, 306, 306, -1, -1, -1, 306, -1, 306, -1, -1, -1, -1, -1, -1, -1, -1, 306, 306, 306, 306, -1, -1, -1, -1, -1, -1, -1, -1, -1, 306, 306, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 308, -1, -1, -1, -1, -1, 308, -1, -1, 308, 308, 308, -1, -1, 308, 308, -1, -1, -1, 308, -1, 308, -1, -1, -1, -1, -1, -1, -1, -1, 308, 308, 308, 308, -1, -1, -1, -1, -1, -1, -1, -1, -1, 308, 308, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33, 34, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 310, -1, -1, -1, -1, -1, 310, -1, -1, 310, 310, 310, -1, -1, 310, 310, -1, -1, -1, 310, -1, 310, -1, -1, -1, -1, -1, -1, -1, -1, 310, 310, 310, 310, -1, -1, -1, -1, -1, -1, -1, -1, -1, 310, 310, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, 17, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 35, 36, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 312, -1, -1, -1, -1, -1, 312, -1, -1, 312, 312, 312, -1, -1, 312, 312, -1, -1, -1, 312, -1, 312, -1, -1, -1, -1, -1, -1, -1, -1, 312, 312, 312, 312, -1, -1, -1, -1, -1, -1, -1, -1, -1, 312, 312, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 37, 38, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 314, -1, -1, -1, -1, -1, 314, -1, -1, 314, 314, 314, -1, -1, 314, 314, -1, -1, -1, 314, -1, 314, -1, -1, -1, -1, -1, -1, -1, -1, 314, 314, 314, 314, -1, -1, -1, -1, -1, -1, -1, -1, -1, 314, 314, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 18, 19, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 316, -1, -1, -1, -1, -1, 316, -1, -1, 316, 316, 316, -1, -1, 316, 316, -1, -1, -1, 316, -1, 316, -1, -1, -1, -1, -1, -1, -1, -1, 316, 316, 316, 316, -1, -1, -1, -1, -1, -1, -1, -1, -1, 316, 316, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 20, 21, 22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 318, -1, -1, -1, -1, -1, 15, -1, -1, 18, 19, 20, -1, -1, 23, 24, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 39, 40, 41, 42, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 320, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 320, -1, 320, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 320, 320, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 326, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 28, -1, 30, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 52, 53, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 140, -1, -1, 141, -1, 142, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 39, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 321, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 322, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 618, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 328, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 329, -1, -1, -1, -1, -1, 329, -1, -1, 329, 329, 329, -1, -1, 329, 329, -1, -1, -1, 329, -1, 329, -1, -1, -1, -1, -1, -1, -1, -1, 329, 329, 329, 329, -1, -1, -1, -1, -1, -1, -1, -1, -1, 329, 329, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 330, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 332, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 336, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 337, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 338, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 339, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 340, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 341, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 107, -1, -1, 107, -1, -1, -1, 107, -1, -1, -1, -1, -1, 107, -1, -1, 107, 107, 107, -1, -1, 107, 107, -1, -1, -1, 107, 335, 107, -1, -1, -1, -1, -1, -1, -1, -1, 107, 107, 107, 107, -1, -1, -1, -1, -1, -1, -1, -1, -1, 107, 107, 107, -1, 107, 334, 107, 107, 107, 107, 107, 107, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 342, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 344, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 344, -1, -1, -1, 344, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 346, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 361, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, 361, -1, 361, 361, 361, 361, 361, 361, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 352, -1, -1, -1, -1, 352, -1, -1, -1, -1, -1, -1, -1, -1, -1, 352, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, 105, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 355, -1, -1, -1, -1, 355, -1, -1, -1, -1, -1, -1, -1, -1, -1, 355, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 401, -1, -1, -1, -1, 401, -1, -1, -1, -1, -1, -1, -1, -1, -1, 401, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 356, -1, -1, -1, 117, -1, -1, -1, -1, -1, 117, -1, -1, 117, 117, 117, -1, -1, 117, 117, -1, -1, -1, 117, -1, 117, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 358, 358, -1, -1, 358, -1, -1, -1, -1, -1, 358, -1, -1, 358, 358, 358, -1, -1, 358, 358, -1, 358, -1, 358, -1, 358, -1, -1, -1, -1, -1, -1, -1, -1, 358, 358, 358, 358, -1, -1, -1, -1, -1, -1, -1, -1, -1, 358, 358, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 162, 0, -1, -1, 162, -1, -1, -1, -1, -1, 162, -1, -1, 162, 162, 162, -1, -1, 162, 162, -1, 0, -1, 162, -1, 162, -1, -1, -1, -1, -1, -1, -1, -1, 162, 162, 162, 162, -1, -1, -1, -1, -1, -1, -1, -1, -1, 162, 162, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 1, 1, 1, 1, 105, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 365, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 366, 366, 367, 367, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, 365, -1, 365, 365, 365, 365, 365, 365, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 368, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 369, 620, 369, 369, 369, 369, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, 368, -1, 368, 368, 368, 368, 368, 368, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 370, 371, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 372, 373, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 245, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 398, -1, -1, -1, -1, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, -1, 395, 395, 396, 396, 397, 397, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 374, 375, 376, 377, 378, 379, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 622, 624, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 626, 628, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 630, 632, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 462, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 421, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 422, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 468, 469, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 407, -1, -1, -1, -1, 406, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 634, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 636, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 415, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, 415, -1, 415, 415, 415, 415, 415, 415, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 420, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 418, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, 418, -1, 418, 418, 418, 418, 418, 418, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 426, 427, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 428, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 429, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 429, -1, -1, -1, 429, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 432, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 436, 437, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 451, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 439, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, 105, -1, 440, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 440, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, -1, 440, 440, 440, 440, 440, 440, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 441, -1, -1, -1, -1, 441, -1, -1, -1, -1, -1, -1, -1, -1, -1, 441, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 444, -1, -1, -1, -1, 444, -1, -1, -1, -1, -1, -1, -1, -1, -1, 444, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 119, -1, -1, -1, -1, -1, 119, -1, -1, 119, 119, 119, -1, -1, 119, 119, -1, -1, -1, 119, -1, 119, -1, -1, -1, -1, -1, -1, -1, -1, 119, 119, 119, 119, -1, -1, -1, -1, -1, -1, -1, -1, -1, 119, 119, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 449, 450, -1, -1, 
			-1, -1, -1, -1, -1, 454, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, 105, -1, 455, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 455, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, 455, -1, 455, 455, 455, 455, 455, 455, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, -1, -1, -1, -1, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, 457, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 460, -1, -1, -1, -1, 460, -1, -1, -1, -1, -1, -1, -1, -1, -1, 460, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 464, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 465, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, 465, -1, 465, 465, 465, 465, 465, 465, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 467, -1, -1, -1, -1, 467, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 467, 467, 
			0, -1, 277, -1, -1, 277, -1, 277, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 277, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, -1, 277, 277, 277, 277, 277, 277, -1, -1, 
			-1, -1, 0, -1, 280, 0, -1, 0, -1, 0, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, 0, -1, 0, 0, -1, -1, -1, -1, 281, -1, -1, -1, -1, 281, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 283, -1, 283, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 285, 0, 285, 285, 285, 0, 285, 0, 285, 0, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 0, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 285, 
			-1, 286, 286, 286, 286, 286, 0, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 286, 
			-1, 288, 288, 288, 288, 288, 288, 288, 0, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 288, 
			-1, 290, 290, 290, 290, 290, 290, 290, 290, 290, 0, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 290, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 292, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 295, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 0, 297, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 299, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 301, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 303, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, 305, 305, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 307, 307, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 307, 307, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 309, 309, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 311, 311, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 313, 313, 313, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 315, -1, -1, -1, -1, -1, 315, -1, -1, 315, 315, 315, -1, -1, 315, 315, -1, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 315, 315, 315, 315, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 317, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 317, 317, 317, 317, 317, 317, 317, 317, 317, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 319, 0, 0, 319, 0, 319, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 319, 319, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 324, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 327, -1, -1, 327, 0, 327, -1, 327, -1, -1, -1, -1, -1, 327, -1, -1, 327, 327, 327, -1, -1, 327, 327, -1, -1, -1, 327, -1, 327, -1, -1, -1, -1, -1, -1, -1, -1, 327, 327, 327, 327, -1, -1, -1, -1, -1, -1, -1, -1, -1, 327, 327, 327, -1, 327, -1, 327, 327, 327, 327, 327, 327, -1, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, 327, -1, 327, 327, 327, 327, 327, 327, -1, -1, 
			-1, -1, 0, -1, -1, 0, 0, 0, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, 0, 0, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 331, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 333, -1, -1, 333, 0, -1, -1, 333, -1, -1, -1, -1, -1, 333, -1, -1, 333, 333, 333, -1, -1, 333, 333, -1, -1, -1, 333, 333, 333, -1, -1, -1, -1, -1, -1, -1, -1, 333, 333, 333, 333, -1, -1, -1, -1, -1, -1, -1, -1, -1, 333, 333, 333, -1, 333, 333, 333, 333, 333, 333, 333, 333, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, 117, -1, -1, -1, -1, -1, 117, -1, -1, 117, 117, 117, -1, -1, 117, 117, -1, -1, -1, 117, -1, 117, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, 117, -1, -1, -1, -1, -1, 117, -1, -1, 117, 117, 117, -1, -1, 117, 117, -1, -1, -1, 117, -1, 117, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 117, 0, -1, -1, -1, -1, 117, -1, -1, 117, 117, 117, -1, -1, 117, 117, -1, -1, -1, 117, -1, 117, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, 117, -1, -1, -1, -1, -1, 117, -1, -1, 117, 117, 117, -1, -1, 117, 117, -1, -1, -1, 117, -1, 117, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, 117, 117, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 343, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 345, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 348, -1, -1, 348, 0, 348, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 348, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, 348, -1, 348, 348, 348, 348, 348, 348, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 351, -1, -1, -1, -1, 351, -1, -1, -1, -1, -1, -1, -1, -1, -1, 351, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 353, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 354, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 357, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 359, 359, 359, 359, 359, 0, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 359, 
			-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 638, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, 362, -1, 362, 362, 362, 362, 362, 362, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, -1, 0, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 639, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, 364, -1, 364, 364, 364, 364, 364, 364, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 399, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 400, -1, -1, -1, -1, 400, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 402, 0, -1, 0, -1, 0, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 403, -1, 403, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 0, -1, 0, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 405, 405, 405, 405, 405, 405, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 416, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 417, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 419, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 424, -1, 0, -1, 0, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 430, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 431, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 190, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 434, -1, 0, -1, 0, 0, -1, 434, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 190, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 438, -1, -1, 438, 0, 438, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 438, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, 438, -1, 438, 438, 438, 438, 438, 438, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 442, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 443, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 445, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 190, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 447, -1, 0, -1, 0, 0, -1, 447, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 190, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 452, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 453, -1, -1, 453, 0, 453, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 453, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, -1, 453, 453, 453, 453, 453, 453, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, 154, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 456, -1, -1, -1, -1, 456, -1, -1, -1, -1, -1, -1, -1, -1, -1, 456, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 458, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, 459, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 461, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 463, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 463, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, 463, -1, 463, 463, 463, 463, 463, 463, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 466, -1, -1, -1, -1, 466, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 
			-1, -1, -1, 108, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			
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
			204, 101,  -1,
			109, 27, 3,  -1,
			205, 470, 580,  -1,
			205, 471, 581, 4,  -1,
			206, 538, 113,  -1,
			207, 206, 112,  -1,
			207, 539, 114,  -1,
			208, 116,  -1,
			208, 116,  -1,
			209, 116,  -1,
			6, 209, 116, 5,  -1,
			210, 116,  -1,
			8, 210, 116, 7,  -1,
			211, 116,  -1,
			10, 211, 9,  -1,
			117, 12, 117, 11,  -1,
			212, 118,  -1,
			472, 117,  -1,
			213, 120, 31,  -1,
			213, 120,  -1,
			214, 121, 32,  -1,
			214, 121,  -1,
			215, 122, 13,  -1,
			215, 122,  -1,
			216, 123, 14,  -1,
			216, 123,  -1,
			217, 124, 15,  -1,
			217, 124,  -1,
			218, 125, 126,  -1,
			218, 125,  -1,
			219, 127, 128,  -1,
			219, 127,  -1,
			220, 129, 130,  -1,
			220, 129,  -1,
			221, 131, 132,  -1,
			221, 131,  -1,
			222, 133, 134,  -1,
			222, 133,  -1,
			223, 135,  -1,
			224, 136, 223,  -1,
			117, 137,  -1,
			10, 111, 9,  -1,
			225, 139,  -1,
			225, 138,  -1,
			30, 4,  -1,
			8, 117, 7,  -1,
			10, 226, 117, 9,  -1,
			226, 117, 26,  -1,
			10, 9,  -1,
			10, 117, 9,  -1,
			227, 106,  -1,
			6, 227, 5,  -1,
			2, 117,  -1,
			228, 107, 10, 117, 9, 54,  -1,
			107, 55,  -1,
			6, 229, 5, 10, 117, 9, 56,  -1,
			229, 153,  -1,
			12, 119, 57,  -1,
			12, 29,  -1,
			107, 10, 117, 9, 58,  -1,
			2, 10, 117, 9, 58, 107, 59,  -1,
			107, 10, 232, 2, 231, 2, 230, 9, 60,  -1,
			2, 61,  -1,
			2, 62,  -1,
			2, 233, 63,  -1,
			8, 155, 7,  -1,
			234, 156, 26,  -1,
			234, 156,  -1,
			115, 25,  -1,
			473, 235, 582,  -1,
			475, 6, 236, 5, 474, 584, 583,  -1,
			236, 101,  -1,
			2, 110, 65,  -1,
			159, 238, 585, 237,  -1,
			540, 158,  -1,
			239, 541, 160,  -1,
			239, 542, 160, 26,  -1,
			115, 25,  -1,
			476, 240, 586,  -1,
			6, 163, 5,  -1,
			241, 164, 26,  -1,
			241, 164,  -1,
			242, 165,  -1,
			6, 242, 5,  -1,
			243, 543, 166,  -1,
			243, 543, 166,  -1,
			244, 544, 167,  -1,
			244, 544, 167,  -1,
			545, 167,  -1,
			546, 168,  -1,
			547, 169,  -1,
			548, 170,  -1,
			549, 171,  -1,
			477, 66,  -1,
			478, 67,  -1,
			479, 68,  -1,
			480, 69,  -1,
			481, 70,  -1,
			482, 71,  -1,
			483, 72,  -1,
			484, 73,  -1,
			485, 74,  -1,
			486, 75,  -1,
			487, 76,  -1,
			488, 77,  -1,
			489, 78,  -1,
			490, 79,  -1,
			491, 80,  -1,
			492, 81,  -1,
			493, 82,  -1,
			494, 83,  -1,
			495, 84,  -1,
			496, 85,  -1,
			497, 86,  -1,
			498, 87,  -1,
			499, 88,  -1,
			500, 89,  -1,
			501, 90,  -1,
			502, 587,  -1,
			503, 588,  -1,
			504, 589,  -1,
			505, 590,  -1,
			506, 591,  -1,
			246, 550, 113,  -1,
			248, 247, 507, 592, 246,  -1,
			508, 593, 4,  -1,
			248, 551, 114,  -1,
			249, 509, 594,  -1,
			249, 552, 171,  -1,
			510, 20,  -1,
			511, 15,  -1,
			553, 180,  -1,
			554, 181,  -1,
			512, 8, 115, 7,  -1,
			513, 8, 7,  -1,
			10, 555, 182, 9,  -1,
			10, 556, 183, 9,  -1,
			10, 9,  -1,
			251, 250, 557, 184,  -1,
			250, 558, 184, 26,  -1,
			559, 183, 26,  -1,
			514, 252, 596, 595,  -1,
			115, 25,  -1,
			515, 91,  -1,
			516, 597,  -1,
			517, 598,  -1,
			253, 518, 600, 599,  -1,
			560, 186,  -1,
			561, 186, 519, 601,  -1,
			520, 92,  -1,
			521, 93,  -1,
			6, 562, 187, 5,  -1,
			254, 563, 188,  -1,
			254, 564, 188, 26,  -1,
			115, 25,  -1,
			522, 255, 602,  -1,
			257, 523, 604, 603,  -1,
			565, 191, 256,  -1,
			566, 191, 258, 524, 605,  -1,
			525, 94,  -1,
			526, 95,  -1,
			259, 567, 192,  -1,
			6, 259, 5,  -1,
			2, 568, 193, 606, 260,  -1,
			261, 569, 194,  -1,
			261, 570, 194, 26,  -1,
			195, 12,  -1,
			527, 263, 262, 607,  -1,
			162, 25,  -1,
			265, 528, 609, 608,  -1,
			571, 197, 264,  -1,
			572, 197, 266, 529, 610,  -1,
			530, 96,  -1,
			531, 97,  -1,
			267, 177, 12,  -1,
			267, 177, 26,  -1,
			268, 573, 198,  -1,
			6, 268, 5,  -1,
			159, 270, 611, 269,  -1,
			574, 199,  -1,
			271, 575, 200,  -1,
			271, 576, 200, 26,  -1,
			195, 12,  -1,
			532, 273, 272, 612,  -1,
			162, 25,  -1,
			533, 613, 71,  -1,
			274, 577, 202,  -1,
			6, 274, 577, 202, 5,  -1,
			534, 159, 615, 614,  -1,
			275, 578, 113,  -1,
			535, 579, 181, 616, 275,  -1,
			536, 98,  -1,
			537, 99,  -1,
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 7, 11, 16, 20, 24, 28, 31, 34, 37, 42, 45, 50, 53, 57, 62, 65, 68, 72, 75, 79, 82, 86, 89, 93, 96, 100, 103, 107, 110, 114, 117, 121, 124, 128, 131, 135, 138, 141, 145, 148, 152, 155, 158, 161, 165, 170, 174, 177, 181, 184, 188, 191, 198, 201, 209, 212, 216, 219, 225, 233, 243, 246, 249, 253, 257, 261, 264, 267, 271, 279, 282, 286, 291, 294, 298, 303, 306, 310, 314, 318, 321, 324, 328, 332, 336, 340, 344, 347, 350, 353, 356, 359, 362, 365, 368, 371, 374, 377, 380, 383, 386, 389, 392, 395, 398, 401, 404, 407, 410, 413, 416, 419, 422, 425, 428, 431, 434, 437, 440, 443, 446, 449, 453, 459, 463, 467, 471, 475, 478, 481, 484, 487, 492, 496, 501, 506, 509, 514, 519, 523, 528, 531, 534, 537, 540, 545, 548, 553, 556, 559, 564, 568, 573, 576, 580, 585, 589, 595, 598, 601, 605, 609, 615, 619, 624, 627, 632, 635, 640, 644, 650, 653, 656, 660, 664, 668, 672, 677, 680, 684, 689, 692, 697, 700, 704, 708, 714, 719, 723, 729, 732, 
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
		
		case '#':
			return 3;
		
		case '.':
			return 4;
		
		case '{':
			return 5;
		
		case '}':
			return 6;
		
		case '[':
			return 7;
		
		case ']':
			return 8;
		
		case '(':
			return 9;
		
		case ')':
			return 10;
		
		case '?':
			return 11;
		
		case ':':
			return 12;
		
		case '|':
			return 13;
		
		case '^':
			return 14;
		
		case '&':
			return 15;
		
		case '<':
			return 16;
		
		case '>':
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
		
		case '~':
			return 23;
		
		case '!':
			return 24;
		
		case '=':
			return 25;
		
		case ',':
			return 26;
		
		case EToken_Pack:
			return 27;
		
		case EToken_Integer:
			return 28;
		
		case EToken_Default:
			return 29;
		
		case EToken_Identifier:
			return 30;
		
		case EToken_LogicalOr:
			return 31;
		
		case EToken_LogicalAnd:
			return 32;
		
		case EToken_Eq:
			return 33;
		
		case EToken_Ne:
			return 34;
		
		case EToken_Le:
			return 35;
		
		case EToken_Ge:
			return 36;
		
		case EToken_Shl:
			return 37;
		
		case EToken_Shr:
			return 38;
		
		case EToken_Inc:
			return 39;
		
		case EToken_Dec:
			return 40;
		
		case EToken_SizeOf:
			return 41;
		
		case EToken_TypeOf:
			return 42;
		
		case EToken_AddAssign:
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
		
		case EToken_Fp:
			return 52;
		
		case EToken_Literal:
			return 53;
		
		case EToken_If:
			return 54;
		
		case EToken_Else:
			return 55;
		
		case EToken_Switch:
			return 56;
		
		case EToken_Case:
			return 57;
		
		case EToken_While:
			return 58;
		
		case EToken_Do:
			return 59;
		
		case EToken_For:
			return 60;
		
		case EToken_Break:
			return 61;
		
		case EToken_Continue:
			return 62;
		
		case EToken_Return:
			return 63;
		
		case EToken_Namespace:
			return 64;
		
		case EToken_Using:
			return 65;
		
		case EToken_Public:
			return 66;
		
		case EToken_Private:
			return 67;
		
		case EToken_Static:
			return 68;
		
		case EToken_Typedef:
			return 69;
		
		case EToken_Const:
			return 70;
		
		case EToken_Property:
			return 71;
		
		case EToken_Signed:
			return 72;
		
		case EToken_Unsigned:
			return 73;
		
		case EToken_BigEndian:
			return 74;
		
		case EToken_LittleEndian:
			return 75;
		
		case EToken_Void:
			return 76;
		
		case EToken_Bool:
			return 77;
		
		case EToken_Int:
			return 78;
		
		case EToken_Char:
			return 79;
		
		case EToken_Short:
			return 80;
		
		case EToken_Long:
			return 81;
		
		case EToken_Float:
			return 82;
		
		case EToken_Double:
			return 83;
		
		case EToken_Int8:
			return 84;
		
		case EToken_Int16:
			return 85;
		
		case EToken_Int32:
			return 86;
		
		case EToken_Int64:
			return 87;
		
		case EToken_WChar:
			return 88;
		
		case EToken_Variant:
			return 89;
		
		case EToken_Auto:
			return 90;
		
		case EToken_Ellipsis:
			return 91;
		
		case EToken_Enum:
			return 92;
		
		case EToken_EnumC:
			return 93;
		
		case EToken_Struct:
			return 94;
		
		case EToken_Union:
			return 95;
		
		case EToken_Interface:
			return 96;
		
		case EToken_Class:
			return 97;
		
		case EToken_Get:
			return 98;
		
		case EToken_Set:
			return 99;
		
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
			'#', 
			'.', 
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
			'<', 
			'>', 
			'+', 
			'-', 
			'*', 
			'/', 
			'%', 
			'~', 
			'!', 
			'=', 
			',', 
			EToken_Pack, 
			EToken_Integer, 
			EToken_Default, 
			EToken_Identifier, 
			EToken_LogicalOr, 
			EToken_LogicalAnd, 
			EToken_Eq, 
			EToken_Ne, 
			EToken_Le, 
			EToken_Ge, 
			EToken_Shl, 
			EToken_Shr, 
			EToken_Inc, 
			EToken_Dec, 
			EToken_SizeOf, 
			EToken_TypeOf, 
			EToken_AddAssign, 
			EToken_MulAssign, 
			EToken_DivAssign, 
			EToken_ModAssign, 
			EToken_ShlAssign, 
			EToken_ShrAssign, 
			EToken_AndAssign, 
			EToken_XorAssign, 
			EToken_OrAssign, 
			EToken_Fp, 
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
			EToken_Namespace, 
			EToken_Using, 
			EToken_Public, 
			EToken_Private, 
			EToken_Static, 
			EToken_Typedef, 
			EToken_Const, 
			EToken_Property, 
			EToken_Signed, 
			EToken_Unsigned, 
			EToken_BigEndian, 
			EToken_LittleEndian, 
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
			_T("compound_statement_1st_pass"),
			_T("local_declaration"),
			_T("statement"),
			_T("pragma_pack"),
			_T("pack_value"),
			_T("qualified_identifier"),
			_T("type_name"),
			_T("type_specifier_modifier_list"),
			_T("pointer"),
			_T("declarator_suffix"),
			_T("expression_1st_pass"),
			_T("primary_expression_1st_pass"),
			_T("expression"),
			_T("logical_or_expression"),
			_T("constant_integer_expression"),
			_T("logical_and_expression"),
			_T("inclusive_or_expression"),
			_T("exclusive_or_expression"),
			_T("and_expression"),
			_T("equality_expression"),
			_T("relational_expression"),
			_T("equality_operator"),
			_T("shift_expression"),
			_T("relational_operator"),
			_T("additive_expression"),
			_T("shift_operator"),
			_T("multiplicative_expression"),
			_T("additivie_operator"),
			_T("unary_expression"),
			_T("multiplicative_operator"),
			_T("unary_operator"),
			_T("postfix_expression"),
			_T("assignment_operator"),
			_T("primary_expression"),
			_T("postfix_operator"),
			_T("member_operator"),
			_T("index_operator"),
			_T("call_operator"),
			_T("compound_statement"),
			_T("expression_statement"),
			_T("if_statement"),
			_T("switch_statement"),
			_T("while_statement"),
			_T("do_statement"),
			_T("for_statement"),
			_T("break_statement"),
			_T("continue_statement"),
			_T("return_statement"),
			_T("switch_block_statement"),
			_T("attribute_block"),
			_T("attribute_declarator_list"),
			_T("attribute_declarator"),
			_T("declaration_specifier_list"),
			_T("init_declarator_list"),
			_T("declaration_terminator"),
			_T("init_declarator"),
			_T("declarator"),
			_T("initializer"),
			_T("initializer_list"),
			_T("initializer_list_entry"),
			_T("statement_1st_pass"),
			_T("declaration_specifier"),
			_T("type_specifier_modifier"),
			_T("access_specifier"),
			_T("storage_class_specifier"),
			_T("type_specifier"),
			_T("type_modifier"),
			_T("enum_specifier"),
			_T("struct_specifier"),
			_T("class_specifier"),
			_T("property_specifier"),
			_T("qualified_type_identifier_rslv"),
			_T("qualified_type_identifier"),
			_T("property_accessor_kind"),
			_T("pointer_kind"),
			_T("array_suffix"),
			_T("formal_argument_suffix"),
			_T("formal_argument_list"),
			_T("formal_argument_vararg"),
			_T("formal_argument"),
			_T("enum_kind"),
			_T("enum_block"),
			_T("enum_member_list"),
			_T("enum_member"),
			_T("struct_kind"),
			_T("inheritance_list"),
			_T("struct_block"),
			_T("struct_member_declaration"),
			_T("struct_member_declarator_list"),
			_T("struct_member_declarator"),
			_T("struct_member_bit_field"),
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
		
		
		
		
		case ESymbol_using_declaration:
			pNode = AXL_MEM_NEW (CSymbolNode_using_declaration);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		
		
		
		case ESymbol_statement:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			break;
			
		
		
		
		case ESymbol_qualified_identifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls1);
				
			break;
			
		
		case ESymbol_type_name:
			pNode = AXL_MEM_NEW (CSymbolNode_type_name);
			
			pNode->m_pAst = AXL_MEM_NEW (_cls2);
			
			break;
			
		
		case ESymbol_type_specifier_modifier_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls6);
				
			break;
			
		
		case ESymbol_pointer:
			pNode = AXL_MEM_NEW (CSymbolNode_pointer);
			
			break;
			
		
		case ESymbol_declarator_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_declarator_suffix);
			
			break;
			
		
		
		
		
		
		case ESymbol_constant_integer_expression:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls4);
				
			break;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_attribute_block:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
				
			pNode->m_pAst = AXL_MEM_NEW (CAst);
				
			break;
			
		
		
		
		case ESymbol_declaration_specifier_list:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls5);
				
			break;
			
		
		case ESymbol_init_declarator_list:
			pNode = AXL_MEM_NEW (CSymbolNode_init_declarator_list);
			
			break;
			
		
		case ESymbol_declaration_terminator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			break;
			
		
		case ESymbol_init_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_init_declarator);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_declarator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls7);
				
			break;
			
		
		
		
		
		case ESymbol_statement_1st_pass:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			break;
			
		
		case ESymbol_declaration_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_declaration_specifier);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_type_specifier_modifier:
			pNode = AXL_MEM_NEW (CSymbolNode_type_specifier_modifier);
			
			break;
			
		
		case ESymbol_access_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_access_specifier);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_storage_class_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_storage_class_specifier);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_type_specifier:
			pNode = AXL_MEM_NEW (CSymbolNode_type_specifier);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_type_modifier:
			pNode = AXL_MEM_NEW (CSymbolNode_type_modifier);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_enum_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_HasLeave;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls10);
				
			break;
			
		
		case ESymbol_struct_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls12);
				
			break;
			
		
		case ESymbol_class_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls14);
				
			break;
			
		
		case ESymbol_property_specifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls16);
				
			break;
			
		
		
		case ESymbol_qualified_type_identifier:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls9);
				
			break;
			
		
		case ESymbol_property_accessor_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls19);
				
			break;
			
		
		case ESymbol_pointer_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls8);
				
			break;
			
		
		case ESymbol_array_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_array_suffix);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_formal_argument_suffix:
			pNode = AXL_MEM_NEW (CSymbolNode_formal_argument_suffix);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_formal_argument_list:
			pNode = AXL_MEM_NEW (CSymbolNode_formal_argument_list);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_formal_argument_vararg:
			pNode = AXL_MEM_NEW (CSymbolNode_formal_argument_vararg);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_formal_argument:
			pNode = AXL_MEM_NEW (CSymbolNode_formal_argument);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_enum_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls11);
				
			break;
			
		
		case ESymbol_enum_block:
			pNode = AXL_MEM_NEW (CSymbolNode_enum_block);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_enum_member_list:
			pNode = AXL_MEM_NEW (CSymbolNode_enum_member_list);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_enum_member:
			pNode = AXL_MEM_NEW (CSymbolNode_enum_member);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_struct_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls13);
				
			break;
			
		
		
		case ESymbol_struct_block:
			pNode = AXL_MEM_NEW (CSymbolNode_struct_block);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_struct_member_declaration:
			pNode = AXL_MEM_NEW (CSymbolNode_struct_member_declaration);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_struct_member_declarator_list:
			pNode = AXL_MEM_NEW (CSymbolNode_struct_member_declarator_list);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_struct_member_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_struct_member_declarator);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		
		case ESymbol_class_kind:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls15);
				
			break;
			
		
		case ESymbol_class_block:
			pNode = AXL_MEM_NEW (CSymbolNode_class_block);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_class_member_declaration:
			pNode = AXL_MEM_NEW (CSymbolNode_class_member_declaration);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_class_member_declarator_list:
			pNode = AXL_MEM_NEW (CSymbolNode_class_member_declarator_list);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_class_member_declarator:
			pNode = AXL_MEM_NEW (CSymbolNode_class_member_declarator);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_property_block:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (CPropertyBlock);
				
			break;
			
		
		case ESymbol_property_accessor_declaration:
			pNode = AXL_MEM_NEW (CSymbolNode_property_accessor_declaration);
			
			pNode->m_pAst = AXL_MEM_NEW (CAst);
			
			break;
			
		
		case ESymbol_property_declarator:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls18);
				
			break;
			
		
		default:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_pAst = AXL_MEM_NEW (CAst);
		};

		if (pNode->m_pAst)
			pNode->m_pAst->m_Kind = (int) Index;

		return pNode;
	}

	static
	size_t*
	GetBeacon (size_t Index)
	{
		static
		size_t _BeaconTable [] [2] = 
		{
			
			{ 0, 30 },
			
			{ 1, 30 },
			
			{ 0, 30 },
			
			{ 0, 64 },
			
			{ 1, 110 },
			
			{ 0, 157 },
			
			{ 0, 161 },
			
			{ 0, 172 },
			
			{ 0, 173 },
			
			{ 0, 174 },
			
			{ 0, 175 },
			
			{ 0, 177 },
			
			{ 0, 30 },
			
			{ 1, 178 },
			
			{ 0, 179 },
			
			{ 0, 112 },
			
			{ 1, 161 },
			
			{ 0, 110 },
			
			{ 0, 110 },
			
			{ 1, 185 },
			
			{ 0, 30 },
			
			{ 0, 185 },
			
			{ 0, 30 },
			
			{ 1, 189 },
			
			{ 0, 30 },
			
			{ 0, 189 },
			
			{ 0, 112 },
			
			{ 0, 161 },
			
			{ 1, 196 },
			
			{ 0, 30 },
			
			{ 0, 196 },
			
			{ 0, 157 },
			
			{ 0, 161 },
			
			{ 0, 201 },
			
			{ 0, 112 },
			
			{ 1, 203 },
			
			{ 0, 178 },
			
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
			_cls1* __pAst = (_cls1*) __pSymbol->m_pAst;
#line 37 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Decl.llk"
			  
			(*__pAst).m_Identifier.m_First = (*GetTokenLocator (0)).m_Data.m_String; 
		;
#line 2660 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls1* __pAst = (_cls1*) __pSymbol->m_pAst;
#line 41 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Decl.llk"
			 
			(*__pAst).m_Identifier.m_List.InsertTail ((*GetTokenLocator (1)).m_Data.m_String);
		;
#line 2673 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAst = (_cls4*) __pSymbol->m_pAst;
#line 38 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Expr.llk"
			
			(*__pAst).m_Value = 10;
		;
#line 2686 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 17 "D:/Projects.v/axl3/axl_jnc/axl_jnc_AttributeBlock.llk"
			
			CAttribute* pAttribute = m_pModule->m_AttributeMgr.CreateAttribute ((*GetTokenLocator (0)).m_Data.m_String, NULL);
			if (!pAttribute)
				return false;

			pAttribute->m_Pos = (*GetTokenLocator (0)).m_Pos;
		;
#line 2703 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 4:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 5 "D:/Projects.v/axl3/axl_jnc/axl_jnc_NamespaceDeclaration.llk"
			 
			CNamespace* pNamespace = m_pModule->m_NamespaceMgr.OpenNamespace ((*GetTokenLocator (0)).m_Pos, (*(_cls1*) GetAstLocator (1)).m_Identifier);
			if (!pNamespace)
				return false;
		;
#line 2718 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 5:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 11 "D:/Projects.v/axl3/axl_jnc/axl_jnc_NamespaceDeclaration.llk"
			 
			m_pModule->m_NamespaceMgr.CloseNamespace ((*(_cls1*) GetAstLocator (1)).m_Identifier.m_List.GetCount () + 1);
		;
#line 2731 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 6:
			{
			CSymbolNode_init_declarator* __pSymbol = (CSymbolNode_init_declarator*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 17 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = Declare (__pSymbol->m_Arg.pDeclSpecifiers, &(*(_cls7*) GetAstLocator (0)).m_Declarator);
			if (!Result)
				return false;
		;
#line 2746 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 7:
			{
			CSymbolNode_access_specifier* __pSymbol = (CSymbolNode_access_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 91 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pAccessSpecifier->SetAccess (EAccess_Public);
			if (!Result)
				return false;
		;
#line 2761 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 8:
			{
			CSymbolNode_access_specifier* __pSymbol = (CSymbolNode_access_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 97 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pAccessSpecifier->SetAccess (EAccess_Private);
			if (!Result)
				return false;
		;
#line 2776 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 9:
			{
			CSymbolNode_storage_class_specifier* __pSymbol = (CSymbolNode_storage_class_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 106 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pDeclSpecifiers->SetStorageClass (EStorageClass_Static);
			if (!Result)
				return false;
		;
#line 2791 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 10:
			{
			CSymbolNode_storage_class_specifier* __pSymbol = (CSymbolNode_storage_class_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 112 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pDeclSpecifiers->SetStorageClass (EStorageClass_Typedef);
			if (!Result)
				return false;
		;
#line 2806 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 11:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 120 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Const);
			if (!Result)
				return false;
		;
#line 2821 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 12:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 126 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Property);
			if (!Result)
				return false;
		;
#line 2836 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 13:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 132 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Signed);
			if (!Result)
				return false;
		;
#line 2851 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 14:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 138 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Unsigned);
			if (!Result)
				return false;
		;
#line 2866 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 15:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 144 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_BigEndian);
			if (!Result)
				return false;
		;
#line 2881 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 16:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 150 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_LittleEndian);
			if (!Result)
				return false;
		;
#line 2896 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 17:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 159 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Void));
			if (!Result)
				return false;
		;
#line 2911 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 18:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 165 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Bool));
			if (!Result)
				return false;
		;
#line 2926 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 19:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 171 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int));
			if (!Result)
				return false;
		;
#line 2941 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 20:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 177 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Char));
			if (!Result)
				return false;
		;
#line 2956 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 21:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 183 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Short));
			if (!Result)
				return false;
		;
#line 2971 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 22:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 189 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Long));
			if (!Result)
				return false;
		;
#line 2986 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 23:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 195 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Float));
			if (!Result)
				return false;
		;
#line 3001 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 24:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 201 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Double));
			if (!Result)
				return false;
		;
#line 3016 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 25:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 207 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int8));
			if (!Result)
				return false;
		;
#line 3031 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 26:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 213 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int16));
			if (!Result)
				return false;
		;
#line 3046 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 27:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 219 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int32));
			if (!Result)
				return false;
		;
#line 3061 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 28:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 225 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int64));
			if (!Result)
				return false;
		;
#line 3076 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 29:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 231 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_WChar));
			if (!Result)
				return false;
		;
#line 3091 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 30:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 237 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Variant));
			if (!Result)
				return false;
		;
#line 3106 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 31:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 243 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			err::SetStringError (_T("'auto' type specifier is not yet supported"));
			return false; 
		;
#line 3120 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 32:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 248 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls10*) GetAstLocator (0)).m_pType);
			if (!Result)
				return false;
		;
#line 3135 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 33:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 254 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls12*) GetAstLocator (0)).m_pType);
			if (!Result)
				return false;
		;
#line 3150 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 34:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 260 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls14*) GetAstLocator (0)).m_pType);
			if (!Result)
				return false;
		;
#line 3165 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 35:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 266 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetProperty ((*(_cls16*) GetAstLocator (0)).m_pProperty);
			if (!Result)
				return false;
		;
#line 3180 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 36:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 273 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls9*) GetAstLocator (0)).m_pType);
			if (!Result)
				return false;
		;
#line 3195 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 37:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAst = (_cls7*) __pSymbol->m_pAst;
#line 289 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAst).m_Declarator.m_Pos = (*GetTokenLocator (0)).m_Pos;
			(*__pAst).m_Declarator.m_Name = (*GetTokenLocator (0)).m_Data.m_String;
		;
#line 3209 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 38:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAst = (_cls7*) __pSymbol->m_pAst;
#line 294 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAst).m_Declarator.m_PropertyAccessorKind = (*(_cls19*) GetAstLocator (1)).m_PropertyAccessorKind;
		;
#line 3222 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 39:
			{
			CSymbolNode_pointer* __pSymbol = (CSymbolNode_pointer*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 310 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Local.pPointer = __pSymbol->m_Arg.pDeclarator->AddPointer ((*(_cls8*) GetAstLocator (0)).m_Type);
		;
#line 3235 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 40:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAst = (_cls8*) __pSymbol->m_pAst;
#line 322 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAst).m_Type = EType_Pointer;
		;
#line 3248 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 41:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAst = (_cls8*) __pSymbol->m_pAst;
#line 326 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAst).m_Type = EType_Reference;
		;
#line 3261 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 42:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 341 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix (0);
		;
#line 3274 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 43:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 345 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix (0);
		;
#line 3287 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 44:
			{
			CSymbolNode_formal_argument* __pSymbol = (CSymbolNode_formal_argument*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 373 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			bool Result = DeclareFormalArg (__pSymbol->m_Arg.pArgSuffix, &(*(_cls6*) GetAstLocator (0)).m_TypeSpecifier, &(*(_cls7*) GetAstLocator (1)).m_Declarator, NULL);
			if (!Result)
				return false;
		;
#line 3302 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 45:
			{
			CSymbolNode_formal_argument_vararg* __pSymbol = (CSymbolNode_formal_argument_vararg*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 382 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			__pSymbol->m_Arg.pArgSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_IsVarArg;
		;
#line 3315 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 46:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 391 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			return IsType ((*(_cls1*) GetAstLocator (0)).m_Identifier);
		;
#line 3328 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 47:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAst = (_cls9*) __pSymbol->m_pAst;
#line 402 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			
			(*__pAst).m_pType = FindType ((*(_cls1*) GetAstLocator (0)).m_Identifier);
		;
#line 3341 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 48:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAst = (_cls10*) __pSymbol->m_pAst;
#line 18 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			rtl::CString Name = (*GetTokenLocator (0)).m_Data.m_String;
			rtl::CString QualifiedName = m_pModule->m_NamespaceMgr.CreateQualifiedName (Name);
			(*__pAst).m_pType = m_pModule->m_TypeMgr.GetEnumType ((*(_cls11*) GetAstLocator (1)).m_Type, Name, QualifiedName);
			
			bool Result = Declare ((*__pAst).m_pType, (*(_cls11*) GetAstLocator (1)).m_FirstToken.m_Pos);
			if (!Result)
				return false;
				
		;
#line 3361 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 49:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAst = (_cls10*) __pSymbol->m_pAst;
#line 30 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			(*__pAst).m_pType = m_pModule->m_TypeMgr.CreateUnnamedEnumType ((*(_cls11*) GetAstLocator (0)).m_Type);
			Declare ((*__pAst).m_pType, (*(_cls11*) GetAstLocator (0)).m_FirstToken.m_Pos);
		;
#line 3375 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 50:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAst = (_cls11*) __pSymbol->m_pAst;
#line 43 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			(*__pAst).m_Type = EType_Enum;
		;
#line 3388 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 51:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAst = (_cls11*) __pSymbol->m_pAst;
#line 47 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			(*__pAst).m_Type = EType_EnumC;
		;
#line 3401 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 52:
			{
			CSymbolNode_enum_member* __pSymbol = (CSymbolNode_enum_member*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 62 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			
			CEnumMember* pMember = __pSymbol->m_Arg.pType->CreateMember ((*GetTokenLocator (0)).m_Data.m_String);
			if (!pMember)
				return false;

			pMember->m_Pos = (*GetTokenLocator (0)).m_Pos;
		;
#line 3418 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 53:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAst = (_cls12*) __pSymbol->m_pAst;
#line 9 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			rtl::CString Name = (*GetTokenLocator (0)).m_Data.m_String;
			rtl::CString QualifiedName = m_pModule->m_NamespaceMgr.CreateQualifiedName (Name);
			(*__pAst).m_pType = m_pModule->m_TypeMgr.GetStructType ((*(_cls13*) GetAstLocator (1)).m_Type, Name, QualifiedName);
			
			bool Result = Declare ((*__pAst).m_pType, (*(_cls13*) GetAstLocator (1)).m_FirstToken.m_Pos);
			if (!Result)
				return false;
		;
#line 3437 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 54:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAst = (_cls12*) __pSymbol->m_pAst;
#line 20 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAst).m_pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ((*(_cls13*) GetAstLocator (0)).m_Type);
			Declare ((*__pAst).m_pType, (*(_cls13*) GetAstLocator (0)).m_FirstToken.m_Pos);
		;
#line 3451 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 55:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAst = (_cls13*) __pSymbol->m_pAst;
#line 34 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAst).m_Type = EType_Struct;
		;
#line 3464 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 56:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAst = (_cls13*) __pSymbol->m_pAst;
#line 38 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			(*__pAst).m_Type = EType_Union;
		;
#line 3477 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 57:
			{
			CSymbolNode_struct_member_declarator* __pSymbol = (CSymbolNode_struct_member_declarator*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 62 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			
			bool Result = DeclareStructMember (__pSymbol->m_Arg.pType, __pSymbol->m_Arg.pTypeSpecifier, &(*(_cls7*) GetAstLocator (0)).m_Declarator);
			if (!Result)
				return false;
		;
#line 3492 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 58:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAst = (_cls14*) __pSymbol->m_pAst;
#line 9 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			rtl::CString Name = (*GetTokenLocator (0)).m_Data.m_String;
			rtl::CString QualifiedName = m_pModule->m_NamespaceMgr.CreateQualifiedName (Name);
			(*__pAst).m_pType = m_pModule->m_TypeMgr.GetClassType ((*(_cls15*) GetAstLocator (1)).m_Type, Name, QualifiedName);
			
			bool Result = Declare ((*__pAst).m_pType, (*(_cls15*) GetAstLocator (1)).m_FirstToken.m_Pos);
			if (!Result)
				return false;
		;
#line 3511 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 59:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAst = (_cls14*) __pSymbol->m_pAst;
#line 20 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			(*__pAst).m_pType = m_pModule->m_TypeMgr.CreateUnnamedClassType ((*(_cls15*) GetAstLocator (0)).m_Type);
			Declare ((*__pAst).m_pType, (*(_cls15*) GetAstLocator (0)).m_FirstToken.m_Pos);
		;
#line 3525 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 60:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAst = (_cls15*) __pSymbol->m_pAst;
#line 34 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			(*__pAst).m_Type = EType_Interface;
		;
#line 3538 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 61:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAst = (_cls15*) __pSymbol->m_pAst;
#line 38 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			(*__pAst).m_Type = EType_Class;
		;
#line 3551 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 62:
			{
			CSymbolNode_class_member_declarator* __pSymbol = (CSymbolNode_class_member_declarator*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 66 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			
			bool Result = DeclareClassMember (__pSymbol->m_Arg.pType, __pSymbol->m_Arg.pDeclSpecifiers, &(*(_cls7*) GetAstLocator (0)).m_Declarator);
			if (!Result)
				return false;
		;
#line 3566 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 63:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAst = (_cls16*) __pSymbol->m_pAst;
#line 9 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAst).m_pProperty = CreateProperty ((*(CPropertyBlock*) GetAstLocator (0)).m_pGetter, (*(CPropertyBlock*) GetAstLocator (0)).m_Setter);
		;
#line 3579 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 64:
			{
			CSymbolNode_property_accessor_declaration* __pSymbol = (CSymbolNode_property_accessor_declaration*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
#line 32 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			bool Result = DeclarePropertyAccessor (__pSymbol->m_Arg.pBlock, &(*(_cls6*) GetAstLocator (0)).m_TypeSpecifier, &(*(_cls18*) GetAstLocator (1)).m_Declarator);
			if (!Result)
				return false;
		;
#line 3594 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 65:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAst = (_cls18*) __pSymbol->m_pAst;
#line 48 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAst).m_Declarator.m_PropertyAccessorKind = (*(_cls19*) GetAstLocator (0)).m_PropertyAccessorKind;
			(*__pAst).m_Declarator.m_Pos = (*(_cls19*) GetAstLocator (0)).m_FirstToken.m_Pos;
		;
#line 3608 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 66:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAst = (_cls19*) __pSymbol->m_pAst;
#line 60 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAst).m_PropertyAccessorKind = EPropertyAccessor_Get;
		;
#line 3621 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}

			return true;
		
		case 67:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAst = (_cls19*) __pSymbol->m_pAst;
#line 64 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			
			(*__pAst).m_PropertyAccessorKind = EPropertyAccessor_Set;
		;
#line 3634 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
			_cls2* __pAst = (_cls2*) __pSymbol->m_pAst;
			
#line 59 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
#line 3662 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAst = (_cls2*) __pSymbol->m_pAst;
			
#line 60 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Decl.llk"
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
#line 3675 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_init_declarator_list* __pTarget = (CSymbolNode_init_declarator_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 6 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = &(*(_cls5*) GetAstLocator (0)).m_DeclSpecifiers;
#line 3688 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_init_declarator* __pTarget = (CSymbolNode_init_declarator*) pSymbol;
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 12 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 3701 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_init_declarator* __pTarget = (CSymbolNode_init_declarator*) pSymbol;
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 12 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 3714 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_declaration_specifier* __pTarget = (CSymbolNode_declaration_specifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAst = (_cls5*) __pSymbol->m_pAst;
			
#line 66 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = &(*__pAst).m_DeclSpecifiers;
#line 3727 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAst = (_cls6*) __pSymbol->m_pAst;
			
#line 74 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAst).m_TypeSpecifier;
#line 3740 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 78 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pDeclSpecifiers;
#line 3753 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_access_specifier* __pTarget = (CSymbolNode_access_specifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 79 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pAccessSpecifier = __pSymbol->m_Arg.pDeclSpecifiers;
#line 3766 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_storage_class_specifier* __pTarget = (CSymbolNode_storage_class_specifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 80 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 3779 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_type_specifier* __pTarget = (CSymbolNode_type_specifier*) pSymbol;
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 85 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
#line 3792 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_type_modifier* __pTarget = (CSymbolNode_type_modifier*) pSymbol;
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 86 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeModifiers = __pSymbol->m_Arg.pTypeSpecifier;
#line 3805 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAst = (_cls7*) __pSymbol->m_pAst;
			
#line 287 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAst).m_Declarator;
#line 3818 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAst = (_cls7*) __pSymbol->m_pAst;
			
#line 298 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAst).m_Declarator;
#line 3831 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_type_modifier* __pTarget = (CSymbolNode_type_modifier*) pSymbol;
			CSymbolNode_pointer* __pSymbol = (CSymbolNode_pointer*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 313 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pTypeModifiers = __pSymbol->m_Local.pPointer;
#line 3844 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_array_suffix* __pTarget = (CSymbolNode_array_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 335 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 3857 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_formal_argument_suffix* __pTarget = (CSymbolNode_formal_argument_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 336 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
#line 3870 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_formal_argument_list* __pTarget = (CSymbolNode_formal_argument_list*) pSymbol;
			CSymbolNode_formal_argument_suffix* __pSymbol = (CSymbolNode_formal_argument_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 359 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pArgSuffix = __pSymbol->m_Local.pArgSuffix;
#line 3883 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_formal_argument_vararg* __pTarget = (CSymbolNode_formal_argument_vararg*) pSymbol;
			CSymbolNode_formal_argument_suffix* __pSymbol = (CSymbolNode_formal_argument_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 360 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pArgSuffix = __pSymbol->m_Local.pArgSuffix;
#line 3896 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_formal_argument* __pTarget = (CSymbolNode_formal_argument*) pSymbol;
			CSymbolNode_formal_argument_list* __pSymbol = (CSymbolNode_formal_argument_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 365 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pArgSuffix = __pSymbol->m_Arg.pArgSuffix;
#line 3909 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_formal_argument* __pTarget = (CSymbolNode_formal_argument*) pSymbol;
			CSymbolNode_formal_argument_list* __pSymbol = (CSymbolNode_formal_argument_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 366 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pArgSuffix = __pSymbol->m_Arg.pArgSuffix;
#line 3922 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_formal_argument_vararg* __pTarget = (CSymbolNode_formal_argument_vararg*) pSymbol;
			CSymbolNode_formal_argument_list* __pSymbol = (CSymbolNode_formal_argument_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 367 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
			__pTarget->m_Arg.pArgSuffix = __pSymbol->m_Arg.pArgSuffix;
#line 3935 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAst = (_cls10*) __pSymbol->m_pAst;
			
#line 28 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
#line 3948 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAst = (_cls10*) __pSymbol->m_pAst;
			
#line 34 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
#line 3961 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_enum_member_list* __pTarget = (CSymbolNode_enum_member_list*) pSymbol;
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 53 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 3974 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 57 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 3987 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 57 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 4000 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_struct_block* __pTarget = (CSymbolNode_struct_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAst = (_cls12*) __pSymbol->m_pAst;
			
#line 18 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
#line 4013 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_struct_block* __pTarget = (CSymbolNode_struct_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAst = (_cls12*) __pSymbol->m_pAst;
			
#line 25 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
#line 4026 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_struct_member_declaration* __pTarget = (CSymbolNode_struct_member_declaration*) pSymbol;
			CSymbolNode_struct_block* __pSymbol = (CSymbolNode_struct_block*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 44 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 4039 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_struct_member_declarator_list* __pTarget = (CSymbolNode_struct_member_declarator_list*) pSymbol;
			CSymbolNode_struct_member_declaration* __pSymbol = (CSymbolNode_struct_member_declaration*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 50 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls6*) GetAstLocator (0)).m_TypeSpecifier;
#line 4052 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 50 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 4056 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_struct_member_declarator* __pTarget = (CSymbolNode_struct_member_declarator*) pSymbol;
			CSymbolNode_struct_member_declarator_list* __pSymbol = (CSymbolNode_struct_member_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 57 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
#line 4069 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 57 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 4073 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_struct_member_declarator* __pTarget = (CSymbolNode_struct_member_declarator*) pSymbol;
			CSymbolNode_struct_member_declarator_list* __pSymbol = (CSymbolNode_struct_member_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 57 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
#line 4086 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 57 "D:/Projects.v/axl3/axl_jnc/axl_jnc_StructSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 4090 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_class_block* __pTarget = (CSymbolNode_class_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAst = (_cls14*) __pSymbol->m_pAst;
			
#line 18 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
#line 4103 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_class_block* __pTarget = (CSymbolNode_class_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAst = (_cls14*) __pSymbol->m_pAst;
			
#line 25 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
#line 4116 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_class_member_declaration* __pTarget = (CSymbolNode_class_member_declaration*) pSymbol;
			CSymbolNode_class_block* __pSymbol = (CSymbolNode_class_block*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 48 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
#line 4129 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_class_member_declarator_list* __pTarget = (CSymbolNode_class_member_declarator_list*) pSymbol;
			CSymbolNode_class_member_declaration* __pSymbol = (CSymbolNode_class_member_declaration*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 54 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pDeclSpecifiers = &(*(_cls5*) GetAstLocator (0)).m_DeclSpecifiers;
#line 4142 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 54 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 4146 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_class_member_declarator* __pTarget = (CSymbolNode_class_member_declarator*) pSymbol;
			CSymbolNode_class_member_declarator_list* __pSymbol = (CSymbolNode_class_member_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 61 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 4159 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 61 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 4163 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_class_member_declarator* __pTarget = (CSymbolNode_class_member_declarator*) pSymbol;
			CSymbolNode_class_member_declarator_list* __pSymbol = (CSymbolNode_class_member_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
#line 61 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
#line 4176 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
#line 61 "D:/Projects.v/axl3/axl_jnc/axl_jnc_ClassSpecifier.llk"
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
#line 4180 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_property_accessor_declaration* __pTarget = (CSymbolNode_property_accessor_declaration*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CPropertyBlock* __pAst = (CPropertyBlock*) __pSymbol->m_pAst;
			
#line 25 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			__pTarget->m_Arg.pBlock = &(*__pAst);
#line 4193 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAst = (_cls18*) __pSymbol->m_pAst;
			
#line 45 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAst).m_Declarator;
#line 4206 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
			}
			break;
		
		case 41:
			{
			CSymbolNode_formal_argument_suffix* __pTarget = (CSymbolNode_formal_argument_suffix*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAst = (_cls18*) __pSymbol->m_pAst;
			
#line 47 "D:/Projects.v/axl3/axl_jnc/axl_jnc_PropertySpecifier.llk"
			__pTarget->m_Arg.pDeclarator = &(*__pAst).m_Declarator;
#line 4219 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			
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
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_attribute_block:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				CAst* __pAst = __pSymbol->m_pAst;
#line 5 "D:/Projects.v/axl3/axl_jnc/axl_jnc_AttributeBlock.llk"
				
		m_pModule->m_AttributeMgr.CreateAttributeSet ();
	;
#line 4297 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_formal_argument_suffix:
			{
				CSymbolNode_formal_argument_suffix* __pSymbol = (CSymbolNode_formal_argument_suffix*) GetSymbolTop ();
				CAst* __pAst = __pSymbol->m_pAst;
#line 356 "D:/Projects.v/axl3/axl_jnc/axl_jnc_CommonDeclaration.llk"
				
		__pSymbol->m_Local.pArgSuffix = __pSymbol->m_Arg.pDeclarator->AddFormalArgSuffix ();
	;
#line 4336 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_enum_specifier:
			{
				CSymbolNode* __pSymbol = GetSymbolTop ();
				_cls10* __pAst = (_cls10*) __pSymbol->m_pAst;
#line 9 "D:/Projects.v/axl3/axl_jnc/axl_jnc_EnumSpecifier.llk"
				
		if ((*__pAst).m_pType->GetTypeKind () == EType_EnumC)
		{
			bool Result = m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->ExposeEnumMembers ((*__pAst).m_pType);
			if (!Result)
				return false;
		}
	;
#line 4459 "D:/Projects.v/axl3/axl_jnc/axl_jnc_Parser.h"
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
					
				pTransition->m_ProductionIndex = 104;
				pTransition->m_ResolverIndex = 176;
				pTransition->m_ResolverElseIndex = 107;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 619;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 325;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 323;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 3:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 621;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 4:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 368;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 369;
				return ELaDfaResult_Production;
					
			}
			
		
		case 5:
			
			switch (LookaheadToken)
			{
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 623;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 6:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 423;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 7:
			
			switch (LookaheadToken)
			{
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 625;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 8:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 425;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 423;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 9:
			
			switch (LookaheadToken)
			{
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 627;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 10:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 435;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 435;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 433;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 11:
			
			switch (LookaheadToken)
			{
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 629;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 12:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 435;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 435;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 433;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 13:
			
			switch (LookaheadToken)
			{
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 631;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 14:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 448;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 448;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 446;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 15:
			
			switch (LookaheadToken)
			{
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 633;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 16:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 448;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 448;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 446;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 17:
			
			switch (LookaheadToken)
			{
			
			case '[':
					
				pTransition->m_ProductionIndex = 635;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 18:
			
			switch (LookaheadToken)
			{
			
			case '\01':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case ';':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '#':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '.':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '}':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '[':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case ']':
					
				pTransition->m_ProductionIndex = 411;
				return ELaDfaResult_Production;
					
			
			case '(':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '?':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case ':':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '|':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '^':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '<':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '>':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '/':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '%':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case '=':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case ',':
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Pack:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Default:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_LogicalOr:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_LogicalAnd:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Eq:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Ne:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Le:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Ge:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Shl:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Shr:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_AddAssign:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_MulAssign:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_DivAssign:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_ModAssign:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_ShlAssign:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_ShrAssign:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_AndAssign:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_XorAssign:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_OrAssign:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_If:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Else:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Switch:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Case:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_While:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Do:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_For:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Break:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Continue:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Return:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Namespace:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Using:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Public:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Private:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Static:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Typedef:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Get:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			case EToken_Set:
					
				pTransition->m_ProductionIndex = 410;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 19:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 637;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 20:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 414;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 413;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 412;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 21:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 362;
				pTransition->m_ResolverIndex = 176;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 22:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 364;
				pTransition->m_ResolverIndex = 176;
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
