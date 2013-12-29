// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"

namespace axl {
namespace jnc {

class CRuntime;

//.............................................................................

enum EObjectFlag
{
	EObjectFlag_Dead         = 0x0001,
	EObjectFlag_DynamicArray = 0x0002,	
	EObjectFlag_Static       = 0x0010,
	EObjectFlag_Stack        = 0x0020,
	EObjectFlag_UHeap        = 0x0040,
	EObjectFlag_Extern       = 0x0080,	
	EObjectFlag_GcMark       = 0x0100,
	EObjectFlag_GcWeakMark   = 0x0200,
	EObjectFlag_GcWeakMark_c = 0x0400,
	EObjectFlag_GcRootsAdded = 0x0800,
	EObjectFlag_GcMask       = 0x0f00,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TObject
{
	size_t m_ScopeLevel; // if scope level != 0 and the object is not of class-type, then the rest can be omitted
	TObject* m_pRoot;

	union
	{
		CType* m_pType;
		CClassType* m_pClassType;
	};

	uintptr_t m_Flags;
	
	void 
	GcMarkData (CRuntime* pRuntime);

	void 
	GcMarkObject (CRuntime* pRuntime);

	void 
	GcWeakMarkObject ()
	{
		m_Flags |= EObjectFlag_GcWeakMark;
	}

	void 
	GcWeakMarkClosureObject (CRuntime* pRuntime);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct TDynamicArray: TObject
{
	size_t m_Count;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
TObject*
GetStaticObject ()
{
	static TObject Object = { 0, &Object, NULL, EObjectFlag_GcMark | EObjectFlag_GcWeakMark };
	return &Object;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
