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
class CTlsSlotT
{
protected:
	size_t m_TlsSlot;
	
public:	
	CTlsSlotT ()
	{
		m_TlsSlot = GetTlsMgr ()->CreateSlot ();
	}

	T*
	GetValue ()
	{
		return (T*) GetTlsMgr ()->GetSlotValue (m_TlsSlot).p ();
	}

	T* 
	SetValue (T* p)
	{
		return (T*) GetTlsMgr ()->SetSlotValue (m_TlsSlot, mt::CTlsValue (p, NULL)).p ();
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
GetTlsSlotValue ()
{
	return rtl::GetSimpleSingleton <CTlsSlotT <T> > ()->GetValue ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
T*
SetTlsSlotValue (T* p)
{
	return rtl::GetSimpleSingleton <CTlsSlotT <T> > ()->SetValue (p);
}

//.............................................................................

template <typename T>
class CScopeTlsSlotT
{
protected:
	T* m_pPrevValue;

public:
	CScopeTlsSlotT (T* p)
	{
		m_pPrevValue = SetTlsSlotValue <T> (p);
	}

	~CScopeTlsSlotT ()
	{
		SetTlsSlotValue <T> (m_pPrevValue);
	}
};

//.............................................................................

} // namespace mt
} // namespace axl
