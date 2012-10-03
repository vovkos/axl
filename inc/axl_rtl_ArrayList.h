// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_RTL_ARRAYLIST_H

#include "axl_rtl_Array.h"
#include "axl_rtl_List.h"

namespace axl {
namespace rtl {

//.............................................................................

template <typename T>
class CStdArrayListT
{
public:
	typedef CIteratorT <T> CIterator;

protected:
	rtl::CStdListT <T> m_List;
	rtl::CArrayT <T*> m_Array;

public:
	bool
	IsEmpty () const
	{
		return m_List.IsEmpty ();
	}

	size_t 
	GetCount () const
	{
		ASSERT (m_List.GetCount () == m_Array.GetCount ());
		return m_List.GetCount ();
	}

	CIterator
	GetHead () const
	{
		return m_List.GetHead ();
	}

	CIterator
	GetTail () const
	{
		return m_List.GetTail ();
	}

	T*
	Add ()
	{
		T* p = AXL_MEM_NEW (T);
		m_List.InsertTail (p);
		
		size_t Count = m_Array.GetCount ();
		m_Array.SetCount (Count + 1);
		m_Array [Count] = p;
		return p;
	}

	T*
	Get (size_t Index)
	{
		size_t Count = m_Array.GetCount ();
		if (Index < Count)
			return m_Array [Index];

		m_Array.SetCount (Index + 1);
			
		for (size_t i = Count; i <= Index; i++)
		{
			T* p = AXL_MEM_NEW (T);
			m_List.InsertTail (p);
			m_Array [i] = p;
		}

		return m_Array [Index];
	}
};

//.............................................................................

} // namespace rtl
} // namespace axl
