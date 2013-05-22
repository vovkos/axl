// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	intptr_t m_Count;

public:
	CInvSemaphore (intptr_t Count = 0):
		CEvent (CEvent::EKind_Notification)
	{
		SetImpl (Count);
	}

	void
	Set (intptr_t Count)
	{
		m_Lock.Lock ();
		SetImpl (Count);
		m_Lock.Unlock ();
	}

	void
	Add (intptr_t Delta)
	{
		m_Lock.Lock ();
		SetImpl (m_Count + Delta);
		m_Lock.Unlock ();
	}

protected:
	void
	SetImpl (intptr_t Count) // called under lock
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

