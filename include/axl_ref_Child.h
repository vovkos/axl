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
	size_t Extra = 0
	>
class CChildT
{
	AXL_DISABLE_COPY (CChildT)

protected:
	struct THdr
	{
		CRefCount* m_pParent;
	};
	
	class CObject: 
		public THdr,
		public T
	{
	};

protected:
	char m_Buffer [sizeof (CObject) + Extra];

public:
	CChildT (CRefCount* pParent)
	{ 
		memset (m_Buffer, 0, sizeof (m_Buffer));
		CObject* pObject = (CObject*) m_Buffer;
		pObject->SetTarget (pObject, &rtl::CTypeT <CObject>::Destruct, &Free);
		pObject->AddRef ();
		pParent->AddWeakRef ();
	}

	~CChildT ()
	{ 
		GetObject ()->Release (); 
	}
	
	operator T* ()
	{ 
		return GetObject (); 
	}

	T*
	operator & ()
	{ 
		return GetObject (); 
	}

	T*
	operator -> ()
	{ 
		return GetObject (); 
	}

	T* GetObject ()
	{
		return (CObject*) m_Buffer; 
	}

protected:
	static 
	void 
	Free (void* p)
	{ 			
		((CObject*) p)->m_pParent->WeakRelease (); 
	}
};

//.............................................................................

} // namespace ref
} // namespace axl
