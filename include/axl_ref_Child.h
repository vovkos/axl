// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_CHILD_H

#include "axl_ref_RefCount.h"
#include "axl_rtl_Func.h"

namespace axl {
namespace ref {

//.............................................................................

// creatable child ref-counted object

template <
	typename T,
	size_t extra = 0
	>
class Child
{
	AXL_DISABLE_COPY (Child)

protected:
	char m_buffer [sizeof (T) + extra];

public:
	Child (RefCount* parent)
	{ 
		memset (m_buffer, 0, sizeof (m_buffer));
		T* p = AXL_REF_INPLACE_NEW (m_buffer, 0);
		p->addRef ();
		p->addWeakRef ();
	}

	~Child ()
	{ 
		p ()->release (); 
	}
	
	operator T* ()
	{ 
		return p (); 
	}

	T*
	operator & ()
	{ 
		return p (); 
	}

	T*
	operator -> ()
	{ 
		return p (); 
	}

	T* p ()
	{
		return (Object*) m_buffer; 
	}

protected:
	static 
	void 
	free (void* p)
	{ 			
		((Object*) p)->m_parent->weakRelease (); 
	}
};

//.............................................................................

} // namespace ref
} // namespace axl
