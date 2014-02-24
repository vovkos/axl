// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_LOCK_H

#include "axl_g_Def.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_mt_win_CriticalSection.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_mt_psx_Mutex.h"
#endif

namespace axl {
namespace mt {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

class CLock
{
public:
	win::CCriticalSection m_CriticalSection;

public:
	void 
	Lock ()
	{
		m_CriticalSection.Enter ();
	}

	void 
	Unlock ()
	{
		m_CriticalSection.Leave ();
	}
};

#elif (_AXL_ENV == AXL_ENV_POSIX)

class CLock
{
public:
	psx::CMutex m_Mutex;

public:
	void 
	Lock ()
	{
		m_Mutex.Lock ();
	}

	void 
	Unlock ()
	{
		m_Mutex.Unlock ();
	}
};

#endif

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CScopeLock 
{
protected:
	CLock* m_pLock;

public:
	CScopeLock (CLock* pLock)
	{
		m_pLock = pLock;		
		pLock->Lock ();
	}

	~CScopeLock ()
	{
		m_pLock->Unlock ();
	}
};

//.............................................................................

} // namespace mt
} // namespace axl


