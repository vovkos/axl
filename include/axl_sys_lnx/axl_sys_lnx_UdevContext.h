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

#define _AXL_SYS_LNX_UDEVCONTEXT_H

#include "axl_sl_Handle.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

// handle closing

template <typename T>
class UnrefUdevContext
{
public:
	void
	operator () (udev* h)
	{
		T::udev_unref(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class UdevContextBase: public sl::Handle<udev*, UnrefUdevContext<T> >
{
public:
	UdevContextBase()
	{
		create();
	}

	UdevContextBase(udev* h)
	{
		this->m_h = h;
	}

	bool
	create()
	{
		this->close();
		return this->m_h = T::udev_new();
	}

	void
	addRef()
	{
		T::udev_ref(this->m_h);
	}

	udev_device*
	getDeviceFromSysPath(const sl::StringRef& sysPath)
	{
		return T::udev_device_new_from_syspath(this->m_h, sysPath.sz());
	}

	udev_device*
	getDeviceFromDevNum(
		char type,
		dev_t devNum
		)
	{
		return T::udev_device_new_from_devnum(this->m_h, type, devNum);
	}

	udev_device*
	getDeviceFromSubsytemSysName(
		const sl::StringRef& subsystem,
		const sl::StringRef& sysName
		)
	{
		return T::udev_device_new_from_subsystem_sysname(this->m_h, subsystem.sz(), sysName.sz());
	}

#if (_AXL_SYS_LNX_LIBUDEV_NEW_API)
	udev_device*
	getDeviceFromDeviceId(const sl::StringRef& deviceId)
	{
		return T::udev_device_new_from_device_id(this->m_h, (char*)deviceId.sz());
	}
#endif

	udev_device*
	getDeviceFromEnvironment()
	{
		return T::udev_device_new_from_environment(this->m_h);
	}

	udev_enumerate*
	createEnumerator()
	{
		return T::udev_enumerate_new(this->m_h);
	}

	udev_monitor*
	createMonitor(const sl::StringRef& name)
	{
		return T::udev_monitor_new_from_netlink(this->m_h, name.sz());
	}
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
