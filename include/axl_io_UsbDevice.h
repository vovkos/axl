// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_USBDEVICE_H

#include "axl_io_UsbError.h"
#include "axl_sl_Handle.h"
#include "axl_sl_Array.h"

namespace axl {
namespace io {

//.............................................................................

const char*
getUsbSpeedString (libusb_speed speed);

const char*
getUsbClassCodeString (libusb_class_code classCode);

const char*
getUsbTransferTypeString (libusb_transfer_type transferType);

//.............................................................................

class CloseUsbContext
{
public:
	void
	operator () (libusb_context* h)
	{
		libusb_exit (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbContext: public sl::Handle <libusb_context*, CloseUsbContext>
{
public:
	UsbContext ()
	{
		open ();
	}

	bool 
	open ();
};

//.............................................................................

class FreeUsbDeviceList
{
public:
	void
	operator () (libusb_device** h)
	{
		libusb_free_device_list (h, true);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbDeviceList: public sl::Handle <libusb_device**, FreeUsbDeviceList>
{
public:
	size_t
	enumerateDevices (libusb_context* context);
};

//.............................................................................

class FreeUsbConfigDescriptor
{
public:
	void
	operator () (libusb_config_descriptor* h)
	{
		libusb_free_config_descriptor (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef sl::Handle <libusb_config_descriptor*, FreeUsbConfigDescriptor> UsbConfigDescriptor;

//.............................................................................

class UsbDevice
{
public:
	enum Def
	{
		Def_BufferSize = 256,
	};

protected:
	libusb_device* m_device;
	libusb_device_handle* m_openHandle;

public:
	UsbDevice ()
	{
		m_device = NULL;
		m_openHandle = NULL;
	}

	~UsbDevice ()
	{
		setDevice (NULL);
	}

	libusb_device* 
	getDevice ()
	{
		return m_device;
	}

	libusb_device_handle* 
	getOpenHandle ()
	{
		return m_openHandle;
	}
	
	bool
	isOpen ()
	{
		return m_openHandle != NULL;
	}

	uint8_t
	getDeviceAddress ()
	{
		ASSERT (m_device);
		return libusb_get_device_address (m_device);
	}

	uint8_t 
	getBusNumber ()
	{
		ASSERT (m_device);
		return libusb_get_bus_number (m_device);
	}

	uint8_t
	getPortNumber ()
	{
		ASSERT (m_device);
		return libusb_get_port_number (m_device);
	}

	size_t
	getPortPath (
		uint8_t* path,
		size_t maxLength
		);

	libusb_speed 
	getDeviceSpeed ()
	{
		ASSERT (m_device);
		return (libusb_speed) libusb_get_device_speed (m_device);
	}

	size_t
	getMaxPacketSize (uint_t endpointId);

	size_t
	getMaxIsoPacketSize (uint_t endpointId);
	
	// open-close

	void
	refDevice ()
	{
		ASSERT (m_device);
		libusb_ref_device (m_device);
	}

	void
	unrefDevice ()
	{
		ASSERT (m_device);
		libusb_unref_device (m_device);
	}

	void
	setDevice (libusb_device* device);

	void
	close ();

	bool
	open ();

	bool
	open (libusb_device* device)
	{
		setDevice (device);
		return open ();
	}

	bool
	open (
		uint_t vendorId,
		uint_t productId
		);

	uint_t
	getConfiguration ();

	bool
	setConfiguration (uint_t configurationId);

	bool
	claimInterface (uint_t ifaceId);

	bool
	releaseInterface (uint_t ifaceId);

	bool
	setInterfaceAltSetting (
		uint_t ifaceId,
		uint_t altSettingId
		);

	bool
	clearHalt (uint_t endpointId);

	bool
	resetDevice ();

	bool
	isKernelDriverActive (uint_t ifaceId);

	bool
	attachKernelDriver (uint_t ifaceId);

	bool
	detachKernelDriver (uint_t ifaceId);

	// descriptors

	sl::Array <char>
	getDescriptor (
		libusb_descriptor_type descriptorType, 
		uint_t descriptorId
		)
	{
		sl::Array <char> descriptor;
		getDescriptor (descriptorType, descriptorId, &descriptor);
		return descriptor;
	}
	
	bool
	getDescriptor (
		libusb_descriptor_type descriptorType, 
		uint_t descriptorId,
		sl::Array <char>* descriptor
		);

	bool
	getDeviceDescriptor (libusb_device_descriptor* descriptor);

	bool
	getConfigDescriptor (
		uint_t configurationId,
		UsbConfigDescriptor* desc
		);

	bool
	getActiveConfigDescriptor (UsbConfigDescriptor* desc);

	sl::String 
	getStringDesrciptor (
		uint_t stringId,
		uint_t langId
		)
	{
		sl::String string;
		getStringDesrciptor (stringId, langId, &string);
		return string;
	}

	bool
	getStringDesrciptor (
		uint_t stringId,
		uint_t langId,
		sl::String* string
		);

	sl::String 
	getStringDesrciptor (uint_t stringId)
	{
		sl::String string;
		getStringDesrciptor (stringId, &string);
		return string;
	}

	bool
	getStringDesrciptor (
		uint_t stringId,
		sl::String* string
		);

	// synchronous transfers

	size_t 
	controlTransfer (
		uint_t requestType,
		uint_t requestId,
		uint_t value,
		uint_t index,
		void* p,
		size_t size,
		uint_t timeout = -1
		);

	size_t 
	bulkTransfer (
		uint_t endpointId,
		void* p,
		size_t size,
		uint_t timeout = -1
		);

	size_t 
	interruptTransfer (
		uint_t endpointId,
		void* p,
		size_t size,
		uint_t timeout = -1
		);
};

//.............................................................................

} // namespace io
} // namespace axl
