// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_TlsMgr.h"
#include "axl_jnc_Value.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_DataPtrType.h"
#include "axl_mt_TlsSlot.h"
#include "axl_mt_Event.h"
#include "axl_mt_Thread.h"
#include "axl_rtl_BuddyAllocMap.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

enum EJit
{
	EJit_Normal = 0,
	EJit_McJit,

	EJit__Count
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
GetJitKindString (EJit JitKind);

//.............................................................................

enum ECreateObjectFlag
{
	ECreateObjectFlag_Construct = 0x01,
	ECreateObjectFlag_Pin       = 0x02,
};

//.............................................................................

class CRuntime
{
	friend class CGcHeap;

protected:
	enum EGcState
	{
		EGcState_Idle = 0,
		EGcState_WaitSafePoint,
		EGcState_Mark,
		EGcState_Sweep,
	};

	struct TGcRoot
	{
		void* m_p;
		CType* m_pType;
	};

	struct TGcDestructGuard: rtl::TListLink
	{
		rtl::CArrayT <TGcRoot> m_GcRootArray;

		TGcDestructGuard ()
		{
		}

		TGcDestructGuard (
			ref::EBuf BufKind,
			void* p,
			size_t Size
			):
			m_GcRootArray (BufKind, p, Size)
		{
		}
	};

protected:
	CModule* m_pModule;
	EJit m_JitKind;
	llvm::ExecutionEngine* m_pLlvmExecutionEngine;
	rtl::CStringHashTableMapT <void*> m_FunctionMap;

	mt::CLock m_Lock;

	// gc-heap

	volatile EGcState m_GcState;

	size_t m_GcHeapLimit;
	size_t m_TotalGcAllocSize;
	size_t m_CurrentGcAllocSize;
	size_t m_PeriodGcAllocLimit;
	size_t m_PeriodGcAllocSize;

	mt::CNotificationEvent m_GcIdleEvent;
	mt::CNotificationEvent m_GcSafePointEvent;
	volatile intptr_t m_GcUnsafeThreadCount;
	rtl::CArrayT <TObject*> m_GcMemBlockArray;
	rtl::CArrayT <TObject*> m_GcObjectArray;

	rtl::CHashTableT <TInterface*, rtl::CHashIdT <TInterface*> > m_GcPinTable;
	rtl::CAuxListT <TGcDestructGuard> m_GcDestructGuardList;
	rtl::CArrayT <TGcRoot> m_StaticGcRootArray;
	rtl::CArrayT <TGcRoot> m_GcRootArray [2];
	size_t m_CurrentGcRootArrayIdx;

	// tls

	size_t m_StackLimit;
	size_t m_TlsSlot;
	size_t m_TlsSize;
	rtl::CAuxListT <TTlsHdr> m_TlsList;

public:
	CRuntime ();

	~CRuntime ()
	{
		Destroy ();
	}

	EJit
	GetJitKind ()
	{
		return m_JitKind;
	}

	bool
	Create (
		CModule* pModule,
		EJit JitKind,
		size_t HeapLimit = -1,
		size_t StackLimit = -1
		);

	void
	Destroy ();

	void
	MapFunction (
		llvm::Function* pLlvmFunction,
		void* pf
		);

	void*
	FindFunctionMapping (const char* pName)
	{
		rtl::CStringHashTableMapIteratorT <void*> It = m_FunctionMap.Find (pName);
		return It ? It->m_Value : NULL;
	}

	bool
	Startup ();

	void
	Shutdown ();

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

	static
	void
	RuntimeError (
		int Error,
		void* pCodeAddr,
		void* pDataAddr
		);

	// gc heap

	void
	RunGc ()
	{
		WaitGcIdleAndLock ();
		RunGc_l ();
	}

	void
	GcEnter ();

	void
	GcLeave ();

	void
	GcPulse ();

	TObject*
	GcAllocateObject (CClassType* pType);

	TDataPtr
	GcAllocateData (
		CType* pType,
		size_t Count
		);

	TObject*
	GcTryAllocateObject (CClassType* pType)
	{
		return GcTryAllocate (pType, 1);
	}

	TDataPtr
	GcTryAllocateData (
		CType* pType,
		size_t Count
		);

	void
	AddGcRoot (
		void* p,
		CType* pType
		);

	// creating objects on gc heap

	TInterface*
	CreateObject (
		CClassType* pType,
		uint_t Flags = ECreateObjectFlag_Construct | ECreateObjectFlag_Pin
		);

	void
	PinObject (TInterface* pObject);

	void
	UnpinObject (TInterface* pObject);

	// tls

	size_t
	GetTlsSlot ()
	{
		return m_TlsSlot;
	}

	TTlsHdr*
	GetTls ();

	TTlsHdr*
	CreateTls ();

	void
	DestroyTls (TTlsHdr* pTls);

protected:
	void
	WaitGcIdleAndLock ();

	TObject*
	GcTryAllocate (
		CType* pType,
		size_t Count
		);

	void
	GcMarkCycle ();

	void
	GcAddObject (TObject* pObject);

	void
	RunGc_l ();
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

enum ERuntimeError
{
	ERuntimeError_OutOfMemory,
	ERuntimeError_StackOverflow,
	ERuntimeError_ScopeMismatch,
	ERuntimeError_DataPtrOutOfRange,
	ERuntimeError_NullClassPtr,
	ERuntimeError_NullFunctionPtr,
	ERuntimeError_NullPropertyPtr,
	ERuntimeError_AbstractFunction,
};

//.............................................................................

} // namespace axl
} // namespace jnc
