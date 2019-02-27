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

#define _AXL_MEM_WIN_VIRTUAL_MEMORY_H

#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class VirtualMemory
{
protected:
	void* m_p;
	size_t m_size;

public:
	VirtualMemory()
	{
		m_p = NULL;
		m_size = 0;
	}

	~VirtualMemory()
	{
		release();
	}

	operator void* () const
	{
		return m_p;
	}

	void* p() const
	{
		return m_p;
	}

	size_t
	getSize() const
	{
		return m_size;
	}

	void*
	alloc(
		void* p,
		size_t size,
		uint_t allocationFlags = MEM_RESERVE | MEM_COMMIT,
		uint_t protection = PAGE_READWRITE
		);

	void*
	alloc(
		size_t size,
		uint_t allocationFlags = MEM_RESERVE | MEM_COMMIT,
		uint_t protectionFlags = PAGE_READWRITE
		)
	{
		return alloc(NULL, size, allocationFlags, protectionFlags);
	}

	static
	bool
	commit(
		void* p,
		size_t size,
		uint_t protectionFlags = PAGE_READWRITE
		)
	{
		void* result = ::VirtualAlloc(p, size, MEM_COMMIT, protectionFlags);
		return err::complete(result != NULL);
	}

	bool
	commit(uint_t protectionFlags = PAGE_READWRITE)
	{
		return commit(m_p, m_size, protectionFlags);
	}

	static
	bool
	decommit(
		void* p,
		size_t size
		)
	{
		bool_t result = ::VirtualFree(p, size, MEM_DECOMMIT);
		return err::complete(result);
	}

	bool
	decommit()
	{
		return decommit(m_p, m_size);
	}

	void
	release();

	bool
	protect(
		uint_t protectionFlags,
		uint_t* prevProtectionFlags = NULL
		);

	static
	bool
	lock(
		void* p,
		size_t size
		)
	{
		bool_t result = ::VirtualLock(p, size);
		return err::complete(result);
	}

	bool
	lock()
	{
		return lock(m_p, m_size);
	}

	static
	bool
	unlock(
		void* p,
		size_t size
		)
	{
		bool_t result = ::VirtualUnlock(p, size);
		return err::complete(result);
	}

	bool
	unlock()
	{
		return unlock(m_p, m_size);
	}

	static
	bool
	query(
		void* p,
		MEMORY_BASIC_INFORMATION* information
		)
	{
		size_t result = ::VirtualQuery(p, information, sizeof(MEMORY_BASIC_INFORMATION));
		return err::complete(result != 0);
	}

	bool
	query(MEMORY_BASIC_INFORMATION* information)
	{
		return query(m_p, information);
	}
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
