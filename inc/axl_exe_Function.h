// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_FUNCTION_H

#include "axl_exe_ArgPacker.h"
#include "axl_exe_InvokeDirect.h"
#include "axl_rtl_Singleton.h"

namespace axl {
namespace exe {

//.............................................................................

[uuid ("a3a5b378-8b60-47c2-bc50-c6bfe35da012")]
struct IFunction: obj::IRoot
{
	virtual 
	IArgPacker*
	GetArgPacker () = 0;

	virtual 
	intptr_t 
	InvokeV (va_list va) = 0;

	intptr_t 
	Invoke (
		int Unused,
		...
		)
	{
		return InvokeV (va_start_e (Unused));
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
	AXL_OBJ_SIMPLE_CLASS (CFunctionT, IFunction)

protected:
	void* m_pfn;
	ECallConv m_Convention;
	ref::CPtrT <CArgBlock> m_Context;

public:
	CFunctionT ()
	{
		m_pfn = NULL;
		m_Convention = ECallConv_Cdecl;
	}

	CFunctionT (
		ECallConv Convention,
		void* pfn,
		...
		)
	{
		SetupV (Convention, pfn, va_start_e (pfn));
	}

	CFunctionT (
		void* pfn,
		...
		)
	{
		SetupV (pfn, va_start_e (pfn));
	}

	void
	SetupV (
		ECallConv Convention,
		void* pfn,
		va_list va
		)
	{
		m_Convention = Convention;
		m_pfn = pfn;
		m_Context = CreateArgBlockV <TCtxArg> (va);
	}

	void
	SetupV (
		void* pfn,
		va_list va
		)
	{
		SetupV (ECallConv_Cdecl, pfn, va);
	}

	void
	Setup (
		ECallConv Convention,
		void* pfn,
		...
		)
	{
		SetupV (Convention, pfn, va_start_e (pfn));
	}

	void
	Setup (
		void* pfn,
		...
		)
	{
		SetupV (pfn, va_start_e (pfn));
	}

	virtual 
	IArgPacker* 
	GetArgPacker ()
	{
		return IArgPackerImplT <TParArg>::GetSingleton ();
	}

	virtual 
	intptr_t 
	InvokeV (va_list va)
	{
		size_t Size;
		TParArg () (NULL, &Size, NULL, va);

		mem::TBlock Stack [2] = 
		{
			m_Context ? *m_Context : mem::g_EmptyBlock,
			{ va, Size }
		};

		return exe::InvokeDirect (m_pfn, m_Convention, Stack, 2);
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
