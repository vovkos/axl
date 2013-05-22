// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_REF_CHILD_H

#include "axl_ref_RefCount.h"
#include "axl_ref_Ptr.h"

namespace axl {
namespace ref {

//.............................................................................

// creatable child object

template <
	typename T,
	size_t Extra = 0
	>
class CChildT
{
protected:
	struct THdr
	{
		IRefCount* m_pParent;
	};

	class CObject: 
		public THdr,
		public T
	{
	public:
		AXL_OBJ_IMPLEMENT_GET_OBJECT (CObject)

		AXL_OBJ_BEGIN_INTERFACE_MAP (CObject)
			AXL_OBJ_INTERFACE_ENTRY (IRefCount)
			AXL_OBJ_INTERFACE_CHAIN (T)
		AXL_OBJ_END_INTERFACE_MAP ()

	public:
		CObject () // for cloning
		{
		}

		CObject (IRefCount* pParent)
		{
			m_pfFree = &CObject::Free; 
			m_pParent = pParent;
			m_pParent->AddWeakRef ();
		}

	protected:
		static 
		void 
		Free (void* p)
		{ 			
			((CObject*) p)->m_pParent->WeakRelease (); 
		}
	};

protected:
	char m_Buffer [sizeof (CObject) + Extra];

public:
	CChildT (IRefCount* pParent)
	{ 
		memset (m_Buffer, 0, sizeof (m_Buffer));
		new (m_Buffer) CObject (pParent); 
		GetObject ()->AddRef ();
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
};

//.............................................................................

} // namespace ref
} // namespace axl
