// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_STD_MODULE_H

#include "axl_rtl_List.h"
#include "axl_mt_Lock.h"
#include "axl_mem_Tracker.h"
#include "axl_exe_CallOnce.h"
#include "axl_ref_Ptr.h"

#include "axl_win_SystemInfo.h"

namespace axl {
namespace g {

//.............................................................................

[uuid("d9a0ff12-8fd4-4d59-88cb-9a85ee2050d6")]
struct IFinalizer: obj::IRoot
{
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
		ref::CPtrT <IFinalizer> m_Finalizer;
	};

protected:
#ifdef _DEBUG
	const char* m_pTag;
#endif

	HMODULE m_hModule;
	win::TSystemInfo m_SystemInfo;

	mem::CTracker m_MemTracker;

	// finalizers
	
	mt::CLock m_FinalizerListLock;
	rtl::CStdListT <CFinalizerEntry> m_FinalizerList;

public:
	CModule ();

	~CModule ();

	HMODULE 
	GetModuleHandle ()
	{
		return m_hModule;
	}

	win::TSystemInfo* 
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
	AddFinalizer (IFinalizer* pFinalizer);		
};

//.............................................................................

#ifndef _AXL_CUSTOM_MODULE

AXL_SELECT_ANY 
CModule g_Module;

inline
CModule*
GetModule ()
{
	return &g_Module;
}

#else

CModule*
GetModule ();

#endif

//.............................................................................

} // namespace g
} // namespace axl