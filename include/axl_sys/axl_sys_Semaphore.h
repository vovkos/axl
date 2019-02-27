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
#	include "axl_sys_psx_Sem.h"
#endif

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
	isOpen()
	{
		return m_semaphore.isOpen();
	}

	void
	close()
	{
		m_semaphore.close();
	}

	bool
	create(const sl::StringRef& name)
	{
		return m_semaphore.create(NULL, 0, MAXLONG, name.s2());
	}

	bool
	open(const sl::StringRef& name)
	{
		return m_semaphore.open(SEMAPHORE_ALL_ACCESS, false, name.s2());
	}

	bool
	signal(size_t count = 1)
	{
		return m_semaphore.signal(count);
	}

	bool
	wait()
	{
		return m_semaphore.wait(-1) == win::WaitResult_Object0;
	}
};

#elif (_AXL_OS_POSIX)

class NamedSemaphore
{
public:
	psx::NamedSem m_sem;

public:
	bool
	isOpen()
	{
		return m_sem.isOpen();
	}

	void
	close()
	{
		m_sem.close();
	}

	bool
	create(const sl::StringRef& name)
	{
		return m_sem.open(name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, 0);
	}

	bool
	open(const sl::StringRef& name)
	{
		return m_sem.open(name, 0, 0, 0);
	}

	bool
	signal()
	{
		return m_sem.post();
	}

	bool
	signal(size_t count)
	{
		for (; count; count--)
		{
			bool result = m_sem.post();
			if (!result)
				return false;
		}

		return true;
	}

	bool
	tryWait()
	{
		return m_sem.tryWait();
	}

	bool
	wait()
	{
		return m_sem.wait();
	}
};

#endif

//..............................................................................

} // namespace sys
} // namespace axl
