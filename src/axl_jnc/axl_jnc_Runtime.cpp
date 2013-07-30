#include "pch.h"
#include "axl_jnc_Runtime.h"
#include "axl_jnc_Module.h"

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

// Memory manager for MCJIT (from lli)

class CJitMemoryManager : public JITMemoryManager {
public:
  CStdLib* m_pStdLib;
	
  SmallVector<sys::MemoryBlock, 16> AllocatedDataMem;
  SmallVector<sys::MemoryBlock, 16> AllocatedCodeMem;
  SmallVector<sys::MemoryBlock, 16> FreeCodeMem;

  CJitMemoryManager() 
  { 
	  m_pStdLib = NULL;
  }
  
  ~CJitMemoryManager();

  virtual uint8_t *allocateCodeSection(uintptr_t Size, unsigned Alignment,
									   unsigned SectionID);

  virtual uint8_t *allocateDataSection(uintptr_t Size, unsigned Alignment,
									   unsigned SectionID);

  virtual void *getPointerToNamedFunction(const std::string &Name,
										  bool AbortOnFailure = true);

  // Invalidate instruction cache for code sections. Some platforms with
  // separate data cache and instruction cache require explicit cache flush,
  // otherwise JIT code manipulations (like resolved relocations) will get to
  // the data cache but not to the instruction cache.
  virtual void invalidateInstructionCache();

  // The RTDyldMemoryManager doesn't use the following functions, so we don't
  // need implement them.
  virtual void setMemoryWritable() {
	llvm_unreachable("Unexpected call!");
  }
  virtual void setMemoryExecutable() {
	llvm_unreachable("Unexpected call!");
  }
  virtual void setPoisonMemory(bool poison) {
	llvm_unreachable("Unexpected call!");
  }
  virtual void AllocateGOT() {
	llvm_unreachable("Unexpected call!");
  }
  virtual uint8_t *getGOTBase() const {
	llvm_unreachable("Unexpected call!");
	return 0;
  }
  virtual uint8_t *startFunctionBody(const Function *F,
									 uintptr_t &ActualSize){
	llvm_unreachable("Unexpected call!");
	return 0;
  }
  virtual uint8_t *allocateStub(const GlobalValue* F, unsigned StubSize,
								unsigned Alignment) {
	llvm_unreachable("Unexpected call!");
	return 0;
  }
  virtual void endFunctionBody(const Function *F, uint8_t *FunctionStart,
							   uint8_t *FunctionEnd) {
	llvm_unreachable("Unexpected call!");
  }
  virtual uint8_t *allocateSpace(intptr_t Size, unsigned Alignment) {
	llvm_unreachable("Unexpected call!");
	return 0;
  }
  virtual uint8_t *allocateGlobal(uintptr_t Size, unsigned Alignment) {
	llvm_unreachable("Unexpected call!");
	return 0;
  }
  virtual void deallocateFunctionBody(void *Body) {
	llvm_unreachable("Unexpected call!");
  }
  virtual uint8_t* startExceptionTable(const Function* F,
									   uintptr_t &ActualSize) {
	llvm_unreachable("Unexpected call!");
	return 0;
  }
  virtual void endExceptionTable(const Function *F, uint8_t *TableStart,
								 uint8_t *TableEnd, uint8_t* FrameRegister) {
	llvm_unreachable("Unexpected call!");
  }
  virtual void deallocateExceptionTable(void *ET) {
	llvm_unreachable("Unexpected call!");
  }
};

uint8_t *CJitMemoryManager::allocateDataSection(uintptr_t Size,
													unsigned Alignment,
													unsigned SectionID) {
  if (!Alignment)
	Alignment = 16;
  // Ensure that enough memory is requested to allow aligning.
  size_t NumElementsAligned = 1 + (Size + Alignment - 1)/Alignment;
  uint8_t *Addr = (uint8_t*)calloc(NumElementsAligned, Alignment);

  // Honour the alignment requirement.
  uint8_t *AlignedAddr = (uint8_t*)RoundUpToAlignment((uint64_t)Addr, Alignment);

  // Store the original address from calloc so we can free it later.
  AllocatedDataMem.push_back(sys::MemoryBlock(Addr, NumElementsAligned*Alignment));
  return AlignedAddr;
}

uint8_t *CJitMemoryManager::allocateCodeSection(uintptr_t Size,
													unsigned Alignment,
													unsigned SectionID) {
  if (!Alignment)
	Alignment = 16;
  unsigned NeedAllocate = Alignment * ((Size + Alignment - 1)/Alignment + 1);
  uintptr_t Addr = 0;
  // Look in the list of free code memory regions and use a block there if one
  // is available.
  for (int i = 0, e = FreeCodeMem.size(); i != e; ++i) {
	sys::MemoryBlock &MB = FreeCodeMem[i];
	if (MB.size() >= NeedAllocate) {
	  Addr = (uintptr_t)MB.base();
	  uintptr_t EndOfBlock = Addr + MB.size();
	  // Align the address.
	  Addr = (Addr + Alignment - 1) & ~(uintptr_t)(Alignment - 1);
	  // Store cutted free memory block.
	  FreeCodeMem[i] = sys::MemoryBlock((void*)(Addr + Size),
										EndOfBlock - Addr - Size);
	  return (uint8_t*)Addr;
	}
  }

  // No pre-allocated free block was large enough. Allocate a new memory region.
  sys::MemoryBlock MB = sys::Memory::AllocateRWX(NeedAllocate, 0, 0);

  AllocatedCodeMem.push_back(MB);
  Addr = (uintptr_t)MB.base();
  uintptr_t EndOfBlock = Addr + MB.size();
  // Align the address.
  Addr = (Addr + Alignment - 1) & ~(uintptr_t)(Alignment - 1);
  // The AllocateRWX may allocate much more memory than we need. In this case,
  // we store the unused memory as a free memory block.
  unsigned FreeSize = EndOfBlock-Addr-Size;
  if (FreeSize > 16)
	FreeCodeMem.push_back(sys::MemoryBlock((void*)(Addr + Size), FreeSize));

  // Return aligned address
  return (uint8_t*)Addr;
}

void CJitMemoryManager::invalidateInstructionCache() {
  for (int i = 0, e = AllocatedCodeMem.size(); i != e; ++i)
	sys::Memory::InvalidateInstructionCache(AllocatedCodeMem[i].base(),
											AllocatedCodeMem[i].size());
}

void *CJitMemoryManager::getPointerToNamedFunction(const std::string &Name,
													   bool AbortOnFailure) {

	void* pf = m_pStdLib->FindFunction (Name.c_str ());
	if (pf)
		return pf;
	
  if (AbortOnFailure)
	report_fatal_error("Program used external function '" + Name +
					  "' which could not be resolved!");
  return 0;
}

CJitMemoryManager::~CJitMemoryManager() {
  for (unsigned i = 0, e = AllocatedCodeMem.size(); i != e; ++i)
	sys::Memory::ReleaseRWX(AllocatedCodeMem[i]);
  for (unsigned i = 0, e = AllocatedDataMem.size(); i != e; ++i)
	free(AllocatedDataMem[i].base());
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

	m_TlsSlot = -1;
	m_TlsSize = 0;

	m_GcIdleEvent.Signal ();
}

bool
CRuntime::Create (
	CModule* pModule,
	CStdLib* pStdLib,
	EJit JitKind,
	size_t HeapBlockSize,
	size_t HeapWidth,
	size_t HeapHeight
	)
{
	Destroy ();

	m_pModule = pModule;

	// execution engine 


	llvm::EngineBuilder EngineBuilder (pModule->GetLlvmModule ());	
	
	std::string errorString;
	EngineBuilder.setErrorStr (&errorString);
	EngineBuilder.setEngineKind(llvm::EngineKind::JIT);

	if (JitKind == EJit_McJit)
	{
		llvm::TargetOptions TargetOptions;
		TargetOptions.JITEmitDebugInfo = true;
	
		CJitMemoryManager* pJitMemoryManager = new CJitMemoryManager;
		pJitMemoryManager->m_pStdLib = pStdLib;

		EngineBuilder.setUseMCJIT (true);
		EngineBuilder.setTargetOptions (TargetOptions);
		EngineBuilder.setJITMemoryManager (pJitMemoryManager);
	}
	
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

	bool Result = m_GcMap.Create (HeapWidth, HeapHeight);
	if (!Result)
		return false;

	size_t BlockCount = m_GcMap.GetTotalSize ();
	size_t HeapSize = BlockCount * HeapBlockSize;

	void* pHeap = AXL_MEM_ALLOC (HeapSize);
	if (!pHeap)
		return false;

	m_GcHeapSize = HeapSize;
	m_pGcHeap = pHeap;
	m_GcBlockSize = HeapBlockSize;

	// static gc roots

	rtl::CArrayT <CVariable*> StaticRootArray = pModule->m_VariableMgr.GetStaticGcRootArray ();
	size_t Count = StaticRootArray.GetCount ();

	m_StaticGcRootArray.SetCount (Count);
	for (size_t i = 0; i < Count; i++)
	{
		CVariable* pVariable = StaticRootArray [i];
		void* p = m_pLlvmExecutionEngine->getPointerToGlobal ((llvm::GlobalVariable*) pVariable->GetLlvmValue ());
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
		GetTlsMgr ()->DestroySlot (m_TlsSlot);

	if (m_pLlvmExecutionEngine)
		delete m_pLlvmExecutionEngine;

	m_pLlvmExecutionEngine = NULL;
	m_TlsSlot = -1;
	m_TlsSize = 0;

	if (m_pGcHeap)
		AXL_MEM_FREE (m_pGcHeap);

	m_pGcHeap = NULL;
	m_GcBlockSize = 0;
	m_GcMap.Clear ();
	m_GcObjectList.Clear ();
	m_GcDestructList.Clear ();
	m_StaticGcRootArray.Clear ();

	m_pModule = NULL;
}

bool
CRuntime::Startup ()
{
	m_TerminateGcDestructThread = false;
	m_GcDestructThread.Start ();
	return true;
}

void
CRuntime::Shutdown ()
{
	rtl::CArrayT <TGcRoot> SaveStaticGcRootArray = m_StaticGcRootArray;
	m_StaticGcRootArray.Clear ();

	RunGc ();

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
			err::SetFormatStringError ("unable to allocate %d bytes", Size);
			return NULL;
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
	printf ("GcAddObject %08x: %s\n", pObject, pObject->m_pType->GetTypeString ().cc ());

	m_GcObjectList.InsertTail (pObject);

	rtl::CArrayT <CStructField*> ClassFieldArray = pObject->m_pType->GetClassMemberFieldArray ();
	size_t Count = ClassFieldArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = ClassFieldArray [i];
		TObject* pChildObject = (TObject*) ((char*) pObject + pField->GetOffset ());
		GcAddObject (pChildObject);
	}
}

void 
CRuntime::MarkGcValue (
	void* p,
	CType* pType
	)
{
	MarkGcRange (p, pType->GetSize ());

	if (pType->GetTypeKind () == EType_Class)
		((TObject*) p)->m_Flags |= EObjectFlag_GcMark;

	if (pType->GetFlags () & ETypeFlag_GcRoot)
		AddGcRoot (p, pType);
}

void
CRuntime::AddGcRoot (
	void* p,
	CType* pType
	)
{
	ASSERT (m_GcState == EGcState_Mark);

	TGcRoot Root;
	Root.m_p = p;
	Root.m_pType = pType;
	m_GcRootArray [m_CurrentGcRootArrayIdx].Append (Root);
}

void
CRuntime::RunGc ()
{
	printf ("Running GC...\n");
	
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

	// 2) add static roots

	size_t Count = m_StaticGcRootArray.GetCount ();	
	for (size_t i = 0; i < Count; i++)
	{
		ASSERT (m_StaticGcRootArray [i].m_pType->GetFlags () & ETypeFlag_GcRoot);
		AddGcRoot (
			m_StaticGcRootArray [i].m_p, 
			m_StaticGcRootArray [i].m_pType
			);
	}

	// 3) add stack roots 

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

	// 4) keep enumerating & marking (breadth first) until done

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
		
	// 4) remove 'alive' flag from unmarked objects and schedule destruction

	rtl::CAuxListT <TObject, CObjectHeapLink> DestructList;	

	rtl::CIteratorT <TObject, CObjectHeapLink> Object = m_GcObjectList.GetTail ();
	while (Object)
	{
		rtl::CIteratorT <TObject, CObjectHeapLink> Next = Object.GetInc (-1);
		
		if (Object->m_Flags & EObjectFlag_GcMark)
		{
			Object->m_Flags &= ~EObjectFlag_GcMark;
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

	if (!DestructList.IsEmpty ())
	{
		m_GcDestructList.InsertListTail (&DestructList);
		m_GcDestructEvent.Signal ();
	}

	// 5) resume all suspended threads

	m_GcState = EGcState_Idle;
	m_GcIdleEvent.Signal ();
	m_Lock.Unlock ();
}

void
CRuntime::DestructThreadProc ()
{
	CScopeThreadRuntime ScopeRuntime (this);
	GetTlsMgr ()->GetTlsData (this); // register thread right away

	while (!m_TerminateGcDestructThread)
	{
		m_GcDestructEvent.Wait ();
		
		WaitGcIdleAndLock ();
		while (!m_GcDestructList.IsEmpty ())
		{
			rtl::CIteratorT <TObject, CObjectHeapLink> Object = m_GcDestructList.GetHead ();
			m_Lock.Unlock ();

			CFunction* pDestructor = Object->m_pType->GetDestructor ();
			ASSERT (pDestructor);

			typedef void (*FDestructor) (void*);
			FDestructor pf = (FDestructor) pDestructor->GetMachineCode ();
			pf (*Object + 1);

			WaitGcIdleAndLock ();
			m_GcDestructList.RemoveHead ();
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
	if (!pTlsData)
		throw err::GetError ();

	return pTlsData + 1;
}

TTlsData*
CRuntime::CreateTlsData ()
{
	size_t Size = sizeof (TTlsData) + m_TlsSize;

	TTlsData* pTlsData = (TTlsData*) AXL_MEM_ALLOC (Size);
	memset (pTlsData, 0, Size);
	pTlsData->m_pRuntime = this;

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

//.............................................................................

} // namespace axl 
} // namespace jnc 
