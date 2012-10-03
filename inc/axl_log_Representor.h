// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2010. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_log_RepresentSink.h"

AXL_EXTERN_C_BEGIN

//.............................................................................

#ifndef __cplusplus

typedef
bool_t 
(AXL_VFT* 
 axl_log_FRepresentorRepresent)(
	void* pRepresentor,
	axl_log_TRepresentSink* pSink,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t VolatileFlags
	);

typedef struct _axl_log_TRepresentorVTable axl_log_TRepresentorVTable;

struct _axl_log_TRepresentorVTable
{
	axl_ref_TObjectVTable;
	axl_log_FRepresentorRepresent m_pfnRepresent;
};

#endif

//.............................................................................

typedef struct _axl_log_TRepresentor      axl_log_TRepresentor;

void
AXL_API
axl_log_TRepresentor_Construct(
	axl_log_TRepresentor* pRepresentor, 
	axl_ref_TBuffer* pStorage
	);

void
AXL_VFT
axl_log_TRepresentor_Destruct(axl_log_TRepresentor* pRepresentor);

AXL_INLINE
bool_t 
axl_log_TRepresentor_Represent(
	axl_log_TRepresentor* pRepresentor,
	axl_log_TRepresentSink* pSink,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t VolatileFlags
	);

bool_t 
AXL_VFT
axl_log_TRepresentor_Represent_b(
	axl_log_TRepresentor* pRepresentor,
	axl_log_TRepresentSink* pSink,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t VolatileFlags
	);

#ifdef __cplusplus
struct AXL_NO_VTABLE _axl_log_TRepresentor: axl_ref_TObject {

	virtual bool_t AXL_VFT Represent(axl_log_TRepresentSink* pSink, ulong_t Code, const void* p, size_t Size, ulong_t VolatileFlags)
		{ return false; } 

#else
struct _axl_log_TRepresentor { axl_ref_TObject;
#endif
};

//.............................................................................

typedef struct _axl_log_TRepresentorStack axl_log_TRepresentorStack;

void
AXL_API
axl_log_TRepresentorStack_Construct(
	axl_log_TRepresentorStack* pStack,
	axl_ref_TBuffer* pStorage
	);

void
AXL_VFT
axl_log_TRepresentorStack_Destruct(axl_log_TRepresentorStack* pStack);

void
AXL_API
axl_log_TRepresentorStack_Clear(axl_log_TRepresentorStack* pStack);

bool_t
AXL_API
axl_log_TRepresentorStack_ClearButFirst(axl_log_TRepresentorStack* pStack);

bool_t
AXL_API
axl_log_TRepresentorStack_AddRepresentor(
	axl_log_TRepresentorStack* pStack,
	axl_log_TRepresentor* pRepresentor
	);

bool_t
AXL_API
axl_log_TRepresentorStack_RemoveRepresentor(
	axl_log_TRepresentorStack* pStack,
	axl_log_TRepresentor* pRepresentor
	);

bool_t 
AXL_VFT
axl_log_TRepresentorStack_Represent(
	axl_log_TRepresentorStack* pStack,
	axl_log_TRepresentSink* pSink,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t VolatileFlags
	);

#ifdef __cplusplus
struct _axl_log_TRepresentorStack: axl_log_TRepresentor {
#else
struct _axl_log_TRepresentorStack { axl_log_TRepresentor;
#endif
	axl_rtl_TListEx m_Stack;

#ifdef __cplusplus
	void Clear()
		{ axl_log_TRepresentorStack_Clear(this); }

	bool ClearButFirst()
		{ return axl_log_TRepresentorStack_ClearButFirst(this) != 0; }

	bool AddRepresentor(axl_log_TRepresentor* pRepresentor)
		{ return axl_log_TRepresentorStack_AddRepresentor(this, pRepresentor) != 0; }

	bool RemoveRepresentor(axl_log_TRepresentor* pRepresentor)
		{ return axl_log_TRepresentorStack_RemoveRepresentor(this, pRepresentor) != 0; }
#endif
};

//.............................................................................

// inlines

AXL_INLINE
bool_t 
axl_log_TRepresentor_Represent(
	axl_log_TRepresentor* pRepresentor,
	axl_log_TRepresentSink* pSink,
	ulong_t Code,
	const void* p,
	size_t Size,
	ulong_t VolatileFlags
	)
{
#ifdef __cplusplus
	return pRepresentor->Represent(pSink, Code, p, Size, VolatileFlags);
#else
	return ((axl_log_TRepresentorVTable*) pRepresentor->m_pVTable)->m_pfnRepresent(pRepresentor, pSink, Code, p, Size, VolatileFlags);
#endif
}

//.............................................................................

AXL_EXTERN_C_END

#ifdef __cplusplus

namespace axl {
namespace log {

typedef axl_log_TRepresentor      TRepresentor;
typedef axl_log_TRepresentorStack TRepresentorStack;

class AXL_NO_VTABLE CRepresentor: public TRepresentor
{
public:
	CRepresentor(axl::ref::TBuffer* pStorage)
		{ axl_log_TRepresentor_Construct(this, pStorage); }

	~CRepresentor()
		{ axl_log_TRepresentor_Destruct(this); }
};

class AXL_NO_VTABLE CRepresentorStack: public TRepresentorStack
{
public:
	CRepresentorStack(axl::ref::TBuffer* pStorage)
		{ axl_log_TRepresentorStack_Construct(this, pStorage); }

	~CRepresentorStack()
		{ axl_log_TRepresentorStack_Destruct(this); }
};

} // namespace log 
} // namespace axl

#endif // __cplusplus

//.............................................................................
