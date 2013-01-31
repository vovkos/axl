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
	AntiModifierTable [32] = 
	{		
		ETypeModifierMask_Sign,       //  0 -- ETypeModifier_Signed        = 0x00000001,		
		ETypeModifierMask_Sign,       //  1 -- ETypeModifier_Unsigned      = 0x00000002,		
		ETypeModifierMask_Endian,     //  2 -- ETypeModifier_LittleEndian  = 0x00000004,		
		ETypeModifierMask_Endian,     //  3 -- ETypeModifier_BigEndian     = 0x00000008,		
		0,                            //  4 -- ETypeModifier_Const         = 0x00000010,		
		0,                            //  5 -- ETypeModifier_Volatile      = 0x00000020,		
		ETypeModifierMask_Safety,     //  6 -- ETypeModifier_Safe          = 0x00000040,	
		ETypeModifierMask_Safety |    //  7 -- ETypeModifier_Unsafe        = 0x00000080,
		ETypeModifierMask_Strength | 
		ETypeModifierMask_Closure,		
		0,                            //  8 -- ETypeModifier_NoNull        = 0x00000100,		
		ETypeModifierMask_Strength |  //  9 -- ETypeModifier_Strong        = 0x00000200,
		ETypeModifier_Thin |
		ETypeModifier_Unsafe,		
		ETypeModifierMask_Strength |  // 10 -- ETypeModifier_Weak          = 0x00000400,
		ETypeModifier_Thin |
		ETypeModifier_Unsafe,		
		ETypeModifierMask_CallConv,   // 11 -- ETypeModifier_Cdecl         = 0x00000800,		
		ETypeModifierMask_CallConv,   // 12 -- ETypeModifier_Stdcall       = 0x00001000,		
		ETypeModifier_Property |      // 13 -- ETypeModifier_Function      = 0x00002000,
		ETypeModifier_Event,		
		ETypeModifier_Function |      // 14 -- ETypeModifier_Property      = 0x00004000,
		ETypeModifier_Event,				
		ETypeModifier_Function |      // 15 -- ETypeModifier_Event         = 0x00008000,
		ETypeModifier_Property,		
		0,                            // 16 -- ETypeModifier_Bindable      = 0x00010000,		
		ETypeModifier_Indexed,        // 17 -- ETypeModifier_AutoGet       = 0x00020000,		
		ETypeModifier_AutoGet,        // 18 -- ETypeModifier_Indexed       = 0x00040000,		
		ETypeModifierMask_Closure |   // 19 -- ETypeModifier_Closure       = 0x00080000,
		ETypeModifier_Unsafe,		
		ETypeModifierMask_Closure |   // 20 -- ETypeModifier_Thin          = 0x00100000,
		ETypeModifierMask_Strength |        
		ETypeModifier_Unsafe,
	};

	if (m_TypeModifiers & Modifier)
	{
		err::SetFormatStringError (_T("type modifier '%s' used more than once"), GetTypeModifierString (Modifier));
		return false;
	}

	size_t i = rtl::GetLoBitIdx32 (Modifier);
	ASSERT (i < 32);

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
	if (m_pType || (m_TypeModifiers & ETypeModifier_Event))
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

	// eat left 'const' modifier for classes 
	// since class to class ptr conversion is implicit,
	// there is a conflict in where to apply 'const' modifier, e.g.
	// const CMyClass x; <-- is it a const class ptr or const variable?

	if (m_TypeModifiers & ETypeModifier_Const)
	{
		if (TypeKind == EType_Class)
		{
			EClassPtrType PtrTypeKind = GetClassPtrTypeKindFromModifiers (m_TypeModifiers);
			pType = ((CClassType*) pType)->GetClassPtrType (EClassPtrType_Normal, EPtrTypeFlag_Const);
			m_TypeModifiers &= ~ETypeModifier_Const;
		}
		else if (TypeKind == EType_ClassPtr)
		{
			CClassPtrType* pPtrType = (CClassPtrType*) pType;
			pType = pPtrType->GetClassType ()->GetClassPtrType (
				pPtrType->GetPtrTypeKind (),
				pPtrType->GetFlags () | EPtrTypeFlag_Const
				);
			m_TypeModifiers &= ~ETypeModifier_Const;
		}
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

CDeclarator::CDeclarator ()
{
	m_DeclaratorKind = EDeclarator_Undefined;
	m_FunctionKind = EFunction_Undefined;
	m_pType = NULL;
	m_BitCount = 0;
}

bool
CDeclarator::SetTypeSpecifier (CTypeSpecifier* pTypeSpecifier)
{
	CModule* pModule = GetCurrentThreadModule ();
	ASSERT (pModule);

	if (!pTypeSpecifier)
	{
		m_pType = pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
		return true;
	}

	m_pType = pTypeSpecifier->GetType ();
	m_TypeModifiers = pTypeSpecifier->GetTypeModifiers ();
	
	if (!m_pType)
	{
		m_pType = (m_TypeModifiers & ETypeModifierMask_Integer) ? 
			pModule->m_TypeMgr.GetPrimitiveType (EType_Int) : 
			pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	}
	
	return true;
}

bool
CDeclarator::AddName (rtl::CString Name)
{
	if (m_FunctionKind && m_FunctionKind != EFunction_Named)
	{
		err::SetFormatStringError (_T("cannot further qualify '%s' declarator"), GetFunctionKindString (m_FunctionKind));
		return false;
	}

	m_FunctionKind = EFunction_Named;

	if (m_Name.m_First.IsEmpty ())
	{
		m_DeclaratorKind = EDeclarator_SimpleName;
		m_Name.m_First = Name;
	}
	else
	{
		m_DeclaratorKind = EDeclarator_QualifiedName;
		m_Name.m_List.InsertTail (Name);
	}

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

	m_FunctionKind = FunctionKind;
	m_DeclaratorKind = m_Name.IsEmpty () ? EDeclarator_UnnamedMethod : EDeclarator_QualifiedName;
	return true;
}

bool
CDeclarator::AddCastOperator (CType* pType)
{
	err::SetFormatStringError (_T("overloaded cast operators not implemented yet"));
	return false;
}

bool
CDeclarator::AddOperator (
	EUnOp UnOpKind,
	EBinOp BinOpKind
	)
{
	err::SetFormatStringError (_T("overloaded unary & binary operators not implemented yet"));
	return false;
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
CDeclarator::AddPointer ()
{
	m_PointerArray.Append (m_TypeModifiers);
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
	if (m_BitCount || !m_SuffixList.IsEmpty () || !m_PointerArray.IsEmpty ())
	{
		err::SetFormatStringError (_T("bit field can only be applied to integer type"));
		return false;
	}

	m_BitCount = BitCount;
	return true;
}

CType*
CDeclarator::GetType (int* pDataPtrTypeFlags)
{
	CDeclTypeCalc TypeCalc;
	return TypeCalc.GetType (
		m_pType, 
		m_TypeModifiers, 
		m_PointerArray, 
		m_PointerArray.GetCount (), 
		m_SuffixList.GetTail (),
		pDataPtrTypeFlags
		);
}

rtl::CStdListT <CFunctionFormalArg>*
CDeclarator::GetArgList ()
{
	rtl::CIteratorT <CDeclSuffix> Suffix = m_SuffixList.GetHead ();
	if (!Suffix || Suffix->GetSuffixKind () != EDeclSuffix_Function)
		return NULL;

	CDeclFunctionSuffix* pSuffix = (CDeclFunctionSuffix*) *Suffix;
	return &pSuffix->m_ArgList;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
