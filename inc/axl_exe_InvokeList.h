// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_EXE_INVOKELIST_H

#include "axl_rtl_List.h"
#include "axl_exe_Arg.h"
#include "axl_ref_Ptr.h"

namespace axl {
namespace exe {

struct IFunction;

//.............................................................................

// most schedulers would maintain a list of pending invokations

class CInvokeList
{
protected:
	class CEntry: public rtl::TListLink
	{
	public:
		ref::CPtrT <CArgBlock> m_Stack;
		ref::CPtrT <IFunction> m_Function;
	};

protected:
	rtl::CStdListT <CEntry> m_List;

public:
	bool 
	IsEmpty () const 
	{
		return m_List.IsEmpty ();
	}

	size_t 
	GetCount () const 
	{
		return m_List.GetCount ();
	}

	void 
	Clear ()
	{
		m_List.Clear ();
	}

	void 
	TakeOver (CInvokeList* pSrc)
	{ 
		m_List.TakeOver (&pSrc->m_List);
	}

	bool 
	AddV (
		IFunction* pFunction, 
		va_list va
		);

	bool 
	Add__ (
		IFunction* pFunction, 
		...
		)
	{
		return AddV (pFunction, va_start_e (pFunction));
	}

	size_t 
	Process (size_t Count = -1);
};

//.............................................................................

} // namespace exe
} // namespace axl
