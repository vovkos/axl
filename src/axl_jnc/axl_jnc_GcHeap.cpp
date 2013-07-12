#include "pch.h"
#include "axl_jnc_GcHeap.h"
#include "axl_jnc_GcStrategy.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Runtime.h"

namespace axl {
namespace jnc {

//.............................................................................

CGcHeap::CGcHeap ()
{
	m_pHeap = NULL;
	m_HeapSize = 0;
	m_BlockSize = 0;
	m_CurrentRootArrayIdx = 0;
	m_pShadowStack = NULL;
}

bool
CGcHeap::Create (
	CRuntime* pRuntime,
	size_t BlockSize,
	size_t Width,
	size_t Height
	)
{
	Clear ();

	bool Result = m_Map.Create (Width, Height);
	if (!Result)
		return false;

	size_t BlockCount = m_Map.GetTotalSize ();
	size_t HeapSize = BlockCount * BlockSize;

	void* pHeap = AXL_MEM_ALLOC (HeapSize);
	if (!pHeap)
		return false;

	m_HeapSize = HeapSize;
	m_pHeap = pHeap;
	m_BlockSize = BlockSize;

	CModule* pModule = pRuntime->GetModule ();
	llvm::ExecutionEngine* pLlvmExecutionEngine = pRuntime->GetLlvmExecutionEngine ();

	// static roots

	rtl::CArrayT <CVariable*> StaticRootArray = pModule->m_VariableMgr.GetStaticGcRootArray ();
	size_t Count = StaticRootArray.GetCount ();

	m_StaticRootArray.SetCount (Count);
	for (size_t i = 0; i < Count; i++)
	{
		CVariable* pVariable = StaticRootArray [i];
		void* p = pLlvmExecutionEngine->getPointerToGlobal ((llvm::GlobalVariable*) pVariable->GetLlvmValue ());
		ASSERT (p);

		m_StaticRootArray [i].m_p = p;
		m_StaticRootArray [i].m_pType = pVariable->GetType ();
	}

	// stack roots

	llvm::GlobalVariable* pLlvmVariable = pModule->GetLlvmModule ()->getGlobalVariable ("llvm_gc_root_chain");
	ASSERT (pLlvmVariable); 

	m_pShadowStack = pLlvmExecutionEngine->getPointerToGlobal (pLlvmVariable);
	ASSERT (m_pShadowStack);

	return true;
}

void
CGcHeap::Clear ()
{
	if (!m_pHeap)
		return;

	AXL_MEM_FREE (m_pHeap);

	m_pHeap = NULL;
	m_BlockSize = 0;
	m_Map.Clear ();
	m_ObjectList.Clear ();
	m_StaticRootArray.Clear ();
}

void*
CGcHeap::Allocate (CType* pType)
{
	ASSERT (m_pHeap);

	size_t Size = pType->GetSize ();
	size_t BlockCount = GetBlockCount (Size);

	size_t Address = m_Map.Allocate (BlockCount);
	if (Address == -1)
	{
		err::SetFormatStringError ("unable to allocate %d bytes", Size);
		return NULL;
	}

	return (char*) m_pHeap + Address * m_BlockSize;
}

void
CGcHeap::AddObject (TObject* pObject)
{
	printf ("AddObject %08x: %s\n", pObject, pObject->m_pType->GetTypeString ().cc ());

	m_ObjectList.InsertTail (pObject);

	rtl::CArrayT <CStructField*> ClassFieldArray = pObject->m_pType->GetClassMemberFieldArray ();
	size_t Count = ClassFieldArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CStructField* pField = ClassFieldArray [i];
		TObject* pChildObject = (TObject*) ((char*) pObject + pField->GetOffset ());
		AddObject (pChildObject);
	}
}

void 
CGcHeap::MarkValue (
	void* p,
	CType* pType
	)
{
	MarkRange (p, pType->GetSize ());

	if (pType->GetTypeKind () == EType_Class)
		((TObject*) p)->m_Flags |= EObjectFlag_GcMark;

	if (pType->GetFlags () & ETypeFlag_GcRoot)
		AddRoot (p, pType);
}

bool
CGcHeap::RunGc ()
{
	printf ("Running GC...\n");

	m_Map.Clear ();
	m_RootArray [0].Clear ();
	m_CurrentRootArrayIdx = 0;

	// 1) suspend all threads at safe points
	
	// 2) enumerate static roots

	size_t Count = m_StaticRootArray.GetCount ();	
	for (size_t i = 0; i < Count; i++)
	{
		ASSERT (m_StaticRootArray [i].m_pType->GetFlags () & ETypeFlag_GcRoot);
		AddRoot (
			m_StaticRootArray [i].m_p, 
			m_StaticRootArray [i].m_pType
			);
	}

	// 3) enumerate tls roots 

	// 3) enumerate stack roots 

	StackEntry* pStackEntry = *(StackEntry**) m_pShadowStack;
	for (; pStackEntry; pStackEntry = pStackEntry->Next)
	{
		ASSERT (pStackEntry->Map->NumMeta == pStackEntry->Map->NumRoots); // all should have meta

		size_t Count = pStackEntry->Map->NumMeta;		
		for (size_t i = 0; i < Count; i++)
		{
			void* p = pStackEntry->Roots [i];
			CType* pType = (CType*) pStackEntry->Map->Meta [i];

			if (!p)
				continue;

			if (ShouldMark (p))
				MarkRange (p, pType->GetSize ());

			if (pType->GetTypeKind () == EType_Class)
				((TObject*) p)->m_Flags |= EObjectFlag_GcMark;

			if (pType->GetFlags () & ETypeFlag_GcRoot)
				AddRoot (p, pType);
		}
	}

	// 4) keep enumerating & marking (breadth first) until done
		
	while (!m_RootArray [m_CurrentRootArrayIdx].IsEmpty ())
	{
		size_t PrevCurrentRootArrayIdx =  m_CurrentRootArrayIdx;
		m_CurrentRootArrayIdx = !m_CurrentRootArrayIdx;
		m_RootArray [m_CurrentRootArrayIdx].Clear ();		

		size_t Count = m_RootArray [PrevCurrentRootArrayIdx].GetCount ();		
		for (size_t i = 0; i < Count; i++)
		{
			const TRoot* pRoot = &m_RootArray [PrevCurrentRootArrayIdx] [i];
			pRoot->m_pType->EnumGcRoots (this, pRoot->m_p);
		}
	}

	// 4) tag alive, destruct dead

	rtl::CIteratorT <TObject, CObjectHeapLink> Object = m_ObjectList.GetTail ();
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
			m_ObjectList.Remove (Object);
			
			CFunction* pDestructor = Object->m_pType->GetDestructor ();
			if (pDestructor)
			{
				typedef void (*FDestructor) (void*);
				FDestructor pf = (FDestructor) pDestructor->GetMachineCode ();
				pf (*Object + 1);
			}
		}

		Object = Next;
	}

	// 5) resume all suspended threads

	printf ("Done.\n");

	return true;
}

//.............................................................................

} // namespace axl
} // namespace jnc