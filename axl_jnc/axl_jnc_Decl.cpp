#include "stdafx.h"
#include "axl_jnc_Decl.h"
#include "axl_jnc_TypeMgr.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetStorageClassString (EStorageClass StorageClass)
{
	switch (StorageClass)
	{
	case EStorageClass_Static:
		return _T("static");

	case EStorageClass_Typedef:
		return _T("typedef");

	default:
		return _T("undefined-storage-class");
	};
}

const tchar_t*
GetAccessString (EAccess Access)
{
	switch (Access)
	{
	case EAccess_Public:
		return _T("public");

	case EAccess_Private:
		return _T("private");

	default:
		return _T("undefined-access");
	};
}

const tchar_t*
GetPropertyAccessorString (EPropertyAccessor PropertyAccessor)
{
	switch (PropertyAccessor)
	{
	case EPropertyAccessor_Get:
		return _T("get");

	case EPropertyAccessor_Set:
		return _T("set");

	default:
		return _T("undefined-property-accessor");
	};
}

//.............................................................................

bool
CStorageClassSpecifier::SetStorageClass (EStorageClass StorageClass)
{
	if (m_StorageClass)
	{
		err::SetStringError (_T("more than one storage class specifiers"));
		return false;
	}

	m_StorageClass = StorageClass;
	return true;
}

bool
CAccessSpecifier::SetAccess (EAccess Access)
{
	if (m_Access)
	{
		err::SetStringError (_T("more than one access specifiers"));
		return false;
	}

	m_Access = Access;
	return true;
}

bool
CTypeSpecifier::SetType (CType* pType)
{
	if (m_pType)
	{
		err::SetStringError (_T("more than one type specified"));
		return false;
	}

	m_pType = pType;
	return true;
}

bool
CTypeSpecifier::SetProperty (CProperty* pProperty)
{
	if (m_pType || m_pProperty)
	{
		err::SetStringError (_T("more than one type specified"));
		return false;
	}

	m_pProperty = pProperty;
	m_pType = pProperty->GetType ();
	return true;
}

bool
CTypeModifiers::SetTypeModifier (ETypeModifier Modifier)
{
	if (m_TypeModifiers & Modifier)
	{
		err::SetFormatStringError (_T("type modifier '%s' used more than once"), GetTypeModifierString (Modifier));
		return false;
	}

	m_TypeModifiers |= Modifier;
	return true;
}

//.............................................................................

CFunctionFormalArg*
CDeclFormalArgSuffix::AddArg (
	const rtl::CString& Name,
	CType* pType,
	CValue* pDefaultValue
	)
{
	CFunctionFormalArg* pArg = AXL_MEM_NEW (CFunctionFormalArg);
	pArg->m_Name = Name;
	pArg->m_pType = pType;
	pArg->m_pDefaultValue = pDefaultValue;
	m_ArgList.InsertTail (pArg);
	return pArg;
}

//.............................................................................

CDeclPointer*
CDeclarator::AddPointer (EType TypeKind)
{
	CDeclPointer* pPointer = AXL_MEM_NEW (CDeclPointer);
	pPointer->m_TypeKind = TypeKind;
	m_PointerList.InsertTail (pPointer);
	return pPointer;
}

CDeclArraySuffix*
CDeclarator::AddArraySuffix (size_t ElementCount)
{
	CDeclArraySuffix* pArraySuffix = AXL_MEM_NEW (CDeclArraySuffix);
	pArraySuffix->m_ElementCount = ElementCount;
	m_SuffixList.InsertTail (pArraySuffix);
	return pArraySuffix;
}

CDeclFormalArgSuffix*
CDeclarator::AddFormalArgSuffix ()
{
	CDeclFormalArgSuffix* pFormalArgSuffix = AXL_MEM_NEW (CDeclFormalArgSuffix);
	m_SuffixList.InsertTail (pFormalArgSuffix);
	return pFormalArgSuffix;
}

rtl::CStdListT <CFunctionFormalArg>*
CDeclarator::GetArgList ()
{
	rtl::CIteratorT <CDeclSuffix> Suffix = m_SuffixList.GetHead ();
	if (Suffix && Suffix->GetSuffixKind () == EDeclSuffix_FormalArg)
	{
		CDeclFormalArgSuffix* pArgSuffix = (CDeclFormalArgSuffix*) *Suffix;
		return &pArgSuffix->m_ArgList;
	}

	return NULL;
}

CType*
CDeclarator::GetType (
	CTypeSpecifierModifiers* pTypeSpecifier,
	CTypeMgr* pTypeMgr
	)
{
	CType* pType = pTypeSpecifier->GetType ();
	int Modifiers = pTypeSpecifier->GetTypeModifiers ();

	if (!pType)
	{
		if (Modifiers & ETypeModifier_Signed |
			Modifiers & ETypeModifier_Unsigned |
			Modifiers & ETypeModifier_BigEndian |
			Modifiers & ETypeModifier_LittleEndian
			)
		{
			pType = pTypeMgr->GetBasicType (EType_Int);
		}
		else
		{
			err::SetFormatStringError (_T("no type specifier in declaration"));
			return NULL;
		}
	}

	if (Modifiers)
	{
		pType = pType->GetTypeMod (Modifiers);
		if (!pType)
			return NULL;
	}

	rtl::CIteratorT <CDeclPointer> Pointer = m_PointerList.GetHead ();
	for (; Pointer; Pointer++)
	{
		EType TypeKind = Pointer->GetTypeKind ();
		int Modifiers = Pointer->GetTypeModifiers ();

		if (TypeKind == EType_Pointer)
		{
			pType = pType->GetPointerType ();
		}
		else 
		{
			ASSERT (TypeKind == EType_Reference);
			pType = pType->GetReferenceType ();
		};

		if (!pType)
			return NULL;

		if (Modifiers)
		{
			pType = pType->GetTypeMod (Modifiers);
			if (!pType)
				return NULL;
		}
	}

	rtl::CIteratorT <CDeclSuffix> Suffix = m_SuffixList.GetTail ();
	for (; Suffix; Suffix--)
	{
		EDeclSuffix SuffixKind = Suffix->GetSuffixKind ();

		if (SuffixKind == EDeclSuffix_Array)
		{
			CDeclArraySuffix* pArraySuffix = (CDeclArraySuffix*) *Suffix;
			pType = pType->GetArrayType (pArraySuffix->m_ElementCount);
			if (!pType)
				return NULL;
		}
		else 
		{
			ASSERT (SuffixKind == EDeclSuffix_FormalArg);
			CDeclFormalArgSuffix* pArgSuffix = (CDeclFormalArgSuffix*) *Suffix;

			int FunctionTypeFlags = pArgSuffix->GetFunctionTypeFlags ();

			size_t Count = pArgSuffix->GetArgCount ();
			if (!Count)
			{
				pType = pTypeMgr->GetFunctionType (pType, NULL, 0, FunctionTypeFlags);
			}		
			else
			{
				rtl::CArrayT <CType*> ArgTypeArray;
				ArgTypeArray.SetCount (Count);

				rtl::CIteratorT <CFunctionFormalArg> Arg = pArgSuffix->GetFirstArg ();
				for (size_t i = 0; Arg; Arg++, i++)
					ArgTypeArray [i] = Arg->GetType ();

				pType = pTypeMgr->GetFunctionType (pType, ArgTypeArray, Count, FunctionTypeFlags);
			}
		}
	}

	return pType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
