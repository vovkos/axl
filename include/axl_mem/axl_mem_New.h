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

namespace axl {
namespace mem {

//..............................................................................

enum class Extra: size_t {};

//..............................................................................

#ifdef _AXL_DEBUG

#define AXL_MEM_NEW(T) \
	(new (AXL_MEM_ALLOCATE_EX(sizeof(T), #T)) T)

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
	(delete p)

//..............................................................................

} // namespace mem
} // namespace axl

void*
operator new (size_t size);

void*
operator new[] (size_t size) {
	return operator new (size);
}

inline
void*
operator new (
    size_t size,
    const axl::mem::NewParams& params
) {
    return operator new (size + params.extra());
}

inline
void*
operator new[] (
    size_t size,
    axl::mem::Extra extra
) {
    return operator new (size, extra);
}

inline
void*
operator new (
    size_t size,
    axl::mem::Zero zero
) {
    void* p = operator new(size);
    if (!p)
        return NULL;

    memset(p, 0, size);
    return p;
}

inline
void*
operator new[] (
    size_t size,
    axl::mem::Zero zero
) {
    return operator new (size, zero);
}

inline
void*
operator new(
    size_t size,
    axl::mem::ZeroExtra extra
) {
    return operator new (size + extra(), axl::mem::Zero());
}

inline
void*
operator new[] (
    size_t size,
    axl::mem::ZeroExtra extra
) {
    return operator new (size, extra);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
operator delete (void* p);

inline
void
operator delete[] (void* p) {
	operator delete (p);
}

inline
void
operator
delete(
	void* p,
	axl::mem::Extra extra
) {
	operator delete (p);
}

void
operator
delete (
	void* p,
	axl::mem::Zero zero
) {
	operator delete (p);
}

void
operator
delete	(
	void* p,
	axl::mem::ZeroExtra extra
) {

}

//..............................................................................
