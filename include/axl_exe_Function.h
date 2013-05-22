// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_FUNCTION_H

#include "axl_exe_ArgPacker.h"
#include "axl_exe_InvokeDirect.h"
#include "axl_rtl_Singleton.h"

namespace axl {
namespace exe {

//.............................................................................

struct IFunction: obj::IRoot
{
	// {99E5B3C8-DEFF-42D3-80E0-33D5FED88025}
	AXL_OBJ_INTERFACE (
		IFunction,
		0x99e5b3c8, 0xdeff, 0x42d3, 0x80, 0xe0, 0x33, 0xd5, 0xfe, 0xd8, 0x80, 0x25
		)

	virtual 
	IArgPacker*
	GetArgPacker () = 0;

	virtual 
	intptr_t 
	InvokeV (axl_va_list va) = 0;

	intptr_t 
	Invoke (
		int Unused,
		...
		)
	{
		AXL_VA_DECL (va, Unused);
		return InvokeV (va);
	}
};

//.............................................................................

template <
	typename TCtxArg,
	typename TParArg
	>
class CFunctionT: public IFunction
{
public:
	AXL_OBJ_CLASS_0 (CFunctionT, IFunction)

protected:
	void* m_pf;
	ECallConv m_Convention;
	ref::CPtrT <CArgBlock> m_Context;

public:
	CFunctionT ()
	{
		m_pf = NULL;
		m_Convention = ECallConv_Cdecl;
	}

	CFunctionT (
		ECallConv Convention,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		SetupV (Convention, pf, va);
	}

	CFunctionT (
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		SetupV (pf, va);
	}

	void
	SetupV (
		ECallConv Convention,
		void* pf,
		axl_va_list va
		)
	{
		m_Convention = Convention;
		m_pf = pf;
		m_Context = CreateArgBlockV <TCtxArg> (va);
	}

	void
	SetupV (
		void* pf,
		axl_va_list va
		)
	{
		SetupV (ECallConv_Cdecl, pf, va);
	}

	void
	Setup (
		ECallConv Convention,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		SetupV (Convention, pf, va);
	}

	void
	Setup (
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		SetupV (pf, va);
	}

	virtual 
	IArgPacker* 
	GetArgPacker ()
	{
		return IArgPackerImplT <TParArg>::GetSingleton ();
	}

	virtual 
	intptr_t 
	InvokeV (axl_va_list va)
	{
		size_t Size;
		TParArg () (NULL, &Size, NULL, va);

		mem::TBlock Stack [2];

		if (m_Context)
			Stack [0] = *m_Context;

		Stack [1].Setup (va.m_va, Size);

		return exe::InvokeDirect (m_pf, m_Convention, Stack, 2);
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
