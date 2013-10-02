// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Value.h"
#include "axl_rtl_StringHashTable.h"

namespace axl {
namespace jnc {

class CMulticastClassType;

//.............................................................................

class CStdLib
{
protected:
	rtl::CStringHashTableMapT <void*> m_FunctionMap;
	
public:
	CStdLib ();
	
	void*
	FindFunction (const char* pName)
	{	
		rtl::CStringHashTableMapIteratorT <void*> It = m_FunctionMap.Find (pName);
		return It ? It->m_Value : NULL;
	}
	
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
	TInterface* 
	DynamicCastClassPtr (
		TInterface* p,
		CClassType* pType
		);

	static
	TInterface* 
	StrengthenClassPtr (TInterface* p);

	static
	void
	MulticastClear (TMulticast* pMulticast);

	static
	handle_t
	MulticastSet (
		TMulticast* pMulticast,
		TFunctionPtr Ptr
		);

	static
	handle_t
	MulticastSet_t (
		TMulticast* pMulticast,
		void* pf
		);

	static
	handle_t
	MulticastAdd (
		TMulticast* pMulticast,
		TFunctionPtr Ptr
		);

	static
	handle_t
	MulticastAdd_t (
		TMulticast* pMulticast,
		void* pf
		);

	static
	TFunctionPtr
	MulticastRemove (
		TMulticast* pMulticast,
		handle_t Handle
		);

	static
	void*
	MulticastRemove_t (
		TMulticast* pMulticast,
		handle_t Handle
		);

	static
	TFunctionPtr
	MulticastGetSnapshot (TMulticast* pMulticast);

	static
	void*
	HeapAlloc (size_t Size);

	static
	void*
	UHeapAlloc (size_t Size);

	static
	void
	UHeapFree (void* p);

	static
	void
	UHeapFreeClassPtr (TInterface* p);

	static
	void
	GcAddObject (TObject* p);

	static
	void
	GcSafePoint ();

	static
	void
	RunGc ();

	static
	void
	RunGcWaitForDestructors ();

	static
	intptr_t
	GetCurrentThreadId ();

	static
	bool
	CreateThread (TFunctionPtr Ptr);

	static
	void
	Sleep (uint32_t MsCount);

	static
	size_t
	StrLen (TDataPtr Ptr);

	static
	int
	Rand ()
	{
		return rand ();
	}

	static
	void*
	GetTls ();

	static
	size_t
	AppendFmtLiteral_a (
		TFmtLiteral* pFmtLiteral,
		const char* p,
		size_t Length
		);	

	static
	size_t
	AppendFmtLiteral_p (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		TDataPtr Ptr
		);

	static
	size_t
	AppendFmtLiteral_i32 (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		int32_t x
		)
	{
		return AppendFmtLiteralImpl (pFmtLiteral, pFmtSpecifier, 'd', x);
	}

	static
	size_t
	AppendFmtLiteral_ui32 (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		uint32_t x
		)
	{
		return AppendFmtLiteralImpl (pFmtLiteral, pFmtSpecifier, 'u', x);
	}

	static
	size_t
	AppendFmtLiteral_i64 (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		int64_t x
		)
	{
		return AppendFmtLiteralImpl (pFmtLiteral, pFmtSpecifier, 'd', x);
	}

	static
	size_t
	AppendFmtLiteral_ui64 (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		uint64_t x
		)
	{
		return AppendFmtLiteralImpl (pFmtLiteral, pFmtSpecifier, 'u', x);
	}

	static
	size_t
	AppendFmtLiteral_f (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		double x
		)
	{
		return AppendFmtLiteralImpl (pFmtLiteral, pFmtSpecifier, 'f', x);
	}

protected:
#if (_AXL_ENV == AXL_ENV_WIN)
	static
	DWORD 
	WINAPI
	ThreadProc (PVOID pContext);
#elif (_AXL_ENV == AXL_ENV_POSIX)	
	static
	void*
	ThreadProc (void* pContext);
#endif

	static
	void
	ExportMulticastMethods (
		CModule* pModule,
		llvm::ExecutionEngine* pLlvmExecutionEngine,
		CMulticastClassType* pMulticastType
		);

	static
	void
	PrepareFormatString (
		rtl::CString* pFormatString,
		const char* pFmtSpecifier,
		char DefaultType
		);

	static
	size_t
	AppendFmtLiteralImpl (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		char DefaultType,
		...
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
