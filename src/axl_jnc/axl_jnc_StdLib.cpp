#include "pch.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Runtime.h"
#include "axl_jnc_Multicast.h"
#include "axl_g_Time.h"

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
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_GcSafePoint, (void*) GcSafePoint);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_GetTls, (void*) GetTls);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_AppendFmtLiteral_a, (void*) AppendFmtLiteral_a);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_AppendFmtLiteral_p, (void*) AppendFmtLiteral_p);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_AppendFmtLiteral_i32, (void*) AppendFmtLiteral_i32);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_AppendFmtLiteral_ui32, (void*) AppendFmtLiteral_ui32);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_AppendFmtLiteral_i64, (void*) AppendFmtLiteral_i64);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_AppendFmtLiteral_ui64, (void*) AppendFmtLiteral_ui64);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, EStdFunc_AppendFmtLiteral_f, (void*) AppendFmtLiteral_f);

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

	pModule->SetFunctionPointer (pLlvmExecutionEngine, "GetCurrentThreadId", (void*) GetCurrentThreadId);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, "CreateThread", (void*) CreateThread);
	pModule->SetFunctionPointer (pLlvmExecutionEngine, "Sleep", (void*) Sleep);
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

TInterface* 
CStdLib::DynamicCastClassPtr (
	TInterface* p,
	CClassType* pType
	)
{
	if (!p)
		return NULL;

	if (p->m_pObject->m_pType->Cmp (pType) == 0)
		return p;
	
	CBaseTypeCoord Coord;
	bool Result = p->m_pObject->m_pType->FindBaseTypeTraverse (pType, &Coord);
	if (!Result)
		return NULL;
	
	TInterface* p2 = (TInterface*) ((uchar_t*) (p->m_pObject + 1) + Coord.m_Offset);
	ASSERT (p2->m_pObject == p->m_pObject);
	return p2;
}

TInterface* 
CStdLib::StrengthenClassPtr (TInterface* p)
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
CStdLib::MulticastClear (TMulticast* pMulticast)
{
	return ((CMulticast*) pMulticast)->Clear ();
}

handle_t
CStdLib::MulticastSet (
	TMulticast* pMulticast,
	TFunctionPtr Ptr
	)
{
	return ((CMulticast*) pMulticast)->SetHandler (Ptr);
}

handle_t
CStdLib::MulticastSet_t (
	TMulticast* pMulticast,
	void* pf
	)
{
	return ((CMulticast*) pMulticast)->SetHandler_t (pf);
}

handle_t
CStdLib::MulticastAdd (
	TMulticast* pMulticast,
	TFunctionPtr Ptr
	)
{
	return ((CMulticast*) pMulticast)->AddHandler (Ptr);
}

handle_t
CStdLib::MulticastAdd_t (
	TMulticast* pMulticast,
	void* pf
	)
{
	return ((CMulticast*) pMulticast)->AddHandler_t (pf);
}

TFunctionPtr
CStdLib::MulticastRemove (
	TMulticast* pMulticast,
	handle_t Handle
	)
{
	return ((CMulticast*) pMulticast)->RemoveHandler (Handle);
}

void*
CStdLib::MulticastRemove_t (
	TMulticast* pMulticast,
	handle_t Handle
	)
{
	return ((CMulticast*) pMulticast)->RemoveHandler_t (Handle);
}

TFunctionPtr
CStdLib::MulticastGetSnapshot (TMulticast* pMulticast)
{
	return ((CMulticast*) pMulticast)->GetSnapshot ();
}

void*
CStdLib::HeapAlloc (size_t Size)
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	return pRuntime->GcAllocate (Size);
}

void*
CStdLib::UHeapAlloc (size_t Size)
{
	void* p = malloc (Size);
	memset (p, 0, Size);
	return p;
}

void
CStdLib::UHeapFree (void* p)
{
	free (p);
}

void
CStdLib::UHeapFreeClassPtr (TInterface* p)
{
	free (p->m_pObject);
}

void
CStdLib::GcAddObject (TObject* p)
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	pRuntime->GcAddObject (p);
}

void
CStdLib::GcSafePoint ()
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	pRuntime->GcSafePoint ();
}

void
CStdLib::RunGc ()
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	pRuntime->RunGc ();
}

#if (_AXL_ENV == AXL_ENV_WIN)

intptr_t
CStdLib::GetCurrentThreadId ()
{
	return ::GetCurrentThreadId ();
}

struct TThreadContext
{
	TFunctionPtr m_Ptr;
	CRuntime* m_pRuntime;
};

DWORD 
WINAPI
CStdLib::ThreadProc (PVOID pRawContext)
{
	TThreadContext* pContext = (TThreadContext*) pRawContext;
	TFunctionPtr Ptr = pContext->m_Ptr;
	CRuntime* pRuntime = pContext->m_pRuntime;
	AXL_MEM_DELETE (pContext);

	CScopeThreadRuntime ScopeRuntime (pRuntime);
	GetTlsMgr ()->GetTlsData (pRuntime); // register thread right away

	((void (__cdecl*) (TInterface*)) Ptr.m_pf) (Ptr.m_pClosure);
	return 0;
}

bool
CStdLib::CreateThread (TFunctionPtr Ptr)
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	TThreadContext* pContext = AXL_MEM_NEW (TThreadContext);
	pContext->m_Ptr = Ptr;
	pContext->m_pRuntime = pRuntime;

	DWORD ThreadId;
	HANDLE h = ::CreateThread (NULL, 0, CStdLib::ThreadProc, pContext, 0, &ThreadId);
	return h != NULL;
}

void
CStdLib::Sleep (uint32_t MsCount)
{
	::Sleep (MsCount);
}

#elif (_AXL_ENV == AXL_ENV_POSIX)

intptr_t
CStdLib::GetCurrentThreadId ()
{
	return (intptr_t) pthread_self ();
}

struct TThreadContext
{
	TFunctionPtr m_Ptr;
	CRuntime* m_pRuntime;
};

void*
CStdLib::ThreadProc (void* pRawContext)
{
	TThreadContext* pContext = (TThreadContext*) pRawContext;
	TFunctionPtr Ptr = pContext->m_Ptr;
	CRuntime* pRuntime = pContext->m_pRuntime;
	AXL_MEM_DELETE (pContext);

	CScopeThreadRuntime ScopeRuntime (pRuntime);
	GetTlsMgr ()->GetTlsData (pRuntime); // register thread right away

	((void (*) (TInterface*)) Ptr.m_pf) (Ptr.m_pClosure);
	return NULL;
}

bool
CStdLib::CreateThread (TFunctionPtr Ptr)
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	TThreadContext* pContext = AXL_MEM_NEW (TThreadContext);
	pContext->m_Ptr = Ptr;
	pContext->m_pRuntime = pRuntime;

	pthread_t Thread;
	int Result = pthread_create (&Thread, NULL, CStdLib::ThreadProc, pContext);
	return Result == 0;
}

void
CStdLib::Sleep (uint32_t MsCount)
{
	timespec Timespec;
	g::GetTimespecFromTimeout (MsCount, &Timespec);	
	nanosleep (&Timespec, NULL);
}

#endif

void*
CStdLib::GetTls ()
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	return pRuntime->GetTls ();
}

size_t
CStdLib::AppendFmtLiteral_a (
	TFmtLiteral* pFmtLiteral,
	const char* p,
	size_t Length
	)
{
	size_t NewLength = pFmtLiteral->m_Length + Length;
	if (NewLength < 64) 
		NewLength = 64;

	if (pFmtLiteral->m_MaxLength < NewLength)
	{
		size_t NewMaxLength = rtl::GetMinPower2Ge (NewLength);
		char* pNew = (char*) AXL_MEM_ALLOC (NewMaxLength + 1);
		memcpy (pNew, pFmtLiteral->m_p, pFmtLiteral->m_Length);

		pFmtLiteral->m_p = pNew;
		pFmtLiteral->m_MaxLength = NewMaxLength;
	}

	memcpy (pFmtLiteral->m_p + pFmtLiteral->m_Length, p, Length);
	pFmtLiteral->m_Length += Length;
	pFmtLiteral->m_p [pFmtLiteral->m_Length] = 0;

	return pFmtLiteral->m_Length;
}

size_t
CStdLib::AppendFmtLiteral_p (
	TFmtLiteral* pFmtLiteral,
	TDataPtr Ptr
	)
{
	if (!Ptr.m_p)
		return AppendFmtLiteral_a (pFmtLiteral, "(null)", 6);

	char* p = (char*) Ptr.m_p;
	while (*p && p < Ptr.m_pRangeEnd)
		p++;

	size_t Length = p - (char*) Ptr.m_p;
	return AppendFmtLiteral_a (pFmtLiteral, (char*) Ptr.m_p, Length);
}

size_t
CStdLib::AppendFmtLiteral_i32 (
	TFmtLiteral* pFmtLiteral,
	int32_t x
	)
{
	char Buffer [16];
	_ltoa (x, Buffer, 10);
	return AppendFmtLiteral_a (pFmtLiteral, Buffer, strlen (Buffer));
}

size_t
CStdLib::AppendFmtLiteral_ui32 (
	TFmtLiteral* pFmtLiteral,
	uint32_t x
	)
{
	char Buffer [16];
	_ultoa (x, Buffer, 10);
	return AppendFmtLiteral_a (pFmtLiteral, Buffer, strlen (Buffer));
}

size_t
CStdLib::AppendFmtLiteral_i64 (
	TFmtLiteral* pFmtLiteral,
	int64_t x
	)
{
	char Buffer [16];
	_i64toa (x, Buffer, 10);
	return AppendFmtLiteral_a (pFmtLiteral, Buffer, strlen (Buffer));
}

size_t
CStdLib::AppendFmtLiteral_ui64 (
	TFmtLiteral* pFmtLiteral,
	uint64_t x
	)
{
	char Buffer [16];
	_ui64toa (x, Buffer, 10);
	return AppendFmtLiteral_a (pFmtLiteral, Buffer, strlen (Buffer));
}

size_t
CStdLib::AppendFmtLiteral_f (
	TFmtLiteral* pFmtLiteral,
	double x
	)
{
	char Buffer [16];
	sprintf (Buffer, "%f", x);
	return AppendFmtLiteral_a (pFmtLiteral, Buffer, strlen (Buffer));
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
