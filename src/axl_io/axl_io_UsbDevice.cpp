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

const char*
getUsbSpeedString (libusb_speed speed)
{
	static const char* stringTable [] =
	{
		"Unknown",                  // LIBUSB_SPEED_UNKNOWN = 0,
		"LowSpeed (1.5 Mbit/s)",    // LIBUSB_SPEED_LOW = 1,
		"FullSpeed (12 Mbit/s)",    // LIBUSB_SPEED_FULL = 2,
		"HighSpeed (480 Mbit/s)",   // LIBUSB_SPEED_HIGH = 3,
		"SuperSpeed (5000 Mbit/s)", // LIBUSB_SPEED_SUPER = 4,
	};

	return (size_t) speed < countof (stringTable) ?
		stringTable [speed] :
		stringTable [LIBUSB_SPEED_UNKNOWN];
}

const char*
getUsbClassCodeString (libusb_class_code classCode)
{
	static const char* stringTable [] =
	{
		"Multi-function",      // LIBUSB_CLASS_PER_INTERFACE = 0,
		"Audio",               // LIBUSB_CLASS_AUDIO = 1,
		"Communication",       // LIBUSB_CLASS_COMM = 2,
		"HID",                 // LIBUSB_CLASS_HID = 3,
		"Unknown",             // 4
		"Physical",            // LIBUSB_CLASS_PHYSICAL = 5,
		"Image",               // LIBUSB_CLASS_IMAGE = 6,
		"Printer",             // LIBUSB_CLASS_PRINTER = 7,
		"Mass Storage",        // LIBUSB_CLASS_MASS_STORAGE = 8,
		"USB HUB",             // LIBUSB_CLASS_HUB = 9,
		"Data",                // LIBUSB_CLASS_DATA = 10,
		"Smart Card",          // LIBUSB_CLASS_SMART_CARD = 0x0b,
		"Unknown",             // 0x0c,
		"Content Security",    // LIBUSB_CLASS_CONTENT_SECURITY = 0x0d,
		"Video",               // LIBUSB_CLASS_VIDEO = 0x0e,
		"Personal Healthcare", // LIBUSB_CLASS_PERSONAL_HEALTHCARE = 0x0f,
	};

	if ((size_t) classCode < countof (stringTable))
		return stringTable [classCode];

	switch (classCode)
	{
	case LIBUSB_CLASS_DIAGNOSTIC_DEVICE:
		return "Diagnostic";

	case LIBUSB_CLASS_WIRELESS:
		return "Wireless";

	case LIBUSB_CLASS_APPLICATION:
		return "Application";

	case LIBUSB_CLASS_VENDOR_SPEC:
		return "Vendor-specific";

	default:
		return "Unknown";
	}
};

const char*
getUsbTransferTypeString (libusb_transfer_type transferType)
{
	static const char* stringTable [] =
	{
		"Control",     // LIBUSB_SPEED_LOW = 1,
		"Isochronous", // LIBUSB_SPEED_FULL = 2,
		"Bulk",        // LIBUSB_SPEED_HIGH = 3,
		"Interrupt",   // LIBUSB_SPEED_SUPER = 4,
		"Bulk stream", // LIBUSB_TRANSFER_TYPE_BULK_STREAM = 4,
	};

	return (size_t) transferType < countof (stringTable) ?
		stringTable [transferType] :
		"Unknown";
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const libusb_interface_descriptor*
findUsbInterfaceDescriptor (
	const libusb_config_descriptor* configDesc,
	uint_t interfaceId,
	uint_t altSettingId
	)
{
	if (interfaceId >= configDesc->bNumInterfaces)
		return NULL;

	const libusb_interface* iface = &configDesc->interface [interfaceId];
	return altSettingId < (uint_t) iface->num_altsetting ?
		&iface->altsetting [altSettingId] :
		NULL;
}

const libusb_endpoint_descriptor*
findUsbEndpointDescriptor (
	const libusb_interface_descriptor* ifaceDesc,
	uint_t endpointId
	)
{
	for (size_t k = 0; k < ifaceDesc->bNumEndpoints; k++)
	{
		const libusb_endpoint_descriptor* endpointDesc = &ifaceDesc->endpoint [k];
		if (endpointDesc->bEndpointAddress == endpointId)
			return endpointDesc;
	}

	return NULL;
}

//..............................................................................

size_t
UsbDeviceList::enumerateDevices (libusb_context* context)
{
	close ();

	ssize_t result = libusb_get_device_list (context, &m_h);
	return result >= 0 ? result : err::fail <size_t> (-1, UsbError ((int) result));
}

//..............................................................................

size_t
UsbDevice::getMaxPacketSize (uint_t endpointId)
{
	ASSERT (m_device);

	int result = libusb_get_max_packet_size (m_device, (uchar_t) endpointId);
	return result >= 0 ? result : err::fail <size_t> (-1, UsbError (result));
}

#if (_AXL_IO_USBDEVICE_PORT)

size_t
UsbDevice::getPortPath (
	uint8_t* path,
	size_t maxLength
	)
{
	ASSERT (m_device);

	int result = libusb_get_port_numbers (m_device, path, maxLength);
	return result >= 0 ? result : err::fail <size_t> (-1, UsbError (result));
}

#endif

size_t
UsbDevice::getMaxIsoPacketSize (uint_t endpointId)
{
	ASSERT (m_device);

	int result = libusb_get_max_iso_packet_size (m_device, (uchar_t) endpointId);
	return result >= 0 ? result : err::fail <size_t> (-1, UsbError (result));
}

void
UsbDevice::setDevice (libusb_device* device)
{
	if (device == m_device)
		return;

	close ();

	if (device)
		libusb_ref_device (device);

	if (m_device)
		libusb_unref_device (m_device);

	m_device = device;
}

void
UsbDevice::takeOver (UsbDevice* srcDevice)
{
	if (m_device != srcDevice->m_device)
	{
		setDevice (NULL);
		m_device = srcDevice->m_device;
	}

	m_openHandle = srcDevice->m_openHandle;

	srcDevice->m_device = NULL;
	srcDevice->m_openHandle = NULL;
}

void
UsbDevice::close ()
{
	if (!m_openHandle)
		return;

	libusb_close (m_openHandle);
	m_openHandle = NULL;
}

bool
UsbDevice::open ()
{
	ASSERT (m_device);

	close ();

	int result = libusb_open (m_device, &m_openHandle);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::open (
	libusb_context* context,
	uint_t vendorId,
	uint_t productId
	)
{
	close ();

	m_openHandle = libusb_open_device_with_vid_pid (context, (uint16_t) vendorId, (uint16_t) productId);
	if (!m_openHandle)
		return err::fail (err::Error (err::SystemErrorCode_ObjectNameNotFound));

	m_device = libusb_get_device (m_openHandle);
	ASSERT (m_device);

	libusb_ref_device (m_device);
	return true;
}

uint_t
UsbDevice::getConfiguration ()
{
	ASSERT (m_openHandle);

	int configurationId;
	int result = libusb_get_configuration (m_openHandle, &configurationId);
	return result == 0 ? configurationId : err::fail <uint_t> (-1, UsbError (result));
}

bool
UsbDevice::setConfiguration (uint_t configurationId)
{
	ASSERT (m_openHandle);

	int result = libusb_set_configuration (m_openHandle, configurationId);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::claimInterface (uint_t ifaceId)
{
	ASSERT (m_openHandle);

	int result = libusb_claim_interface (m_openHandle, ifaceId);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::releaseInterface (uint_t ifaceId)
{
	ASSERT (m_openHandle);

	int result = libusb_release_interface (m_openHandle, ifaceId);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::setInterfaceAltSetting (
	uint_t ifaceId,
	uint_t altSettingId
	)
{
	ASSERT (m_openHandle);

	int result = libusb_set_interface_alt_setting (m_openHandle, ifaceId, altSettingId);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::clearHalt (uint_t endpointId)
{
	ASSERT (m_openHandle);

	int result = libusb_clear_halt (m_openHandle, (uchar_t) endpointId);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::resetDevice ()
{
	ASSERT (m_openHandle);

	int result = libusb_reset_device (m_openHandle);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::isKernelDriverActive (uint_t ifaceId)
{
	ASSERT (m_openHandle);

	int result = libusb_kernel_driver_active (m_openHandle, ifaceId);
	return result == 1;
}

bool
UsbDevice::attachKernelDriver (uint_t ifaceId)
{
	ASSERT (m_openHandle);

	int result = libusb_attach_kernel_driver (m_openHandle, ifaceId);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::detachKernelDriver (uint_t ifaceId)
{
	ASSERT (m_openHandle);

	int result = libusb_detach_kernel_driver (m_openHandle, ifaceId);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::setAutoDetachKernelDriver (bool isAutoDetach)
{
	ASSERT (m_openHandle);

	int result = libusb_set_auto_detach_kernel_driver (m_openHandle, isAutoDetach);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::getDescriptor (
	libusb_descriptor_type descriptorType,
	uint_t descriptorId,
	sl::Array <char>* descriptor
	)
{
	ASSERT (m_openHandle);

	size_t size = descriptor->getCount ();
	size = size < Def_BufferSize ? Def_BufferSize : sl::getAllocSize (size);

	descriptor->setCount (size);

	for (;;)
	{
		int result = libusb_get_descriptor (
			m_openHandle,
			(uint8_t) descriptorType,
			(uint8_t) descriptorId,
			(uchar_t*) descriptor->p (),
			size
			);

		if (result >= 0)
		{
			size = result;
			break;
		}

		if (result != LIBUSB_ERROR_OVERFLOW)
			return err::fail (UsbError (result));

		size *= 2;
		descriptor->setCount (size);
	}

	descriptor->setCount (size);
	return true;
}

bool
UsbDevice::getDeviceDescriptor (libusb_device_descriptor* descriptor)
{
	ASSERT (m_device);

	int result = libusb_get_device_descriptor (m_device, descriptor);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::getConfigDescriptor (
	uint_t configurationId,
	UsbConfigDescriptor* desc
	)
{
	ASSERT (m_device);

	libusb_config_descriptor* buffer;
	int result = libusb_get_config_descriptor (m_device, configurationId, &buffer);
	if (result != 0)
		return err::fail (UsbError (result));

	desc->attach (buffer);
	return true;
}

bool
UsbDevice::getActiveConfigDescriptor (UsbConfigDescriptor* desc)
{
	ASSERT (m_device);

	libusb_config_descriptor* buffer;
	int result = libusb_get_active_config_descriptor (m_device, &buffer);
	if (result != 0)
		return err::fail (UsbError (result));

	desc->attach (buffer);
	return true;
}

bool
UsbDevice::getStringDesrciptor (
	uint_t stringId,
	uint_t langId,
	sl::String* string
	)
{
	ASSERT (m_openHandle);

	size_t length;
	char* p = string->getBuffer (&length);

	if (length < Def_BufferSize)
	{
		length = Def_BufferSize;
		p = string->createBuffer (length - 1);
	}

	for (;;)
	{
		int result = libusb_get_string_descriptor (
			m_openHandle,
			(uint8_t) stringId,
			(uint16_t) langId,
			(uchar_t*) p,
			length
			);

		if (result >= 0)
		{
			length = result;
			break;
		}

		if (result != LIBUSB_ERROR_OVERFLOW)
			return err::fail (UsbError (result));

		length *= 2;
		p = string->createBuffer (length - 1);
	}

	string->setReducedLength (length);
	return true;
}

bool
UsbDevice::getStringDesrciptor (
	uint_t stringId,
	sl::String* string
	)
{
	ASSERT (m_openHandle);

	size_t length;
	char* p = string->getBuffer (&length);

	if (length < Def_BufferSize)
	{
		length = Def_BufferSize;
		p = string->createBuffer (length - 1);
	}

	for (;;)
	{
		int result = libusb_get_string_descriptor_ascii (
			m_openHandle,
			(uint8_t) stringId,
			(uchar_t*) p,
			length
			);

		if (result >= 0)
		{
			length = result;
			break;
		}

		if (result != LIBUSB_ERROR_OVERFLOW)
			return err::fail (UsbError (result));

		length *= 2;
		p = string->createBuffer (length - 1);
	}

	string->setReducedLength (length);
	return true;
}

size_t
UsbDevice::controlTransfer (
	uint_t requestType,
	uint_t request,
	uint_t value,
	uint_t index,
	void* p,
	size_t size,
	uint_t timeout
	)
{
	ASSERT (m_openHandle);

	int result = libusb_control_transfer (
		m_openHandle,
		(uint8_t) requestType,
		(uint8_t) request,
		(uint16_t) value,
		(uint16_t) index,
		(uchar_t*) p,
		(uint16_t) size,
		timeout != -1 ? timeout : 0
		);

	return result >= 0 ? result : err::fail <size_t> (-1, UsbError (result));
}

size_t
UsbDevice::bulkTransfer (
	uint_t endpointId,
	void* p,
	size_t size,
	uint_t timeout
	)
{
	ASSERT (m_openHandle);

	int actualSize;
	int result = libusb_bulk_transfer (
		m_openHandle,
		(uchar_t) endpointId,
		(uchar_t*) p,
		(int) size,
		&actualSize,
		timeout != -1 ? timeout : 0
		);

	return result == 0 ? actualSize : err::fail <size_t> (-1, UsbError (result));
}

size_t
UsbDevice::interruptTransfer (
	uint_t endpointId,
	void* p,
	size_t size,
	uint_t timeout
	)
{
	ASSERT (m_openHandle);

	int actualSize;
	int result = libusb_interrupt_transfer (
		m_openHandle,
		(uchar_t) endpointId,
		(uchar_t*) p,
		(int) size,
		&actualSize,
		timeout != -1 ? timeout : 0
		);

	return result == 0 ? actualSize : err::fail <size_t> (-1, UsbError (result));

}

//..............................................................................

} // namespace io
} // namespace axl
