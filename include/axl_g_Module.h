// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_G_MODULE_H

#include "axl_rtl_List.h"
#include "axl_mt_Lock.h"
#include "axl_mt_CallOnce.h"
#include "axl_mem_Tracker.h"
#include "axl_ref_Ptr.h"

namespace axl {
namespace g {

//.............................................................................
	
struct TSystemInfo
{
	size_t m_ProcessorCount;
	size_t m_PageSize;
	size_t m_MappingAlignFactor;
};

//.............................................................................

class CFinalizer
{
public:
	virtual 
	void 
	Finalize () = 0;
};

//.............................................................................

class CModule
{
protected:
	class CFinalizerEntry: public rtl::TListLink
	{
	public:
		ref::CPtrT <CFinalizer> m_Finalizer;
	};

protected:
#ifdef _DEBUG
	const char* m_pTag;
#endif

#if (_AXL_ENV == AXL_ENV_WIN)
	HMODULE m_hModule;
#endif
	
	TSystemInfo m_SystemInfo;

	mem::CTracker m_MemTracker;

	// finalizers
	
	mt::CLock m_FinalizerListLock;
	rtl::CStdListT <CFinalizerEntry> m_FinalizerList;

public:
	CModule ();

	~CModule ();

#if (_AXL_ENV == AXL_ENV_WIN)
	HMODULE 
	GetModuleHandle ()
	{
		return m_hModule;
	}
#endif

	TSystemInfo* 
	GetSystemInfo ()
	{
		return &m_SystemInfo;
	}

	mem::CTracker*
	GetMemTracker ()
	{
		return &m_MemTracker;
	}

	bool 
	AddFinalizer (const ref::CPtrT <CFinalizer>& Finalizer);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY 
CModule g_Module;

inline
CModule*
GetModule ()
{
	return &g_Module;
}

//.............................................................................

} // namespace g
} // namespace axl
