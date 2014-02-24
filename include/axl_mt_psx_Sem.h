// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_PSX_SEM_H

#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

class CSem
{
protected:
	sem_t m_Sem;
	
public:
	CSem (
		bool IsShared = false,
		uint_t Value = 0
		)
	{		
		sem_init (&m_Sem, IsShared, Value);
	}

	~CSem ()
	{
		sem_destroy (&m_Sem);
	}

	operator sem_t* ()
	{
		return &m_Sem;
	}
	
	bool 
	Post ()
	{
		int Result = sem_post (&m_Sem);
		return Result == 0 ? true : err::Fail (Result);
	}

	bool
	Wait ()
	{
		int Result = sem_wait (&m_Sem);
		return Result == 0 ? true : err::Fail (Result);
	}
	
	bool
	GetValue (int* pValue)
	{
		int Result = sem_getvalue (&m_Sem, pValue);
		return Result == 0 ? true : err::Fail (Result);
	}	
	
	bool
	Wait (uint_t Timeout);
};

//.............................................................................

} // namespace psx
} // namespace mt 
} // namespace axl
