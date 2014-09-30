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
class CChildT
{
	AXL_DISABLE_COPY (CChildT)

protected:
	struct THdr
	{
		CRefCount* m_parent;
	};
	
	class CObject: 
		public THdr,
		public T
	{
	};

protected:
	char m_buffer [sizeof (CObject) + extra];

public:
	CChildT (CRefCount* parent)
	{ 
		memset (m_buffer, 0, sizeof (m_buffer));
		CObject* object = (CObject*) m_buffer;
		object->setTarget (object, &rtl::CTypeT <CObject>::destruct, &free);
		object->addRef ();
		parent->addWeakRef ();
	}

	~CChildT ()
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
		return (CObject*) m_buffer; 
	}

protected:
	static 
	void 
	free (void* p)
	{ 			
		((CObject*) p)->m_parent->weakRelease (); 
	}
};

//.............................................................................

} // namespace ref
} // namespace axl
