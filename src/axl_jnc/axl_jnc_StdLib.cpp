#include "pch.h"
#include "axl_jnc_StdLib.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Runtime.h"
#include "axl_g_Time.h"

namespace axl {
namespace jnc {

//.............................................................................

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
	return ClassTypeKind == EClassType_FunctionClosure || ClassTypeKind == EClassType_PropertyClosure ?
		((CClosureClassType*) p->m_pObject->m_pType)->Strengthen (p) :
		(p->m_pObject->m_Flags & EObjectFlag_Alive) ? p : NULL;
}

void*
CStdLib::HeapAlloc (size_t Size)
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	return pRuntime->GcAllocate (Size);
}

void*
CStdLib::HeapUAlloc (size_t Size)
{
	void* p = malloc (Size);
	if (!p)
	{
		OnRuntimeError (ERuntimeError_OutOfMemory, NULL, NULL);
		ASSERT (false);
	}

	memset (p, 0, Size);
	return p;
}

void
CStdLib::HeapUFree (void* p)
{
	free (p);
}

void
CStdLib::HeapUFreeClassPtr (TInterface* p)
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

void
CStdLib::RunGcWaitForDestructors ()
{
	CRuntime* pRuntime = GetCurrentThreadRuntime ();
	ASSERT (pRuntime);

	pRuntime->RunGcWaitForDestructors ();
}

size_t
CStdLib::StrLen (TDataPtr Ptr)
{
	char* p = (char*) Ptr.m_p;
	if (!p)
		return 0;

	char* p0 = p;
	char* pEnd = (char*) Ptr.m_pRangeEnd;
	while (*p && p < pEnd)
		p++;

	return p - p0;
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

void
CStdLib::PrepareFormatString (
	rtl::CString* pFormatString,
	const char* pFmtSpecifier,
	char DefaultType
	)
{
	if (!pFmtSpecifier)
	{
		char FormatBuffer [2] = { '%', DefaultType };
		pFormatString->Copy (FormatBuffer, 2);
		return;
	}

	pFormatString->Clear ();

	if (pFmtSpecifier [0] != '%')
		pFormatString->Copy ('%');

	pFormatString->Append (pFmtSpecifier);

	size_t Length = pFormatString->GetLength ();
	if (!isalpha (pFormatString->cc () [Length - 1]))
		pFormatString->Append (DefaultType);
}

size_t
CStdLib::AppendFmtLiteral_p (
	TFmtLiteral* pFmtLiteral,
	const char* pFmtSpecifier,
	TDataPtr Ptr
	)
{
	if (!Ptr.m_p)
		return AppendFmtLiteral_a (pFmtLiteral, "(null)", 6);

	char* p = (char*) Ptr.m_p;
	while (*p && p < Ptr.m_pRangeEnd)
		p++;

	if (!pFmtSpecifier || !*pFmtSpecifier)
	{
		size_t Length = p - (char*) Ptr.m_p;
		return AppendFmtLiteral_a (pFmtLiteral, (char*) Ptr.m_p, Length);
	}

	char Buffer1 [256];
	rtl::CString FormatString (ref::EBuf_Stack, Buffer1, sizeof (Buffer1));
	PrepareFormatString (&FormatString, pFmtSpecifier, 's');

	char Buffer2 [256];
	rtl::CString String (ref::EBuf_Stack, Buffer2, sizeof (Buffer2));

	if (p < Ptr.m_pRangeEnd) // null terminated
	{
		ASSERT (!*p);
		String.Format (FormatString, Ptr.m_p);
	}
	else
	{
		char Buffer3 [256];
		rtl::CString NullTermString (ref::EBuf_Stack, Buffer3, sizeof (Buffer3));
		String.Format (FormatString, NullTermString.cc ());
	}

	return AppendFmtLiteral_a (pFmtLiteral, String, String.GetLength ());
}

size_t
CStdLib::AppendFmtLiteralImpl (
	TFmtLiteral* pFmtLiteral,
	const char* pFmtSpecifier,
	char DefaultType,
	...
	)
{
	AXL_VA_DECL (va, DefaultType);

	char Buffer1 [256];
	rtl::CString FormatString (ref::EBuf_Stack, Buffer1, sizeof (Buffer1));
	PrepareFormatString (&FormatString, pFmtSpecifier,  DefaultType);

	char Buffer2 [256];
	rtl::CString String (ref::EBuf_Stack, Buffer2, sizeof (Buffer2));
	String.Format_va (FormatString, va);

	return AppendFmtLiteral_a (pFmtLiteral, String, String.GetLength ());
}

//.............................................................................

} // namespace jnc {
} // namespace axl {