// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_TLSSLOT_H

#include "axl_sys_TlsMgr.h"

namespace axl {
namespace sys {

//.............................................................................

template <typename T>
class TlsSlot
{
protected:
	size_t m_tlsSlot;
	
public:	
	TlsSlot ()
	{
		m_tlsSlot = getTlsMgr ()->createSlot ();
	}

	T*
	getValue ()
	{
		return (T*) getTlsMgr ()->getSlotValue (m_tlsSlot).p ();
	}

	T* 
	setValue (T* p)
	{
		return (T*) getTlsMgr ()->setSlotValue (m_tlsSlot, sys::TlsValue (p, NULL)).p ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
getTlsSlotValue ()
{
	return sl::getSimpleSingleton <TlsSlot <T> > ()->getValue ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
setTlsSlotValue (T* p)
{
	return sl::getSimpleSingleton <TlsSlot <T> > ()->setValue (p);
}

//.............................................................................

template <typename T>
class ScopeTlsSlot
{
protected:
	T* m_prevValue;

public:
	ScopeTlsSlot (T* p)
	{
		m_prevValue = setTlsSlotValue <T> (p);
	}

	~ScopeTlsSlot ()
	{
		setTlsSlotValue <T> (m_prevValue);
	}
};

//.............................................................................

} // namespace sys
} // namespace axl
