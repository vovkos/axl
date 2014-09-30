// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_CHILD_H

#include "axl_ref_RefCount.h"
#include "axl_rtl_Type.h"

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
	struct Hdr
	{
		RefCount* m_parent;
	};
	
	class Object: 
		public Hdr,
		public T
	{
	};

protected:
	char m_buffer [sizeof (Object) + extra];

public:
	Child (RefCount* parent)
	{ 
		memset (m_buffer, 0, sizeof (m_buffer));
		Object* object = (Object*) m_buffer;
		object->setTarget (object, &rtl::Type <Object>::destruct, &free);
		object->addRef ();
		parent->addWeakRef ();
	}

	~Child ()
	{ 
		getObject ()->release (); 
	}
	
	operator T* ()
	{ 
		return getObject (); 
	}

	T*
	operator & ()
	{ 
		return getObject (); 
	}

	T*
	operator -> ()
	{ 
		return getObject (); 
	}

	T* getObject ()
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
