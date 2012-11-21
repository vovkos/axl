// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

//.............................................................................

enum EFunctionTypeFlag
{
	EFunctionTypeFlag_IsVarArg       = 0x1000,
	EFunctionTypeFlag_IsUnsafeVarArg = 0x2000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CFunctionType: 
	public CType,
	public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CType* m_pReturnType;
	rtl::CArrayT <CType*> m_ArgTypeArray;

public:
	CFunctionType ();

	llvm::FunctionType* 
	GetLlvmType ();

	CType*
	GetReturnType ()
	{
		return m_pReturnType;
	}

	size_t 
	GetArgCount ()
	{
		return m_ArgTypeArray.GetCount ();
	}

	CType* 
	GetArgType (size_t Index)
	{
		ASSERT (Index < m_ArgTypeArray.GetCount ());
		return m_ArgTypeArray [Index];
	}

	static
	rtl::CStringA
	CreateSignature (
		EType TypeKind,
		CType* pReturnType,
		CType** ppArgType,
		size_t ArgCount,
		int Flags
		);

	static
	rtl::CString
	CreateTypeString (
		EType TypeKind,
		CType* pReturnType,
		CType** ppArgType,
		size_t ArgCount,
		int Flags
		);

	rtl::CString
	CreateTypeString ()
	{
		return CreateTypeString (m_TypeKind, m_pReturnType, m_ArgTypeArray, m_ArgTypeArray.GetCount (), m_Flags);
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
