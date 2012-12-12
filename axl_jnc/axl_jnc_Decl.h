// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Function.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EStorageClass
{
	EStorageClass_Undefined = 0,
	EStorageClass_Static,
	EStorageClass_Typedef,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EAccess
{
	EAccess_Undefined = 0,
	EAccess_Public,
	EAccess_Private,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetStorageClassString (EStorageClass StorageClass);

const tchar_t*
GetAccessString (EAccess Access);

const tchar_t*
GetPropertyAccessorString (EPropertyAccessor PropertyAccessor);

//.............................................................................

class CStorageClassSpecifier
{
protected:
	EStorageClass m_StorageClass;

public:
	CStorageClassSpecifier ()
	{
		m_StorageClass = EStorageClass_Undefined;
	}

	EStorageClass 
	GetStorageClass ()
	{
		return m_StorageClass;
	}

	bool
	SetStorageClass (EStorageClass StorageClass);
};

//.............................................................................

class CAccessSpecifier
{
protected:
	EAccess m_Access;

public:
	CAccessSpecifier ()
	{
		m_Access = EAccess_Undefined;
	}

	EAccess 
	GetAccess ()
	{
		return m_Access;
	}

	bool
	SetAccess (EAccess Access);
};

//.............................................................................

class CTypeSpecifier
{
protected:
	CType* m_pType;

public:
	CTypeSpecifier ()
	{
		m_pType = NULL;
	}

	CType* 
	GetType ()
	{
		return m_pType;
	}

	bool
	SetType (CType* pType);
};

//.............................................................................

class CTypeModifiers
{
protected:
	int m_TypeModifiers;

public:
	CTypeModifiers ()
	{
		m_TypeModifiers = 0;
	}

	int 
	GetTypeModifiers ()
	{
		return m_TypeModifiers;
	}

	bool
	SetTypeModifier (ETypeModifier Modifier);
};

//.............................................................................

class CTypeSpecifierModifiers:
	public CTypeSpecifier,
	public CTypeModifiers
{
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CDeclSpecifiers: 
	public CAccessSpecifier,
	public CStorageClassSpecifier,
	public CTypeSpecifierModifiers
{
};

//.............................................................................

class CDeclPointer: 
	public CTypeModifiers,
	public rtl::TListLink
{
protected:
	friend class CDeclarator;

	EType m_TypeKind;

public:
	CDeclPointer ()
	{
		m_TypeKind = EType_Pointer;
	}

	EType
	GetTypeKind ()
	{
		return m_TypeKind;
	}
};

//.............................................................................

enum EDeclSuffix
{
	EDeclSuffix_Undefined = 0,
	EDeclSuffix_Array,
	EDeclSuffix_FormalArg,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CDeclSuffix: public rtl::TListLink
{
protected:
	friend class CDeclarator;

	EDeclSuffix m_SuffixKind;

public:
	CDeclSuffix ()
	{
		m_SuffixKind = EDeclSuffix_Undefined;
	}

	virtual
	~CDeclSuffix ()
	{
	}

	EDeclSuffix
	GetSuffixKind ()
	{
		return m_SuffixKind;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CDeclArraySuffix: public CDeclSuffix
{
protected:
	friend class CDeclarator;

	size_t m_ElementCount;

public:
	CDeclArraySuffix ()
	{
		m_SuffixKind = EDeclSuffix_Array;
		m_ElementCount = 0;
	}

	size_t
	GetElementCount ()
	{
		return m_ElementCount;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CDeclFunctionSuffix: public CDeclSuffix
{
protected:
	friend class CDeclarator;
	friend class CParser;

	rtl::CStdListT <CFunctionFormalArg> m_ArgList;
	int m_FunctionTypeFlags;

public:
	CDeclFunctionSuffix ()
	{
		m_SuffixKind = EDeclSuffix_FormalArg;
		m_FunctionTypeFlags = 0;
	}

	size_t 
	GetArgCount ()
	{
		return m_ArgList.GetCount ();
	}

	rtl::CIteratorT <CFunctionFormalArg> 
	GetFirstArg ()
	{
		return m_ArgList.GetHead ();
	}
	
	int 
	GetFunctionTypeFlags ()
	{
		return m_FunctionTypeFlags;
	}
};

//.............................................................................

class CDeclarator
{
protected:
	friend class CParser;

	CToken::CPos m_Pos;
	CQualifiedName m_Name;
	EPropertyAccessor m_PropertyAccessorKind;

	rtl::CStdListT <CDeclPointer> m_PointerList;
	rtl::CStdListT <CDeclSuffix> m_SuffixList;

public:
	CDeclarator ()
	{
		m_PropertyAccessorKind = EPropertyAccessor_Undefined;
	}

	bool
	IsSimple ()
	{
		return !m_PropertyAccessorKind && m_Name.IsSimple ();
	}

	const CQualifiedName*
	GetName ()
	{
		return &m_Name;
	}

	EPropertyAccessor 
	GetPropertyAccessorKind ()
	{
		return m_PropertyAccessorKind;
	}

	bool
	AddName (const rtl::CString Name);

	bool
	AddPropertyAccessorKind (EPropertyAccessor AccessorKind);

	CDeclPointer*
	AddPointer (EType TypeKind);

	CDeclArraySuffix*
	AddArraySuffix (size_t ElementCount);

	CDeclFunctionSuffix*
	AddFormalArgSuffix ();

	static
	CType*
	GetType_s (CTypeSpecifierModifiers* pTypeSpecifier);

	CType*
	GetType (CTypeSpecifierModifiers* pTypeSpecifier);

	rtl::CStdListT <CFunctionFormalArg>*
	GetArgList ();
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
