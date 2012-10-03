// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_INVSEMAPHORE_H

#include "axl_mt_Lock.h"
#include "axl_mt_Event.h"

namespace axl {
namespace mt {

//.............................................................................

class CInvSemaphore: public CEvent
{
protected:
	CLock m_Lock;
	long m_Count;

public:
	CInvSemaphore (long Count = 0):
		CEvent (CEvent::EKind_Notification)
	{
		SetImpl (Count);
	}

	void
	Set (long Count)
	{
		m_Lock.Lock ();
		SetImpl (Count);
		m_Lock.Unlock ();
	}

	void
	Add (long Delta)
	{
		m_Lock.Lock ();
		SetImpl (m_Count + Delta);
		m_Lock.Unlock ();
	}

protected:
	void
	SetImpl (long Count) // called under lock
	{
		m_Count = Count;
		
		if (m_Count)
			Reset ();
		else
			Signal ();
	}

};

//.............................................................................

} // namespace sys
} // namespace axl

