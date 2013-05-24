// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_FunctionArg.h"

namespace axl {
namespace jnc {

class CFunctionPtrType;
class CFunctionPtrTypeTuple;
class CMulticastType;
class CNamedType;
class CClassType;
class CClassPtrType;
class CFunction;

//.............................................................................

enum EFunctionTypeFlag
{
	EFunctionTypeFlag_User         = 0x010000,
	EFunctionTypeFlag_VarArg       = 0x020000,
	EFunctionTypeFlag_UnsafeVarArg = 0x040000,
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
	EFunctionPtrType_Unsafe,
	EFunctionPtrType__Count,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetFunctionPtrTypeKindString (EFunctionPtrType PtrTypeKind);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
EFunctionPtrType 
GetFunctionPtrTypeKindFromModifiers (uint_t Modifiers)
{
	return 
		(Modifiers & ETypeModifier_Unsafe) ? EFunctionPtrType_Unsafe :
		(Modifiers & ETypeModifier_Weak) ? EFunctionPtrType_Weak : 
		(Modifiers & ETypeModifier_Thin) ? EFunctionPtrType_Thin : EFunctionPtrType_Normal;
}

//.............................................................................

class CFunctionType: public CType
{
	friend class CTypeMgr;

protected:
	CType* m_pReturnType;
	rtl::CArrayT <CFunctionArg*> m_ArgArray;
	ECallConv m_CallConv;
	rtl::CString m_ArgSignature;
	rtl::CString m_ArgString;
	CFunctionType* m_pShortType;
	CFunctionType* m_pStdObjectMemberMethodType;
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

	rtl::CArrayT <CFunctionArg*> 
	GetArgArray ()
	{
		return m_ArgArray;
	}

	rtl::CString
	GetArgSignature ();

	rtl::CString 
	GetArgString ();

	bool
	IsMemberMethodType ()
	{
		return !m_ArgArray.IsEmpty () && (m_ArgArray [0]->GetPtrTypeFlags () & EPtrTypeFlag_This) != 0;
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
		int ThisArgFlags = 0
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

	CMulticastType*
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
