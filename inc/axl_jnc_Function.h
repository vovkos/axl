// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"
#include "axl_jnc_PropertyType.h"
#include "axl_jnc_Namespace.h"
#include "axl_jnc_Value.h"
#include "axl_llk_Ast.h"

namespace axl {
namespace jnc {

class CBasicBlock;
class CFunctionOverload;

//.............................................................................

enum EFunction
{
	EFunction_Global,
	EFunction_Method,
	EFunction_Constructor,
	EFunction_Destructor,
	EFunction_PropertyGetter,
	EFunction_PropertySetter,
};

//.............................................................................

class CFunctionFormalArg: public rtl::TListLink
{
protected:
	friend class CFunctionMgr;
	friend class CDeclFunctionSuffix;

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
	friend class CFunctionOverload;

	EFunction m_FunctionKind;
	CFunctionType* m_pType;
	CFunctionOverload* m_pOverload;
	rtl::CString m_Tag;
	rtl::CStdListT <CFunctionFormalArg> m_ArgList;
	rtl::CBoxListT <CToken> m_Body;

	ref::CBufT <llk::CAstT <llk::CAstNodeT <CToken> > > m_Ast;
	CBasicBlock* m_pBlock;
	CScope* m_pScope;

	llvm::Function* m_pLlvmFunction;

public:
	CFunction ();

	rtl::CString 
	GetTag ()
	{
		return m_Tag;
	}

	llvm::Function* 
	GetLlvmFunction ();
	
	EFunction
	GetFunctionKind ()
	{
		return m_FunctionKind;
	}

	CFunctionType* 
	GetType ()
	{
		return m_pType;
	}

	CFunctionOverload*
	GetOverload ()
	{
		return m_pOverload;
	};

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
	HasBody ()
	{
		return !m_Body.IsEmpty ();
	}

	void
	SetBody (rtl::CBoxListT <CToken>* pTokenList)
	{
		m_Body.TakeOver (pTokenList);
	}

	rtl::CBoxIteratorT <CToken>
	GetBodyFirstToken ()
	{
		return m_Body.GetHead ();
	}

	rtl::CBoxIteratorT <CToken>
	GetBodyLastToken ()
	{
		return m_Body.GetTail ();
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
	FindOverload (rtl::CBoxListT <CValue>* pArgList) const;
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
