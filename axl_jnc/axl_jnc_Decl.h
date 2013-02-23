// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Function.h"
#include "axl_jnc_UnOp.h"
#include "axl_jnc_BinOp.h"

namespace axl {
namespace jnc {

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

	int
	ClearTypeModifiers (int ModifierMask);

protected:
	bool
	CheckAntiTypeModifiers (int ModifierMask);
};

//.............................................................................

class CTypeSpecifier: public CTypeModifiers
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

enum EDeclPrefix
{
	EDeclPrefix_Undefined = 0,
	EDeclPrefix_Pointer,
	EDeclPrefix_Multicast,
	EDeclPrefix_Event,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TDeclPrefix
{
	EDeclPrefix m_PrefixKind;
	int m_TypeModifiers;
};

//.............................................................................

enum EDeclSuffix
{
	EDeclSuffix_Undefined = 0,
	EDeclSuffix_Array,
	EDeclSuffix_Function,
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
		m_SuffixKind = EDeclSuffix_Function;
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

	rtl::CArrayT <CType*>
	GetArgTypeArray ();
};

//.............................................................................

enum EPostDeclaratorModifier
{
	EPostDeclaratorModifier_Const = 1
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EPostDeclaratorModifier
GetFirstPostDeclaratorModifier (int Modifiers)
{
	return (EPostDeclaratorModifier) (1 << rtl::GetLoBitIdx (Modifiers));
}

const tchar_t* 
GetPostDeclaratorModifierString (EPostDeclaratorModifier Modifier);

inline
const tchar_t* 
GetPostDeclaratorModifierString (int Modifiers)
{
	return GetPostDeclaratorModifierString (GetFirstPostDeclaratorModifier (Modifiers));
}

//.............................................................................

enum EDeclarator
{
	EDeclarator_Undefined,
	EDeclarator_SimpleName,
	EDeclarator_QualifiedName,
	EDeclarator_UnnamedMethod,
	EDeclarator_CastOperator,
	EDeclarator_UnaryBinaryOperator,
	EDeclarator_PropValue,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CDeclarator: public CTypeModifiers
{
protected:
	friend class CParser;

	EDeclarator m_DeclaratorKind;
	EFunction m_FunctionKind;
	EUnOp m_UnOpKind;
	EBinOp m_BinOpKind;
	CType* m_pCastOpType;
	CQualifiedName m_Name;
	CToken::CPos m_Pos;
	size_t m_BitCount;
	int m_PostDeclaratorModifiers;
	CType* m_pBaseType;

	rtl::CArrayT <TDeclPrefix> m_PrefixArray;
	rtl::CStdListT <CDeclSuffix> m_SuffixList;

public:
	CDeclarator ();

	EDeclarator
	GetDeclaratorKind ()
	{
		return m_DeclaratorKind;
	}

	EFunction
	GetFunctionKind ()
	{
		return m_FunctionKind;
	}

	EUnOp
	GetUnOpKind ()
	{
		return m_UnOpKind;
	}

	EBinOp
	GetBinOpKind ()
	{
		return m_BinOpKind;
	}

	CType*
	GetCastOpType ()
	{
		return m_pCastOpType;
	}

	void
	SetTypeSpecifier (CTypeSpecifier* pTypeSpecifier);

	const CQualifiedName*
	GetName ()
	{
		return &m_Name;
	}

	const CToken::CPos&
	GetPos ()
	{
		return m_Pos;
	}

	size_t 
	GetBitCount ()
	{
		return m_BitCount;
	}

	int 
	GetPostDeclaratorModifiers ()
	{
		return m_PostDeclaratorModifiers;
	}

	CType* 
	GetBaseType ()
	{
		return m_pBaseType;
	}

	rtl::CArrayT <TDeclPrefix> 
	GetPrefixArray ()
	{
		return m_PrefixArray;
	}

	rtl::CConstListT <CDeclSuffix> 
	GetSuffixList ()
	{
		return m_SuffixList;
	}

	bool
	SetPostDeclaratorModifier (EPostDeclaratorModifier Modifier);

	rtl::CStdListT <CFunctionFormalArg>*
	GetArgList ();
	
	CType*
	CalcType (int* pDataPtrTypeFlags = NULL);

	bool
	AddName (rtl::CString Name);

	bool
	AddUnnamedMethod (EFunction FunctionKind);

	bool
	AddCastOperator (CType* pType);

	bool
	AddUnaryBinaryOperator (
		EUnOp UnOpKind, 
		EBinOp BinOpKind
		);

	bool
	SetPropValue ();

	bool
	AddPrefix (EDeclPrefix PrefixKind);

	CDeclArraySuffix*
	AddArraySuffix (size_t ElementCount);

	CDeclFunctionSuffix*
	AddFunctionSuffix ();

	bool
	AddBitFieldSuffix (size_t BitCount);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {


