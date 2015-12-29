// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_DRW_SEMAPHORE_H

#include "axl_sys_drw_MachError.h"

namespace axl {
namespace mt {
namespace drw {

//.............................................................................

class Semaphore
{
protected:
	semaphore_t m_semaphore;

public:
	Semaphore ()
	{
		mach_error_t result = ::semaphore_create (mach_task_self (), &m_semaphore, SYNC_POLICY_FIFO, 0);
		ASSERT (result == 0);
	}

	~Semaphore ()
	{
		mach_error_t result = ::semaphore_destroy (mach_task_self (), m_semaphore);
		ASSERT (result == 0);
	}

	operator semaphore_t* ()
	{
		return &m_semaphore;
	}

	bool
	signal ()
	{
		mach_error_t result = ::semaphore_signal (m_semaphore);
		return result == 0 ? true : err::fail (sys::drw::MachError (result));
	}

	bool
	signalAll ()
	{
		mach_error_t result = ::semaphore_signal_all (m_semaphore);
		return result == 0 ? true : err::fail (sys::drw::MachError (result));
	}

	bool
	wait ()
	{
		mach_error_t result = ::semaphore_wait (m_semaphore);
		return result == 0 ? true : err::fail (sys::drw::MachError (result));
	}

	bool
	wait (uint_t timeout);
};

//.............................................................................

} // namespace drw
} // namespace mt
} // namespace axl
