// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_PSX_MUTEX_H

#include "axl_g_Def.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

class CMutex
{
protected:
	pthread_mutex_t m_Mutex;
	
public:
	CMutex ()
	{		
		pthread_mutex_init (&m_Mutex, NULL);
	}

	~CMutex ()
	{
		pthread_mutex_destroy (&m_Mutex);
	}

	void 
	Lock ()
	{
		pthread_mutex_lock (&m_Mutex);
	}

	void 
	Unlock ()
	{
		pthread_mutex_unlock (&m_Mutex);
	}
};

//.............................................................................

} // namespace psx
} // namespace mt 
} // namespace axl
