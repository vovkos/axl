// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_OBJ_ROOT_H

#include "axl_obj_Type.h"

namespace axl {
namespace obj {
	
//.............................................................................

// root interface

struct IRoot
{
	// {430F1A01-517D-4AFA-AB46-B28E30479E59}	
	AXL_OBJ_INTERFACE (
		IRoot,
		0x430f1a01, 0x517d, 0x4afa, 0xab, 0x46, 0xb2, 0x8e, 0x30, 0x47, 0x9e, 0x59
		)

	virtual
	void*
	GetObject (IType** ppType) = 0;

	void* 
	GetObject ()
	{
		return GetObject (NULL);
	}	

	IType* 
	GetType ()
	{
		IType* pType;
		GetObject (&pType);
		return pType;
	}
	
	void*
	GetInterface (const rtl::TGuid& Guid)
	{
		IType* pType;
		void* p = GetObject (&pType);
		size_t Offset = pType->GetInterfaceOffset (Guid);
		return Offset != -1 ? (uchar_t*) p + Offset : NULL;
	}
};

//.............................................................................

} // namespace obj
} // namespace axl
