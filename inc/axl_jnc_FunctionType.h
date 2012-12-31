// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_StructType.h"

namespace axl {
namespace jnc {

class CFunctionPointerType;
class CEventType;

//.............................................................................

enum EFunctionTypeFlag
{
	EFunctionTypeFlag_IsVarArg       = 0x010000,
	EFunctionTypeFlag_IsUnsafeVarArg = 0x020000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ECallConv
{
	ECallConv_Default = 0,
	ECallConv_Cdecl,
	ECallConv_Stdcall,
};

const tchar_t*
GetCallingConventionString (ECallConv CallingConvention);

inline
llvm::CallingConv::ID
GetLlvmCallingConvention (ECallConv CallingConvention)
{
	return CallingConvention == ECallConv_Stdcall ?
		llvm::CallingConv::X86_StdCall : 
		llvm::CallingConv::C;
}

//.............................................................................

class CFunctionType: 
	public CType,
	public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CType* m_pReturnType;
	rtl::CArrayT <CType*> m_ArgTypeArray;
	ECallConv m_CallingConvention;
	rtl::CStringA m_ArgSignature;

	CFunctionType* m_pDefCallConvFunctionType;
	CFunctionPointerType* m_pFunctionPointerType;
	CEventType* m_pEventType;

public:
	CFunctionType ();

	llvm::FunctionType* 
	GetLlvmType ();

	llvm::CallingConv::ID
	GetLlvmCallingConvention ()
	{
		return jnc::GetLlvmCallingConvention (m_CallingConvention);
	}

	ECallConv 
	GetCallingConvention ()
	{
		return m_CallingConvention;
	}
	
	CType*
	GetReturnType ()
	{
		return m_pReturnType;
	}

	rtl::CArrayT <CType*>
	GetArgTypeArray ()
	{
		return m_ArgTypeArray;
	}

	rtl::CStringA
	GetArgSignature ();

	CFunctionType*
	GetDefCallConvFunctionType ();

	CFunctionPointerType* 
	GetFunctionPointerType ();

	CEventType*
	GetEventType ();

	static
	rtl::CStringA
	CreateArgSignature (
		CType* const* ppArgType,
		size_t ArgCount,
		int Flags
		);

	static
	rtl::CStringA
	CreateSignature (
		ECallConv CallingConvention,
		CType* pReturnType,
		CType* const* ppArgType,
		size_t ArgCount,
		int Flags
		);

	static
	rtl::CString
	CreateTypeString (
		ECallConv CallingConvention,
		CType* pReturnType,
		CType* const* ppArgType,
		size_t ArgCount,
		int Flags
		);

	rtl::CString
	CreateTypeString ()
	{
		return CreateTypeString (
			m_CallingConvention, 
			m_pReturnType, 
			m_ArgTypeArray, 
			m_ArgTypeArray.GetCount (), 
			m_Flags
			);
	}
};

//.............................................................................

class CFunctionTypeOverload
{
protected:
	friend class CTypeMgr;

	CFunctionType* m_pType;
	rtl::CArrayT <CFunctionType*> m_OverloadArray;

public:
	CFunctionTypeOverload ()
	{
		m_pType = NULL;
	}

	CFunctionTypeOverload (CFunctionType* pType)
	{
		m_pType = pType;
	}

	bool
	IsEmpty () const
	{
		return m_pType == NULL;
	}

	bool 
	IsOverloaded () const
	{
		return m_OverloadArray.IsEmpty ();
	}

	bool
	AddOverload (CFunctionType* pType);

	size_t
	GetOverloadCount () const
	{
		return !IsEmpty () ? m_OverloadArray.GetCount () + 1 : 0;
	}

	CFunctionType*
	GetType (size_t Overload = 0) const
	{
		return 
			Overload == 0 ? m_pType : 
			Overload <= m_OverloadArray.GetCount () ? m_OverloadArray [Overload - 1] : NULL;
	}
};

//.............................................................................

} // namespace axl {
} // namespace jnc {
