#include "pch.h"
#include "axl_jnc_Runtime.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_StdLib.h"

namespace axl {
namespace jnc {

using namespace llvm;

//.............................................................................

const char*
GetJitKindString (EJit JitKind)
{
	static const char* StringTable [EJit__Count] =
	{
		"normal-jit", // EJit_Normal = 0,
		"mc-jit",     // EJit_McJit,
	};

	return (size_t) JitKind < EJit__Count ?
		StringTable [JitKind] :
		"undefined-jit";
}

//.............................................................................

class CJitMemoryManager: public llvm::SectionMemoryManager
{
public:
	CRuntime* m_pRuntime;

	CJitMemoryManager (CRuntime* pRuntime)
	{
		m_pRuntime = pRuntime;
	}

	virtual
	void*
	getPointerToNamedFunction (
		const std::string &Name,
		bool AbortOnFailure
		);
};

void*
CJitMemoryManager::getPointerToNamedFunction (
	const std::string& Name,
	bool AbortOnFailure
	)
{
	void* pf = m_pRuntime->FindFunctionMapping (Name.c_str ());
	if (pf)
		return pf;

	if (AbortOnFailure)
		report_fatal_error ("CJitMemoryManager::getPointerToNamedFunction: unresolved external function '" + Name + "'");

	return 0;
}

//.............................................................................

CRuntime::CRuntime ()
{
	m_pModule = NULL;
	m_pLlvmExecutionEngine = NULL;
	m_JitKind = EJit_Normal;

	m_GcState = EGcState_Idle;
	m_pGcHeap = NULL;
	m_GcHeapSize = 0;
	m_GcBlockSize = 0;
	m_CurrentGcRootArrayIdx = 0;

	m_GcDestructThread.m_pRuntime = this;
	m_TerminateGcDestructThread = false;

	m_StackLimit = 0;
	m_TlsSlot = -1;
	m_TlsSize = 0;
}

bool
CRuntime::Create (
	CModule* pModule,
	EJit JitKind,
	size_t HeapSize,
	size_t StackLimit
	)
{
	Destroy ();

	m_pModule = pModule;
	m_JitKind = JitKind;

	// execution engine

	llvm::EngineBuilder EngineBuilder (pModule->GetLlvmModule ());

	std::string errorString;
	EngineBuilder.setErrorStr (&errorString);
	EngineBuilder.setEngineKind(llvm::EngineKind::JIT);

	llvm::TargetOptions TargetOptions;
	TargetOptions.JITExceptionHandling = true;
	TargetOptions.JITEmitDebugInfo = true;

	if (JitKind == EJit_McJit)
	{
		CJitMemoryManager* pJitMemoryManager = new CJitMemoryManager (this);
		EngineBuilder.setUseMCJIT (true);
		EngineBuilder.setJITMemoryManager (pJitMemoryManager);
	}

	EngineBuilder.setTargetOptions (TargetOptions);

#if (_AXL_CPU == AXL_CPU_X86)
	EngineBuilder.setMArch ("x86");
#endif

	m_pLlvmExecutionEngine = EngineBuilder.create ();
	if (!m_pLlvmExecutionEngine)
	{
		err::SetFormatStringError ("cannot create execution engine: %s\n", errorString.c_str());
		return false;
	}

	// gc heap

	size_t BlockSize = _AXL_PTR_SIZE * 4; // 1 block = 4 pointers
	size_t PageSize = BlockSize * _AXL_PTR_BITNESS;
	size_t PageCount = HeapSize / PageSize;
	if (HeapSize & (PageSize - 1))
		PageCount++;

	size_t HeapHeight = rtl::GetHiBitIdx (PageCount);
	if (PageCount & ((1 << HeapHeight) - 1))
		HeapHeight++;

	bool Result = m_GcMap.Create (1, HeapHeight + 1);
	if (!Result)
		return false;

	size_t BlockCount = m_GcMap.GetTotalSize ();
	HeapSize = BlockCount * BlockSize; // update

	void* pHeap = AXL_MEM_ALLOC (HeapSize);
	if (!pHeap)
		return false;

	m_GcHeapSize = HeapSize;
	m_pGcHeap = pHeap;
	m_GcBlockSize = BlockSize;

	// static gc roots

	rtl::CArrayT <CVariable*> StaticRootArray = pModule->m_VariableMgr.GetStaticGcRootArray ();
	size_t Count = StaticRootArray.GetCount ();

	m_StaticGcRootArray.SetCount (Count);
	for (size_t i = 0; i < Count; i++)
	{
		CVariable* pVariable = StaticRootArray [i];
		void* p = m_pLlvmExecutionEngine->getPointerToGlobal ((llvm::GlobalVariable*) pVariable->GetLlvmAllocValue ());
		ASSERT (p);

		m_StaticGcRootArray [i].m_p = p;
		m_StaticGcRootArray [i].m_pType = pVariable->GetType ();
	}

	// tls

	m_StackLimit = StackLimit;
	m_TlsSize = pModule->m_VariableMgr.GetTlsStructType ()->GetSize ();
	m_TlsSlot = GetTlsMgr ()->CreateSlot ();

	return true;
}

void
CRuntime::Destroy ()
{
	if (!m_pModule)
		return;

	if (m_GcDestructThread.IsOpen ())
	{
		m_TerminateGcDestructThread = true;
		m_GcDestructEvent.Signal ();
		m_GcDestructThread.WaitAndClose ();
	}

	if (m_TlsSlot != -1)
	{
		GetTlsMgr ()->NullifyTlsData (this);
		GetTlsMgr ()->DestroySlot (m_TlsSlot);
	}

	if (m_pLlvmExecutionEngine)
		delete m_pLlvmExecutionEngine;

	m_pLlvmExecutionEngine = NULL;

	m_TlsSlot = -1;
	m_TlsSize = 0;

	while (!m_TlsList.IsEmpty ())
	{
		TTlsData* pTlsData = m_TlsList.RemoveHead ();
		AXL_MEM_FREE (pTlsData);
	}

	if (m_pGcHeap)
		AXL_MEM_FREE (m_pGcHeap);

	m_pGcHeap = NULL;
	m_GcBlockSize = 0;
	m_GcMap.Clear ();
	m_GcObjectList.Clear ();
	m_GcDestructList.Clear ();
	m_StaticGcRootArray.Clear ();
	m_FunctionMap.Clear ();

	m_pModule = NULL;
}

void
CRuntime::MapFunction (
	llvm::Function* pLlvmFunction,
	void* pf
	)
{
	if (m_JitKind == EJit_McJit)
	{
		m_FunctionMap [pLlvmFunction->getName ().data ()] = pf;
	}
	else
	{
		ASSERT (m_pLlvmExecutionEngine);
		m_pLlvmExecutionEngine->addGlobalMapping (pLlvmFunction, pf);
	}
}

bool
CRuntime::Startup ()
{
	m_TerminateGcDestructThread = false;
	m_GcDestructThread.Start ();
	m_GcDestructThreadIdleEvent.Wait (); // make sure GC destruct thread is running and registered with TLS mgr
	m_GcIdleEvent.Signal ();
	return true;
}

void
CRuntime::Shutdown ()
{
	rtl::CArrayT <TGcRoot> SaveStaticGcRootArray = m_StaticGcRootArray;
	m_StaticGcRootArray.Clear ();

	RunGcWaitForDestructors ();

	m_TerminateGcDestructThread = true;
	m_GcDestructEvent.Signal ();
	m_GcDestructThread.WaitAndClose ();

	TTlsData* pTlsData = GetTlsMgr ()->NullifyTlsData (this);
	if (pTlsData)
	{
		m_TlsList.Remove (pTlsData);
		AXL_MEM_FREE (pTlsData);
	}

	ASSERT (m_TlsList.IsEmpty ());

	m_StaticGcRootArray = SaveStaticGcRootArray; // recover
}

void*
CRuntime::GcAllocate (size_t Size)
{
	ASSERT (m_pGcHeap);

	size_t BlockCount = GetGcBlockCount (Size);

	WaitGcIdleAndLock ();
	size_t Address = m_GcMap.Allocate (BlockCount);
	m_Lock.Unlock ();

	if (Address == -1)
	{
		RunGc ();

		WaitGcIdleAndLock ();
		Address = m_GcMap.Allocate (BlockCount);
		m_Lock.Unlock ();

		if (Address == -1) // double fault
		{
			CStdLib::OnRuntimeError (ERuntimeError_OutOfMemory, NULL, NULL);
			ASSERT (false);
		}
	}

	void* p = (char*) m_pGcHeap + Address * m_GcBlockSize;
	memset (p, 0, Size);
	return p;
}

void
CRuntime::GcAddObject (TObject* pObject)
{
	WaitGcIdleAndLock ();
	GcAddObject_l (pObject);
	m_Lock.Unlock ();
}

void
CRuntime::GcAddObject_l (TObject* pObject)
{
	// printf ("GcAddObject %08x: %s\n", pObject, pObject->m_pType->GetTypeString ().cc ());

	// recursively add all the class fields

	char* pBase = (char*) (pObject + 1);

	rtl::CArrayT <CStructField*> ClassFieldArray = pObject->m_pType->GetClassMemberFieldArray ();
	size_t Count = ClassFieldArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = ClassFieldArray [i];
		TObject* pChildObject = (TObject*) (pBase + pField->GetOffset ());
		GcAddObject_l (pChildObject);
	}

	// and then add the object itself

	m_GcObjectList.InsertTail (pObject);
}

void
CRuntime::MarkGcObject (TObject* pObject)
{
	if (ShouldMarkGcPtr (pObject)) // prevent re-marking of weakly marked objects
		MarkGcRange (pObject, pObject->m_pType->GetSize ());

	pObject->m_Flags |= EObjectFlag_GcMark;

	if (pObject->m_pType->GetFlags () & ETypeFlag_GcRoot)
		AddGcRoot (pObject, pObject->m_pType);
}

void
CRuntime::AddGcRoot (
	void* p,
	CType* pType
	)
{
	ASSERT (m_GcState == EGcState_Mark);
	ASSERT (pType->GetFlags () & ETypeFlag_GcRoot);

	TGcRoot Root;
	Root.m_p = p;
	Root.m_pType = pType;
	m_GcRootArray [m_CurrentGcRootArrayIdx].Append (Root);
}

void
CRuntime::RunGcEx (uint_t Flags)
{
	// printf ("Running GC...\n");

	WaitGcIdleAndLock ();

	// 1) suspend all threads at safe points

	m_GcState = EGcState_WaitSafePoint;
	m_GcIdleEvent.Reset ();
	m_WaitGcSafePointCount = m_TlsList.GetCount ();

	if (GetTlsMgr ()->FindTlsData (this)) // minus current
		m_WaitGcSafePointCount--;

	if (m_WaitGcSafePointCount)
	{
		m_GcDestructEvent.Signal (); // the destruct thread
		m_Lock.Unlock ();
		m_WaitGcSafePointEvent.Wait ();
		m_Lock.Lock ();
	}

	m_GcState = EGcState_Mark;

	m_GcMap.Clear ();
	m_GcRootArray [0].Clear ();
	m_CurrentGcRootArrayIdx = 0;

	// 2) mark

	// 2.1) add static roots

	size_t Count = m_StaticGcRootArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		ASSERT (m_StaticGcRootArray [i].m_pType->GetFlags () & ETypeFlag_GcRoot);
		AddGcRoot (
			m_StaticGcRootArray [i].m_p,
			m_StaticGcRootArray [i].m_pType
			);
	}

	// 2.2) add stack roots

	CVariable* pGcShadowStackTop = m_pModule->m_VariableMgr.GetStdVariable (EStdVariable_GcShadowStackTop);
	size_t GcShadowStackTopOffset = pGcShadowStackTop->GetTlsField ()->GetOffset ();

	rtl::CIteratorT <TTlsData> TlsData = m_TlsList.GetHead ();
	for (; TlsData; TlsData++)
	{
		TGcShadowStackFrame* pStackFrame = *(TGcShadowStackFrame**) ((char*) (*TlsData + 1) + GcShadowStackTopOffset);
		for (; pStackFrame; pStackFrame = pStackFrame->m_pNext)
		{
			size_t Count = pStackFrame->m_pMap->m_Count;
			void** ppRootArray = (void**) (pStackFrame + 1);
			CType** ppTypeArray = (CType**) (pStackFrame->m_pMap + 1);

			for (size_t i = 0; i < Count; i++)
			{
				void* p = ppRootArray [i];
				CType* pType = ppTypeArray [i];

				if (!p)
					continue;

				if (ShouldMarkGcPtr (p))
					MarkGcRange (p, pType->GetSize ());

				if (pType->GetTypeKind () == EType_Class)
					((TObject*) p)->m_Flags |= EObjectFlag_GcMark;

				if (pType->GetFlags () & ETypeFlag_GcRoot)
					AddGcRoot (p, pType);
			}
		}
	}

	// 2.3) run mark cycle

	GcMarkCycle ();

	// 4) remove 'alive' flag from unmarked objects and schedule destruction

	rtl::CAuxListT <TObject, CObjectGcHeapLink> DestructList;

	rtl::CIteratorT <TObject, CObjectGcHeapLink> Object = m_GcObjectList.GetTail ();
	while (Object)
	{
		rtl::CIteratorT <TObject, CObjectGcHeapLink> Next = Object.GetInc (-1);

		// weakly-marked closures are saved as well

		if (Object->m_Flags & (EObjectFlag_GcMark | EObjectFlag_GcMark_wc))
		{
			Object->m_Flags &= ~(EObjectFlag_GcMark | EObjectFlag_GcMark_wc);
		}
		else
		{
			Object->m_Flags &= ~EObjectFlag_Alive;
			Object->m_ScopeLevel = 1; // prevent resurrection
			m_GcObjectList.Remove (Object);

			if (Object->m_pType->GetDestructor ())
				DestructList.InsertTail (*Object);
		}

		Object = Next;
	}

	bool IsDestructListEmpty = DestructList.IsEmpty ();
	if (!IsDestructListEmpty)
	{
		m_GcDestructList.InsertListTail (&DestructList);
		m_GcDestructThreadIdleEvent.Reset ();
	}

	// 5) secondary mark (to prevent re-allocating the objects currently scheduled for destruction)

	Object = m_GcDestructList.GetHead ();
	for (; Object; Object++)
	{
		TObject* pObject = *Object;

		ASSERT (ShouldMarkGcObject (pObject));
		MarkGcObject (pObject);

		if (pObject->m_pType->GetFlags () & ETypeFlag_GcRoot)
			AddGcRoot (pObject, pObject->m_pType);
	}

	GcMarkCycle ();

	// 6) resume all suspended threads and wake up destruct thread (if needed)

	m_GcState = EGcState_Idle;
	m_GcIdleEvent.Signal ();

	if (!IsDestructListEmpty)
		m_GcDestructEvent.Signal ();

	m_Lock.Unlock ();

	// 7) wait for destructors

	if (Flags & EGcFlag_WaitForDestructors)
		m_GcDestructThreadIdleEvent.Wait ();
}

void
CRuntime::GcMarkCycle ()
{
	// mark breadth first

	while (!m_GcRootArray [m_CurrentGcRootArrayIdx].IsEmpty ())
	{
		size_t PrevGcRootArrayIdx =  m_CurrentGcRootArrayIdx;
		m_CurrentGcRootArrayIdx = !m_CurrentGcRootArrayIdx;
		m_GcRootArray [m_CurrentGcRootArrayIdx].Clear ();

		size_t Count = m_GcRootArray [PrevGcRootArrayIdx].GetCount ();
		for (size_t i = 0; i < Count; i++)
		{
			const TGcRoot* pRoot = &m_GcRootArray [PrevGcRootArrayIdx] [i];
			pRoot->m_pType->EnumGcRoots (this, pRoot->m_p);
		}
	}
}

void
CRuntime::GcDestructThreadProc ()
{
	CScopeThreadRuntime ScopeRuntime (this);
	GetTlsMgr ()->GetTlsData (this); // register thread with TLS mgr right away
	m_GcDestructThreadIdleEvent.Signal ();

	while (!m_TerminateGcDestructThread)
	{
		m_GcDestructEvent.Wait ();

		WaitGcIdleAndLock ();
		if (!m_GcDestructList.IsEmpty ())
		{
			m_GcDestructThreadIdleEvent.Reset ();

			while (!m_GcDestructList.IsEmpty ())
			{
				rtl::CIteratorT <TObject, CObjectGcHeapLink> Object = m_GcDestructList.GetHead ();
				m_Lock.Unlock ();

				CFunction* pDestructor = Object->m_pType->GetDestructor ();
				ASSERT (pDestructor);

				FObject_Destruct* pf = (FObject_Destruct*) pDestructor->GetMachineCode ();
				pf ((jnc::TInterface*) (*Object + 1));

				WaitGcIdleAndLock ();
				m_GcDestructList.RemoveHead ();
			}

			m_GcDestructThreadIdleEvent.Signal ();
		}

		m_Lock.Unlock ();
	}
}

void
CRuntime::GcSafePoint ()
{
	if (m_GcState == EGcState_Idle)
		return;

	m_Lock.Lock ();
	switch (m_GcState)
	{
	case EGcState_Idle: // everything was finished between checking and locking
		m_Lock.Unlock ();
		break;

	case EGcState_WaitSafePoint:
		if (--m_WaitGcSafePointCount == 0)
			m_WaitGcSafePointEvent.Signal ();

		m_Lock.Unlock ();
		m_GcIdleEvent.Wait ();
		break;

	default:
		ASSERT (false); // execution of user code is only allowed during 'idle' or 'wait-gc-safe-point'
		m_Lock.Unlock ();
	}
}

void
CRuntime::WaitGcIdleAndLock ()
{
	for (;;)
	{
		m_Lock.Lock ();

		if (m_GcState == EGcState_Idle)
			break;

		if (m_GcState == EGcState_WaitSafePoint && --m_WaitGcSafePointCount == 0)
			m_WaitGcSafePointEvent.Signal ();

		m_Lock.Unlock ();
		m_GcIdleEvent.Wait ();
	}
}

void*
CRuntime::GetTls ()
{
	TTlsData* pTlsData = GetTlsMgr ()->GetTlsData (this);
	ASSERT (pTlsData);

	// check for stack overflow

	char* p = (char*) _alloca (1);

	if (!pTlsData->m_pStackEpoch) // first time call
	{
		pTlsData->m_pStackEpoch = p;
	}
	else
	{
		char* p0 = (char*) pTlsData->m_pStackEpoch;
		if (p0 >= p) // the opposite could happen, but it's stack-overflow-safe
		{
			size_t StackSize = p0 - p;
			if (StackSize > m_StackLimit)
			{
				CStdLib::OnRuntimeError (ERuntimeError_StackOverflow, NULL, NULL);
				ASSERT (false);
			}
		}
	}

	return pTlsData + 1;
}

TTlsData*
CRuntime::CreateTlsData ()
{
	size_t Size = sizeof (TTlsData) + m_TlsSize;

	TTlsData* pTlsData = (TTlsData*) AXL_MEM_ALLOC (Size);
	memset (pTlsData, 0, Size);
	pTlsData->m_pRuntime = this;
	pTlsData->m_pStackEpoch = NULL;

	WaitGcIdleAndLock ();
	m_TlsList.InsertTail (pTlsData);
	m_Lock.Unlock ();

	return pTlsData;
}

void
CRuntime::DestroyTlsData (TTlsData* pTlsData)
{
	WaitGcIdleAndLock ();
	m_TlsList.Remove (pTlsData);
	m_Lock.Unlock ();

	AXL_MEM_FREE (pTlsData);
}

TInterface*
CRuntime::CreateObject (
	CClassType* pType,
	uint_t Flags
	)
{
	CFunction* pPrimer = pType->GetPrimer ();
	if (!pPrimer) // abstract
	{
		err::SetFormatStringError ("cannot create abstract '%s'", pType->m_Tag.cc ());
		return NULL;
	}

	TObject* pObject = (TObject*) GcAllocate (pType->GetSize ());
	if (!pObject)
		return NULL;

	CScopeThreadRuntime ScopeRuntime (this);

	FObject_Prime* pfPrime = (FObject_Prime*) pPrimer->GetMachineCode ();
	pfPrime (pObject);

	TInterface* pInterface = (TInterface*) (pObject + 1);

	if ((Flags & ECreateObjectFlag_Construct) && pType->GetConstructor ())
	{
		CFunction* pConstructor = pType->GetDefaultConstructor ();
		if (!pConstructor)
			return NULL;

		FObject_Construct* pfConstruct = (FObject_Construct*) pConstructor->GetMachineCode ();
		pfConstruct (pInterface);
	}

	if (Flags & ECreateObjectFlag_Pin)
		PinObject (pInterface);

	return pInterface;
}

void
CRuntime::PinObject (TInterface* pObject)
{
}

void
CRuntime::UnpinObject (TInterface* pObject)
{
}

//.............................................................................

} // namespace axl
} // namespace jnc
