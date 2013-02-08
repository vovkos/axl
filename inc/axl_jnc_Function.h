// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"
#include "axl_jnc_FunctionTypeOverload.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_BasicBlock.h"
#include "axl_jnc_Scope.h"
#include "axl_jnc_Value.h"
#include "axl_jnc_Closure.h"
#include "axl_llk_Ast.h"

namespace axl {
namespace jnc {

class CClassType;
class CPropertyType;

//.............................................................................

enum EFunction
{
	EFunction_Undefined = 0,	
	EFunction_Named,
	EFunction_Getter,
	EFunction_Setter,
	EFunction_PreConstructor,
	EFunction_Constructor,
	EFunction_StaticConstructor,
	EFunction_Destructor,
	EFunction_CallOperator,
	EFunction_CastOperator,
	EFunction_UnaryOperator,
	EFunction_BinaryOperator,
	EFunction_Internal, 
	EFunction_Thunk,
	EFunction_AutoEv,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetFunctionKindString (EFunction FunctionKind);

//.............................................................................

enum EFunctionFlag
{
	EFunctionFlag_Const = 1,
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

//.............................................................................

class CFunction: public CNamedModuleItem
{
protected:
	friend class CFunctionMgr;
	friend class CClassType;
	friend class CProperty;
	friend class CParser;
	friend class CCast_FunctionPtr;

	EFunction m_FunctionKind;	
	CNamespace* m_pParentNamespace;

	CFunctionType* m_pType;
	CFunctionTypeOverload m_TypeOverload;
	rtl::CArrayT <CFunction*> m_OverloadArray;

	// orphan functions

	CNamespace* m_pOrphanNamespace;
	CQualifiedName m_OrphanName;
	CFunction* m_pExternFunction;

	// for non-static method members

	CClassType* m_pClassType; 

	// for virtual method members

	CClassType* m_pVirtualOriginClassType; 
	size_t m_ClassVTableIndex;

	// for property accessors

	CProperty* m_pProperty;
	size_t m_PropertyVTableIndex;

	rtl::CStdListT <CFunctionFormalArg> m_ArgList;
	rtl::CBoxListT <CToken> m_Body;

	ref::CBufT <llk::CAstT <llk::CAstNodeT <CToken> > > m_Ast;
	CBasicBlock* m_pBlock;
	CScope* m_pScope;
	CVariable* m_pScopeLevelVariable;

	llvm::Function* m_pLlvmFunction;
	void* m_pfn;

public:
	CFunction ();

	EFunction 
	GetFunctionKind ()
	{
		return m_FunctionKind;
	}

	CNamespace* 
	GetParentNamespace ()
	{
		return m_pParentNamespace;
	}

	CFunctionType* 
	GetType ()
	{
		return m_pType;
	}

	CFunctionTypeOverload*
	GetTypeOverload ()
	{	
		return &m_TypeOverload;
	}

	CClassType* 
	GetClassType ()
	{
		return m_pClassType;
	}

	CClassType* 
	GetVirtualOriginClassType ()
	{
		return m_pVirtualOriginClassType;
	}

	size_t
	GetClassVTableIndex ()
	{
		return m_ClassVTableIndex;
	}

	CProperty* 
	GetProperty ()
	{
		return m_pProperty;
	}

	size_t
	GetPropertyVTableIndex ()
	{
		return m_PropertyVTableIndex;
	}

	rtl::CConstListT <CFunctionFormalArg> 
	GetArgList ()
	{
		return m_ArgList;
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

	bool
	IsOverloaded ()
	{
		return !m_OverloadArray.IsEmpty ();
	}

	size_t
	GetOverloadCount ()
	{
		return m_OverloadArray.GetCount () + 1;
	}

	CFunction*
	GetOverload (size_t Overload = 0)
	{
		return 
			Overload == 0 ? this : 
			Overload <= m_OverloadArray.GetCount () ? m_OverloadArray [Overload - 1] : NULL;
	}

	CFunction*
	FindOverload (CFunctionType* pType)
	{
		size_t i = m_TypeOverload.FindOverload (pType);
		return i != -1 ? GetOverload (i) : NULL;
	}

	CFunction*
	FindShortOverload (CFunctionType* pType)
	{
		size_t i = m_TypeOverload.FindShortOverload (pType);
		return i != -1 ? GetOverload (i) : NULL;
	}

	CFunction*
	ChooseOverload (
		const rtl::CArrayT <CType*>& ArgTypeArray,
		ECast* pCastKind = NULL
		)
	{
		return ChooseOverload (ArgTypeArray, ArgTypeArray.GetCount (), pCastKind);
	}

	CFunction*
	ChooseOverload (
		CType* const* ppArgTypeArray,
		size_t Count,
		ECast* pCastKind = NULL
		)
	{
		size_t i = m_TypeOverload.ChooseOverload (ppArgTypeArray, Count, pCastKind);
		return i != -1 ? GetOverload (i) : NULL;
	}

	CFunction*
	ChooseOverload (const rtl::CConstBoxListT <CValue>& ArgList)
	{
		size_t i = m_TypeOverload.ChooseOverload (ArgList);
		return i != -1 ? GetOverload (i) : NULL;
	}

	bool
	AddOverload (CFunction* pFunction);

	bool
	ResolveOrphan ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
