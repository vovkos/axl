// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SL_ARRAYLIST_H

#include "axl_sl_Array.h"
#include "axl_sl_List.h"

namespace axl {
namespace sl {

//..............................................................................

template <typename T>
class StdArrayList
{
public:
	typedef sl::Iterator <T> Iterator;

protected:
	sl::StdList <T> m_list;
	sl::Array <T*> m_array;

public:
	bool
	isEmpty () const
	{
		return m_list.isEmpty ();
	}

	size_t
	getCount () const
	{
		ASSERT (m_list.getCount () == m_array.getCount ());
		return m_list.getCount ();
	}

	Iterator
	getHead () const
	{
		return m_list.getHead ();
	}

	Iterator
	getTail () const
	{
		return m_list.getTail ();
	}

	T*
	add ()
	{
		T* p = AXL_MEM_NEW (T);
		m_list.insertTail (p);

		size_t count = m_array.getCount ();
		m_array.setCount (count + 1);
		m_array [count] = p;
		return p;
	}

	T*
	get (size_t index)
	{
		size_t count = m_array.getCount ();
		if (index < count)
			return m_array [index];

		m_array.setCount (index + 1);

		for (size_t i = count; i <= index; i++)
		{
			T* p = AXL_MEM_NEW (T);
			m_list.insertTail (p);
			m_array [i] = p;
		}

		return m_array [index];
	}
};

//..............................................................................

} // namespace sl
} // namespace axl
