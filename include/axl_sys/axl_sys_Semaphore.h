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

#define _AXL_SYS_SEMAPHORE_H

#if (_AXL_OS_WIN)
#	include "axl_sys_win_Semaphore.h"
#elif (_AXL_OS_POSIX)

#endif

#include "axl_sys_psx_Sem.h"

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

class NamedSemaphore
{
public:
	win::Semaphore m_semaphore;

public:
	bool
	isOpen ()
	{
		return m_semaphore.isOpen ();
	}

	void
	close ()
	{
		m_semaphore.close ();
	}

	bool
	create ()
	{
		return m_semaphore.create (NULL, (uint_t) initialCount, MAXLONG, NULL);
	}

	bool
	signal ()
	{
		return m_semaphore.signal (1);
	}

	bool
	wait ()
	{
		return m_semaphore.wait (-1) == win::WaitResult_Object0;
	}
};

#elif (_AXL_OS_POSIX)



#endif

class NamedSemaphore
{
public:
	psx::NamedSem m_sem;

public:
	bool
	isOpen ()
	{
		return m_sem.isOpen ();
	}

	void
	close ()
	{
		m_sem.close ();
	}

	bool
	create (const sl::StringRef& name)
	{
		return m_sem.open (name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 0);
	}

	bool
	open (const sl::StringRef& name)
	{
		return m_sem.open (name, 0, 0, 0);
	}

	bool
	signal ()
	{
		return m_sem.post ();
	}

	bool
	tryWait ()
	{
		return m_sem.tryWait ();
	}

	bool
	wait ()
	{
		return m_sem.wait ();
	}
};

//..............................................................................

} // namespace sys
} // namespace axl
