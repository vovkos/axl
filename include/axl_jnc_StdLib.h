// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Runtime.h"
#include "axl_jnc_Api.h"
#include "axl_jnc_MulticastLib.h"
#include "axl_g_Time.h"

namespace axl {
namespace jnc {

//.............................................................................

class CStdLib
{
public:
	AXL_JNC_API_BEGIN_LIB ()
		AXL_JNC_API_STD_FUNCTION (EStdFunc_RuntimeError, RuntimeError)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_DynamicCastClassPtr, DynamicCastClassPtr)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_StrengthenClassPtr, StrengthenClassPtr)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_GcAllocate, GcAllocate)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_GcTryAllocate, GcTryAllocate)
		AXL_JNC_API_STD_FUNCTION_FORCED (EStdFunc_GcEnter, GcEnter)
		AXL_JNC_API_STD_FUNCTION_FORCED (EStdFunc_GcLeave, GcLeave)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_GcPulse, GcPulse)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_RunGc, RunGc)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_GetCurrentThreadId, GetCurrentThreadId)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_CreateThread, CreateThread)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_Sleep, Sleep)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_GetTimestamp, GetTimestamp)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_GetLastError, GetLastError)
		AXL_JNC_API_CONST_PROPERTY ("jnc.Error.m_description", GetErrorDescription)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_StrLen, StrLen)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_Rand, Rand)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_GetTls, GetTls)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_AppendFmtLiteral_a, AppendFmtLiteral_a)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_AppendFmtLiteral_p, AppendFmtLiteral_p)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_AppendFmtLiteral_i32, AppendFmtLiteral_i32)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_AppendFmtLiteral_ui32, AppendFmtLiteral_ui32)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_AppendFmtLiteral_i64, AppendFmtLiteral_i64)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_AppendFmtLiteral_ui64, AppendFmtLiteral_ui64)
		AXL_JNC_API_STD_FUNCTION (EStdFunc_AppendFmtLiteral_f, AppendFmtLiteral_f)
		AXL_JNC_API_LIB (CMulticastLib)
	AXL_JNC_API_END_LIB ()

public:
	static
	void
	RuntimeError (
		int Error,
		void* pCodeAddr,
		void* pDataAddr
		)
	{
		CRuntime::RuntimeError (Error, pCodeAddr, pDataAddr);
	}

	static
	TIfaceHdr*
	DynamicCastClassPtr (
		TIfaceHdr* p,
		CClassType* pType
		);

	static
	TIfaceHdr*
	StrengthenClassPtr (TIfaceHdr* p);

	static
	void*
	GcAllocate (
		CType* pType,
		size_t ElementCount
		);

	static
	void*
	GcTryAllocate (
		CType* pType,
		size_t ElementCount
		);

	static
	void
	GcEnter ();

	static
	void
	GcLeave ();

	static
	void
	GcPulse ();

	static
	void
	RunGc ();

	static
	intptr_t
	GetCurrentThreadId ();

	static
	bool
	CreateThread (TFunctionPtr Ptr);

	static
	void
	Sleep (uint32_t MsCount)
	{
		g::Sleep (MsCount);
	}

	static
	uint64_t
	GetTimestamp ()
	{
		return g::GetTimestamp ();
	}

	static
	TDataPtr
	GetLastError ();

	static
	TDataPtr
	GetErrorDescription (TDataPtr Error);

	static
	size_t
	StrLen (TDataPtr Ptr);

	static
	int
	Rand ()
	{
		return rand ();
	}

	static
	void*
	GetTls ();

	static
	size_t
	AppendFmtLiteral_a (
		TFmtLiteral* pFmtLiteral,
		const char* p,
		size_t Length
		);

	static
	size_t
	AppendFmtLiteral_p (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		TDataPtr Ptr
		);

	static
	size_t
	AppendFmtLiteral_i32 (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		int32_t x
		)
	{
		return AppendFmtLiteralImpl (pFmtLiteral, pFmtSpecifier, 'd', x);
	}

	static
	size_t
	AppendFmtLiteral_ui32 (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		uint32_t x
		)
	{
		return AppendFmtLiteralImpl (pFmtLiteral, pFmtSpecifier, 'u', x);
	}

	static
	size_t
	AppendFmtLiteral_i64 (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		int64_t x
		)
	{
		return AppendFmtLiteralImpl (pFmtLiteral, pFmtSpecifier, 'd', x);
	}

	static
	size_t
	AppendFmtLiteral_ui64 (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		uint64_t x
		)
	{
		return AppendFmtLiteralImpl (pFmtLiteral, pFmtSpecifier, 'u', x);
	}

	static
	size_t
	AppendFmtLiteral_f (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		double x
		)
	{
		return AppendFmtLiteralImpl (pFmtLiteral, pFmtSpecifier, 'f', x);
	}

protected:
#if (_AXL_ENV == AXL_ENV_WIN)
	static
	DWORD
	WINAPI
	ThreadProc (PVOID pContext);
#elif (_AXL_ENV == AXL_ENV_POSIX)
	static
	void*
	ThreadProc (void* pContext);
#endif

	static
	void
	PrepareFormatString (
		rtl::CString* pFormatString,
		const char* pFmtSpecifier,
		char DefaultType
		);

	static
	size_t
	AppendFmtLiteralImpl (
		TFmtLiteral* pFmtLiteral,
		const char* pFmtSpecifier,
		char DefaultType,
		...
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
