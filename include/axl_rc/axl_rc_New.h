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

#define _AXL_RC_NEW_H

#include "axl_rc_Ptr.h"
#include "axl_mem_New.h"

namespace axl {
namespace rc {

//..............................................................................

template <typename T>
T*
primeRefCount(
	T* p,
	FreeFunc* freeFunc = &mem::deallocate
) {
	p->prime(freeFunc);
	return p;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_RC_NEW(T) \
	(axl::rc::primeRefCount(new (axl::mem::allocate(sizeof(T))) T))

#define AXL_RC_NEW_EXTRA(T, extra) \
	(axl::rc::primeRefCount(new (axl::mem::allocate(sizeof(T) + extra)) T))

#define AXL_RC_NEW_INPLACE(T, p, freeFunc) \
	(axl::rc::primeRefCount(new (p) T, freeFunc))

#define AXL_RC_NEW_ARGS(T, args) \
	(axl::rc::primeRefCount(new (axl::mem::allocate(sizeof(T))) T args))

#define AXL_RC_NEW_ARGS_EXTRA(T, args, extra) \
	(axl::rc::primeRefCount(new (axl::mem::allocate(sizeof(T) + extra)) T args))

#define AXL_RC_NEW_ARGS_INPLACE(T, args, p, freeFunc) \
	(axl::rc::primeRefCount(new (p) T args, freeFunc))

//..............................................................................

} // namespace rc
} // namespace axl
