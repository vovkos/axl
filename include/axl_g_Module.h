// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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

struct IFinalizer: obj::IRoot
{
	// {DD222152-03D6-4D4E-8291-F48D36F4035E}
	AXL_OBJ_INTERFACE (
		IFinalizer,
		0xdd222152, 0x03d6, 0x4d4e, 0x82, 0x91, 0xf4, 0x8d, 0x36, 0xf4, 0x03, 0x5e
		)

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
	AddFinalizer (const ref::CPtrT <IFinalizer>& Finalizer);
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
