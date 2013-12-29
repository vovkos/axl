#include "pch.h"
#include "axl_jnc_Runtime.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_GcShadowStack.h"

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
	m_GcHeapLimit = -1;
	m_TotalGcAllocSize = 0;
	m_CurrentGcAllocSize = 0;
	m_PeriodGcAllocSize = 0;
	m_PeriodGcAllocLimit = 16 * 1024;
	m_GcUnsafeThreadCount = 1;
	m_CurrentGcRootArrayIdx = 0;

	m_StackLimit = -1;
	m_TlsSlot = -1;
	m_TlsSize = 0;
}

bool
CRuntime::Create (
	CModule* pModule,
	EJit JitKind,
	size_t HeapLimit,
	size_t StackLimit
	)
{
	Destroy ();

	m_pModule = pModule;
	m_JitKind = JitKind;
	m_GcHeapLimit = HeapLimit;
	m_StackLimit = StackLimit;

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

	m_TlsSize = pModule->m_VariableMgr.GetTlsStructType ()->GetSize ();
	m_TlsSlot = GetTlsMgr ()->CreateSlot ();
	return true;
}

void
CRuntime::Destroy ()
{
	if (!m_pModule)
		return;

	if (m_TlsSlot != -1)
	{
		GetTlsMgr ()->NullifyTls (this);
		GetTlsMgr ()->DestroySlot (m_TlsSlot);
	}

	if (m_pLlvmExecutionEngine)
		delete m_pLlvmExecutionEngine;

	m_pLlvmExecutionEngine = NULL;

	m_TlsSlot = -1;
	m_TlsSize = 0;

	while (!m_TlsList.IsEmpty ())
	{
		TTlsHdr* pTls = m_TlsList.RemoveHead ();
		AXL_MEM_FREE (pTls);
	}

/*	m_GcHeap = NULL;
	m_GcBlockSize = 0;
	m_GcMap.Clear ();
	m_GcObjectList.Clear ();
	m_GcDestructList.Clear ();
*/

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
	m_GcIdleEvent.Signal ();
	return true;
}

void
CRuntime::Shutdown ()
{
	rtl::CArrayT <TGcRoot> SaveStaticGcRootArray = m_StaticGcRootArray;
	m_StaticGcRootArray.Clear ();

	RunGc ();

	TTlsHdr* pTls = GetTlsMgr ()->NullifyTls (this);
	if (pTls)
	{
		m_TlsList.Remove (pTls);
		AXL_MEM_FREE (pTls);
	}

	ASSERT (m_TlsList.IsEmpty ());

	m_StaticGcRootArray = SaveStaticGcRootArray; // recover
}

void
CRuntime::RuntimeError (
	int Error,
	void* pCodeAddr,
	void* pDataAddr
	)
{
	const char* pErrorString;

	switch (Error)
	{
	case ERuntimeError_OutOfMemory:
		pErrorString = "OUT_OF_MEMORY";
		break;

	case ERuntimeError_StackOverflow:
		pErrorString = "STACK_OVERFLOW";
		break;

	case ERuntimeError_DataPtrOutOfRange:
		pErrorString = "DATA_PTR_OOR";
		break;

	case ERuntimeError_ScopeMismatch:
		pErrorString = "SCOPE_MISMATCH";
		break;

	case ERuntimeError_NullClassPtr:
		pErrorString = "NULL_CLASS_PTR";
		break;

	case ERuntimeError_NullFunctionPtr:
		pErrorString = "NULL_FUNCTION_PTR";
		break;

	case ERuntimeError_NullPropertyPtr:
		pErrorString = "NULL_PROPERTY_PTR";
		break;

	case ERuntimeError_AbstractFunction:
		pErrorString = "ABSTRACT_FUNCTION";
		break;

	default:
		ASSERT (false);
		pErrorString = "<UNDEF>";
	}

	throw err::FormatStringError (
		"RUNTIME ERROR: %s (code %x accessing data %x)",
		pErrorString,
		pCodeAddr,
		pDataAddr
		);
}

TObject*
CRuntime::GcAllocateObject (CClassType* pType)
{
	TObject* pObject = GcTryAllocateObject (pType);
	if (!pObject)
	{
		RuntimeError (ERuntimeError_OutOfMemory, NULL, NULL);
		ASSERT (false);
	}

	return pObject;
}

TDataPtr
CRuntime::GcAllocateData (
	CType* pType,
	size_t Count
	)
{
	TDataPtr Ptr = GcTryAllocateData (pType, Count);
	if (!Ptr.m_p)
	{
		RuntimeError (ERuntimeError_OutOfMemory, NULL, NULL);
		ASSERT (false);
	}

	return Ptr;
}

TDataPtr
CRuntime::GcTryAllocateData (
	CType* pType,
	size_t Count
	)
{
	TDataPtr Ptr = { 0 };
	TObject* pObject = GcTryAllocate (pType, Count);
	if (!pObject)
		return Ptr;

	char* p = Count > 1 ? 
		(char*) ((TDynamicArray*) pObject + 1) : 
		(char*) (pObject + 1);

	Ptr.m_p = p;
	Ptr.m_pRangeBegin = p;
	Ptr.m_pRangeBegin = p + pType->GetSize () *  Count;
	Ptr.m_pObject = pObject;
	return Ptr;
}

TObject*
CRuntime::GcTryAllocate (
	CType* pType,
	size_t Count
	)
{
	size_t Size = Count > 1 ? 
		sizeof (TDynamicArray) + pType->GetSize () * Count :
		sizeof (TObject) + pType->GetSize ();
			
	WaitGcIdleAndLock ();	
	if (m_PeriodGcAllocSize > m_PeriodGcAllocLimit)
	{
		RunGc_l ();
		WaitGcIdleAndLock ();	
	}

	TObject* pObject = (TObject*) AXL_MEM_ALLOC (Size);
	if (!pObject)
	{
		m_Lock.Unlock ();
		return NULL;
	}

	pObject->m_ScopeLevel = 0;
	pObject->m_pRoot = pObject;
	pObject->m_pType = pType;
	pObject->m_Flags = 0;

	if (Count > 1)
	{
		TDynamicArray* pArray = (TDynamicArray*) pObject;

		pArray->m_Flags |= EObjectFlag_DynamicArray;
		pArray->m_Count = Count;
	}
	else if (pType->GetTypeKind () == EType_Class)
	{
		// TODO: Prime
		GcAddObject (pObject);
	}

	m_PeriodGcAllocSize += Size;
	m_GcMemBlockArray.Append (pObject);

	// mark thread unsafe for gc-collection (until the user code saves pointer thus making it reachable)

	GcEnter ();

	m_Lock.Unlock ();
	
	return pObject;
}

void
CRuntime::GcAddObject (TObject* pObject)
{
	char* p = (char*) (pObject + 1);

	rtl::CArrayT <CStructField*> ClassFieldArray = pObject->m_pClassType->GetClassMemberFieldArray ();
	size_t Count = ClassFieldArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = ClassFieldArray [i];
		TObject* pChildObject = (TObject*) (p + pField->GetOffset ());
		GcAddObject (pChildObject);
	}

	m_GcObjectArray.Append (pObject);
}

void
CRuntime::AddGcRoot (
	void* p,
	CType* pType
	)
{
	ASSERT (m_GcState == EGcState_Mark);
	ASSERT (pType->GetFlags () & ETypeFlag_GcRoot);

	TGcRoot Root = { p, pType };
	m_GcRootArray [m_CurrentGcRootArrayIdx].Append (Root);
}

void
CRuntime::RunGc_l ()
{
	m_GcIdleEvent.Reset ();	

	// 1) suspend all mutator threads at safe points

	m_GcState = EGcState_WaitSafePoint;

	ASSERT (m_GcUnsafeThreadCount); 
	m_GcSafePointEvent.Reset ();	
	intptr_t UnsafeCount = mt::AtomicDec (&m_GcUnsafeThreadCount);
	if (UnsafeCount)
	{
		m_Lock.Unlock ();
		m_GcSafePointEvent.Wait ();
		m_Lock.Lock ();
	}

	// 2) mark

	m_GcState = EGcState_Mark;

	m_GcRootArray [0].Clear ();
	m_CurrentGcRootArrayIdx = 0;

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

	// 2.2) add destructible roots

	// 2.2) add stack roots

	rtl::CIteratorT <TTlsHdr> Tls = m_TlsList.GetHead ();
	for (; Tls; Tls++)
	{
		TTls* pTls = (TTls*) (*Tls + 1);

		TGcShadowStackFrame* pStackFrame = pTls->m_pGcShadowStackTop;
		for (; pStackFrame; pStackFrame = pStackFrame->m_pNext)
		{
			size_t Count = pStackFrame->m_pMap->m_Count;
			void** ppRootArray = (void**) (pStackFrame + 1);
			CType** ppTypeArray = (CType**) (pStackFrame->m_pMap + 1);

			for (size_t i = 0; i < Count; i++)
			{
				void* p = ppRootArray [i];
				CType* pType = ppTypeArray [i];

				if (p) // check needed, stack roots could be nullified
					AddGcRoot (p, pType);
			}
		}
	}

	// 2.3) run mark cycle

	GcMarkCycle ();

	// 3) sweep

	m_GcState = EGcState_Sweep;

	// 3.1) mark objects as dead and schedule destruction

	char Buffer1 [256];
	char Buffer2 [256];
	rtl::CArrayT <TObject*> DestructArray (ref::EBuf_Stack, Buffer1, sizeof (Buffer1));
	TGcDestructGuard DestructGuard (ref::EBuf_Stack, Buffer2, sizeof (Buffer2));

	Count = m_GcObjectArray.GetCount ();
	for (intptr_t i = Count - 1; i >= 0; i--)
	{
		jnc::TObject* pObject = m_GcObjectArray [i];
		
		if (!(pObject->m_Flags & (EObjectFlag_GcMark | EObjectFlag_GcWeakMark_c)))
		{
			m_GcObjectArray.Remove (i);
			pObject->m_Flags |= EObjectFlag_Dead;
			
			if (pObject->m_pClassType->GetDestructor ())
			{
				pObject->m_ScopeLevel = 1; // prevent resurrection
				DestructArray.Append (pObject);

				if (pObject->m_pType->GetFlags () & ETypeFlag_GcRoot)
				{
					TGcRoot GcRoot = { pObject, pObject->m_pType };
					DestructGuard.m_GcRootArray.Append (GcRoot);
				}
			}
		}
	}
	
	if (!DestructGuard.m_GcRootArray.IsEmpty ())
		m_GcDestructGuardList.InsertTail (&DestructGuard);

	// 3.2) mark all destruct guard gc roots

	rtl::CIteratorT <TGcDestructGuard> It = m_GcDestructGuardList.GetHead ();
	for (; It; It++)
	{
		TGcDestructGuard* pDestructGuard = *It;

		Count = pDestructGuard->m_GcRootArray.GetCount ();
		for (size_t i = 0; i < Count; i++)
		{
			ASSERT (pDestructGuard->m_GcRootArray [i].m_pType->GetFlags () & ETypeFlag_GcRoot);
			AddGcRoot (
				pDestructGuard->m_GcRootArray [i].m_p,
				pDestructGuard->m_GcRootArray [i].m_pType
				);
		}
	}

	GcMarkCycle ();

	// 3.4) free memory blocks

	Count = m_GcMemBlockArray.GetCount ();
	for (intptr_t i = Count - 1; i >= 0; i--)
	{
		TObject* pObject = m_GcMemBlockArray [i];
		if (!(pObject->m_Flags & EObjectFlag_GcWeakMark))
		{
			m_GcMemBlockArray.Remove (i);
			AXL_MEM_FREE (pObject);
		}
		else
		{
			pObject->m_Flags &= ~EObjectFlag_GcMask; // unmark
		}
	}

	// 3.5) unmark the remaining objects

	Count = m_GcObjectArray.GetCount ();
	for (intptr_t i = Count - 1; i >= 0; i--)
	{
		jnc::TObject* pObject = m_GcObjectArray [i];
		pObject->m_Flags &= ~EObjectFlag_GcMask;
	}

	// 4) gc run is done, resume all suspended threads

	mt::AtomicInc (&m_GcUnsafeThreadCount);
	m_PeriodGcAllocSize = 0;
	m_GcState = EGcState_Idle;
	m_GcIdleEvent.Signal ();	
	m_Lock.Unlock ();

	// 5) run destructors 

	if (!DestructArray.IsEmpty ())
	{
		Count = DestructArray.GetCount ();
		for (size_t i = 0; i < Count; i++)
		{
			TObject* pObject = DestructArray [i];
			CFunction* pDestructor = pObject->m_pClassType->GetDestructor ();
			ASSERT (pDestructor);

			FObject_Destruct* pf = (FObject_Destruct*) pDestructor->GetMachineCode ();
			pf ((jnc::TInterface*) (pObject + 1));
		}

		if (!DestructGuard.m_GcRootArray.IsEmpty ())
		{
			m_Lock.Lock ();
			m_GcDestructGuardList.Remove (&DestructGuard);
			m_Lock.Unlock ();
		}
	}
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
			pRoot->m_pType->GcMark (this, pRoot->m_p);
		}
	}
}

void
CRuntime::GcEnter ()
{
	TTlsHdr* pTls = GetTlsMgr ()->GetTls (this);
	ASSERT (pTls);
	if (pTls->m_Flags & ETlsFlag_GcUnsafe) // already
		return; 

	// what we try to prevent here is entering an unsafe region when collector thread 
	// thinks all the mutators are parked at safe regions and therefore moves on to mark/sweep

	for (;;)
	{
		if (m_GcState != EGcState_Idle)
			m_GcIdleEvent.Wait ();

		intptr_t OldCount = m_GcUnsafeThreadCount;
		if (OldCount == 0) // oh-oh -- we started gc run in between these two 'if's
			continue;

		intptr_t NewCount = OldCount + 1;
		intptr_t PrevCount = mt::AtomicCmpXchg (&m_GcUnsafeThreadCount, OldCount, NewCount);
		if (PrevCount == OldCount)
			break;
	}

	pTls->m_Flags |= ETlsFlag_GcUnsafe;
}

void
CRuntime::GcLeave ()
{
	ASSERT (m_GcState == EGcState_Idle || m_GcState == EGcState_WaitSafePoint);

	TTlsHdr* pTls = GetTlsMgr ()->GetTls (this);
	ASSERT (pTls && (pTls->m_Flags & ETlsFlag_GcUnsafe));

	pTls->m_Flags &= ~ETlsFlag_GcUnsafe;
	intptr_t Count = mt::AtomicDec (&m_GcUnsafeThreadCount);
	
	if (m_GcState == EGcState_WaitSafePoint)
	{
		if (!Count)
			m_GcSafePointEvent.Signal ();

		m_GcIdleEvent.Wait ();
	}
}

void
CRuntime::GcPulse ()
{
	ASSERT (m_GcState == EGcState_Idle || m_GcState == EGcState_WaitSafePoint);

	if (m_GcState == EGcState_WaitSafePoint)
	{
		GcLeave ();
		GcEnter ();
	}
}

void
CRuntime::WaitGcIdleAndLock ()
{
	TTlsHdr* pTls = GetTlsMgr ()->GetTls (this);
	ASSERT (pTls && !(pTls->m_Flags & ETlsFlag_GcUnsafe)); // unsafe == deadlock

	for (;;)
	{
		m_Lock.Lock ();

		if (m_GcState == EGcState_Idle)
			break;

		m_Lock.Unlock ();
		m_GcIdleEvent.Wait ();
	}
}

TTlsHdr*
CRuntime::GetTls ()
{
	TTlsHdr* pTls = GetTlsMgr ()->GetTls (this);
	ASSERT (pTls);

	// check for stack overflow

	char* p = (char*) _alloca (1);

	if (!pTls->m_pStackEpoch) // first time call
	{
		pTls->m_pStackEpoch = p;
	}
	else
	{
		char* p0 = (char*) pTls->m_pStackEpoch;
		if (p0 >= p) // the opposite could happen, but it's stack-overflow-safe
		{
			size_t StackSize = p0 - p;
			if (StackSize > m_StackLimit)
			{
				CStdLib::RuntimeError (ERuntimeError_StackOverflow, NULL, NULL);
				ASSERT (false);
			}
		}
	}

	return pTls;
}

TTlsHdr*
CRuntime::CreateTls ()
{
	size_t Size = sizeof (TTlsHdr) + m_TlsSize;

	TTlsHdr* pTls = (TTlsHdr*) AXL_MEM_ALLOC (Size);
	memset (pTls, 0, Size);
	pTls->m_pRuntime = this;
	pTls->m_pStackEpoch = NULL;

	WaitGcIdleAndLock ();
	m_TlsList.InsertTail (pTls);
	m_Lock.Unlock ();

	return pTls;
}

void
CRuntime::DestroyTls (TTlsHdr* pTls)
{
	WaitGcIdleAndLock ();
	m_TlsList.Remove (pTls);
	m_Lock.Unlock ();

	AXL_MEM_FREE (pTls);
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

	TObject* pObject = (TObject*) GcAllocateObject (pType);
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
