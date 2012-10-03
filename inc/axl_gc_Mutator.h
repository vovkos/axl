// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2011. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_rtl_List.h"
#include "axl_obj_Root.h"

namespace axl {
namespace gc {

//.............................................................................

enum 
{
	axl_gc_ETypeFlag_Trace    = 0x80000000,
	axl_gc_ETypeFlag_Destruct = 0x40000000,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef struct _axl_gc_TType axl_gc_TType;

struct _axl_gc_TType
{
	size_t m_Size;
	ulong_t m_Type;
	void* m_pTypeDesc;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef struct _axl_gc_TValue axl_gc_TValue;

struct _axl_gc_TValue
{
	void* m_p;
	axl_gc_TType m_Type;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef struct _axl_gc_TValueEntry axl_gc_TValueEntry;

struct _axl_gc_TValueEntry: rtl::TListLink
{
	axl_gc_TValue m_Value;
};

//.............................................................................

class CCollector;

[uuid ("13ebd5b0-a9b2-498f-8a18-4e03628e7603")]
struct IMutator: obj::IRoot
{
	virtual 
	void 
	GetRootSet (CCollector* pCollector) = 0;

	virtual 
	void 
	TraceValue (
		CCollector* pCollector, 
		axl_gc_TValue* pValue
		) = 0;

	virtual 
	void 
	DestructValue (axl_gc_TValue* pValue) = 0;
};

//.............................................................................

} // namespace gc {
} // namespace axl {

