#include "pch.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Runtime.h"
#include "axl_jnc_Multicast.h"

namespace axl {
namespace jnc {

//.............................................................................

void
CStdLib::Export (
	CModule* pModule,
	llvm::ExecutionEngine* pLlvmExecutionEngine
	)
{
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_OnRuntimeError, (void*) OnRuntimeError);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_DynamicCastClassPtr, (void*) DynamicCastClassPtr);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_StrengthenClassPtr, (void*) StrengthenClassPtr);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_HeapAlloc, (void*) HeapAlloc);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_UHeapAlloc, (void*) UHeapAlloc);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_UHeapFree, (void*) UHeapFree);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_UHeapFreeClassPtr, (void*) UHeapFreeClassPtr);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_GcAddObject, (void*) GcAddObject);

	// implementation for thin and unsafe is the same

	rtl::CConstListT <CMulticastClassType> McTypeList = pModule->m_TypeMgr.GetMulticastClassTypeList ();
	rtl::CIteratorT <CMulticastClassType> McType = McTypeList.GetHead ();
	for (; McType; McType++)
	{
		ExportMulticastMethods (
			pModule, 
			pLlvmExecutionEngine, 
			*McType
			);
	}


/*	

	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastSet,   (void*) MulticastSet);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastSet_w, (void*) MulticastSet_w);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastSet_t, (void*) MulticastSet_u);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastSet_u, (void*) MulticastSet_u);

	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastAdd,   (void*) MulticastAdd);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastAdd_w, (void*) MulticastAdd_w);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastAdd_t, (void*) MulticastAdd_u);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastAdd_u, (void*) MulticastAdd_u);

	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastRemove,   (void*) MulticastRemove);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastRemove_w, (void*) MulticastRemove_w);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastRemove_t, (void*) MulticastRemove_u);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastRemove_u, (void*) MulticastRemove_u);

	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastSnapshot,   (void*) MulticastSnapshot);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastSnapshot_w, (void*) MulticastSnapshot_w);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastSnapshot_t, (void*) MulticastSnapshot_u);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_MulticastSnapshot_u, (void*) MulticastSnapshot_u);

*/
	pModule->SetFunctionPointer (pLlvmExecutionEngine, "GetCurrentThreadId", (void*) GetCurrentThreadId);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, "CreateThread", (void*) CreateThread);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, "RunGc", (void*) RunGc);
}

void
CStdLib::OnRuntimeError (
	int Error,
	void* pCodeAddr,
	void* pDataAddr
	)
{
	const char* pErrorString;

	switch (Error)
	{
	case jnc::ERuntimeError_DataPtrOutOfRange:
		pErrorString = "DATA_PTR_OOR";
		break;

	case jnc::ERuntimeError_ScopeMismatch:
		pErrorString = "SCOPE_MISMATCH";
		break;

	case jnc::ERuntimeError_NullClassPtr:
		pErrorString = "NULL_CLASS_PTR";
		break;

	case jnc::ERuntimeError_NullFunctionPtr:
		pErrorString = "NULL_FUNCTION_PTR";
		break;

	case jnc::ERuntimeError_NullPropertyPtr:
		pErrorString = "NULL_PROPERTY_PTR";
		break;

	case jnc::ERuntimeError_AbstractFunction:
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

jnc::TInterface* 
CStdLib::DynamicCastClassPtr (
	jnc::TInterface* p,
	jnc::CClassType* pType
	)
{
	if (!p)
		return NULL;

	if (p->m_pObject->m_pType->Cmp (pType) == 0)
		return p;
	
	jnc::CBaseTypeCoord Coord;
	bool Result = p->m_pObject->m_pType->FindBaseTypeTraverse (pType, &Coord);
	if (!Result)
		return NULL;
	
	jnc::TInterface* p2 = (jnc::TInterface*) ((uchar_t*) (p->m_pObject + 1) + Coord.m_Offset);
	ASSERT (p2->m_pObject == p->m_pObject);
	return p2;
}

jnc::TInterface* 
CStdLib::StrengthenClassPtr (jnc::TInterface* p)
{
	if (!p)
		return NULL;

	EClassType ClassTypeKind = p->m_pObject->m_pType->GetClassTypeKind ();
	switch (ClassTypeKind)
	{
	case EClassType_FunctionClosure:
		return ((CFunctionClosureClassType*) p->m_pObject->m_pType)->Strengthen (p);

	case EClassType_PropertyClosure:
		return ((CPropertyClosureClassType*) p->m_pObject->m_pType)->Strengthen (p);

	default:
		return (p->m_pObject->m_Flags & EObjectFlag_Alive) ? p : NULL;
	}
}

void
CStdLib::MulticastClear (jnc::TMulticast* pMulticast)
{
	return ((CMulticast*) pMulticast)->Clear ();
}

handle_t
CStdLib::MulticastSet (
	jnc::TMulticast* pMulticast,
	jnc::TFunctionPtr Ptr
	)
{
	return ((CMulticast*) pMulticast)->SetHandler (Ptr);
}

handle_t
CStdLib::MulticastSet_t (
	jnc::TMulticast* pMulticast,
	void* pf
	)
{
	return ((CMulticast*) pMulticast)->SetHandler_t (pf);
}

handle_t
CStdLib::MulticastAdd (
	jnc::TMulticast* pMulticast,
	jnc::TFunctionPtr Ptr
	)
{
	return ((CMulticast*) pMulticast)->AddHandler (Ptr);
}

handle_t
CStdLib::MulticastAdd_t (
	jnc::TMulticast* pMulticast,
	void* pf
	)
{
	return ((CMulticast*) pMulticast)->AddHandler_t (pf);
}

jnc::TFunctionPtr
CStdLib::MulticastRemove (
	jnc::TMulticast* pMulticast,
	handle_t Handle
	)
{
	return ((CMulticast*) pMulticast)->RemoveHandler (Handle);
}

void*
CStdLib::MulticastRemove_t (
	jnc::TMulticast* pMulticast,
	handle_t Handle
	)
{
	return ((CMulticast*) pMulticast)->RemoveHandler_t (Handle);
}

jnc::TFunctionPtr
CStdLib::MulticastGetSnapshot (jnc::TMulticast* pMulticast)
{
	return ((CMulticast*) pMulticast)->GetSnapshot ();
}

void*
CStdLib::HeapAlloc (jnc::CType* pType)
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	return pRuntime->m_GcHeap.Allocate (pType);
}

void*
CStdLib::UHeapAlloc (jnc::CType* pType)
{
	void* p = malloc (pType->GetSize ());
	memset (p, 0, pType->GetSize ());
	return p;
}

void
CStdLib::UHeapFree (void* p)
{
	free (p);
}

void
CStdLib::UHeapFreeClassPtr (jnc::TInterface* p)
{
	free (p->m_pObject);
}

void
CStdLib::GcAddObject (jnc::TObject* p)
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	pRuntime->m_GcHeap.AddObject (p);
}

void
CStdLib::RunGc ()
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	pRuntime->m_GcHeap.RunGc ();
}

#if (_AXL_ENV == AXL_ENV_WIN)

dword_t
CStdLib::GetCurrentThreadId ()
{
	return ::GetCurrentThreadId ();
}

DWORD 
WINAPI
CStdLib::ThreadProc (PVOID pContext)
{
	jnc::TFunctionPtr* pPtr = (jnc::TFunctionPtr*) pContext;
	jnc::TFunctionPtr Ptr = *pPtr;
	AXL_MEM_DELETE (pPtr);

	((void (__cdecl*) (jnc::TInterface*)) Ptr.m_pf) (Ptr.m_pClosure);
	return 0;
}

int
CStdLib::CreateThread (jnc::TFunctionPtr Ptr)
{
	jnc::TFunctionPtr* pPtr = AXL_MEM_NEW (jnc::TFunctionPtr);
	*pPtr = Ptr;

	DWORD ThreadId;
	HANDLE h = ::CreateThread (NULL, 0, CStdLib::ThreadProc, pPtr, 0, &ThreadId);
	return h != NULL;
}

#elif (_AXL_ENV == AXL_ENV_POSIX)

dword_t
CStdLib::GetCurrentThreadId ()
{
	return 0; // TODO
}

int
CStdLib::CreateThread (jnc::TFunctionPtr Ptr)
{
	return 0; // TODO
}

#endif

void*
CStdLib::GetThreadVariableData ()
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	return GetTlsMgr ()->GetThreadVariableData (pRuntime->GetTlsSlot (), pRuntime->GetTlsSize ());
}

void
CStdLib::ExportMulticastMethods (
	CModule* pModule,
	llvm::ExecutionEngine* pLlvmExecutionEngine,
	CMulticastClassType* pMulticastType
	)
{
	static 
	void*
	MethodTable [3] [5] = 
	{
		{
			(void*) MulticastClear,
			(void*) MulticastSet,
			(void*) MulticastAdd,
			(void*) MulticastRemove,
			(void*) MulticastGetSnapshot,
		},

		{
			(void*) MulticastClear,
			(void*) MulticastSet,
			(void*) MulticastAdd,
			(void*) MulticastRemove,
			(void*) MulticastGetSnapshot,
		},

		{
			(void*) MulticastClear,
			(void*) MulticastSet_t,
			(void*) MulticastAdd_t,
			(void*) MulticastRemove_t,
			(void*) MulticastGetSnapshot,
		},
	};

	EFunctionPtrType PtrTypeKind = pMulticastType->GetTargetType ()->GetPtrTypeKind ();
	ASSERT (PtrTypeKind < EFunctionPtrType__Count);

	for (size_t i = 0; i < 5; i++)
	{
		pModule->SetFunctionPointer (
			pLlvmExecutionEngine, 
			pMulticastType->GetMethod ((EMulticastMethod) i), 
			MethodTable [PtrTypeKind] [i]
			);
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
