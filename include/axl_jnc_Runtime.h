// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_GcHeap.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_TlsMgr.h"
#include "axl_mt_TlsSlot.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

class CRuntime
{
protected:
	CModule* m_pModule;
	llvm::ExecutionEngine* m_pLlvmExecutionEngine;
	size_t m_TlsSlot;
	size_t m_TlsSize;

public:
	CGcHeap m_GcHeap;

public:
	CRuntime ();

	~CRuntime ()
	{
		Clear ();
	}

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	llvm::ExecutionEngine* 
	GetLlvmExecutionEngine ()
	{
		return m_pLlvmExecutionEngine;
	}

	size_t 
	GetTlsSlot ()
	{
		return m_TlsSlot;
	}

	size_t 
	GetTlsSize ()
	{
		return m_TlsSize;
	}

	bool
	Create (CModule* pModule);

	void
	Clear ();
};

//.............................................................................

typedef mt::CScopeTlsSlotT <CRuntime> CScopeThreadRuntime;

inline
CRuntime*
GetCurrentThreadRuntime ()
{
	return mt::GetTlsSlotValue <CRuntime> ();
}

//.............................................................................

} // namespace axl 
} // namespace jnc 
