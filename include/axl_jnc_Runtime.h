// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_TlsMgr.h"
#include "axl_jnc_Value.h"
#include "axl_jnc_ClassType.h"
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

enum EGcFlag
{
	EGcFlag_WaitForDestructors = 0x01,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ECreateObjectFlag
{
	ECreateObjectFlag_Construct = 0x01,
	ECreateObjectFlag_Pin       = 0x02,
};

//.............................................................................

class CRuntime
{
protected:
	enum EGcState
	{
		EGcState_Idle = 0,
		EGcState_WaitSafePoint,
		EGcState_Mark,
	};

	struct TGcRoot
	{
		void* m_p;
		CType* m_pType;
	};

	class CGcDestructThread: public mt::CThreadImplT <CGcDestructThread>
	{
	public:
		CRuntime* m_pRuntime;

		dword_t
		ThreadProc ()
		{
			m_pRuntime->GcDestructThreadProc ();
			return 0;
		}
	};

	friend class CGcDestructThread;

protected:
	CModule* m_pModule;
	EJit m_JitKind;
	llvm::ExecutionEngine* m_pLlvmExecutionEngine;
	rtl::CStringHashTableMapT <void*> m_FunctionMap;

	mt::CLock m_Lock;

	// gc

	EGcState m_GcState;
	mt::CNotificationEvent m_GcIdleEvent;
	mt::CEvent m_WaitGcSafePointEvent;
	size_t m_WaitGcSafePointCount;

	void* m_pGcHeap;
	size_t m_GcHeapSize;
	size_t m_GcBlockSize;
	rtl::CBuddyAllocMap m_GcMap;

	rtl::CAuxListT <TObject, CObjectGcHeapLink> m_GcObjectList;
	rtl::CAuxListT <TObject, CObjectGcHeapLink> m_GcDestructList;
	mt::CEvent m_GcDestructEvent;
	mt::CNotificationEvent m_GcDestructThreadIdleEvent;
	CGcDestructThread m_GcDestructThread;
	bool m_TerminateGcDestructThread;

	rtl::CArrayT <TGcRoot> m_StaticGcRootArray;
	rtl::CArrayT <TGcRoot> m_GcRootArray [2];
	size_t m_CurrentGcRootArrayIdx;

	// tls

	size_t m_StackLimit;
	size_t m_TlsSlot;
	size_t m_TlsSize;
	rtl::CAuxListT <TTlsData> m_TlsList;

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
		size_t HeapSize,
		size_t StackLimit
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

	// gc heap

	void
	RunGcEx (uint_t Flags);

	void
	RunGc ()
	{
		RunGcEx (0);
	}

	void
	RunGcWaitForDestructors ()
	{
		RunGcEx (EGcFlag_WaitForDestructors);
	}

	void
	GcEnter ()
	{
		GcSafePoint ();
	}

	void
	GcLeave ()
	{
		GcSafePoint ();
	}

	void
	GcPulse ()
	{
		GcSafePoint ();
	}

	void
	GcSafePoint ();

	void*
	GcAllocate (size_t Size);

	void
	GcAddObject (TObject* pObject);

	bool
	ShouldMarkGcPtr (void* p)
	{
		return IsInGcHeap (p) && !IsMarkedGcPtr (p);
	}

	bool
	ShouldMarkGcObject (TObject* pObject)
	{
		return IsInGcHeap (pObject) && !(pObject->m_Flags & EObjectFlag_GcMark);
	}

	bool
	ShouldWeakMarkGcClosureObject (TObject* pObject)
	{
		return IsInGcHeap (pObject) && !(pObject->m_Flags & (EObjectFlag_GcMark | EObjectFlag_GcMark_wc));
	}

	void
	MarkGcObject (TObject* pObject);

	void
	MarkGcRange (
		void* p,
		size_t Size
		)
	{
		ASSERT (m_GcState == EGcState_Mark && IsInGcHeap (p));
		m_GcMap.Mark (GetGcAddress (p), GetGcBlockCount (Size));
	}

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

	void*
	GetTls ();

	TTlsData*
	CreateTlsData ();

	void
	DestroyTlsData (TTlsData* pTlsData);

protected:
	void
	WaitGcIdleAndLock ();

	void
	GcAddObject_l (TObject* pObject);

	size_t
	GetGcAddress (void* p)
	{
		ASSERT (p >= m_pGcHeap);
		return ((char*) p - (char*) m_pGcHeap) / m_GcBlockSize;
	}

	size_t
	GetGcBlockCount (size_t Size)
	{
		return Size % m_GcBlockSize ? Size / m_GcBlockSize + 1 : Size / m_GcBlockSize;
	}

	bool
	IsInGcHeap (void* p)
	{
		return p >= m_pGcHeap && p < (char*) m_pGcHeap + m_GcHeapSize;
	}

	bool
	IsMarkedGcPtr (void* p)
	{
		return m_GcMap.GetBit (GetGcAddress (p));
	}

	void
	GcMarkCycle ();

	void
	GcDestructThreadProc ();
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
