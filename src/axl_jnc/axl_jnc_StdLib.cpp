#include "pch.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_Module.h"
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
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_HeapAlloc, (void*) HeapAlloc);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_UHeapAlloc, (void*) UHeapAlloc);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_UHeapFree, (void*) UHeapFree);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_UHeapFreeClassPtr, (void*) UHeapFreeClassPtr);

	// implementation for thin and unsafe is the same

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

	pModule->SetFunctionPointer (pLlvmExecutionEngine, "GetCurrentThreadId", (void*) GetCurrentThreadId);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, "CreateThread", (void*) CreateThread);
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
	case jnc::ERuntimeError_LoadOutOfRange:
		pErrorString = "READ_OOR";
		break;

	case jnc::ERuntimeError_StoreOutOfRange:
		pErrorString = "WRITE_OOR";
		break;

	case jnc::ERuntimeError_ScopeMismatch:
		pErrorString = "SCOPE_MISMATCH";
		break;

	case jnc::ERuntimeError_NullInterface:
		pErrorString = "NULL_INTERFACE";
		break;

	case jnc::ERuntimeError_NullFunction:
		pErrorString = "NULL_FUNCTION";
		break;

	case jnc::ERuntimeError_NullProperty:
		pErrorString = "NULL_PROPERTY";
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

handle_t
CStdLib::MulticastSet (
	jnc::TMulticast* pMulticast,
	jnc::TFunctionPtr Ptr
	)
{
	return ((CMulticast*) pMulticast)->SetHandler (Ptr);
}

handle_t
CStdLib::MulticastSet_w (
	jnc::TMulticast* pMulticast,
	jnc::TFunctionPtr_w Ptr
	)
{
	return ((CMulticast*) pMulticast)->SetHandler_w (Ptr);
}

handle_t
CStdLib::MulticastSet_u (
	jnc::TMulticast* pMulticast,
	void* pf
	)
{
	return ((CMulticast*) pMulticast)->SetHandler_u (pf);
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
CStdLib::MulticastAdd_w (
	jnc::TMulticast* pMulticast,
	jnc::TFunctionPtr_w Ptr
	)
{
	return ((CMulticast*) pMulticast)->AddHandler_w (Ptr);
}

handle_t
CStdLib::MulticastAdd_u (
	jnc::TMulticast* pMulticast,
	void* pf
	)
{
	return ((CMulticast*) pMulticast)->AddHandler_u (pf);
}

jnc::TFunctionPtr
CStdLib::MulticastRemove (
	jnc::TMulticast* pMulticast,
	handle_t Handle
	)
{
	return ((CMulticast*) pMulticast)->RemoveHandler (Handle);
}

jnc::TFunctionPtr_w
CStdLib::MulticastRemove_w (
	jnc::TMulticast* pMulticast,
	handle_t Handle
	)
{
	return ((CMulticast*) pMulticast)->RemoveHandler_w (Handle);
}

void*
CStdLib::MulticastRemove_u (
	jnc::TMulticast* pMulticast,
	handle_t Handle
	)
{
	return ((CMulticast*) pMulticast)->RemoveHandler_u (Handle);
}

jnc::TMcSnapshot
CStdLib::MulticastSnapshot (jnc::TMulticast* pMulticast)
{
	return ((CMulticast*) pMulticast)->Snapshot ();
}

jnc::TMcSnapshot
CStdLib::MulticastSnapshot_w (jnc::TMulticast* pMulticast)
{
	return ((CMulticast*) pMulticast)->Snapshot_w ();
}

jnc::TMcSnapshot
CStdLib::MulticastSnapshot_u (jnc::TMulticast* pMulticast)
{
	return ((CMulticast*) pMulticast)->Snapshot_u ();
}

void*
CStdLib::HeapAlloc (jnc::CType* pType)
{
	void* p = malloc (pType->GetSize ());
	memset (p, 0, pType->GetSize ());

	if (pType->GetTypeKind () == jnc::EType_Struct && ((jnc::CStructType*) pType)->IsClassStructType ())
	{
		jnc::CClassType* pClassType = (jnc::CClassType*) ((jnc::CStructType*) pType)->GetParentNamespace ();
		jnc::CFunction* pDestructor = pClassType->GetDestructor ();
		
		if (pDestructor)
		{
			// add to finalizer list
		}
	}

	return p;
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

void
CStdLib::PointerCheck (jnc::TDataPtr Ptr)
{
	printf (
		"PointerCheck (%p; range = %p:%p; scope = %d)\n", 
		Ptr.m_p,
		Ptr.m_pRangeBegin,
		Ptr.m_pRangeEnd,
		Ptr.m_ScopeLevel
		);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
