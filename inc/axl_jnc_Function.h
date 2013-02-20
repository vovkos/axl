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
#include "axl_jnc_UnOp.h"
#include "axl_jnc_BinOp.h"
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
	EFunction_AutoEv,
	EFunction_Internal, 
	EFunction_Thunk,
	EFunction__Count
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EFunctionKindFlag
{
	EFunctionKindFlag_NoStorage    = 0x01,
	EFunctionKindFlag_NoOverloads  = 0x02,
	EFunctionKindFlag_NoArgs       = 0x04,
}; 

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetFunctionKindString (EFunction FunctionKind);

int
GetFunctionKindFlags (EFunction FunctionKind);

//.............................................................................

class CFunctionFormalArg: public rtl::TListLink
{
protected:
	friend class CFunction;
	friend class CParser;

	rtl::CString m_Name;
	CType* m_pType;
	int m_PtrTypeFlags;

	CValue m_DefaultValue;

public:
	CFunctionFormalArg ()
	{
		m_pType = NULL;
		m_PtrTypeFlags = 0;
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

	int 
	GetPtrTypeFlags ()
	{
		return m_PtrTypeFlags;
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

	union
	{
		EUnOp m_UnOpKind;
		EBinOp m_BinOpKind;
		CType* m_pCastOpType;
	};

	CNamespace* m_pParentNamespace;

	CFunctionType* m_pType;
	CFunctionTypeOverload m_TypeOverload;
	rtl::CArrayT <CFunction*> m_OverloadArray;

	// override & orphan functions

	CNamespace* m_pOrphanNamespace;
	CQualifiedName m_DeclaratorName;
	CFunction* m_pExternFunction;

	// for non-static method members

	CClassType* m_pClassType; 
	CClassPtrType* m_pThisArgType;
	CClassPtrType* m_pThisType;
	intptr_t m_ThisArgDelta;
	int m_ThisArgTypeFlags; 

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

	EUnOp
	GetUnOpKind ()
	{
		ASSERT (m_FunctionKind == EFunction_UnaryOperator);
		return m_UnOpKind;
	}

	EBinOp
	GetBinOpKind ()
	{
		ASSERT (m_FunctionKind == EFunction_BinaryOperator);
		return m_BinOpKind;
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

	const CFunctionTypeOverload*
	GetTypeOverload ()
	{	
		return &m_TypeOverload;
	}

	CClassType* 
	GetClassType ()
	{
		return m_pClassType;
	}

	bool
	IsOrphan ()
	{
		return m_pOrphanNamespace != NULL;
	}

	bool
	IsAccessor ()
	{
		return m_FunctionKind == EFunction_Getter || m_FunctionKind == EFunction_Setter;
	}

	bool
	IsVirtual ()
	{
		return m_StorageKind >= EStorage_Abstract && m_StorageKind <= EStorage_Override;
	}

	CClassType* 
	GetVirtualOriginClassType ()
	{
		return m_pVirtualOriginClassType;
	}

	CClassPtrType* 
	GetThisArgType ()
	{
		return m_pThisArgType;
	}

	CClassPtrType* 
	GetThisType ()
	{
		return m_pThisType;
	}

	intptr_t
	GetThisArgDelta ()
	{
		return m_ThisArgDelta;
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

	void
	ConvertToMethodMember (CClassType* pClassType);

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
	ChooseOverload (
		const rtl::CConstBoxListT <CValue>& ArgList,
		ECast* pCastKind = NULL
		)
	{
		size_t i = m_TypeOverload.ChooseOverload (ArgList, pCastKind);
		return i != -1 ? GetOverload (i) : NULL;
	}

	CFunction*
	ChooseSetterOverload (
		CType* pArgType,
		ECast* pCastKind = NULL
		)
	{
		size_t i = m_TypeOverload.ChooseSetterOverload (pArgType, pCastKind);
		return i != -1 ? GetOverload (i) : NULL;
	}

	CFunction*
	ChooseSetterOverload (
		const CValue& ArgValue,
		ECast* pCastKind = NULL
		)
	{
		size_t i = m_TypeOverload.ChooseSetterOverload (ArgValue, pCastKind);
		return i != -1 ? GetOverload (i) : NULL;
	}

	CFunction*
	ChooseSetterOverload (
		CFunctionType* pFunctionType,
		ECast* pCastKind = NULL
		)
	{
		size_t i = m_TypeOverload.ChooseSetterOverload (pFunctionType, pCastKind);
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
