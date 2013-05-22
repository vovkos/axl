// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"

namespace axl {
namespace jnc {

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
	handle_t
	MulticastSet (
		jnc::TMulticast* pMulticast,
		jnc::TFunctionPtr Ptr
		);

	static
	handle_t
	MulticastSet_w (
		jnc::TMulticast* pMulticast,
		jnc::TFunctionPtr_w Ptr
		);

	static
	handle_t
	MulticastSet_u (
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
	MulticastAdd_w (
		jnc::TMulticast* pMulticast,
		jnc::TFunctionPtr_w Ptr
		);

	static
	handle_t
	MulticastAdd_u (
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
	jnc::TFunctionPtr_w
	MulticastRemove_w (
		jnc::TMulticast* pMulticast,
		handle_t Handle
		);

	static
	void*
	MulticastRemove_u (
		jnc::TMulticast* pMulticast,
		handle_t Handle
		);

	static
	jnc::TMcSnapshot
	MulticastSnapshot (jnc::TMulticast* pMulticast);

	static
	jnc::TMcSnapshot
	MulticastSnapshot_w (jnc::TMulticast* pMulticast);

	static
	jnc::TMcSnapshot
	MulticastSnapshot_u (jnc::TMulticast* pMulticast);

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
	dword_t
	GetCurrentThreadId ();

	static
	int
	CreateThread (jnc::TFunctionPtr Ptr);

	static
	void
	PointerCheck (jnc::TDataPtr Ptr);

protected:
#if (_AXL_ENV == AXL_ENV_WIN)
	static
	DWORD 
	WINAPI
	ThreadProc (PVOID pContext);
#endif
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
