// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionType.h"
#include "axl_jnc_FunctionTypeOverload.h"
#include "axl_jnc_NamedType.h"
#include "axl_jnc_BasicBlock.h"
#include "axl_jnc_Scope.h"
#include "axl_jnc_Value.h"
#include "axl_jnc_Closure.h"
#include "axl_jnc_UnOp.h"
#include "axl_jnc_BinOp.h"
#include "axl_jnc_Variable.h"

namespace axl {
namespace jnc {

class CDerivableType;
class CClassType;
class CPropertyType;
class CAutoEvClassType;
class CProperty;
class CJitEventListener;

//.............................................................................

enum EFunction
{
	EFunction_Undefined = 0,
	EFunction_Named,
	EFunction_Getter,
	EFunction_Setter,
	EFunction_Binder,
	EFunction_Primer,
	EFunction_PreConstructor,
	EFunction_Constructor,
	EFunction_Destructor,
	EFunction_StaticConstructor,
	EFunction_StaticDestructor,
	EFunction_ModuleConstructor,
	EFunction_ModuleDestructor,
	EFunction_CallOperator,
	EFunction_CastOperator,
	EFunction_UnaryOperator,
	EFunction_BinaryOperator,
	EFunction_Internal, 
	EFunction_Thunk,
	EFunction_AutoEvHandler,
	EFunction_ScheduleLauncher,
	EFunction__Count
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EFunctionKindFlag
{
	EFunctionKindFlag_NoStorage   = 0x01,
	EFunctionKindFlag_NoOverloads = 0x02,
	EFunctionKindFlag_NoArgs      = 0x04,
}; 

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetFunctionKindString (EFunction FunctionKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int
GetFunctionKindFlags (EFunction FunctionKind);

//.............................................................................

class CFunction: public CUserModuleItem
{
	friend class CModule;
	friend class CFunctionMgr;
	friend class CTypeMgr;
	friend class CDerivableType;
	friend class CClassType;
	friend class CProperty;
	friend class CParser;
	friend class CCast_FunctionPtr;
	friend class CJitEventListener;

protected:
	EFunction m_FunctionKind;	

	union
	{
		EUnOp m_UnOpKind;
		EBinOp m_BinOpKind;
		CType* m_pCastOpType;
	};

	CFunctionType* m_pType;
	CFunctionTypeOverload m_TypeOverload;
	rtl::CArrayT <CFunction*> m_OverloadArray;

	// override & orphan functions

	CQualifiedName m_DeclaratorName;

	// for non-static member methods

	CType* m_pThisArgType;
	CType* m_pThisType;
	intptr_t m_ThisArgDelta;
	uint_t m_ThisArgTypeFlags; 

	// for virtual member methods

	CClassType* m_pVirtualOriginClassType; 
	size_t m_ClassVTableIndex;

	// for property gettes/setters

	CProperty* m_pProperty;
	size_t m_PropertyVTableIndex;

	rtl::CBoxListT <CToken> m_Body;

	CBasicBlock* m_pEntryBlock;
	CScope* m_pScope;

	llvm::Function* m_pLlvmFunction;
	llvm::Instruction* m_pLlvmPostTlsPrologueInst;
	
	rtl::CArrayT <TTlsVariable> m_TlsVariableArray;

	// native machine code

	void* m_pfMachineCode;
	size_t m_MachineCodeSize;

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

	bool
	IsAccessor ()
	{
		return m_FunctionKind == EFunction_Getter || m_FunctionKind == EFunction_Setter;
	}

	bool
	IsMember ()
	{
		return m_pThisType != NULL;
	}

	bool
	IsVirtual ()
	{
		return m_StorageKind >= EStorage_Abstract && m_StorageKind <= EStorage_Override;
	}

	bool
	NeedsVTablePtrCut ()
	{
		return 
			(m_FunctionKind == EFunction_PreConstructor ||
			m_FunctionKind == EFunction_Constructor ||
			m_FunctionKind == EFunction_Destructor) &&
			m_pThisType && 
			m_pThisType->GetTypeKind () == EType_ClassPtr;
	}

	CClassType* 
	GetVirtualOriginClassType ()
	{
		return m_pVirtualOriginClassType;
	}

	CDerivableType*
	GetParentType ()
	{
		return m_pParentNamespace->GetNamespaceKind () == ENamespace_Type ? 
			(CDerivableType*) (CNamedType*) m_pParentNamespace : NULL;
	}

	CType* 
	GetThisArgType ()
	{
		return m_pThisArgType;
	}

	CType* 
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

	void
	ConvertToMemberMethod (CNamedType* pParentType);

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

	bool
	SetBody (rtl::CBoxListT <CToken>* pTokenList);

	CScope*
	GetScope ()
	{
		return m_pScope;
	}

	CBasicBlock*
	GetEntryBlock ()
	{
		return m_pEntryBlock;
	}

	llvm::Function* 
	GetLlvmFunction ();
	
	llvm::Instruction* 
	GetLlvmPostTlsPrologueInst ()
	{
		return m_pLlvmPostTlsPrologueInst;
	}

	void*
	GetMachineCode ()
	{
		return m_pfMachineCode;
	}

	size_t 
	GetMachineCodeSize ()
	{
		return m_MachineCodeSize;
	}

	rtl::CArrayT <TTlsVariable> 
	GetTlsVariableArray ()
	{
		return m_TlsVariableArray;
	}

	void 
	AddTlsVariable (CVariable* pVariable);

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
		CFunctionArg* const* pArgArray,
		size_t ArgCount,
		ECast* pCastKind = NULL
		)
	{
		size_t i = m_TypeOverload.ChooseOverload (pArgArray, ArgCount, pCastKind);
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

	virtual
	bool
	Compile ();

protected:
	virtual
	bool
	CalcLayout ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
