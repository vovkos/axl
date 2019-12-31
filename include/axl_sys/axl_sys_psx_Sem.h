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

#define _AXL_SYS_PSX_SEM_H

#include "axl_err_Error.h"
#include "axl_sl_Handle.h"

namespace axl {
namespace sys {
namespace psx {


//..............................................................................

#if (!_AXL_OS_DARWIN) // no unnamed semaphores on darwin

class Sem
{
protected:
	sem_t m_sem;

public:
	Sem(
		bool isShared = false,
		uint_t value = 0
		)
	{
		int result = sem_init(&m_sem, isShared, value);
		ASSERT(result == 0);
	}

	~Sem()
	{
		int result = ::sem_destroy(&m_sem);
		ASSERT(result == 0);
	}

	operator sem_t* ()
	{
		return &m_sem;
	}

	bool
	post()
	{
		int result = ::sem_post(&m_sem);
		return err::complete(result == 0);
	}

	bool
	signal()
	{
		return post();
	}

	bool
	tryWait()
	{
		int result = ::sem_trywait(&m_sem);
		return err::complete(result == 0);
	}

	bool
	wait()
	{
		int result = ::sem_wait(&m_sem);
		return err::complete(result == 0);
	}

	bool
	wait(uint_t timeout);

	bool
	getValue(int* value)
	{
		int result = ::sem_getvalue(&m_sem, value);
		return err::complete(result == 0);
	}
};

#endif

//..............................................................................

class CloseNamedSem
{
public:
	void
	operator () (sem_t* h)
	{
		::sem_close(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class NamedSem: public sl::Handle<sem_t*, CloseNamedSem>
{
public:
	bool
	open(
		const sl::StringRef& name,
		int flags = O_CREAT,
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
		uint_t value = 0
		);

	static
	bool
	unlink(const sl::StringRef& name)
	{
		int result = ::sem_unlink(name.sz());
		return err::complete(result == 0);
	}

	bool
	post()
	{
		int result = ::sem_post(m_h);
		return err::complete(result == 0);
	}

	bool
	signal()
	{
		return post();
	}

	bool
	tryWait()
	{
		int result = ::sem_trywait(m_h);
		return err::complete(result == 0);
	}

	bool
	wait()
	{
		int result = ::sem_wait(m_h);
		return err::complete(result == 0);
	}

#if (!_AXL_OS_DARWIN)
	bool
	wait(uint_t timeout);

	bool
	getValue(int* value)
	{
		int result = ::sem_getvalue(m_h, value);
		return err::complete(result == 0);
	}
#endif
};

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
