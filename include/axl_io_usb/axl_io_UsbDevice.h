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

#define _AXL_IO_USBDEVICE_H

#include "axl_io_UsbError.h"

namespace axl {
namespace io {

#if (LIBUSB_API_VERSION >= 0x01000000)
#	define _AXL_IO_USBDEVICE_GETPORTPATH    1
#endif

#if (LIBUSB_API_VERSION >= 0x01000103)
#	define _AXL_IO_USBDEVICE_GETPORTNUMBERS 1
#endif

//..............................................................................

const char*
getUsbSpeedString(libusb_speed speed);

const char*
getUsbClassCodeString(libusb_class_code classCode);

const char*
getUsbTransferTypeString(libusb_transfer_type transferType);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const libusb_interface_descriptor*
findUsbInterfaceDescriptor(
	const libusb_config_descriptor* configDesc,
	uint_t interfaceId,
	uint_t altSettingId
);

const libusb_endpoint_descriptor*
findUsbEndpointDescriptor(
	const libusb_interface_descriptor* ifaceDesc,
	uint_t endpointId
);

inline
const libusb_endpoint_descriptor*
findUsbEndpointDescriptor(
	const libusb_config_descriptor* configDesc,
	uint_t endpointId,
	uint_t interfaceId = 0,
	uint_t altSettingId = 0
) {
	const libusb_interface_descriptor* ifaceDesc = findUsbInterfaceDescriptor(configDesc, interfaceId, altSettingId);
	return ifaceDesc ? findUsbEndpointDescriptor(ifaceDesc, endpointId) : NULL;
}

//..............................................................................

class FreeUsbConfigDescriptor {
public:
	void
	operator () (libusb_config_descriptor* h) {
		libusb_free_config_descriptor(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef sl::Handle<libusb_config_descriptor*, FreeUsbConfigDescriptor> UsbConfigDescriptor;

//..............................................................................

class UsbDevice {
public:
	enum {
		DefaultDescriptorBufferSize         = 256,
		DefaultStringDescriptorBufferLength = DefaultDescriptorBufferSize / sizeof(utf16_t),
	};

protected:
	libusb_device* m_device;
	libusb_device_handle* m_openHandle;

public:
	UsbDevice() {
		m_device = NULL;
		m_openHandle = NULL;
	}

	UsbDevice(libusb_device* device);

	~UsbDevice() {
		setDevice(NULL);
	}

	libusb_device*
	getDevice() const {
		return m_device;
	}

	libusb_device_handle*
	getOpenHandle() const {
		return m_openHandle;
	}

	bool
	isOpen() const {
		return m_openHandle != NULL;
	}

	uint8_t
	getBusNumber() const {
		ASSERT(m_device);
		return libusb_get_bus_number(m_device);
	}

	uint8_t
	getDeviceAddress() const {
		ASSERT(m_device);
		return libusb_get_device_address(m_device);
	}

#if (_AXL_IO_USBDEVICE_GETPORTPATH)
	uint8_t
	getPortNumber() const {
		ASSERT(m_device);
		return libusb_get_port_number(m_device);
	}

	size_t
	getPortPath(
		libusb_context* context,
		uint8_t* path,
		size_t maxLength
	) const;

	size_t
	getPortPath(
		uint8_t* path,
		size_t maxLength
	) const {
		return getPortPath(NULL, path, maxLength);
	}
#endif

#if (_AXL_IO_USBDEVICE_GETPORTNUMBERS)
	size_t
	getPortNumbers(
		uint8_t* path,
		size_t maxLength
	) const;
#endif

	libusb_speed
	getDeviceSpeed() const {
		ASSERT(m_device);
		return (libusb_speed)libusb_get_device_speed(m_device);
	}

	size_t
	getMaxPacketSize(uint_t endpointId) const;

	size_t
	getMaxIsoPacketSize(uint_t endpointId) const;

	// open-close

	void
	refDevice() {
		ASSERT(m_device);
		libusb_ref_device(m_device);
	}

	void
	unrefDevice() {
		ASSERT(m_device);
		libusb_unref_device(m_device);
	}

	void
	setDevice(libusb_device* device);

	void
	close();

	bool
	open();

	bool
	open(libusb_device* device) {
		setDevice(device);
		return open();
	}

	bool
	open(
		uint_t vendorId,
		uint_t productId
	) {
		return open(NULL, vendorId, productId);
	}

	bool
	open(
		libusb_context* context,
		uint_t vendorId,
		uint_t productId
	);

	uint_t
	getConfiguration() const;

	bool
	setConfiguration(uint_t configurationId);

	bool
	claimInterface(uint_t ifaceId);

	bool
	releaseInterface(uint_t ifaceId);

	bool
	setInterfaceAltSetting(
		uint_t ifaceId,
		uint_t altSettingId
	);

	bool
	clearHalt(uint_t endpointId);

	bool
	resetDevice();

	bool
	isKernelDriverActive(uint_t ifaceId) const;

	bool
	attachKernelDriver(uint_t ifaceId);

	bool
	detachKernelDriver(uint_t ifaceId);

	bool
	setAutoDetachKernelDriver(bool isAutoDetach);

	// descriptors

	size_t
	getDescriptor(
		void* p,
		size_t size,
		libusb_descriptor_type descriptorType,
		uint_t descriptorId
	) const;

	size_t
	getDescriptor(
		sl::Array<char>* descriptor,
		libusb_descriptor_type descriptorType,
		uint_t descriptorId
	) const;

	sl::Array<char>
	getDescriptor(
		libusb_descriptor_type descriptorType,
		uint_t descriptorId
	) const;

	bool
	getDeviceDescriptor(libusb_device_descriptor* descriptor) const;

	bool
	getConfigDescriptor(
		UsbConfigDescriptor* desc,
		uint_t configurationId
	) const;

	bool
	getActiveConfigDescriptor(UsbConfigDescriptor* desc) const;

	sl::String_utf16
	getStringDescriptor(
		uint_t stringId,
		uint_t langId
	) const;

	size_t
	getStringDescriptor(
		sl::String_utf16* string,
		uint_t stringId,
		uint_t langId
	) const;

	sl::String_utf16
	getStringDescriptor(uint_t stringId) const;

	size_t
	getStringDescriptor(
		sl::String_utf16* string,
		uint_t stringId
	) const;

	// synchronous transfers

	size_t
	controlTransfer(
		uint_t requestType,
		uint_t requestCode,
		uint_t value,
		uint_t index,
		void* p,
		size_t size,
		uint_t timeout = -1
	);

	size_t
	bulkTransfer(
		uint_t endpointId,
		void* p,
		size_t size,
		uint_t timeout = -1
	);

	size_t
	interruptTransfer(
		uint_t endpointId,
		void* p,
		size_t size,
		uint_t timeout = -1
	);

protected:
	template <typename UseLangId>
	size_t
	getStringDescriptorImpl(
		sl::String_utf16* string,
		uint_t stringId,
		uint_t langId
	) const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
UsbDevice::UsbDevice(libusb_device* device) {
	m_device = NULL;
	m_openHandle = NULL;
	setDevice(device);
}

inline
size_t
UsbDevice::getMaxPacketSize(uint_t endpointId) const {
	ASSERT(m_device);

	int result = libusb_get_max_packet_size(m_device, (uchar_t)endpointId);
	return result >= 0 ? result : err::fail<size_t> (-1, UsbError(result));
}

inline
size_t
UsbDevice::getMaxIsoPacketSize(uint_t endpointId) const {
	ASSERT(m_device);
	int result = libusb_get_max_iso_packet_size(m_device, (uchar_t)endpointId);
	return result >= 0 ? result : err::fail<size_t> (-1, UsbError(result));
}

#if (_AXL_IO_USBDEVICE_GETPORTPATH)
inline
size_t
UsbDevice::getPortPath(
	libusb_context* context,
	uint8_t* path,
	size_t maxLength
) const {
	ASSERT(m_device);
	int result = libusb_get_port_path(context, m_device, path, (uint8_t)maxLength);
	return result >= 0 ? result : err::fail<size_t> (-1, UsbError(result));
}
#endif

#if (_AXL_IO_USBDEVICE_GETPORTNUMBERS)
inline
size_t
UsbDevice::getPortNumbers(
	uint8_t* path,
	size_t maxLength
) const {
	ASSERT(m_device);
	int result = libusb_get_port_numbers(m_device, path, (int)maxLength);
	return result >= 0 ? result : err::fail<size_t> (-1, UsbError(result));
}
#endif

inline
size_t
UsbDevice::getDescriptor(
	void* p,
	size_t size,
	libusb_descriptor_type descriptorType,
	uint_t descriptorId
) const {
	ASSERT(m_openHandle);

	int result = libusb_get_descriptor(
		m_openHandle,
		(uint8_t)descriptorType,
		(uint8_t)descriptorId,
		(uint8_t*)p,
		size
	);

	return result >= 0 ? result : err::fail<size_t> (-1, UsbError(result));
}

inline
sl::Array<char>
UsbDevice::getDescriptor(
	libusb_descriptor_type descriptorType,
	uint_t descriptorId
) const {
	sl::Array<char> descriptor;
	getDescriptor(&descriptor, descriptorType, descriptorId);
	return descriptor;
}

inline
bool
UsbDevice::getDeviceDescriptor(libusb_device_descriptor* descriptor) const {
	ASSERT(m_device);

	int result = libusb_get_device_descriptor(m_device, descriptor);
	return result == 0 ? true : err::fail(UsbError(result));
}

inline
sl::String_utf16
UsbDevice::getStringDescriptor(
	uint_t stringId,
	uint_t langId
) const {
	sl::String_utf16 string;
	getStringDescriptor(&string, stringId, langId);
	return string;
}

inline
sl::String_utf16
UsbDevice::getStringDescriptor(uint_t stringId) const {
	sl::String_utf16 string;
	getStringDescriptor(&string, stringId);
	return string;
}

inline
uint_t
UsbDevice::getConfiguration() const {
	ASSERT(m_openHandle);
	int configurationId;
	int result = libusb_get_configuration(m_openHandle, &configurationId);
	return result == 0 ? configurationId : err::fail<uint_t> (-1, UsbError(result));
}

inline
bool
UsbDevice::setConfiguration(uint_t configurationId) {
	ASSERT(m_openHandle);
	int result = libusb_set_configuration(m_openHandle, configurationId);
	return result == 0 ? true : err::fail(UsbError(result));
}

inline
bool
UsbDevice::claimInterface(uint_t ifaceId) {
	ASSERT(m_openHandle);
	int result = libusb_claim_interface(m_openHandle, ifaceId);
	return result == 0 ? true : err::fail(UsbError(result));
}

inline
bool
UsbDevice::releaseInterface(uint_t ifaceId) {
	ASSERT(m_openHandle);
	int result = libusb_release_interface(m_openHandle, ifaceId);
	return result == 0 ? true : err::fail(UsbError(result));
}

inline
bool
UsbDevice::setInterfaceAltSetting(
	uint_t ifaceId,
	uint_t altSettingId
) {
	ASSERT(m_openHandle);
	int result = libusb_set_interface_alt_setting(m_openHandle, ifaceId, altSettingId);
	return result == 0 ? true : err::fail(UsbError(result));
}

inline
bool
UsbDevice::clearHalt(uint_t endpointId) {
	ASSERT(m_openHandle);
	int result = libusb_clear_halt(m_openHandle, (uchar_t)endpointId);
	return result == 0 ? true : err::fail(UsbError(result));
}

inline
bool
UsbDevice::resetDevice() {
	ASSERT(m_openHandle);
	int result = libusb_reset_device(m_openHandle);
	return result == 0 ? true : err::fail(UsbError(result));
}

inline
bool
UsbDevice::isKernelDriverActive(uint_t ifaceId) const {
	ASSERT(m_openHandle);
	int result = libusb_kernel_driver_active(m_openHandle, ifaceId);
	return result == 1;
}

inline
bool
UsbDevice::attachKernelDriver(uint_t ifaceId) {
	ASSERT(m_openHandle);
	int result = libusb_attach_kernel_driver(m_openHandle, ifaceId);
	return result == 0 ? true : err::fail(UsbError(result));
}

inline
bool
UsbDevice::detachKernelDriver(uint_t ifaceId) {
	ASSERT(m_openHandle);
	int result = libusb_detach_kernel_driver(m_openHandle, ifaceId);
	return result == 0 ? true : err::fail(UsbError(result));
}

inline
bool
UsbDevice::setAutoDetachKernelDriver(bool isAutoDetach) {
	ASSERT(m_openHandle);
	int result = libusb_set_auto_detach_kernel_driver(m_openHandle, isAutoDetach);
	return result == 0 ? true : err::fail(UsbError(result));
}

inline
size_t
UsbDevice::controlTransfer(
	uint_t requestType,
	uint_t requestCode,
	uint_t value,
	uint_t index,
	void* p,
	size_t size,
	uint_t timeout
) {
	ASSERT(m_openHandle);

	int result = libusb_control_transfer(
		m_openHandle,
		(uint8_t)requestType,
		(uint8_t)requestCode,
		(uint16_t)value,
		(uint16_t)index,
		(uchar_t*)p,
		(uint16_t)size,
		timeout != -1 ? timeout : 0
	);

	return result >= 0 ? result : err::fail<size_t> (-1, UsbError(result));
}

inline
size_t
UsbDevice::bulkTransfer(
	uint_t endpointId,
	void* p,
	size_t size,
	uint_t timeout
) {
	ASSERT(m_openHandle);

	int actualSize;
	int result = libusb_bulk_transfer(
		m_openHandle,
		(uchar_t)endpointId,
		(uchar_t*)p,
		(int)size,
		&actualSize,
		timeout != -1 ? timeout : 0
	);

	return result == 0 ? actualSize : err::fail<size_t> (-1, UsbError(result));
}

inline
size_t
UsbDevice::interruptTransfer(
	uint_t endpointId,
	void* p,
	size_t size,
	uint_t timeout
) {
	ASSERT(m_openHandle);

	int actualSize;
	int result = libusb_interrupt_transfer(
		m_openHandle,
		(uchar_t)endpointId,
		(uchar_t*)p,
		(int)size,
		&actualSize,
		timeout != -1 ? timeout : 0
	);

	return result == 0 ? actualSize : err::fail<size_t> (-1, UsbError(result));
}

//..............................................................................

class FreeUsbDeviceList {
public:
	void
	operator () (libusb_device** h) {
		libusb_free_device_list(h, true);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbDeviceList: public sl::Handle<libusb_device**, FreeUsbDeviceList> {
public:
	size_t
	enumerateDevices(libusb_context* context = NULL);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
size_t
UsbDeviceList::enumerateDevices(libusb_context* context) {
	close();

	ssize_t result = libusb_get_device_list(context, &m_h);
	return result >= 0 ? result : err::fail<size_t> (-1, UsbError((int)result));
}

//..............................................................................

} // namespace io
} // namespace axl
