#include "stdafx.h"
#include "axl_jnc_Decl.h"
#include "axl_jnc_DeclTypeCalc.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CTypeModifiers::SetTypeModifier (ETypeModifier Modifier)
{
	static
	int 
	AntiModifierTable [] = 
	{		
		ETypeModifier_Unsigned,     // ETypeModifier_Signed           = 0x00000001,
		ETypeModifier_Signed,       // ETypeModifier_Unsigned         = 0x00000002,
		0,                          // ETypeModifier_BigEndian        = 0x00000004,
		0,                          // ETypeModifier_Nullable           = 0x00000008,
		ETypeModifier_ReadOnly,     // ETypeModifier_Const            = 0x00000010,
		ETypeModifier_Const,        // ETypeModifier_ReadOnly         = 0x00000020,
		0,                          // ETypeModifier_Volatile         = 0x00000040,
		ETypeModifier_Thin |        // ETypeModifier_Weak             = 0x00000080,
		ETypeModifier_Unsafe,   
		ETypeModifier_Weak |        // ETypeModifier_Thin             = 0x00000100,
		ETypeModifier_Unsafe,   
		ETypeModifier_Weak |        // ETypeModifier_Unsafe           = 0x00000200,
		ETypeModifier_Thin,   
		ETypeModifier_Stdcall,      // ETypeModifier_Cdecl            = 0x00000400,
		ETypeModifier_Cdecl,        // ETypeModifier_Stdcall          = 0x00000800,
		ETypeModifier_Function |    // ETypeModifier_Class            = 0x00001000,
		ETypeModifier_Property,      
		ETypeModifier_Class |       // ETypeModifier_Function         = 0x00002000,
		ETypeModifier_Property,      
		ETypeModifier_Class |       // ETypeModifier_Property         = 0x00004000,
		ETypeModifier_Function,      
		0,                          // ETypeModifier_Bindable         = 0x00008000,
		ETypeModifier_Indexed,      // ETypeModifier_AutoGet          = 0x00010000,
		ETypeModifier_AutoGet,      // ETypeModifier_Indexed          = 0x00020000,
	};

	// check duplicates

	if (m_TypeModifiers & Modifier)
	{
		err::SetFormatStringError (_T("type modifier '%s' used more than once"), GetTypeModifierString (Modifier));
		return false;
	}

	size_t i = rtl::GetLoBitIdx32 (Modifier);
	if (i >= countof (AntiModifierTable))
		return true; // allow adding new modifiers without changing table

	// check anti-modifiers

	if (m_TypeModifiers & AntiModifierTable [i])
	{
		ETypeModifier Modifier2 = GetFirstTypeModifier (m_TypeModifiers);
		err::SetFormatStringError (
			_T("type modifiers '%s' and '%s' cannot be used together"),
			GetTypeModifierString (Modifier2),
			GetTypeModifierString (Modifier)
			);

		return false;
	}

	m_TypeModifiers |= Modifier;
	return true;
}

int
CTypeModifiers::ClearTypeModifiers (int ModifierMask)
{
	int TypeModifiers = m_TypeModifiers & ModifierMask;
	m_TypeModifiers &= ~ModifierMask;
	return TypeModifiers;
}

bool
CTypeModifiers::CheckAntiTypeModifiers (int ModifierMask)
{
	int Modifiers = m_TypeModifiers;

	Modifiers &= ModifierMask;
	if (!Modifiers)
		return true;

	ETypeModifier FirstModifier = GetFirstTypeModifier (Modifiers);
	Modifiers &= ~FirstModifier;
	if (!Modifiers)
		return true;

	// more than one

	ETypeModifier SecondModifier = GetFirstTypeModifier (Modifiers);
	err::SetFormatStringError (
		_T("type modifiers '%s' and '%s' cannot be used together"),
		GetTypeModifierString (FirstModifier),
		GetTypeModifierString (SecondModifier)
		);

	return false;
}

//.............................................................................

bool
CTypeSpecifier::SetType (CType* pType)
{
	if (m_pType)
	{
		err::SetFormatStringError (
			_T("more than one type specifiers ('%s' and '%s')"), 
			m_pType->GetTypeString (),
			pType->GetTypeString ()
			);

		return false;
	}

	int TypeKind = pType->GetTypeKind ();
	if (TypeKind == EType_Import)
	{
		err::SetFormatStringError (_T("import types are not supported yet"));
		return NULL;
	}

	if (TypeKind == EType_Class || TypeKind == EType_ClassPtr)
	{
		m_TypeModifiers |= PromoteClassPtrTypeModifiers (m_TypeModifiers);
		m_TypeModifiers &= ~ETypeModifierMask_ClassPtr;
	}
	
	m_pType = pType;
	return true;
}

//.............................................................................

rtl::CArrayT <CType*>
CDeclFunctionSuffix::GetArgTypeArray ()
{
	rtl::CArrayT <CType*> ArgTypeArray;
	ArgTypeArray.SetCount (m_ArgList.GetCount ());

	rtl::CIteratorT <CFunctionFormalArg> Arg = m_ArgList.GetHead ();
	for (size_t i = 0; Arg; Arg++, i++)
		ArgTypeArray [i] = Arg->GetType ();

	return ArgTypeArray;
}

//.............................................................................

const tchar_t* 
GetPostDeclaratorModifierString (EPostDeclaratorModifier Modifier)
{
	switch (Modifier)
	{
	case EPostDeclaratorModifier_Const:
		return _T("const");

	default:
		return _T("undefined-declarator-modifier");
	}
}

//.............................................................................

CDeclarator::CDeclarator ()
{
	m_DeclaratorKind = EDeclarator_Undefined;
	m_FunctionKind = EFunction_Undefined;
	m_UnOpKind = EUnOp_Undefined;
	m_BinOpKind = EBinOp_Undefined;
	m_pCastOpType = NULL;
	m_BitCount = 0;
	m_PostDeclaratorModifiers = 0;
	m_pBaseType = NULL;
}

void
CDeclarator::SetTypeSpecifier (CTypeSpecifier* pTypeSpecifier)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	if (!pTypeSpecifier)
	{
		m_pBaseType = pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	}
	else
	{
		m_pBaseType = pTypeSpecifier->GetType ();
		m_TypeModifiers = pTypeSpecifier->GetTypeModifiers ();
	
		if (!m_pBaseType)
		{
			m_pBaseType = (m_TypeModifiers & ETypeModifierMask_Integer) ? 
				pModule->m_TypeMgr.GetPrimitiveType (EType_Int) : 
				pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
		}
	}
}

bool
CDeclarator::AddName (rtl::CString Name)
{
	if (m_FunctionKind && m_FunctionKind != EFunction_Named)
	{
		err::SetFormatStringError (_T("cannot further qualify '%s' declarator"), GetFunctionKindString (m_FunctionKind));
		return false;
	}

	m_DeclaratorKind = EDeclarator_Name;
	m_FunctionKind = EFunction_Named;
	m_Name.AddName (Name);
	return true;
}

bool
CDeclarator::AddUnnamedMethod (EFunction FunctionKind)
{
	if (m_FunctionKind && m_FunctionKind != EFunction_Named)
	{
		err::SetFormatStringError (_T("cannot further qualify '%s' declarator"), GetFunctionKindString (m_FunctionKind));
		return false;
	}

	m_DeclaratorKind = EDeclarator_UnnamedMethod;
	m_FunctionKind = FunctionKind;
	return true;
}

bool
CDeclarator::AddCastOperator (CType* pType)
{
	m_DeclaratorKind = EDeclarator_CastOperator;
	m_FunctionKind = EFunction_CastOperator;
	m_pCastOpType = pType;
	return false;
}

bool
CDeclarator::AddUnaryBinaryOperator (
	EUnOp UnOpKind,
	EBinOp BinOpKind
	)
{
	if (m_FunctionKind && m_FunctionKind != EFunction_Named)
	{
		err::SetFormatStringError (_T("cannot further qualify '%s' declarator"), GetFunctionKindString (m_FunctionKind));
		return false;
	}

	if (BinOpKind == EBinOp_Assign)
	{
		err::SetFormatStringError (_T("assignment operator could not be overloaded"));
		return false;
	}

	m_DeclaratorKind = EDeclarator_UnaryBinaryOperator;
	m_UnOpKind = UnOpKind;
	m_BinOpKind = BinOpKind;
	return true;
}

bool
CDeclarator::SetPostDeclaratorModifier (EPostDeclaratorModifier Modifier)
{
	if (m_PostDeclaratorModifiers & Modifier)
	{
		err::SetFormatStringError (_T("type modifier '%s' used more than once"), GetPostDeclaratorModifierString (Modifier));
		return false;
	}

	m_PostDeclaratorModifiers |= Modifier;
	return true;
}

bool
CDeclarator::SetPropValue ()
{
	if (m_DeclaratorKind)
	{
		err::SetFormatStringError (_T("cannot create qualified 'propvalue' declarator"));
		return false;
	}

	m_DeclaratorKind = EDeclarator_PropValue;
	return true;
}

bool
CDeclarator::AddPrefix (EDeclPrefix PrefixKind)
{
	TDeclPrefix Prefix;
	Prefix.m_PrefixKind = PrefixKind;
	Prefix.m_TypeModifiers = m_TypeModifiers;

	m_PrefixArray.Append (Prefix);
	m_TypeModifiers = 0;
	return true;
}

CDeclArraySuffix*
CDeclarator::AddArraySuffix (size_t ElementCount)
{
	CDeclArraySuffix* pArraySuffix = AXL_MEM_NEW (CDeclArraySuffix);
	pArraySuffix->m_ElementCount = ElementCount;
	m_SuffixList.InsertTail (pArraySuffix);
	return pArraySuffix;
}

CDeclFunctionSuffix*
CDeclarator::AddFunctionSuffix ()
{
	CDeclFunctionSuffix* pFunctionSuffix = AXL_MEM_NEW (CDeclFunctionSuffix);
	m_SuffixList.InsertTail (pFunctionSuffix);
	return pFunctionSuffix;
}

bool
CDeclarator::AddBitFieldSuffix (size_t BitCount)
{
	if (m_BitCount || !m_SuffixList.IsEmpty () || !m_PrefixArray.IsEmpty ())
	{
		err::SetFormatStringError (_T("bit field can only be applied to integer type"));
		return false;
	}

	m_BitCount = BitCount;
	return true;
}

CType*
CDeclarator::CalcType (int* pDataPtrTypeFlags)
{
	CDeclTypeCalc TypeCalc;
	return TypeCalc.CalcType (this, pDataPtrTypeFlags);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
