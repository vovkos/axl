// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_TYPE_H

#include "axl_g_Def.h"

namespace axl {
namespace sl {
	
//.............................................................................

// compile-time type information

template <typename T>
class Type
{
public:
	static
	size_t 
	getSize ()
	{ 
		return sizeof (T);
	}

	static
	const char*
	getName ()
	{ 
		return typeid (T).name ();
	}

	static
	void
	construct (void* p)
	{
		new (p) T;
	}

	static
	void
	destruct (void* p)
	{
		((T*) p)->~T ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// specialization for void

template <>
class Type <void>
{
public:
	static
	size_t 
	getSize ()
	{ 
		return 0;
	}

	static
	const char*
	getName ()
	{ 
		return typeid (void).name ();
	}

	static
	void
	construct (void* p)
	{
	}

	static
	void
	destruct (void* p)
	{
	}
};

//.............................................................................

} // namespace sl
} // namespace axl
