// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_TLSSLOT_H

#include "axl_mt_TlsMgr.h"

namespace axl {
namespace mt {

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
		return (T*) getTlsMgr ()->setSlotValue (m_tlsSlot, mt::TlsValue (p, NULL)).p ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
getTlsSlotValue ()
{
	return mt::getSimpleSingleton <TlsSlot <T> > ()->getValue ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
setTlsSlotValue (T* p)
{
	return mt::getSimpleSingleton <TlsSlot <T> > ()->setValue (p);
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

} // namespace mt
} // namespace axl
