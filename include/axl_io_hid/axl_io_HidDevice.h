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

#define _AXL_IO_HIDDEVICE_H

#include "axl_io_HidPch.h"
#include "axl_sl_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {

//..............................................................................

inline
err::Error
setHidError(hid_device* device = NULL) {
	return err::setError(sl::String(::hid_error(device)));
}

template <typename T>
T
hidComplete(
	T result,
	T failResult,
	hid_device* device = NULL
) {
	if (result == failResult)
		setHidError(device);

	return result;
}

inline
bool
hidComplete(
	int result,
	hid_device* device = NULL
) {
	return hidComplete<bool>(result != 0, false, device);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
hidInit() {
	int result = ::hid_init();
	return hidComplete(result);
}

inline
bool
hidExit() {
	int result = ::hid_exit();
	return hidComplete(result);
}

const char*
getHidBusTypeString(hid_bus_type busType);

//..............................................................................

class CloseHidDevice {
public:
	void
	operator () (hid_device* device) {
		::hid_close(device);
	}
};

//..............................................................................

class HidDevice: public sl::Handle<hid_device*, CloseHidDevice> {
public:
	enum {
		StringBufferLength = 256,
	};

public:
	bool
	open(
		uint16_t vid,
		uint16_t pid
	) {
		close();
		m_h = ::hid_open(vid, pid, NULL);
		return hidComplete(m_h != NULL);
	}

	bool
	open(
		uint16_t vid,
		uint16_t pid,
		const sl::StringRef_w& serial
	) {
		close();
		m_h = ::hid_open(vid, pid, serial.sz());
		return hidComplete(m_h != NULL);
	}

	bool
	open(
		uint16_t vid,
		uint16_t pid,
		const sl::StringRef& serial
	);

	bool
	open(const sl::StringRef& path) {
		close();
		m_h = ::hid_open_path(path.sz());
		return hidComplete(m_h != NULL);
	}

	const hid_device_info*
	getDeviceInfo() const {
		return ::hid_get_device_info(m_h);
	}

	template <typename T>
	bool
	getIndexedString(
		sl::StringBase<T>* string,
		int i
	) const;

	sl::String
	getIndexedString(int i) const {
		sl::String string;
		getIndexedString(&string, i);
		return string;
	}

	template <typename T>
	bool
	getManufacturerString(sl::StringBase<T>* string) const;

	sl::String
	getManufacturerString() const {
		sl::String string;
		getManufacturerString(&string);
		return string;
	}

	template <typename T>
	bool
	getProductString(sl::StringBase<T>* string) const;

	sl::String
	getProductString() const {
		sl::String string;
		getProductString(&string);
		return string;
	}

	template <typename T>
	bool
	getSerialNumberString(sl::StringBase<T>* string) const;

	sl::String
	getSerialNumberString() const {
		sl::String string;
		getSerialNumberString(&string);
		return string;
	}

	bool
	setNonBlocking(bool isNonBlocking = true) {
		int result = ::hid_set_nonblocking(m_h, isNonBlocking);
		return hidComplete(result, m_h);
	}

	size_t
	read(
		void* p,
		size_t size
	) {
		int result = ::hid_read(m_h, (uchar_t*)p, size);
		return hidComplete<size_t>(result, -1, m_h);
	}

	size_t
	read(
		void* p,
		size_t size,
		uint_t timeout
	) {
		int result = ::hid_read_timeout(m_h, (uchar_t*)p, size, timeout);
		return hidComplete<size_t>(result, -1, m_h);
	}

	size_t
	write(
		const void* p,
		size_t size
	) {
		int result = ::hid_write(m_h, (uchar_t*)p, size);
		return hidComplete<size_t>(result, -1, m_h);
	}

	size_t
	getReportDescriptor(
		void* p,
		size_t size
	) {
		int result = ::hid_get_report_descriptor(m_h, (uchar_t*)p, size);
		return hidComplete<size_t>(result, -1, m_h);
	}

	size_t
	getInputReport(
		const void* p,
		size_t size
	) {
		int result = ::hid_get_input_report(m_h, (uchar_t*)p, size);
		return hidComplete<size_t>(result, -1, m_h);
	}

	size_t
	getFeatureReport(
		const void* p,
		size_t size
	) {
		int result = ::hid_get_feature_report(m_h, (uchar_t*)p, size);
		return hidComplete<size_t>(result, -1, m_h);
	}

	size_t
	sendFeatureReport(
		const void* p,
		size_t size
	) {
		int result = ::hid_send_feature_report(m_h, (uchar_t*)p, size);
		return hidComplete<size_t>(result, -1, m_h);
	}

protected:
	template <typename T>
	bool
	completeGetString(
		sl::StringBase<T>* string,
		const wchar_t* buffer,
		int result
	) const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
HidDevice::open(
	uint16_t vid,
	uint16_t pid,
	const sl::StringRef& serial
) {
	char buffer[256];
	sl::String_w serial_w(rc::BufKind_Stack, buffer, sizeof(buffer));
	serial_w = serial;
	return open(vid, pid, serial_w);
}

template <typename T>
bool
HidDevice::completeGetString(
	sl::StringBase<T>* string,
	const wchar_t* buffer,
	int result
) const {
	if (result == -1) {
		setHidError(m_h);
		return false;
	} else {
		*string = buffer;
		return true;
	}
}

template <typename T>
bool
HidDevice::getIndexedString(
	sl::StringBase<T>* string,
	int i
) const {
	wchar_t buffer[StringBufferLength];
	int result = ::hid_get_indexed_string(m_h, i, buffer, StringBufferLength);
	return completeGetString(string, buffer, result);
}

template <typename T>
bool
HidDevice::getManufacturerString(sl::StringBase<T>* string) const {
	wchar_t buffer[StringBufferLength];
	int result = ::hid_get_manufacturer_string(m_h, buffer, StringBufferLength);
	return completeGetString(string, buffer, result);
}

template <typename T>
bool
HidDevice::getProductString(sl::StringBase<T>* string) const {
	wchar_t buffer[StringBufferLength];
	int result = ::hid_get_product_string(m_h, buffer, StringBufferLength);
	return completeGetString(string, buffer, result);
}

template <typename T>
bool
HidDevice::getSerialNumberString(sl::StringBase<T>* string) const {
	wchar_t buffer[StringBufferLength];
	int result = ::hid_get_serial_number_string(m_h, buffer, StringBufferLength);
	return completeGetString(string, buffer, result);
}

//..............................................................................

} // namespace io
} // namespace axl
