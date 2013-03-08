// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

class CFunctionPtrType;
class CFunctionPtrTypeTuple;
class CMulticastType;
class CClassType;
class CClassPtrType;
enum EFunctionPtrType;

//.............................................................................

enum EFunctionTypeFlag
{
	EFunctionTypeFlag_VarArg       = 0x010000,
	EFunctionTypeFlag_UnsafeVarArg = 0x020000,
};

//.............................................................................

enum ECallConv
{
	ECallConv_Cdecl = 0,
	ECallConv_Stdcall,
	ECallConv__Count,

	ECallConv_Default = ECallConv_Cdecl,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const tchar_t*
GetCallConvString (ECallConv CallConv);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ECallConv
GetCallConvFromModifiers (int Modifiers)
{
	return 
		(Modifiers & ETypeModifier_Cdecl) ? ECallConv_Cdecl :
		(Modifiers & ETypeModifier_Stdcall) ? ECallConv_Stdcall : ECallConv_Default;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
llvm::CallingConv::ID
GetLlvmCallConv (ECallConv CallConv)
{
	return CallConv == ECallConv_Stdcall ?
		llvm::CallingConv::X86_StdCall : 
		llvm::CallingConv::C;
}

//.............................................................................

class CFunctionType: public CType
{
protected:
	friend class CTypeMgr;

	CType* m_pReturnType;
	rtl::CArrayT <CType*> m_ArgTypeArray;
	ECallConv m_CallConv;
	rtl::CStringA m_ArgSignature;
	rtl::CString m_ArgTypeString;
	CFunctionType* m_pShortType;
	CFunctionType* m_pStdObjectMethodMemberType;
	CFunction* m_pAbstractFunction;
	CFunctionPtrTypeTuple* m_pFunctionPtrTypeTuple;

public:
	CFunctionType ();

	llvm::CallingConv::ID
	GetLlvmCallConv ()
	{
		return jnc::GetLlvmCallConv (m_CallConv);
	}

	ECallConv 
	GetCallConv ()
	{
		return m_CallConv;
	}
	
	CType*
	GetReturnType ()
	{
		return m_pReturnType;
	}

	bool
	HasArgs ()
	{
		return !m_ArgTypeArray.IsEmpty ();
	}

	rtl::CArrayT <CType*>
	GetArgTypeArray ()
	{
		return m_ArgTypeArray;
	}

	rtl::CStringA
	GetArgSignature ();

	rtl::CString 
	GetArgTypeString ();

	bool
	IsMethodMemberType ()
	{
		return !m_ArgTypeArray.IsEmpty () && m_ArgTypeArray [0]->GetTypeKind () == EType_ClassPtr;
	}

	CClassPtrType* 
	GetThisArgType ()
	{
		return !m_ArgTypeArray.IsEmpty () && m_ArgTypeArray [0]->GetTypeKind () == EType_ClassPtr ? 
			(CClassPtrType*) m_ArgTypeArray [0] : NULL;
	}

	CFunctionType*
	GetShortType ();
	
	CFunctionType*
	GetMethodMemberType (
		CClassType* pType, 
		int ThisArgFlags = 0
		);

	CFunctionType*
	GetStdObjectMethodMemberType ();

	CFunction*
	GetAbstractFunction ();

	CFunctionPtrType* 
	GetFunctionPtrType (
		EType TypeKind,
		EFunctionPtrType PtrTypeKind = (EFunctionPtrType) 0,
		int Flags = 0
		);

	CFunctionPtrType* 
	GetFunctionPtrType (
		EFunctionPtrType PtrTypeKind = (EFunctionPtrType) 0,
		int Flags = 0
		)
	{
		return GetFunctionPtrType (EType_FunctionPtr, PtrTypeKind, Flags);
	}

	CMulticastType*
	GetMulticastType ();

	static
	rtl::CStringA
	CreateSignature (
		ECallConv CallConv,
		CType* pReturnType,
		CType* const* ppArgType,
		size_t ArgCount,
		int Flags
		);

	static
	rtl::CStringA
	CreateArgSignature (
		CType* const* ppArgType,
		size_t ArgCount,
		int Flags
		);

	rtl::CStringA
	CreateArgSignature ()
	{
		return CreateArgSignature (
			m_ArgTypeArray, 
			m_ArgTypeArray.GetCount (), 
			m_Flags
			);
	}

protected:
	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
