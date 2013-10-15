// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_FunctionArg.h"

namespace axl {
namespace jnc {

class CFunctionPtrType;
class CNamedType;
class CClassType;
class CClassPtrType;
class CReactorClassType;
class CFunction;

struct TFunctionPtrTypeTuple;

//.............................................................................

enum EFunctionTypeFlag
{
	EFunctionTypeFlag_VarArg       = 0x010000,
	EFunctionTypeFlag_UnsafeVarArg = 0x020000,
	EFunctionTypeFlag_Unwinder     = 0x040000,
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

const char*
GetCallConvString (ECallConv CallConv);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ECallConv
GetCallConvFromModifiers (uint_t Modifiers)
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

enum EFunctionPtrType
{
	EFunctionPtrType_Normal = 0,
	EFunctionPtrType_Weak,
	EFunctionPtrType_Thin,
	EFunctionPtrType__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetFunctionPtrTypeKindString (EFunctionPtrType PtrTypeKind);

//.............................................................................

class CFunctionType: public CType
{
	friend class CTypeMgr;
	friend class CClassType;

protected:
	CType* m_pReturnType;
	CImportType* m_pReturnType_i;
	rtl::CArrayT <CFunctionArg*> m_ArgArray;
	ECallConv m_CallConv;
	rtl::CString m_ArgSignature;
	rtl::CString m_TypeModifierString;
	rtl::CString m_ArgString;
	CFunctionType* m_pShortType;
	CFunctionType* m_pStdObjectMemberMethodType;
	CFunction* m_pAbstractFunction;
	TFunctionPtrTypeTuple* m_pFunctionPtrTypeTuple;
	CReactorClassType* m_pReactorInterfaceType;

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

	CImportType*
	GetReturnType_i ()
	{
		return m_pReturnType_i;
	}

	rtl::CArrayT <CFunctionArg*> 
	GetArgArray ()
	{
		return m_ArgArray;
	}

	rtl::CString
	GetArgSignature ();

	rtl::CString 
	GetArgString ();

	rtl::CString 
	GetTypeModifierString ();

	bool
	IsMemberMethodType ()
	{
		return !m_ArgArray.IsEmpty () && m_ArgArray [0]->GetStorageKind () == EStorage_This;
	}

	CType* 
	GetThisArgType ()
	{
		return IsMemberMethodType () ? m_ArgArray [0]->GetType () : NULL;
	}

	CNamedType*
	GetThisTargetType ();

	CFunctionType*
	GetShortType ()
	{
		return m_pShortType;
	}
	
	CFunctionType*
	GetMemberMethodType (
		CNamedType* pType, 
		uint_t ThisArgFlags = 0
		);

	CFunctionType*
	GetStdObjectMemberMethodType ();

	CFunction*
	GetAbstractFunction ();

	CFunctionPtrType* 
	GetFunctionPtrType (
		EType TypeKind,
		EFunctionPtrType PtrTypeKind = EFunctionPtrType_Normal,
		uint_t Flags = 0
		);

	CFunctionPtrType* 
	GetFunctionPtrType (
		EFunctionPtrType PtrTypeKind = EFunctionPtrType_Normal,
		uint_t Flags = 0
		)
	{
		return GetFunctionPtrType (EType_FunctionPtr, PtrTypeKind, Flags);
	}

	CClassType*
	GetMulticastType ();

	static
	rtl::CString 
	CreateCallConvSignature (ECallConv CallConv);

	static
	rtl::CString
	CreateSignature (
		ECallConv CallConv,
		CType* pReturnType,
		CType* const* pArgTypeArray,
		size_t ArgCount,
		uint_t Flags
		);

	static
	rtl::CString
	CreateSignature (
		ECallConv CallConv,
		CType* pReturnType,
		CFunctionArg* const* pArgArray,
		size_t ArgCount,
		uint_t Flags
		);

	static
	rtl::CString
	CreateArgSignature (
		CType* const* pArgTypeArray,
		size_t ArgCount,
		uint_t Flags
		);

	static
	rtl::CString
	CreateArgSignature (
		CFunctionArg* const* pArgArray,
		size_t ArgCount,
		uint_t Flags
		);

	rtl::CString
	CreateArgSignature ()
	{
		return CreateArgSignature (m_ArgArray, m_ArgArray.GetCount (), m_Flags);
	}

	virtual 
	bool
	Compile ();

protected:
	virtual 
	void
	PrepareTypeString ();

	virtual 
	void
	PrepareLlvmType ();

	virtual 
	void
	PrepareLlvmDiType ();

	virtual 
	bool
	CalcLayout ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
