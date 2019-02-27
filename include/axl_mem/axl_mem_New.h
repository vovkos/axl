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

#define _AXL_MEM_NEW_H

#include "axl_mem_Allocator.h"

namespace axl {
namespace mem {

//..............................................................................

template <typename T>
class StdDelete
{
public:
	void
	operator () (T* p) const
	{
		p->~T();
		StdAllocator::free(p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class CppDelete
{
public:
	void
	operator () (T* p) const
	{
		delete p;
	}
};

//..............................................................................

template <typename T>
void
stdDelete(T* p)
{
	p->~T();
	StdAllocator::free(p);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifdef _AXL_DEBUG

#define AXL_MEM_NEW(T) \
	(new(AXL_MEM_ALLOCATE_EX(sizeof(T), #T)) T)

#define AXL_MEM_NEW_EXTRA(T, extra) \
	(new(AXL_MEM_ALLOCATE_EX(sizeof(T) + extra, #T)) T)

#define AXL_MEM_NEW_ARGS(T, args) \
	(new(AXL_MEM_ALLOCATE_EX(sizeof(T), #T)) T args)

#define AXL_MEM_NEW_ARGS_EXTRA(T, args, extra) \
	(new(AXL_MEM_ALLOCATE_EX(sizeof(T) + extra, #T)) T args)

#define AXL_MEM_ZERO_NEW(T) \
	(new(AXL_MEM_ZERO_ALLOCATE_EX(sizeof(T), #T)) T)

#define AXL_MEM_ZERO_NEW_EXTRA(T, extra) \
	(new(AXL_MEM_ZERO_ALLOCATE_EX(sizeof(T) + extra, #T)) T)

#define AXL_MEM_ZERO_NEW_ARGS(T, args) \
	(new(AXL_MEM_ZERO_ALLOCATE_EX(sizeof(T), #T)) T args)

#define AXL_MEM_ZERO_NEW_ARGS_EXTRA(T, args, extra) \
	(new(AXL_MEM_ZERO_ALLOCATE_EX(sizeof(T) + extra, #T)) T args)

#else

#define AXL_MEM_NEW(T) \
	(new(AXL_MEM_ALLOCATE(sizeof(T))) T)

#define AXL_MEM_NEW_EXTRA(T, extra) \
	(new(AXL_MEM_ALLOCATE(sizeof(T) + extra)) T)

#define AXL_MEM_NEW_ARGS(T, args) \
	(new(AXL_MEM_ALLOCATE(sizeof(T))) T args)

#define AXL_MEM_NEW_ARGS_EXTRA(T, args, extra) \
	(new(AXL_MEM_ALLOCATE(sizeof(T) + extra)) T args)

#define AXL_MEM_ZERO_NEW(T) \
	(new(AXL_MEM_ZERO_ALLOCATE(sizeof(T))) T)

#define AXL_MEM_ZERO_NEW_EXTRA(T, extra) \
	(new(AXL_MEM_ZERO_ALLOCATE(sizeof(T) + extra)) T)

#define AXL_MEM_ZERO_NEW_ARGS(T, args) \
	(new(AXL_MEM_ZERO_ALLOCATE(sizeof(T))) T args)

#define AXL_MEM_ZERO_NEW_ARGS_EXTRA(T, args, extra) \
	(new(AXL_MEM_ZERO_ALLOCATE(sizeof(T) + extra)) T args)

#endif

#define AXL_MEM_DELETE(p) \
	(axl::mem::stdDelete(p))

//..............................................................................

} // namespace mem
} // namespace axl
