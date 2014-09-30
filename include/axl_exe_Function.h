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
	getArgPacker () = 0;

	virtual 
	intptr_t 
	invokeV (axl_va_list va) = 0;

	intptr_t 
	invoke (
		int unused,
		...
		)
	{
		AXL_VA_DECL (va, unused);
		return invokeV (va);
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
	ECallConv m_convention;
	ref::CPtrT <CArgBlock> m_context;

public:
	CFunctionT ()
	{
		m_pf = NULL;
		m_convention = ECallConv_Cdecl;
	}

	CFunctionT (
		ECallConv convention,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		setupV (convention, pf, va);
	}

	CFunctionT (
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		setupV (pf, va);
	}

	void
	setupV (
		ECallConv convention,
		void* pf,
		axl_va_list va
		)
	{
		m_convention = convention;
		m_pf = pf;
		m_context = createArgBlockV <TCtxArg> (va);
	}

	void
	setupV (
		void* pf,
		axl_va_list va
		)
	{
		setupV (ECallConv_Cdecl, pf, va);
	}

	void
	setup (
		ECallConv convention,
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		setupV (convention, pf, va);
	}

	void
	setup (
		void* pf,
		...
		)
	{
		AXL_VA_DECL (va, pf);
		setupV (pf, va);
	}

	virtual 
	IArgPacker* 
	getArgPacker ()
	{
		return IArgPackerImplT <TParArg>::getSingleton ();
	}

	virtual 
	intptr_t 
	invokeV (axl_va_list va)
	{
		size_t size;
		TParArg () (NULL, &size, NULL, va);

		mem::TBlock stack [2];

		if (m_context)
			stack [0] = *m_context;

		stack [1].setup (va.m_va, size);

		return exe::invokeDirect (m_pf, m_convention, stack, 2);
	}
};

//.............................................................................

} // namespace exe
} // namespace axl
