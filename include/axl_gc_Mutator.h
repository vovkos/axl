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
	size_t m_size;
	ulong_t m_type;
	void* m_typeDesc;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef struct _axl_gc_TValue axl_gc_TValue;

struct _axl_gc_TValue
{
	void* m_p;
	axl_gc_TType m_type;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef struct _axl_gc_TValueEntry axl_gc_TValueEntry;

struct _axl_gc_TValueEntry: rtl::ListLink
{
	axl_gc_TValue m_value;
};

//.............................................................................

class Collector;

[uuid ("13ebd5b0-a9b2-498f-8a18-4e03628e7603")]
struct IMutator: obj::IRoot
{
	virtual 
	void 
	getRootSet (Collector* collector) = 0;

	virtual 
	void 
	traceValue (
		Collector* collector, 
		axl_gc_TValue* value
		) = 0;

	virtual 
	void 
	destructValue (axl_gc_TValue* value) = 0;
};

//.............................................................................

} // namespace gc {
} // namespace axl {

