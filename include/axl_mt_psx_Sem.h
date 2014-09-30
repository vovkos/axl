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

class Sem
{
protected:
	sem_t* m_sem;
	sem_t m_unnamedSem;

public:
	Sem (
		bool isShared = false,
		uint_t value = 0
		)
	{
		m_sem = NULL;
		init (isShared, value);
	}

	~Sem ()
	{
		close ();
	}

	operator sem_t* ()
	{
		return m_sem;
	}

	bool
	init (
		bool isShared = false,
		uint_t value = 0
		);

	bool
	open (
		const char* name,
		int flags = O_CREAT,
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
		uint_t value = 0
		);

	void
	close ();

	static
	bool
	unlink (const char* name)
	{
		int result = sem_unlink (name);
		return err::complete (result == 0);
	}

	bool
	post ()
	{
		int result = sem_post (m_sem);
		return err::complete (result == 0);
	}

	bool
	wait ()
	{
		int result = sem_wait (m_sem);
		return err::complete (result == 0);
	}

	bool
	getValue (int* value)
	{
		int result = sem_getvalue (m_sem, value);
		return err::complete (result == 0);
	}

	bool
	wait (uint_t timeout);
};

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
