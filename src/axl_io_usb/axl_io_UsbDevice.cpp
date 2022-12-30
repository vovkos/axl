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

#include "pch.h"
#include "axl_io_UsbDevice.h"
#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//..............................................................................

const libusb_interface_descriptor*
findUsbInterfaceDescriptor(
	const libusb_config_descriptor* configDesc,
	uint_t interfaceId,
	uint_t altSettingId
) {
	if (interfaceId >= configDesc->bNumInterfaces)
		return NULL;

	const libusb_interface* iface = &configDesc->interface[interfaceId];
	return altSettingId < (uint_t)iface->num_altsetting ?
		&iface->altsetting[altSettingId] :
		NULL;
}

const libusb_endpoint_descriptor*
findUsbEndpointDescriptor(
	const libusb_interface_descriptor* ifaceDesc,
	uint_t endpointId
) {
	for (size_t k = 0; k < ifaceDesc->bNumEndpoints; k++) {
		const libusb_endpoint_descriptor* endpointDesc = &ifaceDesc->endpoint[k];
		if (endpointDesc->bEndpointAddress == endpointId)
			return endpointDesc;
	}

	return NULL;
}

//..............................................................................

void
UsbDevice::setDevice(libusb_device* device) {
	if (device == m_device)
		return;

	close();

	if (device)
		libusb_ref_device(device);

	if (m_device)
		libusb_unref_device(m_device);

	m_device = device;
}

void
UsbDevice::close() {
	if (!m_openHandle)
		return;

	libusb_close(m_openHandle);
	m_openHandle = NULL;
}

bool
UsbDevice::open() {
	ASSERT(m_device);

	close();

	int result = libusb_open(m_device, &m_openHandle);
	return result == 0 ? true : err::fail(UsbError(result));
}

bool
UsbDevice::open(
	libusb_context* context,
	uint_t vendorId,
	uint_t productId
) {
	close();

	m_openHandle = libusb_open_device_with_vid_pid(context, (uint16_t)vendorId, (uint16_t)productId);
	if (!m_openHandle)
		return err::fail(err::Error(err::SystemErrorCode_ObjectNameNotFound));

	m_device = libusb_get_device(m_openHandle);
	ASSERT(m_device);

	libusb_ref_device(m_device);
	return true;
}

size_t
UsbDevice::getDescriptor(
	sl::Array<char>* descriptor,
	libusb_descriptor_type descriptorType,
	uint_t descriptorId
) const {
	ASSERT(m_openHandle);

	size_t size = descriptor->getCount();
	if (!size) {
		size = DefaultDescriptorBufferSize;
		descriptor->setCount(DefaultDescriptorBufferSize);
	}

	for (;;) {
		int result = libusb_get_descriptor(
			m_openHandle,
			(uint8_t)descriptorType,
			(uint8_t)descriptorId,
			(uchar_t*)descriptor->p(),
			size
		);

		if (result >= 0) {
			size = result;
			break;
		}

		if (result != LIBUSB_ERROR_OVERFLOW) {
			descriptor->clear();
			return err::fail(UsbError(result));
		}

		size = size < DefaultDescriptorBufferSize ? DefaultDescriptorBufferSize : size * 2;
		descriptor->setCount(size);
	}

	descriptor->setCount(size);
	return true;
}

bool
UsbDevice::getConfigDescriptor(
	UsbConfigDescriptor* desc,
	uint_t configurationId
) const {
	ASSERT(m_device);

	libusb_config_descriptor* buffer;
	int result = libusb_get_config_descriptor(m_device, configurationId, &buffer);
	if (result != 0)
		return err::fail(UsbError(result));

	desc->attach(buffer);
	return true;
}

bool
UsbDevice::getActiveConfigDescriptor(UsbConfigDescriptor* desc) const {
	ASSERT(m_device);

	libusb_config_descriptor* buffer;
	int result = libusb_get_active_config_descriptor(m_device, &buffer);
	if (result != 0)
		return err::fail(UsbError(result));

	desc->attach(buffer);
	return true;
}

struct UsbDescriptorHdr {
	uint8_t m_size;
	uint8_t m_type;
};

size_t
UsbDevice::getStringDescriptor(
	sl::String_utf16* string,
	uint_t stringId,
	uint_t langId
) const {
	ASSERT(m_openHandle);

	size_t length;
	utf16_t* p = string->getBuffer(&length);

	if (!length) {
		length = DefaultStringDescriptorBufferLength;
		p = string->createBuffer(DefaultStringDescriptorBufferLength);
	}

	for (;;) {
		int result = libusb_get_string_descriptor(
			m_openHandle,
			(uint8_t)stringId,
			(uint16_t)langId,
			(uchar_t*)p,
			length * sizeof(utf16_t)
		);

		if (result >= 0) {
			UsbDescriptorHdr* hdr = (UsbDescriptorHdr*)p;
			if (hdr->m_type != LIBUSB_DT_STRING)
				return err::fail<size_t>(-1, "unexpected USB descriptor type");

			if (hdr->m_size / sizeof(utf16_t) > length)
				return err::fail<size_t>(-1, "unexpected USB descriptor size");

			string->overrideLength(hdr->m_size / sizeof(utf16_t));
			string->remove(0);
			return string->getLength();
		}

		if (result != LIBUSB_ERROR_OVERFLOW)
			return err::fail<size_t>(-1, UsbError(result));

		length = length < DefaultStringDescriptorBufferLength ?
			DefaultStringDescriptorBufferLength :
			length * 2;

		p = string->createBuffer(length);
		if (!p)
			return -1;
	}
}

size_t
UsbDevice::getStringDescriptorAscii(
	sl::String* string,
	uint_t stringId
) const {
		ASSERT(m_openHandle);

		size_t length;
		char* p = string->getBuffer(&length);

		if (!length) {
			length = DefaultStringDescriptorBufferLength;
			p = string->createBuffer(DefaultStringDescriptorBufferLength);
		}

		for (;;) {
			int result = libusb_get_string_descriptor_ascii(
				m_openHandle,
				(uint8_t)stringId,
				(uchar_t*)p,
				length
			);

			if (result >= 0) {
				string->overrideLength(result);
				return result;
			}

			if (result != LIBUSB_ERROR_OVERFLOW)
				return err::fail<size_t>(-1, UsbError(result));

			length = length < DefaultStringDescriptorBufferLength ?
				DefaultStringDescriptorBufferLength :
				length * 2;

			p = string->createBuffer(length);
		}
}

//..............................................................................

} // namespace io
} // namespace axl
