// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_BasicBlock.h"
#include "axl_jnc_Scope.h"
#include "axl_jnc_Value.h"
#include "axl_jnc_Closure.h"
#include "axl_llk_Ast.h"

namespace axl {
namespace jnc {

class CClassType;
class CVTableType;
class CClassMethodMember;

//.............................................................................

enum EFunction
{
	EFunction_Undefined = 0,
	EFunction_Global,
	EFunction_Method,
	EFunction_PreConstructor,
	EFunction_Constructor,
	EFunction_Destructor,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EPropertyAccessor
{
	EPropertyAccessor_Undefined = 0,
	EPropertyAccessor_Get,
	EPropertyAccessor_Set,
};

//.............................................................................

class CFunctionFormalArg: public rtl::TListLink
{
protected:
	friend class CParser;
	friend class CClassType;

	rtl::CString m_Name;
	CType* m_pType;
	CValue m_DefaultValue;

public:
	CFunctionFormalArg ()
	{
		m_pType = NULL;
	}

	rtl::CString 
	GetName ()
	{
		return m_Name;
	}

	CType* 
	GetType ()
	{
		return m_pType;
	}

	const CValue&
	GetDefaultValue ()
	{
		return m_DefaultValue;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CFunction: 
	public CModuleItem,
	public rtl::TListLink
{
protected:
	friend class CFunctionMgr;
	friend class CClassType;
	friend class CVTableType;
	friend class CPropertyType;
	friend class CParser;
	friend class CCast_fn;

	EFunction m_FunctionKind;
	EPropertyAccessor m_PropertyAccessorKind;
	
	CFunctionType* m_pType;
	CFunctionType* m_pShortType;
	CNamespace* m_pNamespace;

	CNamespace* m_pAnchorNamespace;
	CQualifiedName m_Name;
	rtl::CString m_Tag;

	CClassType* m_pClassType;
	CVTableType* m_pVTableType;
	size_t m_VTableIndex;

	rtl::CStdListT <CFunctionFormalArg> m_ArgList;
	rtl::CBoxListT <CToken> m_Body;

	ref::CBufT <llk::CAstT <llk::CAstNodeT <CToken> > > m_Ast;
	CBasicBlock* m_pBlock;
	CScope* m_pScope;
	CVariable* m_pScopeLevelVariable;

	CFunction* m_pExternFunction;
	llvm::Function* m_pLlvmFunction;
	void* m_pfn;

public:
	CFunction ();

	EFunction
	GetFunctionKind ()
	{
		return m_FunctionKind;
	}

	EPropertyAccessor
	GetPropertyAccessorKind ()
	{
		return m_PropertyAccessorKind;
	}

	const CQualifiedName&
	GetQualifiedName ()
	{
		return m_Name;
	}

	rtl::CString 
	GetTag ()
	{
		return m_Tag;
	}
	
	CFunctionType* 
	GetType ()
	{
		return m_pType;
	}

	CFunctionType* 
	GetShortType ()
	{
		return m_pShortType ? m_pShortType : m_pType;
	}

	CNamespace* 
	GetNamespace ()
	{
		return m_pNamespace;
	}

	CClassType* 
	GetClassType ()
	{
		return m_pClassType;
	}

	CVTableType* 
	GetVTableType ()
	{
		return m_pVTableType;
	}

	CClassType* 
	GetVTableClassType ();

	size_t
	GetVTableIndex ()
	{
		return m_VTableIndex;
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

	rtl::CString
	CreateArgString ();

	bool
	IsDefined ()
	{
		return HasBody () || m_pExternFunction && m_pExternFunction->IsDefined ();
	}

	bool
	HasBody ()
	{
		return !m_Body.IsEmpty ();
	}

	EHasReturn
	HasReturn ()
	{
		return m_pBlock ? m_pBlock->HasReturn () : EHasReturn_Undefined;
	}

	const rtl::CBoxListT <CToken>*
	GetBody ()
	{
		return &m_Body;
	}

	void
	SetBody (rtl::CBoxListT <CToken>* pTokenList)
	{
		m_Body.TakeOver (pTokenList);
	}

	CScope*
	GetScope ()
	{
		return m_pScope;
	}

	CBasicBlock*
	GetBlock ()
	{
		return m_pBlock;
	}

	CVariable* 
	GetScopeLevelVariable ()
	{
		return m_pScopeLevelVariable;
	}

	CFunction*
	GetExternFunction ()
	{
		return m_pExternFunction;
	}

	llvm::Function* 
	GetLlvmFunction ();
	
	void*
	GetFunctionPointer ()
	{
		return m_pfn;
	}

	ref::CBufT <llk::CAstT <llk::CAstNodeT <CToken> > > 
	GetAst ()
	{
		return m_Ast;
	}
};

//.............................................................................

class CFunctionOverload
{
protected:
	friend class CFunctionMgr;

	CFunction* m_pFunction;
	rtl::CArrayT <CFunction*> m_OverloadArray;

public:
	CFunctionOverload ()
	{
		m_pFunction = NULL;
	}

	bool 
	IsEmpty () const
	{
		return m_pFunction == NULL;
	}

	bool
	IsOverloaded () const
	{
		return !m_OverloadArray.IsEmpty ();
	}

	size_t
	GetOverloadCount () const
	{
		return !IsEmpty () ? m_OverloadArray.GetCount () + 1 : 0;
	}

	CFunction*
	GetFunction (size_t Overload = 0) const
	{
		return 
			Overload == 0 ? m_pFunction : 
			Overload <= m_OverloadArray.GetCount () ? m_OverloadArray [Overload - 1] : NULL;
	}

	bool
	AddOverload (CFunction* pFunction);
	
	CFunction*
	ChooseOverload (rtl::CBoxListT <CValue>* pArgList) const;

	CFunction*
	FindOverload (CFunctionType* pType) const;

	CFunction*
	FindShortOverload (CFunctionType* pType) const;
};

//.............................................................................

class CGlobalFunction:
	public CModuleItem,
	public CFunctionOverload,
	public CName,
	public rtl::TListLink
{
protected:
	friend class CFunctionMgr;

public:
	CGlobalFunction ()
	{
		m_ItemKind = EModuleItem_GlobalFunction;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
