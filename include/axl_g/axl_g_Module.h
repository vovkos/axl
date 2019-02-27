//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_G_MODULE_H

#include "axl_sl_List.h"
#include "axl_sys_Lock.h"
#include "axl_sl_CallOnce.h"
#include "axl_mem_Tracker.h"
#include "axl_ref_Ptr.h"

namespace axl {
namespace g {

//..............................................................................

struct SystemInfo
{
	size_t m_processorCount;
	size_t m_pageSize;
	size_t m_mappingAlignFactor;
};

//..............................................................................

class Finalizer
{
public:
	virtual
	void
	finalize() = 0;
};

//..............................................................................

class Module
{
protected:
	struct FinalizerEntry: sl::ListLink
	{
		ref::Ptr<Finalizer> m_finalizer;
	};

protected:
#if (_AXL_OS_WIN)
	HMODULE m_hModule;
#endif

	SystemInfo m_systemInfo;

	mem::Tracker m_memTracker;

	// finalizers

	sys::Lock m_finalizerListLock;
	sl::List<FinalizerEntry> m_finalizerList;

#ifdef _AXL_DEBUG
	const char* m_tag;
#endif

public:
	Module();
	~Module();

#ifdef _AXL_DEBUG
	const char*
	getTag()
	{
		return m_tag;
	}

	void
	setTag(const char* tag)
	{
		m_tag = tag;
	}
#else
	const char*
	getTag()
	{
		return "<untagged-module>";
	}

	void
	setTag(const char* tag)
	{
	}
#endif

#if (_AXL_OS_WIN)
	HMODULE
	getModuleHandle()
	{
		return m_hModule;
	}
#endif

	const SystemInfo*
	getSystemInfo()
	{
		return &m_systemInfo;
	}

	mem::Tracker*
	getMemTracker()
	{
		return &m_memTracker;
	}

	bool
	addFinalizer(const ref::Ptr<Finalizer>& finalizer);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
Module*
getModule()
{
	// guaranteed to be thread safe since C++11
	// otherwise it's still OK -- unless going MT in static constructors

	static Module module;
	return &module;
}

//..............................................................................

} // namespace g
} // namespace axl
