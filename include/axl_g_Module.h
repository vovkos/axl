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
	size_t m_processorCount;
	size_t m_pageSize;
	size_t m_mappingAlignFactor;
};

//.............................................................................

class CFinalizer
{
public:
	virtual 
	void 
	finalize () = 0;
};

//.............................................................................

class CModule
{
protected:
	class CFinalizerEntry: public rtl::TListLink
	{
	public:
		ref::CPtrT <CFinalizer> m_finalizer;
	};

protected:
#ifdef _DEBUG
	const char* m_tag;
#endif

#if (_AXL_ENV == AXL_ENV_WIN)
	HMODULE m_hModule;
#endif
	
	TSystemInfo m_systemInfo;

	mem::CTracker m_memTracker;

	// finalizers
	
	mt::CLock m_finalizerListLock;
	rtl::CStdListT <CFinalizerEntry> m_finalizerList;

public:
	CModule ();

	~CModule ();

#if (_AXL_ENV == AXL_ENV_WIN)
	HMODULE 
	getModuleHandle ()
	{
		return m_hModule;
	}
#endif

	TSystemInfo* 
	getSystemInfo ()
	{
		return &m_systemInfo;
	}

	mem::CTracker*
	getMemTracker ()
	{
		return &m_memTracker;
	}

	bool 
	addFinalizer (const ref::CPtrT <CFinalizer>& finalizer);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY 
CModule g_module;

inline
CModule*
getModule ()
{
	return &g_module;
}

//.............................................................................

} // namespace g
} // namespace axl
