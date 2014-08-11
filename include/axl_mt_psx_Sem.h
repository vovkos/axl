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
	sem_t* m_pSem;
	sem_t m_UnnamedSem;

public:
	CSem (
		bool IsShared = false,
		uint_t Value = 0
		)
	{
		m_pSem = NULL;
		Init (IsShared, Value);
	}

	~CSem ()
	{
		Close ();
	}

	operator sem_t* ()
	{
		return m_pSem;
	}

	bool
	Init (
		bool IsShared = false,
		uint_t Value = 0
		);

	bool
	Open (
		const char* pName,
		int Flags = O_CREAT,
		mode_t Mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
		uint_t Value = 0
		);

	void
	Close ();

	static
	bool
	Unlink (const char* pName)
	{
		int Result = sem_unlink (pName);
		return err::Complete (Result == 0);
	}

	bool
	Post ()
	{
		int Result = sem_post (m_pSem);
		return err::Complete (Result == 0);
	}

	bool
	Wait ()
	{
		int Result = sem_wait (m_pSem);
		return err::Complete (Result == 0);
	}

	bool
	GetValue (int* pValue)
	{
		int Result = sem_getvalue (m_pSem, pValue);
		return err::Complete (Result == 0);
	}

	bool
	Wait (uint_t Timeout);
};

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
