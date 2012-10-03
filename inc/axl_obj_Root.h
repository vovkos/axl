// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_OBJ_ROOT_H

#include "axl_obj_Class.h"

namespace axl {
namespace obj {

struct IClass;

//.............................................................................

// no GUID is needed 

struct IRoot
{
	AXL_OBJ_EMPTY_INTERFACE_MAP () // if class is inhereted from a single interface, no macros are needed

	virtual
	void*
	GetObject (IClass** ppClass) = 0;

	void* 
	GetObject ()
	{
		return GetObject (NULL);
	}

	IClass* 
	GetClass ()
	{
		IClass* pClass;
		GetObject (&pClass);
		return pClass;
	}
	
	void*
	GetInterface (const GUID& Guid)
	{
		IClass* pClass;
		void* p = GetObject (&pClass);
		size_t Offset = pClass->GetInterfaceOffset (Guid);
		return Offset != -1 ? (uchar_t*) p + Offset : NULL;
	}
};

//.............................................................................

} // namespace obj
} // namespace axl
