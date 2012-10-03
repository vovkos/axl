// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_Line.h"
#include "axl_log_ColorizeSink.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

#ifndef __cplusplus

typedef
bool_t 
(AXL_VFT* 
 axl_log_FFilterFilter)(
	void* pFilter,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t* pVolatileFlags
	);

typedef struct _axl_log_TFilterVTable axl_log_TFilterVTable;

struct _axl_log_TFilterVTable
{
	axl_ref_TObjectVTable;
	axl_log_FFilterFilter m_pfnFilter;
};

#endif

//.............................................................................

typedef struct _axl_log_TFilter axl_log_TFilter;

void
AXL_API
axl_log_TFilter_Construct(
	axl_log_TFilter* pFilter, 
	axl_ref_TBuffer* pStorage
	);

void
AXL_VFT
axl_log_TFilter_Destruct(axl_log_TFilter* pFilter);

AXL_INLINE
bool_t 
axl_log_TFilter_Filter(
	axl_log_TFilter* pFilter,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t* pVolatileFlags
	);

#ifdef __cplusplus
typedef struct AXL_NO_VTABLE _axl_log_TFilter: axl_ref_TObject 
{
	virtual bool_t AXL_VFT Filter(ulong_t Code, const void* p, size_t Size, ulong_t* pVolatileFlags)
		{ return false; }
#else
typedef struct _axl_log_TFilter { axl_ref_TObject;
#endif
} axl_log_TFilter;

//.............................................................................

typedef struct _axl_log_TFilterStack axl_log_TFilterStack;

void
AXL_API
axl_log_TFilterStack_Construct(
	axl_log_TFilterStack* pStack,
	axl_ref_TBuffer* pStorage
	);

void
AXL_VFT
axl_log_TFilterStack_Destruct(axl_log_TFilterStack* pStack);

void
AXL_API
axl_log_TFilterStack_Clear(axl_log_TFilterStack* pStack);

bool_t
AXL_API
axl_log_TFilterStack_ClearButFirst(axl_log_TFilterStack* pStack);

bool_t
AXL_API
axl_log_TFilterStack_AddFilter(
	axl_log_TFilterStack* pStack,
	axl_log_TFilter* pFilter
	);

bool_t
AXL_API
axl_log_TFilterStack_RemoveFilter(
	axl_log_TFilterStack* pStack,
	axl_log_TFilter* pFilter
	);

bool_t 
AXL_VFT
axl_log_TFilterStack_Filter(
	axl_log_TFilterStack* pStack,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t* pVolatileFlags
	);

#ifdef __cplusplus
typedef struct _axl_log_TFilterStack: axl_log_TFilter {
#else
typedef struct _axl_log_TFilterStack { axl_log_TFilter;
#endif
	axl_rtl_TList m_Stack;

#ifdef __cplusplus
	void Clear()
		{ axl_log_TFilterStack_Clear(this); }

	bool ClearButFirst()
		{ return axl_log_TFilterStack_ClearButFirst(this) != 0; }

	bool AddFilter(axl_log_TFilter* pFilter)
		{ return axl_log_TFilterStack_AddFilter(this, pFilter) != 0; }

	bool RemoveFilter(axl_log_TFilter* pFilter)
		{ return axl_log_TFilterStack_RemoveFilter(this, pFilter) != 0; }
#endif
} axl_log_TFilterStack;

//.............................................................................

// inlines

AXL_INLINE
bool_t
axl_log_TFilter_Filter(
	axl_log_TFilter* pFilter,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t* pVolatileFlags
	)
{
#ifdef __cplusplus
	return pFilter->Filter(Code, p, Size, pVolatileFlags);
#else
	return ((axl_log_TFilterVTable*) pFilter->m_pVTable)->m_pfnFilter(pFilter, Code, p, Size, pVolatileFlags);
#endif
}

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TFilter       TFilter;
typedef axl_log_TFilterStack  TFilterStack;

class AXL_NO_VTABLE CFilter: public TFilter
{
public:
	CFilter(axl::ref::TBuffer* pStorage)
		{ axl_log_TFilter_Construct(this, pStorage); }

	~CFilter()
		{ axl_log_TFilter_Destruct(this); }
};

class AXL_NO_VTABLE CFilterStack: public TFilterStack
{
public:
	CFilterStack(axl::ref::TBuffer* pStorage)
		{ axl_log_TFilterStack_Construct(this, pStorage); }

	~CFilterStack()
		{ axl_log_TFilterStack_Destruct(this); }
};

} // namespace log 
} // namespace axl

#endif // __cplusplus

//.............................................................................
