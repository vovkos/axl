// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_RTL_TYPE_H

#include "axl_g_Def.h"

namespace axl {
namespace rtl {
	
//.............................................................................

// compile-time type information

template <typename T>
class CTypeT
{
public:
	static
	size_t 
	GetSize ()
	{ 
		return sizeof (T);
	}

	static
	const char*
	GetName ()
	{ 
		return typeid (T).name ();
	}

	static
	void
	Construct (void* p)
	{
		new (p) T;
	}

	static
	void
	Destruct (void* p)
	{
		((T*) p)->~T ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specialization for void

template <>
class CTypeT <void>
{
public:
	static
	size_t 
	GetSize ()
	{ 
		return 0;
	}

	static
	const char*
	GetName ()
	{ 
		return typeid (void).name ();
	}

	static
	void
	Construct (void* p)
	{
	}

	static
	void
	Destruct (void* p)
	{
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
