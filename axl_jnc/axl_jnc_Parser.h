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
	ESymbol_constant_integer_expression = 9, 
	ESymbol_qualified_identifier = 10, 
	ESymbol_type_name = 11, 
	ESymbol_type_specifier_modifier_list = 12, 
	ESymbol_pointer = 13, 
	ESymbol_declarator_suffix = 14, 
	ESymbol_expression = 15, 
	ESymbol_logical_or_expression = 16, 
	ESymbol_constant_expression = 17, 
	ESymbol_logical_and_expression = 18, 
	ESymbol_inclusive_or_expression = 19, 
	ESymbol_exclusive_or_expression = 20, 
	ESymbol_and_expression = 21, 
	ESymbol_equality_expression = 22, 
	ESymbol_relational_expression = 23, 
	ESymbol_equality_operator = 24, 
	ESymbol_shift_expression = 25, 
	ESymbol_relational_operator = 26, 
	ESymbol_additive_expression = 27, 
	ESymbol_shift_operator = 28, 
	ESymbol_multiplicative_expression = 29, 
	ESymbol_additivie_operator = 30, 
	ESymbol_unary_expression = 31, 
	ESymbol_multiplicative_operator = 32, 
	ESymbol_unary_operator = 33, 
	ESymbol_postfix_expression = 34, 
	ESymbol_assignment_operator = 35, 
	ESymbol_primary_expression = 36, 
	ESymbol_postfix_operator = 37, 
	ESymbol_member_operator = 38, 
	ESymbol_index_operator = 39, 
	ESymbol_call_operator = 40, 
	ESymbol_compound_statement = 41, 
	ESymbol_expression_statement = 42, 
	ESymbol_if_statement = 43, 
	ESymbol_switch_statement = 44, 
	ESymbol_while_statement = 45, 
	ESymbol_do_statement = 46, 
	ESymbol_for_statement = 47, 
	ESymbol_break_statement = 48, 
	ESymbol_continue_statement = 49, 
	ESymbol_return_statement = 50, 
	ESymbol_switch_block_statement = 51, 
	ESymbol_attribute_block = 52, 
	ESymbol_attribute_declarator_list = 53, 
	ESymbol_attribute_declarator = 54, 
	ESymbol_declaration_specifier_list = 55, 
	ESymbol_init_declarator_list = 56, 
	ESymbol_declaration_terminator = 57, 
	ESymbol_init_declarator = 58, 
	ESymbol_declarator = 59, 
	ESymbol_initializer = 60, 
	ESymbol_initializer_list = 61, 
	ESymbol_statement_1st_pass = 62, 
	ESymbol_declaration_specifier = 63, 
	ESymbol_type_specifier_modifier = 64, 
	ESymbol_access_specifier = 65, 
	ESymbol_storage_class_specifier = 66, 
	ESymbol_type_specifier = 67, 
	ESymbol_type_modifier = 68, 
	ESymbol_enum_specifier = 69, 
	ESymbol_struct_specifier = 70, 
	ESymbol_class_specifier = 71, 
	ESymbol_property_specifier = 72, 
	ESymbol_qualified_type_identifier_rslv = 73, 
	ESymbol_qualified_type_identifier = 74, 
	ESymbol_property_accessor_kind = 75, 
	ESymbol_pointer_kind = 76, 
	ESymbol_array_suffix = 77, 
	ESymbol_formal_argument_suffix = 78, 
	ESymbol_formal_argument_list = 79, 
	ESymbol_formal_argument_vararg = 80, 
	ESymbol_formal_argument = 81, 
	ESymbol_enum_kind = 82, 
	ESymbol_enum_block = 83, 
	ESymbol_enum_member_list = 84, 
	ESymbol_enum_member = 85, 
	ESymbol_struct_kind = 86, 
	ESymbol_inheritance_list = 87, 
	ESymbol_struct_block = 88, 
	ESymbol_struct_member_declaration = 89, 
	ESymbol_struct_member_declarator_list = 90, 
	ESymbol_struct_member_declarator = 91, 
	ESymbol_struct_member_bit_field = 92, 
	ESymbol_class_kind = 93, 
	ESymbol_class_block = 94, 
	ESymbol_class_member_declaration = 95, 
	ESymbol_class_member_declarator_list = 96, 
	ESymbol_class_member_declarator = 97, 
	ESymbol_property_block = 98, 
	ESymbol_property_accessor_declaration = 99, 
	ESymbol_property_declarator = 100, 
	
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
		StartPragmaSymbol  = 171,
		EofToken           = 0,
		AnyToken           = 1,

		TokenCount         = 100,
		SymbolCount        = 172,
		SequenceCount      = 186,
		ActionCount        = 70,
		ArgumentCount      = 42,
		BeaconCount        = 40,
		LaDfaCount         = 23,

		TotalCount         = 633,

		NamedSymbolCount   = 101,

		TokenFirst         = 0,
		TokenEnd           = 100,
		SymbolFirst        = 100,
		NamedSymbolEnd     = 201,
		SymbolEnd          = 272,
		SequenceFirst      = 272,
		SequenceEnd        = 458,
		ActionFirst        = 458,
		ActionEnd          = 528,
		ArgumentFirst      = 528,
		ArgumentEnd        = 570,
		BeaconFirst        = 570,
		BeaconEnd          = 610,
		LaDfaFirst         = 610,
		LaDfaEnd           = 633,
	};

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	class _cls1: public CAst
	{
	public:
		
	CQualifiedIdentifier m_Identifier;
;
	};
	
	class _cls2: public CAst
	{
	public:
		
	CType* m_pType;
;
	};
	
	class _cls3: public CAst
	{
	public:
		
	CValue* m_pValue;
;
	};
	
	class _cls4: public CAst
	{
	public:
		
	intptr_t m_Value;
;
	};
	
	class _cls5: public CAst
	{
	public:
		
	CDeclSpecifiers m_DeclSpecifiers;
;
	};
	
	class _cls6: public CAst
	{
	public:
		
	CTypeSpecifierModifiers m_TypeSpecifier;
;
	};
	
	class _cls7: public CAst
	{
	public:
		
	CDeclarator m_Declarator;
;
	};
	
	class _cls8: public CAst
	{
	public:
		
	EType m_Type;
;
	};
	
	class _cls9: public CAst
	{
	public:
		
	CType* m_pType;
;
	};
	
	class _cls10: public CAst
	{
	public:
		
	CEnumType* m_pType;
;
	};
	
	class _cls11: public CAst
	{
	public:
		
	EType m_Type;
;
	};
	
	class _cls12: public CAst
	{
	public:
		
	CStructType* m_pType;
;
	};
	
	class _cls13: public CAst
	{
	public:
		
	EType m_Type;
;
	};
	
	class _cls14: public CAst
	{
	public:
		
	CClassType* m_pType;
;
	};
	
	class _cls15: public CAst
	{
	public:
		
	EType m_Type;
;
	};
	
	class _cls16: public CAst
	{
	public:
		
	CProperty* m_pProperty;
;
	};
	
	class CPropertyBlock: public CAst
	{
	public:
		
	CFunction* m_pGetter;
	CFunctionOverload m_Setter;

	CPropertyBlock ()
	{
		m_pGetter = NULL;
	}
;
	};
	
	class _cls18: public CAst
	{
	public:
		
	CDeclarator m_Declarator;
;
	};
	
	class _cls19: public CAst
	{
	public:
		
	EPropertyAccessor m_PropertyAccessorKind;
;
	};
	

	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

	
	
	
	
	class CSymbolNode_using_declaration: public CSymbolNode
	{
	public:
			
		struct
		{
			
		CQualifiedIdentifier Identifier;
	;
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
				
		CDeclarator Declarator;
	;
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
			CDeclarator* pDeclarator;
		} m_Arg;	
			
		struct
		{
				
		CDeclPointer* pPointer;
	;
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
			CDeclarator* pDeclarator;
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
			CDeclSpecifiers* pDeclSpecifiers;
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
			CDeclSpecifiers* pDeclSpecifiers;
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
			CDeclSpecifiers* pDeclSpecifiers;
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
			CTypeSpecifierModifiers* pTypeSpecifier;
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
			CAccessSpecifier* pAccessSpecifier;
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
			CDeclSpecifiers* pDeclSpecifiers;
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
			CTypeSpecifier* pTypeSpecifier;
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
			CTypeModifiers* pTypeModifiers;
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
			CDeclarator* pDeclarator;
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
			CDeclarator* pDeclarator;
		} m_Arg;	
			
		struct
		{
			
		CDeclFormalArgSuffix* pArgSuffix;
	;
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
			CDeclFormalArgSuffix* pArgSuffix;
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
			CDeclFormalArgSuffix* pArgSuffix;
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
			CDeclFormalArgSuffix* pArgSuffix;
		} m_Arg;	
			
		struct
		{
			
		CValue* pDefaultValue;
	;
		} m_Local;	
			
		CSymbolNode_formal_argument ()
		{
			m_Flags = axl::llk::ESymbolNodeFlag_IsNamed;
			m_Index = ESymbol_formal_argument;
			
			m_Flags |= axl::llk::ESymbolNodeFlag_HasEnter;
			
		}
	};
		
	
	
	class CSymbolNode_enum_block: public CSymbolNode
	{
	public:
			
		struct
		{
			CEnumType* pType;
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
			CEnumType* pType;
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
			CEnumType* pType;
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
			CStructType* pType;
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
			CStructType* pType;
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
			CTypeSpecifierModifiers* pTypeSpecifier; CStructType* pType;
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
			CTypeSpecifierModifiers* pTypeSpecifier; CStructType* pType;
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
			CClassType* pType;
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
			CClassType* pType;
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
			CDeclSpecifiers* pDeclSpecifiers; CClassType* pType;
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
			CDeclSpecifiers* pDeclSpecifiers; CClassType* pType;
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
			CPropertyBlock* pBlock;
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
			0, -1, 272, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 272, -1, -1, 272, -1, -1, -1, 272, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, -1, 272, 272, 272, 272, 272, 272, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 104, -1, -1, 105, -1, -1, -1, 104, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 102, 103, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, -1, 104, 104, 104, 104, 104, 104, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 335, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 337, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 338, -1, -1, -1, -1, -1, -1, 338, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, 338, -1, 338, 338, 338, 338, 338, 338, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 348, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 107, -1, 107, -1, -1, -1, -1, -1, -1, 107, -1, -1, 107, 107, 107, -1, -1, 107, 107, -1, 104, -1, -1, 107, -1, -1, -1, 610, -1, -1, -1, -1, -1, -1, -1, -1, 107, 107, 107, 107, -1, -1, -1, -1, -1, -1, -1, -1, -1, 107, 107, 107, 107, -1, 107, -1, 107, 107, 107, 107, 107, 107, -1, 103, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, -1, 104, 104, 104, 104, 104, 104, -1, -1, 
			-1, -1, 2, -1, 142, -1, -1, -1, -1, -1, -1, 142, -1, -1, 142, 142, 142, -1, -1, 142, 142, -1, -1, -1, -1, 141, -1, -1, -1, 142, -1, -1, -1, -1, -1, -1, -1, -1, 142, 142, 142, 142, -1, -1, -1, -1, -1, -1, -1, -1, -1, 142, 142, 142, 143, -1, 144, -1, 145, 146, 147, 148, 149, 150, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, 273, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 282, -1, -1, -1, -1, -1, -1, 282, -1, -1, 282, 282, 282, -1, -1, 282, 282, -1, -1, -1, -1, -1, -1, -1, -1, 282, -1, -1, -1, -1, -1, -1, -1, -1, 282, 282, 282, 282, -1, -1, -1, -1, -1, -1, -1, -1, -1, 282, 282, 282, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 274, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 277, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, 277, -1, 277, 277, 277, 277, 277, 277, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 351, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, 351, -1, 351, 351, 351, 351, 351, 351, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 392, -1, -1, -1, -1, 392, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 397, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 396, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 280, -1, -1, -1, -1, -1, -1, 280, -1, -1, 280, 280, 280, -1, -1, 280, 280, -1, -1, -1, -1, -1, -1, -1, -1, 280, -1, -1, -1, -1, -1, -1, -1, -1, 280, 280, 280, 280, -1, -1, -1, -1, -1, -1, -1, -1, -1, 280, 280, 280, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 284, -1, -1, -1, -1, -1, -1, 284, -1, -1, 284, 284, 284, -1, -1, 284, 284, -1, -1, -1, -1, -1, -1, -1, -1, 284, -1, -1, -1, -1, -1, -1, -1, -1, 284, 284, 284, 284, -1, -1, -1, -1, -1, -1, -1, -1, -1, 284, 284, 284, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 281, -1, -1, -1, -1, -1, -1, 281, -1, -1, 281, 281, 281, -1, -1, 281, 281, -1, -1, -1, -1, -1, -1, -1, -1, 281, -1, -1, -1, -1, -1, -1, -1, -1, 281, 281, 281, 281, -1, -1, -1, -1, -1, -1, -1, -1, -1, 281, 281, 281, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 286, -1, -1, -1, -1, -1, -1, 286, -1, -1, 286, 286, 286, -1, -1, 286, 286, -1, -1, -1, -1, -1, -1, -1, -1, 286, -1, -1, -1, -1, -1, -1, -1, -1, 286, 286, 286, 286, -1, -1, -1, -1, -1, -1, -1, -1, -1, 286, 286, 286, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 288, -1, -1, -1, -1, -1, -1, 288, -1, -1, 288, 288, 288, -1, -1, 288, 288, -1, -1, -1, -1, -1, -1, -1, -1, 288, -1, -1, -1, -1, -1, -1, -1, -1, 288, 288, 288, 288, -1, -1, -1, -1, -1, -1, -1, -1, -1, 288, 288, 288, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 290, -1, -1, -1, -1, -1, -1, 290, -1, -1, 290, 290, 290, -1, -1, 290, 290, -1, -1, -1, -1, -1, -1, -1, -1, 290, -1, -1, -1, -1, -1, -1, -1, -1, 290, 290, 290, 290, -1, -1, -1, -1, -1, -1, -1, -1, -1, 290, 290, 290, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 292, -1, -1, -1, -1, -1, -1, 292, -1, -1, 292, 292, 292, -1, -1, 292, 292, -1, -1, -1, -1, -1, -1, -1, -1, 292, -1, -1, -1, -1, -1, -1, -1, -1, 292, 292, 292, 292, -1, -1, -1, -1, -1, -1, -1, -1, -1, 292, 292, 292, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 294, -1, -1, -1, -1, -1, -1, 294, -1, -1, 294, 294, 294, -1, -1, 294, 294, -1, -1, -1, -1, -1, -1, -1, -1, 294, -1, -1, -1, -1, -1, -1, -1, -1, 294, 294, 294, 294, -1, -1, -1, -1, -1, -1, -1, -1, -1, 294, 294, 294, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 296, -1, -1, -1, -1, -1, -1, 296, -1, -1, 296, 296, 296, -1, -1, 296, 296, -1, -1, -1, -1, -1, -1, -1, -1, 296, -1, -1, -1, -1, -1, -1, -1, -1, 296, 296, 296, 296, -1, -1, -1, -1, -1, -1, -1, -1, -1, 296, 296, 296, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, 33, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 298, -1, -1, -1, -1, -1, -1, 298, -1, -1, 298, 298, 298, -1, -1, 298, 298, -1, -1, -1, -1, -1, -1, -1, -1, 298, -1, -1, -1, -1, -1, -1, -1, -1, 298, 298, 298, 298, -1, -1, -1, -1, -1, -1, -1, -1, -1, 298, 298, 298, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 12, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 34, 35, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 300, -1, -1, -1, -1, -1, -1, 300, -1, -1, 300, 300, 300, -1, -1, 300, 300, -1, -1, -1, -1, -1, -1, -1, -1, 300, -1, -1, -1, -1, -1, -1, -1, -1, 300, 300, 300, 300, -1, -1, -1, -1, -1, -1, -1, -1, -1, 300, 300, 300, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 36, 37, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 302, -1, -1, -1, -1, -1, -1, 302, -1, -1, 302, 302, 302, -1, -1, 302, 302, -1, -1, -1, -1, -1, -1, -1, -1, 302, -1, -1, -1, -1, -1, -1, -1, -1, 302, 302, 302, 302, -1, -1, -1, -1, -1, -1, -1, -1, -1, 302, 302, 302, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 304, -1, -1, -1, -1, -1, -1, 304, -1, -1, 304, 304, 304, -1, -1, 304, 304, -1, -1, -1, -1, -1, -1, -1, -1, 304, -1, -1, -1, -1, -1, -1, -1, -1, 304, 304, 304, 304, -1, -1, -1, -1, -1, -1, -1, -1, -1, 304, 304, 304, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, 17, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 306, -1, -1, -1, -1, -1, -1, 11, -1, -1, 14, 15, 16, -1, -1, 19, 20, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 38, 39, 40, 41, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 308, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 308, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 308, 308, 308, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 21, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 42, 43, 44, 45, 46, 47, 48, 49, 50, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 314, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 29, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 51, 52, 53, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 140, -1, 138, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 139, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 38, 39, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, 309, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 310, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 611, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 316, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 317, -1, -1, -1, -1, -1, -1, 317, -1, -1, 317, 317, 317, -1, -1, 317, 317, -1, -1, -1, -1, -1, -1, -1, -1, 317, -1, -1, -1, -1, -1, -1, -1, -1, 317, 317, 317, 317, -1, -1, -1, -1, -1, -1, -1, -1, -1, 317, 317, 317, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 318, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 320, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 324, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 325, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 326, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 327, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 328, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 329, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 107, -1, 107, -1, -1, -1, -1, -1, -1, 107, -1, -1, 107, 107, 107, -1, -1, 107, 107, -1, -1, -1, -1, 107, -1, -1, 323, 107, -1, -1, -1, -1, -1, -1, -1, -1, 107, 107, 107, 107, -1, -1, -1, -1, -1, -1, -1, -1, -1, 107, 107, 107, 107, -1, 107, 322, 107, 107, 107, 107, 107, 107, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 330, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 332, 332, -1, -1, -1, -1, 332, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1, 334, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 349, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, -1, 349, 349, 349, 349, 349, 349, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 340, -1, -1, -1, -1, 340, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 340, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 105, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 343, -1, -1, -1, -1, 343, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 343, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 389, -1, -1, -1, -1, 389, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 389, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 115, -1, -1, -1, -1, -1, -1, 115, -1, -1, 115, 115, 115, -1, -1, 115, 115, -1, -1, -1, -1, 344, -1, -1, -1, 115, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 346, -1, -1, -1, -1, -1, -1, 346, -1, -1, 346, 346, 346, -1, -1, 346, 346, -1, -1, -1, -1, 346, -1, -1, -1, 346, -1, -1, -1, -1, -1, -1, -1, -1, 346, 346, 346, 346, -1, -1, -1, -1, -1, -1, -1, -1, -1, 346, 346, 346, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 105, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 353, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 354, 354, 355, 355, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, 353, -1, 353, 353, 353, 353, 353, 353, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 356, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 357, 613, 357, 357, 357, 357, 356, 356, 356, 356, 356, 356, 356, 356, 356, 356, 356, 356, 356, 356, 356, -1, 356, 356, 356, 356, 356, 356, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 358, 359, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 360, 361, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 240, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 386, -1, -1, -1, -1, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, -1, 383, 383, 384, 384, 385, 385, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 362, 363, 364, 365, 366, 367, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 615, 617, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 619, 621, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 623, 625, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 450, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 409, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 410, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 456, 457, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 395, -1, -1, -1, -1, 394, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 627, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 629, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 403, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, 403, -1, 403, 403, 403, 403, 403, 403, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 408, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 406, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, 406, -1, 406, 406, 406, 406, 406, 406, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 414, 415, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 416, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 417, -1, 417, -1, -1, 417, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, 0, -1, -1, 420, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 424, 425, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 439, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 427, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 428, -1, -1, 105, -1, -1, -1, 428, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, 428, -1, 428, 428, 428, 428, 428, 428, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 429, -1, -1, -1, -1, 429, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 429, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 432, -1, -1, -1, -1, 432, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 432, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 109, -1, -1, -1, -1, -1, -1, 109, -1, -1, 109, 109, 109, -1, -1, 109, 109, -1, -1, -1, -1, -1, -1, -1, -1, 109, -1, -1, -1, -1, -1, -1, -1, -1, 109, 109, 109, 109, -1, -1, -1, -1, -1, -1, -1, -1, -1, 109, 109, 109, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 437, 438, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 442, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 443, -1, -1, 105, -1, -1, -1, 443, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, 443, -1, 443, 443, 443, 443, 443, 443, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 445, -1, -1, -1, -1, 445, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 445, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 448, -1, -1, -1, -1, 448, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 448, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 452, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 453, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, 453, -1, 453, 453, 453, 453, 453, 453, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 455, -1, -1, -1, -1, 455, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 455, 455, 
			0, -1, 272, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 272, -1, -1, 272, -1, -1, -1, 272, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, -1, 272, 272, 272, 272, 272, 272, -1, -1, 
			-1, -1, -1, -1, 109, -1, -1, -1, -1, -1, -1, 109, -1, -1, 109, 109, 109, -1, -1, 109, 109, -1, -1, -1, -1, -1, -1, -1, 28, 109, -1, -1, -1, -1, -1, -1, -1, -1, 109, 109, 109, 109, -1, -1, -1, -1, -1, -1, -1, -1, -1, 109, 109, 109, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, 275, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, 276, -1, -1, -1, -1, 276, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 278, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 278, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 279, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 283, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 0, 285, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 287, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 0, 289, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 0, 0, 291, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 293, 293, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 295, 295, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 295, 295, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 297, 297, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 299, 299, 0, 0, 0, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 301, 301, 301, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 303, -1, -1, -1, -1, -1, -1, 303, -1, -1, 303, 303, 303, -1, -1, 303, 303, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 303, 303, 303, 303, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 305, -1, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 305, 305, 305, 305, 305, 305, 305, 305, 305, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 307, 0, 307, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 307, 0, 0, 0, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 307, 307, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 312, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 315, -1, 315, -1, -1, -1, -1, -1, -1, 315, -1, -1, 315, 315, 315, -1, -1, 315, 315, -1, 315, -1, -1, 315, 0, -1, -1, 315, -1, -1, -1, -1, -1, -1, -1, -1, 315, 315, 315, 315, -1, -1, -1, -1, -1, -1, -1, -1, -1, 315, 315, 315, 315, -1, 315, -1, 315, 315, 315, 315, 315, 315, -1, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, 315, -1, 315, 315, 315, 315, 315, 315, -1, -1, 
			-1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, 0, -1, -1, 0, 0, -1, 0, -1, -1, 0, 0, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 319, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 321, -1, 321, -1, -1, -1, -1, -1, -1, 321, -1, -1, 321, 321, 321, -1, -1, 321, 321, -1, -1, -1, -1, 321, 0, -1, 321, 321, -1, -1, -1, -1, -1, -1, -1, -1, 321, 321, 321, 321, -1, -1, -1, -1, -1, -1, -1, -1, -1, 321, 321, 321, 321, -1, 321, 321, 321, 321, 321, 321, 321, 321, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 115, -1, -1, -1, -1, -1, -1, 115, -1, -1, 115, 115, 115, -1, -1, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, 115, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 115, -1, -1, -1, -1, -1, -1, 115, -1, -1, 115, 115, 115, -1, -1, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, 115, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 115, 0, -1, -1, -1, -1, -1, 115, -1, -1, 115, 115, 115, -1, -1, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, 115, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 115, -1, -1, -1, -1, -1, -1, 115, -1, -1, 115, 115, 115, -1, -1, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, 115, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, 115, 115, 115, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 331, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 333, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 336, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 336, -1, -1, 336, 0, -1, -1, 336, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, 336, -1, 336, 336, 336, 336, 336, 336, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 152, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 339, -1, -1, -1, -1, 339, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 339, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 341, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 342, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 24, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 345, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 0, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 347, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 631, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, -1, 350, 350, 350, 350, 350, 350, -1, -1, 
			-1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 632, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, 352, -1, 352, 352, 352, 352, 352, 352, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 387, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 388, -1, -1, -1, -1, 388, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, 390, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 391, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 391, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 393, 393, 393, 393, 393, 393, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 404, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 405, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 407, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 412, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 418, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 419, -1, -1, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 187, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, -1, -1, 422, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 422, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, 187, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 426, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 426, -1, -1, 426, 0, -1, -1, 426, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, 426, -1, 426, 426, 426, 426, 426, 426, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 152, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 430, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, 431, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 433, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, 187, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, 0, 0, -1, -1, 435, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, 0, -1, -1, 435, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 
			-1, -1, -1, -1, -1, -1, -1, -1, 187, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 440, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 441, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 441, -1, -1, 441, 0, -1, -1, 441, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, 441, -1, 441, 441, 441, 441, 441, 441, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 152, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 444, -1, -1, -1, -1, 444, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, 444, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 446, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, 447, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 449, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, 451, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, 451, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, 451, -1, 451, 451, 451, 451, 451, 451, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 454, -1, -1, -1, -1, 454, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 
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
			201, 101,  -1,
			2, 5, 202, 4, 27, 3,  -1,
			203, 458, 570,  -1,
			203, 459, 571, 6,  -1,
			204, 528, 113,  -1,
			205, 204, 112,  -1,
			205, 529, 114,  -1,
			115, 8, 115, 7,  -1,
			206, 116,  -1,
			460, 115,  -1,
			461, 115,  -1,
			207, 118, 30,  -1,
			207, 118,  -1,
			208, 119, 31,  -1,
			208, 119,  -1,
			209, 120, 9,  -1,
			209, 120,  -1,
			210, 121, 10,  -1,
			210, 121,  -1,
			211, 122, 11,  -1,
			211, 122,  -1,
			212, 123, 124,  -1,
			212, 123,  -1,
			213, 125, 126,  -1,
			213, 125,  -1,
			214, 127, 128,  -1,
			214, 127,  -1,
			215, 129, 130,  -1,
			215, 129,  -1,
			216, 131, 132,  -1,
			216, 131,  -1,
			217, 133,  -1,
			218, 134, 217,  -1,
			115, 135,  -1,
			5, 111, 4,  -1,
			219, 137,  -1,
			219, 136,  -1,
			29, 6,  -1,
			23, 115, 22,  -1,
			5, 220, 115, 4,  -1,
			220, 115, 24,  -1,
			5, 4,  -1,
			5, 115, 4,  -1,
			221, 106,  -1,
			26, 221, 25,  -1,
			2, 115,  -1,
			222, 107, 5, 115, 4, 54,  -1,
			107, 55,  -1,
			26, 223, 25, 5, 115, 4, 56,  -1,
			223, 151,  -1,
			8, 109, 57,  -1,
			8, 28,  -1,
			107, 5, 115, 4, 58,  -1,
			2, 5, 115, 4, 58, 107, 59,  -1,
			107, 5, 226, 2, 225, 2, 224, 4, 60,  -1,
			2, 61,  -1,
			2, 62,  -1,
			2, 227, 63,  -1,
			23, 153, 22,  -1,
			228, 154, 24,  -1,
			228, 154,  -1,
			573, 21,  -1,
			462, 229, 572,  -1,
			464, 26, 230, 25, 463, 575, 574,  -1,
			230, 101,  -1,
			2, 110, 65,  -1,
			157, 232, 576, 231,  -1,
			530, 156,  -1,
			233, 531, 158,  -1,
			233, 532, 158, 24,  -1,
			160, 21,  -1,
			465, 234, 577,  -1,
			26, 235, 161, 25,  -1,
			236, 160, 24,  -1,
			236, 160,  -1,
			237, 162,  -1,
			26, 237, 25,  -1,
			238, 533, 163,  -1,
			238, 533, 163,  -1,
			239, 534, 164,  -1,
			239, 534, 164,  -1,
			535, 164,  -1,
			536, 165,  -1,
			537, 166,  -1,
			538, 167,  -1,
			539, 168,  -1,
			466, 66,  -1,
			467, 67,  -1,
			468, 68,  -1,
			469, 69,  -1,
			470, 70,  -1,
			471, 71,  -1,
			472, 72,  -1,
			473, 73,  -1,
			474, 74,  -1,
			475, 75,  -1,
			476, 76,  -1,
			477, 77,  -1,
			478, 78,  -1,
			479, 79,  -1,
			480, 80,  -1,
			481, 81,  -1,
			482, 82,  -1,
			483, 83,  -1,
			484, 84,  -1,
			485, 85,  -1,
			486, 86,  -1,
			487, 87,  -1,
			488, 88,  -1,
			489, 89,  -1,
			490, 90,  -1,
			491, 578,  -1,
			492, 579,  -1,
			493, 580,  -1,
			494, 581,  -1,
			495, 582,  -1,
			241, 540, 113,  -1,
			243, 242, 496, 583, 241,  -1,
			497, 584, 6,  -1,
			243, 541, 114,  -1,
			244, 498, 585,  -1,
			244, 542, 168,  -1,
			499, 16,  -1,
			500, 11,  -1,
			543, 177,  -1,
			544, 178,  -1,
			501, 23, 586, 22,  -1,
			502, 23, 22,  -1,
			5, 545, 179, 4,  -1,
			5, 546, 180, 4,  -1,
			5, 4,  -1,
			246, 245, 547, 181,  -1,
			245, 548, 181, 24,  -1,
			549, 180, 24,  -1,
			504, 247, 588, 587,  -1,
			503, 117, 21,  -1,
			505, 91,  -1,
			506, 589,  -1,
			507, 590,  -1,
			248, 508, 592, 591,  -1,
			550, 183,  -1,
			551, 183, 509, 593,  -1,
			510, 92,  -1,
			511, 93,  -1,
			26, 552, 184, 25,  -1,
			249, 553, 185,  -1,
			249, 554, 185, 24,  -1,
			595, 21,  -1,
			512, 250, 594,  -1,
			252, 513, 597, 596,  -1,
			555, 188, 251,  -1,
			556, 188, 253, 514, 598,  -1,
			515, 94,  -1,
			516, 95,  -1,
			254, 557, 189,  -1,
			26, 254, 25,  -1,
			2, 558, 190, 599, 255,  -1,
			256, 559, 191,  -1,
			256, 560, 191, 24,  -1,
			192, 8,  -1,
			517, 258, 257, 600,  -1,
			160, 21,  -1,
			260, 518, 602, 601,  -1,
			561, 194, 259,  -1,
			562, 194, 261, 519, 603,  -1,
			520, 96,  -1,
			521, 97,  -1,
			262, 174, 8,  -1,
			262, 174, 24,  -1,
			263, 563, 195,  -1,
			26, 263, 25,  -1,
			157, 265, 604, 264,  -1,
			564, 196,  -1,
			266, 565, 197,  -1,
			266, 566, 197, 24,  -1,
			192, 8,  -1,
			522, 268, 267, 605,  -1,
			160, 21,  -1,
			523, 606, 71,  -1,
			269, 567, 199,  -1,
			26, 269, 567, 199, 25,  -1,
			524, 157, 608, 607,  -1,
			270, 568, 113,  -1,
			525, 569, 178, 609, 270,  -1,
			526, 98,  -1,
			527, 99,  -1,
			
			-1
		};

		static
		size_t _SequenceIndexTable [] = 
		{
			0, 3, 10, 14, 19, 23, 27, 31, 36, 39, 42, 45, 49, 52, 56, 59, 63, 66, 70, 73, 77, 80, 84, 87, 91, 94, 98, 101, 105, 108, 112, 115, 118, 122, 125, 129, 132, 135, 138, 142, 147, 151, 154, 158, 161, 165, 168, 175, 178, 186, 189, 193, 196, 202, 210, 220, 223, 226, 230, 234, 238, 241, 244, 248, 256, 259, 263, 268, 271, 275, 280, 283, 287, 292, 296, 299, 302, 306, 310, 314, 318, 322, 325, 328, 331, 334, 337, 340, 343, 346, 349, 352, 355, 358, 361, 364, 367, 370, 373, 376, 379, 382, 385, 388, 391, 394, 397, 400, 403, 406, 409, 412, 415, 418, 421, 424, 427, 431, 437, 441, 445, 449, 453, 456, 459, 462, 465, 470, 474, 479, 484, 487, 492, 497, 501, 506, 510, 513, 516, 519, 524, 527, 532, 535, 538, 543, 547, 552, 555, 559, 564, 568, 574, 577, 580, 584, 588, 594, 598, 603, 606, 611, 614, 619, 623, 629, 632, 635, 639, 643, 647, 651, 656, 659, 663, 668, 671, 676, 679, 683, 687, 693, 698, 702, 708, 711, 
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
		
		case '(':
			return 4;
		
		case ')':
			return 5;
		
		case '.':
			return 6;
		
		case '?':
			return 7;
		
		case ':':
			return 8;
		
		case '|':
			return 9;
		
		case '^':
			return 10;
		
		case '&':
			return 11;
		
		case '<':
			return 12;
		
		case '>':
			return 13;
		
		case '+':
			return 14;
		
		case '-':
			return 15;
		
		case '*':
			return 16;
		
		case '/':
			return 17;
		
		case '%':
			return 18;
		
		case '~':
			return 19;
		
		case '!':
			return 20;
		
		case '=':
			return 21;
		
		case '[':
			return 22;
		
		case ']':
			return 23;
		
		case ',':
			return 24;
		
		case '{':
			return 25;
		
		case '}':
			return 26;
		
		case EToken_Pack:
			return 27;
		
		case EToken_Default:
			return 28;
		
		case EToken_Identifier:
			return 29;
		
		case EToken_LogicalOr:
			return 30;
		
		case EToken_LogicalAnd:
			return 31;
		
		case EToken_Eq:
			return 32;
		
		case EToken_Ne:
			return 33;
		
		case EToken_Le:
			return 34;
		
		case EToken_Ge:
			return 35;
		
		case EToken_Shl:
			return 36;
		
		case EToken_Shr:
			return 37;
		
		case EToken_Inc:
			return 38;
		
		case EToken_Dec:
			return 39;
		
		case EToken_SizeOf:
			return 40;
		
		case EToken_TypeOf:
			return 41;
		
		case EToken_AddAssign:
			return 42;
		
		case EToken_MulAssign:
			return 43;
		
		case EToken_DivAssign:
			return 44;
		
		case EToken_ModAssign:
			return 45;
		
		case EToken_ShlAssign:
			return 46;
		
		case EToken_ShrAssign:
			return 47;
		
		case EToken_AndAssign:
			return 48;
		
		case EToken_XorAssign:
			return 49;
		
		case EToken_OrAssign:
			return 50;
		
		case EToken_Integer:
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
			'(', 
			')', 
			'.', 
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
			'[', 
			']', 
			',', 
			'{', 
			'}', 
			EToken_Pack, 
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
			EToken_Integer, 
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
			_T("constant_integer_expression"),
			_T("qualified_identifier"),
			_T("type_name"),
			_T("type_specifier_modifier_list"),
			_T("pointer"),
			_T("declarator_suffix"),
			_T("expression"),
			_T("logical_or_expression"),
			_T("constant_expression"),
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
			
		
		
		case ESymbol_constant_integer_expression:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls4);
				
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
			
		
		
		
		case ESymbol_constant_expression:
			pNode = CreateStdSymbolNode (Index);
			pNode->m_Flags |= axl::llk::ESymbolNodeFlag_IsNamed;
				
			pNode->m_pAst = AXL_MEM_NEW (_cls3);
				
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
			
			{ 0, 29 },
			
			{ 1, 29 },
			
			{ 1, 29 },
			
			{ 0, 117 },
			
			{ 0, 64 },
			
			{ 1, 110 },
			
			{ 0, 155 },
			
			{ 0, 159 },
			
			{ 0, 169 },
			
			{ 0, 170 },
			
			{ 0, 171 },
			
			{ 0, 172 },
			
			{ 0, 174 },
			
			{ 0, 29 },
			
			{ 1, 175 },
			
			{ 0, 176 },
			
			{ 0, 109 },
			
			{ 0, 112 },
			
			{ 1, 159 },
			
			{ 0, 29 },
			
			{ 0, 29 },
			
			{ 1, 182 },
			
			{ 0, 29 },
			
			{ 0, 182 },
			
			{ 1, 29 },
			
			{ 0, 109 },
			
			{ 1, 186 },
			
			{ 0, 29 },
			
			{ 0, 186 },
			
			{ 0, 112 },
			
			{ 0, 159 },
			
			{ 1, 193 },
			
			{ 0, 29 },
			
			{ 0, 193 },
			
			{ 0, 155 },
			
			{ 0, 159 },
			
			{ 0, 198 },
			
			{ 0, 112 },
			
			{ 1, 200 },
			
			{ 0, 175 },
			
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
			  
			(*__pAst).m_Identifier.m_First = (*GetTokenLocator (0)).m_Data.m_String; 
		;
			}
			return true;
		
		case 1:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls1* __pAst = (_cls1*) __pSymbol->m_pAst;
			 
			(*__pAst).m_Identifier.m_List.InsertTail ((*GetTokenLocator (1)).m_Data.m_String);
		;
			}
			return true;
		
		case 2:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls3* __pAst = (_cls3*) __pSymbol->m_pAst;
			
			(*__pAst).m_pValue = NULL;
		;
			}
			return true;
		
		case 3:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls4* __pAst = (_cls4*) __pSymbol->m_pAst;
			
			(*__pAst).m_Value = 10;
		;
			}
			return true;
		
		case 4:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			CValue* pValue = IsValidLocator ((*(_cls3*) GetAstLocator (0))) ? (*(_cls3*) GetAstLocator (0)).m_pValue : NULL;
			CAttribute* pAttribute = m_pModule->m_AttributeMgr.CreateAttribute ((*GetTokenLocator (1)).m_Data.m_String, pValue);
			if (!pAttribute)
				return false;

			pAttribute->m_Pos = (*GetTokenLocator (1)).m_Pos;
		;
			}
			return true;
		
		case 5:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			 
			CNamespace* pNamespace = m_pModule->m_NamespaceMgr.OpenNamespace ((*GetTokenLocator (0)).m_Pos, (*(_cls1*) GetAstLocator (1)).m_Identifier);
			if (!pNamespace)
				return false;
		;
			}
			return true;
		
		case 6:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			 
			m_pModule->m_NamespaceMgr.CloseNamespace ((*(_cls1*) GetAstLocator (1)).m_Identifier.m_List.GetCount () + 1);
		;
			}
			return true;
		
		case 7:
			{
			CSymbolNode_init_declarator* __pSymbol = (CSymbolNode_init_declarator*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
		bool Result = Declare (__pSymbol->m_Arg.pDeclSpecifiers, &(*(_cls7*) GetAstLocator (0)).m_Declarator);
		if (!Result)
			return false;
	;
			}
			return true;
		
		case 8:
			{
			CSymbolNode_access_specifier* __pSymbol = (CSymbolNode_access_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pAccessSpecifier->SetAccess (EAccess_Public);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 9:
			{
			CSymbolNode_access_specifier* __pSymbol = (CSymbolNode_access_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pAccessSpecifier->SetAccess (EAccess_Private);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 10:
			{
			CSymbolNode_storage_class_specifier* __pSymbol = (CSymbolNode_storage_class_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pDeclSpecifiers->SetStorageClass (EStorageClass_Static);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 11:
			{
			CSymbolNode_storage_class_specifier* __pSymbol = (CSymbolNode_storage_class_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pDeclSpecifiers->SetStorageClass (EStorageClass_Typedef);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 12:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Const);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 13:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Property);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 14:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Signed);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 15:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_Unsigned);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 16:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_BigEndian);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 17:
			{
			CSymbolNode_type_modifier* __pSymbol = (CSymbolNode_type_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeModifiers->SetTypeModifier (ETypeModifier_LittleEndian);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 18:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Void));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 19:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Bool));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 20:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 21:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Char));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 22:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Short));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 23:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Long));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 24:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Float));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 25:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Double));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 26:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int8));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 27:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int16));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 28:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int32));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 29:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Int64));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 30:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_WChar));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 31:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType (m_pModule->m_TypeMgr.GetBasicType (EType_Variant));
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 32:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			err::SetStringError (_T("'auto' type specifier is not yet supported"));
			return false; 
		;
			}
			return true;
		
		case 33:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls10*) GetAstLocator (0)).m_pType);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 34:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls12*) GetAstLocator (0)).m_pType);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 35:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls14*) GetAstLocator (0)).m_pType);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 36:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetProperty ((*(_cls16*) GetAstLocator (0)).m_pProperty);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 37:
			{
			CSymbolNode_type_specifier* __pSymbol = (CSymbolNode_type_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = __pSymbol->m_Arg.pTypeSpecifier->SetType ((*(_cls9*) GetAstLocator (0)).m_pType);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 38:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAst = (_cls7*) __pSymbol->m_pAst;
			
			(*__pAst).m_Declarator.m_Pos = (*GetTokenLocator (0)).m_Pos;
			(*__pAst).m_Declarator.m_Name = (*GetTokenLocator (0)).m_Data.m_String;
		;
			}
			return true;
		
		case 39:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAst = (_cls7*) __pSymbol->m_pAst;
			
			(*__pAst).m_Declarator.m_PropertyAccessorKind = (*(_cls19*) GetAstLocator (1)).m_PropertyAccessorKind;
		;
			}
			return true;
		
		case 40:
			{
			CSymbolNode_pointer* __pSymbol = (CSymbolNode_pointer*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pSymbol->m_Local.pPointer = __pSymbol->m_Arg.pDeclarator->AddPointer ((*(_cls8*) GetAstLocator (0)).m_Type);
		;
			}
			return true;
		
		case 41:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAst = (_cls8*) __pSymbol->m_pAst;
			
			(*__pAst).m_Type = EType_Pointer;
		;
			}
			return true;
		
		case 42:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls8* __pAst = (_cls8*) __pSymbol->m_pAst;
			
			(*__pAst).m_Type = EType_Reference;
		;
			}
			return true;
		
		case 43:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix ((*(_cls4*) GetAstLocator (0)).m_Value);
		;
			}
			return true;
		
		case 44:
			{
			CSymbolNode_array_suffix* __pSymbol = (CSymbolNode_array_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pSymbol->m_Arg.pDeclarator->AddArraySuffix (0);
		;
			}
			return true;
		
		case 45:
			{
			CSymbolNode_formal_argument* __pSymbol = (CSymbolNode_formal_argument*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pSymbol->m_Local.pDefaultValue = NULL; // TODO: create default value
		;
			}
			return true;
		
		case 46:
			{
			CSymbolNode_formal_argument* __pSymbol = (CSymbolNode_formal_argument*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = DeclareFormalArg (__pSymbol->m_Arg.pArgSuffix, &(*(_cls6*) GetAstLocator (0)).m_TypeSpecifier, &(*(_cls7*) GetAstLocator (1)).m_Declarator, __pSymbol->m_Local.pDefaultValue);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 47:
			{
			CSymbolNode_formal_argument_vararg* __pSymbol = (CSymbolNode_formal_argument_vararg*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pSymbol->m_Arg.pArgSuffix->m_FunctionTypeFlags |= EFunctionTypeFlag_IsVarArg;
		;
			}
			return true;
		
		case 48:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			return false; // IsType ((*GetTokenLocator (0)).m_Identifier);
		;
			}
			return true;
		
		case 49:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls9* __pAst = (_cls9*) __pSymbol->m_pAst;
			
			(*__pAst).m_pType = NULL; // FindType ((*GetTokenLocator (0)).m_Identifier);
		;
			}
			return true;
		
		case 50:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAst = (_cls10*) __pSymbol->m_pAst;
			
			rtl::CString Name = (*GetTokenLocator (0)).m_Data.m_String;
			rtl::CString QualifiedName = m_pModule->m_NamespaceMgr.CreateQualifiedName (Name);
			(*__pAst).m_pType = m_pModule->m_TypeMgr.GetEnumType ((*(_cls11*) GetAstLocator (1)).m_Type, Name, QualifiedName);
			
			bool Result = Declare ((*__pAst).m_pType, (*(_cls11*) GetAstLocator (1)).m_FirstToken.m_Pos);
			if (!Result)
				return false;
				
		;
			}
			return true;
		
		case 51:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAst = (_cls10*) __pSymbol->m_pAst;
			
			(*__pAst).m_pType = m_pModule->m_TypeMgr.CreateUnnamedEnumType ((*(_cls11*) GetAstLocator (0)).m_Type);
			Declare ((*__pAst).m_pType, (*(_cls11*) GetAstLocator (0)).m_FirstToken.m_Pos);
		;
			}
			return true;
		
		case 52:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAst = (_cls11*) __pSymbol->m_pAst;
			
			(*__pAst).m_Type = EType_Enum;
		;
			}
			return true;
		
		case 53:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls11* __pAst = (_cls11*) __pSymbol->m_pAst;
			
			(*__pAst).m_Type = EType_EnumC;
		;
			}
			return true;
		
		case 54:
			{
			CSymbolNode_enum_member* __pSymbol = (CSymbolNode_enum_member*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			CEnumMember* pMember = IsValidLocator ((*(_cls4*) GetAstLocator (0))) ?
				__pSymbol->m_Arg.pType->CreateMember ((*GetTokenLocator (1)).m_Data.m_String, (*(_cls4*) GetAstLocator (0)).m_Value) :
				__pSymbol->m_Arg.pType->CreateMember ((*GetTokenLocator (1)).m_Data.m_String);

			if (!pMember)
				return false;

			pMember->m_Pos = (*GetTokenLocator (1)).m_Pos;
		;
			}
			return true;
		
		case 55:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAst = (_cls12*) __pSymbol->m_pAst;
			
			rtl::CString Name = (*GetTokenLocator (0)).m_Data.m_String;
			rtl::CString QualifiedName = m_pModule->m_NamespaceMgr.CreateQualifiedName (Name);
			(*__pAst).m_pType = m_pModule->m_TypeMgr.GetStructType ((*(_cls13*) GetAstLocator (1)).m_Type, Name, QualifiedName);
			
			bool Result = Declare ((*__pAst).m_pType, (*(_cls13*) GetAstLocator (1)).m_FirstToken.m_Pos);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 56:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAst = (_cls12*) __pSymbol->m_pAst;
			
			(*__pAst).m_pType = m_pModule->m_TypeMgr.CreateUnnamedStructType ((*(_cls13*) GetAstLocator (0)).m_Type);
			Declare ((*__pAst).m_pType, (*(_cls13*) GetAstLocator (0)).m_FirstToken.m_Pos);
		;
			}
			return true;
		
		case 57:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAst = (_cls13*) __pSymbol->m_pAst;
			
			(*__pAst).m_Type = EType_Struct;
		;
			}
			return true;
		
		case 58:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls13* __pAst = (_cls13*) __pSymbol->m_pAst;
			
			(*__pAst).m_Type = EType_Union;
		;
			}
			return true;
		
		case 59:
			{
			CSymbolNode_struct_member_declarator* __pSymbol = (CSymbolNode_struct_member_declarator*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = DeclareStructMember (__pSymbol->m_Arg.pType, __pSymbol->m_Arg.pTypeSpecifier, &(*(_cls7*) GetAstLocator (0)).m_Declarator);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 60:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAst = (_cls14*) __pSymbol->m_pAst;
			
			rtl::CString Name = (*GetTokenLocator (0)).m_Data.m_String;
			rtl::CString QualifiedName = m_pModule->m_NamespaceMgr.CreateQualifiedName (Name);
			(*__pAst).m_pType = m_pModule->m_TypeMgr.GetClassType ((*(_cls15*) GetAstLocator (1)).m_Type, Name, QualifiedName);
			
			bool Result = Declare ((*__pAst).m_pType, (*(_cls15*) GetAstLocator (1)).m_FirstToken.m_Pos);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 61:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAst = (_cls14*) __pSymbol->m_pAst;
			
			(*__pAst).m_pType = m_pModule->m_TypeMgr.CreateUnnamedClassType ((*(_cls15*) GetAstLocator (0)).m_Type);
			Declare ((*__pAst).m_pType, (*(_cls15*) GetAstLocator (0)).m_FirstToken.m_Pos);
		;
			}
			return true;
		
		case 62:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAst = (_cls15*) __pSymbol->m_pAst;
			
			(*__pAst).m_Type = EType_Interface;
		;
			}
			return true;
		
		case 63:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls15* __pAst = (_cls15*) __pSymbol->m_pAst;
			
			(*__pAst).m_Type = EType_Class;
		;
			}
			return true;
		
		case 64:
			{
			CSymbolNode_class_member_declarator* __pSymbol = (CSymbolNode_class_member_declarator*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = DeclareClassMember (__pSymbol->m_Arg.pType, __pSymbol->m_Arg.pDeclSpecifiers, &(*(_cls7*) GetAstLocator (0)).m_Declarator);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 65:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls16* __pAst = (_cls16*) __pSymbol->m_pAst;
			
			(*__pAst).m_pProperty = CreateProperty ((*(CPropertyBlock*) GetAstLocator (0)).m_pGetter, (*(CPropertyBlock*) GetAstLocator (0)).m_Setter);
		;
			}
			return true;
		
		case 66:
			{
			CSymbolNode_property_accessor_declaration* __pSymbol = (CSymbolNode_property_accessor_declaration*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			bool Result = DeclarePropertyAccessor (__pSymbol->m_Arg.pBlock, &(*(_cls6*) GetAstLocator (0)).m_TypeSpecifier, &(*(_cls18*) GetAstLocator (1)).m_Declarator);
			if (!Result)
				return false;
		;
			}
			return true;
		
		case 67:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAst = (_cls18*) __pSymbol->m_pAst;
			
			(*__pAst).m_Declarator.m_PropertyAccessorKind = (*(_cls19*) GetAstLocator (0)).m_PropertyAccessorKind;
			(*__pAst).m_Declarator.m_Pos = (*(_cls19*) GetAstLocator (0)).m_FirstToken.m_Pos;
		;
			}
			return true;
		
		case 68:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAst = (_cls19*) __pSymbol->m_pAst;
			
			(*__pAst).m_PropertyAccessorKind = EPropertyAccessor_Get;
		;
			}
			return true;
		
		case 69:
			{
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls19* __pAst = (_cls19*) __pSymbol->m_pAst;
			
			(*__pAst).m_PropertyAccessorKind = EPropertyAccessor_Set;
		;
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
			
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
			
			}
			break;
		
		case 1:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode_type_name* __pSymbol = (CSymbolNode_type_name*) GetSymbolTop ();
			_cls2* __pAst = (_cls2*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclarator = &__pSymbol->m_Local.Declarator;
			
			}
			break;
		
		case 2:
			{
			CSymbolNode_init_declarator_list* __pTarget = (CSymbolNode_init_declarator_list*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclSpecifiers = &(*(_cls5*) GetAstLocator (0)).m_DeclSpecifiers;
			
			}
			break;
		
		case 3:
			{
			CSymbolNode_init_declarator* __pTarget = (CSymbolNode_init_declarator*) pSymbol;
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
			
			}
			break;
		
		case 4:
			{
			CSymbolNode_init_declarator* __pTarget = (CSymbolNode_init_declarator*) pSymbol;
			CSymbolNode_init_declarator_list* __pSymbol = (CSymbolNode_init_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
			
			}
			break;
		
		case 5:
			{
			CSymbolNode_declaration_specifier* __pTarget = (CSymbolNode_declaration_specifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls5* __pAst = (_cls5*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclSpecifiers = &(*__pAst).m_DeclSpecifiers;
			
			}
			break;
		
		case 6:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls6* __pAst = (_cls6*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pTypeSpecifier = &(*__pAst).m_TypeSpecifier;
			
			}
			break;
		
		case 7:
			{
			CSymbolNode_type_specifier_modifier* __pTarget = (CSymbolNode_type_specifier_modifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pDeclSpecifiers;
			
			}
			break;
		
		case 8:
			{
			CSymbolNode_access_specifier* __pTarget = (CSymbolNode_access_specifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pAccessSpecifier = __pSymbol->m_Arg.pDeclSpecifiers;
			
			}
			break;
		
		case 9:
			{
			CSymbolNode_storage_class_specifier* __pTarget = (CSymbolNode_storage_class_specifier*) pSymbol;
			CSymbolNode_declaration_specifier* __pSymbol = (CSymbolNode_declaration_specifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
			
			}
			break;
		
		case 10:
			{
			CSymbolNode_type_specifier* __pTarget = (CSymbolNode_type_specifier*) pSymbol;
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
			
			}
			break;
		
		case 11:
			{
			CSymbolNode_type_modifier* __pTarget = (CSymbolNode_type_modifier*) pSymbol;
			CSymbolNode_type_specifier_modifier* __pSymbol = (CSymbolNode_type_specifier_modifier*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pTypeModifiers = __pSymbol->m_Arg.pTypeSpecifier;
			
			}
			break;
		
		case 12:
			{
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAst = (_cls7*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclarator = &(*__pAst).m_Declarator;
			
			}
			break;
		
		case 13:
			{
			CSymbolNode_declarator_suffix* __pTarget = (CSymbolNode_declarator_suffix*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls7* __pAst = (_cls7*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclarator = &(*__pAst).m_Declarator;
			
			}
			break;
		
		case 14:
			{
			CSymbolNode_type_modifier* __pTarget = (CSymbolNode_type_modifier*) pSymbol;
			CSymbolNode_pointer* __pSymbol = (CSymbolNode_pointer*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pTypeModifiers = __pSymbol->m_Local.pPointer;
			
			}
			break;
		
		case 15:
			{
			CSymbolNode_array_suffix* __pTarget = (CSymbolNode_array_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
			
			}
			break;
		
		case 16:
			{
			CSymbolNode_formal_argument_suffix* __pTarget = (CSymbolNode_formal_argument_suffix*) pSymbol;
			CSymbolNode_declarator_suffix* __pSymbol = (CSymbolNode_declarator_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclarator = __pSymbol->m_Arg.pDeclarator;
			
			}
			break;
		
		case 17:
			{
			CSymbolNode_formal_argument_list* __pTarget = (CSymbolNode_formal_argument_list*) pSymbol;
			CSymbolNode_formal_argument_suffix* __pSymbol = (CSymbolNode_formal_argument_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pArgSuffix = __pSymbol->m_Local.pArgSuffix;
			
			}
			break;
		
		case 18:
			{
			CSymbolNode_formal_argument_vararg* __pTarget = (CSymbolNode_formal_argument_vararg*) pSymbol;
			CSymbolNode_formal_argument_suffix* __pSymbol = (CSymbolNode_formal_argument_suffix*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pArgSuffix = __pSymbol->m_Local.pArgSuffix;
			
			}
			break;
		
		case 19:
			{
			CSymbolNode_formal_argument* __pTarget = (CSymbolNode_formal_argument*) pSymbol;
			CSymbolNode_formal_argument_list* __pSymbol = (CSymbolNode_formal_argument_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pArgSuffix = __pSymbol->m_Arg.pArgSuffix;
			
			}
			break;
		
		case 20:
			{
			CSymbolNode_formal_argument* __pTarget = (CSymbolNode_formal_argument*) pSymbol;
			CSymbolNode_formal_argument_list* __pSymbol = (CSymbolNode_formal_argument_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pArgSuffix = __pSymbol->m_Arg.pArgSuffix;
			
			}
			break;
		
		case 21:
			{
			CSymbolNode_formal_argument_vararg* __pTarget = (CSymbolNode_formal_argument_vararg*) pSymbol;
			CSymbolNode_formal_argument_list* __pSymbol = (CSymbolNode_formal_argument_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pArgSuffix = __pSymbol->m_Arg.pArgSuffix;
			
			}
			break;
		
		case 22:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAst = (_cls10*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
			
			}
			break;
		
		case 23:
			{
			CSymbolNode_enum_block* __pTarget = (CSymbolNode_enum_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls10* __pAst = (_cls10*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
			
			}
			break;
		
		case 24:
			{
			CSymbolNode_enum_member_list* __pTarget = (CSymbolNode_enum_member_list*) pSymbol;
			CSymbolNode_enum_block* __pSymbol = (CSymbolNode_enum_block*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 25:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 26:
			{
			CSymbolNode_enum_member* __pTarget = (CSymbolNode_enum_member*) pSymbol;
			CSymbolNode_enum_member_list* __pSymbol = (CSymbolNode_enum_member_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 27:
			{
			CSymbolNode_struct_block* __pTarget = (CSymbolNode_struct_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAst = (_cls12*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
			
			}
			break;
		
		case 28:
			{
			CSymbolNode_struct_block* __pTarget = (CSymbolNode_struct_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls12* __pAst = (_cls12*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
			
			}
			break;
		
		case 29:
			{
			CSymbolNode_struct_member_declaration* __pTarget = (CSymbolNode_struct_member_declaration*) pSymbol;
			CSymbolNode_struct_block* __pSymbol = (CSymbolNode_struct_block*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 30:
			{
			CSymbolNode_struct_member_declarator_list* __pTarget = (CSymbolNode_struct_member_declarator_list*) pSymbol;
			CSymbolNode_struct_member_declaration* __pSymbol = (CSymbolNode_struct_member_declaration*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pTypeSpecifier = &(*(_cls6*) GetAstLocator (0)).m_TypeSpecifier;
			
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 31:
			{
			CSymbolNode_struct_member_declarator* __pTarget = (CSymbolNode_struct_member_declarator*) pSymbol;
			CSymbolNode_struct_member_declarator_list* __pSymbol = (CSymbolNode_struct_member_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
			
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 32:
			{
			CSymbolNode_struct_member_declarator* __pTarget = (CSymbolNode_struct_member_declarator*) pSymbol;
			CSymbolNode_struct_member_declarator_list* __pSymbol = (CSymbolNode_struct_member_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pTypeSpecifier = __pSymbol->m_Arg.pTypeSpecifier;
			
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 33:
			{
			CSymbolNode_class_block* __pTarget = (CSymbolNode_class_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAst = (_cls14*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
			
			}
			break;
		
		case 34:
			{
			CSymbolNode_class_block* __pTarget = (CSymbolNode_class_block*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls14* __pAst = (_cls14*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = (*__pAst).m_pType;
			
			}
			break;
		
		case 35:
			{
			CSymbolNode_class_member_declaration* __pTarget = (CSymbolNode_class_member_declaration*) pSymbol;
			CSymbolNode_class_block* __pSymbol = (CSymbolNode_class_block*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pType = __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 36:
			{
			CSymbolNode_class_member_declarator_list* __pTarget = (CSymbolNode_class_member_declarator_list*) pSymbol;
			CSymbolNode_class_member_declaration* __pSymbol = (CSymbolNode_class_member_declaration*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclSpecifiers = &(*(_cls5*) GetAstLocator (0)).m_DeclSpecifiers;
			
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 37:
			{
			CSymbolNode_class_member_declarator* __pTarget = (CSymbolNode_class_member_declarator*) pSymbol;
			CSymbolNode_class_member_declarator_list* __pSymbol = (CSymbolNode_class_member_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
			
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 38:
			{
			CSymbolNode_class_member_declarator* __pTarget = (CSymbolNode_class_member_declarator*) pSymbol;
			CSymbolNode_class_member_declarator_list* __pSymbol = (CSymbolNode_class_member_declarator_list*) GetSymbolTop ();
			CAst* __pAst = __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclSpecifiers = __pSymbol->m_Arg.pDeclSpecifiers;
			
			__pTarget->m_Arg.pType =  __pSymbol->m_Arg.pType;
			
			}
			break;
		
		case 39:
			{
			CSymbolNode_property_accessor_declaration* __pTarget = (CSymbolNode_property_accessor_declaration*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			CPropertyBlock* __pAst = (CPropertyBlock*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pBlock = &(*__pAst);
			
			}
			break;
		
		case 40:
			{
			CSymbolNode_pointer* __pTarget = (CSymbolNode_pointer*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAst = (_cls18*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclarator = &(*__pAst).m_Declarator;
			
			}
			break;
		
		case 41:
			{
			CSymbolNode_formal_argument_suffix* __pTarget = (CSymbolNode_formal_argument_suffix*) pSymbol;
			CSymbolNode* __pSymbol = GetSymbolTop ();
			_cls18* __pAst = (_cls18*) __pSymbol->m_pAst;
			
			__pTarget->m_Arg.pDeclarator = &(*__pAst).m_Declarator;
			
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
				
		m_pModule->m_AttributeMgr.CreateAttributeSet ();
	;
			}
			return true;
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		case ESymbol_formal_argument_suffix:
			{
				CSymbolNode_formal_argument_suffix* __pSymbol = (CSymbolNode_formal_argument_suffix*) GetSymbolTop ();
				CAst* __pAst = __pSymbol->m_pAst;
				
		__pSymbol->m_Local.pArgSuffix = __pSymbol->m_Arg.pDeclarator->AddFormalArgSuffix ();
	;
			}
			return true;
			
		
		
		
		case ESymbol_formal_argument:
			{
				CSymbolNode_formal_argument* __pSymbol = (CSymbolNode_formal_argument*) GetSymbolTop ();
				CAst* __pAst = __pSymbol->m_pAst;
				
		__pSymbol->m_Local.pDefaultValue = NULL;
	;
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
				
		if ((*__pAst).m_pType->GetTypeKind () == EType_EnumC)
		{
			bool Result = m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->ExposeEnumMembers ((*__pAst).m_pType);
			if (!Result)
				return false;
		}
	;
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
				pTransition->m_ResolverIndex = 173;
				pTransition->m_ResolverElseIndex = 107;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 1:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 612;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 2:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case ')':
					
				pTransition->m_ProductionIndex = 313;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 311;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 3:
			
			switch (LookaheadToken)
			{
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 614;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 4:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 356;
				return ELaDfaResult_Production;
					
			
			default:
					
				pTransition->m_ProductionIndex = 357;
				return ELaDfaResult_Production;
					
			}
			
		
		case 5:
			
			switch (LookaheadToken)
			{
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 616;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 6:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 413;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 411;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 7:
			
			switch (LookaheadToken)
			{
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 618;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 8:
			
			switch (LookaheadToken)
			{
			
			case '{':
					
				pTransition->m_ProductionIndex = 413;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 411;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 9:
			
			switch (LookaheadToken)
			{
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 620;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 10:
			
			switch (LookaheadToken)
			{
			
			case ':':
					
				pTransition->m_ProductionIndex = 423;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 423;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 421;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 11:
			
			switch (LookaheadToken)
			{
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 622;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 12:
			
			switch (LookaheadToken)
			{
			
			case ':':
					
				pTransition->m_ProductionIndex = 423;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 423;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 421;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 13:
			
			switch (LookaheadToken)
			{
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 624;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 14:
			
			switch (LookaheadToken)
			{
			
			case ':':
					
				pTransition->m_ProductionIndex = 436;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 436;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 434;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 15:
			
			switch (LookaheadToken)
			{
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 626;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 16:
			
			switch (LookaheadToken)
			{
			
			case ':':
					
				pTransition->m_ProductionIndex = 436;
				return ELaDfaResult_Production;
					
			
			case '{':
					
				pTransition->m_ProductionIndex = 436;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 434;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 17:
			
			switch (LookaheadToken)
			{
			
			case '[':
					
				pTransition->m_ProductionIndex = 628;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 18:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case '&':
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case '+':
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case '-':
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case '*':
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case '~':
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case '!':
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case ']':
					
				pTransition->m_ProductionIndex = 399;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case EToken_Inc:
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case EToken_Dec:
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case EToken_SizeOf:
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case EToken_TypeOf:
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case EToken_Integer:
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case EToken_Fp:
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			case EToken_Literal:
					
				pTransition->m_ProductionIndex = 398;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 19:
			
			switch (LookaheadToken)
			{
			
			case '(':
					
				pTransition->m_ProductionIndex = 630;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 20:
			
			switch (LookaheadToken)
			{
			
			case ')':
					
				pTransition->m_ProductionIndex = 402;
				return ELaDfaResult_Production;
					
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Const:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Property:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Signed:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Unsigned:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_BigEndian:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_LittleEndian:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Void:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Bool:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Int:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Char:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Short:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Long:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Float:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Double:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Int8:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Int16:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Int32:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Int64:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_WChar:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Variant:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Auto:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Ellipsis:
					
				pTransition->m_ProductionIndex = 401;
				return ELaDfaResult_Production;
					
			
			case EToken_Enum:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_EnumC:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Struct:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Union:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Interface:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			case EToken_Class:
					
				pTransition->m_ProductionIndex = 400;
				return ELaDfaResult_Production;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 21:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 350;
				pTransition->m_ResolverIndex = 173;
				pTransition->m_ResolverElseIndex = 0;
						
				return ELaDfaResult_Resolver;
					
			
			default:
									
				return ELaDfaResult_Fail;
					
			}
			
		
		case 22:
			
			switch (LookaheadToken)
			{
			
			case EToken_Identifier:
					
				pTransition->m_ProductionIndex = 352;
				pTransition->m_ResolverIndex = 173;
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
