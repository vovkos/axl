// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

class CMulticastClassType;

//.............................................................................

class CStdLib
{
public:
	static
	void
	Export (
		CModule* pModule,
		llvm::ExecutionEngine* pLlvmExecutionEngine
		);
	
	static
	void
	OnRuntimeError (
		int Error,
		void* pCodeAddr,
		void* pDataAddr
		);

	static
	jnc::TInterface* 
	DynamicCastClassPtr (
		jnc::TInterface* p,
		jnc::CClassType* pType
		);

	static
	jnc::TInterface* 
	StrengthenClassPtr (jnc::TInterface* p);

	static
	void
	MulticastClear (jnc::TMulticast* pMulticast);

	static
	handle_t
	MulticastSet (
		jnc::TMulticast* pMulticast,
		jnc::TFunctionPtr Ptr
		);

	static
	handle_t
	MulticastSet_t (
		jnc::TMulticast* pMulticast,
		void* pf
		);

	static
	handle_t
	MulticastAdd (
		jnc::TMulticast* pMulticast,
		jnc::TFunctionPtr Ptr
		);

	static
	handle_t
	MulticastAdd_t (
		jnc::TMulticast* pMulticast,
		void* pf
		);

	static
	jnc::TFunctionPtr
	MulticastRemove (
		jnc::TMulticast* pMulticast,
		handle_t Handle
		);

	static
	void*
	MulticastRemove_t (
		jnc::TMulticast* pMulticast,
		handle_t Handle
		);

	static
	jnc::TFunctionPtr
	MulticastGetSnapshot (jnc::TMulticast* pMulticast);

	static
	void*
	HeapAlloc (jnc::CType* pType);

	static
	void*
	UHeapAlloc (jnc::CType* pType);

	static
	void
	UHeapFree (void* p);

	static
	void
	UHeapFreeClassPtr (jnc::TInterface* p);

	static
	void
	GcAddObject (jnc::TObject* p);

	static
	void
	RunGc ();

	static
	dword_t
	GetCurrentThreadId ();

	static
	int
	CreateThread (jnc::TFunctionPtr Ptr);

	static
	void
	Sleep (uint32_t MsCount);

	static
	void*
	GetTls ();

protected:
#if (_AXL_ENV == AXL_ENV_WIN)
	static
	DWORD 
	WINAPI
	ThreadProc (PVOID pContext);
#endif

	static
	void
	ExportMulticastMethods (
		CModule* pModule,
		llvm::ExecutionEngine* pLlvmExecutionEngine,
		CMulticastClassType* pMulticastType
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {