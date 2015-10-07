// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_NEW_H

#include "axl_ref_Ptr.h"
#include "axl_mem_New.h"
#include "axl_sl_Func.h"

namespace axl {
namespace ref {

//.............................................................................

inline
void*
postAllocateRefCount (
	void* p,
	FreeFunc* freeFunc = &mem::StdAllocator::free
	)
{
	if (!p)
		return NULL;

	RefCountAllocHdr* hdr = (RefCountAllocHdr*) p;
	hdr->m_freeFunc = freeFunc;
	return hdr + 1;
}

template <typename T>
T*
primeAllocatedRefCount (T* p)
{
	if (!p)
		return NULL;

	p->prime (NULL, RefCountFlag_Allocated);
	return p;
}

template <typename T>
T*
primeInPlaceRefCount (
	T* p,
	uint_t flags
	)
{
	p->prime (NULL, flags);
	return p;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_REF_ALLOC_SIZEOF(T) \
	(sizeof (axl::ref::RefCountAllocHdr) + sizeof (T))

#ifdef _AXL_DEBUG

#define AXL_REF_ALLOCATE(T) \
	axl::ref::postAllocateRefCount (AXL_MEM_ZERO_ALLOCATE_EX (AXL_REF_ALLOC_SIZEOF(T), axl::mem::getTypeName <T> ()))

#define AXL_REF_ALLOCATE_EXTRA(T, extra) \
	axl::ref::postAllocateRefCount (AXL_MEM_ZERO_ALLOCATE_EX (AXL_REF_ALLOC_SIZEOF(T) + extra, axl::mem::getTypeName <T> ()))

#else

#define AXL_REF_ALLOCATE(T) \
	axl::ref::postAllocateRefCount (AXL_MEM_ZERO_ALLOCATE (AXL_REF_ALLOC_SIZEOF(T)))

#define AXL_REF_ALLOCATE_EXTRA(T, extra) \
	axl::ref::postAllocateRefCount (AXL_MEM_ZERO_ALLOCATE (AXL_REF_ALLOC_SIZEOF(T) + extra))

#endif

#define AXL_REF_NEW(T) \
	(axl::ref::primeAllocatedRefCount (new (AXL_REF_ALLOCATE (T)) T))

#define AXL_REF_NEW_EXTRA(T, extra) \
	(axl::ref::primeAllocatedRefCount (new (AXL_REF_ALLOCATE_EXTRA (T, extra)) T))

#define AXL_REF_NEW_INPLACE(T, p, flags) \
	(axl::ref::primeInPlaceRefCount (new (p) T, flags))

#define AXL_REF_NEW_ARGS(T, args) \
	(axl::ref::primeAllocatedRefCount (new (AXL_REF_ALLOCATE (T)) T args))

#define AXL_REF_NEW_ARGS_EXTRA(T, args, extra) \
	(axl::ref::primeAllocatedRefCount (new (AXL_REF_ALLOCATE_EXTRA (T, extra)) T args))

#define AXL_REF_NEW_ARGS_INPLACE(T, args, p, flags) \
	(axl::ref::primeInPlaceRefCount (new (p) T args,  flags))

//.............................................................................

} // namespace ref
} // namespace axl
