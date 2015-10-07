// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_INVOKELIST_H

#include "axl_sl_List.h"
#include "axl_exe_Arg.h"
#include "axl_ref_Ptr.h"

namespace axl {
namespace exe {

struct IFunction;

//.............................................................................

// most schedulers would maintain a list of pending invokations

class InvokeList
{
protected:
	class Entry: public sl::ListLink
	{
	public:
		ref::Ptr <ArgBlock> m_stack;
		ref::Ptr <IFunction> m_function;
	};

protected:
	sl::StdList <Entry> m_list;

public:
	bool 
	isEmpty () const 
	{
		return m_list.isEmpty ();
	}

	size_t 
	getCount () const 
	{
		return m_list.getCount ();
	}

	void 
	clear ()
	{
		m_list.clear ();
	}

	void 
	takeOver (InvokeList* src)
	{ 
		m_list.takeOver (&src->m_list);
	}

	bool 
	addV (
		IFunction* function, 
		axl_va_list va
		);

	bool 
	add__ (
		IFunction* function, 
		...
		)
	{
		AXL_VA_DECL (va, function);
		return addV (function, va);
	}

	size_t 
	process (size_t count = -1);
};

//.............................................................................

} // namespace exe
} // namespace axl
