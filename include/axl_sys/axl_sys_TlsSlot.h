//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_SYS_TLSSLOT_H

#include "axl_sys_TlsMgr.h"

namespace axl {
namespace sys {

//..............................................................................

class TlsSlot
{
protected:
	size_t m_slot;

public:
	TlsSlot()
	{
		m_slot = getTlsMgr()->createSlot();
	}

	size_t
	getSlot()
	{
		return m_slot;
	}

	operator size_t()
	{
		return m_slot;
	}
};

//..............................................................................

template <typename T>
class TlsPtrSlot: public TlsSlot
{
public:
	T*
	getValue()
	{
		return (T*)getTlsMgr()->getSlotValue(m_slot).p();
	}

	T*
	setValue(T* p)
	{
		return (T*)getTlsMgr()->setSlotValue(m_slot, sys::TlsValue(p, NULL)).p();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
getTlsPtrSlotValue()
{
	return sl::getSimpleSingleton<TlsPtrSlot<T> > ()->getValue();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
setTlsPtrSlotValue(T* p)
{
	return sl::getSimpleSingleton<TlsPtrSlot<T> > ()->setValue(p);
}

//..............................................................................

template <typename T>
class ScopedTlsPtrSlot
{
protected:
	T* m_prevValue;

public:
	ScopedTlsPtrSlot(T* p)
	{
		m_prevValue = setTlsPtrSlotValue<T> (p);
	}

	~ScopedTlsPtrSlot()
	{
		setTlsPtrSlotValue<T> (m_prevValue);
	}
};

//..............................................................................

} // namespace sys
} // namespace axl
