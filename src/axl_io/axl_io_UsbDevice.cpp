#include "pch.h"
#include "axl_io_UsbDevice.h"

namespace axl {
namespace io {

//.............................................................................

bool 
UsbContext::open ()
{
	close ();

	int result = libusb_init (&m_h);
	return result == 0 ? true : err::fail (UsbError ((int) result));
}

//.............................................................................

size_t
UsbDeviceList::enumerateDevices (libusb_context* context)
{
	close ();

	ssize_t result = libusb_get_device_list (context, &m_h);
	return result >= 0 ? result : err::fail <size_t> (-1, UsbError ((int) result));
}

//.............................................................................

size_t
UsbDevice::getMaxPacketSize (uint_t endpoint)
{
	ASSERT (m_device);
	
	int result = libusb_get_max_packet_size (m_device, (uchar_t) endpoint);
	return result >= 0 ? result : err::fail <size_t> (-1, UsbError (result));
}

size_t
UsbDevice::getMaxIsoPacketSize (uint_t endpoint)
{
	ASSERT (m_device);
	
	int result = libusb_get_max_iso_packet_size (m_device, (uchar_t) endpoint);
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
	uint_t vendorId,
	uint_t productId
	)
{
	setDevice (NULL);

	m_openHandle = libusb_open_device_with_vid_pid (NULL, (uint16_t) vendorId, (uint16_t) productId);
	return m_openHandle ? true : err::fail (err::Error (err::SystemErrorCode_ObjectNameNotFound));
}

int
UsbDevice::getConfiguration ()
{
	ASSERT (m_openHandle);

	int configuration;
	int result = libusb_get_configuration (m_openHandle, &configuration);
	return result == 0 ? configuration : err::fail <int> (-1, UsbError (result));
}

bool
UsbDevice::setConfiguration (int configuration)
{
	ASSERT (m_openHandle);

	int result = libusb_get_configuration (m_openHandle, &configuration);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::claimInterface (int interface)
{
	ASSERT (m_openHandle);

	int result = libusb_claim_interface (m_openHandle, interface);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::releaseInterface (int interface)
{
	ASSERT (m_openHandle);

	int result = libusb_release_interface (m_openHandle, interface);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::setInterfaceAltSetting (
	int interface,
	int altSetting
	)
{
	ASSERT (m_openHandle);

	int result = libusb_set_interface_alt_setting (m_openHandle, interface, altSetting);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::clearHalt (uint_t endpoint)
{
	ASSERT (m_openHandle);

	int result = libusb_clear_halt (m_openHandle, (uchar_t) endpoint);
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
UsbDevice::isKernelDriverActive (int interface)
{
	ASSERT (m_openHandle);

	int result = libusb_kernel_driver_active (m_openHandle, interface);
	return result == 1;
}

bool
UsbDevice::attachKernelDriver (int interface)
{
	ASSERT (m_openHandle);

	int result = libusb_attach_kernel_driver (m_openHandle, interface);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::detachKernelDriver (int interface)
{
	ASSERT (m_openHandle);

	int result = libusb_detach_kernel_driver (m_openHandle, interface);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::getDesrciptor (
	libusb_descriptor_type type, 
	size_t index,
	rtl::Array <char>* descriptor
	)
{
	ASSERT (m_openHandle);

	size_t size = descriptor->getCount ();
	size = size < Def_BufferSize ? Def_BufferSize : rtl::getMinPower2Ge (size);

	descriptor->setCount (size);

	for (;;)
	{
		int result = libusb_get_descriptor (
			m_openHandle, 
			(uint8_t) type, 
			(uint8_t) index, 
			(uchar_t*) descriptor->a (), 
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
	int configuration,
	libusb_config_descriptor** descriptor
	)
{
	ASSERT (m_device);

	int result = libusb_get_config_descriptor (m_device, configuration, descriptor);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::getActiveConfigDescriptor (libusb_config_descriptor** descriptor)
{
	ASSERT (m_device);

	int result = libusb_get_active_config_descriptor (m_device, descriptor);
	return result == 0 ? true : err::fail (UsbError (result));
}

bool
UsbDevice::getStringDesrciptor (
	size_t index,
	uint_t langId,
	rtl::String* string
	)
{
	ASSERT (m_openHandle);

	char* p = string->getBuffer ();
	size_t length = string->getBufferLength ();

	if (length < Def_BufferSize)
	{
		length = Def_BufferSize;
		p = string->getBuffer (length - 1, false);
	}

	for (;;)
	{
		int result = libusb_get_string_descriptor (
			m_openHandle, 
			(uint8_t) index, 
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
		p = string->getBuffer (length - 1, false);
	}

	string->reduceLength (length);
	return true;
}

bool
UsbDevice::getStringDesrciptor (
	size_t index,
	rtl::String* string
	)
{
	ASSERT (m_openHandle);

	char* p = string->getBuffer ();
	size_t length = string->getBufferLength ();

	if (length < Def_BufferSize)
	{
		length = Def_BufferSize;
		p = string->getBuffer (length - 1, false);
	}

	for (;;)
	{
		int result = libusb_get_string_descriptor_ascii (
			m_openHandle, 
			(uint8_t) index, 
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
		p = string->getBuffer (length - 1, false);
	}

	string->reduceLength (length);
	return true;
}

size_t 
UsbDevice::controlTransfer (
	uint_t requestType,
	uint_t request,
	uint_t value,
	size_t index,
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
	uint_t endpoint,
	void* p,
	size_t size,
	uint_t timeout
	)
{
	ASSERT (m_openHandle);

	int actualSize;
	int result = libusb_bulk_transfer (
		m_openHandle,
		(uchar_t) endpoint, 
		(uchar_t*) p, 
		(int) size, 
		&actualSize,
		timeout != -1 ? timeout : 0
		);

	return result == 0 ? actualSize : err::fail <size_t> (-1, UsbError (result));
}

size_t 
UsbDevice::interruptTransfer (
	uint_t endpoint,
	void* p,
	size_t size,
	uint_t timeout
	)
{
	ASSERT (m_openHandle);

	int actualSize;
	int result = libusb_interrupt_transfer (
		m_openHandle,
		(uchar_t) endpoint, 
		(uchar_t*) p, 
		(int) size, 
		&actualSize,
		timeout != -1 ? timeout : 0
		);

	return result == 0 ? actualSize : err::fail <size_t> (-1, UsbError (result));

}

//.............................................................................

} // namespace io {
} // namespace axl {
